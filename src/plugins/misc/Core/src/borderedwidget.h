//==============================================================================
// Bordered widget
//==============================================================================

#ifndef BORDEREDWIDGET_H
#define BORDEREDWIDGET_H

//==============================================================================

#include "widget.h"
#include "coreglobal.h"

//==============================================================================

class QFrame;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT BorderedWidget : public Widget
{
    Q_OBJECT

public:
    explicit BorderedWidget(QWidget *pWidget,
                            const bool &pTop, const bool &pLeft,
                            const bool &pBottom, const bool &pRight);

    QWidget * widget();

    void setTopBorderVisible(const bool &pVisible) const;
    void setLeftBorderVisible(const bool &pVisible) const;
    void setBottomBorderVisible(const bool &pVisible) const;
    void setRightBorderVisible(const bool &pVisible) const;

private:
    QWidget *mWidget;

    QFrame *mTopBorder;
    QFrame *mLeftBorder;
    QFrame *mBottomBorder;
    QFrame *mRightBorder;
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
