#ifndef FILEORGANISERWIDGET_H
#define FILEORGANISERWIDGET_H

#include "commonwidget.h"

#include <QTreeView>

class QStandardItem;
class QStandardItemModel;

class FileOrganiserWidget : public QTreeView, public CommonWidget
{
    Q_OBJECT

public:
    FileOrganiserWidget(const QString &pName, QWidget *pParent);

    bool newFolder(const QModelIndex &pItemIndex);
    bool deleteItems(const QModelIndex &pItemIndex);

protected:
    virtual QSize sizeHint() const;

    virtual void mousePressEvent(QMouseEvent *pEvent);

private:
    QStandardItemModel *mDataModel;

    QString newFolderName(QStandardItem *pFolderItem);

private slots:
    void resizeToContents();
};

#endif
