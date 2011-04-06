#include "treeview.h"

TreeView::TreeView(const QString &pName, QWidget *pWidget,
                   QWidget *pParent) :
    QTreeView(pParent),
    CommonWidget(pName, pWidget, pParent)
{
}
