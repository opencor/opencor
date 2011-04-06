#ifndef TREEVIEW_H
#define TREEVIEW_H

#include "commonwidget.h"

#include <QTreeView>

class TreeView : public QTreeView, public CommonWidget
{
    Q_OBJECT

public:
    explicit TreeView(const QString &pName, QWidget *pWidget,
                      QWidget *pParent);
};

#endif
