#ifndef TABWIDGET_H
#define TABWIDGET_H

#include "commonwidget.h"

#include <QTabWidget>

namespace OpenCOR {
namespace Core {

class TabWidget : public QTabWidget, public CommonWidget
{
    Q_OBJECT

public:
    explicit TabWidget(QWidget *pParent);

protected:
    virtual void paintEvent(QPaintEvent *pEvent);

private:
    QPixmap mLogo;
    QBrush mBackgroundBrush;

private Q_SLOTS:
    void tabChanged(const int &pIndex) const;
    void getFocus(QWidget *, QWidget *pNew) const;
};

} }

#endif
