#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include "commonwidget.h"

#include <QWidget>

namespace Ui {
    class CentralWidget;
}

class CentralWidget : public QWidget, public CommonWidget
{
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *pParent);
    ~CentralWidget();

protected:
    virtual void paintEvent(QPaintEvent *pEvent);

private:
    Ui::CentralWidget *mUi;
};

#endif
