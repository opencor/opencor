//==============================================================================
// Widget
//==============================================================================

#include "widget.h"

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

Widget::Widget(const QSize &pSizeHint, QWidget *pParent) :
    QWidget(pParent),
    CommonWidget(pParent),
    mSizeHint(pSizeHint)
{
}

//==============================================================================

Widget::Widget(QWidget *pParent) :
    QWidget(pParent),
    CommonWidget(pParent),
    mSizeHint(defaultSize(0.15))
{
}

//==============================================================================

QSize Widget::sizeHint() const
{
    // Suggest our default size for the widget

    if (mSizeHint.isValid())
        return mSizeHint;
    else
        return QWidget::sizeHint();
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
