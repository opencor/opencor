#ifndef FILEORGANISERWIDGET_H
#define FILEORGANISERWIDGET_H

#include "commonwidget.h"

#include <QTreeView>

class FileOrganiserWidget : public QTreeView, public CommonWidget
{
    Q_OBJECT

public:
    FileOrganiserWidget(const QString &pName, QWidget *pParent);

protected:
    virtual QSize sizeHint() const;
};

#endif
