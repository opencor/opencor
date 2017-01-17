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

class PmrWorkspacesWindowItemDelegate : public QStyledItemDelegate
{
public:
    explicit PmrWorkspacesWindowItemDelegate(Core::TreeViewWidget *pParent);

    virtual void paint(QPainter *pPainter, const QStyleOptionViewItem &pOption,
                       const QModelIndex &pIndex) const;

private:
    Core::TreeViewWidget *mTreeViewWidget;
};

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

    explicit PmrWorkspacesWindowItem(const Type &pType, const QString &pText,
                                     const QString &pUrlOrFileName = QString());

    virtual int type() const;

    QString url() const;
    QString fileName() const;

private:
    Type mType;

    QString mUrlOrFileName;
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

    void aboutWorkspace(const QString &pUrl);
    void addWorkspace(PMRSupport::PmrWorkspace *pWorkspace,
                      const bool &pOwned = false);
    QString addWorkspaceFolder(const QString &pFolder);
    void refreshWorkspace(const QString &pUrl);
    void refreshWorkspaceFile(const QString &pPath);
    void requestWorkspaces();

protected:
/*---GRY---
    virtual void contextMenuEvent(QContextMenuEvent *pEvent);
*/
    virtual void keyPressEvent(QKeyEvent *pEvent);
    virtual void mouseMoveEvent(QMouseEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);
    virtual QSize sizeHint() const;

private:
    PMRSupport::PmrWebService *mPmrWebService;

    PMRSupport::PmrWorkspaceManager *mWorkspaceManager;

    QMap<QString, QString> mWorkspaceFolderUrls;
    QMap<QString, QPair<QString, bool>> mWorkspaceUrlFolderMines;

    QString mCurrentWorkspaceUrl;
    QSet<QString> mExpandedItems;

    bool mInitialized;

    QString mErrorMessage;
    bool mAuthenticated;

    int mRow;

    QTimer *mTimer;

    QMenu *mContextMenu;

    QAction *mViewInPmrAction;
    QAction *mCopyUrlAction;

    Core::UserMessageWidget *mUserMessageWidget;

    QStandardItemModel *mTreeViewModel;
    Core::TreeViewWidget *mTreeViewWidget;

    void updateGui();

    PmrWorkspacesWindowItem * currentItem() const;

    void populateWorkspace(PmrWorkspacesWindowItem *pFolderItem,
                           PMRSupport::PmrWorkspaceFileNode *pFileNode);

    void expandHtmlTree(const QString &pId);

    void setCurrentWorkspaceUrl(const QString &pUrl);

    QString containerHtml(const QString &pClass, const QString &pId,
                          const QString &pIcon, const QString &pName,
                          const QString &pStatus, const StringPairs &pActions);

    QString fileNodeContentsHtml(const PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                 const bool &pHidden);
    QString emptyContentsHtml();

    QStringList workspaceHtml(const PMRSupport::PmrWorkspace *pWorkspace);

    static const QString actionHtml(const StringPairs &pActions);

    static QStringList fileStatusActionHtml(const QString &pPath,
                                            const PMRSupport::CharPair &pGitStatus);
    static QStringList fileStatusActionHtml(const PMRSupport::PmrWorkspace *pWorkspace,
                                            const QString &pPath);
    static QStringList fileStatusActionHtml(const PMRSupport::PmrWorkspaceFileNode *pFileNode);

    QString fileHtml(const PMRSupport::PmrWorkspaceFileNode *pFileNode);
    QStringList folderHtml(const PMRSupport::PmrWorkspaceFileNode *pFileNode);

/*---GRY---
    static QWebElement parentWorkspaceElement(const QWebElement &pRowElement);
*/

    void cloneWorkspace(const QString &pUrl);
    void duplicateCloneMessage(const QString &pUrl, const QString &pPath1,
                               const QString &pPath2);

    void commitWorkspace(const QString &pUrl);
    void synchronizeWorkspace(const QString &pUrl, const bool &pPush = true);

    void showInGraphicalShell(const QString &pPath);

    void startStopTimer();

signals:
    void openFileRequested(const QString &pFileName);
    void openFilesRequested(const QStringList &pFileNames);

    void information(const QString &pMessage);
    void warning(const QString &pMessage);

public slots:
    void initialize(const PMRSupport::PmrWorkspaces &pWorkspaces,
                    const QString &pErrorMessage = QString(),
                    const bool &pAuthenticated = true);

private slots:
    void showCustomContextMenu(const QPoint &pPosition) const;
    void itemDoubleClicked(const QModelIndex &pIndex);

    void resizeTreeViewToContents();

    void focusWindowChanged();
    void refreshWorkspaces();

    void workspaceCloned(PMRSupport::PmrWorkspace *pWorkspace);
    void workspaceSynchronized(PMRSupport::PmrWorkspace *pWorkspace);

    void viewInPmr();
    void copyUrl();
};

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
