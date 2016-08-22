/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// PMR Workspaces widget
//==============================================================================

#include "coreguiutils.h"
#include "pmrrepository.h"
#include "pmrworkspace.h"
#include "pmrworkspacefilenode.h"
#include "pmrworkspacescommit.h"
#include "pmrworkspacesmanager.h"
#include "pmrworkspaceswidget.h"

//==============================================================================

#include <QApplication>
#include <QContextMenuEvent>
#include <QDesktopServices>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHeaderView>
#include <QHelpEvent>
#include <QMenu>
#include <QMutableMapIterator>
#include <QProcess>
#include <QSettings>
#include <QStandardPaths>
#include <QTimer>
#include <QToolTip>
#include <QUrl>
#include <QWebElement>
#include <QWebFrame>

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspaces {

//==============================================================================

#define ICON_ABOUT        ":/oxygen/actions/help-about.png"
#define ICON_CLONE        ":/oxygen/places/folder-downloads.png"
#define ICON_COMMIT       ":/oxygen/actions/view-task.png"
#define ICON_FOLDER       ":/oxygen/places/folder.png"
#define ICON_FOLDER_OPEN  ":/oxygen/places/folder-open.png"
#define ICON_OWNED        ":/oxygen/places/folder-favorites.png"
//#define ICON_PULL         ":/oxygen/actions/arrow-down-double.png"
//#define ICON_PUSH         ":/oxygen/actions/arrow-up-double.png"
#define ICON_REFRESH      ":/oxygen/actions/view-refresh.png"
#define ICON_STAGE        ":/oxygen/actions/dialog-ok-apply.png"
#define ICON_SYNC_PULL    ":/PMRWorkspaces/icons/sync-pull.png"
#define ICON_SYNC_PUSH    ":/PMRWorkspaces/icons/sync-push.png"
#define ICON_SYNCHRONISE  ":/PMRWorkspaces/icons/synchronise.png"
#define ICON_UNSTAGE      ":/oxygen/actions/dialog-cancel.png"

//==============================================================================

PmrWorkspacesWidget::PmrWorkspacesWidget(PMRSupport::PmrRepository *pPmrRepository, QWidget *pParent) :
    OpenCOR::WebViewerWidget::WebViewerWidget(pParent),
    Core::CommonWidget(),
    mPmrRepository(pPmrRepository),
    mWorkspacesManager(OpenCOR::PMRSupport::PmrWorkspacesManager::instance()),
    mWorkspaceFolders(QMap<QString, QString>()),
    mWorkspaceUrls(QMap<QString, QPair<QString, bool> >()),
    mCurrentWorkspaceUrl(QString()),
    mExpandedItems(QSet<QString>()),
    mSelectedItem(QString()),
    mRowAnchor(0),
    mAnchors(QMap<QString, int>())
{
    // Prevent objects from being dropped on us

    setAcceptDrops(false);

    // We track the mouse ourselves

    setMouseTracking(true);

    // Connections to handle responses from PMR

    connect(mPmrRepository, SIGNAL(workspaceCloned(PMRSupport::PmrWorkspace *)),
            this, SLOT(workspaceCloned(PMRSupport::PmrWorkspace *)));
    connect(mPmrRepository, SIGNAL(workspaceCreated(QString)),
            this, SLOT(workspaceCreated(QString)));
    connect(mPmrRepository, SIGNAL(workspaceSynchronised(PMRSupport::PmrWorkspace *)),
            this, SLOT(workspaceSynchronised(PMRSupport::PmrWorkspace *)));

    // Handle workspace cloned signals from other plugins

    connect(OpenCOR::PMRSupport::PmrWorkspacesManager::instance(), SIGNAL(workspaceCloned(PMRSupport::PmrWorkspace *)),
            this, SLOT(workspaceCloned(PMRSupport::PmrWorkspace *)));

    // Retrieve the HTML template

    QString fileContents;
    Core::readFileContentsFromFile(":/PMRWorkspaces/output.html", fileContents);
    mTemplate = fileContents.arg( // clone, commit, folder, open, star, stage, pull, push, sync, unstage
                                 Core::iconDataUri(ICON_CLONE, 16, 16),
                                 Core::iconDataUri(ICON_COMMIT, 16, 16),
                                 Core::iconDataUri(ICON_FOLDER, 16, 16),
                                 Core::iconDataUri(ICON_FOLDER_OPEN, 16, 16),
                                 Core::iconDataUri(ICON_OWNED, 16, 16),
                                 Core::iconDataUri(ICON_STAGE, 16, 16),
                                 Core::iconDataUri(ICON_SYNC_PULL, 16, 16),
                                 Core::iconDataUri(ICON_SYNC_PUSH, 16, 16),
                                 Core::iconDataUri(ICON_SYNCHRONISE, 16, 16))
                                 .arg(Core::iconDataUri(ICON_UNSTAGE, 16, 16), "%1");

    // Create our timer for refreshing the current workspace

    mTimer = new QTimer(this);

    // A connection to handle the timing out of our timer

    connect(mTimer, SIGNAL(timeout()),
            this, SLOT(refreshCurrentWorkspace()));

    // Keep track of when OpenCOR gets/loses the focus
    // Note: the focusWindowChanged() signal comes from QGuiApplication, so we
    //       need to check whether we are running the console version of OpenCOR
    //       or its GUI version...

    if (dynamic_cast<QGuiApplication *>(QCoreApplication::instance())) {
        connect(qApp, SIGNAL(focusWindowChanged(QWindow *)),
                this, SLOT(focusWindowChanged()));
    }
}

//==============================================================================

PmrWorkspacesWidget::~PmrWorkspacesWidget()
{
    // Delete some internal objects

    delete mTimer;
}

//==============================================================================

QSize PmrWorkspacesWidget::sizeHint() const
{
    // Suggest a default size for our PMR workspaces widget
    // Note: this is critical if we want a docked widget, with a PMR workspaces
    //        widget on it, to have a decent size when docked to the main window.

    return defaultSize(0.15);
}

//==============================================================================

static const auto SettingsWorkspaces    = QStringLiteral("Workspaces");
static const auto SettingsFolders       = QStringLiteral("Folders");
static const auto SettingsExpandedItems = QStringLiteral("ExpandedItems");
static const auto SettingsSelectedItem  = QStringLiteral("SelectedItem");
static const auto SettingsCurrentWorkspace = QStringLiteral("CurrentWorkspace");

//==============================================================================

void PmrWorkspacesWidget::loadSettings(QSettings *pSettings)
{
    pSettings->beginGroup(SettingsWorkspaces);

        // Retrieve the current workspace url, if any

        mCurrentWorkspaceUrl = pSettings->value(SettingsCurrentWorkspace).toString();

        // Retrieve the names of folders containing cloned workspaces

        QStringList folders = pSettings->value(SettingsFolders).toStringList();
        foreach (QString folder, folders) {
            if (!folder.isEmpty()) {
                auto url = addWorkspaceFolder(folder);

                // Ensure only the current workspace is expanded
                if (url == mCurrentWorkspaceUrl)
                    mExpandedItems.insert(mCurrentWorkspaceUrl);
                else if (mExpandedItems.contains(url))
                    mExpandedItems.remove(url);
            }
        }

        // Retrieve the names of expanded workspaces and folders

        mExpandedItems = pSettings->value(SettingsExpandedItems).toStringList().toSet();

        // Retrieve the currently selected item, if any

        mSelectedItem = pSettings->value(SettingsSelectedItem).toString();

    pSettings->endGroup();
}

//==============================================================================

void PmrWorkspacesWidget::saveSettings(QSettings *pSettings) const
{
    pSettings->remove(SettingsWorkspaces);
    pSettings->beginGroup(SettingsWorkspaces);

        // Keep track of the names of folders containing cloned workspaces

        pSettings->setValue(SettingsFolders, QVariant(mWorkspaceFolders.keys()));

        // Keep track of the names of expanded workspaces and folders

        pSettings->setValue(SettingsExpandedItems, QVariant(mExpandedItems.toList()));

        // Keep track of the currently selected item

        pSettings->setValue(SettingsSelectedItem, mSelectedItem);

        // Keep track of the current workspace url

        pSettings->setValue(SettingsCurrentWorkspace, mCurrentWorkspaceUrl);

    pSettings->endGroup();
}

//==============================================================================
//==============================================================================

void PmrWorkspacesWidget::addWorkspace(PMRSupport::PmrWorkspace *pWorkspace,
                                       const bool &pOwned)
{
    const QString &folder = pWorkspace->path();
    const QString &url = pWorkspace->url();

    if (!mWorkspaceFolders.contains(folder)) {
        if (mWorkspaceUrls.contains(url)) {
            duplicateCloneMessage(url, mWorkspaceUrls.value(url).first, folder);
        }
        else {
            mWorkspaceFolders.insert(folder, url);
            mWorkspaceUrls.insert(url, QPair<QString, bool>(folder, pOwned));
            mWorkspacesManager->addWorkspace(pWorkspace);
        }
    }
}

//==============================================================================

void PmrWorkspacesWidget::duplicateCloneMessage(const QString &pUrl,
                                                const QString &pPath1, const QString &pPath2)
{
    emit warning(QString("Workspace '%1' is cloned into both '%2' and '%3'").arg(pUrl, pPath1, pPath2));
}

//==============================================================================

const QString PmrWorkspacesWidget::addWorkspaceFolder(const QString &pFolder)
{
    if (!mWorkspaceFolders.contains(pFolder)) {
        // Get the workspace url (= remote.origin.url)

        QString url = PMRSupport::PmrWorkspace::getUrlFromFolder(pFolder);
        if (!url.isEmpty()) {
            if (mWorkspaceUrls.contains(url)) {
                duplicateCloneMessage(url, mWorkspaceUrls.value(url).first, pFolder);
            }
            else {
                mWorkspaceFolders.insert(pFolder, url);
                mWorkspaceUrls.insert(url, QPair<QString, bool>(pFolder, false));
            }
        }
        return url;
    }
    else {
        return mWorkspaceFolders.value(pFolder);
    }
}

//==============================================================================
//==============================================================================

void PmrWorkspacesWidget::scanDefaultWorkspaceDirectory(void)
{
    QDir workspaceDirectory = QDir(PMRSupport::PmrWorkspace::WorkspacesDirectory());
    foreach(QFileInfo info, workspaceDirectory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name)) {
        if (info.isDir()) addWorkspaceFolder(info.absoluteFilePath());
    }
}

//==============================================================================

const QString PmrWorkspacesWidget::actionHtml(const QList<QPair<QString, QString> > &pActions)
{
    QPair<QString, QString> action;
    QStringList actions;
    foreach (action, pActions)
        actions << QString("<a href=\"%1|%2\"><img class=\"button noHover %1\" /></a>").arg(action.first, action.second);

    return actions.join("");
}

//==============================================================================

QString PmrWorkspacesWidget::containerHtml(const QString &pClass, const QString &pIcon,
                                           const QString &pId, const QString &pName,
                                           const QString &pStatus,
                                           const QList<QPair<QString, QString> > &pActionList)
{
    static const QString html = "<tr class=\"%1\" id=\"%2\">\n"
                                "  <td class=\"icon\"><a id=\"a_%7\">%3</a></td>\n"
                                "  <td class=\"name\">%4</td>\n"
                                "  <td class=\"status\">%5</td>\n"
                                "  <td class=\"action\">%6</td>\n"
                                "</tr>\n";

    const QString iconHtml = QString("<img class=\"%1\" />").arg(pIcon);
    const QString rowClass = pClass + ((pId == mSelectedItem || pId == mCurrentWorkspaceUrl) ? " selected" : "");

    // Use an anchor element to allow us to set the scroll position at a row

    mRowAnchor += 1;
    mAnchors.insert(pId, mRowAnchor);

    return html.arg(rowClass, pId, iconHtml, pName, pStatus, actionHtml(pActionList)).arg(mRowAnchor);
}

//==============================================================================

const QStringList PmrWorkspacesWidget::fileStatusActionHtml(const QString &pPath,
                                                            const QPair<QChar, QChar> &pGitStatus)
{
    static const QString statusHtml = "<span class=\"istatus\">%1</span><span class=\"wstatus\">%2</span>";

    auto actionList = QList<QPair<QString, QString> >();
    if      (pGitStatus.second != ' ')
        actionList << QPair<QString, QString>("stage", pPath);
    else if (pGitStatus.first != ' ')
        actionList << QPair<QString, QString>("unstage", pPath);

    return QStringList() << statusHtml.arg(pGitStatus.first).arg(pGitStatus.second)
                         << actionHtml(actionList);
}

//==============================================================================

const QStringList PmrWorkspacesWidget::fileStatusActionHtml(const PMRSupport::PmrWorkspace *pWorkspace,
                                                            const QString &pPath)
{
    return fileStatusActionHtml(pPath, pWorkspace->gitFileStatus(pPath));
}

//==============================================================================

const QStringList PmrWorkspacesWidget::fileStatusActionHtml(const PMRSupport::PmrWorkspaceFileNode *pFileNode)
{
    return fileStatusActionHtml(pFileNode->fullName(), pFileNode->status());
}

//==============================================================================

QString PmrWorkspacesWidget::fileHtml(const PMRSupport::PmrWorkspaceFileNode *pFileNode)
{
    static const QString html = "<tr class=\"file%1\" id=\"%2\">\n"
                                "  <td colspan=\"2\" class=\"name\"><a id=\"a_%6\" href=\"%2\">%3</a></td>\n"
                                "  <td class=\"status\">%4</td>\n"
                                "  <td class=\"action\">%5</td>\n"
                                "</tr>\n";
    auto path = pFileNode->fullName();

    mRow += 1;
    QString rowClass = (mRow % 2) ? "" : " even";
    if (path == mSelectedItem) rowClass += " selected";

    auto statusActionHtml = fileStatusActionHtml(pFileNode);

    // Use an anchor element to allow us to set the scroll position at a row

    mRowAnchor += 1;
    mAnchors.insert(path, mRowAnchor);

    return html.arg(rowClass, path, pFileNode->shortName(), statusActionHtml[0], statusActionHtml[1]).arg(mRowAnchor);
}

//==============================================================================

QString PmrWorkspacesWidget::emptyContentsHtml(void)
{
    static const QString html = "<tr></tr>\n";

    return html;
}

//==============================================================================

QString PmrWorkspacesWidget::contentsHtml(const PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                          const bool &pHidden)
{
    static const QString html = "<tr class=\"contents%1\">\n"
                                "  <td></td>\n"
                                "  <td colspan=\"3\">\n"
                                "    <table>\n"
                                "      <tr class=\"spacing\">\n"
                                "        <td class=\"icon\"></td>\n"
                                "        <td class=\"name\"></td>\n"
                                "        <td class=\"status\"></td>\n"
                                "        <td class=\"action\"></td>\n"
                                "      </tr>\n"
                                "      %2\n"
                                "    </table>\n"
                                "  </td>\n"
                                "</tr>\n";

    if (pFileNode) {
        QStringList itemHtml;
        foreach(PMRSupport::PmrWorkspaceFileNode *fileNode, pFileNode->children()) {
            if (fileNode->hasChildren() ) itemHtml << folderHtml(fileNode);
            else                          itemHtml << fileHtml(fileNode);
        }
        return html.arg(pHidden ? " hidden" : "", itemHtml.join("\n"));
    }
    else {
        return emptyContentsHtml();
    }
}

//==============================================================================


QStringList PmrWorkspacesWidget::workspaceHtml(const PMRSupport::PmrWorkspace *pWorkspace)
{
    const QString &url = pWorkspace->url();
    const QString &name = pWorkspace->name();
    const QString &path = pWorkspace->path();

    const QString icon = pWorkspace->isOwned() ? "star" : "folder";
    auto status = QString("");
    auto actionList = QList<QPair<QString, QString> >();

    auto remoteStatus = pWorkspace->gitRemoteStatus();

    if (path.isEmpty()) {
        actionList << QPair<QString, QString>("clone", url);
    }
    else {

        // Indicate whether there are unstaged files

        if (remoteStatus & PMRSupport::PmrWorkspace::StatusUnstaged)
            status = "?";   // Have an icon (but without an action)??

        // Indicate whether files are staged for commit

        if (remoteStatus & PMRSupport::PmrWorkspace::StatusCommit)
            actionList << QPair<QString, QString>("commit", url);

        // Show synchronisation button, with different styles to
        // indicate exactly what it will do

        if (pWorkspace->isOwned()) {
            if (remoteStatus & PMRSupport::PmrWorkspace::StatusAhead)
                actionList << QPair<QString, QString>("push", url);
            else
                actionList << QPair<QString, QString>("sync", url);
        }
        else {
            actionList << QPair<QString, QString>("pull", url);
        }
    }

    mRow = 0;
    QStringList html = QStringList(containerHtml("workspace", icon, url, name, status, actionList));

    if (!path.isEmpty()) html << contentsHtml(pWorkspace->rootFileNode(),
                                             !mExpandedItems.contains(url));
    else                 html << emptyContentsHtml();

    return html;
}

//==============================================================================

QStringList PmrWorkspacesWidget::folderHtml(const PMRSupport::PmrWorkspaceFileNode *pFileNode)
{
    QString fullname = pFileNode->fullName();

    const QString icon = mExpandedItems.contains(fullname) ? "open" : "folder";

    mRow += 1;
    QStringList html = QStringList(containerHtml((mRow % 2) ? "folder" : "folder even",
                                                 icon, fullname, pFileNode->shortName(), "",
                                                 QList<QPair<QString, QString> >()));
    html << contentsHtml(pFileNode, !mExpandedItems.contains(fullname));

    return html;
}

//==============================================================================

const QWebElement PmrWorkspacesWidget::parentWorkspaceElement(const QWebElement &pRowElement)
{
    auto workspaceElement = pRowElement;

    // Find parent workspace

    while (!workspaceElement.isNull()
        && !(workspaceElement.tagName() == "TR"
          && workspaceElement.hasClass("workspace"))) {
        workspaceElement = workspaceElement.parent();
        if (workspaceElement.hasClass("contents"))
            workspaceElement = workspaceElement.previousSibling();
    }
    return workspaceElement;
}

//==============================================================================

void PmrWorkspacesWidget::setCurrentWorkspaceUrl(const QString &pUrl)
{
    if (pUrl != mCurrentWorkspaceUrl) {

        // Close the current workspace if we are selecting a different one

        if (!mCurrentWorkspaceUrl.isEmpty()) {
            auto workspaceContents = page()->mainFrame()->documentElement().findFirst(
                                        QString("tr.workspace[id=\"%1\"] + tr").arg(mCurrentWorkspaceUrl));

            if (!workspaceContents.isNull()) {
                workspaceContents.previousSibling().removeClass("selected");
                workspaceContents.addClass("hidden");
                mExpandedItems.remove(mCurrentWorkspaceUrl);
            }
        }

        // Set the new current workspace and ensure it is expanded when displayed

        mCurrentWorkspaceUrl = pUrl;
        mExpandedItems.insert(mCurrentWorkspaceUrl);

        // Set the active directory to the workspace

        auto workspace = mWorkspacesManager->workspace(pUrl);
        if (workspace->isLocal()) Core::setActiveDirectory(workspace->path());
    }
}

//==============================================================================

void PmrWorkspacesWidget::startStopTimer()
{
    // Start our timer if OpenCOR is active and we have a current workspace, or stop
    // it if either OpenCOR is not active or we no longer have have a current workspace.
    // Note: If we are to start our timer, then we refresh the workspace first since
    //       waiting one second may seem long to a user.

    if (Core::opencorActive() && !mCurrentWorkspaceUrl.isEmpty() && !mTimer->isActive()) {
        disconnect(qApp, SIGNAL(focusWindowChanged(QWindow *)),
                   this, SLOT(focusWindowChanged()));

        /* TODO: To avoid redrawing on every timed refresh we could check if
           `workspace->modified()` (or have `refreshStatus()` return true if
           modifications) where the `PmrWorkspace` class compares (via SHA?)
           current and new status.
        */
        refreshCurrentWorkspace();

        connect(qApp, SIGNAL(focusWindowChanged(QWindow *)),
                this, SLOT(focusWindowChanged()));

        mTimer->start(1000);
    } else if ((!Core::opencorActive() || mCurrentWorkspaceUrl.isEmpty()) && mTimer->isActive()) {
        mTimer->stop();
    }
}

//==============================================================================

void PmrWorkspacesWidget::focusWindowChanged()
{
    // Start/stop our timer

    startStopTimer();
}

//==============================================================================

void PmrWorkspacesWidget::refreshCurrentWorkspace()
{
    refreshWorkspace(mCurrentWorkspaceUrl);
}

//==============================================================================

void PmrWorkspacesWidget::setSelected(QWebElement pNewSelectedRow)
{
    QString id = pNewSelectedRow.attribute("id");

    if (!id.isEmpty() && id != mSelectedItem) {

        QWebElement trElement = page()->mainFrame()->documentElement().findFirst(
                                    QString("tr.selected[id=\"%1\"]").arg(mSelectedItem));
        if (!trElement.isNull()) trElement.removeClass("selected");

        pNewSelectedRow.addClass("selected");
        mSelectedItem = id;
    }
}

//==============================================================================

void PmrWorkspacesWidget::scrollToSelected(void)
{
    // Position the frame so that the selected line is shown

    if (!mSelectedItem.isEmpty() && mAnchors.contains(mSelectedItem)) {
        // Position two rows before the selected item to show some context.

        page()->mainFrame()->scrollToAnchor(QString("a_%1").arg(mAnchors.value(mSelectedItem) - 2));
    }
}

//==============================================================================

void PmrWorkspacesWidget::expandHtmlTree(const QString &pId)
{
    QWebElement trElement = page()->mainFrame()->documentElement().findFirst(
                                QString("tr[id=\"%1\"] + tr").arg(pId));
    if (!trElement.isNull()) {
        if (mExpandedItems.contains(pId)) {
            trElement.addClass("hidden");
            mExpandedItems.remove(pId);
        }
        else if (trElement.hasClass("contents")) {
            trElement.removeClass("hidden");
            mExpandedItems.insert(pId);
        }
    }
}

//==============================================================================

void PmrWorkspacesWidget::clearWorkspaces(void)
{
    setHtml(mTemplate.arg(QString()));
}

//==============================================================================

void PmrWorkspacesWidget::displayWorkspaces(void)
{
    QList<PMRSupport::PmrWorkspace *> workspaces = mWorkspacesManager->workspaces();

    // We want the HTML table in name order

    std::sort(workspaces.begin(), workspaces.end(), PMRSupport::PmrWorkspace::compare);

    // Reset our row anchors

    mRowAnchor = 0;
    mAnchors.clear();

    // Finally generate and emit HTML

    QStringList html;

    foreach (PMRSupport::PmrWorkspace *workspace, workspaces)
        html.append(workspaceHtml(workspace));

    setHtml(mTemplate.arg(html.join("\n")));
}

//==============================================================================

void PmrWorkspacesWidget::mouseMoveEvent(QMouseEvent *event)
{
    auto webElement = page()->mainFrame()->hitTestContent(event->pos()).element();
    while (!webElement.isNull() && webElement.tagName() != "A" && webElement.tagName() != "TR")
        webElement = webElement.parent();

    auto toolTip = QString();
    auto mouseCursor = QCursor(Qt::ArrowCursor);

    if (!webElement.isNull()) {
        if (webElement.tagName() == "A" && !webElement.attribute("href").isEmpty()) {
            QString action = webElement.attribute("href").split("|")[0];

            if      (action == "stage") {
                toolTip = tr("Stage");
            }
            else if (action == "unstage") {
                toolTip = tr("Unstage");
            }
            else if (webElement.parent().parent().hasClass("file")) {
                toolTip = tr("Open file");
                mouseCursor = QCursor(Qt::PointingHandCursor);
            }
            else if      (action == "clone") {
                toolTip = tr("Clone workspace");
            }
            else if (action == "sync" || action == "pull") {
                toolTip = tr("Synchronise with PMR");
            }
            else if (action == "push") {
                toolTip = tr("Synchronise and upload to PMR");
            }
            else if (action == "commit") {
                toolTip = tr("Commit staged changes");
            }
        }
        else {
            QString link = webElement.attribute("id");
            if (webElement.hasClass("workspace")) {
                auto workspace = mWorkspacesManager->workspace(link);

                toolTip = QString("%1\n%2").arg(workspace->url(), workspace->path());
            }
        }
    }

    setCursor(mouseCursor);
    QToolTip::showText(mapToGlobal(event->pos()), toolTip, this, QRect());
}

//==============================================================================

void PmrWorkspacesWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // Find the containing row and highlight it

        auto trElement = page()->mainFrame()->hitTestContent(event->pos()).element();
        while (!trElement.isNull() && trElement.tagName() != "TR")
            trElement = trElement.parent();

        // Select the row that's been clicked in before doing anything else

        auto rowLink = trElement.attribute("id");
        if (!trElement.hasClass("workspace"))
            setSelected(trElement);

        auto aElement = page()->mainFrame()->hitTestContent(event->pos()).element();
        while (!aElement.isNull() && aElement.tagName() != "A" && aElement.tagName() != "TR")
            aElement = aElement.parent();

        // Check if an `<a>` element has been clicked

        auto aLink = aElement.attribute("href");
        if (!aElement.isNull() && aElement.tagName() == "A" && !aLink.isEmpty()) {
            if (aElement.toPlainText().isEmpty() && aLink.contains("|")) {
                QStringList linkList = aLink.split("|");
                QString action = linkList[0];
                QString linkUrl = linkList[1];
                if      (action == "clone") {
                    cloneWorkspace(linkUrl);
                }
                else if (action == "commit") {
                    commitWorkspace(linkUrl);
                }
                else if (action == "pull" || action == "sync") {
                    synchroniseWorkspace(linkUrl, true);
                }
                else if (action == "push") {
                    synchroniseWorkspace(linkUrl, false);
                }
                else if (action == "stage" || action == "unstage") {
                    auto workspaceElement = parentWorkspaceElement(trElement);

                    if (!workspaceElement.isNull()) {
                        // Stage/unstage the file

                        auto workspace = mWorkspacesManager->workspace(workspaceElement.attribute("id"));
                        workspace->stageFile(linkUrl, (action == "stage"));

                        // Now update the file's status and action

                        auto statusActionHtml = fileStatusActionHtml(workspace, linkUrl);

                        auto statusElement = trElement.findFirst("td.status");
                        if (!statusElement.isNull()) statusElement.setInnerXml(statusActionHtml[0]);

                        auto actionElement = trElement.findFirst("td.action");
                        if (!actionElement.isNull()) actionElement.setInnerXml(statusActionHtml[1]);

                        // And update the workspace's header icons

                        refreshWorkspace(workspace->url());
                    }
               }
            }
            else if (!aLink.isEmpty()) {
                // Text link clicked, e.g. to open a file

                emit openFileRequested(aLink);
            }
        }
        else if (trElement.hasClass("workspace")) {
            if (rowLink == mCurrentWorkspaceUrl) expandHtmlTree(rowLink);
            else                                 setCurrentWorkspaceUrl(rowLink);
            if (mExpandedItems.contains(rowLink)) refreshWorkspace(rowLink);
        }
        else if (trElement.hasClass("folder")) {
            expandHtmlTree(rowLink);
        }
    }
}

//==============================================================================

void PmrWorkspacesWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    // We handle this event ourselves so default event processing
    // doesn't do anything (such as follow a link or open a file).
}

//==============================================================================

void PmrWorkspacesWidget::contextMenuEvent(QContextMenuEvent *event)
{
// TODO: Context menus for folders and files

    auto menu = new QMenu(this);

    auto pos = event->pos();

    auto trElement = page()->mainFrame()->hitTestContent(pos).element();
    while (!trElement.isNull() && trElement.tagName() != "TR")
        trElement = trElement.parent();

    if (!trElement.isNull() && trElement.hasClass("workspace")) {
        QString elementId = trElement.attribute("id");

        if (!trElement.findFirst("img.clone").isNull()) {
            auto action = new QAction(QIcon(ICON_CLONE), tr("Clone workspace"), this);
            action->setData(QString("clone|%1").arg(elementId));
            menu->addAction(action);
        }
        else {
            if (!trElement.findFirst("img.commit").isNull()) {
                auto action = new QAction(QIcon(ICON_COMMIT), tr("Commit staged changes"), this);
                action->setData(QString("commit|%1").arg(elementId));
                menu->addAction(action);
            }

            if (!trElement.findFirst("img.pull").isNull()) {
                auto action = new QAction(QIcon(ICON_SYNC_PULL), tr("Synchronise with PMR"), this);
                action->setData(QString("pull|%1").arg(elementId));
                menu->addAction(action);
            }
            if (!trElement.findFirst("img.sync").isNull()) {
                auto action = new QAction(QIcon(ICON_SYNCHRONISE), tr("Synchronise with PMR"), this);
                action->setData(QString("sync|%1").arg(elementId));
                menu->addAction(action);
            }
            else if (!trElement.findFirst("img.push").isNull()) {
                auto action = new QAction(QIcon(ICON_SYNC_PUSH), tr("Synchronise and upload to PMR"), this);
                action->setData(QString("push|%1").arg(elementId));
                menu->addAction(action);
            }

        }
        menu->addSeparator();

        auto refreshAction = new QAction(QIcon(ICON_REFRESH), tr("Refresh display"), this);
        refreshAction->setData(QString("refresh|%1").arg(elementId));
        menu->addAction(refreshAction);

        auto workspace = mWorkspacesManager->workspace(elementId);
        if (workspace) {
            auto action = new QAction(tr("View in PMR..."), this);
            action->setData(QString("pmr|%1").arg(workspace->url()));
            menu->addAction(action);
            if (workspace->isLocal()) {
                action = new QAction(tr("Show containing folder..."), this);
                action->setData(QString("show|%1").arg(workspace->path()));
                menu->addAction(action);
            }
        }

        menu->addSeparator();
        auto aboutAction = new QAction(QIcon(ICON_ABOUT), tr("About the workspace"), this);
        aboutAction->setData(QString("about|%1").arg(elementId));
        menu->addAction(aboutAction);
    }

    if (!menu->isEmpty()) {
        QAction *item = menu->exec(mapToGlobal(pos));
        if (item) {
            QStringList linkList = item->data().toString().split("|");
            QString action = linkList[0];
            QString linkId = linkList[1];
            if      (action == "about") {
                aboutWorkspace(linkId);
            }
            else if (action == "clone") {
                cloneWorkspace(linkId);
            }
            else if (action == "refresh") {
                refreshWorkspace(linkId);
            }
            else if (action == "pull" || action == "sync") {
                synchroniseWorkspace(linkId, true);
            }
            else if (action == "commit") {
                commitWorkspace(linkId);
            }
            else if (action == "push") {
                synchroniseWorkspace(linkId, false);
            }
            else if (action == "pmr") {
                QDesktopServices::openUrl(linkId);
            }
            else if (action == "show") {
                showInGraphicalShell(linkId);
            }
        }
    }
}

//==============================================================================

void PmrWorkspacesWidget::showInGraphicalShell(const QString &pPath)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(pPath));
}

//==============================================================================

void PmrWorkspacesWidget::initialiseWorkspaceWidget(const PMRSupport::PmrWorkspaceList &pWorkspaceList)
{
    // First clear existing workspaces from the manager

    mWorkspacesManager->clearWorkspaces();

    // We now reconcile URLs of `my-workspaces` (on PMR) with those from workspace
    // folders. In doing so folders/URLs that don't correspond to an actual PMR
    // workspace are pruned from the relevant maps.

    // First clear the `owned` flag from the list of URLs with workspace folders

    QMutableMapIterator<QString, QPair<QString, bool> > urlsIterator(mWorkspaceUrls);
    while (urlsIterator.hasNext()) {
        urlsIterator.next();
        urlsIterator.setValue(QPair<QString, bool>(urlsIterator.value().first, false));
    }

    foreach (PMRSupport::PmrWorkspace *workspace, pWorkspaceList) {

        // Remember our workspace so we can find it by URL

        const QString &url = workspace->url();

        mWorkspacesManager->addWorkspace(workspace);

        // Check if we know its folder and flag it is ours

        if (mWorkspaceUrls.contains(url)) {
            QString path = mWorkspaceUrls.value(url).first;

            mWorkspaceUrls.insert(url, QPair<QString, bool>(path, true));
            workspace->setPath(path);
            workspace->open();
        }
    }

    // Now check the workspace folders that aren't from `my-workspaces`

    urlsIterator.toFront();
    while (urlsIterator.hasNext()) {
        urlsIterator.next();

        if (!urlsIterator.value().second) {
            QString url = urlsIterator.key();

            auto workspace = mPmrRepository->getWorkspace(url);
            if (workspace) {
                mWorkspacesManager->addWorkspace(workspace);
                workspace->setPath(urlsIterator.value().first);
                workspace->open();
            }
            else {
                mWorkspaceFolders.remove(urlsIterator.value().first);
                urlsIterator.remove();
            }
        }
    }

    // Display the list of workspaces

    displayWorkspaces();

    // And scroll to the current selected item

    scrollToSelected();
}

//==============================================================================

void PmrWorkspacesWidget::aboutWorkspace(const QString &pUrl)
{
    QWebElement workspaceElement = page()->mainFrame()->documentElement().findFirst(
                                                          QString("tr.workspace[id=\"%1\"]").arg(pUrl));
    if (!workspaceElement.isNull()) {
        auto workspace = mWorkspacesManager->workspace(workspaceElement.attribute("id"));
        if (workspace) {
            QStringList workspaceInformation = QStringList() << workspace->name();
            if (!workspace->description().isEmpty()) workspaceInformation << workspace->description();
            if (!workspace->owner().isEmpty()) workspaceInformation << tr("Owner: %1").arg(workspace->owner());
            workspaceInformation << tr("PMR: <a href=\"%1\">%1</a>").arg(workspace->url());
            if (workspace->isLocal()) workspaceInformation << tr("Path: <a href=\"file://%1/\">%1</a>").arg(workspace->path());
            emit information(workspaceInformation.join("<br></br><br></br>"));
        }
    }
}

//==============================================================================

void PmrWorkspacesWidget::cloneWorkspace(const QString &pUrl)
{
    auto workspace = mWorkspacesManager->workspace(pUrl);

    if (workspace && !(workspace->isNull() || workspace->isLocal())) {
        auto dirName = PMRSupport::PmrWorkspace::getEmptyWorkspaceDirectory();

        if (!dirName.isEmpty()) {
            // Create the folder for the new workspace

            auto workspaceFolder = QDir(dirName);
            if (!workspaceFolder.exists())
                workspaceFolder.mkpath(".");

            mPmrRepository->requestWorkspaceClone(workspace, dirName);
        }
    }
}

//==============================================================================

void PmrWorkspacesWidget::commitWorkspace(const QString &pUrl)
{
    auto workspace = mWorkspacesManager->workspace(pUrl);

    if (workspace && !workspace->isNull() && workspace->isLocal()) {

        auto commitDialog = new PmrWorkspacesCommit(workspace->stagedFilesList());

        if (commitDialog->exec() == QDialog::Accepted
         && workspace->commit(commitDialog->message()))
            refreshWorkspace(workspace->url());
    }
}

//==============================================================================

void PmrWorkspacesWidget::refreshWorkspace(const QString &pUrl)
{
    QWebElement workspaceElement = page()->mainFrame()->documentElement().findFirst(
                                       QString("tr.workspace[id=\"%1\"]").arg(pUrl));
    if (!workspaceElement.isNull()) {
        auto workspace = mWorkspacesManager->workspace(workspaceElement.attribute("id"));
        if (workspace) {
            // We have a valid workspace so refresh its status

            workspace->refreshStatus();

            // And replace the header and content rows

            auto htmlRows = workspaceHtml(workspace);

            // Using workspaceElement.nextSibling() directly doesn't update Xml

            auto contentsElement = workspaceElement.nextSibling();

            workspaceElement.setOuterXml(htmlRows[0]);    // header
            contentsElement.setOuterXml(htmlRows[1]);     // contents
        }
    }
}

//==============================================================================

void PmrWorkspacesWidget::refreshWorkspaceFile(const QString &pPath)
{
    auto fileElement = page()->mainFrame()->documentElement().findFirst(
                                                     QString("tr.file[id=\"%1\"]").arg(pPath));
    auto workspaceElement = parentWorkspaceElement(fileElement);

    if (!workspaceElement.isNull()) {
        auto workspace = mWorkspacesManager->workspace(workspaceElement.attribute("id"));
        if (workspace) {
            // We have a valid workspace so update file's status

            auto statusActionHtml = fileStatusActionHtml(workspace, pPath);

            auto statusElement = fileElement.findFirst("td.status");
            if (!statusElement.isNull()) statusElement.setInnerXml(statusActionHtml[0]);

            auto actionElement = fileElement.findFirst("td.action");
            if (!actionElement.isNull()) actionElement.setInnerXml(statusActionHtml[1]);
        }
    }
}

//==============================================================================

void PmrWorkspacesWidget::refreshWorkspaces(const bool &pScanFolders)
{
    if (pScanFolders) scanDefaultWorkspaceDirectory();

    // `initialiseWorkspaceWidget()` will be called when list received.

    mPmrRepository->requestWorkspacesList();
}

//==============================================================================

void PmrWorkspacesWidget::synchroniseWorkspace(const QString &pUrl, const bool pOnlyPull)
{
    auto workspace = mWorkspacesManager->workspace(pUrl);

    if (workspace && !workspace->isNull() && workspace->isLocal())
        mPmrRepository->requestWorkspaceSynchronise(workspace, pOnlyPull);
}

//==============================================================================

void PmrWorkspacesWidget::workspaceCreated(const QString &pUrl)
{
    Q_UNUSED(pUrl)

    // Ignore as `workspaceCloned` will be called next
}

//==============================================================================

void PmrWorkspacesWidget::workspaceCloned(PMRSupport::PmrWorkspace *pWorkspace)
{
    if (pWorkspace) {
        QString url = pWorkspace->url();

        // Ensure our widget knows about the workspace

        if (!mWorkspaceUrls.contains(url))
            addWorkspace(pWorkspace);

        // Close display of current workspace and set the cloned one current

        setCurrentWorkspaceUrl(url);

        // Redisplay with workspace expanded and selected

        pWorkspace->open();
        displayWorkspaces();
        scrollToSelected();
    }
}

//==============================================================================

void PmrWorkspacesWidget::workspaceSynchronised(PMRSupport::PmrWorkspace *pWorkspace)
{
    if (pWorkspace) {
        refreshWorkspace(pWorkspace->url());
    }
}

//==============================================================================

}   // namespace PMRWorkspaces
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
