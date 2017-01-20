/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// PMR Workspaces window widget
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "i18ninterface.h"
#include "pmrwebservice.h"
#include "pmrworkspacemanager.h"
#include "pmrworkspaceswindowcommitdialog.h"
#include "pmrworkspaceswindowwidget.h"
#include "treeviewwidget.h"
#include "usermessagewidget.h"

//==============================================================================

#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QDir>
#include <QFileIconProvider>
#include <QKeyEvent>
#include <QLayout>
#include <QMainWindow>
#include <QMenu>
#include <QSettings>
#include <QTimer>

//==============================================================================

#include "git2/remote.h"
#include "git2/repository.h"

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspacesWindow {

//==============================================================================

void PmrWorkspacesWindowItem::constructor(const Type &pType,
                                          const QIcon &pCollapsedIcon,
                                          const QIcon &pExpandedIcon,
                                          const QIcon &pIcon,
                                          PMRSupport::PmrWorkspace *pWorkspace,
                                          PMRSupport::PmrWorkspaceFileNode *pFileNode)
{
    // Some initialisations

    mType = pType;
    mWorkspace = pWorkspace;
    mFileNode = pFileNode;
    mCollapsedIcon = pCollapsedIcon;
    mExpandedIcon = pExpandedIcon;

    // Customise ourselves

    QStandardItem::setIcon(pCollapsedIcon.isNull()?pIcon:pCollapsedIcon);

    setToolTip(text());
}

//==============================================================================

PmrWorkspacesWindowItem::PmrWorkspacesWindowItem(const Type &pType,
                                                 const QIcon &pCollapsedIcon,
                                                 const QIcon &pExpandedIcon,
                                                 const QString &pText,
                                                 PMRSupport::PmrWorkspace *pWorkspace) :
    QStandardItem(pText)
{
    // Construct our object

    constructor(pType, pCollapsedIcon, pExpandedIcon, QIcon(), pWorkspace, 0);
}

//==============================================================================

PmrWorkspacesWindowItem::PmrWorkspacesWindowItem(const Type &pType,
                                                 const QIcon &pIcon,
                                                 PMRSupport::PmrWorkspaceFileNode *pFileNode) :
    QStandardItem(pFileNode->name())
{
    // Construct our object

    constructor(pType, QIcon(), QIcon(), pIcon, 0, pFileNode);
}

//==============================================================================

int PmrWorkspacesWindowItem::type() const
{
    // Return our type

    return mType;
}

//==============================================================================

PMRSupport::PmrWorkspace * PmrWorkspacesWindowItem::workspace() const
{
    // Return our workspace

    return mWorkspace;
}

//==============================================================================

PMRSupport::PmrWorkspaceFileNode * PmrWorkspacesWindowItem::fileNode() const
{
    // Return our file node

    return mFileNode;
}

//==============================================================================

QIcon PmrWorkspacesWindowItem::collapsedIcon() const
{
    // Return our collapsed icon

    return mCollapsedIcon;
}

//==============================================================================

QIcon PmrWorkspacesWindowItem::expandedIcon() const
{
    // Return our expanded icon

    return mExpandedIcon;
}

//==============================================================================

QString PmrWorkspacesWindowItem::url() const
{
    // Return our URL

    return mWorkspace?mWorkspace->url():QString();
}

//==============================================================================

QString PmrWorkspacesWindowItem::path() const
{
    // Return our path

    return mWorkspace?mWorkspace->path():QString();
}

//==============================================================================

QString PmrWorkspacesWindowItem::fileName() const
{
    // Return our file name

    return mFileNode?mFileNode->fullName():QString();
}

//==============================================================================

PmrWorkspacesWindowWidget::PmrWorkspacesWindowWidget(PMRSupport::PmrWebService *pPmrWebService,
                                                     QWidget *pParent) :
    Core::Widget(pParent),
    mPmrWebService(pPmrWebService),
    mWorkspaceManager(PMRSupport::PmrWorkspaceManager::instance()),
    mWorkspaceFolderUrls(QMap<QString, QString>()),
    mOwnedWorkspaceUrlFolders(QMap<QString, QPair<QString, bool>>()),
    mInitialized(false),
    mErrorMessage(QString()),
    mAuthenticated(false)
{
    // Create and customise some objects

    mUserMessageWidget = new Core::UserMessageWidget(this);

    mUserMessageWidget->setScale(0.85);

    mTreeViewModel = new QStandardItemModel(this);
    mTreeViewWidget = new Core::TreeViewWidget(this);

    mTreeViewWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    mTreeViewWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mTreeViewWidget->setHeaderHidden(true);
    mTreeViewWidget->setModel(mTreeViewModel);
    mTreeViewWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mTreeViewWidget->setVisible(false);

    connect(mTreeViewWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu(const QPoint &)));

    connect(mTreeViewWidget, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(itemDoubleClicked(const QModelIndex &)));

    connect(mTreeViewWidget, SIGNAL(expanded(const QModelIndex &)),
            this, SLOT(resizeTreeViewToContents()));
    connect(mTreeViewWidget, SIGNAL(expanded(const QModelIndex &)),
            this, SLOT(itemExpanded(const QModelIndex &)));

    connect(mTreeViewWidget, SIGNAL(collapsed(const QModelIndex &)),
            this, SLOT(resizeTreeViewToContents()));
    connect(mTreeViewWidget, SIGNAL(collapsed(const QModelIndex &)),
            this, SLOT(itemCollapsed(const QModelIndex &)));

    // Create our owned workspace icons

    static const QIcon FavoriteIcon = QIcon(":/oxygen/places/favorites.png");

    QIcon collapsedFolderIcon = QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon);
    QIcon expandedFolderIcon = QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon);
    int folderIconSize = collapsedFolderIcon.availableSizes().first().width();
    int overlayIconSize = 0.57*folderIconSize;

    mCollapsedOwnedWorkspaceIcon = Core::overlayedIcon(collapsedFolderIcon,
                                                       FavoriteIcon,
                                                       folderIconSize, folderIconSize,
                                                       folderIconSize-overlayIconSize, folderIconSize-overlayIconSize,
                                                       overlayIconSize, overlayIconSize);
    mExpandedOwnedWorkspaceIcon = Core::overlayedIcon(expandedFolderIcon,
                                                      FavoriteIcon,
                                                      folderIconSize, folderIconSize,
                                                      folderIconSize-overlayIconSize, folderIconSize-overlayIconSize,
                                                      overlayIconSize, overlayIconSize);

    // Populate ourselves

    createLayout();

    layout()->addWidget(mUserMessageWidget);
    layout()->addWidget(mTreeViewWidget);

    // Connections to handle responses from PMR

    connect(mPmrWebService, SIGNAL(workspaceCloned(PMRSupport::PmrWorkspace *)),
            this, SLOT(workspaceCloned(PMRSupport::PmrWorkspace *)));
    connect(mPmrWebService, SIGNAL(workspaceSynchronized(PMRSupport::PmrWorkspace *)),
            this, SLOT(workspaceSynchronized(PMRSupport::PmrWorkspace *)));

    // Connection to handle a response from our workspace manager

    connect(mWorkspaceManager, SIGNAL(workspaceCloned(PMRSupport::PmrWorkspace *)),
            this, SLOT(workspaceCloned(PMRSupport::PmrWorkspace *)));

    // Create a timer for refreshing our workspaces

    mTimer = new QTimer(this);

    connect(mTimer, SIGNAL(timeout()),
            this, SLOT(refreshWorkspaces()));

    // Keep track of when OpenCOR gets/loses the focus, so we can enable/disable
    // the timer that we use to refresh our workspaces
    // Note: the focusWindowChanged() signal comes from QGuiApplication, so we
    //       need to check whether we are running the console version of OpenCOR
    //       or its GUI version...

    if (dynamic_cast<QGuiApplication *>(QCoreApplication::instance())) {
        connect(qApp, SIGNAL(focusWindowChanged(QWindow *)),
                this, SLOT(startStopTimer()));
    }

    // Create and populate our context menu

    static const QIcon ArrowDownIcon = QIcon(":/oxygen/actions/arrow-down.png");

    mContextMenu = new QMenu(this);

    mViewInPmrAction = Core::newAction(QIcon(":/oxygen/categories/applications-internet.png"),
                                       this);
    mViewOncomputerAction = Core::newAction(QIcon(":/oxygen/devices/computer.png"),
                                       this);
    mCopyUrlAction = Core::newAction(QIcon(":/oxygen/actions/edit-copy.png"),
                                     this);
    mCopyPathAction = Core::newAction(QIcon(":/oxygen/actions/edit-copy.png"),
                                      this);
    mCloneAction = Core::newAction(Core::overlayedIcon(collapsedFolderIcon, ArrowDownIcon,
                                                       folderIconSize, folderIconSize,
                                                       folderIconSize-overlayIconSize, folderIconSize-overlayIconSize,
                                                       overlayIconSize, overlayIconSize),
                                            this);
    mAboutAction = Core::newAction(QIcon(":/oxygen/actions/help-about.png"),
                                   this);

    connect(mViewInPmrAction, SIGNAL(triggered(bool)),
            this, SLOT(viewInPmr()));
    connect(mViewOncomputerAction, SIGNAL(triggered(bool)),
            this, SLOT(viewOnComputer()));
    connect(mCopyUrlAction, SIGNAL(triggered(bool)),
            this, SLOT(copyUrl()));
    connect(mCopyPathAction, SIGNAL(triggered(bool)),
            this, SLOT(copyPath()));
    connect(mCloneAction, SIGNAL(triggered(bool)),
            this, SLOT(clone()));
    connect(mAboutAction, SIGNAL(triggered(bool)),
            this, SLOT(about()));

    mContextMenu->addAction(mViewInPmrAction);
    mContextMenu->addAction(mViewOncomputerAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mCopyUrlAction);
    mContextMenu->addAction(mCopyPathAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mCloneAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mAboutAction);

    // Make our tree view widget our focus proxy

    setFocusProxy(mTreeViewWidget);
}

//==============================================================================

PmrWorkspacesWindowWidget::~PmrWorkspacesWindowWidget()
{
    // Delete some internal objects

    delete mTimer;
}

//==============================================================================

void PmrWorkspacesWindowWidget::retranslateUi()
{
    // Retranslate our actions

    I18nInterface::retranslateAction(mViewInPmrAction, tr("View In PMR"),
                                     tr("View in PMR"));
    I18nInterface::retranslateAction(mViewOncomputerAction, tr("View On Computer"),
                                     tr("View on computer"));
    I18nInterface::retranslateAction(mCopyUrlAction, tr("Copy URL"),
                                     tr("Copy the URL to the clipboard"));
    I18nInterface::retranslateAction(mCopyPathAction, tr("Copy Path"),
                                     tr("Copy the path to the clipboard"));
    I18nInterface::retranslateAction(mCloneAction, tr("Clone..."),
                                     tr("Clone the current workspace"));
    I18nInterface::retranslateAction(mAboutAction, tr("About"),
                                     tr("Some information about the current workspace"));

    // Retranslate the rest of our GUI by updating it, if we have been
    // initialised

    if (mInitialized)
        updateGui();
}

//==============================================================================

static const auto SettingsClonedWorkspaceFolders = QStringLiteral("ClonedWorkspaceFolders");

//==============================================================================

void PmrWorkspacesWindowWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the names of folders containing a cloned workspace, and add them

    foreach (const QString &cloneWorkspaceFolder,
             pSettings->value(SettingsClonedWorkspaceFolders).toStringList()) {
        addWorkspaceFolder(cloneWorkspaceFolder);
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the names of folders containing cloned workspaces

    pSettings->setValue(SettingsClonedWorkspaceFolders, QVariant(mWorkspaceFolderUrls.keys()));
}

//==============================================================================

void PmrWorkspacesWindowWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Default handling of the event

    Core::Widget::keyPressEvent(pEvent);

    // Retrieve all the files that are currently selected
    // Note: if there is folder among the selected items, then ignore
    //       everything...

    QStringList fileNames = QStringList();
    QModelIndexList selectedIndexes = mTreeViewWidget->selectedIndexes();

    for (int i = 0, iMax = selectedIndexes.count(); i < iMax; ++i) {
        PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(selectedIndexes[i]));

        if (item->type() != PmrWorkspacesWindowItem::File) {
            fileNames = QStringList();

            break;
        } else {
            fileNames << item->fileName();
        }
    }

    // Let people know about a key having been pressed with the view of opening
    // one or several files

    if (   fileNames.count()
        && ((pEvent->key() == Qt::Key_Enter) || (pEvent->key() == Qt::Key_Return))) {
        // There are some files that are selected and we want to open them, so
        // let people know about it

        emit openFilesRequested(fileNames);
    }
}

//==============================================================================

QSize PmrWorkspacesWindowWidget::sizeHint() const
{
    // Suggest a default size for our PMR workspaces widget
    // Note: this is critical if we want a docked widget, with a PMR workspaces
    //       widget on it, to have a decent size when docked to the main
    //       window...

    return defaultSize(0.15);
}

//==============================================================================

void PmrWorkspacesWindowWidget::updateGui()
{
    // Determine the message to be displayed, if any

    if (!mAuthenticated) {
        mUserMessageWidget->setIconMessage(":/oxygen/actions/help-hint.png",
                                           tr("Authenticate yourself..."),
                                           tr("Click on the top-right button."));
    } else if (mErrorMessage.isEmpty()) {
        if (!mWorkspaceManager->count()) {
            mUserMessageWidget->setIconMessage(":/oxygen/actions/help-about.png",
                                               tr("No workspaces were found..."));
        } else {
            mUserMessageWidget->resetMessage();
        }
    } else {
        mUserMessageWidget->setIconMessage(":/oxygen/emblems/emblem-important.png",
                                           Core::formatMessage(mErrorMessage, false, true));
    }

    // Show/hide our user message widget and our tree view widget, and then
    // check whether we should start/stop our timer

    mUserMessageWidget->setVisible(!mUserMessageWidget->text().isEmpty());
    mTreeViewWidget->setVisible(mUserMessageWidget->text().isEmpty());

    startStopTimer();
}

//==============================================================================

void PmrWorkspacesWindowWidget::initialize(const PMRSupport::PmrWorkspaces &pWorkspaces,
                                           const QString &pErrorMessage,
                                           const bool &pAuthenticated)
{
    // Initialise / keep track of some properties

    mWorkspaceManager->clearWorkspaces();

    mErrorMessage = pErrorMessage;
    mAuthenticated = pAuthenticated;

    if (pErrorMessage.isEmpty() && pAuthenticated) {
        // Reconcile the URLs of my-workspaces (on PMR) with those from our
        // workspace folders (in doing so, folders/URLs that don't correspond to
        // an actual PMR workspace are pruned from the relevant maps)

        // First, clear the owned flag from the list of URLs with workspace
        // folders

        QMutableMapIterator<QString, QPair<QString, bool>> urlsIterator(mOwnedWorkspaceUrlFolders);

        while (urlsIterator.hasNext()) {
            urlsIterator.next();

            urlsIterator.setValue(QPair<QString, bool>(urlsIterator.value().first, false));
        }

        foreach (PMRSupport::PmrWorkspace *workspace, pWorkspaces) {
            // Remember our workspace, so we can find it by URL

            QString url = workspace->url();

            mWorkspaceManager->addWorkspace(workspace);

            // Check if we know its folder and flag it as ours

            if (mOwnedWorkspaceUrlFolders.contains(url)) {
                QString path = mOwnedWorkspaceUrlFolders.value(url).first;

                mOwnedWorkspaceUrlFolders.insert(url, QPair<QString, bool>(path, true));

                workspace->setPath(path);
                workspace->open();
            }
        }

        // Now check the workspace folders that aren't from my-workspaces (on
        // PMR)

        urlsIterator.toFront();

        while (urlsIterator.hasNext()) {
            urlsIterator.next();

            if (!urlsIterator.value().second) {
                QString url = urlsIterator.key();
                PMRSupport::PmrWorkspace *workspace = mPmrWebService->workspace(url);

                if (workspace) {
                    // The workspace is known, so ask our workspace manager to
                    // track it, and then open it

                    mWorkspaceManager->addWorkspace(workspace);

                    workspace->setPath(urlsIterator.value().first);
                    workspace->open();
                } else {
                    // The workspace is not known, so forget about it

                    mWorkspaceFolderUrls.remove(urlsIterator.value().first);

                    urlsIterator.remove();
                }
            }
        }
    }

    // Populate our tree view widget with our different workspaces

    mTreeViewModel->clear();

    foreach (PMRSupport::PmrWorkspace *workspace, mWorkspaceManager->workspaces())
        addWorkspace(workspace);

    updateGui();

    mInitialized = true;
}

//==============================================================================

void PmrWorkspacesWindowWidget::resizeTreeViewToContents()
{
    // Resize our tree view widget so that all of its contents is visible

    mTreeViewWidget->resizeColumnToContents(0);
}

//==============================================================================

PmrWorkspacesWindowItem * PmrWorkspacesWindowWidget::currentItem() const
{
    // Return our current item

    return static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(mTreeViewWidget->currentIndex()));
}

//==============================================================================

void PmrWorkspacesWindowWidget::addWorkspace(PMRSupport::PmrWorkspace *pWorkspace)
{
    // Determine where in our tree view widget the given workspace should be
    // inserted

    QString workspaceName = pWorkspace->name();
    int row = -1;

    for (int i = 0, iMax = mTreeViewModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
        if (workspaceName.compare(mTreeViewModel->invisibleRootItem()->child(i)->text()) < 0) {
            row = i;

            break;
        }
    }

    // Add the given workspace to our tree view widget

    PMRSupport::PmrWorkspace::WorkspaceStatus workspaceStatus = pWorkspace->gitWorkspaceStatus();
    QIcon icon = pWorkspace->isOwned()?
                     mCollapsedOwnedWorkspaceIcon:
                     QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon);
    QIcon otherIcon = pWorkspace->isOwned()?
                          mExpandedOwnedWorkspaceIcon:
                          QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon);

    if (   (workspaceStatus & PMRSupport::PmrWorkspace::StatusUnstaged)
        || (workspaceStatus & PMRSupport::PmrWorkspace::StatusConflict)) {
        QIcon overlayIcon = QIcon(QString(":/PMRWorkspacesWindow/w%1.png").arg((workspaceStatus & PMRSupport::PmrWorkspace::StatusUnstaged)?"Q":"E"));
        int folderIconSize = icon.availableSizes().first().width();
        int overlayIconSize = 0.57*folderIconSize;

        icon = Core::overlayedIcon(icon, overlayIcon,
                                   folderIconSize, folderIconSize,
                                   0, 0, overlayIconSize, overlayIconSize);
        otherIcon = Core::overlayedIcon(otherIcon, overlayIcon,
                                        folderIconSize, folderIconSize,
                                        0, 0, overlayIconSize, overlayIconSize);
    }

    PmrWorkspacesWindowItem *workspaceItem = new PmrWorkspacesWindowItem(pWorkspace->isOwned()?
                                                                             PmrWorkspacesWindowItem::OwnedWorkspace:
                                                                             PmrWorkspacesWindowItem::Workspace,
                                                                         icon, otherIcon,
                                                                         workspaceName,
                                                                         pWorkspace);

    if (row == -1)
        mTreeViewModel->invisibleRootItem()->appendRow(workspaceItem);
    else
        mTreeViewModel->invisibleRootItem()->insertRow(row, workspaceItem);

    if (pWorkspace->rootFileNode())
        populateWorkspace(workspaceItem, pWorkspace->rootFileNode());

    resizeTreeViewToContents();
}

//==============================================================================

void PmrWorkspacesWindowWidget::populateWorkspace(PMRSupport::PmrWorkspace *pWorkspace)
{
    // Retrieve the folder item for the given workspace and populate it

    for (int i = 0, iMax = mTreeViewModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
        PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->invisibleRootItem()->child(i));

        if (item->workspace() == pWorkspace) {
            populateWorkspace(item, pWorkspace->rootFileNode());

            break;
        }
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::populateWorkspace(PmrWorkspacesWindowItem *pFolderItem,
                                                  PMRSupport::PmrWorkspaceFileNode *pFileNode)
{
    // Populate the given folder item with its children, which are referenced in
    // the given file node

    foreach(PMRSupport::PmrWorkspaceFileNode *fileNode, pFileNode->children()) {
        if (fileNode->hasChildren()) {
            PmrWorkspacesWindowItem *folderItem = new PmrWorkspacesWindowItem(PmrWorkspacesWindowItem::Folder,
                                                                              QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon),
                                                                              QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon),
                                                                              fileNode->name());

            pFolderItem->appendRow(folderItem);

            if (fileNode->hasChildren())
                populateWorkspace(folderItem, fileNode);
        } else {
            // We are dealing with a file, so retrieve its status and generate a
            // special icon for it, if needed

            static const QIcon DefaultFileIcon = QFileIconProvider().icon(QFileIconProvider::File);

            PMRSupport::CharPair status = fileNode->status();
            QIcon icon = DefaultFileIcon;
            int iconSize = icon.availableSizes().first().width();
            int overlaySize = 0.57*iconSize;

            if ((status.first != ' ') || (status.second != ' ')) {
                icon = Core::overlayedIcon(icon,
                                           QIcon(QString(":/PMRWorkspacesWindow/%1%2.png").arg((status.first != ' ')?"i":"w",
                                                                                               (status.first != ' ')?status.first:status.second)),
                                           iconSize, iconSize,
                                           0, 0, overlaySize, overlaySize);
            }

            pFolderItem->appendRow(new PmrWorkspacesWindowItem(PmrWorkspacesWindowItem::File,
                                                               icon, fileNode));
        }
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::showCustomContextMenu(const QPoint &pPosition) const
{
    // Determine whether to show the context menu based on whether we are over
    // an item

    PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(mTreeViewWidget->indexAt(pPosition)));

    if (item) {
        // We are over an item, so update our context menu and show it

        bool workspaceItem =    (item->type() == PmrWorkspacesWindowItem::OwnedWorkspace)
                             || (item->type() == PmrWorkspacesWindowItem::Workspace);

        mViewInPmrAction->setVisible(workspaceItem);
        mViewOncomputerAction->setVisible(workspaceItem);
        mCopyUrlAction->setVisible(workspaceItem);
        mCopyPathAction->setVisible(workspaceItem);
        mCloneAction->setVisible(item->type() == PmrWorkspacesWindowItem::OwnedWorkspace);
        mAboutAction->setVisible(workspaceItem);

        bool onlyOneItem = mTreeViewWidget->selectedIndexes().count() == 1;
        bool clonedItem = !mOwnedWorkspaceUrlFolders.value(item->url()).first.isEmpty();

        mViewOncomputerAction->setEnabled(clonedItem);
        mCopyUrlAction->setEnabled(onlyOneItem);
        mCopyPathAction->setEnabled(onlyOneItem && clonedItem);
        mCloneAction->setEnabled(!clonedItem);
        mAboutAction->setEnabled(onlyOneItem);

        mContextMenu->exec(QCursor::pos());
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::itemDoubleClicked(const QModelIndex &pIndex)
{
    // Ask for a file to be opened

    PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(pIndex));

    if (item->type() == PmrWorkspacesWindowItem::File)
        emit openFileRequested(item->fileName());
}

//==============================================================================

void PmrWorkspacesWindowWidget::itemExpanded(const QModelIndex &pIndex)
{
    // Update the icon of the item, if its type is that an owned workspace

    PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(pIndex));

    item->setIcon(item->expandedIcon());
}

//==============================================================================

void PmrWorkspacesWindowWidget::itemCollapsed(const QModelIndex &pIndex)
{
    // Update the icon of the item, if its type is that an owned workspace

    PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(pIndex));

    item->setIcon(item->collapsedIcon());
}

//==============================================================================

void PmrWorkspacesWindowWidget::duplicateCloneMessage(const QString &pUrl,
                                                      const QString &pPath1,
                                                      const QString &pPath2)
{
    // Let people know that the given workspace has been cloned twice, but only
    // if it doesn't occur when initialising ourselves
    // Note: indeed, someone handling the warning() signal might decide to show
    //       a dialog box to warn the user. Such a thing is fine when everything
    //       is initialised, but not when initialising ourselves (since the
    //       dialog box would show up from nowhere)...

    if (mInitialized)
        emit warning(QString("Workspace '%1' is cloned into both '%2' and '%3'").arg(pUrl, pPath1, pPath2));
}

//==============================================================================

QString PmrWorkspacesWindowWidget::addWorkspaceFolder(const QString &pWorkspaceFolder)
{
    // Add the given workspace folder, if it has not already been added

    if (!mWorkspaceFolderUrls.contains(pWorkspaceFolder)) {
        // Retrieve the workspace URL (i.e. remote.origin.url) for the given
        // folder

        QString res = QString();
        QByteArray folderByteArray = pWorkspaceFolder.toUtf8();
        git_repository *gitRepository = 0;

        if (!git_repository_open(&gitRepository, folderByteArray.constData())) {
            git_strarray remotes;

            if (!git_remote_list(&remotes, gitRepository)) {
                for (size_t i = 0; i < remotes.count; ++i) {
                    char *name = remotes.strings[i];

                    if (!strcmp(name, "origin")) {
                        git_remote *remote = 0;

                        if (!git_remote_lookup(&remote, gitRepository, name)) {
                            const char *remoteUrl = git_remote_url(remote);

                            if (remoteUrl)
                                res = QString(remoteUrl);
                        }
                    }
                }
            }

            git_repository_free(gitRepository);
        }

        // Keep track of the workspace space URL, if there is one and it's not
        // already tracked

        if (!res.isEmpty()) {
            if (mOwnedWorkspaceUrlFolders.contains(res)) {
                duplicateCloneMessage(res, mOwnedWorkspaceUrlFolders.value(res).first, pWorkspaceFolder);
            } else {
                mWorkspaceFolderUrls.insert(pWorkspaceFolder, res);
                mOwnedWorkspaceUrlFolders.insert(res, QPair<QString, bool>(pWorkspaceFolder, false));
            }
        }

        return res;
    } else {
        return mWorkspaceFolderUrls.value(pWorkspaceFolder);
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::startStopTimer()
{
    // Start our timer if OpenCOR is active and we have at least one workspace,
    // or stop it if either OpenCOR is not active or we no longer have at least
    // one workspace
    // Note: if we are to start our timer, then we refresh the workspace first
    //       since waiting one second may seem long to a user...

    if (   !mTimer->isActive()
        &&  Core::opencorActive() && mTreeViewModel->rowCount()) {
        disconnect(qApp, SIGNAL(focusWindowChanged(QWindow *)),
                   this, SLOT(startStopTimer()));

        refreshWorkspaces();

        connect(qApp, SIGNAL(focusWindowChanged(QWindow *)),
                this, SLOT(startStopTimer()));

        mTimer->start(1000);
    } else if (   mTimer->isActive()
               && (!Core::opencorActive() || !mTreeViewModel->rowCount())) {
        mTimer->stop();
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::refreshWorkspaces()
{
    // Refresh our workspaces

    foreach (PMRSupport::PmrWorkspace *workspace, mWorkspaceManager->workspaces())
        refreshWorkspace(workspace);
}

//==============================================================================

bool PmrWorkspacesWindowWidget::hasWorkspaces() const
{
    // Return whether we have some workspaces

    return !mWorkspaceManager->workspaces().isEmpty();
}

//==============================================================================

void PmrWorkspacesWindowWidget::commitWorkspace(const QString &pUrl)
{
//---GRY--- To be reviewed...
    PMRSupport::PmrWorkspace *workspace = mWorkspaceManager->workspace(pUrl);

    if (workspace && workspace->isLocal()) {
        if (workspace->isMerging()) {
            workspace->commitMerge();
        } else {
            PmrWorkspacesWindowCommitDialog commitDialog(workspace->stagedFiles(),
                                                         Core::mainWindow());

            if (commitDialog.exec() == QDialog::Accepted)
                workspace->commit(commitDialog.message());
        }

        refreshWorkspace(workspace);
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::refreshWorkspace(PMRSupport::PmrWorkspace *pWorkspace)
{
    // Refresh the status of the given workspace

    pWorkspace->refreshStatus();

//---GRY--- Somehow need to update our tree view, if needed...
}

//==============================================================================

void PmrWorkspacesWindowWidget::refreshWorkspaceFile(const QString &pPath)
{
Q_UNUSED(pPath);
/*---GRY---
    QWebElement fileElement = page()->mainFrame()->documentElement().findFirst(QString("tr.file[id=\"%1\"]").arg(pPath));
    QWebElement workspaceElement = parentWorkspaceElement(fileElement);

    if (!workspaceElement.isNull()) {
        PMRSupport::PmrWorkspace *workspace = mWorkspaceManager->workspace(workspaceElement.attribute("id"));

        if (workspace) {
            // We have a valid workspace so update file's status

            QStringList statusActionHtml = fileStatusActionHtml(workspace, pPath);
            QWebElement statusElement = fileElement.findFirst("td.status");

            if (!statusElement.isNull()) {
                if (statusActionHtml[0].isEmpty())
                    statusElement.addClass("hidden");
                else
                    statusElement.removeClass("hidden");

                statusElement.setInnerXml(statusActionHtml[0]);
            }

            QWebElement actionElement = fileElement.findFirst("td.actions");

            if (!actionElement.isNull()) {
                if (statusActionHtml[1].isEmpty())
                    actionElement.addClass("hidden");
                else
                    actionElement.removeClass("hidden");

                actionElement.setInnerXml(statusActionHtml[1]);
            }
        }
    }
*/
}

//==============================================================================

void PmrWorkspacesWindowWidget::requestWorkspaces()
{
    // Get the list of workspaces from our PMR web service, after making sure
    // that we have cleared existing workspaces
    // Note: initialize() will be called when the list of workspaces has been
    //       received...

    mWorkspaceManager->clearWorkspaces();

    mPmrWebService->requestWorkspaces();
}

//==============================================================================

void PmrWorkspacesWindowWidget::synchronizeWorkspace(const QString &pUrl,
                                                     const bool &pPush)
{
//---GRY--- To be reviewed...
    PMRSupport::PmrWorkspace *workspace = mWorkspaceManager->workspace(pUrl);

    if (workspace && workspace->isLocal())
        mPmrWebService->requestWorkspaceSynchronize(workspace, pPush);
}

//==============================================================================

void PmrWorkspacesWindowWidget::workspaceCloned(PMRSupport::PmrWorkspace *pWorkspace)
{
    // Add the given cloned workspace to ourselves, if possible

    if (pWorkspace) {
        QString url = pWorkspace->url();

        // Check whether we already know about the workspace

        if (!mOwnedWorkspaceUrlFolders.contains(url)) {
            // This is a cloned workspace that we don't own, so keep track of
            // it, open it and then add it to our GUI

            QString folder = pWorkspace->path();

            mWorkspaceFolderUrls.insert(folder, url);
            mOwnedWorkspaceUrlFolders.insert(url, QPair<QString, bool>(folder, false));

            mWorkspaceManager->addWorkspace(pWorkspace);

            pWorkspace->open();

            addWorkspace(pWorkspace);
        } else {
            // We already about that workspace, so check whether it's because we
            // have just cloned a workspace that we own

            QPair<QString, bool> folderOwned = mOwnedWorkspaceUrlFolders.value(url);

            if (folderOwned.second) {
                // We have just cloned a workspace that we own, so simply
                // populate it

                populateWorkspace(pWorkspace);
            } else {
                // This is not a workspace that we own, meaning that we have
                // already cloned a workspace with the same URL, so let the user
                // know about it

                duplicateCloneMessage(url, folderOwned.first, pWorkspace->path());
            }
        }
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::workspaceSynchronized(PMRSupport::PmrWorkspace *pWorkspace)
{
//---GRY--- To be reviewed...
    if (pWorkspace)
        refreshWorkspace(pWorkspace);
}

//==============================================================================

void PmrWorkspacesWindowWidget::viewInPmr()
{
    // Show the selected items in PMR

    QModelIndexList selectedIndexes = mTreeViewWidget->selectedIndexes();

    for (int i = 0, iMax = selectedIndexes.count(); i < iMax; ++i)
        QDesktopServices::openUrl(static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(selectedIndexes[i]))->url());
}

//==============================================================================

void PmrWorkspacesWindowWidget::viewOnComputer()
{
    // Show the selected items on the user's computer

    QModelIndexList selectedIndexes = mTreeViewWidget->selectedIndexes();

    for (int i = 0, iMax = selectedIndexes.count(); i < iMax; ++i)
        QDesktopServices::openUrl(QUrl::fromLocalFile(static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(selectedIndexes[i]))->path()));
}

//==============================================================================

void PmrWorkspacesWindowWidget::copyUrl()
{
    // Copy the current item's URL to the clipboard

    QApplication::clipboard()->setText(currentItem()->url());
}

//==============================================================================

void PmrWorkspacesWindowWidget::copyPath()
{
    // Copy the current item's path to the clipboard

    QApplication::clipboard()->setText(currentItem()->path());
}

//==============================================================================

void PmrWorkspacesWindowWidget::clone()
{
    // Let people know that we want to clone the current (owned) workspace

    emit cloneOwnedWorkspaceRequested(mWorkspaceManager->workspace(currentItem()->url()));
}

//==============================================================================

void PmrWorkspacesWindowWidget::about()
{
    // Let people know that we want to show some information about the current
    // workspace

    static const QString Entry = "    <tr>\n"
                                 "        <td style=\"font-weight: bold;\">%1</td>\n"
                                 "        <td>%2</td>\n"
                                 "    </tr>\n";

    PMRSupport::PmrWorkspace *workspace = mWorkspaceManager->workspace(currentItem()->url());
    QString message = QString("<p style=\"font-weight: bold;\">\n"
                              "    %1\n"
                              "</p>\n").arg(workspace->name());

    if (!workspace->description().isEmpty()) {
        message += QString("\n"
                           "<p>\n"
                           "    %1\n"
                           "</p>\n").arg(workspace->description());
    }

    message += "\n"
               "<br/>\n"
               "\n"
               "<table>\n";

    if (!workspace->owner().isEmpty())
        message += Entry.arg(tr("Owner:"), workspace->owner());

    message += Entry.arg(tr("PMR:"))
                    .arg(QString("<a href=\"%1\">%1</a>").arg(workspace->url()));

    if (workspace->isLocal()) {
        message += Entry.arg(tr("Path:"))
                        .arg(QString("<a href=\"%1\">%2</a>").arg(QUrl::fromLocalFile(workspace->path()).url())
                                                             .arg(workspace->path()));
    }

    message += "</table>\n";

    emit information(message);
}

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
