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

#pragma once

//==============================================================================

#include "pmrworkspace.h"
#include "widget.h"

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
}   // namespace Core

//==============================================================================

namespace PMRSupport {
    class PmrWebService;
}   // namespace PMRSupport

//==============================================================================

namespace PMRWorkspacesWindow {

//==============================================================================

class PmrWorkspacesWindowItem : public QStandardItem
{
public:
    enum Type {
        OwnedWorkspace = QStandardItem::UserType,
        Workspace      = QStandardItem::UserType+1,
        Folder         = QStandardItem::UserType+2,
        File           = QStandardItem::UserType+3
    };

    explicit PmrWorkspacesWindowItem(const Type &pType,
                                     PMRSupport::PmrWorkspace *pWorkspace,
                                     const QIcon &pCollapsedIcon,
                                     const QIcon &pExpandedIcon);
    explicit PmrWorkspacesWindowItem(const Type &pType,
                                     PMRSupport::PmrWorkspace *pWorkspace,
                                     PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                     const QIcon &pCollapsedIcon,
                                     const QIcon &pExpandedIcon);
    explicit PmrWorkspacesWindowItem(const Type &pType,
                                     PMRSupport::PmrWorkspace *pWorkspace,
                                     PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                     const QIcon &pIcon);

    virtual int type() const;

    PMRSupport::PmrWorkspace * workspace() const;
    PMRSupport::PmrWorkspaceFileNode * fileNode() const;

    QIcon collapsedIcon() const;
    void setCollapsedIcon(const QIcon &pCollapsedIcon);

    QIcon expandedIcon() const;
    void setExpandedIcon(const QIcon &pExpandedIcon);

private:
    Type mType;

    PMRSupport::PmrWorkspace *mWorkspace;
    PMRSupport::PmrWorkspaceFileNode *mFileNode;

    QIcon mCollapsedIcon;
    QIcon mExpandedIcon;

    void constructor(const Type &pType, PMRSupport::PmrWorkspace *pWorkspace,
                     PMRSupport::PmrWorkspaceFileNode *pFileNode,
                     const QIcon &pCollapsedIcon, const QIcon &pExpandedIcon);
};

//==============================================================================

typedef QList<PmrWorkspacesWindowItem *> PmrWorkspacesWindowItems;

//==============================================================================

class PmrWorkspacesWindowProxyModel : public QSortFilterProxyModel
{
public:
    explicit PmrWorkspacesWindowProxyModel(QStandardItemModel *pModel,
                                           QObject *pParent);

protected:
    virtual bool lessThan(const QModelIndex &pSourceLeft,
                          const QModelIndex &pSourceRight) const;

private:
    QStandardItemModel *mModel;
};

//==============================================================================

class PmrWorkspacesWindowWindow;

//==============================================================================

class PmrWorkspacesWindowWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit PmrWorkspacesWindowWidget(const QString &pPmrUrl,
                                       PMRSupport::PmrWebService *pPmrWebService,
                                       PmrWorkspacesWindowWindow *pParent);
    ~PmrWorkspacesWindowWidget();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void update(const QString &pPmrUrl);

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);

    virtual QSize sizeHint() const;

private:
    QString mSettingsGroup;
    QString mPmrUrl;

    PMRSupport::PmrWebService *mPmrWebService;

    QMap<QString, QString> mClonedWorkspaceFolderUrls;
    QMap<QString, QPair<QString, bool>> mWorkspaceUrlFoldersOwned;

    bool mInitialized;

    QString mErrorMessage;
    bool mAuthenticated;

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

    QStandardItemModel *mTreeViewModel;
    PmrWorkspacesWindowProxyModel *mTreeViewProxyModel;
    Core::TreeViewWidget *mTreeViewWidget;

    QIcon mCollapsedWorkspaceIcon;
    QIcon mExpandedWorkspaceIcon;

    QIcon mUnstagedCollapsedWorkspaceIcon;
    QIcon mUnstagedExpandedWorkspaceIcon;

    QIcon mConflictCollapsedWorkspaceIcon;
    QIcon mConflictExpandedWorkspaceIcon;

    QIcon mCollapsedOwnedWorkspaceIcon;
    QIcon mExpandedOwnedWorkspaceIcon;

    QIcon mUnstagedCollapsedOwnedWorkspaceIcon;
    QIcon mUnstagedExpandedOwnedWorkspaceIcon;

    QIcon mConflictCollapsedOwnedWorkspaceIcon;
    QIcon mConflictExpandedOwnedWorkspaceIcon;

    QIcon mFileIcon;

    QIcon mWaFileIcon;
    QIcon mWcFileIcon;
    QIcon mWdFileIcon;
    QIcon mWeFileIcon;
    QIcon mWmFileIcon;
    QIcon mWqFileIcon;
    QIcon mWrFileIcon;
    QIcon mWtFileIcon;

    void reset(const QString &pPmrUrl);

    void updateGui();

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
                                               PMRSupport::PmrWorkspaceFileNode *pFileNode);
    void refreshWorkspace(PMRSupport::PmrWorkspace *pWorkspace,
                          const bool &pSortAndResize = true);

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
    void initialize(const PMRSupport::PmrWorkspaces &pWorkspaces,
                    const QString &pErrorMessage = QString(),
                    const bool &pAuthenticated = true);

private slots:
    void showCustomContextMenu() const;
    void itemDoubleClicked();
    void itemExpanded(const QModelIndex &pIndex);
    void itemCollapsed(const QModelIndex &pIndex);

    void resizeTreeViewToContents();

    void refreshWorkspaces(const PMRSupport::PmrWorkspaces &pWorkspaces = PMRSupport::PmrWorkspaces());

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

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
