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
#include "webviewerwidget.h"

//==============================================================================

namespace OpenCOR {

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

class PmrWorkspacesWindowWidget : public WebViewerWidget::WebViewerWidget
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
    void addWorkspace(PMRSupport::PmrWorkspace *pWorkspace, const bool &pOwned=false);
    const QString addWorkspaceFolder(const QString &pFolder);
    void refreshWorkspace(const QString &pUrl);
    void refreshWorkspaceFile(const QString &pPath);
    void requestWorkspaces();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *pEvent);
    virtual void mouseMoveEvent(QMouseEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);
    virtual QSize sizeHint() const;

private:
    PMRSupport::PmrWebService *mPmrWebService;

    PMRSupport::PmrWorkspaceManager *mWorkspaceManager;

    QMap<QString, QString> mWorkspaceFolderUrls;
    QMap<QString, QPair<QString, bool>> mUrlFolderNameMines;

    QString mCurrentWorkspaceUrl;
    QSet<QString> mExpandedItems;

    bool mInitialized;

    QString mTemplate;
    QString mErrorMessage;
    bool mAuthenticated;

    int mRow;
    int mRowAnchor;
    QMap<QString, int> mItemAnchors;

    QTimer *mTimer;

    QString message() const;

    void displayWorkspaces();
    void expandHtmlTree(const QString &pId);

    void setCurrentWorkspaceUrl(const QString &pUrl);

    static const QString actionHtml(const StringPairs &pActions);
    QString containerHtml(const QString &pClass, const QString &pIcon,
                          const QString &pId, const QString &pName,
                          const QString &pStatus,
                          const StringPairs &pActionList);
    QString contentsHtml(const PMRSupport::PmrWorkspaceFileNode *pFileNode, const bool &pHidden);
    QString emptyContentsHtml();

    static QStringList fileStatusActionHtml(const QString &pPath,
                                            const PMRSupport::CharPair &pGitStatus);
    static QStringList fileStatusActionHtml(const PMRSupport::PmrWorkspace *pWorkspace,
                                            const QString &pPath);
    static QStringList fileStatusActionHtml(const PMRSupport::PmrWorkspaceFileNode *pFileNode);

    QString fileHtml(const PMRSupport::PmrWorkspaceFileNode *pFileNode);
    QStringList folderHtml(const PMRSupport::PmrWorkspaceFileNode *pFileNode);
    QStringList workspaceHtml(const PMRSupport::PmrWorkspace *pWorkspace);

    static QWebElement parentWorkspaceElement(const QWebElement &pRowElement);

    void cloneWorkspace(const QString &pUrl);
    void duplicateCloneMessage(const QString &pUrl,
                               const QString &pPath1, const QString &pPath2);

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
