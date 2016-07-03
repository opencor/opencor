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
#include "pmrworkspacesmanager.h"
#include "pmrworkspaceswidget.h"

//==============================================================================

#include <QContextMenuEvent>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHeaderView>
#include <QHelpEvent>
#include <QMenu>
#include <QMutableMapIterator>
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

    QByteArray fileContents;
    Core::readFileContentsFromFile(":/PMRWorkspaces/output.html", fileContents);
    mTemplate = QString(fileContents).arg( // clone, folder, open, star, pull, commit, push
                                          Core::iconDataUri(":/oxygen/places/folder-downloads.png", 16, 16),
                                          Core::iconDataUri(":/oxygen/places/folder.png", 16, 16),
                                          Core::iconDataUri(":/oxygen/places/folder-open.png", 16, 16),
                                          Core::iconDataUri(":/oxygen/places/folder-favorites.png", 16, 16),
                                          Core::iconDataUri(":/oxygen/actions/arrow-down-double.png", 16, 16),
                                          Core::iconDataUri(":/oxygen/actions/dialog-ok-apply.png", 16, 16),
                                          Core::iconDataUri(":/oxygen/actions/arrow-up-double.png", 16, 16),
                                          "%1");
// TODO Use constants for icon names -- CloneIcon etc
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

QString PmrWorkspacesWidget::actionHtml(const QList<QPair<QString, QString> > &pActions)
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
                                           const QList<QPair<QString, QString> > &pActions)
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

    return html.arg(rowClass, pId, iconHtml, pName, pStatus, actionHtml(pActions)).arg(mRowAnchor);
}

//==============================================================================

QString PmrWorkspacesWidget::fileHtml(const PMRSupport::PmrWorkspace *pWorkspace,
                                      const QString &pPath, const QString &pFileName,
                                      const QList<QPair<QString, QString> > &pActions)
{
    static const QString html = "<tr class=\"file%1\" id=\"%2\">\n"
                                "  <td colspan=\"2\"><a id=\"a_%6\" href=\"%2\">%3</a></td>\n"
                                "  <td class=\"status\">%4</td>\n"
                                "  <td class=\"action\">%5</td>\n"
                                "</tr>\n";
    mRow += 1;
    QString rowClass = (mRow % 2) ? "" : " even";
    if (pPath == mSelectedItem) rowClass += " selected";

    QString gitStatus = pWorkspace->gitFileStatus(pPath);

    // Use an anchor element to allow us to set the scroll position at a row

    mRowAnchor += 1;
    mAnchors.insert(pPath, mRowAnchor);

    return html.arg(rowClass, pPath, pFileName, gitStatus, actionHtml(pActions)).arg(mRowAnchor);
}

//==============================================================================

QString PmrWorkspacesWidget::emptyContentsHtml(void)
{
    static const QString html = "<tr></tr>\n";

    return html;
}

//==============================================================================

QString PmrWorkspacesWidget::contentsHtml(const PMRSupport::PmrWorkspace *pWorkspace,
                                          const QString &pPath)
{
    static const QString html = "<tr class=\"contents\">\n"
                                "  <td></td>\n"
                                "  <td colspan=\"3\">\n"
                                "    <table>\n"
                                "      %1\n"
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
                                                   info.absoluteFilePath(), info.fileName(),
                                                   QList<QPair<QString, QString> >()); // status, actions
        }
    }

    return html.arg(itemHtml.join("\n"));
}

//==============================================================================


QStringList PmrWorkspacesWidget::workspaceHtml(const PMRSupport::PmrWorkspace *pWorkspace)
{
    const QString &url = pWorkspace->url();
    const QString &name = pWorkspace->name();
    const QString &path = pWorkspace->path();

    const QString icon = pWorkspace->isOwned() ? "star" : "folder";

    auto remoteStatus = pWorkspace->gitRemoteStatus();
    auto workplaceStatus = QString();

    auto actionList = QList<QPair<QString, QString> >();
    if (path.isEmpty()) {
        actionList << QPair<QString, QString>("clone", url);
    }
    else if (remoteStatus == PMRSupport::PmrWorkspace::StatusBehind) {
        actionList << QPair<QString, QString>("pull", url);
        workplaceStatus = "B";
    }
    else if (remoteStatus == PMRSupport::PmrWorkspace::StatusCommit) {
        actionList << QPair<QString, QString>("commit", url);
        workplaceStatus = "C";
    }
    else if (remoteStatus == PMRSupport::PmrWorkspace::StatusAhead) {
        actionList << QPair<QString, QString>("push", url);
        workplaceStatus = "A";
    }

    mRow = 0;
    QStringList html = QStringList(containerHtml("workspace", icon, url, name,
                                                 workplaceStatus, actionList));

    if (!path.isEmpty() && mExpandedItems.contains(url)) html << contentsHtml(pWorkspace, path);
    else                                                 html << emptyContentsHtml();

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

    if (mExpandedItems.contains(fullname)) html << contentsHtml(pWorkspace, pPath);
    else                                   html << emptyContentsHtml();

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
            trElement.setStyleProperty("display", "none");
            mExpandedItems.remove(pId);
        }
        else if (trElement.hasClass("contents")) {
            trElement.setStyleProperty("display", "table-row");
            mExpandedItems.insert(pId);
        }
        else {
            // First find parent workspace

            QWebElement workspaceElement = trElement.previousSibling();
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
                    // We have a valid parent so fill empty row with content

                    if (pId == workspace->url()) {
                        QString path = workspace->path();
                        if (!path.isEmpty()) trElement.setOuterXml(contentsHtml(workspace, path));
                    }
                    else {
                        trElement.setOuterXml(contentsHtml(workspace, pId));
                    }
                }
            }

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
        if (webElement.tagName() == "A") {
            QString link = webElement.attribute("href");
            if (webElement.parent().parent().hasClass("file")) {
                toolTip = tr("Open File");
                mouseCursor = QCursor(Qt::PointingHandCursor);
            }
            else {
                QStringList linkList = link.split("|");
                if (!linkList[0].compare("clone")) {
                    toolTip = tr("Clone the Workspace");
                }
                else if (!linkList[0].compare("pull")) {
                    toolTip = tr("Pull updates from PMR");
                }
                else if (!linkList[0].compare("commit")) {
                    toolTip = tr("Commit changes");
                }
                else if (!linkList[0].compare("push")) {
                    toolTip = tr("Push committed changes to PMR");
                }
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

        auto aElement = page()->mainFrame()->hitTestContent(event->pos()).element();
        while (!aElement.isNull() && aElement.tagName() != "A")
            aElement = aElement.parent();

        // Check if an `<a>` element has been clicked

        if (!aElement.isNull()) {
            if (aElement.toPlainText().isEmpty()) {
                QStringList linkList = aElement.attribute("href").split("|");
                if (!linkList[0].compare("clone")) {
                    cloneWorkspace(linkList[1]);
                }
                else if (!linkList[0].compare("pull")) {
                    pullWorkspace(linkList[1]);
                }
                else if (!linkList[0].compare("commit")) {
                    commitWorkspace(linkList[1]);
                }
                else if (!linkList[0].compare("push")) {
qDebug() << "pushing " << linkList[1];
                    pushWorkspace(linkList[1]);
                }

// TODO
// commit      }  Both against a workspace (i.e. commit is for all C/A/D ??)
// push        }  Auto-push after a commit??

            }
            else {
                // Text link clicked, e.g. to open a file

                emit openFileRequested(aElement.attribute("href"));
            }
        }
        else if (!trElement.isNull()) {
            QString link = trElement.attribute("id");
            if (!link.isEmpty()) {
                if (trElement.hasClass("workspace")
                 || trElement.hasClass("folder"))
                    expandHtmlTree(link);
                setSelected(link);
            }
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

        if (!trElement.findFirst("img.clone").isNull()) {
            auto cloneAction = new QAction(QIcon(":/oxygen/places/folder-downloads.png"),
                                                 tr("Clone"), this);
            cloneAction->setData(QString("clone|%1").arg(trElement.attribute("id")));
            menu->addAction(cloneAction);
            menu->addSeparator();
        }
        else if (!trElement.findFirst("img.pull").isNull()) {
            auto cloneAction = new QAction(QIcon(":/oxygen/places/folder-downloads.png"),
                                                 tr("Pull"), this);
            cloneAction->setData(QString("pull|%1").arg(trElement.attribute("id")));
            menu->addAction(cloneAction);
            menu->addSeparator();
        }
        else if (!trElement.findFirst("img.commit").isNull()) {
            auto cloneAction = new QAction(QIcon(":/oxygen/places/folder-downloads.png"),
                                                 tr("Commit"), this);
            cloneAction->setData(QString("commit|%1").arg(trElement.attribute("id")));
            menu->addAction(cloneAction);
            menu->addSeparator();
        }
        else if (!trElement.findFirst("img.push").isNull()) {
            auto cloneAction = new QAction(QIcon(":/oxygen/actions/arrow-down-double.png"),
                                                 tr("Push"), this);
            cloneAction->setData(QString("push|%1").arg(trElement.attribute("id")));
            menu->addAction(cloneAction);
            menu->addSeparator();
        }
        else {
            auto refreshAction = new QAction(QIcon(":/oxygen/actions/dialog-ok-apply.png"),
                                                 tr("Refresh"), this);
            refreshAction->setData(QString("refresh|%1").arg(trElement.attribute("id")));
            menu->addAction(refreshAction);
            menu->addSeparator();
        }

        auto aboutAction = new QAction(QIcon(":/oxygen/actions/arrow-up-double.png"),
                                       tr("About"), this);
        aboutAction->setData(QString("about|%1").arg(trElement.attribute("id")));
        menu->addAction(aboutAction);

        // TODO Add "Show in Finder" (plus Windows/Linux equivalents) to menu...
    }

    if (!menu->isEmpty()) {
        QAction *item = menu->exec(mapToGlobal(pos));
        if (item) {
            QStringList linkList = item->data().toString().split("|");
            if      (linkList[0] == "about") {
                aboutWorkspace(linkList[1]);
            }
            else if (linkList[0] == "clone") {
                cloneWorkspace(linkList[1]);
            }
            else if (linkList[0] == "refresh") {
                refreshWorkspace(linkList[1]);
            }
            else if (linkList[0] == "pull") {
                pullWorkspace(linkList[1]);
            }
            else if (linkList[0] == "commit") {
                commitWorkspace(linkList[1]);
            }
            else if (linkList[0] == "push") {
                pushWorkspace(linkList[1]);
            }
        }
    }
}

//==============================================================================
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
// TODO  Show list of files to be committed (the index?) and get commit message
//      workspace->commit(message);
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

// TODO this after a commit

void PmrWorkspacesWidget::refreshWorkspace(const QString &pUrl)
{
    QWebElement workspaceElement = page()->mainFrame()->documentElement().findFirst(
                                                          QString("tr.workspace[id=\"%1\"]").arg(pUrl));
    if (!workspaceElement.isNull()) {
        auto workspace = mWorkspacesManager->workspace(workspaceElement.attribute("id"));
        if (workspace) {
            // We have a valid workplace so replace its header and content row

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

            auto statusElement = fileElement.findFirst("td.status");
            if (!statusElement.isNull())
                statusElement.setPlainText(workspace->gitFileStatus(pPath));
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

        if (!mWorkspacesManager->hasWorkspace(url))
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
