/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Editor list widget
//==============================================================================

#ifndef EDITORLISTWIDGET_H
#define EDITORLISTWIDGET_H

//==============================================================================

#include "commonwidget.h"
#include "editorlistglobal.h"
#include "editorlistitem.h"

//==============================================================================

#include <QListView>

//==============================================================================

class QAction;
class QMenu;

//==============================================================================

namespace OpenCOR {
namespace EditorList {

//==============================================================================

class EDITORLIST_EXPORT EditorListWidget : public QListView,
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
    void itemRequested(EditorList::EditorListItem *pItem);

public Q_SLOTS:
    void clear();

private Q_SLOTS:
    void showCustomContextMenu(const QPoint &pPosition) const;

    void copyToClipboard();

    void requestItem(const QModelIndex &pItemIndex);
};

//==============================================================================

}   // namespace EditorList
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
