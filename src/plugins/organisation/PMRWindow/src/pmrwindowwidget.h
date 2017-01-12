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
// PMR window widget
//==============================================================================

#pragma once

//==============================================================================

#include "corecliutils.h"
#include "pmrexposure.h"
#include "widget.h"

//==============================================================================

#include <QStandardItem>

//==============================================================================

class QLabel;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class TreeViewWidget;
    class UserMessageWidget;
}   // namespace Core

//==============================================================================

namespace PMRWindow {

//==============================================================================

class PmrWindowItem : public QStandardItem
{
public:
    enum Type {
        Exposure     = QStandardItem::UserType,
        ExposureFile = QStandardItem::UserType+1
    };

    explicit PmrWindowItem(const Type &pType, const QString &pText,
                           const QString &pUrl);

    virtual int type() const;

    QString url() const;

private:
    Type mType;
    QString mUrl;
};

//==============================================================================

class PmrWindowWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit PmrWindowWidget(QWidget *pParent);

    virtual void retranslateUi();

    void initialize(const PMRSupport::PmrExposures &pExposures,
                    const QString &pFilter,
                    const QString &pErrorMessage);

    void filter(const QString &pFilter);

    bool hasExposures() const;

protected:
    virtual QSize sizeHint() const;

private:
    QAction *mViewInPmrAction;
    QAction *mCopyUrlAction;
    QAction *mCloneWorkspaceAction;

    Core::UserMessageWidget *mUserMessageWidget;

    QStandardItemModel *mTreeViewModel;
    Core::TreeViewWidget *mTreeViewWidget;

    QStringList mExposureNames;

    bool mInitialized;

    QString mErrorMessage;

    int mNumberOfFilteredExposures;

    void updateMessage();

    void resizeTreeViewToContents();

    PmrWindowItem * currentItem() const;

signals:
    void cloneWorkspaceRequested(const QString &pUrl);
    void exposureFilesRequested(const QString &pUrl);

    void openExposureFileRequested(const QString &pUrl);

    void itemDoubleClicked();

public slots:
    void addAndShowExposureFiles(const QString &pUrl,
                                 const QStringList &pExposureFiles);

private slots:
    void showCustomContextMenu(const QPoint &pPosition) const;
    void itemDoubleClicked(const QModelIndex &pIndex);

    void expandedExposure(const QModelIndex &pExposureIndex);
    void collapsedExposure(const QModelIndex &pExposureIndex);

    void viewInPmr();
    void copyUrl();
    void cloneWorkspace();
};

//==============================================================================

}   // namespace PMRWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
