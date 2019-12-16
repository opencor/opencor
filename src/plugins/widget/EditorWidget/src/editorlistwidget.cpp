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

#include "corecliutils.h"
#include "coreguiutils.h"
#include "editorlistwidget.h"
#include "i18ninterface.h"

//==============================================================================

#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QMenu>

//==============================================================================

namespace OpenCOR {
namespace EditorWidget {

//==============================================================================

EditorListItem::EditorListItem(Type pType, int pLine, int pColumn,
                               const QString &pMessage,
                               const QString &pFileName,
                               const QString &pFileInfo) :
    mType(pType),
    mLine(pLine),
    mColumn(pColumn),
    mMessage(pMessage),
    mFileName(pFileName),
    mFileInfo(pFileInfo)
{
    // Customise ourselves

    static const QIcon ErrorIcon       = QIcon(":/oxygen/emblems/emblem-important.png");
    static const QIcon WarningIcon     = QIcon(":/oxygen/status/task-attention.png");
    static const QIcon HintIcon        = QIcon(":/oxygen/actions/help-hint.png");
    static const QIcon InformationIcon = QIcon(":/oxygen/actions/help-about.png");
    static const QIcon FatalIcon       = QIcon(":/oxygen/status/edit-bomb.png");

    if ((pLine == -1) && (pColumn == -1)) {
        if (pFileInfo.isEmpty()) {
            setText(pMessage);
        } else {
            setText(QString("[%1] %s").arg(pFileInfo,
                                           pMessage));
        }
    } else if (pColumn == -1) {
        if (pFileInfo.isEmpty()) {
            setText(QString("[%1] %2").arg(pLine)
                                      .arg(pMessage));
        } else {
            setText(QString("[%1:%2] %3").arg(pFileInfo)
                                         .arg(pLine)
                                         .arg(pMessage));
        }
    } else {
        if (pFileInfo.isEmpty()) {
            setText(QString("[%1:%2] %3").arg(pLine)
                                         .arg(pColumn)
                                         .arg(pMessage));
        } else {
            setText(QString("[%1:%2:%3] %4").arg(pFileInfo)
                                            .arg(pLine)
                                            .arg(pColumn)
                                            .arg(pMessage));
        }
    }

    setToolTip(text());

    switch (pType) {
    case Type::Error:
        setIcon(ErrorIcon);

        break;
    case Type::Warning:
        setIcon(WarningIcon);

        break;
    case Type::Hint:
        setIcon(HintIcon);

        break;
    case Type::Information:
        setIcon(InformationIcon);

        break;
    case Type::Fatal:
        setIcon(FatalIcon);

        break;
    }
}

//==============================================================================

int EditorListItem::type() const
{
    // Return the item's type

    return int(mType);
}

//==============================================================================

int EditorListItem::line() const
{
    // Return the item's line

    return mLine;
}

//==============================================================================

int EditorListItem::column() const
{
    // Return the item's column

    return mColumn;
}

//==============================================================================

QString EditorListItem::message() const
{
    // Return the item's message

    return mMessage;
}

//==============================================================================

QString EditorListItem::fileName() const
{
    // Return the item's file name

    return mFileName;
}

//==============================================================================

EditorListWidget::EditorListWidget(QWidget *pParent) :
    QListView(pParent),
    Core::CommonWidget(this),
    mModel(new QStandardItemModel(this))
{
    // Customise ourselves

#ifdef Q_OS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, false);
#endif
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setFrameShape(QFrame::NoFrame);
    setModel(mModel);

    // Create our context menu

    mContextMenu = new QMenu(this);

    mClearAction = Core::newAction(this);
    mCopyToClipboardAction = Core::newAction(this);

    connect(mClearAction, &QAction::triggered,
            this, &EditorListWidget::clear);
    connect(mCopyToClipboardAction, &QAction::triggered,
            this, &EditorListWidget::copyToClipboard);

    mContextMenu->addAction(mClearAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mCopyToClipboardAction);

    // Make sure that we are properly initialised

    clear();

    // A connection to handle a double click on a given item

    connect(this, &EditorListWidget::doubleClicked,
            this, &EditorListWidget::requestItem);

    // Retranslate ourselves, so that our actions are properly initialised

    retranslateUi();
}

//==============================================================================

void EditorListWidget::retranslateUi()
{
    // Retranslate our actions

    I18nInterface::retranslateAction(mClearAction, tr("Clear List"),
                                     tr("Clear the list"));
    I18nInterface::retranslateAction(mCopyToClipboardAction, tr("Copy List to Clipboard"),
                                     tr("Copy the list to the clipboard"));
}

//==============================================================================

void EditorListWidget::clear()
{
    // Reset our list of items

    mModel->clear();

    mClearAction->setEnabled(false);
    mCopyToClipboardAction->setEnabled(false);
}

//==============================================================================

void EditorListWidget::addItem(EditorListItem::Type pType,
                               int pLine, int pColumn, const QString &pMessage,
                               const QString &pFileName,
                               const QString &pFileInfo)
{
    // Add the given item to our list

    mModel->invisibleRootItem()->appendRow(new EditorListItem(pType, pLine, pColumn, pMessage, pFileName, pFileInfo));

    mClearAction->setEnabled(true);
    mCopyToClipboardAction->setEnabled(true);
}

//==============================================================================

void EditorListWidget::addItem(EditorListItem::Type pType, int pLine,
                               const QString &pMessage,
                               const QString &pFileName,
                               const QString &pFileInfo)
{
    // Add the given item to our list

    addItem(pType, pLine, -1, pMessage, pFileName, pFileInfo);
}

//==============================================================================

void EditorListWidget::addItem(EditorListItem::Type pType,
                               const QString &pMessage,
                               const QString &pFileName,
                               const QString &pFileInfo)
{
    // Add the given item to our list

    addItem(pType, -1, -1, pMessage, pFileName, pFileInfo);
}

//==============================================================================

int EditorListWidget::count() const
{
    // Return the number of items we hold

    return mModel->invisibleRootItem()->rowCount();
}

//==============================================================================

void EditorListWidget::selectFirstItem()
{
    // Select the first item in our list and 'request' it

    QStandardItem *firstItem = mModel->invisibleRootItem()->child(0);

    if (firstItem != nullptr) {
        QModelIndex firstItemIndex = firstItem->index();

        setCurrentIndex(firstItemIndex);

        requestItem(firstItemIndex);
    }
}

//==============================================================================

void EditorListWidget::contextMenuEvent(QContextMenuEvent *pEvent)
{
    // Show our custom context menu

    mContextMenu->exec(pEvent->globalPos());
}

//==============================================================================

void EditorListWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Default handling of the event

    QListView::keyPressEvent(pEvent);

    // Check whether the user wants the current item to be requested

    if ((pEvent->key() == Qt::Key_Enter) || (pEvent->key() == Qt::Key_Return)) {
        requestItem(currentIndex());
    }
}

//==============================================================================

void EditorListWidget::copyToClipboard()
{
    // Copy our list to the clipboard

    QStringList list = QStringList();

    for (int i = 0, iMax = mModel->rowCount(); i < iMax; ++i) {
        auto item = static_cast<EditorListItem *>(mModel->item(i));
        QString itemType;

        switch (EditorListItem::Type(item->type())) {
        case EditorListItem::Type::Error:
            itemType = tr("Error");

            break;
        case EditorListItem::Type::Warning:
            itemType = tr("Warning");

            break;
        case EditorListItem::Type::Hint:
            itemType = tr("Hint");

            break;
        case EditorListItem::Type::Information:
            itemType = tr("Information");

            break;
        case EditorListItem::Type::Fatal:
            itemType = tr("Fatal");

            break;
        }

        list << "["+itemType+"] "+item->text();
    }

    QApplication::clipboard()->setText(list.join(Core::eolString())+Core::eolString());
}

//==============================================================================

void EditorListWidget::requestItem(const QModelIndex &pItemIndex)
{
    // Check what kind of item has been double clicked and if it is a file, then
    // open it

    auto item = static_cast<EditorListItem *>(mModel->itemFromIndex(pItemIndex));

    if ((item != nullptr) && (item->line() != -1)) {
        emit itemRequested(item);
    }
}

//==============================================================================

} // namespace EditorWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
