#ifndef DOCKTOOLBAR_H
#define DOCKTOOLBAR_H

#include <QToolBar>

class DockToolBar : public QToolBar
{
    Q_OBJECT

public:
    DockToolBar(QWidget *pParent);
};

#endif
