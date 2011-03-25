#ifndef TABWIDGET_H
#define TABWIDGET_H

#include "commonwidget.h"

#include <QTabWidget>

class TabWidget : public QTabWidget, public CommonWidget
{
    Q_OBJECT

public:
    TabWidget(QWidget *pParent);

protected:
    virtual void paintEvent(QPaintEvent *pEvent);
};

#endif
