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

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHeaderView>
#include <QHelpEvent>
#include <QSettings>
#include <QStandardPaths>
#include <QUrl>
#include <QWebElement>
#include <QWebFrame>

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspaces {

//==============================================================================

PmrWorkspacesWidget::PmrWorkspacesWidget(PMRSupport::PmrRepository *pPmrRepository, QWidget *pParent) :
    WebViewerWidget::WebViewerWidget(pParent),
    Core::CommonWidget(),
    mPmrRepository(pPmrRepository),
    mExpandedItems(QMap<QString, bool>()),
    mSelectedUrl(QString())
{
    // Prevent objects from being dropped on us

    setAcceptDrops(false);

    // Some connections to handle the clicking and hovering of a link

    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    connect(page(), SIGNAL(linkClicked(const QUrl &)),
            this, SLOT(linkClicked()));
    connect(page(), SIGNAL(linkHovered(const QString &, const QString &, const QString &)),
            this, SLOT(linkHovered()));

    // Retrieve the output template

    QByteArray fileContents;

    Core::readFileContentsFromFile(":/PMRWorkspaces/output.html", fileContents);

    mTemplate = QString(fileContents).arg( // about, clone, close, open
                                          Core::iconDataUri(":/oxygen/actions/help-about.png", 16, 16),
                                          Core::iconDataUri(":/oxygen/actions/document-open-remote.png", 16, 16),
                                          Core::iconDataUri(":/PMRWorkspaces/icons/workspace-open.png", 16, 16),
                                          Core::iconDataUri(":/PMRWorkspaces/icons/workspace-closed.png", 16, 16),
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
static const auto SettingsExpandedItems = QStringLiteral("ExpandedItems");
static const auto SettingsSelectedUrl   = QStringLiteral("SelectedUrl");

//==============================================================================

void PmrWorkspacesWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the names of expanded workspaces and folders

    mExpandedItems.clear();
    pSettings->beginGroup(SettingsWorkspaces);
        QStringList expanded = pSettings->value(SettingsExpandedItems).toStringList();
        QString item;
        foreach (item, expanded) mExpandedItems.insert(item, true);
    pSettings->endGroup();

    // Retrieve the currently selected item, if any

    mSelectedUrl = pSettings->value(SettingsSelectedUrl).toString();
}

//==============================================================================

void PmrWorkspacesWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the names of expanded workspaces and folders

    pSettings->remove(SettingsWorkspaces);
    pSettings->beginGroup(SettingsWorkspaces);
        pSettings->setValue(SettingsExpandedItems, mExpandedItems.key(true));
    pSettings->endGroup();

    // Keep track of the currently selected item

    pSettings->setValue(SettingsSelectedUrl, mSelectedUrl);
}

//==============================================================================
//==============================================================================

void PmrWorkspacesWidget::setSelected(const QString &pSelectedUrl)
{
    mSelectedUrl = pSelectedUrl;
}

//==============================================================================

// This needs to go into pmrworkspaces.cpp ...

QString PmrWorkspacesWidget::workspacePath(const QString &pUrl, const QString &pPath)
{
    Q_UNUSED(pUrl);

    QString workspacePath = PMRSupport::PmrWorkspace::WorkspacesDirectory + QDir::separator() + pPath;
qDebug() << "Checking: " << pUrl << "  Local: " << workspacePath;
    QDir workspaceDirectory = QDir(workspacePath);
    if (!workspacePath.isEmpty() && workspaceDirectory.exists()) {
        return workspaceDirectory.absolutePath();

    }
    return "";
}

//==============================================================================

QString PmrWorkspacesWidget::actionHtml(const QList<QPair<QString, QString> > &pActions)
{
    QPair<QString, QString> action;
    QStringList actions;
    foreach (action, pActions)
        actions << QString("<a href=\"%1|%2\"><img class=\"%1\" /></a>").arg(action.first, action.second);

    return actions.join("");
}

//==============================================================================

QString PmrWorkspacesWidget::containerHtml(const QString &pClass, const QString &pIcon,
                                           const QString &pId, const QString &pName,
                                           const QString &pStatus,
                                           const QList<QPair<QString, QString> > &pActions)
{
    static const QString html = "<tr class=\"%1\" id=\"%2\">\n"
                                "  <td class=\"icon\"><img class=\"%3\" /></td>\n" /* tr.CLASS td.icon img.clone/open/close { } */
                                "  <td class=\"name\">%4</td>\n"
                                "  <td class=\"status\">%5</td>\n"
                                "  <td class=\"action\">%6</td>\n"
                                "</tr>\n";
qDebug() << pClass << "container for " << pName << " is " << pIcon;
    return html.arg(pClass, pId, pIcon, pName, pStatus, actionHtml(pActions));
}

//==============================================================================

QString PmrWorkspacesWidget::fileHtml(const QString &pName, const QString &pStatus,
                                      const QList<QPair<QString, QString> > &pActions)
{
    static const QString html = "<tr>\n"
                                "  <td colspan=\"2\">%1</td>\n"
                                "  <td class=\"status\">%2</td>\n"
                                "  <td class=\"action\">%3</td>\n"
                                "</tr>\n";
qDebug() << "File " << pName;
static bool nomod = false;
nomod = !nomod;
    return html.arg(pName, nomod ? pStatus : "M", actionHtml(pActions));
}

//==============================================================================

QString PmrWorkspacesWidget::emptyContentsHtml(void)
{
    static const QString html = "<tr></tr>\n";

    return html;
}

//==============================================================================

QString PmrWorkspacesWidget::contentsHtml(const QString &pPath) const
{
    static const QString html = "<tr>\n"
                                "  <td></td>\n"
                                "  <td colspan=\"3\">\n"
                                "    <table>\n"
                                "      <tr class=\"packer\">\n"
                                "        <td class=\"icon\"><img /></td>\n"
                                "        <td class=\"name\"></td>\n"
                                "        <td class=\"status\"><img /></td>\n"
                                "        <td class=\"action\"><img /></td>\n"
                                "      </tr>\n"
                                "      %1\n"
                                "    </table>\n"
                                "  </td>\n"
                                "</tr>\n";

    QStringList itemHtml;

    QDir directory = QDir(pPath);
    if (directory.exists()) {
        QFileInfo info;
        foreach(info, directory.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name)) {
            if (info.isDir()) itemHtml << folderHtml(info.absoluteFilePath());
            else              itemHtml << fileHtml(info.fileName(), "", QList<QPair<QString, QString> >()); // status, actions
        }
    }

    return html.arg(itemHtml.join("\n"));
}

//==============================================================================


QStringList PmrWorkspacesWidget::workspaceHtml(const QString &pUrl, const QString &pName, const QString &pPath) const
{
    QString icon = pPath.isEmpty()               ? "clone"
                 : mExpandedItems.contains(pUrl) ? "close"
                 :                                 "open";

    auto actionList = QList<QPair<QString, QString> >()
                    << QPair<QString, QString>("about", pUrl);
    QStringList html = QStringList(containerHtml("workspace", icon, pUrl, pName, "", actionList));

    if (!pPath.isEmpty() /*&& mExpandedItems.contains(pUrl)*/) html << contentsHtml(pPath);
    else                                                   html << emptyContentsHtml();

    return html;
}

//==============================================================================

QStringList PmrWorkspacesWidget::folderHtml(const QString &pPath) const
{
    QFileInfo info = QFileInfo(pPath);
    QString fullname = info.absoluteFilePath();

    QString icon = mExpandedItems.contains(fullname) ? "close"
                 :                                     "open";

    QStringList html = QStringList(containerHtml("folder", icon, fullname, info.fileName(), "", QList<QPair<QString, QString> >())); // status, actions

    if (true /* mExpandedItems.contains(fullname) */) html << contentsHtml(pPath);
    else                                   html << emptyContentsHtml();
qDebug() << "Folder " << pPath << "  Name: " << fullname;
    return html;
}

//==============================================================================

void PmrWorkspacesWidget::clearWorkspaces(void)
{
//    mWorkspaceNames = QStringList();
//    mWorkspaceUrlId = QMap<QString, int>();

    setHtml(mTemplate.arg(QString()));
}

//==============================================================================

void PmrWorkspacesWidget::initialiseWorkspaces(const PMRSupport::PmrWorkspaceList &pWorkspaceList)
{
/*

    A workspace view is either closed or open (= contents visible).
    Contents are an alphabetical list of non-hidden files and folders.
    A folder may be closed or open. If open then show its contents.

        * workspace identified by its URL
        * file/folder (within a workspace) identified by its full path

    Operations:

        1. Load settings (open/closed/selection)
        2. When list of workspaces received (from PMR), update settings as details are displayed.
            a. If workspace is known then get its state, otherwise add workspace with state closed.
            b. If open then get (top-level) files/folders from file system.
                i. For each folder check if known and get its state or add folder with state closed.
                ii. If folder open then recurse.
            c. Highlight selected item if it exists.
            d. Remove all workspaces/folders that no longer exist.
        3. Save settings.

    Need list of expanded items. Is there a need to prune this list and remove unknown items? Maybe only
    when saving?? In which case we need QMap<QString, bool>.

    Save just the identifiers; there's no need to group them -- it's just a list of expanded items.

*/
//    mWorkspaceNames = QStringList();
//    mWorkspaceUrlId = QMap<QString, int>();

    QStringList html;

    foreach (PMRSupport::PmrWorkspace *workspace, pWorkspaceList) {
        QString url = workspace->url();
        QString name = workspace->name();

        // Want to have a LocalWorkspace...
        QString path = workspacePath(url, name);

        html.append(workspaceHtml(url, name, path));
        }
//qDebug() << mTemplate.arg(html.join("\n"));
    setHtml(mTemplate.arg(html.join("\n")));
    // Can we position view so that the selected line is shown??
}

//==============================================================================

/***
initially:
  <tr>
    folder/workspace
  </tr>
  <tr></tr>

OR:

  <tr>
    folder/workspace
  </tr>
  <tr>
    CONTENTS
  </tr>

tr[#ID].nextSibling



Close will simply set display none in DOM tree. Open needs to remove any existing
non-displayed TR and add a new one.

Is it then best to always have a <TABLE> element that gets rebuilt in the DOM??

            <tr>
              <td colspan="2">first file</td>
              <td class="status">?</td>
              <td class="action">?</td>
            </tr>


            <tr class="folder">
              <td class="icon"><a href="folderClicked|ID"><img class="icon" src="folder-open.png"/></a></td>
              <td class="name">Animals</td>
              <td class="status">x</td>
              <td></td>
            </tr>  <!-- Click closed --> set display none; click open --> refresh from disk -->
            <tr style="display: none;" id="display_ID">
              <td></td>
              <td colspan="2">
                <table>
                  <tr class="packer">
                    <td class="icon"><img src="space30.png" /></td>  <!-- <img class="spacer30"/> -->
                    <td class="name"></td>
                    <td class="status"><img src="space30.png" /></td>
                    <td class="action"><img src="space30.png" /></td>
                  </tr>

                  CONTENTS

                </table>
              </td>
            </tr>


***/

//==============================================================================

void PmrWorkspacesWidget::refreshWorkspaces(void)
{
    mPmrRepository->requestWorkspacesList();
}

//==============================================================================


void PmrWorkspacesWidget::linkClicked()
{
    // Retrieve some information about the link

    QString link;
    QString textContent;

    QWebElement element = retrieveLinkInformation(link, textContent);

    // Check whether we have clicked a text or button link

    if (textContent.isEmpty()) {
        // We have clicked on a button link, so let people know whether we want
        // to clone a workspace or whether we want to show/hide exposure files

        QStringList linkList = link.split("|");

        if (!linkList[0].compare("about")) {
            mPmrRepository->requestWorkspaceInformation(linkList[1]);
            // And somewhere connect workspaceInformation SIGNAL to display result
        }
    }
}

//==============================================================================

void PmrWorkspacesWidget::linkHovered()
{
    // Retrieve some information about the link

    QString link;
    QString textContent;

    QWebElement element = retrieveLinkInformation(link, textContent);

    // Update our tool tip based on whether we are hovering a text or button
    // link
    // Note: this follows the approach used in linkClicked()...

    QString linkToolTip = QString();

    if (textContent.isEmpty()) {
        QStringList linkList = link.split("|");

        if (!linkList[0].compare("about")) {
            linkToolTip = tr("About the Workspace");
        }
    }

    setLinkToolTip(linkToolTip);
void PmrWorkspacesWidget::workspaceCreated(const QString &pUrl)
{
    Q_UNUSED(pUrl)

    // Ignore as `workspaceCloned` will be called next
}

//==============================================================================

/***************

Add workspace:

Post to https://models.physiomeproject.org/workspace/+/addWorkspace
Fields: form-widgets-title, form-widgets-description, form-widgets-storage (= git)

JSON:  "{
    "collection": {
        "href": "https://models.physiomeproject.org/workspace/addWorkspace",
        "template": {
            "data": [
                {
                    "description": "",
                    "name": "form.widgets.title",
                    "options": null,
                    "prompt": "Title",
                    "required": false,
                    "type": "TextLine",
                    "value": ""
                },
                {
                    "description": "",
                    "name": "form.widgets.description",
                    "options": null,
                    "prompt": "Description",
                    "required": false,
                    "type": "Text",
                    "value": ""
                },
                {
                    "description": "The type of storage backend used for this workspace.",
                    "name": "form.widgets.storage",
                    "options": [
                        {
                            "text": "Git",
                            "value": "git"
                        }
                    ],
                    "prompt": "Storage Method",
                    "required": true,
                    "type": "Choice",
                    "value": [
                    ]
                },
                {
                    "description": null,
                    "name": "form.buttons.add",
                    "prompt": "Add",
                    "required": false,
                    "type": "Button",
                    "value": null
                }
            ]
        },
        "version": "1.0"
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
"

*************/

//==============================================================================

/*
void PmrWorkspacesWidget::dragEnterEvent(QDragEnterEvent *pEvent)
{
    // Accept the proposed action for the event, but only if we are dropping
    // URIs

    if (   (pEvent->mimeData()->hasFormat(Core::FileSystemMimeType))
        || (pEvent->mimeData()->hasFormat(PmrWorkspacesMimeType))) {
        pEvent->acceptProposedAction();
    } else {
        pEvent->ignore();
    }
}

//==============================================================================

void PmrWorkspacesWidget::dragMoveEvent(QDragMoveEvent *pEvent)
{
    // Set the state to dragging, since we can only come here when dragging
    // Note: the state is properly set for workspaces objects being dragged,
    //       but should we be dragging external objects over our workspaces
    //       widget, then the state will (obviously) not be set. This wouldn't
    //       be a problem in itself if it was for the fact that this prevents
    //       the drop indicator from being painted...

    setState(QAbstractItemView::DraggingState);

    // Default handling of the event
    // Note: this gives us the drop indicator...

    TreeViewWidget::dragMoveEvent(pEvent);

    // Accept the proposed action for the event, but only if there are objects
    // to drop and if we are not trying to drop the objects above/on/below one
    // of them (should the objects come from the workspaces widget) or on a
    // file item
    // Note #1: for the number of objects being dropped, we have to check the
    //          number of URLs information (i.e. external objects), as well as
    //          the MIME data associated with PmrWorkspacesMimeType (i.e.
    //          objects from the workspaces widget, after we have )
    // Note #2: for the dropping location, it can be either a folder or a file
    //          (as long as the indicator position isn't on the item itself),
    //          but not above/on/below any of the objects (or any of their
    //          children) being dragged (only relevant when dragging items from
    //          the workspaces widget)

    QByteArray data = pEvent->mimeData()->data(PmrWorkspacesMimeType);
    QModelIndexList indexes = mModel->decodeData(data);
    QStandardItem *dropItem = mModel->itemFromIndex(indexAt(pEvent->pos()));
    bool draggingOnSelfOrChild = false;

    if (dropItem)
        for (int i = 0; (i < indexes.count()) && !draggingOnSelfOrChild; ++i)
            draggingOnSelfOrChild = itemIsOrIsChildOf(dropItem, mModel->itemFromIndex(indexes[i]));

    if (   (pEvent->mimeData()->urls().count() || indexes.count())
        && (   (dropItem && dropItem->data(Item::Folder).toBool())
            || (dropIndicatorPosition() != QAbstractItemView::OnItem))
        && !draggingOnSelfOrChild) {
        pEvent->acceptProposedAction();
    } else {
        pEvent->ignore();
    }
}

//==============================================================================

void PmrWorkspacesWidget::dropEvent(QDropEvent *pEvent)
{
    // Note: the MIME data definitely contains the FileSystemMimeType MIME type
    //       (for objects originating from outside this widget), but it may also
    //       contain the PmrWorkspacesMimeType MIME type (for objects
    //       originating from within this widget). PmrWorkspacesMimeType
    //       is used by this widget while FileSystemMimeType by external
    //       widgets. So, this means that we must check for
    //       PmrWorkspacesMimeType first

    // Files have been dropped, so add them to the widget and this at the right
    // place (i.e. above/on/below a folder, above/below a file or on the
    // invisible root folder)

    // First, determine the item above/on/below which objects are to be dropped,
    // as well as the drop position (i.e. above, on or below)

    QStandardItem *dropItem;
    DropIndicatorPosition dropPosition = dropIndicatorPosition();

    if (dropPosition == QAbstractItemView::OnViewport) {
        // We dropped the files on the viewport

        dropItem = mModel->invisibleRootItem();

        // Change the drop position since we know that we want want the objects
        // to be dropped on the root folder

        dropPosition = QAbstractItemView::OnItem;
    } else {
        // We dropped the files above/on/below a folder or above/below a file

        dropItem = mModel->itemFromIndex(indexAt(pEvent->pos()));
    }

    // Check the type of MIME data to be dropped

    if (pEvent->mimeData()->hasFormat(PmrWorkspacesMimeType)) {
        // The user is dropping folders/files from ourselves, i.e. s/he wants
        // some folders/files to be moved around

        // Retrieve the list of indexes to move around and clean it

        QByteArray data = pEvent->mimeData()->data(PmrWorkspacesMimeType);
        QModelIndexList indexes = cleanIndexList(mModel->decodeData(data));

        // Convert our list of indexes to a list of items
        // Note: indeed, by moving the item corresponding to a particular index,
        //       we may mess up the other indexes, meaning that we may not be
        //       able to retrieve their corresponding item...

        QList<QStandardItem *> items;

        for (int i = 0, iMax = indexes.count(); i < iMax; ++i)
            items << mModel->itemFromIndex(indexes[i]);

        // Move the contents of the list to its final destination

        if (dropPosition != QAbstractItemView::BelowItem) {
            // Move the items in the order they were dropped

            for (int i = 0, iMax = items.count(); i < iMax; ++i)
                moveItem(items[i], dropItem, dropPosition);
        } else {
            // Move the items in a reverse order to that they were dropped since
            // we want them moved below the current item

            for (int i = items.count()-1; i >= 0; --i)
                moveItem(items[i], dropItem, dropPosition);
        }
    } else {
        // The user wants to drop files, so add them to the widget and this at
        // the right place

        QList<QUrl> urls = pEvent->mimeData()->urls();

        if (dropPosition != QAbstractItemView::BelowItem) {
            // Add the files in the order they were dropped

            for (int i = 0, iMax = urls.count(); i < iMax; ++i)
                addFile(urls[i].toLocalFile(), dropItem, dropPosition);
        } else {
            // Add the files in a reverse order to that they were dropped since
            // we want them added below the current item

            for (int i = urls.count()-1; i >= 0; --i)
                addFile(urls[i].toLocalFile(), dropItem, dropPosition);
        }
    }

    // Accept the proposed action for the event

    pEvent->acceptProposedAction();

    // Reset the state of the widget
    // Note: there doesn't seem to be and there shouldn't be a need to reset the
    //       state after the widget (the resetting seems to be done elsewhere),
    //       but then if we don't reset the state of the widget, then the drop
    //       indicator may, in some cases, remain visible (a bug in Qt?)...

    setState(QAbstractItemView::NoState);
}

//==============================================================================

void PmrWorkspacesWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Default handling of the event

    TreeViewWidget::keyPressEvent(pEvent);

    // Let people know about a key having been pressed with the view of opening
    // one or several files

    QStringList crtSelectedFiles = selectedFiles();

    if (   crtSelectedFiles.count()
        && ((pEvent->key() == Qt::Key_Enter) || (pEvent->key() == Qt::Key_Return))) {
        // There are some files that are selected and we want to open them, so
        // let people know about it

        emit filesOpenRequested(crtSelectedFiles);
    }
}

//==============================================================================

bool PmrWorkspacesWidget::viewportEvent(QEvent *pEvent)
{
    if (pEvent->type() == QEvent::ToolTip) {
        // We need to show a tool tip, so make sure that it's up to date by
        // setting it to the path of the current file item or to nothing if we
        // are dealing with a folder item

        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(pEvent);
        QStandardItem *crtItem = mModel->itemFromIndex(indexAt(helpEvent->pos()));

        if (crtItem) {
            setToolTip(Core::nativeCanonicalDirName(crtItem->data(Item::Folder).toBool()?
                                                         QString():
                                                         crtItem->data(Item::Path).toString()));
        }
    }

    // Default handling of the event

    return TreeViewWidget::viewportEvent(pEvent);
}

//==============================================================================


//==============================================================================

QString PmrWorkspacesWidget::newFolderName(QStandardItem *pFolderItem) const
{
    // Come up with the name for a new folder which is to be under pFolderItem

    // Retrieve the name of the folders under pFolderItem

    QStringList subFolderNames;

    for (int i = 0, iMax = pFolderItem->rowCount(); i < iMax; ++i)
        subFolderNames << pFolderItem->child(i)->text();

    // Compare the suggested name of our new folder with that of the folders
    // under pFolderItem, this until we come up with a suggested name which is
    // not already taken

    QString baseFolderName = tr("New Folder");
    QString templateFolderName = baseFolderName+" (%1)";

    int folderNb = 1;
    QString res = baseFolderName;

    while (subFolderNames.contains(res))
        res = templateFolderName.arg(++folderNb);

    return res;
}

//==============================================================================

void PmrWorkspacesWidget::newFolder()
{
    // Make sure that the conditions are met to create a new folder

    if (!canCreateNewFolder())
        return;

    // Either create a folder item below the current folder item or below the
    // root item, depending on the situation

    static const QIcon CollapsedFolderIcon = QIcon(":oxygen/places/folder.png");

    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();
    int selectedIndexesCount = selectedIndexes.count();
    QStandardItem *crtItem = !selectedIndexesCount?
                                mModel->invisibleRootItem():
                                mModel->itemFromIndex(selectedIndexes.first());
    QStandardItem *newFolderItem = new QStandardItem(CollapsedFolderIcon,
                                                     newFolderName(crtItem));

    newFolderItem->setData(true, Item::Folder);

    crtItem->appendRow(newFolderItem);

    // Some post-processing, but only if no other item is currently being edited

    if (!isEditing()) {
        // Expand the current index (so that we can see the new folder)
        // Note: this is only relevant in the case of a folder item being
        //       currently selected (i.e. it's not the root folder item)

        if (selectedIndexesCount)
            setExpanded(crtItem->index(), true);

        // Offer the user to edit the newly added folder item

        edit(newFolderItem->index());
    }

    // Resize the widget, just to be on the safe side

    resizeToContents();
}

//==============================================================================

QStandardItem * PmrWorkspacesWidget::parentItem(QStandardItem *pDropItem,
                                                      const QAbstractItemView::DropIndicatorPosition &pDropPosition)
{
    // Return who should be the parent item of pDropItem based on pDropPosition

    return (pDropPosition == QAbstractItemView::OnItem)?
                pDropItem:
                pDropItem->parent()?
                    pDropItem->parent():
                    mModel->invisibleRootItem();
}

//==============================================================================

bool PmrWorkspacesWidget::ownedBy(const QString &pFileName,
                                        QStandardItem *pItem)
{
    // Check whether pFileName is already owned by pItem

    for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i) {
        // Check whether the current item is a file with pFileName as its name

        QStandardItem *crtItem = pItem->child(i);

        if (   !crtItem->data(Item::Folder).toBool()
            && (crtItem->data(Item::Path).toString() == pFileName)) {
            return true;
        }
    }

    return false;
}

//==============================================================================

void PmrWorkspacesWidget::dropItems(QStandardItem *pDropItem,
                                          const QAbstractItemView::DropIndicatorPosition &pDropPosition,
                                          QStandardItem *pNewParentItem,
                                          QList<QStandardItem *> pItems)
{
    // Drop pItems based on pDropPosition's value

    switch (pDropPosition) {
    case QAbstractItemView::AboveItem:
        pNewParentItem->insertRow(pDropItem->row(), pItems);

        break;
    case QAbstractItemView::BelowItem:
        pNewParentItem->insertRow(pDropItem->row()+1, pItems);

        break;
    default:
        pNewParentItem->appendRow(pItems);

        // Expand pNewParentItem, so the user knows that the item has been moved
        // to it (assuming that pNewParentItem was collapsed)

        setExpanded(pNewParentItem->index(), true);
    }
}

//==============================================================================

void PmrWorkspacesWidget::dropItem(QStandardItem *pDropItem,
                                         const QAbstractItemView::DropIndicatorPosition &pDropPosition,
                                         QStandardItem *pNewParentItem,
                                         QStandardItem *pItem)
{
    // Drop the item as if we wanted to drop a list of items

    dropItems(pDropItem, pDropPosition, pNewParentItem,
              QList<QStandardItem *>() << pItem);
}

//==============================================================================

void PmrWorkspacesWidget::addFile(const QString &pFileName,
                                        QStandardItem *pDropItem,
                                        const QAbstractItemView::DropIndicatorPosition &pDropPosition)
{
    // Make sure that we have a drop item

    if (!pDropItem)
        return;

    // Make sure that we are indeed dealing with a file

    QFileInfo fileInfo = pFileName;

    if (!fileInfo.isFile())
        return;

    // Check whether we are dropping a symbolic link

    QString fileName = pFileName;

    if (fileInfo.isSymLink())
        // We are dropping a symbolic link, so retrieve its target

        fileName = fileInfo.symLinkTarget();

    // Check whether the file exists

    if (QFile::exists(fileName)) {
        // The target file exists, so add it above/on/below pDropItem, depending
        // on the drop position and only if the file isn't already present

        // First, determine the item that will own the file

        QStandardItem *newParentItem = parentItem(pDropItem, pDropPosition);

        // Second, if the file is not already owned, then add it to
        // newParentItem and this to the right place, depending on the value of
        // pDropPosition

        if (!ownedBy(fileName, newParentItem)) {
            static const QIcon FileIcon = QIcon(":oxygen/mimetypes/application-x-zerosize.png");

            QStandardItem *newFileItem = new QStandardItem(FileIcon,
                                                           QFileInfo(fileName).fileName());

            newFileItem->setData(fileName, Item::Path);

            dropItem(pDropItem, pDropPosition, newParentItem, newFileItem);

            // Add the file to our file manager
            // Note: it doesn't matter whether or not the file is already being
            //       monitored, since if that's the case then the current
            //       instance will be ignored

            mFileManager->manage(fileName);

            // Resize the widget, just in case the new file takes more space
            // than is visible

            resizeToContents();
        }
    }
}

//==============================================================================

void PmrWorkspacesWidget::moveItem(QStandardItem *pItem,
                                         QStandardItem *pDropItem,
                                         const QAbstractItemView::DropIndicatorPosition &pDropPosition)
{
    // Make sure that we have a drop item

    if (!pDropItem)
        return;

    // Move pItem above/on/below pDropItem, depending on the drop position, but
    // first, determine the item that will own pItem

    QStandardItem *crtParentItem = pItem->parent()?
                                       pItem->parent():
                                       mModel->invisibleRootItem();
    QStandardItem *newParentItem = parentItem(pDropItem, pDropPosition);

    // Second, check whether the (file) item points to a file which is already
    // owned by newParentItem

    bool fileAlreadyOwned = false;

    if (!pItem->data(Item::Folder).toBool())
        // The current item is a file item, so retrieve its file name and check
        // whether it's already owned by newParentItem

        fileAlreadyOwned = ownedBy(pItem->data(Item::Path).toString(), newParentItem);

    // Third, move pItem to newParentItem and this to the right place, depending
    // on the value of pDropPosition and only if the destination doesn't already
    // have that item (should it be a file item, since folder items are always
    // moved)

    if (!fileAlreadyOwned || (crtParentItem == newParentItem)) {
        // Either newParentItem doesn't already own an item which points to the
        // same file as pItem or pItem's current parent is the same as
        // newParentItem in which case it means that we want to move the item
        // within its current location

        // First, check whether the item is a folder and, if so, whether it's
        // expanded (and the same with any (in)direct child folder it may
        // contain)

        backupExpandedInformation(pItem);

        // Second, move the item (and any of its children)

        dropItems(pDropItem, pDropPosition, newParentItem,
                  crtParentItem->takeRow(pItem->row()));

        // Third, re-expand folders, if necessary

        restoreExpandedInformation(pItem);

        // Fourth, resize the widget, just in case the new location of the
        // item(s) requires more space than is visible

        resizeToContents();
    } else {
        // A (file) item pointing to the same file is already owned by
        // newParentItem, so just remove the item rather than move it

        crtParentItem->removeRow(pItem->row());
    }
}

//==============================================================================

void PmrWorkspacesWidget::collapseEmptyFolders(QStandardItem *pFolder)
{
    // Recursively collapse any empty child folder

    for (int i = 0, iMax = pFolder->rowCount(); i < iMax; ++i) {
        if (pFolder->child(i)->data(Item::Folder).toBool())
            collapseEmptyFolders(pFolder->child(i));
    }

    // Collapse the current folder, if necessary and if it isn't the root folder

    if ((pFolder != mModel->invisibleRootItem()) && !pFolder->rowCount())
        collapse(mModel->indexFromItem(pFolder));
}

//==============================================================================

void PmrWorkspacesWidget::deleteItems()
{
    // Check whether whether there are selected items to delete

    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();

    if (selectedIndexes.isEmpty())
        return;

    // Delete the selected items one by one, making sure that we update our list
    // of items every time (this is because the row value of the remaining
    // selected items may become different after deleting an item)

    while (!selectedIndexes.isEmpty()) {
        // Remove the file from our file manager, should the index refer to a
        // file item
        // Note: it doesn't matter whether or not the file has already been
        //       removed, since if that's the case then nothing will be done...

        QModelIndex crtIndex = selectedIndexes.first();
        QStandardItem *crtItem = mModel->itemFromIndex(crtIndex);

        if (crtItem && !crtItem->data(Item::Folder).toBool())
            mFileManager->unmanage(crtItem->data(Item::Path).toString());

        // Remove the item from the model itself

        mModel->removeRow(crtIndex.row(), crtIndex.parent());

        // Update our list of selected indexes

        selectedIndexes = selectionModel()->selectedIndexes();
    }

    // Collapse any folder that doesn't contain any file/folder anymore

    collapseEmptyFolders(mModel->invisibleRootItem());

    // Resize the widget to its contents in case its width was too wide (and
    // therefore required a horizontal scrollbar), but is now fine

    resizeToContents();
}

//==============================================================================

QString PmrWorkspacesWidget::filePath(const QModelIndex &pFileIndex) const
{
    // Return the file path of pFileIndex

    return mModel->filePath(pFileIndex);
}

//==============================================================================

void PmrWorkspacesWidget::resizeToContents()
{
    // Make sure that the first column allows for all of its contents to be
    // visible

    resizeColumnToContents(0);
}

//==============================================================================

QStringList PmrWorkspacesWidget::selectedFiles() const
{
    // Retrieve all the files that are currently selected
    // Note: if there is a folder among the selected items, then we return an
    //       empty list

    QStringList res;
    QModelIndexList crtSelectedIndexes = selectedIndexes();

    for (int i = 0, iMax = crtSelectedIndexes.count(); i < iMax; ++i) {
        QString fileName = filePath(crtSelectedIndexes[i]);

        if (fileName.isEmpty())
            return QStringList();
        else
            res << fileName;
    }

    return res;
}

//==============================================================================

void PmrWorkspacesWidget::expandedFolder(const QModelIndex &pFolderIndex)
{
    // The folder is being expanded, so update its icon to reflect its new state

    static const QIcon ExpandedFolderIcon = QIcon(":oxygen/actions/document-open-folder.png");

    mModel->itemFromIndex(pFolderIndex)->setIcon(ExpandedFolderIcon);

    // Resize the widget, just to be on the safe side

    resizeToContents();
}

//==============================================================================

void PmrWorkspacesWidget::collapsedFolder(const QModelIndex &pFolderIndex)
{
    // The folder is being expanded, so update its icon to reflect its new state

    static const QIcon CollapsedFolderIcon = QIcon(":oxygen/places/folder.png");

    mModel->itemFromIndex(pFolderIndex)->setIcon(CollapsedFolderIcon);

    // Resize the widget, just to be on the safe side

    resizeToContents();
}

//==============================================================================

bool PmrWorkspacesWidget::canCreateNewFolder() const
{
    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();
    int selectedIndexesCount = selectedIndexes.count();

    // Determnine whether we can create a new folder

    if (selectedIndexesCount == 1) {
        // One item is currently selected, so the only way we could create a new
        // folder is if the current item is also a folder

        return mModel->itemFromIndex(selectedIndexes.first())->data(Item::Folder).toBool();
    } else {
        // Either no item or several items are currently selected, so the only
        // way we could create a new folder is if no item is currently selected

        return !selectedIndexesCount;
    }
}

//==============================================================================

void PmrWorkspacesWidget::emitItemsRelatedSignals()
{
    // Let the user know whether a new folder could be created and whether items
    // could be deleted

    emit newFolderEnabled(canCreateNewFolder());
    emit deleteItemsEnabled(selectionModel()->selectedIndexes().count() >= 1);
}

//==============================================================================

void PmrWorkspacesWidget::updateFileItems(QStandardItem *pItem,
                                                const QString &pFileName,
                                                const Core::File::Status &pStatus) const
{
    // Recursively update the icon of all file items that refer to pFileName

    if (   !pItem->data(Item::Folder).toBool()
        && !pItem->data(Item::Path).toString().compare(pFileName)) {
        // The current item is a file item and it refers to pFileName, so update
        // its icon based on the value of pStatus

        static const QIcon FileIcon        = QIcon(":oxygen/mimetypes/application-x-zerosize.png");
        static const QIcon DeletedFileIcon = QIcon(":oxygen/status/image-missing.png");

        pItem->setIcon((pStatus == Core::File::Deleted)?DeletedFileIcon:FileIcon);
    }

    // Update our child file items, if any

    for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i)
        updateFileItems(pItem->child(i), pFileName, pStatus);
}

//==============================================================================

void PmrWorkspacesWidget::fileChanged(const QString &pFileName) const
{
    // The file has changed, which may also mean that a file may have been
    // deleted and recreated, so go through all the (file) items and update the
    // icon of the ones that refer to the file in question

    updateFileItems(mModel->invisibleRootItem(), pFileName,
                    Core::File::Changed);
}

//==============================================================================

void PmrWorkspacesWidget::fileDeleted(const QString &pFileName) const
{
    // A file has been deleted, so go through all the (file) items and update
    // the icon of the ones that refer to the file in question

    updateFileItems(mModel->invisibleRootItem(), pFileName,
                    Core::File::Deleted);
}

//==============================================================================
**/

}   // namespace PMRWorkspaces
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
