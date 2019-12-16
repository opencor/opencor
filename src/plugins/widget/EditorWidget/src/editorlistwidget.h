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

namespace OpenCOR {
namespace EditorWidget {

//==============================================================================

class EDITORWIDGET_EXPORT EditorListItem : public QStandardItem
{
public:
    enum class Type {
        Error       = QStandardItem::UserType,
        Warning     = QStandardItem::UserType+1,
        Hint        = QStandardItem::UserType+2,
        Information = QStandardItem::UserType+3,
        Fatal       = QStandardItem::UserType+4
    };

    explicit EditorListItem(Type pType, int pLine, int pColumn,
                            const QString &pMessage, const QString &pFileName,
                            const QString &pFileInfo);

    int type() const override;
    int line() const;
    int column() const;
    QString message() const;
    QString fileName() const;
    QString fileInfo() const;

private:
    Type mType;
    int mLine;
    int mColumn;
    QString mMessage;
    QString mFileName;
    QString mFileInfo;
};

//==============================================================================

class EDITORWIDGET_EXPORT EditorListWidget : public QListView,
                                             public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit EditorListWidget(QWidget *pParent);

    void retranslateUi() override;

    void addItem(EditorListItem::Type pType, int pLine, int pColumn,
                 const QString &pMessage, const QString &pFileName = QString(),
                 const QString &pFileInfo = QString());
    void addItem(EditorListItem::Type pType, int pLine,
                 const QString &pMessage, const QString &pFileName = QString(),
                 const QString &pFileInfo = QString());
    void addItem(EditorListItem::Type pType, const QString &pMessage,
                 const QString &pFileName = QString(),
                 const QString &pFileInfo = QString());

    int count() const;

    void selectFirstItem();

protected:
    void contextMenuEvent(QContextMenuEvent *pEvent) override;
    void keyPressEvent(QKeyEvent *pEvent) override;

private:
    QStandardItemModel *mModel;

    QMenu *mContextMenu;

    QAction *mClearAction;
    QAction *mCopyToClipboardAction;

signals:
    void itemRequested(EditorListItem *pItem);

public slots:
    void clear();

private slots:
    void copyToClipboard();

    void requestItem(const QModelIndex &pItemIndex);
};

//==============================================================================

} // namespace EditorWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
