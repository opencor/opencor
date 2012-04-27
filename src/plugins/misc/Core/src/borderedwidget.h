//==============================================================================
// Bordered widget
//==============================================================================

#ifndef BORDEREDWIDGET_H
#define BORDEREDWIDGET_H

//==============================================================================

#include "widget.h"
#include "coreglobal.h"

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT BorderedWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit BorderedWidget(QWidget *pWidget,
                            const bool &pTop, const bool &pLeft,
                            const bool &pBottom, const bool &pRight);

    QWidget * widget();

private:
    QWidget *mWidget;
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
