//==============================================================================
// File organiser widget
//==============================================================================

#ifndef FILEORGANISERWIDGET_H
#define FILEORGANISERWIDGET_H

//==============================================================================

#include "filemanager.h"
#include "fileorganisermodel.h"
#include "treeviewwidget.h"

//==============================================================================

namespace OpenCOR {
namespace FileOrganiser {

//==============================================================================

class FileOrganiserWidget : public Core::TreeViewWidget
{
    Q_OBJECT

public:
    explicit FileOrganiserWidget(QWidget *pParent = 0);
    ~FileOrganiserWidget();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void newFolder();
    void deleteItems();

    QString filePath(const QModelIndex &pFileIndex) const;

protected:
    virtual bool viewportEvent(QEvent *pEvent);

    virtual void dragEnterEvent(QDragEnterEvent *pEvent);
    virtual void dragMoveEvent(QDragMoveEvent *pEvent);
    virtual void dropEvent(QDropEvent *pEvent);

    virtual void keyPressEvent(QKeyEvent *pEvent);

private:
    FileOrganiserModel *mDataModel;
    Core::FileManager *mFileManager;

    void loadItemSettings(QSettings *pSettings, QStandardItem *pParentItem);
    void saveItemSettings(QSettings *pSettings, QStandardItem *pItem,
                          const int &pParentItemIndex) const;

    QString newFolderName(QStandardItem *pFolderItem) const;
    void collapseEmptyFolders(QStandardItem *pFolder);

    QStandardItem *parentItem(QStandardItem *pDropItem,
                              const QAbstractItemView::DropIndicatorPosition &pDropPosition);

    bool ownedBy(const QString &pFileName, QStandardItem *pItem);

    void dropItems(QStandardItem *pDropItem,
                   const QAbstractItemView::DropIndicatorPosition &pDropPosition,
                   QStandardItem *pNewParentItem, QList<QStandardItem *> pItems);
    void dropItem(QStandardItem *pDropItem,
                  const QAbstractItemView::DropIndicatorPosition &pDropPosition,
                  QStandardItem *pNewParentItem, QStandardItem *pItem);

    void addFile(const QString &pFileName, QStandardItem *pDropItem,
                 const QAbstractItemView::DropIndicatorPosition &pDropPosition);

    void moveItem(QStandardItem *pItem, QStandardItem *pDropItem,
                  const QAbstractItemView::DropIndicatorPosition &pDropPosition);

    void resizeToContents();

    QStringList selectedFiles() const;

    bool parentIndexExists(const QModelIndex &pIndex,
                           const QModelIndexList &pIndexes) const;

    QModelIndexList cleanIndexList(const QModelIndexList &pIndexes) const;

    bool itemIsOrIsChildOf(QStandardItem *pItem, QStandardItem *pOtherItem) const;

    void backupExpandedInformation(QStandardItem *pItem) const;
    void restoreExpandedInformation(QStandardItem *pItem);

    void updateFileItems(QStandardItem *pItem, const QString &pFileName,
                         const Core::File::Status &pStatus) const;

    bool canCreateNewFolder() const;

Q_SIGNALS:
    void filesOpened(const QStringList &pFileNames);

    void newFolderEnabled(const bool &pEnabled);
    void deleteItemsEnabled(const bool &pEnabled);

private Q_SLOTS:
    void expandedFolder(const QModelIndex &pFolderIndex);
    void collapsedFolder(const QModelIndex &pFolderIndex);

    void emitItemsRelatedSignals();

    void fileContentsChanged(const QString &pFileName) const;
    void fileDeleted(const QString &pFileName) const;
};

//==============================================================================

}   // namespace FileOrganiser
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
