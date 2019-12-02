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
// PMR window widget
//==============================================================================

#pragma once

//==============================================================================

#include "pmrexposure.h"
#include "treeviewwidget.h"

//==============================================================================

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

namespace PMRWindow {

//==============================================================================

class PmrWindowItem : public QStandardItem
{
public:
    enum class Type {
        Exposure     = QStandardItem::UserType,
        ExposureFile = QStandardItem::UserType+1
    };

    explicit PmrWindowItem(Type pType, const QString &pText,
                           const QString &pUrl);

    int type() const override;

    QString url() const;

private:
    Type mType;

    QString mUrl;
};

//==============================================================================

class PmrWindowWidget : public Core::TreeViewWidget
{
    Q_OBJECT

public:
    explicit PmrWindowWidget(QWidget *pParent);

    void retranslateUi() override;

    void initialize(const PMRSupport::PmrExposures &pExposures,
                    const QString &pFilter,
                    const QString &pErrorMessage);

    void filter(const QString &pFilter);

    bool hasExposures() const;

protected:
    void keyPressEvent(QKeyEvent *pEvent) override;

private:
    QMenu *mContextMenu;

    QAction *mViewInPmrAction;
    QAction *mCopyUrlAction;
    QAction *mMakeLocalCopyAction;

    Core::UserMessageWidget *mUserMessageWidget;

    QStandardItemModel *mTreeViewModel;

    QStringList mExposureNames;

    bool mInitialized = false;

    QString mErrorMessage;

    int mNumberOfFilteredExposures = 0;

    QStringList mDontExpandExposures;

    void updateGui(bool pForceUserMessageVisibility = false);

    PmrWindowItem * currentItem() const;

signals:
    void cloneWorkspaceRequested(const QString &pUrl);
    void exposureFilesRequested(const QString &pUrl);

    void openExposureFileRequested(const QString &pUrl);
    void openExposureFilesRequested(const QStringList &pUrls);

    void itemDoubleClicked();

public slots:
    void addAndShowExposureFiles(const QString &pUrl,
                                 const QStringList &pExposureFiles);

private slots:
    void showCustomContextMenu(const QPoint &pPosition) const;
    void itemDoubleClicked(const QModelIndex &pIndex);

    void resizeTreeViewToContents();

    void viewInPmr();
    void copyUrl();
    void makeLocalCopy();
};

//==============================================================================

} // namespace PMRWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
