#ifndef TREEVIEW_H
#define TREEVIEW_H

#include "commonwidget.h"

#include <QTreeView>

namespace OpenCOR {

class TreeView : public QTreeView, public CommonWidget
{
    Q_OBJECT

public:
    explicit TreeView(const QString &pName, QWidget *pWidget,
                      QWidget *pParent);

protected:
    virtual void startDrag(Qt::DropActions pSupportedActions);
};

}

#endif
