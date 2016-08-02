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
#include "pmrworkspacescommit.h"
#include "pmrworkspacesmanager.h"
#include "pmrworkspaceswidget.h"

//==============================================================================

#include <QContextMenuEvent>
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
#include <QToolTip>
#include <QUrl>
#include <QWebElement>
#include <QWebFrame>

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspaces {

//==============================================================================

#define ICON_CLONE        ":/oxygen/places/folder-downloads.png"
#define ICON_FOLDER       ":/oxygen/places/folder.png"
#define ICON_FOLDER_OPEN  ":/oxygen/places/folder-open.png"
#define ICON_OWNED        ":/oxygen/places/folder-favorites.png"
#define ICON_PULL         ":/oxygen/actions/arrow-down-double.png"
#define ICON_STAGE        ":/oxygen/actions/dialog-ok-apply.png"
#define ICON_UNSTAGE      ":/oxygen/actions/dialog-cancel.png"
#define ICON_COMMIT       ":/oxygen/actions/view-task.png"
#define ICON_PUSH         ":/oxygen/actions/arrow-up-double.png"
#define ICON_REFRESH      ":/oxygen/actions/view-refresh.png"
#define ICON_ABOUT        ":/oxygen/actions/help-about.png"

//==============================================================================

PmrWorkspacesWidget::PmrWorkspacesWidget(PMRSupport::PmrRepository *pPmrRepository, QWidget *pParent) :
    OpenCOR::WebViewerWidget::WebViewerWidget(pParent),
    Core::CommonWidget(),
    mPmrRepository(pPmrRepository),
    mWorkspacesManager(OpenCOR::PMRSupport::PmrWorkspacesManager::instance()),
    mWorkspaceFolders(QMap<QString, QString>()),
    mWorkspaceUrls(QMap<QString, QPair<QString, bool> >()),
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

    connect(mPmrRepository, SIGNAL(workspaceCreated(QString)),
            this, SLOT(workspaceCreated(QString)));
    connect(mPmrRepository, SIGNAL(workspaceCloned(PMRSupport::PmrWorkspace *)),
            this, SLOT(workspaceCloned(PMRSupport::PmrWorkspace *)));
    connect(mPmrRepository, SIGNAL(workspacePushed(PMRSupport::PmrWorkspace *)),
            this, SLOT(workspacePushed(PMRSupport::PmrWorkspace *)));

    // Handle workspace cloned signals from other plugins

    connect(OpenCOR::PMRSupport::PmrWorkspacesManager::instance(), SIGNAL(workspaceCloned(PMRSupport::PmrWorkspace *)),
            this, SLOT(workspaceCloned(PMRSupport::PmrWorkspace *)));

    // Retrieve the HTML template

    QString fileContents;
    Core::readFileContentsFromFile(":/PMRWorkspaces/output.html", fileContents);
    mTemplate = fileContents.arg( // clone, folder, open, star, pull, stage, unstage, commit, push
                                 Core::iconDataUri(ICON_CLONE, 16, 16),
                                 Core::iconDataUri(ICON_FOLDER, 16, 16),
                                 Core::iconDataUri(ICON_FOLDER_OPEN, 16, 16),
                                 Core::iconDataUri(ICON_OWNED, 16, 16),
                                 Core::iconDataUri(ICON_PULL, 16, 16),
                                 Core::iconDataUri(ICON_STAGE, 16, 16),
                                 Core::iconDataUri(ICON_UNSTAGE, 16, 16),
                                 Core::iconDataUri(ICON_COMMIT, 16, 16),
                                 Core::iconDataUri(ICON_PUSH, 16, 16))
                                 .arg("%1");
}

//==============================================================================

PmrWorkspacesWidget::~PmrWorkspacesWidget()
{
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

//==============================================================================

void PmrWorkspacesWidget::loadSettings(QSettings *pSettings)
{
    pSettings->beginGroup(SettingsWorkspaces);

        // Retrieve the names of folders containing cloned workspaces

        QStringList folders = pSettings->value(SettingsFolders).toStringList();
        foreach (QString folder, folders) {
            if (!folder.isEmpty()) addWorkspaceFolder(folder);
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

void PmrWorkspacesWidget::addWorkspaceFolder(const QString &pFolder)
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
    const QString rowClass = pClass + ((pId == mSelectedItem) ? " selected" : "");

    // Use an anchor element to allow us to set the scroll position at a row

    mRowAnchor += 1;
    mAnchors.insert(pId, mRowAnchor);

    return html.arg(rowClass, pId, iconHtml, pName, pStatus, actionHtml(pActionList)).arg(mRowAnchor);
}

//==============================================================================

const QStringList PmrWorkspacesWidget::fileStatusActionHtml(const PMRSupport::PmrWorkspace *pWorkspace,
                                                            const QString &pPath)
{
    static const QString statusHtml = "<span class=\"istatus\">%1</span><span class=\"wstatus\">%2</span>";

    auto gitStatus = pWorkspace->gitFileStatus(pPath);

    auto actionList = QList<QPair<QString, QString> >();
    if      (gitStatus.second != ' ')
        actionList << QPair<QString, QString>("stage", pPath);
    else if (gitStatus.first != ' ')
        actionList << QPair<QString, QString>("unstage", pPath);
//qDebug() << "S: " << gitStatus << " " << actionList;

    return QStringList() << statusHtml.arg(gitStatus.first).arg(gitStatus.second)
                         << actionHtml(actionList);
}

//==============================================================================

QString PmrWorkspacesWidget::fileHtml(const PMRSupport::PmrWorkspace *pWorkspace,
                                      const QString &pPath, const QString &pFileName)
{
    static const QString html = "<tr class=\"file%1\" id=\"%2\">\n"
                                "  <td colspan=\"2\" class=\"name\"><a id=\"a_%6\" href=\"%2\">%3</a></td>\n"
                                "  <td class=\"status\">%4</td>\n"
                                "  <td class=\"action\">%5</td>\n"
                                "</tr>\n";
    mRow += 1;
    QString rowClass = (mRow % 2) ? "" : " even";
    if (pPath == mSelectedItem) rowClass += " selected";

    auto statusActionHtml = fileStatusActionHtml(pWorkspace, pPath);

    // Use an anchor element to allow us to set the scroll position at a row

    mRowAnchor += 1;
    mAnchors.insert(pPath, mRowAnchor);

    return html.arg(rowClass, pPath, pFileName, statusActionHtml[0], statusActionHtml[1]).arg(mRowAnchor);
}

//==============================================================================

QString PmrWorkspacesWidget::emptyContentsHtml(void)
{
    static const QString html = "<tr></tr>\n";

    return html;
}

//==============================================================================

QString PmrWorkspacesWidget::contentsHtml(const PMRSupport::PmrWorkspace *pWorkspace,
                                          const QString &pPath, const bool &pHidden)
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
    QStringList itemHtml;

    QDir directory = QDir(pPath);
    if (directory.exists()) {
        foreach(QFileInfo info,
                directory.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name)) {
            if (info.isDir()) itemHtml << folderHtml(pWorkspace, info.absoluteFilePath());
            else              itemHtml << fileHtml(pWorkspace,
                                                   info.absoluteFilePath(), info.fileName());
        }
    }

    return html.arg(pHidden ? " hidden" : "", itemHtml.join("\n"));
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
    else if (remoteStatus & PMRSupport::PmrWorkspace::StatusBehind) {
        actionList << QPair<QString, QString>("pull", url);
    }
    else {
        if (remoteStatus & PMRSupport::PmrWorkspace::StatusCommit) {
            actionList << QPair<QString, QString>("commit", url);
        }
        if (remoteStatus & PMRSupport::PmrWorkspace::StatusAhead && pWorkspace->isOwned()) {
            actionList << QPair<QString, QString>("push", url);
        }
    }
    if (remoteStatus & PMRSupport::PmrWorkspace::StatusUnstaged) {
        status = "?";
    }

    mRow = 0;
    QStringList html = QStringList(containerHtml("workspace", icon, url, name, status, actionList));

    if (!path.isEmpty()) html << contentsHtml(pWorkspace, path, !mExpandedItems.contains(url));
    else                 html << emptyContentsHtml();

    return html;
}

//==============================================================================

QStringList PmrWorkspacesWidget::folderHtml(const PMRSupport::PmrWorkspace *pWorkspace,
                                            const QString &pPath)
{
    QFileInfo info = QFileInfo(pPath);
    QString fullname = info.absoluteFilePath();

    const QString icon = mExpandedItems.contains(fullname) ? "open" : "folder";

    mRow += 1;
    QStringList html = QStringList(containerHtml((mRow % 2) ? "folder" : "folder even",
                                                 icon, fullname, info.fileName(), "",
                                                 QList<QPair<QString, QString> >())); // status, actions

    html << contentsHtml(pWorkspace, pPath, !mExpandedItems.contains(fullname));

    return html;
}

//==============================================================================

void PmrWorkspacesWidget::setSelected(const QString &pId)
{
    if (pId != mSelectedItem) {
        QWebElement trElement = page()->mainFrame()->documentElement().findFirst(
                                                       QString("tr.selected[id=\"%1\"]")
                                                               .arg(mSelectedItem));
        if (!trElement.isNull()) trElement.removeClass("selected");

        trElement = page()->mainFrame()->documentElement().findFirst(QString("tr[id=\"%1\"]")
                                                                             .arg(pId));
        if (!trElement.isNull()) trElement.addClass("selected");

        mSelectedItem = pId;
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
    setSelected("");
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
            else if (action == "pull") {
                toolTip = tr("Pull updates from PMR");
            }
            else if (action == "commit") {
                toolTip = tr("Commit staged changes");
            }
            else if (action == "push") {
                toolTip = tr("Push committed changes to PMR");
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

        QString rowLink = trElement.attribute("id");
        if (!rowLink.isEmpty())
            setSelected(rowLink);

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
                else if (action == "pull") {
                    pullWorkspace(linkUrl);
                }
                else if (action == "commit") {
                    commitWorkspace(linkUrl);
                }
                else if (action == "push") {
                    pushWorkspace(linkUrl);
                }
                else if (action == "stage" || action == "unstage") {
                    auto workspaceElement = trElement;
                    while (!workspaceElement.isNull()
                        && !(workspaceElement.tagName() == "TR"
                          && workspaceElement.hasClass("workspace"))) {
                        workspaceElement = workspaceElement.parent();
                        if (workspaceElement.hasClass("contents"))
                            workspaceElement = workspaceElement.previousSibling();
                    }
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
        else if (trElement.hasClass("workspace") || trElement.hasClass("folder")) {
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
        bool separator = false ;

        if (!trElement.findFirst("img.clone").isNull()) {
            auto action = new QAction(QIcon(ICON_CLONE), tr("Clone workspace"), this);
            action->setData(QString("clone|%1").arg(elementId));
            menu->addAction(action);
            separator = true;
        }
        else if (!trElement.findFirst("img.pull").isNull()) {
            auto action = new QAction(QIcon(ICON_PULL), tr("Pull updates"), this);
            action->setData(QString("pull|%1").arg(elementId));
            menu->addAction(action);
            separator = true;
        }
        else {
            if (!trElement.findFirst("img.commit").isNull()) {
                auto action = new QAction(QIcon(ICON_COMMIT), tr("Commit staged changes"), this);
                action->setData(QString("commit|%1").arg(elementId));
                menu->addAction(action);
                separator = true;
            }
            if (!trElement.findFirst("img.push").isNull()) {
                auto action = new QAction(QIcon(ICON_PUSH), tr("Push commits"), this);
                action->setData(QString("push|%1").arg(elementId));
                menu->addAction(action);
                separator = true;
            }
        }
        if (separator) menu->addSeparator();

        auto refreshAction = new QAction(QIcon(ICON_REFRESH), tr("Refresh display"), this);
        refreshAction->setData(QString("refresh|%1").arg(elementId));
        menu->addAction(refreshAction);

        auto workspace = mWorkspacesManager->workspace(elementId);
        if (workspace && workspace->isLocal()) {
#if defined(Q_OS_WIN)
            auto action = new QAction(tr("Show in Explorer..."), this);
#elif defined(Q_OS_MAC)
            auto action = new QAction(tr("Show in Finder..."), this);
#else
// TODO auto showAction = new QAction(tr("Show containing folder..."), this);
#endif
            action->setData(QString("show|%1").arg(workspace->path()));
            menu->addAction(action);
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
            else if (action == "pull") {
                pullWorkspace(linkId);
            }
            else if (action == "commit") {
                commitWorkspace(linkId);
            }
            else if (action == "push") {
                pushWorkspace(linkId);
            }
            else if (action == "show") {
                showInGraphicalShell(linkId);
            }
        }
    }
}

//==============================================================================

// Based on https://gitlab.com/pteam/pteam-qt-creator/blob/master/src/plugins/coreplugin/fileutils.cpp#L76

void PmrWorkspacesWidget::showInGraphicalShell(const QString &pPath)
{
#if defined(Q_OS_WIN)
    const QString explorer = QStandardPaths::findExecutable("explorer.exe");
    if (explorer.isEmpty()) {
        emit warning(tr("Windows Explorer is not in PATH"));
        return;
    }
    auto parameters = QStringList();
    if (!QFileInfo(pPath).isDir())
        parameters << QString("/select,");
    parameters << QDir::toNativeSeparators(pPath);
    QProcess::startDetached(explorer, parameters);
#elif defined(Q_OS_MAC)
    auto parameters = QStringList();
    parameters << "-e" << "tell application \"Finder\"";
    parameters << "-e" << "activate";
    parameters << "-e" << QString("open POSIX file \"%1\"").arg(pPath);
    parameters << "-e" << "end tell";
    QProcess::startDetached("osascript", parameters);
#else
    Q_UNUSED(pPath)
/*  TODO
    // we cannot select a file here, because no file browser really supports it...
    const QFileInfo fileInfo(pPath);
    const QString folder = fileInfo.isDir() ? fileInfo.absoluteFilePath() : fileInfo.filePath();
    // https://gitlab.com/pteam/pteam-qt-creator/blob/master/src/libs/utils/unixutils.cpp#L44
    const QString app = UnixUtils::fileBrowser(ICore::settings());
    // https://gitlab.com/pteam/pteam-qt-creator/blob/master/src/libs/utils/unixutils.cpp#L71
    const QString browserArgs = UnixUtils::substituteFileBrowserParameters(app, folder);
    QProcess browserProc;
    bool success = browserProc.startDetached(browserArgs);
    const QString error = QString::fromLocal8Bit(browserProc.readAllStandardError());
    success = success && error.isEmpty();
    if (!success)
        showGraphicalShellError(parent, app, error);
*/
#endif
}

//==============================================================================

void PmrWorkspacesWidget::initialiseWorkspaces(const PMRSupport::PmrWorkspaceList &pWorkspaceList)
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
            workspaceInformation << tr("Url: %1").arg(workspace->url());
            if (workspace->isLocal()) workspaceInformation << tr("Path: %1").arg(workspace->path());
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
            if (!workspaceFolder.exists()) {
                workspaceFolder.mkpath(".");
            }

            mPmrRepository->getWorkspaceCredentials(workspace);
            mPmrRepository->requestWorkspaceClone(workspace, dirName);
        }
    }
}

//==============================================================================

void PmrWorkspacesWidget::pullWorkspace(const QString &pUrl)
{
    auto workspace = mWorkspacesManager->workspace(pUrl);

    if (workspace && !workspace->isNull() && workspace->isLocal()) {
        mPmrRepository->getWorkspaceCredentials(workspace);
// TODO
//      mPmrRepository->requestWorkspacePull(workspace);
    }
}

//==============================================================================

void PmrWorkspacesWidget::commitWorkspace(const QString &pUrl)
{
    auto workspace = mWorkspacesManager->workspace(pUrl);

    if (workspace && !workspace->isNull() && workspace->isLocal()) {

        auto commitDialog = new PmrWorkspacesCommit();

// TODO  Set list of files to be committed (the index?) and get commit message
//      commitDialog->setMessage(workspace->defaultCommitMessage());
        if (commitDialog->exec() == QDialog::Accepted) {

            if (workspace->commit(commitDialog->message()))
                refreshWorkspace(workspace->url());

        }
    }
}

//==============================================================================

void PmrWorkspacesWidget::pushWorkspace(const QString &pUrl)
{
    auto workspace = mWorkspacesManager->workspace(pUrl);

    if (workspace && !workspace->isNull() && workspace->isLocal()) {
        mPmrRepository->getWorkspaceCredentials(workspace);
        mPmrRepository->requestWorkspacePush(workspace);
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
            // We have a valid workplace so reopen it

            workspace->open();

            // And replace its header and content row

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
    // Find parent workspace

    auto workspaceElement = fileElement;
    while (!workspaceElement.isNull()
        && !(workspaceElement.tagName() == "TR"
          && workspaceElement.hasClass("workspace"))) {
        workspaceElement = workspaceElement.parent();
        if (workspaceElement.hasClass("contents"))
            workspaceElement = workspaceElement.previousSibling();
    }

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

    mPmrRepository->requestWorkspacesList();
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

        // Redisplay with workspace expanded and selected

        mExpandedItems.insert(url);
        setSelected(url);
        displayWorkspaces();
        scrollToSelected();
    }
}

//==============================================================================

void PmrWorkspacesWidget::workspacePushed(PMRSupport::PmrWorkspace *pWorkspace)
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
