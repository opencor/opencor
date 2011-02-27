#ifndef CELLMLMODELREPOSITORYWIDGET_H
#define CELLMLMODELREPOSITORYWIDGET_H

#include "commonwidget.h"

#include <QWebView>

class CellmlModelRepositoryWidget : public QWebView, public CommonWidget
{
    Q_OBJECT

public:
    CellmlModelRepositoryWidget(QWidget *pParent = 0);

protected:
    virtual QSize sizeHint() const;

    virtual void paintEvent(QPaintEvent *pEvent);
};

#endif
