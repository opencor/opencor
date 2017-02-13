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
#include "editorwidgetglobal.h"

//==============================================================================

#include <QListView>
#include <QStandardItem>

//==============================================================================

class QAction;
class QMenu;

//==============================================================================

namespace OpenCOR {
namespace EditorWidget {

//==============================================================================

class EDITORWIDGET_EXPORT EditorListItem : public QStandardItem
{
public:
    enum Type {
        Error       = QStandardItem::UserType,
        Warning     = QStandardItem::UserType+1,
        Hint        = QStandardItem::UserType+2,
        Information = QStandardItem::UserType+3,
        Fatal       = QStandardItem::UserType+4
    };

    explicit EditorListItem(const Type &pType, const int &pLine,
                            const int &pColumn, const QString &pMessage);

    virtual int type() const;
    int line() const;
    int column() const;
    QString message() const;

private:
    Type mType;
    int mLine;
    int mColumn;
    QString mMessage;
};

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
    virtual void contextMenuEvent(QContextMenuEvent *pEvent);
    virtual void keyPressEvent(QKeyEvent *pEvent);

private:
    QStandardItemModel *mModel;

    QMenu *mContextMenu;

    QAction *mClearAction;
    QAction *mCopyToClipboardAction;

signals:
    void itemRequested(OpenCOR::EditorWidget::EditorListItem *pItem);

public slots:
    void clear();

private slots:
    void copyToClipboard();

    void requestItem(const QModelIndex &pItemIndex);
};

//==============================================================================

}   // namespace EditorWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
