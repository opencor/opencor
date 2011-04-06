#ifndef DOCKTOOLBAR_H
#define DOCKTOOLBAR_H

#include <QToolBar>

class DockToolBar : public QToolBar
{
    Q_OBJECT

public:
    explicit DockToolBar(QWidget *pParent);
};

#endif
