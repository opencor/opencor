#ifndef TABWIDGET_H
#define TABWIDGET_H

#include "commonwidget.h"

#include <QSvgRenderer>
#include <QTabWidget>

namespace OpenCOR {
namespace Core {

class TabWidget : public QTabWidget, public CommonWidget
{
    Q_OBJECT

public:
    explicit TabWidget(const QString &pLogoFileName, QWidget *pParent = 0);

protected:
    virtual void paintEvent(QPaintEvent *pEvent);

private:
    QSvgRenderer mLogoRenderer;

    int mLogoWidth;
    int mLogoHeight;

    double mLogoOneOverWidth;
    double mLogoOneOverHeight;

    double mLogoWidthOverHeight;
    double mLogoHeightOverWidth;

private Q_SLOTS:
    void tabChanged(const int &pIndex) const;
    void getFocus(QWidget *, QWidget *pNew) const;
};

} }

#endif
