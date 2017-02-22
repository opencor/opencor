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
// PMR Workspaces window synchronise dialog
//==============================================================================

#pragma once

//==============================================================================

#include "coreguiutils.h"

//==============================================================================

#include <QMap>

//==============================================================================

class QCheckBox;
class QDialogButtonBox;
class QListView;
class QSortFilterProxyModel;
class QStandardItem;
class QStandardItemModel;
class QTextEdit;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class SplitterWidget;
}   // namespace Core


//==============================================================================

namespace PMRSupport {
    class PmrWorkspace;
    class PmrWorkspaceFileNode;
}   // namespace PMRSupport

//==============================================================================

namespace WebViewerWidget {
    class WebViewerWidget;
}   // namespace WebViewerWidget

//==============================================================================

namespace PMRWorkspacesWindow {

//==============================================================================

class PmrWorkspacesWindowSynchronizeDialog : public Core::Dialog
{
    Q_OBJECT

public:
    explicit PmrWorkspacesWindowSynchronizeDialog(const QString &pSettingsGroup,
                                                  PMRSupport::PmrWorkspace *pWorkspace,
                                                  QWidget *pParent);
    ~PmrWorkspacesWindowSynchronizeDialog();

    QString message() const;

    QStringList fileNames() const;

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);

private:
    QString mSettingsGroup;

    PMRSupport::PmrWorkspace *mWorkspace;

    QString mDiffTemplate;

    Core::SplitterWidget *mHorizontalSplitter;
    Core::SplitterWidget *mVerticalSplitter;

    QStandardItemModel *mModel;
    QSortFilterProxyModel *mProxyModel;

    QTextEdit *mMessageValue;
    QListView *mChangesValue;

    WebViewerWidget::WebViewerWidget *mWebViewer;

    QCheckBox *mSelectAllChangesCheckBox;

    QDialogButtonBox *mButtonBox;

    QMap<QString, QString> mDiffHtmls;

    void populateModel(PMRSupport::PmrWorkspaceFileNode *pFileNode);

    QString diffHtml(const QString &pFileName);

private slots:
    void updateSelectAllChangesCheckBox(QStandardItem *pItem = 0);
    void selectAllChangesCheckBoxClicked();

    void updateOkButton();

    void acceptSynchronization();

    void updateDiffInformation();
};

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
