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
// Editor list widget
//==============================================================================

#pragma once

//==============================================================================

#include "commonwidget.h"
#include "editorlistitem.h"
#include "editorwidgetglobal.h"

//==============================================================================

#include <QListView>

//==============================================================================

class QAction;
class QMenu;

//==============================================================================

namespace OpenCOR {
namespace EditorWidget {

//==============================================================================

class EDITORWIDGET_EXPORT EditorListWidget : public QListView,
                                             public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit EditorListWidget(QWidget *pParent);

    virtual void retranslateUi();

    void addItem(const EditorListItem::Type &pType, const int &pLine,
                 const int &pColumn, const QString &pMessage);
    void addItem(const EditorListItem::Type &pType, const int &pLine,
                 const QString &pMessage);
    void addItem(const EditorListItem::Type &pType, const QString &pMessage);

    int count() const;

    void selectFirstItem();

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);

private:
    QStandardItemModel *mModel;

    QMenu *mContextMenu;

    QAction *mClearAction;
    QAction *mCopyToClipboardAction;

Q_SIGNALS:
    void itemRequested(OpenCOR::EditorWidget::EditorListItem *pItem);

public Q_SLOTS:
    void clear();

private Q_SLOTS:
    void showCustomContextMenu() const;

    void copyToClipboard();

    void requestItem(const QModelIndex &pItemIndex);
};

//==============================================================================

}   // namespace EditorWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
