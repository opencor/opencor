#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QDockWidget>

class DockWidget : public QDockWidget
{
    Q_OBJECT

public:
    DockWidget(QWidget *pParent = 0);
};

#endif
