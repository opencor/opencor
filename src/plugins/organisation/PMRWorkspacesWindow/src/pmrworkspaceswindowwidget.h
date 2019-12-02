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

#pragma once

//==============================================================================

#include "pmrworkspace.h"
#include "treeviewwidget.h"

//==============================================================================

#include <QSortFilterProxyModel>
#include <QStandardItem>

//==============================================================================

class QMenu;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class TreeViewWidget;
    class UserMessageWidget;
} // namespace Core

//==============================================================================

namespace PMRSupport {
    class PmrWebService;
} // namespace PMRSupport

//==============================================================================

namespace PMRWorkspacesWindow {

//==============================================================================

class PmrWorkspacesWindowProxyModel;

//==============================================================================

class PmrWorkspacesWindowItem : public QStandardItem
{
public:
    enum class Type {
        OwnedWorkspace = QStandardItem::UserType,
        Workspace      = QStandardItem::UserType+1,
        Folder         = QStandardItem::UserType+2,
        File           = QStandardItem::UserType+3
    };

    explicit PmrWorkspacesWindowItem(Type pType,
                                     Core::TreeViewWidget *pTreeViewWidget,
                                     PmrWorkspacesWindowProxyModel *pTreeViewProxyModel,
                                     PMRSupport::PmrWorkspace *pWorkspace,
                                     const QIcon &pCollapsedIcon,
                                     const QIcon &pExpandedIcon);
    explicit PmrWorkspacesWindowItem(Type pType,
                                     Core::TreeViewWidget *pTreeViewWidget,
                                     PmrWorkspacesWindowProxyModel *pTreeViewProxyModel,
                                     PMRSupport::PmrWorkspace *pWorkspace,
                                     PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                     const QIcon &pCollapsedIcon,
                                     const QIcon &pExpandedIcon);
    explicit PmrWorkspacesWindowItem(Type pType,
                                     Core::TreeViewWidget *pTreeViewWidget,
                                     PmrWorkspacesWindowProxyModel *pTreeViewProxyModel,
                                     PMRSupport::PmrWorkspace *pWorkspace,
                                     PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                     const QIcon &pIcon);

    int type() const override;

    PMRSupport::PmrWorkspace * workspace() const;
    PMRSupport::PmrWorkspaceFileNode * fileNode() const;

    QIcon collapsedIcon() const;
    void setCollapsedIcon(const QIcon &pCollapsedIcon);

    QIcon expandedIcon() const;
    void setExpandedIcon(const QIcon &pExpandedIcon);

private:
    Type mType;

    Core::TreeViewWidget *mTreeViewWidget;
    PmrWorkspacesWindowProxyModel *mTreeViewProxyModel;

    PMRSupport::PmrWorkspace *mWorkspace;
    PMRSupport::PmrWorkspaceFileNode *mFileNode;

    QIcon mCollapsedIcon;
    QIcon mExpandedIcon;

    explicit PmrWorkspacesWindowItem(Type pType,
                                     Core::TreeViewWidget *pTreeViewWidget,
                                     PmrWorkspacesWindowProxyModel *pTreeViewProxyModel,
                                     PMRSupport::PmrWorkspace *pWorkspace,
                                     PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                     const QIcon &pIcon, const QString &pText,
                                     const QIcon &pCollapsedIcon,
                                     const QIcon &pExpandedIcon);
};

//==============================================================================

using PmrWorkspacesWindowItems = QList<PmrWorkspacesWindowItem *>;

//==============================================================================

class PmrWorkspacesWindowProxyModel : public QSortFilterProxyModel
{
public:
    explicit PmrWorkspacesWindowProxyModel(QStandardItemModel *pModel,
                                           QObject *pParent);

protected:
    bool lessThan(const QModelIndex &pSourceLeft,
                  const QModelIndex &pSourceRight) const override;

private:
    QStandardItemModel *mModel;
};

//==============================================================================

class PmrWorkspacesWindowWindow;

//==============================================================================

class PmrWorkspacesWindowWidget : public Core::TreeViewWidget
{
    Q_OBJECT

public:
    enum class Message {
        None,
        Information,
        Error,
        Warning
    };

    explicit PmrWorkspacesWindowWidget(const QString &pPmrUrl,
                                       PMRSupport::PmrWebService *pPmrWebService,
                                       PmrWorkspacesWindowWindow *pParent);
    ~PmrWorkspacesWindowWidget() override;

    void retranslateUi() override;

    void loadSettings(QSettings &pSettings) override;
    void saveSettings(QSettings &pSettings) const override;

    void update(const QString &pPmrUrl);

    void initialize();
    void initialize(Message pMessage, const QString &pString);

    using QTreeView::reset;

protected:
    void keyPressEvent(QKeyEvent *pEvent) override;

private:
    QString mSettingsGroup;
    QString mPmrUrl;

    PMRSupport::PmrWebService *mPmrWebService;

    QMap<QString, QString> mClonedWorkspaceFolderUrls;
    QMap<QString, QPair<QString, bool>> mWorkspaceUrlFoldersOwned;

    bool mInitialized = false;

    Message mMessage = Message::None;
    QString mString;
    bool mAuthenticated = false;

    QTimer *mTimer;

    QMenu *mContextMenu;

    QAction *mParentNewAction;
    QAction *mParentReloadAction;

    QAction *mNewWorkspaceAction;
    QAction *mViewWorkspaceInPmrAction;
    QAction *mViewWorkspaceOncomputerAction;
    QAction *mCopyWorkspaceUrlAction;
    QAction *mCopyWorkspacePathAction;
    QAction *mMakeLocalWorkspaceCopyAction;
    QAction *mSynchronizeWorkspaceAction;
    QAction *mReloadWorkspacesAction;
    QAction *mAboutWorkspaceAction;

    Core::UserMessageWidget *mUserMessageWidget;

    QStandardItemModel *mModel;
    PmrWorkspacesWindowProxyModel *mProxyModel;

    QIcon mCollapsedWorkspaceIcon;
    QIcon mExpandedWorkspaceIcon;

    QIcon mStagedCollapsedWorkspaceIcon;
    QIcon mStagedExpandedWorkspaceIcon;

    QIcon mUnstagedCollapsedWorkspaceIcon;
    QIcon mUnstagedExpandedWorkspaceIcon;

    QIcon mConflictCollapsedWorkspaceIcon;
    QIcon mConflictExpandedWorkspaceIcon;

    QIcon mCollapsedOwnedWorkspaceIcon;
    QIcon mExpandedOwnedWorkspaceIcon;

    QIcon mStagedCollapsedOwnedWorkspaceIcon;
    QIcon mStagedExpandedOwnedWorkspaceIcon;

    QIcon mUnstagedCollapsedOwnedWorkspaceIcon;
    QIcon mUnstagedExpandedOwnedWorkspaceIcon;

    QIcon mConflictCollapsedOwnedWorkspaceIcon;
    QIcon mConflictExpandedOwnedWorkspaceIcon;

    QIcon mFileIcon;

    QIcon mGcFileIcon;
    QIcon mGiFileIcon;

    QIcon mIaFileIcon;
    QIcon mIdFileIcon;
    QIcon mImFileIcon;
    QIcon mIrFileIcon;
    QIcon mItFileIcon;

    QIcon mWaFileIcon;
    QIcon mWdFileIcon;
    QIcon mWmFileIcon;
    QIcon mWrFileIcon;
    QIcon mWtFileIcon;
    QIcon mWuFileIcon;

    void initialize(const PMRSupport::PmrWorkspaces &pWorkspaces,
                    Message pMessage, const QString &pString,
                    bool pAuthenticated);

    void reset(const QString &pPmrUrl);

    void updateGui(bool pForceUserMessageVisibility = false);

    PmrWorkspacesWindowItem * currentItem() const;
    PmrWorkspacesWindowItem * workspaceItem(PMRSupport::PmrWorkspace *pWorkspace) const;

    void retrieveWorkspaceIcons(PMRSupport::PmrWorkspace *pWorkspace,
                                QIcon &pCollapsedIcon, QIcon &pExpandedIcon);

    PmrWorkspacesWindowItems retrieveItems(PmrWorkspacesWindowItem *pItem) const;
    void deleteItems(PmrWorkspacesWindowItem *pItem,
                     PmrWorkspacesWindowItems &pItems);

    void addWorkspace(PMRSupport::PmrWorkspace *pWorkspace);
    PmrWorkspacesWindowItems populateWorkspace(PMRSupport::PmrWorkspace *pWorkspace,
                                               PmrWorkspacesWindowItem *pFolderItem,
                                               PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                               bool &pIsStaged,
                                               bool &pIsUnstaged,
                                               bool &pHasConflicts);
    PmrWorkspacesWindowItems populateWorkspace(PMRSupport::PmrWorkspace *pWorkspace,
                                               PmrWorkspacesWindowItem *pFolderItem,
                                               PMRSupport::PmrWorkspaceFileNode *pFileNode);
    void refreshWorkspace(PMRSupport::PmrWorkspace *pWorkspace,
                          bool pSortAndResize = true);

    void duplicateCloneMessage(const QString &pUrl, const QString &pPath1,
                               const QString &pPath2);

    void sortAndResizeTreeViewToContents();

    QStringList selectedWorkspaceUrls() const;
    QStringList selectedWorkspacePaths() const;

signals:
    void information(const QString &pMessage);
    void warning(const QString &pMessage);

    void openFileRequested(const QString &pFileName);
    void openFilesRequested(const QStringList &pFileNames);

public slots:
    void initialize(const PMRSupport::PmrWorkspaces &pWorkspaces);

private slots:
    void showCustomContextMenu() const;
    void itemDoubleClicked();
    void itemExpanded(const QModelIndex &pIndex);
    void itemCollapsed(const QModelIndex &pIndex);

    void resizeTreeViewToContents();

    void refreshWorkspaces();

    void workspaceCloned(PMRSupport::PmrWorkspace *pWorkspace);
    void workspaceUncloned(PMRSupport::PmrWorkspace *pWorkspace);
    void workspaceSynchronized(PMRSupport::PmrWorkspace *pWorkspace);

    void viewWorkspaceInPmr();
    void viewWorkspaceOncomputer();
    void copyWorkspaceUrl();
    void copyWorkspacePath();
    void makeLocalWorkspaceCopy();
    void synchronizeWorkspace();
    void aboutWorkspace();
};

//==============================================================================

} // namespace PMRWorkspacesWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
