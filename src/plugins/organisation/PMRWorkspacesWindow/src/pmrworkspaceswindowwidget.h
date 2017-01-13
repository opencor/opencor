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

#include "pmrworkspacemanager.h"
#include "widget.h"

//==============================================================================

#include <QSet>
#include <QStandardItemModel>

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

typedef QPair<QString, QString> StringPair;
typedef QList<StringPair> StringPairs;

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

    QString mTemplate;
    QString mErrorMessage;
    bool mAuthenticated;

    int mRow;

    QTimer *mTimer;

    Core::UserMessageWidget *mUserMessageWidget;

    QStandardItemModel *mTreeViewModel;
    Core::TreeViewWidget *mTreeViewWidget;

    void updateMessage();

    void resizeTreeViewToContents();

    void displayWorkspaces();
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
    void openFileRequested(const QString &pFile);

    void information(const QString &pMessage);
    void warning(const QString &pMessage);

private slots:
    void focusWindowChanged();
    void refreshCurrentWorkspace();

public slots:
    void initialize(const PMRSupport::PmrWorkspaces &pWorkspaces,
                    const QString &pErrorMessage = QString(),
                    const bool &pAuthenticated = true);

    void workspaceCloned(PMRSupport::PmrWorkspace *pWorkspace);
    void workspaceSynchronized(PMRSupport::PmrWorkspace *pWorkspace);
};

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
