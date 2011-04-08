#ifndef TABWIDGET_H
#define TABWIDGET_H

#include "commonwidget.h"

#include <QTabWidget>

class TabWidget : public QTabWidget, public CommonWidget
{
    Q_OBJECT

public:
    explicit TabWidget(QWidget *pParent);

protected:
    virtual void paintEvent(QPaintEvent *pEvent);

private Q_SLOTS:
    void activateWidget(const int &pIndex);
};

#endif
