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

    bool newFolder();
    bool deleteItems();

protected:
    virtual QSize sizeHint() const;

private:
    QStandardItemModel *mDataModel;

    QString newFolderName(QStandardItem *pFolderItem);
};

#endif
