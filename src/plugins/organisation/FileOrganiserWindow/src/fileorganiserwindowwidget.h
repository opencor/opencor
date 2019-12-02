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
// File Organiser window widget
//==============================================================================

#pragma once

//==============================================================================

#include "treeviewwidget.h"

//==============================================================================

#include <QStandardItem>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class FileManager;
} // namespace Core

//==============================================================================

namespace FileOrganiserWindow {

//==============================================================================

class FileOrganiserWindowItem : public QStandardItem
{
public:
    explicit FileOrganiserWindowItem(const QIcon &pIcon,
                                     const QString &pTextOrPath,
                                     bool pFolder = false);
    ~FileOrganiserWindowItem() override;

    bool isFolder() const;

    bool isExpanded() const;
    void setExpanded(bool pExpanded);

    QString path() const;

private:
    bool mFolder;
    bool mExpanded = false;

    QString mPath;
};

//==============================================================================

class FileOrganiserWindowModel : public QStandardItemModel
{
    Q_OBJECT

public:
    explicit FileOrganiserWindowModel(QObject *pParent);

    QStringList mimeTypes() const override;
    QMimeData * mimeData(const QModelIndexList &pIndexes) const override;

    QByteArray encodeHierarchyData(const QModelIndex &pIndex) const;

    QModelIndex decodeHierarchyData(QByteArray &pData) const;
    QModelIndexList decodeData(QByteArray &pData) const;

    QString filePath(const QModelIndex &pFileIndex) const;

private:
    void encodeHierarchyData(const QModelIndex &pIndex, QDataStream &pStream,
                             int pLevel = 0) const;
    QByteArray encodeData(const QModelIndexList &pIndexes) const;

    QModelIndex decodeHierarchyData(QDataStream &pStream) const;
};

//==============================================================================

class FileOrganiserWindowWidget : public Core::TreeViewWidget
{
    Q_OBJECT

public:
    explicit FileOrganiserWindowWidget(QWidget *pParent);
    ~FileOrganiserWindowWidget() override;

    void loadSettings(QSettings &pSettings) override;
    void saveSettings(QSettings &pSettings) const override;

    void newFolder();
    void deleteItems();

    QString filePath(const QModelIndex &pFileIndex) const;

protected:
    void dragEnterEvent(QDragEnterEvent *pEvent) override;
    void dragMoveEvent(QDragMoveEvent *pEvent) override;
    void dropEvent(QDropEvent *pEvent) override;
    void keyPressEvent(QKeyEvent *pEvent) override;

private:
    Core::FileManager *mFileManager;
    FileOrganiserWindowModel *mModel;

    void loadItemSettings(QSettings &pSettings,
                          QStandardItem *pParentItem = nullptr);
    void saveItemSettings(QSettings &pSettings, QStandardItem *pItem,
                          int pParentItemIndex) const;

    QString newFolderName(QStandardItem *pFolderItem) const;
    void collapseEmptyFolders(QStandardItem *pFolder);

    QStandardItem *parentItem(QStandardItem *pDropItem,
                              const QAbstractItemView::DropIndicatorPosition &pDropPosition);

    bool ownedBy(const QString &pFileName, QStandardItem *pItem);

    void dropItems(QStandardItem *pDropItem,
                   const QAbstractItemView::DropIndicatorPosition &pDropPosition,
                   QStandardItem *pNewParentItem,
                   const QList<QStandardItem *> &pItems);
    void dropItem(QStandardItem *pDropItem,
                  const QAbstractItemView::DropIndicatorPosition &pDropPosition,
                  QStandardItem *pNewParentItem, QStandardItem *pItem);

    void addFile(const QString &pFileName, QStandardItem *pDropItem,
                 const QAbstractItemView::DropIndicatorPosition &pDropPosition);

    void moveItem(QStandardItem *pItem, QStandardItem *pDropItem,
                  const QAbstractItemView::DropIndicatorPosition &pDropPosition);

    bool parentIndexExists(const QModelIndex &pIndex,
                           const QModelIndexList &pIndexes) const;

    QModelIndexList cleanIndexList(const QModelIndexList &pIndexes) const;

    bool itemIsOrIsChildOf(QStandardItem *pItem,
                           QStandardItem *pOtherItem) const;

    void backupExpandedInformation(QStandardItem *pItem) const;
    void restoreExpandedInformation(QStandardItem *pItem);

    void selectFileItem(QStandardItem *pItem, const QString &pFileName) const;

    bool canCreateNewFolder() const;

signals:
    void openFilesRequested(const QStringList &pFileNames);

    void newFolderEnabled(bool pEnabled);
    void deleteItemsEnabled(bool pEnabled);

private slots:
    void resizeToContents();

    void emitItemsRelatedSignals();

    void fileDeleted(const QString &pFileName);
};

//==============================================================================

} // namespace FileOrganiserWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
