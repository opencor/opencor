//==============================================================================
// Progress bar widget
//==============================================================================

#ifndef PROGRESSBARWIDGET_H
#define PROGRESSBARWIDGET_H

//==============================================================================

#include "coreglobal.h"
#include "widget.h"

//==============================================================================

#include <QWidget>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT ProgressBarWidget : public Widget
{
    Q_OBJECT

public:
    explicit ProgressBarWidget(QWidget *pParent = 0);

    void setValue(const double &pValue);

protected:
    virtual void paintEvent(QPaintEvent *pEvent);
    virtual void resizeEvent(QResizeEvent *pEvent);

private:
    int mWidth;

    double mOldValue;
    double mValue;
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
