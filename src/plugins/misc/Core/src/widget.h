//==============================================================================
// Widget
//==============================================================================

#ifndef WIDGET_H
#define WIDGET_H

//==============================================================================

#include "commonwidget.h"
#include "coreglobal.h"

//==============================================================================

#include <QWidget>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT Widget : public QWidget, public CommonWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *pParent);

protected:
    virtual QSize sizeHint() const;
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
