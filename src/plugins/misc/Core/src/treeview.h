#ifndef TREEVIEW_H
#define TREEVIEW_H

#include "commonwidget.h"
#include "coreglobal.h"

#include <QTreeView>

namespace OpenCOR {

class CORE_EXPORT TreeView : public QTreeView, public CommonWidget
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
