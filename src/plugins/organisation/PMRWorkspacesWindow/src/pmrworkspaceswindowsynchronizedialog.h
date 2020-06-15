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
// PMR Workspaces window synchronise dialog
//==============================================================================

#pragma once

//==============================================================================

#include "coreguiutils.h"

//==============================================================================

#include <QMap>
#include <QModelIndexList>
#include <QStandardItem>

//==============================================================================

class QCheckBox;
class QDialogButtonBox;
class QListView;
class QSortFilterProxyModel;
class QTextEdit;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class SplitterWidget;
} // namespace Core

//==============================================================================

namespace PMRSupport {
    class PmrWorkspace;
    class PmrWorkspaceFileNode;
} // namespace PMRSupport

//==============================================================================

namespace WebViewerWidget {
    class WebViewerWidget;
} // namespace WebViewerWidget

//==============================================================================

namespace PMRWorkspacesWindow {

//==============================================================================

class PmrWorkspacesWindowSynchronizeDialogItem : public QStandardItem
{
public:
    explicit PmrWorkspacesWindowSynchronizeDialogItem(PMRSupport::PmrWorkspaceFileNode *pFileNode);
    ~PmrWorkspacesWindowSynchronizeDialogItem() override;

    PMRSupport::PmrWorkspaceFileNode * fileNode() const;

private:
    PMRSupport::PmrWorkspaceFileNode *mFileNode;
};

//==============================================================================

using PmrWorkspacesWindowSynchronizeDialogItems = QList<PmrWorkspacesWindowSynchronizeDialogItem *>;

//==============================================================================

class PmrWorkspacesWindowSynchronizeDialogDifferenceData
{
public:
    explicit PmrWorkspacesWindowSynchronizeDialogDifferenceData(const QString &pOperation,
                                                                const QChar &pTag,
                                                                const QString &pAddLineNumber,
                                                                const QString &pRemoveLineNumber,
                                                                const QString &pDifference);

    QString operation() const;
    QChar tag() const;
    QString addLineNumber() const;
    QString removeLineNumber() const;
    QString difference() const;

private:
    QString mOperation;
    QChar mTag;
    QString mAddLineNumber;
    QString mRemoveLineNumber;
    QString mDifference;
};

//==============================================================================

using PmrWorkspacesWindowSynchronizeDialogDifferencesData = QList<PmrWorkspacesWindowSynchronizeDialogDifferenceData>;

//==============================================================================

class PmrWorkspacesWindowSynchronizeDialog : public Core::Dialog
{
    Q_OBJECT

public:
    explicit PmrWorkspacesWindowSynchronizeDialog(PMRSupport::PmrWorkspace *pWorkspace,
                                                  QTimer *pTimer,
                                                  QWidget *pParent);
    ~PmrWorkspacesWindowSynchronizeDialog() override;

    QString message() const;

    QStringList fileNames() const;

protected:
    void keyPressEvent(QKeyEvent *pEvent) override;

private:
    PMRSupport::PmrWorkspace *mWorkspace;

    QString mDiffTemplate;

    Core::SplitterWidget *mHorizontalSplitter;
    Core::SplitterWidget *mVerticalSplitter;

    QStandardItemModel *mModel;
    QSortFilterProxyModel *mProxyModel;

    QListView *mChangesValue;
    QTextEdit *mMessageValue;

    WebViewerWidget::WebViewerWidget *mWebViewer;
    QAction *mWebViewerCellmlTextFormatAction;

    QCheckBox *mSelectAllChangesCheckBox;

    QDialogButtonBox *mButtonBox;

    QMap<QString, QString> mSha1s;

    QMap<QString, QString> mDiffHtmls;
    QMap<QString, QString> mCellmlDiffHtmls;

    int mNbOfCheckableFiles = 0;

    QModelIndexList mPreviouslySelectedIndexes;

    QStringList mInvalidCellmlCode;

    bool mNeedUpdateDiffInformation = false;

    PmrWorkspacesWindowSynchronizeDialogItems populateModel(PMRSupport::PmrWorkspaceFileNode *pFileNode);

    bool cellmlText(const QString &pFileName, QString &pCellmlText);

    QString diffHtml(PmrWorkspacesWindowSynchronizeDialogDifferencesData &pDifferencesData);
    QString diffHtml(const QString &pOld, const QString &pNew);
    QString diffHtml(const QString &pFileName);

    QString cleanHtmlEscaped(const QString &pString);

private slots:
    void refreshChanges();

    void updateSelectAllChangesCheckBox(QStandardItem *pItem = nullptr);
    void selectAllChangesCheckBoxClicked();

    void updateOkButton();

    void acceptSynchronization();

    void updateDiffInformation();
};

//==============================================================================

} // namespace PMRWorkspacesWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
