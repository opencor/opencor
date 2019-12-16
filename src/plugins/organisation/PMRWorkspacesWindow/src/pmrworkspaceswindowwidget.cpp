/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// PMR Workspaces window widget
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "i18ninterface.h"
#include "pmrsupport.h"
#include "pmrsupportplugin.h"
#include "pmrsupportpreferenceswidget.h"
#include "pmrwebservice.h"
#include "pmrworkspace.h"
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

#include "libgit2begin.h"
    #include "git2/errors.h"
    #include "git2/remote.h"
    #include "git2/repository.h"
#include "libgit2end.h"

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspacesWindow {

//==============================================================================

PmrWorkspacesWindowItem::PmrWorkspacesWindowItem(Type pType,
                                                 Core::TreeViewWidget *pTreeViewWidget,
                                                 PmrWorkspacesWindowProxyModel *pTreeViewProxyModel,
                                                 PMRSupport::PmrWorkspace *pWorkspace,
                                                 PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                                 const QIcon &pIcon,
                                                 const QString &pText,
                                                 const QIcon &pCollapsedIcon,
                                                 const QIcon &pExpandedIcon) :
    QStandardItem(pIcon, pText),
    mType(pType),
    mTreeViewWidget(pTreeViewWidget),
    mTreeViewProxyModel(pTreeViewProxyModel),
    mWorkspace(pWorkspace),
    mFileNode(pFileNode),
    mCollapsedIcon(pCollapsedIcon),
    mExpandedIcon(pExpandedIcon)
{
    // Customise ourselves

    setToolTip(text());
}

//==============================================================================

PmrWorkspacesWindowItem::PmrWorkspacesWindowItem(Type pType,
                                                 Core::TreeViewWidget *pTreeViewWidget,
                                                 PmrWorkspacesWindowProxyModel *pTreeViewProxyModel,
                                                 PMRSupport::PmrWorkspace *pWorkspace,
                                                 const QIcon &pCollapsedIcon,
                                                 const QIcon &pExpandedIcon) :
    PmrWorkspacesWindowItem(pType, pTreeViewWidget, pTreeViewProxyModel,
                            pWorkspace, nullptr, pCollapsedIcon,
                            pWorkspace->name(), pCollapsedIcon, pExpandedIcon)
{
}

//==============================================================================

PmrWorkspacesWindowItem::PmrWorkspacesWindowItem(Type pType,
                                                 Core::TreeViewWidget *pTreeViewWidget,
                                                 PmrWorkspacesWindowProxyModel *pTreeViewProxyModel,
                                                 PMRSupport::PmrWorkspace *pWorkspace,
                                                 PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                                 const QIcon &pCollapsedIcon,
                                                 const QIcon &pExpandedIcon) :
    PmrWorkspacesWindowItem(pType, pTreeViewWidget, pTreeViewProxyModel,
                            pWorkspace, pFileNode, pCollapsedIcon,
                            pFileNode->name(), pCollapsedIcon, pExpandedIcon)
{
}

//==============================================================================

PmrWorkspacesWindowItem::PmrWorkspacesWindowItem(Type pType,
                                                 Core::TreeViewWidget *pTreeViewWidget,
                                                 PmrWorkspacesWindowProxyModel *pTreeViewProxyModel,
                                                 PMRSupport::PmrWorkspace *pWorkspace,
                                                 PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                                 const QIcon &pIcon) :
    PmrWorkspacesWindowItem(pType, pTreeViewWidget, pTreeViewProxyModel,
                            pWorkspace, pFileNode, pIcon, pFileNode->name(),
                            QIcon(), QIcon())
{
}

//==============================================================================

int PmrWorkspacesWindowItem::type() const
{
    // Return our type

    return int(mType);
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

    if (   (leftType != int(PmrWorkspacesWindowItem::Type::File))
        && (rightType == int(PmrWorkspacesWindowItem::Type::File))) {
        return true;
    }

    if (   (leftType == int(PmrWorkspacesWindowItem::Type::File))
        && (rightType != int(PmrWorkspacesWindowItem::Type::File))) {
        return false;
    }

    return QSortFilterProxyModel::lessThan(pSourceLeft, pSourceRight);
}

//==============================================================================

PmrWorkspacesWindowWidget::PmrWorkspacesWindowWidget(const QString &pPmrUrl,
                                                     PMRSupport::PmrWebService *pPmrWebService,
                                                     PmrWorkspacesWindowWindow *pParent) :
    Core::TreeViewWidget(pParent),
    mPmrWebService(pPmrWebService)
{
    // Initialise our internals by 'resetting' them

    reset(pPmrUrl);

    // Customise ourselves

    mModel = new QStandardItemModel(this);
    mProxyModel = new PmrWorkspacesWindowProxyModel(mModel, this);

    mProxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    mProxyModel->setSourceModel(mModel);

    setContextMenuPolicy(Qt::CustomContextMenu);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setHeaderHidden(true);
    setModel(mProxyModel);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(this, &PmrWorkspacesWindowWidget::customContextMenuRequested,
            this, &PmrWorkspacesWindowWidget::showCustomContextMenu);

    connect(this, &PmrWorkspacesWindowWidget::doubleClicked,
            this, &PmrWorkspacesWindowWidget::itemDoubleClicked);

    connect(this, &PmrWorkspacesWindowWidget::expanded,
            this, &PmrWorkspacesWindowWidget::resizeTreeViewToContents);
    connect(this, &PmrWorkspacesWindowWidget::expanded,
            this, &PmrWorkspacesWindowWidget::itemExpanded);

    connect(this, &PmrWorkspacesWindowWidget::collapsed,
            this, &PmrWorkspacesWindowWidget::resizeTreeViewToContents);
    connect(this, &PmrWorkspacesWindowWidget::collapsed,
            this, &PmrWorkspacesWindowWidget::itemCollapsed);

    // Create and set ourselves a layout

    auto layout = new QVBoxLayout(this);

    layout->setContentsMargins(QMargins());

    setLayout(layout);

    // Create and customise our user menssage

    mUserMessageWidget = new Core::UserMessageWidget(this);

    mUserMessageWidget->setOpenExternalLinks(true);
    mUserMessageWidget->setScale(0.85);

    layout->addWidget(mUserMessageWidget);

    // Create our various non-owned workspace icons

    static const QIcon StagedIcon = QIcon(":/PMRWorkspacesWindow/iQ.png");
    static const QIcon UnstagedIcon = QIcon(":/PMRWorkspacesWindow/wQ.png");
    static const QIcon ConflictIcon = QIcon(":/PMRWorkspacesWindow/gC.png");

    mCollapsedWorkspaceIcon = Core::standardIcon(QStyle::SP_DirClosedIcon);
    mExpandedWorkspaceIcon = Core::standardIcon(QStyle::SP_DirOpenIcon);

    int folderIconSize = mCollapsedWorkspaceIcon.availableSizes().first().width();
    int overlayIconSize = int(0.57*folderIconSize);

    mStagedCollapsedWorkspaceIcon = Core::overlayedIcon(mCollapsedWorkspaceIcon, StagedIcon,
                                                        folderIconSize, folderIconSize,
                                                        0, 0, overlayIconSize, overlayIconSize);
    mStagedExpandedWorkspaceIcon = Core::overlayedIcon(mExpandedWorkspaceIcon, StagedIcon,
                                                       folderIconSize, folderIconSize,
                                                       0, 0, overlayIconSize, overlayIconSize);

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

    mStagedCollapsedOwnedWorkspaceIcon = Core::overlayedIcon(mCollapsedOwnedWorkspaceIcon, StagedIcon,
                                                             folderIconSize, folderIconSize,
                                                             0, 0, overlayIconSize, overlayIconSize);
    mStagedExpandedOwnedWorkspaceIcon = Core::overlayedIcon(mExpandedOwnedWorkspaceIcon, StagedIcon,
                                                            folderIconSize, folderIconSize,
                                                            0, 0, overlayIconSize, overlayIconSize);

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

    overlayIconSize = int(0.57*fileIconSize);

    mGcFileIcon = Core::overlayedIcon(mFileIcon, QIcon(":/PMRWorkspacesWindow/gC.png"),
                                      fileIconSize, fileIconSize,
                                      0, 0, overlayIconSize, overlayIconSize);
    mGiFileIcon = Core::overlayedIcon(mFileIcon, QIcon(":/PMRWorkspacesWindow/gI.png"),
                                      fileIconSize, fileIconSize,
                                      0, 0, overlayIconSize, overlayIconSize);

    mIaFileIcon = Core::overlayedIcon(mFileIcon, QIcon(":/PMRWorkspacesWindow/iA.png"),
                                      fileIconSize, fileIconSize,
                                      0, 0, overlayIconSize, overlayIconSize);
    mIdFileIcon = Core::overlayedIcon(mFileIcon, QIcon(":/PMRWorkspacesWindow/iD.png"),
                                      fileIconSize, fileIconSize,
                                      0, 0, overlayIconSize, overlayIconSize);
    mImFileIcon = Core::overlayedIcon(mFileIcon, QIcon(":/PMRWorkspacesWindow/iM.png"),
                                      fileIconSize, fileIconSize,
                                      0, 0, overlayIconSize, overlayIconSize);
    mIrFileIcon = Core::overlayedIcon(mFileIcon, QIcon(":/PMRWorkspacesWindow/iR.png"),
                                      fileIconSize, fileIconSize,
                                      0, 0, overlayIconSize, overlayIconSize);
    mItFileIcon = Core::overlayedIcon(mFileIcon, QIcon(":/PMRWorkspacesWindow/iT.png"),
                                      fileIconSize, fileIconSize,
                                      0, 0, overlayIconSize, overlayIconSize);

    mWaFileIcon = Core::overlayedIcon(mFileIcon, QIcon(":/PMRWorkspacesWindow/wA.png"),
                                      fileIconSize, fileIconSize,
                                      0, 0, overlayIconSize, overlayIconSize);
    mWdFileIcon = Core::overlayedIcon(mFileIcon, QIcon(":/PMRWorkspacesWindow/wD.png"),
                                      fileIconSize, fileIconSize,
                                      0, 0, overlayIconSize, overlayIconSize);
    mWmFileIcon = Core::overlayedIcon(mFileIcon, QIcon(":/PMRWorkspacesWindow/wM.png"),
                                      fileIconSize, fileIconSize,
                                      0, 0, overlayIconSize, overlayIconSize);
    mWrFileIcon = Core::overlayedIcon(mFileIcon, QIcon(":/PMRWorkspacesWindow/wR.png"),
                                      fileIconSize, fileIconSize,
                                      0, 0, overlayIconSize, overlayIconSize);
    mWtFileIcon = Core::overlayedIcon(mFileIcon, QIcon(":/PMRWorkspacesWindow/wT.png"),
                                      fileIconSize, fileIconSize,
                                      0, 0, overlayIconSize, overlayIconSize);
    mWuFileIcon = Core::overlayedIcon(mFileIcon, QIcon(":/PMRWorkspacesWindow/wU.png"),
                                      fileIconSize, fileIconSize,
                                      0, 0, overlayIconSize, overlayIconSize);

    // Connection to handle a response from our workspace manager

    PMRSupport::PmrWorkspaceManager *workspaceManager = PMRSupport::PmrWorkspaceManager::instance();

    connect(workspaceManager, &PMRSupport::PmrWorkspaceManager::workspaceCloned,
            this, &PmrWorkspacesWindowWidget::workspaceCloned);
    connect(workspaceManager, &PMRSupport::PmrWorkspaceManager::workspaceUncloned,
            this, &PmrWorkspacesWindowWidget::workspaceUncloned);
    connect(workspaceManager, &PMRSupport::PmrWorkspaceManager::workspaceSynchronized,
            this, &PmrWorkspacesWindowWidget::workspaceSynchronized);

    // Create and start a timer for refreshing our workspaces

    mTimer = new QTimer(this);

    connect(mTimer, &QTimer::timeout,
            this, &PmrWorkspacesWindowWidget::refreshWorkspaces);

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

    connect(mNewWorkspaceAction, &QAction::triggered,
            mParentNewAction, &QAction::triggered);
    connect(mViewWorkspaceInPmrAction, &QAction::triggered,
            this, &PmrWorkspacesWindowWidget::viewWorkspaceInPmr);
    connect(mViewWorkspaceOncomputerAction, &QAction::triggered,
            this, &PmrWorkspacesWindowWidget::viewWorkspaceOncomputer);
    connect(mCopyWorkspaceUrlAction, &QAction::triggered,
            this, &PmrWorkspacesWindowWidget::copyWorkspaceUrl);
    connect(mCopyWorkspacePathAction, &QAction::triggered,
            this, &PmrWorkspacesWindowWidget::copyWorkspacePath);
    connect(mMakeLocalWorkspaceCopyAction, &QAction::triggered,
            this, &PmrWorkspacesWindowWidget::makeLocalWorkspaceCopy);
    connect(mSynchronizeWorkspaceAction, &QAction::triggered,
            this, &PmrWorkspacesWindowWidget::synchronizeWorkspace);
    connect(mReloadWorkspacesAction, &QAction::triggered,
            mParentReloadAction, &QAction::triggered);
    connect(mAboutWorkspaceAction, &QAction::triggered,
            this, &PmrWorkspacesWindowWidget::aboutWorkspace);

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

    if (mInitialized) {
        updateGui();
    }
}

//==============================================================================

static const char *SettingsClonedWorkspaceFolders = "ClonedWorkspaceFolders/%1";

//==============================================================================

void PmrWorkspacesWindowWidget::loadSettings(QSettings &pSettings)
{
    // Keep track of the given settings' group

    mSettingsGroup = pSettings.group();

    // Retrieve and keep track of some information about the previously cloned
    // workspace folders
    // Note: for the key, we use the PMR URL's host since the URL itself
    //       contains a "://" and this messes things up with QSettings...

    for (const auto &clonedWorkspaceFolder : pSettings.value(QString(SettingsClonedWorkspaceFolders).arg(QUrl(mPmrUrl).host())).toStringList()) {
        // Retrieve the URL (i.e. remote.origin.url) of the cloned workspace
        // folder

        QString clonedWorkspaceUrl = QString();
        git_repository *gitRepository = nullptr;

        if (git_repository_open(&gitRepository,
                                clonedWorkspaceFolder.toUtf8().constData()) == GIT_OK) {
            git_strarray remotes;

            if (git_remote_list(&remotes, gitRepository) == GIT_OK) {
                for (size_t i = 0; i < remotes.count; ++i) {
                    char *name = remotes.strings[i];

                    if (strcmp(name, "origin") == 0) {
                        git_remote *remote = nullptr;

                        if (git_remote_lookup(&remote, gitRepository, name) == GIT_OK) {
                            const char *remoteUrl = git_remote_url(remote);

                            if (remoteUrl != nullptr) {
                                clonedWorkspaceUrl = QString(remoteUrl);
                            }
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

void PmrWorkspacesWindowWidget::saveSettings(QSettings &pSettings) const
{
    // Keep track of the names of folders containing cloned workspaces
    // Note: for the key, we use the PMR URL's host since the URL itself
    //       contains a "://" and this messes things up with QSettings...

    pSettings.setValue(QString(SettingsClonedWorkspaceFolders).arg(QUrl(mPmrUrl).host()),
                       QVariant(mClonedWorkspaceFolderUrls.keys()));
}

//==============================================================================

void PmrWorkspacesWindowWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Default handling of the event

    Core::TreeViewWidget::keyPressEvent(pEvent);

    // Retrieve all the files that are currently selected
    // Note: if there is a folder among the selected items, then ignore
    //       everything...

    QStringList fileNames = QStringList();
    QModelIndexList items = selectionModel()->selectedIndexes();

    for (int i = 0, iMax = items.count(); i < iMax; ++i) {
        PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(mModel->itemFromIndex(mProxyModel->mapToSource(items[i])));

        if (item->type() == int(PmrWorkspacesWindowItem::Type::File)) {
            fileNames << item->fileNode()->path();
        } else {
            fileNames = QStringList();

            break;
        }
    }

    // Let people know about a key having been pressed with the view of opening
    // one or several files

    if (   !fileNames.isEmpty()
        &&  ((pEvent->key() == Qt::Key_Enter) || (pEvent->key() == Qt::Key_Return))) {
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

    saveSettings(settings);

    reset(pPmrUrl);

    loadSettings(settings);
}

//==============================================================================

void PmrWorkspacesWindowWidget::reset(const QString &pPmrUrl)
{
    // Reset our internals

    mPmrUrl = pPmrUrl;

    mClonedWorkspaceFolderUrls = QMap<QString, QString>();
    mWorkspaceUrlFoldersOwned = QMap<QString, QPair<QString, bool>>();

    mInitialized = false;

    mMessage = Message::None;
    mString = QString();
    mAuthenticated = false;
}

//==============================================================================

void PmrWorkspacesWindowWidget::updateGui(bool pForceUserMessageVisibility)
{
    // Determine the message to be displayed, if any

    if (!mAuthenticated) {
        mUserMessageWidget->setIconMessage(":/oxygen/actions/help-hint.png",
                                           tr("Authenticate yourself..."),
                                           tr("Click on the top-right button."));
    } else if (mString.isEmpty()) {
        if (PMRSupport::PmrWorkspaceManager::instance()->count() == 0) {
            mUserMessageWidget->setIconMessage(":/oxygen/actions/help-about.png",
                                               tr("No workspaces were found..."));
        } else {
            mUserMessageWidget->resetMessage();
        }
    } else {
        switch (mMessage) {
        case Message::Information:
            mUserMessageWidget->setIconMessage(":/oxygen/actions/help-about.png",
                                               Core::formatMessage(mString, false, true));

            break;
        case Message::Error:
            mUserMessageWidget->setIconMessage(":/oxygen/emblems/emblem-important.png",
                                               Core::formatMessage(mString, false, true));

            break;
        case Message::Warning:
            mUserMessageWidget->setIconMessage(":/oxygen/status/task-attention.png",
                                               Core::formatMessage(mString, false, true));

            break;
        case Message::None:
            // Not a relevant type, so do nothing

            ;
        }
    }

    // Show/hide our user message widget

    mUserMessageWidget->setVisible(    pForceUserMessageVisibility
                                   || !mUserMessageWidget->text().isEmpty());
}

//==============================================================================

void PmrWorkspacesWindowWidget::initialize(const PMRSupport::PmrWorkspaces &pWorkspaces,
                                           Message pMessage,
                                           const QString &pString,
                                           bool pAuthenticated)
{
    // Initialise / keep track of some properties

    PMRSupport::PmrWorkspaceManager *workspaceManager = PMRSupport::PmrWorkspaceManager::instance();

    workspaceManager->clearWorkspaces();

    mMessage = pMessage;
    mString = pString;
    mAuthenticated = pAuthenticated;

    if (pString.isEmpty() && pAuthenticated) {
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

        for (auto workspace : pWorkspaces) {
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

                if (workspace != nullptr) {
                    // The workspace is known, so ask our workspace manager to
                    // track it, and then open it

                    workspaceManager->addWorkspace(workspace);

                    workspace->open(urlsIterator.value().first);
                } else {
                    // The workspace is not known, so forget about it

                    mClonedWorkspaceFolderUrls.remove(urlsIterator.value().first);
                }
            }
        }
    }

    // Populate our tree view widget with our different workspaces

    mModel->clear();

    for (auto workspace : workspaceManager->workspaces()) {
        addWorkspace(workspace);
    }

    updateGui(pWorkspaces.isEmpty() && pString.isEmpty() && !pAuthenticated);

    mInitialized = true;
}

//==============================================================================

void PmrWorkspacesWindowWidget::initialize(const PMRSupport::PmrWorkspaces &pWorkspaces)
{
    // Initialise ourselves using the given workspaces

    initialize(pWorkspaces, Message::None, QString(), true);
}

//==============================================================================

void PmrWorkspacesWindowWidget::initialize()
{
    // Initialise ourselves

    initialize(PMRSupport::PmrWorkspaces(), Message::None, QString(), false);
}

//==============================================================================

void PmrWorkspacesWindowWidget::initialize(Message pMessage,
                                           const QString &pString)
{
    // Initialise ourselves using the given message

    initialize(PMRSupport::PmrWorkspaces(), pMessage, pString, true);
}

//==============================================================================

void PmrWorkspacesWindowWidget::resizeTreeViewToContents()
{
    // Resize our tree view widget so that all of its contents is visible

    resizeColumnToContents(0);
}

//==============================================================================

PmrWorkspacesWindowItem * PmrWorkspacesWindowWidget::currentItem() const
{
    // Return our current item

    PmrWorkspacesWindowItem *res = static_cast<PmrWorkspacesWindowItem *>(mModel->itemFromIndex(mProxyModel->mapToSource(currentIndex())));

    if (res == nullptr) {
        // There is no current item, so return the one under our mouse pointer

        res = static_cast<PmrWorkspacesWindowItem *>(mModel->itemFromIndex(mProxyModel->mapToSource(indexAt(mapFromGlobal(QCursor::pos())))));
    }

    return res;
}

//==============================================================================

PmrWorkspacesWindowItem * PmrWorkspacesWindowWidget::workspaceItem(PMRSupport::PmrWorkspace *pWorkspace) const
{
    // Return the item, if any, corresponding to the given workspace

    for (int i = 0, iMax = mModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
        auto item = static_cast<PmrWorkspacesWindowItem *>(mModel->invisibleRootItem()->child(i));

        if (item->workspace() == pWorkspace) {
            return item;
        }
    }

    return nullptr;
}

//==============================================================================

void PmrWorkspacesWindowWidget::retrieveWorkspaceIcons(PMRSupport::PmrWorkspace *pWorkspace,
                                                       QIcon &pCollapsedIcon,
                                                       QIcon &pExpandedIcon)
{
    // Retrieve the icons to use for the given workspace
    // Note: we want to check for the unstaged status before the staged status
    //       since a workspace may have both staged and unstaged files, so the
    //       unstaged status should have precedence...

    PMRSupport::PmrWorkspace::WorkspaceStatus workspaceStatus = pWorkspace->gitWorkspaceStatus();

    if ((workspaceStatus & PMRSupport::PmrWorkspace::StatusConflict) != 0) {
        pCollapsedIcon = pWorkspace->isOwned()?mConflictCollapsedOwnedWorkspaceIcon:mConflictCollapsedWorkspaceIcon;
        pExpandedIcon = pWorkspace->isOwned()?mConflictExpandedOwnedWorkspaceIcon:mConflictExpandedWorkspaceIcon;
    } else if ((workspaceStatus & PMRSupport::PmrWorkspace::StatusUnstaged) != 0) {
        pCollapsedIcon = pWorkspace->isOwned()?mUnstagedCollapsedOwnedWorkspaceIcon:mUnstagedCollapsedWorkspaceIcon;
        pExpandedIcon = pWorkspace->isOwned()?mUnstagedExpandedOwnedWorkspaceIcon:mUnstagedExpandedWorkspaceIcon;
    } else if ((workspaceStatus & PMRSupport::PmrWorkspace::StatusStaged) != 0) {
        pCollapsedIcon = pWorkspace->isOwned()?mStagedCollapsedOwnedWorkspaceIcon:mStagedCollapsedWorkspaceIcon;
        pExpandedIcon = pWorkspace->isOwned()?mStagedExpandedOwnedWorkspaceIcon:mStagedExpandedWorkspaceIcon;
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
        auto item = static_cast<PmrWorkspacesWindowItem *>(pItem->child(i));

        res << item;

        if (item->hasChildren()) {
            res << retrieveItems(item);
        }
    }

    return res;
}

//==============================================================================

void PmrWorkspacesWindowWidget::deleteItems(PmrWorkspacesWindowItem *pItem,
                                            PmrWorkspacesWindowItems &pItems)
{
    // Recursively delete the given items, unless there is none left

    if (pItems.isEmpty()) {
        return;
    }

    if (pItem->hasChildren()) {
        // Note: we delete the child items in reverse order since deletion is
        //       done using their row number...

        for (int i = pItem->rowCount()-1; i >= 0; --i) {
            deleteItems(static_cast<PmrWorkspacesWindowItem *>(pItem->child(i)), pItems);
        }

        // Remove the folder item, if it is now empty, or workspace, if it's not
        // owned and the folder where it was cloned has been deleted

        if (!pItem->hasChildren()) {
            pItems.removeOne(pItem);

            if (pItem->parent() != nullptr) {
                pItem->parent()->removeRow(pItem->row());
            } else if (!pItem->workspace()->isOwned()) {
                mModel->invisibleRootItem()->removeRow(pItem->row());
            }
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

    auto item = new PmrWorkspacesWindowItem(pWorkspace->isOwned()?
                                                PmrWorkspacesWindowItem::Type::OwnedWorkspace:
                                                PmrWorkspacesWindowItem::Type::Workspace,
                                            this, mProxyModel, pWorkspace,
                                            collapsedIcon, expandedIcon);

    mModel->invisibleRootItem()->appendRow(item);

    populateWorkspace(pWorkspace, item, pWorkspace->rootFileNode());

    // Make sure that everything is properly sorted and that all of the contents
    // of our tree view widget is visible

    sortAndResizeTreeViewToContents();
}

//==============================================================================

PmrWorkspacesWindowItems PmrWorkspacesWindowWidget::populateWorkspace(PMRSupport::PmrWorkspace *pWorkspace,
                                                                      PmrWorkspacesWindowItem *pFolderItem,
                                                                      PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                                                      bool &pIsStaged,
                                                                      bool &pIsUnstaged,
                                                                      bool &pHasConflicts)
{
    // Populate the given folder item with its children, which are referenced in
    // the given file node

    PmrWorkspacesWindowItems res = PmrWorkspacesWindowItems();

    pIsStaged = false;
    pIsUnstaged = false;
    pHasConflicts = false;

    for (auto fileNode : pFileNode->children()) {
        // Check whether we already know about the file node

        PmrWorkspacesWindowItem *newItem = nullptr;

        for (int i = 0, iMax = pFolderItem->rowCount(); i < iMax; ++i) {
            auto item = static_cast<PmrWorkspacesWindowItem *>(pFolderItem->child(i));

            if (item->fileNode() == fileNode) {
                newItem = item;

                break;
            }
        }

        // Add a new item for the file node or use the one that already exists
        // for it
        // Note: an unstaged status must have precedence over a staged status...

        if (fileNode->hasChildren()) {
            PmrWorkspacesWindowItem *folderItem = (newItem != nullptr)?
                                                      newItem:
                                                      new PmrWorkspacesWindowItem(PmrWorkspacesWindowItem::Type::Folder,
                                                                                  this,
                                                                                  mProxyModel,
                                                                                  pWorkspace,
                                                                                  fileNode,
                                                                                  mCollapsedWorkspaceIcon,
                                                                                  mExpandedWorkspaceIcon);

            if (newItem == nullptr) {
                pFolderItem->appendRow(folderItem);
            }

            bool isStaged;
            bool isUnstaged;
            bool hasConflicts;

            res << folderItem
                << populateWorkspace(pWorkspace, folderItem, fileNode,
                                     isStaged, isUnstaged, hasConflicts);

            pIsStaged = pIsStaged || isStaged;
            pIsUnstaged = pIsUnstaged || isUnstaged;
            pHasConflicts = pHasConflicts || hasConflicts;

            folderItem->setCollapsedIcon(hasConflicts?
                                             mConflictCollapsedWorkspaceIcon:
                                             isUnstaged?
                                                 mUnstagedCollapsedWorkspaceIcon:
                                                 isStaged?
                                                     mStagedCollapsedWorkspaceIcon:
                                                     mCollapsedWorkspaceIcon);
            folderItem->setExpandedIcon(hasConflicts?
                                            mConflictExpandedWorkspaceIcon:
                                            isUnstaged?
                                                mUnstagedExpandedWorkspaceIcon:
                                                isStaged?
                                                    mStagedExpandedWorkspaceIcon:
                                                    mExpandedWorkspaceIcon);
        } else {
            // We are dealing with a file, so retrieve its status and use the
            // corresponding icon for it, if needed
            // Note: it may happen (e.g. when deleting a folder) that the Git
            //       status is not up to date, hence we need to check for the
            //       I and W values not to be nullptr (which would be the case for
            //       a folder that doesn't contain any files anymore)...

            QChar iStatus = fileNode->status().first;
            QChar wStatus = fileNode->status().second;

            if ((iStatus == nullptr) && (wStatus == nullptr)) {
                continue;
            }

            QIcon icon = mFileIcon;

            // Git status

            if (wStatus == 'C') {
                icon = mGcFileIcon;
            } else if (wStatus == 'I') {
                icon = mGiFileIcon;

            // iStatus

            } else if (iStatus == 'A') {
                icon = mIaFileIcon;
            } else if (iStatus == 'D') {
                icon = mIdFileIcon;
            } else if (iStatus == 'M') {
                icon = mImFileIcon;
            } else if (iStatus == 'R') {
                icon = mIrFileIcon;
            } else if (iStatus == 'T') {
                icon = mItFileIcon;

            // wStatus

            } else if (wStatus == 'A') {
                icon = mWaFileIcon;
            } else if (wStatus == 'D') {
                icon = mWdFileIcon;
            } else if (wStatus == 'M') {
                icon = mWmFileIcon;
            } else if (wStatus == 'R') {
                icon = mWrFileIcon;
            } else if (wStatus == 'T') {
                icon = mWtFileIcon;
            } else if (wStatus == 'U') {
                icon = mWuFileIcon;
            }

            pIsStaged =    pIsStaged
                        || (    (iStatus != nullptr) && (iStatus != ' ')
                            && ((wStatus == nullptr) || (wStatus == ' ')));
            pIsUnstaged =    pIsUnstaged
                          || ((wStatus != nullptr) && (wStatus != ' ') && (wStatus != 'C'));
            pHasConflicts = pHasConflicts || (wStatus == 'C');

            if (newItem != nullptr) {
                // We already have an item, so just update its icon

                newItem->setIcon(icon);
            } else {
                // We don't already have an item, so create one and add it

                newItem = new PmrWorkspacesWindowItem(PmrWorkspacesWindowItem::Type::File,
                                                      this, mProxyModel,
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

    bool isStaged;
    bool isUnstaged;
    bool hasConflicts;

    return populateWorkspace(pWorkspace, pFolderItem, pFileNode,
                             isStaged, isUnstaged, hasConflicts);
}

//==============================================================================

void PmrWorkspacesWindowWidget::sortAndResizeTreeViewToContents()
{
    // Sort the contents of our tree view widget and make sure that all of its
    // contents is visible

    mProxyModel->sort(0);

    resizeTreeViewToContents();
}

//==============================================================================

void PmrWorkspacesWindowWidget::refreshWorkspace(PMRSupport::PmrWorkspace *pWorkspace,
                                                 bool pSortAndResize)
{
    // Refresh the status of the given workspace

    pWorkspace->refreshStatus();

    // Retrieve the item for the given workspace, if any

    PmrWorkspacesWindowItem *item = workspaceItem(pWorkspace);

    if (item != nullptr) {
        // There is an item for the given workspace, retrieve and use the
        // (potentially new) icons to use with the item

        QIcon collapsedIcon;
        QIcon expandedIcon;

        retrieveWorkspaceIcons(pWorkspace, collapsedIcon, expandedIcon);

        item->setCollapsedIcon(collapsedIcon);
        item->setExpandedIcon(expandedIcon);

        item->setIcon(isExpanded(mProxyModel->mapFromSource(item->index()))?expandedIcon:collapsedIcon);

        // Keep track of existing items

        PmrWorkspacesWindowItems oldItems = PmrWorkspacesWindowItems() << retrieveItems(item);

        // Populate the given workspace

        PmrWorkspacesWindowItems newItems = populateWorkspace(pWorkspace, item,
                                                              pWorkspace->rootFileNode());

        // Delete old unused items

        PmrWorkspacesWindowItems oldItemsToDelete = PmrWorkspacesWindowItems();

        for (auto oldItem : oldItems) {
            if (!newItems.contains(oldItem)) {
                oldItemsToDelete << oldItem;
            }
        }

        if (!oldItemsToDelete.isEmpty()) {
            deleteItems(item, oldItemsToDelete);
        }

        // Make sure that everything is properly sorted and that all of the
        // contents of our tree view widget is visible

        if (pSortAndResize) {
            sortAndResizeTreeViewToContents();
        }
    }
}

//==============================================================================

QStringList PmrWorkspacesWindowWidget::selectedWorkspaceUrls() const
{
    // Return the list of (unique) workspace URLs based on the selected items in
    // our tree view widget

    QStringList res = QStringList();
    QModelIndexList items = selectionModel()->selectedIndexes();

    for (int i = 0, iMax = items.count(); i < iMax; ++i) {
        res << static_cast<PmrWorkspacesWindowItem *>(mModel->itemFromIndex(mProxyModel->mapToSource(items[i])))->workspace()->url();
    }

    res.removeDuplicates();

    return res;
}

//==============================================================================

QStringList PmrWorkspacesWindowWidget::selectedWorkspacePaths() const
{
    // Return the list of (unique) workspace paths based on the selected items
    // in our tree view widget

    QStringList res = QStringList();
    QModelIndexList items = selectionModel()->selectedIndexes();

    for (int i = 0, iMax = items.count(); i < iMax; ++i) {
        QString workspacePath = static_cast<PmrWorkspacesWindowItem *>(mModel->itemFromIndex(mProxyModel->mapToSource(items[i])))->workspace()->path();

        if (!workspacePath.isEmpty()) {
            res << workspacePath;
        }
    }

    res.removeDuplicates();

    return res;
}

//==============================================================================

void PmrWorkspacesWindowWidget::showCustomContextMenu() const
{
    // Customise our context menu and show it

    QModelIndexList items = selectionModel()->selectedIndexes();
    bool oneItem = (items.count() == 1);
    PMRSupport::PmrWorkspaces workspaces = PMRSupport::PmrWorkspaces();
    int nbOfWorkspacePaths = selectedWorkspacePaths().count();
    bool oneWorkspaceUrl = selectedWorkspaceUrls().count() == 1;
    bool oneWorkspacePath = nbOfWorkspacePaths == 1;

    for (int i = 0, iMax = items.count(); i < iMax; ++i) {
        PMRSupport::PmrWorkspace *workspace = static_cast<PmrWorkspacesWindowItem *>(mModel->itemFromIndex(mProxyModel->mapToSource(items[i])))->workspace();

        if (!workspaces.contains(workspace)) {
            workspaces << workspace;
        }
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

    PMRSupport::PmrWorkspace *workspace = (currentItem() != nullptr)?currentItem()->workspace():nullptr;
    PMRSupport::PmrWorkspace::WorkspaceStatus workspaceStatus = (workspace != nullptr)?
                                                                    workspace->gitWorkspaceStatus():
                                                                    PMRSupport::PmrWorkspace::StatusUnknown;

    mViewWorkspaceInPmrAction->setEnabled(!items.isEmpty());
    mViewWorkspaceOncomputerAction->setEnabled((nbOfWorkspacePaths != 0) && (nbOfWorkspacePaths == workspaces.count()));
    mCopyWorkspaceUrlAction->setEnabled(oneWorkspaceUrl);
    mCopyWorkspacePathAction->setEnabled(oneWorkspacePath);
    mMakeLocalWorkspaceCopyAction->setEnabled(oneItem && (nbOfWorkspacePaths == 0));
    mSynchronizeWorkspaceAction->setEnabled(   oneWorkspacePath
                                            && (   (   (workspace != nullptr) && workspace->isOwned()
                                                    && (   ((workspaceStatus & PMRSupport::PmrWorkspace::StatusAhead) != 0)
                                                        || ((workspaceStatus & PMRSupport::PmrWorkspace::StatusStaged) != 0)))
                                                || ((workspaceStatus & PMRSupport::PmrWorkspace::StatusUnstaged) != 0)));
    mAboutWorkspaceAction->setEnabled(oneWorkspaceUrl);

    mContextMenu->exec(QCursor::pos());
}

//==============================================================================

void PmrWorkspacesWindowWidget::itemDoubleClicked()
{
    // Ask for a file to be opened

    PmrWorkspacesWindowItem *item = currentItem();

    if (item->type() == int(PmrWorkspacesWindowItem::Type::File)) {
        emit openFileRequested(item->fileNode()->path());
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::itemExpanded(const QModelIndex &pIndex)
{
    // Update the icon of the item

    PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(mModel->itemFromIndex(mProxyModel->mapToSource(pIndex)));

    item->setIcon(item->expandedIcon());
}

//==============================================================================

void PmrWorkspacesWindowWidget::itemCollapsed(const QModelIndex &pIndex)
{
    // Update the icon of the item

    PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(mModel->itemFromIndex(mProxyModel->mapToSource(pIndex)));

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

    if (mInitialized) {
        emit warning(QString("Workspace '%1' is cloned into both '%2' and '%3'").arg(pUrl,
                                                                                     pPath1,
                                                                                     pPath2));
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::refreshWorkspaces()
{
    // Refresh our workspaces, but only if we are visible

    if (!isVisible()) {
        return;
    }

    PMRSupport::PmrWorkspaces workspaces = PMRSupport::PmrWorkspaceManager::instance()->workspaces();
    int workspacesCount = workspaces.count();
    int workspaceNb = 0;

    for (auto workspace : workspaces) {
        refreshWorkspace(workspace, ++workspaceNb == workspacesCount);
    }
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

        if (pWorkspace->isOwned() && (item != nullptr)) {
            populateWorkspace(pWorkspace, item, pWorkspace->rootFileNode());
        } else {
            addWorkspace(pWorkspace);
        }
    } else {
        // We already know about the workspace, which means that we have already
        // cloned it and that we need to let the user know about it
        // Note: this should never happen, but better be safe than sorry...

        QPair<QString, bool> folderOwned = mWorkspaceUrlFoldersOwned.value(url);

        duplicateCloneMessage(url, folderOwned.first, pWorkspace->path());
    }

    // Make sure that our GUI is up to date
    // Note: in case, for example, where we started OpenCOR with no workspaces
    //       and then cloned one...

    updateGui();
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

    for (int i = 0, iMax = workspaceUrls.count(); i < iMax; ++i) {
        QDesktopServices::openUrl(workspaceUrls[i]);
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::viewWorkspaceOncomputer()
{
    // Show on the user's computer the workspace(s) corresponding to the
    // selected items

    QStringList workspacePaths = selectedWorkspacePaths();

    for (int i = 0, iMax = workspacePaths.count(); i < iMax; ++i) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(workspacePaths[i]));
    }
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

    QString dirName = PMRSupport::getEmptyDirectory();

    if (!dirName.isEmpty()) {
        // Create, if needed, the folder where the workspace will be cloned

        QDir workspaceFolder = QDir(dirName);

        if (!workspaceFolder.exists()) {
            workspaceFolder.mkpath(".");
        }

        // Ask our PMR Web service to effectively clone our owned workspace

        mPmrWebService->requestWorkspaceClone(PMRSupport::PmrWorkspaceManager::instance()->workspace(currentItem()->workspace()->url()),
                                              dirName);
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::synchronizeWorkspace()
{
    // Make sure that the user provided both a user name and email address

    bool hasName = !PreferencesInterface::preference(PMRSupport::PluginName, PMRSupport::SettingsPreferencesName, PMRSupport::SettingsPreferencesNameDefault).toString().isEmpty();
    bool hasEmail = !PreferencesInterface::preference(PMRSupport::PluginName, PMRSupport::SettingsPreferencesEmail, PMRSupport::SettingsPreferencesEmailDefault).toString().isEmpty();

    if (!hasName) {
        if (!hasEmail) {
            Core::warningMessageBox(tr("Synchronise With PMR"),
                                    tr("Both a <strong>name</strong> and an <strong>email</strong> must be set before you can synchronise with PMR."));
        } else {
            Core::warningMessageBox(tr("Synchronise With PMR"),
                                    tr("A <strong>name</strong> must be set before you can synchronise with PMR."));
        }
    } else if (!hasEmail) {
        Core::warningMessageBox(tr("Synchronise With PMR"),
                                tr("An <strong>email</strong> must be set before you can synchronise with PMR."));
    } else {
        // Synchronise the current workspace, which involves letting the user
        // decide which files should be staged (if they are not already staged),
        // commit those files, pull things from PMR and, if we own the
        // workspace, push things to PMR before refreshing our workspace
        // Note: if we come here and the Internet connection is down, then we
        //       will be able to do everything except pushing things to PMR,
        //       which means that we will be 'ahead', hence we need to check our
        //       current state...

        PMRSupport::PmrWorkspace *workspace = currentItem()->workspace();
        PMRSupport::PmrWorkspace::WorkspaceStatus workspaceStatus = workspace->gitWorkspaceStatus();
        bool needRequestWorkspaceSynchronize = false;

        if (   (    workspace->isOwned()
                && ((workspaceStatus & PMRSupport::PmrWorkspace::StatusStaged) != 0))
            || ((workspaceStatus & PMRSupport::PmrWorkspace::StatusUnstaged) != 0)) {
            PmrWorkspacesWindowSynchronizeDialog synchronizeDialog(workspace, mTimer, Core::mainWindow());

            synchronizeDialog.exec();

            if (synchronizeDialog.result() == QMessageBox::Ok) {
                QStringList fileNames = synchronizeDialog.fileNames();

                for (int i = 0, iMax = fileNames.count(); i < iMax; ++i) {
                    workspace->stageFile(fileNames[i], true);
                }

                workspace->commit(synchronizeDialog.message());

                needRequestWorkspaceSynchronize = true;
            }
        } else if (    workspace->isOwned()
                   && ((workspaceStatus & PMRSupport::PmrWorkspace::StatusAhead) != 0)) {
            // Something went wrong during a previous synchronisation (and the
            // Git push didn't work), so try again

            needRequestWorkspaceSynchronize = true;
        }

        if (needRequestWorkspaceSynchronize) {
            mPmrWebService->requestWorkspaceSynchronize(workspace, workspace->isOwned());
        }
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::aboutWorkspace()
{
    // Let people know that we want to show some information about the current
    // workspace

    static const QString Entry =  "    <tr>\n"
                                 R"(        <td style="font-weight: bold;">%1</td>)""\n"
                                  "        <td>%2</td>\n"
                                  "    </tr>\n";

    PMRSupport::PmrWorkspace *workspace = PMRSupport::PmrWorkspaceManager::instance()->workspace(currentItem()->workspace()->url());
    QString message = QString(R"(<p style="font-weight: bold;">)""\n"
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

    if (!workspace->owner().isEmpty()) {
        message += Entry.arg(tr("Owner:"),
                             workspace->owner());
    }

    message += Entry.arg(tr("PMR:"),
                         QString(R"(<a href="%1">%1</a>)").arg(workspace->url()));

    if (workspace->isLocal()) {
        message += Entry.arg(tr("Path:"),
                             QString(R"(<a href="%1">%2</a>)").arg(QUrl::fromLocalFile(workspace->path()).url(),
                                                                   workspace->path()));
    }

    message += "</table>\n";

    emit information(message);
}

//==============================================================================

} // namespace PMRWorkspacesWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
