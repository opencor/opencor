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
#include "pmrsupportpreferenceswidget.h"
#include "pmrwebservice.h"
#include "pmrworkspacemanager.h"
#include "pmrworkspaceswindowsynchronizedialog.h"
#include "pmrworkspaceswindowwidget.h"
#include "pmrworkspaceswindowwindow.h"
#include "preferencesinterface.h"
#include "treeviewwidget.h"
#include "usermessagewidget.h"

//==============================================================================

#include "ui_pmrworkspaceswindowwindow.h"

//==============================================================================

#include <QClipboard>
#include <QDesktopServices>
#include <QDir>
#include <QFileIconProvider>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMenu>
#include <QSettings>
#include <QTimer>
#include <QUrl>

//==============================================================================

#include "git2/remote.h"
#include "git2/repository.h"

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspacesWindow {

//==============================================================================

void PmrWorkspacesWindowItem::constructor(const Type &pType,
                                          Core::TreeViewWidget *pTreeViewWidget,
                                          PmrWorkspacesWindowProxyModel *pTreeViewProxyModel,
                                          PMRSupport::PmrWorkspace *pWorkspace,
                                          PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                          const QIcon &pCollapsedIcon,
                                          const QIcon &pExpandedIcon)
{
    // Some initialisations

    mType = pType;
    mTreeViewWidget = pTreeViewWidget;
    mTreeViewProxyModel = pTreeViewProxyModel;
    mWorkspace = pWorkspace;
    mFileNode = pFileNode;
    mCollapsedIcon = pCollapsedIcon;
    mExpandedIcon = pExpandedIcon;

    // Customise ourselves

    setToolTip(text());
}

//==============================================================================

PmrWorkspacesWindowItem::PmrWorkspacesWindowItem(const Type &pType,
                                                 Core::TreeViewWidget *pTreeViewWidget,
                                                 PmrWorkspacesWindowProxyModel *pTreeViewProxyModel,
                                                 PMRSupport::PmrWorkspace *pWorkspace,
                                                 const QIcon &pCollapsedIcon,
                                                 const QIcon &pExpandedIcon) :
    QStandardItem(pCollapsedIcon, pWorkspace->name())
{
    // Construct our object

    constructor(pType, pTreeViewWidget, pTreeViewProxyModel,
                pWorkspace, 0, pCollapsedIcon, pExpandedIcon);
}

//==============================================================================

PmrWorkspacesWindowItem::PmrWorkspacesWindowItem(const Type &pType,
                                                 Core::TreeViewWidget *pTreeViewWidget,
                                                 PmrWorkspacesWindowProxyModel *pTreeViewProxyModel,
                                                 PMRSupport::PmrWorkspace *pWorkspace,
                                                 PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                                 const QIcon &pCollapsedIcon,
                                                 const QIcon &pExpandedIcon) :
    QStandardItem(pCollapsedIcon, pFileNode->name())
{
    // Construct our object

    constructor(pType, pTreeViewWidget, pTreeViewProxyModel,
                pWorkspace, pFileNode, pCollapsedIcon, pExpandedIcon);
}

//==============================================================================

PmrWorkspacesWindowItem::PmrWorkspacesWindowItem(const Type &pType,
                                                 Core::TreeViewWidget *pTreeViewWidget,
                                                 PmrWorkspacesWindowProxyModel *pTreeViewProxyModel,
                                                 PMRSupport::PmrWorkspace *pWorkspace,
                                                 PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                                 const QIcon &pIcon) :
    QStandardItem(pIcon, pFileNode->name())
{
    // Construct our object

    constructor(pType, pTreeViewWidget, pTreeViewProxyModel,
                pWorkspace, pFileNode, QIcon(), QIcon());
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

void PmrWorkspacesWindowItem::setCollapsedIcon(const QIcon &pCollapsedIcon)
{
    // Set our collapsed icon

    mCollapsedIcon = pCollapsedIcon;

    setIcon(mTreeViewWidget->isExpanded(mTreeViewProxyModel->mapFromSource(index()))?mExpandedIcon:mCollapsedIcon);
}

//==============================================================================

QIcon PmrWorkspacesWindowItem::expandedIcon() const
{
    // Return our expanded icon

    return mExpandedIcon;
}

//==============================================================================

void PmrWorkspacesWindowItem::setExpandedIcon(const QIcon &pExpandedIcon)
{
    // Set our expanded icon

    mExpandedIcon = pExpandedIcon;

    setIcon(mTreeViewWidget->isExpanded(mTreeViewProxyModel->mapFromSource(index()))?mExpandedIcon:mCollapsedIcon);
}

//==============================================================================

PmrWorkspacesWindowProxyModel::PmrWorkspacesWindowProxyModel(QStandardItemModel *pModel,
                                                             QObject *pParent) :
    QSortFilterProxyModel(pParent),
    mModel(pModel)
{
}

//==============================================================================

bool PmrWorkspacesWindowProxyModel::lessThan(const QModelIndex &pSourceLeft,
                                             const QModelIndex &pSourceRight) const
{
    // Determine and return whether the left source should be before the right
    // source

    int leftType = static_cast<PmrWorkspacesWindowItem *>(mModel->itemFromIndex(pSourceLeft))->type();
    int rightType = static_cast<PmrWorkspacesWindowItem *>(mModel->itemFromIndex(pSourceRight))->type();

    if (   (leftType != PmrWorkspacesWindowItem::File)
        && (rightType == PmrWorkspacesWindowItem::File)) {
        return true;
    } else if (   (leftType == PmrWorkspacesWindowItem::File)
               && (rightType != PmrWorkspacesWindowItem::File)) {
        return false;
    } else {
        return QSortFilterProxyModel::lessThan(pSourceLeft, pSourceRight);
    }
}

//==============================================================================

PmrWorkspacesWindowWidget::PmrWorkspacesWindowWidget(const QString &pPmrUrl,
                                                     PMRSupport::PmrWebService *pPmrWebService,
                                                     PmrWorkspacesWindowWindow *pParent) :
    Core::Widget(pParent),
    mSettingsGroup(QString()),
    mPmrWebService(pPmrWebService)
{
    // Initialise our internals by 'resetting' them

    reset(pPmrUrl);

    // Create and customise some objects

    mUserMessageWidget = new Core::UserMessageWidget(this);

    mUserMessageWidget->setScale(0.85);

    mTreeViewModel = new QStandardItemModel(this);
    mTreeViewProxyModel = new PmrWorkspacesWindowProxyModel(mTreeViewModel, this);

    mTreeViewProxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    mTreeViewProxyModel->setSourceModel(mTreeViewModel);

    mTreeViewWidget = new Core::TreeViewWidget(this);

    mTreeViewWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    mTreeViewWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mTreeViewWidget->setHeaderHidden(true);
    mTreeViewWidget->setModel(mTreeViewProxyModel);
    mTreeViewWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mTreeViewWidget->setVisible(false);

    connect(mTreeViewWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu()));

    connect(mTreeViewWidget, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(itemDoubleClicked()));

    connect(mTreeViewWidget, SIGNAL(expanded(const QModelIndex &)),
            this, SLOT(resizeTreeViewToContents()));
    connect(mTreeViewWidget, SIGNAL(expanded(const QModelIndex &)),
            this, SLOT(itemExpanded(const QModelIndex &)));

    connect(mTreeViewWidget, SIGNAL(collapsed(const QModelIndex &)),
            this, SLOT(resizeTreeViewToContents()));
    connect(mTreeViewWidget, SIGNAL(collapsed(const QModelIndex &)),
            this, SLOT(itemCollapsed(const QModelIndex &)));

    // Create our various non-owned workspace icons

    static const QIcon UnstagedIcon = QIcon(":/PMRWorkspacesWindow/wQ.png");
    static const QIcon ConflictIcon = QIcon(":/PMRWorkspacesWindow/wE.png");

    mCollapsedWorkspaceIcon = QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon);
    mExpandedWorkspaceIcon = QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon);

    int folderIconSize = mCollapsedWorkspaceIcon.availableSizes().first().width();
    int overlayIconSize = 0.57*folderIconSize;

    mUnstagedCollapsedWorkspaceIcon = Core::overlayedIcon(mCollapsedWorkspaceIcon, UnstagedIcon,
                                                          folderIconSize, folderIconSize,
                                                          0, 0, overlayIconSize, overlayIconSize);
    mUnstagedExpandedWorkspaceIcon = Core::overlayedIcon(mExpandedWorkspaceIcon, UnstagedIcon,
                                                         folderIconSize, folderIconSize,
                                                         0, 0, overlayIconSize, overlayIconSize);

    mConflictCollapsedWorkspaceIcon = Core::overlayedIcon(mCollapsedWorkspaceIcon, ConflictIcon,
                                                          folderIconSize, folderIconSize,
                                                          0, 0, overlayIconSize, overlayIconSize);
    mConflictExpandedWorkspaceIcon = Core::overlayedIcon(mExpandedWorkspaceIcon, ConflictIcon,
                                                         folderIconSize, folderIconSize,
                                                         0, 0, overlayIconSize, overlayIconSize);

    // Create our various owned workspace icons

    static const QIcon FavoriteIcon = QIcon(":/oxygen/places/favorites.png");

    int overlayIconPos = folderIconSize-overlayIconSize;

    mCollapsedOwnedWorkspaceIcon = Core::overlayedIcon(mCollapsedWorkspaceIcon, FavoriteIcon,
                                                       folderIconSize, folderIconSize,
                                                       overlayIconPos, overlayIconPos,
                                                       overlayIconSize, overlayIconSize);
    mExpandedOwnedWorkspaceIcon = Core::overlayedIcon(mExpandedWorkspaceIcon, FavoriteIcon,
                                                      folderIconSize, folderIconSize,
                                                      overlayIconPos, overlayIconPos,
                                                      overlayIconSize, overlayIconSize);

    mUnstagedCollapsedOwnedWorkspaceIcon = Core::overlayedIcon(mCollapsedOwnedWorkspaceIcon, UnstagedIcon,
                                                               folderIconSize, folderIconSize,
                                                               0, 0, overlayIconSize, overlayIconSize);
    mUnstagedExpandedOwnedWorkspaceIcon = Core::overlayedIcon(mExpandedOwnedWorkspaceIcon, UnstagedIcon,
                                                              folderIconSize, folderIconSize,
                                                              0, 0, overlayIconSize, overlayIconSize);

    mConflictCollapsedOwnedWorkspaceIcon = Core::overlayedIcon(mCollapsedOwnedWorkspaceIcon, ConflictIcon,
                                                               folderIconSize, folderIconSize,
                                                               0, 0, overlayIconSize, overlayIconSize);
    mConflictExpandedOwnedWorkspaceIcon = Core::overlayedIcon(mExpandedOwnedWorkspaceIcon, ConflictIcon,
                                                              folderIconSize, folderIconSize,
                                                              0, 0, overlayIconSize, overlayIconSize);

    // Create our various file icons

    mFileIcon = QFileIconProvider().icon(QFileIconProvider::File);

    int fileIconSize = mFileIcon.availableSizes().first().width();

    overlayIconSize = 0.57*fileIconSize;

    mWaFileIcon = Core::overlayedIcon(mFileIcon, QIcon(":/PMRWorkspacesWindow/wA.png"),
                                      fileIconSize, fileIconSize,
                                      0, 0, overlayIconSize, overlayIconSize);
    mWcFileIcon = Core::overlayedIcon(mFileIcon, QIcon(":/PMRWorkspacesWindow/wC.png"),
                                      fileIconSize, fileIconSize,
                                      0, 0, overlayIconSize, overlayIconSize);
    mWdFileIcon = Core::overlayedIcon(mFileIcon, QIcon(":/PMRWorkspacesWindow/wD.png"),
                                      fileIconSize, fileIconSize,
                                      0, 0, overlayIconSize, overlayIconSize);
    mWeFileIcon = Core::overlayedIcon(mFileIcon, QIcon(":/PMRWorkspacesWindow/wE.png"),
                                      fileIconSize, fileIconSize,
                                      0, 0, overlayIconSize, overlayIconSize);
    mWmFileIcon = Core::overlayedIcon(mFileIcon, QIcon(":/PMRWorkspacesWindow/wM.png"),
                                      fileIconSize, fileIconSize,
                                      0, 0, overlayIconSize, overlayIconSize);
    mWqFileIcon = Core::overlayedIcon(mFileIcon, QIcon(":/PMRWorkspacesWindow/wQ.png"),
                                      fileIconSize, fileIconSize,
                                      0, 0, overlayIconSize, overlayIconSize);
    mWrFileIcon = Core::overlayedIcon(mFileIcon, QIcon(":/PMRWorkspacesWindow/wR.png"),
                                      fileIconSize, fileIconSize,
                                      0, 0, overlayIconSize, overlayIconSize);
    mWtFileIcon = Core::overlayedIcon(mFileIcon, QIcon(":/PMRWorkspacesWindow/wT.png"),
                                      fileIconSize, fileIconSize,
                                      0, 0, overlayIconSize, overlayIconSize);

    // Populate ourselves

    QLayout *layout = createLayout();

    layout->addWidget(mUserMessageWidget);
    layout->addWidget(mTreeViewWidget);

    // Connection to handle a response from our workspace manager

    PMRSupport::PmrWorkspaceManager *workspaceManager = PMRSupport::PmrWorkspaceManager::instance();

    connect(workspaceManager, SIGNAL(workspaceCloned(PMRSupport::PmrWorkspace *)),
            this, SLOT(workspaceCloned(PMRSupport::PmrWorkspace *)));
    connect(workspaceManager, SIGNAL(workspaceUncloned(PMRSupport::PmrWorkspace *)),
            this, SLOT(workspaceUncloned(PMRSupport::PmrWorkspace *)));
    connect(workspaceManager, SIGNAL(workspaceSynchronized(PMRSupport::PmrWorkspace *)),
            this, SLOT(workspaceSynchronized(PMRSupport::PmrWorkspace *)));

    // Create and start a timer for refreshing our workspaces

    mTimer = new QTimer(this);

    connect(mTimer, SIGNAL(timeout()),
            this, SLOT(refreshWorkspaces()));

    mTimer->start(1000);

    // Create and populate our context menu

    mContextMenu = new QMenu(this);

    mParentNewAction = pParent->gui()->actionNew;
    mParentReloadAction = pParent->gui()->actionReload;

    mNewWorkspaceAction = Core::newAction(mParentNewAction->icon(),
                                          this);
    mViewWorkspaceInPmrAction = Core::newAction(QIcon(":/oxygen/categories/applications-internet.png"),
                                                this);
    mViewWorkspaceOncomputerAction = Core::newAction(QIcon(":/oxygen/devices/computer.png"),
                                                     this);
    mCopyWorkspaceUrlAction = Core::newAction(QIcon(":/oxygen/actions/edit-copy.png"),
                                              this);
    mCopyWorkspacePathAction = Core::newAction(QIcon(":/oxygen/actions/edit-copy.png"),
                                               this);
    mMakeLocalWorkspaceCopyAction = Core::newAction(Core::overlayedIcon(mCollapsedWorkspaceIcon,
                                                                        QIcon(":/PMRWorkspacesWindow/pull.png"),
                                                                        folderIconSize, folderIconSize,
                                                                        overlayIconPos, overlayIconPos,
                                                                        overlayIconSize, overlayIconSize),
                                                    this);
    mSynchronizeWorkspaceAction = Core::newAction(QIcon(":/PMRWorkspacesWindow/synchronize.png"),
                                                  this);
    mReloadWorkspacesAction = Core::newAction(mParentReloadAction->icon(),
                                              this);
    mAboutWorkspaceAction = Core::newAction(QIcon(":/oxygen/actions/help-about.png"),
                                            this);

    connect(mNewWorkspaceAction, SIGNAL(triggered(bool)),
            mParentNewAction, SIGNAL(triggered(bool)));
    connect(mViewWorkspaceInPmrAction, SIGNAL(triggered(bool)),
            this, SLOT(viewWorkspaceInPmr()));
    connect(mViewWorkspaceOncomputerAction, SIGNAL(triggered(bool)),
            this, SLOT(viewWorkspaceOncomputer()));
    connect(mCopyWorkspaceUrlAction, SIGNAL(triggered(bool)),
            this, SLOT(copyWorkspaceUrl()));
    connect(mCopyWorkspacePathAction, SIGNAL(triggered(bool)),
            this, SLOT(copyWorkspacePath()));
    connect(mMakeLocalWorkspaceCopyAction, SIGNAL(triggered(bool)),
            this, SLOT(makeLocalWorkspaceCopy()));
    connect(mSynchronizeWorkspaceAction, SIGNAL(triggered(bool)),
            this, SLOT(synchronizeWorkspace()));
    connect(mReloadWorkspacesAction, SIGNAL(triggered(bool)),
            mParentReloadAction, SIGNAL(triggered(bool)));
    connect(mAboutWorkspaceAction, SIGNAL(triggered(bool)),
            this, SLOT(aboutWorkspace()));

    mContextMenu->addAction(mNewWorkspaceAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mViewWorkspaceInPmrAction);
    mContextMenu->addAction(mViewWorkspaceOncomputerAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mCopyWorkspaceUrlAction);
    mContextMenu->addAction(mCopyWorkspacePathAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mMakeLocalWorkspaceCopyAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mSynchronizeWorkspaceAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mReloadWorkspacesAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mAboutWorkspaceAction);

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
    // Note: some actions are translated prior to showing the context menu since
    //       they can be used for one or several items...

    I18nInterface::retranslateAction(mNewWorkspaceAction, tr("New Workspace..."),
                                     mParentNewAction->statusTip());
    I18nInterface::retranslateAction(mCopyWorkspaceUrlAction, tr("Copy Workspace URL"),
                                     tr("Copy the current workspace URL to the clipboard"));
    I18nInterface::retranslateAction(mCopyWorkspacePathAction, tr("Copy Workspace Path"),
                                     tr("Copy the current workspace path to the clipboard"));
    I18nInterface::retranslateAction(mMakeLocalWorkspaceCopyAction, tr("Make Local Workspace Copy..."),
                                     tr("Make a local copy of the current workspace"));
    I18nInterface::retranslateAction(mSynchronizeWorkspaceAction, tr("Synchronise Workspace With PMR..."),
                                     tr("Synchronise the curent workspace with PMR"));
    I18nInterface::retranslateAction(mReloadWorkspacesAction, tr("Reload Workspaces"),
                                     mParentReloadAction->statusTip());
    I18nInterface::retranslateAction(mAboutWorkspaceAction, tr("About Workspace"),
                                     tr("Some information about the current workspace"));

    // Retranslate the rest of our GUI by updating it, if we have been
    // initialised

    if (mInitialized)
        updateGui();
}

//==============================================================================

static const auto SettingsClonedWorkspaceFolders = QStringLiteral("ClonedWorkspaceFolders/%1");

//==============================================================================

void PmrWorkspacesWindowWidget::loadSettings(QSettings *pSettings)
{
    // Keep track of our settings' group

    mSettingsGroup = pSettings->group();

    // Retrieve and keep track of some information about the previously cloned
    // workspace folders
    // Note: for the key, we use the PMR URL's host since the URL itself
    //       contains a "://" and this messes things up with QSettings...

    foreach (const QString &clonedWorkspaceFolder,
             pSettings->value(SettingsClonedWorkspaceFolders.arg(QUrl(mPmrUrl).host())).toStringList()) {
        // Retrieve the URL (i.e. remote.origin.url) of the cloned workspace
        // folder

        QString clonedWorkspaceUrl = QString();
        git_repository *gitRepository = 0;

        if (!git_repository_open(&gitRepository,
                                 clonedWorkspaceFolder.toUtf8().constData())) {
            git_strarray remotes;

            if (!git_remote_list(&remotes, gitRepository)) {
                for (size_t i = 0; i < remotes.count; ++i) {
                    char *name = remotes.strings[i];

                    if (!strcmp(name, "origin")) {
                        git_remote *remote = 0;

                        if (!git_remote_lookup(&remote, gitRepository, name)) {
                            const char *remoteUrl = git_remote_url(remote);

                            if (remoteUrl)
                                clonedWorkspaceUrl = QString(remoteUrl);
                        }
                    }
                }
            }

            git_repository_free(gitRepository);
        }

        // Keep track of the URL of the cloned workspace folder, if there is one
        // and it's not already tracked

        if (!clonedWorkspaceUrl.isEmpty()) {
            if (mWorkspaceUrlFoldersOwned.contains(clonedWorkspaceUrl)) {
                duplicateCloneMessage(clonedWorkspaceUrl, mWorkspaceUrlFoldersOwned.value(clonedWorkspaceUrl).first, clonedWorkspaceFolder);
            } else {
                mClonedWorkspaceFolderUrls.insert(clonedWorkspaceFolder, clonedWorkspaceUrl);
                mWorkspaceUrlFoldersOwned.insert(clonedWorkspaceUrl, QPair<QString, bool>(clonedWorkspaceFolder, false));
            }
        }
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the names of folders containing cloned workspaces
    // Note: for the key, we use the PMR URL's host since the URL itself
    //       contains a "://" and this messes things up with QSettings...

    pSettings->setValue(SettingsClonedWorkspaceFolders.arg(QUrl(mPmrUrl).host()),
                        QVariant(mClonedWorkspaceFolderUrls.keys()));
}

//==============================================================================

void PmrWorkspacesWindowWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Default handling of the event

    Core::Widget::keyPressEvent(pEvent);

    // Retrieve all the files that are currently selected
    // Note: if there is a folder among the selected items, then ignore
    //       everything...

    QStringList fileNames = QStringList();
    QModelIndexList items = mTreeViewWidget->selectionModel()->selectedIndexes();

    for (int i = 0, iMax = items.count(); i < iMax; ++i) {
        PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(mTreeViewProxyModel->mapToSource(items[i])));

        if (item->type() == PmrWorkspacesWindowItem::File) {
            fileNames << item->fileNode()->path();
        } else {
            fileNames = QStringList();

            break;
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

void PmrWorkspacesWindowWidget::update(const QString &pPmrUrl)
{
    // Save our settings using the 'old' PMR URL, reset ourselves and load our
    // 'new' settings

    QSettings settings;

    settings.beginGroup(mSettingsGroup);
        saveSettings(&settings);

        reset(pPmrUrl);

        loadSettings(&settings);
    settings.endGroup();
}

//==============================================================================

void PmrWorkspacesWindowWidget::reset(const QString &pPmrUrl)
{
    // Reset our internals

    mPmrUrl = pPmrUrl;

    mClonedWorkspaceFolderUrls = QMap<QString, QString>();
    mWorkspaceUrlFoldersOwned = QMap<QString, QPair<QString, bool>>();

    mInitialized = false;

    mErrorMessage = QString();
    mAuthenticated = false;
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
        if (!PMRSupport::PmrWorkspaceManager::instance()->count()) {
            mUserMessageWidget->setIconMessage(":/oxygen/actions/help-about.png",
                                               tr("No workspaces were found..."));
        } else {
            mUserMessageWidget->resetMessage();
        }
    } else {
        mUserMessageWidget->setIconMessage(":/oxygen/emblems/emblem-important.png",
                                           Core::formatMessage(mErrorMessage, false, true));
    }

    // Show/hide our user message widget and our tree view widget

    mUserMessageWidget->setVisible(!mUserMessageWidget->text().isEmpty());
    mTreeViewWidget->setVisible(mUserMessageWidget->text().isEmpty());
}

//==============================================================================

void PmrWorkspacesWindowWidget::initialize(const PMRSupport::PmrWorkspaces &pWorkspaces,
                                           const QString &pErrorMessage,
                                           const bool &pAuthenticated)
{
    // Initialise / keep track of some properties

    PMRSupport::PmrWorkspaceManager *workspaceManager = PMRSupport::PmrWorkspaceManager::instance();

    workspaceManager->clearWorkspaces();

    mErrorMessage = pErrorMessage;
    mAuthenticated = pAuthenticated;

    if (pErrorMessage.isEmpty() && pAuthenticated) {
        // Reconcile the URLs of my-workspaces (on PMR) with those from our
        // workspace folders (in doing so, folders/URLs that don't correspond to
        // an actual PMR workspace are pruned from the relevant maps)

        // First, clear the owned flag from the list of URLs with workspace
        // folders

        QMutableMapIterator<QString, QPair<QString, bool>> urlsIterator(mWorkspaceUrlFoldersOwned);

        while (urlsIterator.hasNext()) {
            urlsIterator.next();

            urlsIterator.setValue(QPair<QString, bool>(urlsIterator.value().first, false));
        }

        foreach (PMRSupport::PmrWorkspace *workspace, pWorkspaces) {
            // Remember our workspace, so we can find it by URL

            QString url = workspace->url();

            workspaceManager->addWorkspace(workspace);

            // Check if we know about its folder and, if so, flag it as ours

            if (mWorkspaceUrlFoldersOwned.contains(url)) {
                QString path = mWorkspaceUrlFoldersOwned.value(url).first;

                mWorkspaceUrlFoldersOwned.insert(url, QPair<QString, bool>(path, true));

                workspace->open(path);
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

                    workspaceManager->addWorkspace(workspace);

                    workspace->open(urlsIterator.value().first);
                } else {
                    // The workspace is not known, so forget about it

                    mClonedWorkspaceFolderUrls.remove(urlsIterator.value().first);

                    urlsIterator.remove();
                }
            }
        }
    }

    // Populate our tree view widget with our different workspaces

    mTreeViewModel->clear();

    foreach (PMRSupport::PmrWorkspace *workspace, workspaceManager->workspaces())
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

    PmrWorkspacesWindowItem *res = static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(mTreeViewProxyModel->mapToSource(mTreeViewWidget->currentIndex())));

    if (!res) {
        // There is no current item, so return the one under our mouse pointer

        res = static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(mTreeViewProxyModel->mapToSource(mTreeViewWidget->indexAt(mTreeViewWidget->mapFromGlobal(QCursor::pos())))));
    }

    return res;
}

//==============================================================================

PmrWorkspacesWindowItem * PmrWorkspacesWindowWidget::workspaceItem(PMRSupport::PmrWorkspace *pWorkspace) const
{
    // Return the item, if any, corresponding to the given workspace

    for (int i = 0, iMax = mTreeViewModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
        PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->invisibleRootItem()->child(i));

        if (item->workspace() == pWorkspace)
            return item;
    }

    return 0;
}

//==============================================================================

void PmrWorkspacesWindowWidget::retrieveWorkspaceIcons(PMRSupport::PmrWorkspace *pWorkspace,
                                                       QIcon &pCollapsedIcon,
                                                       QIcon &pExpandedIcon)
{
    // Retrieve the icons to use for the given workspace

    PMRSupport::PmrWorkspace::WorkspaceStatus workspaceStatus = pWorkspace->gitWorkspaceStatus();

    if (workspaceStatus & PMRSupport::PmrWorkspace::StatusConflict) {
        pCollapsedIcon = pWorkspace->isOwned()?mConflictCollapsedOwnedWorkspaceIcon:mConflictCollapsedWorkspaceIcon;
        pExpandedIcon = pWorkspace->isOwned()?mConflictExpandedOwnedWorkspaceIcon:mConflictExpandedWorkspaceIcon;
    } else if (workspaceStatus & PMRSupport::PmrWorkspace::StatusUnstaged) {
        pCollapsedIcon = pWorkspace->isOwned()?mUnstagedCollapsedOwnedWorkspaceIcon:mUnstagedCollapsedWorkspaceIcon;
        pExpandedIcon = pWorkspace->isOwned()?mUnstagedExpandedOwnedWorkspaceIcon:mUnstagedExpandedWorkspaceIcon;
    } else {
        pCollapsedIcon = pWorkspace->isOwned()?mCollapsedOwnedWorkspaceIcon:mCollapsedWorkspaceIcon;
        pExpandedIcon = pWorkspace->isOwned()?mExpandedOwnedWorkspaceIcon:mExpandedWorkspaceIcon;
    }
}

//==============================================================================

PmrWorkspacesWindowItems PmrWorkspacesWindowWidget::retrieveItems(PmrWorkspacesWindowItem *pItem) const
{
    // Return the child items of the given item

    PmrWorkspacesWindowItems res = PmrWorkspacesWindowItems();

    for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i) {
        PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(pItem->child(i));

        res << item;

        if (item->hasChildren())
            res << retrieveItems(item);
    }

    return res;
}

//==============================================================================

void PmrWorkspacesWindowWidget::deleteItems(PmrWorkspacesWindowItem *pItem,
                                            PmrWorkspacesWindowItems &pItems)
{
    // Recursively delete the given items, unless there is none left

    if (pItems.isEmpty())
        return;

    if (pItem->hasChildren()) {
        // Note: we delete the child items in reverse order since deletion is
        //       done using their row number...

        for (int i = pItem->rowCount()-1; i >= 0; --i)
            deleteItems(static_cast<PmrWorkspacesWindowItem *>(pItem->child(i)), pItems);

        // Remove the folder item, if it is now empty, or workspace, if it's not
        // owned and the folder where it was cloned has been deleted

        if (!pItem->hasChildren()) {
            pItems.removeOne(pItem);

            if (pItem->parent())
                pItem->parent()->removeRow(pItem->row());
            else if (!pItem->workspace()->isOwned())
                mTreeViewModel->invisibleRootItem()->removeRow(pItem->row());
        }
    } else if (pItems.contains(pItem)) {
        pItems.removeOne(pItem);

        pItem->parent()->removeRow(pItem->row());
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::addWorkspace(PMRSupport::PmrWorkspace *pWorkspace)
{
    // Add the given workspace to our tree view widget

    QIcon collapsedIcon;
    QIcon expandedIcon;

    retrieveWorkspaceIcons(pWorkspace, collapsedIcon, expandedIcon);

    PmrWorkspacesWindowItem *item = new PmrWorkspacesWindowItem(pWorkspace->isOwned()?
                                                                    PmrWorkspacesWindowItem::OwnedWorkspace:
                                                                    PmrWorkspacesWindowItem::Workspace,
                                                                mTreeViewWidget,
                                                                mTreeViewProxyModel,
                                                                pWorkspace,
                                                                collapsedIcon, expandedIcon);

    mTreeViewModel->invisibleRootItem()->appendRow(item);

    populateWorkspace(pWorkspace, item, pWorkspace->rootFileNode());

    // Make sure that everything is properly sorted and that all of the contents
    // of our tree view widget is visible

    sortAndResizeTreeViewToContents();
}

//==============================================================================

PmrWorkspacesWindowItems PmrWorkspacesWindowWidget::populateWorkspace(PMRSupport::PmrWorkspace *pWorkspace,
                                                                      PmrWorkspacesWindowItem *pFolderItem,
                                                                      PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                                                      bool &pIsUnstaged,
                                                                      bool &pHasConflicts)
{
    // Populate the given folder item with its children, which are referenced in
    // the given file node

    PmrWorkspacesWindowItems res = PmrWorkspacesWindowItems();

    pIsUnstaged = false;
    pHasConflicts = false;

    foreach(PMRSupport::PmrWorkspaceFileNode *fileNode, pFileNode->children()) {
        // Check whether we already know about the file node

        PmrWorkspacesWindowItem *newItem = 0;

        for (int i = 0, iMax = pFolderItem->rowCount(); i < iMax; ++i) {
            PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(pFolderItem->child(i));

            if (item->fileNode() == fileNode) {
                newItem = item;

                break;
            }
        }

        // Add a new item for the file node or use the one that already exists
        // for it

        if (fileNode->hasChildren()) {
            PmrWorkspacesWindowItem *folderItem = newItem?
                                                      newItem:
                                                      new PmrWorkspacesWindowItem(PmrWorkspacesWindowItem::Folder,
                                                                                  mTreeViewWidget,
                                                                                  mTreeViewProxyModel,
                                                                                  pWorkspace,
                                                                                  fileNode,
                                                                                  mCollapsedWorkspaceIcon,
                                                                                  mExpandedWorkspaceIcon);

            if (!newItem)
                pFolderItem->appendRow(folderItem);

            bool isUnstaged;
            bool hasConflicts;

            res << folderItem
                << populateWorkspace(pWorkspace, folderItem, fileNode,
                                     isUnstaged, hasConflicts);

            folderItem->setCollapsedIcon(hasConflicts?
                                             mConflictCollapsedWorkspaceIcon:
                                             isUnstaged?
                                                 mUnstagedCollapsedWorkspaceIcon:
                                                 mCollapsedWorkspaceIcon);
            folderItem->setExpandedIcon(hasConflicts?
                                            mConflictExpandedWorkspaceIcon:
                                            isUnstaged?
                                                mUnstagedExpandedWorkspaceIcon:
                                                mExpandedWorkspaceIcon);
        } else {
            // We are dealing with a file, so retrieve its status and use the
            // corresponding icon for it, if needed
            // Note: it may happen (e.g. when deleting a folder) that the Git
            //       status is not up to date, hence we need to check for the
            //       I and W values not to be '\0' (which would be the case for
            //       a folder that doesn't contain any files anymore)...

            PMRSupport::CharPair status = fileNode->status();

            if ((status.first == '\0') && (status.second == '\0'))
                continue;

            QIcon icon = mFileIcon;

            if (status.second == 'A')
                icon = mWaFileIcon;
            else if (status.second == 'C')
                icon = mWcFileIcon;
            else if (status.second == 'D')
                icon = mWdFileIcon;
            else if (status.second == 'E')
                icon = mWeFileIcon;
            else if (status.second == 'M')
                icon = mWmFileIcon;
            else if (status.second == 'Q')
                icon = mWqFileIcon;
            else if (status.second == 'R')
                icon = mWrFileIcon;
            else if (status.second == 'T')
                icon = mWtFileIcon;

            pIsUnstaged = pIsUnstaged || ((status.second != ' ') && (status.second != 'C'));
            pHasConflicts = pHasConflicts || (status.second == 'C');

            if (newItem) {
                // We already have an item, so just update its icon

                newItem->setIcon(icon);
            } else {
                // We don't already have an item, so create one and add it

                newItem = new PmrWorkspacesWindowItem(PmrWorkspacesWindowItem::File,
                                                      mTreeViewWidget,
                                                      mTreeViewProxyModel,
                                                      pWorkspace, fileNode, icon);

                pFolderItem->appendRow(newItem);
            }

            res << newItem;
        }
    }

    return res;
}

//==============================================================================

PmrWorkspacesWindowItems PmrWorkspacesWindowWidget::populateWorkspace(PMRSupport::PmrWorkspace *pWorkspace,
                                                                      PmrWorkspacesWindowItem *pFolderItem,
                                                                      PMRSupport::PmrWorkspaceFileNode *pFileNode)
{
    // Populate the given folder item with its children, which are referenced in
    // the given file node

    bool isUnstaged;
    bool hasConflicts;

    return populateWorkspace(pWorkspace, pFolderItem, pFileNode, isUnstaged, hasConflicts);
}

//==============================================================================

void PmrWorkspacesWindowWidget::sortAndResizeTreeViewToContents()
{
    // Sort the contents of our tree view widget and make sure that all of its
    // the contents is visible

    mTreeViewProxyModel->sort(0);

    resizeTreeViewToContents();
}

//==============================================================================

void PmrWorkspacesWindowWidget::refreshWorkspace(PMRSupport::PmrWorkspace *pWorkspace,
                                                 const bool &pSortAndResize)
{
    // Refresh the status of the given workspace

    pWorkspace->refreshStatus();

    // Retrieve the item for the given workspace, if any

    PmrWorkspacesWindowItem *item = workspaceItem(pWorkspace);

    if (item) {
        // There is an item for the given workspace, retrieve and use the
        // (potentially new) icons to use with the item

        QIcon collapsedIcon;
        QIcon expandedIcon;

        retrieveWorkspaceIcons(pWorkspace, collapsedIcon, expandedIcon);

        item->setCollapsedIcon(collapsedIcon);
        item->setExpandedIcon(expandedIcon);

        item->setIcon(mTreeViewWidget->isExpanded(mTreeViewProxyModel->mapFromSource(item->index()))?expandedIcon:collapsedIcon);

        // Keep track of existing items

        PmrWorkspacesWindowItems oldItems = PmrWorkspacesWindowItems() << retrieveItems(item);

        // Populate the given workspace

        PmrWorkspacesWindowItems newItems = populateWorkspace(pWorkspace, item,
                                                              pWorkspace->rootFileNode());

        // Delete old unused items

        PmrWorkspacesWindowItems oldItemsToDelete = PmrWorkspacesWindowItems();

        foreach (PmrWorkspacesWindowItem *oldItem, oldItems) {
            if (!newItems.contains(oldItem))
                oldItemsToDelete << oldItem;
        }

        if (!oldItemsToDelete.isEmpty())
            deleteItems(item, oldItemsToDelete);

        // Make sure that everything is properly sorted and that all of the
        // contents of our tree view widget is visible

        if (pSortAndResize)
            sortAndResizeTreeViewToContents();
    }
}

//==============================================================================

QStringList PmrWorkspacesWindowWidget::selectedWorkspaceUrls() const
{
    // Return the list of (unique) workspace URLs based on the selected items in
    // our tree view widget

    QStringList res = QStringList();
    QModelIndexList items = mTreeViewWidget->selectionModel()->selectedIndexes();

    for (int i = 0, iMax = items.count(); i < iMax; ++i)
        res << static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(mTreeViewProxyModel->mapToSource(items[i])))->workspace()->url();

    res.removeDuplicates();

    return res;
}

//==============================================================================

QStringList PmrWorkspacesWindowWidget::selectedWorkspacePaths() const
{
    // Return the list of (unique) workspace paths based on the selected items
    // in our tree view widget

    QStringList res = QStringList();
    QModelIndexList items = mTreeViewWidget->selectionModel()->selectedIndexes();

    for (int i = 0, iMax = items.count(); i < iMax; ++i) {
        QString workspacePath = static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(mTreeViewProxyModel->mapToSource(items[i])))->workspace()->path();

        if (!workspacePath.isEmpty())
            res << workspacePath;
    }

    res.removeDuplicates();

    return res;
}

//==============================================================================

void PmrWorkspacesWindowWidget::showCustomContextMenu() const
{
    // Customise our context menu and show it

    QModelIndexList items = mTreeViewWidget->selectionModel()->selectedIndexes();
    bool oneItem = (items.count() == 1);
    PMRSupport::PmrWorkspaces workspaces = PMRSupport::PmrWorkspaces();
    int nbOfWorkspacePaths = selectedWorkspacePaths().count();
    bool oneWorkspaceUrl = selectedWorkspaceUrls().count() == 1;
    bool oneWorkspacePath = nbOfWorkspacePaths == 1;

    for (int i = 0, iMax = items.count(); i < iMax; ++i) {
        PMRSupport::PmrWorkspace *workspace = static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(mTreeViewProxyModel->mapToSource(items[i])))->workspace();

        if (!workspaces.contains(workspace))
            workspaces << workspace;
    }

    I18nInterface::retranslateAction(mViewWorkspaceInPmrAction,
                                     oneWorkspaceUrl?
                                         tr("View Workspace In PMR"):
                                         tr("View Workspaces In PMR"),
                                     oneWorkspaceUrl?
                                         tr("View the current workspace in PMR"):
                                         tr("View the current workspaces in PMR"));
    I18nInterface::retranslateAction(mViewWorkspaceOncomputerAction,
                                     (oneItem || oneWorkspacePath)?
                                         tr("View Workspace On Computer"):
                                         tr("View Workspaces On Computer"),
                                     (oneItem || oneWorkspacePath)?
                                         tr("View the current workspace on the computer"):
                                         tr("View the current workspaces on the computer"));

    mViewWorkspaceInPmrAction->setEnabled(items.count());
    mViewWorkspaceOncomputerAction->setEnabled(nbOfWorkspacePaths && (nbOfWorkspacePaths == workspaces.count()));
    mCopyWorkspaceUrlAction->setEnabled(oneWorkspaceUrl);
    mCopyWorkspacePathAction->setEnabled(oneWorkspacePath);
    mMakeLocalWorkspaceCopyAction->setEnabled(oneItem && !nbOfWorkspacePaths);
    mSynchronizeWorkspaceAction->setEnabled(   oneWorkspacePath
                                            && (currentItem()->workspace()->gitWorkspaceStatus() & PMRSupport::PmrWorkspace::StatusUnstaged));
    mAboutWorkspaceAction->setEnabled(oneWorkspaceUrl);

    mContextMenu->exec(QCursor::pos());
}

//==============================================================================

void PmrWorkspacesWindowWidget::itemDoubleClicked()
{
    // Ask for a file to be opened

    PmrWorkspacesWindowItem *item = currentItem();

    if (item->type() == PmrWorkspacesWindowItem::File)
        emit openFileRequested(item->fileNode()->path());
}

//==============================================================================

void PmrWorkspacesWindowWidget::itemExpanded(const QModelIndex &pIndex)
{
    // Update the icon of the item

    PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(mTreeViewProxyModel->mapToSource(pIndex)));

    item->setIcon(item->expandedIcon());
}

//==============================================================================

void PmrWorkspacesWindowWidget::itemCollapsed(const QModelIndex &pIndex)
{
    // Update the icon of the item

    PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(mTreeViewProxyModel->mapToSource(pIndex)));

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
    //       a message box to warn the user. Such a thing is fine when
    //       everything is initialised, but not when initialising ourselves
    //       (since the message box would show up from nowhere)...

    if (mInitialized)
        emit warning(QString("Workspace '%1' is cloned into both '%2' and '%3'").arg(pUrl, pPath1, pPath2));
}

//==============================================================================

void PmrWorkspacesWindowWidget::refreshWorkspaces(const PMRSupport::PmrWorkspaces &pWorkspaces)
{
    // Refresh our workspaces

    PMRSupport::PmrWorkspaces workspaces = pWorkspaces.isEmpty()?
                                               PMRSupport::PmrWorkspaceManager::instance()->workspaces():
                                               pWorkspaces;
    int workspacesCount = workspaces.count();
    int workspaceNb = 0;

    foreach (PMRSupport::PmrWorkspace *workspace, workspaces)
        refreshWorkspace(workspace, ++workspaceNb == workspacesCount);
}

//==============================================================================

void PmrWorkspacesWindowWidget::workspaceCloned(PMRSupport::PmrWorkspace *pWorkspace)
{
    // The given workspace has been cloned, so update ourselves accordingly

    QString url = pWorkspace->url();

    if (!mWorkspaceUrlFoldersOwned.contains(url)) {
        // We don't know about the workspace, so keep track of it and open it

        QString folder = pWorkspace->path();

        mClonedWorkspaceFolderUrls.insert(folder, url);
        mWorkspaceUrlFoldersOwned.insert(url, QPair<QString, bool>(folder, false));

        // Populate the workspace, if we own it and we have an entry for it
        // (i.e. we just cloned a workspace that we own), or add the workspace,
        // if we don't own it or just created a new one (that we own)

        PmrWorkspacesWindowItem *item = workspaceItem(pWorkspace);

        if (pWorkspace->isOwned() && item)
            populateWorkspace(pWorkspace, item, pWorkspace->rootFileNode());
        else
            addWorkspace(pWorkspace);
    } else {
        // We already know about the workspace, which means that we have already
        // cloned it and that we need to let the user know about it
        // Note: this should never happen, but better be safe than sorry...

        QPair<QString, bool> folderOwned = mWorkspaceUrlFoldersOwned.value(url);

        duplicateCloneMessage(url, folderOwned.first, pWorkspace->path());
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::workspaceUncloned(PMRSupport::PmrWorkspace *pWorkspace)
{
    // The given workspace has been uncloned, so update ourselves accordingly

    QString url = pWorkspace->url();

    mClonedWorkspaceFolderUrls.remove(mWorkspaceUrlFoldersOwned.value(url).first);
    mWorkspaceUrlFoldersOwned.remove(url);
}

//==============================================================================

void PmrWorkspacesWindowWidget::workspaceSynchronized(PMRSupport::PmrWorkspace *pWorkspace)
{
    // The workspace has been synchronised, so refresh it

    refreshWorkspace(pWorkspace);
}

//==============================================================================

void PmrWorkspacesWindowWidget::viewWorkspaceInPmr()
{
    // Show in PMR the workspace(s) corresponding to the selected items

    QStringList workspaceUrls = selectedWorkspaceUrls();

    for (int i = 0, iMax = workspaceUrls.count(); i < iMax; ++i)
        QDesktopServices::openUrl(workspaceUrls[i]);
}

//==============================================================================

void PmrWorkspacesWindowWidget::viewWorkspaceOncomputer()
{
    // Show on the user's computer the workspace(s) corresponding to the
    // selected items

    QStringList workspacePaths = selectedWorkspacePaths();

    for (int i = 0, iMax = workspacePaths.count(); i < iMax; ++i)
        QDesktopServices::openUrl(QUrl::fromLocalFile(workspacePaths[i]));
}

//==============================================================================

void PmrWorkspacesWindowWidget::copyWorkspaceUrl()
{
    // Copy the current workspace item's URL to the clipboard

    QApplication::clipboard()->setText(currentItem()->workspace()->url());
}

//==============================================================================

void PmrWorkspacesWindowWidget::copyWorkspacePath()
{
    // Copy the current workspace item's path to the clipboard

    QApplication::clipboard()->setText(currentItem()->workspace()->path());
}

//==============================================================================

void PmrWorkspacesWindowWidget::makeLocalWorkspaceCopy()
{
    // Make a local copy (i.e. clone) of the current owned workspace

    QString dirName = PMRSupport::PmrWebService::getEmptyDirectory();

    if (!dirName.isEmpty()) {
        // Create, if needed, the folder where the workspace will be cloned

        QDir workspaceFolder = QDir(dirName);

        if (!workspaceFolder.exists())
            workspaceFolder.mkpath(".");

        // Ask our PMR web service to effectively clone our owned workspace

        mPmrWebService->requestWorkspaceClone(PMRSupport::PmrWorkspaceManager::instance()->workspace(currentItem()->workspace()->url()),
                                              dirName);
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::synchronizeWorkspace()
{
    // Make sure that the user provided both a user name and email address

    bool hasName = !PreferencesInterface::preference(PMRSupport::PluginName, PMRSupport::SettingsPreferencesName).toByteArray().isEmpty();
    bool hasEmail = !PreferencesInterface::preference(PMRSupport::PluginName, PMRSupport::SettingsPreferencesEmail).toByteArray().isEmpty();

    if (!hasName) {
        if (!hasEmail) {
            Core::warningMessageBox(tr("Synchronise With PMR"),
                                    tr("Both a <a href=\"opencor://openPreferencesDialog/PMRSupport\">name</a> and an <a href=\"opencor://openPreferencesDialog/PMRSupport\">email</a> must be set before you can synchronise with PMR."));
        } else {
            Core::warningMessageBox(tr("Synchronise With PMR"),
                                    tr("A <a href=\"opencor://openPreferencesDialog/PMRSupport\">name</a> must be set before you can synchronise with PMR."));
        }
    } else if (!hasEmail) {
        Core::warningMessageBox(tr("Synchronise With PMR"),
                                tr("An <a href=\"opencor://openPreferencesDialog/PMRSupport\">email</a> must be set before you can synchronise with PMR."));
    } else {
        // Synchronise the current workspace, which involves letting the user
        // decide which files should be staged, commit those files, pull things
        // from PMR and, if we own the workspace, push things to PMR before
        // refreshing our workspace

        QSettings settings;

        settings.beginGroup(mSettingsGroup);
            settings.beginGroup("PmrWorkspacesWindowSynchronizeDialog");
                PMRSupport::PmrWorkspace *workspace = currentItem()->workspace();
                PmrWorkspacesWindowSynchronizeDialog synchronizeDialog(mSettingsGroup, workspace, mTimer, Core::mainWindow());

                synchronizeDialog.exec(&settings);

                if (synchronizeDialog.result() == QMessageBox::Ok) {
                    QStringList fileNames = synchronizeDialog.fileNames();

                    for (int i = 0, iMax = fileNames.count(); i < iMax; ++i)
                        workspace->stageFile(fileNames[i], true);

                    workspace->commit(synchronizeDialog.message());

                    mPmrWebService->requestWorkspaceSynchronize(currentItem()->workspace(), workspace->isOwned());
                }
            settings.endGroup();
        settings.endGroup();
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::aboutWorkspace()
{
    // Let people know that we want to show some information about the current
    // workspace

    static const QString Entry = "    <tr>\n"
                                 "        <td style=\"font-weight: bold;\">%1</td>\n"
                                 "        <td>%2</td>\n"
                                 "    </tr>\n";

    PMRSupport::PmrWorkspace *workspace = PMRSupport::PmrWorkspaceManager::instance()->workspace(currentItem()->workspace()->url());
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
