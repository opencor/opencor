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
#include "pmrworkspacefilenode.h"
#include "widget.h"

//==============================================================================

#include <QSet>
#include <QStandardItem>
#include <QStyledItemDelegate>

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
    class PmrWorkspaceManager;
}   // namespace PMRSupport

//==============================================================================

namespace PMRWorkspacesWindow {

//==============================================================================

typedef QPair<QString, QString> StringPair;
typedef QList<StringPair> StringPairs;

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
                                     const QIcon &pCollapsedIcon,
                                     const QIcon &pExpandedIcon,
                                     const QString &pText,
                                     PMRSupport::PmrWorkspace *pWorkspace = 0);
    explicit PmrWorkspacesWindowItem(const Type &pType, const QIcon &pIcon,
                                     const QString &pText,
                                     const QString &pFileName);

    virtual int type() const;

    PMRSupport::PmrWorkspace * workspace() const;

    QIcon collapsedIcon() const;
    QIcon expandedIcon() const;

    QString url() const;
    QString path() const;
    QString fileName() const;

private:
    Type mType;

    PMRSupport::PmrWorkspace *mWorkspace;

    QIcon mCollapsedIcon;
    QIcon mExpandedIcon;

    QString mFileName;

    void constructor(const Type &pType, const QIcon &pCollapsedIcon,
                     const QIcon &pExpandedIcon, const QIcon &pIcon,
                     const QString &pText, PMRSupport::PmrWorkspace *pWorkspace,
                     const QString &pFileName);
};

//==============================================================================

class PmrWorkspacesWindowWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit PmrWorkspacesWindowWidget(PMRSupport::PmrWebService *pPmrWebService,
                                       QWidget *pParent);
    ~PmrWorkspacesWindowWidget();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    bool hasWorkspaces() const;

    QString addWorkspaceFolder(const QString &pWorkspaceFolder);
    void refreshWorkspace(PMRSupport::PmrWorkspace *pWorkspace);
    void refreshWorkspaceFile(const QString &pPath);
    void requestWorkspaces();

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);
    virtual QSize sizeHint() const;

private:
    PMRSupport::PmrWebService *mPmrWebService;

    PMRSupport::PmrWorkspaceManager *mWorkspaceManager;

    QMap<QString, QString> mWorkspaceFolderUrls;
    QMap<QString, QPair<QString, bool>> mOwnedWorkspaceUrlFolders;

    bool mInitialized;

    QString mErrorMessage;
    bool mAuthenticated;

    int mRow;

    QTimer *mTimer;

    QMenu *mContextMenu;

    QAction *mViewInPmrAction;
    QAction *mViewOncomputerAction;
    QAction *mCopyUrlAction;
    QAction *mCopyPathAction;
    QAction *mCloneWorkspaceAction;
    QAction *mAboutWorkspaceAction;

    Core::UserMessageWidget *mUserMessageWidget;

    QStandardItemModel *mTreeViewModel;
    Core::TreeViewWidget *mTreeViewWidget;

    QIcon mCollapsedOwnedWorkspaceIcon;
    QIcon mExpandedOwnedWorkspaceIcon;

    void updateGui();

    PmrWorkspacesWindowItem * currentItem() const;

    void addWorkspace(PMRSupport::PmrWorkspace *pWorkspace);
    void populateWorkspace(PMRSupport::PmrWorkspace *pWorkspace);
    void populateWorkspace(PmrWorkspacesWindowItem *pFolderItem,
                           PMRSupport::PmrWorkspaceFileNode *pFileNode);

    void duplicateCloneMessage(const QString &pUrl, const QString &pPath1,
                               const QString &pPath2);

    void commitWorkspace(const QString &pUrl);
    void synchronizeWorkspace(const QString &pUrl, const bool &pPush = true);

signals:
    void information(const QString &pMessage);
    void warning(const QString &pMessage);

    void cloneOwnedWorkspaceRequested(PMRSupport::PmrWorkspace *pWorkspace);

    void openFileRequested(const QString &pFileName);
    void openFilesRequested(const QStringList &pFileNames);

public slots:
    void initialize(const PMRSupport::PmrWorkspaces &pWorkspaces,
                    const QString &pErrorMessage = QString(),
                    const bool &pAuthenticated = true);

private slots:
    void showCustomContextMenu(const QPoint &pPosition) const;
    void itemDoubleClicked(const QModelIndex &pIndex);
    void itemExpanded(const QModelIndex &pIndex);
    void itemCollapsed(const QModelIndex &pIndex);

    void resizeTreeViewToContents();

    void startStopTimer();
    void refreshWorkspaces();

    void workspaceCloned(PMRSupport::PmrWorkspace *pWorkspace);
    void workspaceSynchronized(PMRSupport::PmrWorkspace *pWorkspace);

    void viewInPmr();
    void viewOnComputer();
    void copyUrl();
    void copyPath();
    void clone();
    void about();
};

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
