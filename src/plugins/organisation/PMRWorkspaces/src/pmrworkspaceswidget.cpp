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
    mWorkspacesMap(QMap<QString, PMRSupport::PmrWorkspace *>()),
    mWorkspaceFolders(QMap<QString, QString>()),
    mWorkspaceUrls(QMap<QString, QPair<QString, bool> >()),
    mExpandedItems(QSet<QString>()),
    mSelectedItem(QString())
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

    // Retrieve the HTML template

    QByteArray fileContents;
    Core::readFileContentsFromFile(":/PMRWorkspaces/output.html", fileContents);
    mTemplate = QString(fileContents).arg( // clone, folder, open, star
                                          Core::iconDataUri(":/oxygen/places/folder-downloads.png", 16, 16),
                                          Core::iconDataUri(":/oxygen/places/folder.png", 16, 16),
                                          Core::iconDataUri(":/oxygen/places/folder-open.png", 16, 16),
                                          Core::iconDataUri(":/oxygen/places/folder-favorites.png", 16, 16),
                                          "%1");
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
            mWorkspacesMap.insert(url, pWorkspace);

// TODO This is where we need to insert workspace into displayed list...
// At a minimum we need to redisplay html...
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
    if (workspaceDirectory.exists()) {
        foreach(QFileInfo info, workspaceDirectory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name)) {
            if (info.isDir()) addWorkspaceFolder(info.absoluteFilePath());
        }
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
                                "  <td class=\"icon\">%3</td>\n"
                                "  <td class=\"name\">%4</td>\n"
                                "  <td class=\"status\">%5</td>\n"
                                "  <td class=\"action\">%6</td>\n"
                                "</tr>\n";

    const QString iconHtml = QString("<img class=\"%1\" />").arg(pIcon);
    const QString rowClass = pClass + ((pId == mSelectedItem) ? " selected" : "");

    return html.arg(rowClass, pId, iconHtml, pName, pStatus, actionHtml(pActions));
}

//==============================================================================

QString PmrWorkspacesWidget::fileHtml(const PMRSupport::PmrWorkspace *pWorkspace,
                                      const QString &pPath, const QString &pFileName,
                                      const QList<QPair<QString, QString> > &pActions)
{
    static const QString html = "<tr class=\"file%1\" id=\"%2\">\n"
                                "  <td colspan=\"2\"><a href=\"%2\">%3</a></td>\n"
                                "  <td class=\"status\">%4</td>\n"
                                "  <td class=\"action\">%5</td>\n"
                                "</tr>\n";
    mRow += 1;
    QString rowClass = (mRow % 2) ? "" : " even";
    if (pPath == mSelectedItem) rowClass += " selected";

    QString gitStatus = pWorkspace->gitStatus(pPath);

    return html.arg(rowClass, pPath, pFileName, gitStatus, actionHtml(pActions));
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

    auto actionList = QList<QPair<QString, QString> >();
    if (path.isEmpty()) {
        actionList << QPair<QString, QString>("clone", url);
    }

    mRow = 0;
    QStringList html = QStringList(containerHtml("workspace", icon, url, name,
                                                 pWorkspace->gitStatus(), actionList));

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
                auto workspace = mWorkspacesMap.value(workspaceElement.attribute("id"));
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
//    mWorkspaceNames = QStringList();
//    mWorkspaceUrlId = QMap<QString, int>();

    setHtml(mTemplate.arg(QString()));
}

//==============================================================================

void PmrWorkspacesWidget::displayWorkspaces(void)
{
    auto workspaces = mWorkspacesMap.values();

    // We want the HTML table in name order

    std::sort(workspaces.begin(), workspaces.end(), PMRSupport::PmrWorkspace::compare);

    // Finally generate and emit HTML

    QStringList html;

    foreach (PMRSupport::PmrWorkspace *workspace, workspaces)
        html.append(workspaceHtml(workspace));

    setHtml(mTemplate.arg(html.join("\n")));

    // Can we position view so that the selected line is shown??

}

//==============================================================================

void PmrWorkspacesWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        auto aElement = page()->mainFrame()->hitTestContent(event->pos()).element();
        while (!aElement.isNull() && aElement.tagName() != "A")
            aElement = aElement.parent();

        if (!aElement.isNull() && aElement.toPlainText().isEmpty()) {
            QStringList linkList = aElement.attribute("href").split("|");
            if (!linkList[0].compare("clone")) {
                cloneWorkspace(linkList[1]);
            }
        }
    }
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
            }
        }
        else {
            QString link = webElement.attribute("id");
            if (webElement.hasClass("workspace")) {
                auto workspace = mWorkspacesMap.value(link);

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
        auto trElement = page()->mainFrame()->hitTestContent(event->pos()).element();
        while (!trElement.isNull() && trElement.tagName() != "TR")
            trElement = trElement.parent();

        if (!trElement.isNull()) {
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
                                                 tr("Clone the workspace"), this);
            cloneAction->setData(QString("clone|%1").arg(trElement.attribute("id")));
            menu->addAction(cloneAction);
            menu->addSeparator();
        }

        auto aboutAction = new QAction(QIcon(":/oxygen/actions/help-about.png"),
                                       tr("About the workspace"), this);
        aboutAction->setData(QString("about|%1").arg(trElement.attribute("id")));
        menu->addAction(aboutAction);
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
        }
    }
}

//==============================================================================
//==============================================================================

void PmrWorkspacesWidget::initialiseWorkspaces(const PMRSupport::PmrWorkspaceList &pWorkspaceList)
{
    // First clear existing workspaces' map

    mWorkspacesMap.clear();

    // We populate `mWorkspacesMap`, a map from a URL to the corresponding
    // `PmrWorkspace` instance by reconciling URLs of `my-workspaces` (on PMR)
    // with those from workspace folders. In doing so folders/URLs that do not
    // correspond to an actual PMR workspace are pruned from the relevant maps.

    // First clear the `owned` flag from the list of URLs with workspace folders

    QMutableMapIterator<QString, QPair<QString, bool> > urlsIterator(mWorkspaceUrls);
    while (urlsIterator.hasNext()) {
        urlsIterator.next();
        urlsIterator.setValue(QPair<QString, bool>(urlsIterator.value().first, false));
    }

    foreach (PMRSupport::PmrWorkspace *workspace, pWorkspaceList) {

        // Remember our workspace so we can find it by URL

        const QString &url = workspace->url();

        mWorkspacesMap.insert(url, workspace);

        // Check if we know its folder and flag it is ours

        if (mWorkspaceUrls.contains(url)) {
            QString path = mWorkspaceUrls.value(url).first;

            mWorkspaceUrls.insert(url, QPair<QString, bool>(path, true));
            workspace->setPath(path);
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
                mWorkspacesMap.insert(url, workspace);
                workspace->setPath(urlsIterator.value().first);
            }
            else {
                mWorkspaceFolders.remove(urlsIterator.value().first);
                urlsIterator.remove();
            }
        }
    }

    // Finally display the list of workspaces

    displayWorkspaces();
}

//==============================================================================

void PmrWorkspacesWidget::refreshWorkspaces(const bool &pScanFolders)
{
    if (pScanFolders) scanDefaultWorkspaceDirectory();

    mPmrRepository->requestWorkspacesList();
}

//==============================================================================

void PmrWorkspacesWidget::aboutWorkspace(const QString &pUrl)
{
    Q_UNUSED(pUrl);
}

//==============================================================================

void PmrWorkspacesWidget::cloneWorkspace(const QString &pUrl)
{
    auto workspace = mWorkspacesMap.value(pUrl);

    if (workspace && !(workspace->isNull() || workspace->isLocal())) {

        auto workspacesFolder = QDir(PMRSupport::PmrWorkspace::WorkspacesDirectory());
        if (!workspacesFolder.exists()) {
            workspacesFolder.mkpath(".");
        }

        auto path = workspacesFolder.absolutePath() + QDir::separator()
                                                    + workspace->name().trimmed();

        auto dirName = Core::getExistingDirectory(tr("Select Empty Directory"), path, true);
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

        if (!mWorkspacesMap.contains(url))
            addWorkspace(pWorkspace);

        // Redisplay with workspace expanded and selected

        mExpandedItems.insert(url);
        displayWorkspaces();
        setSelected(url);
    }
}

//==============================================================================

}   // namespace PMRWorkspaces
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
