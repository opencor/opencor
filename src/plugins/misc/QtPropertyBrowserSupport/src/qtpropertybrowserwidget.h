//==============================================================================
// QtPropertyBrowserWidget class
//==============================================================================

#ifndef QTPROPERTYBROWSERWIDGET_H
#define QTPROPERTYBROWSERWIDGET_H

//==============================================================================

#include "qtpropertybrowsersupportglobal.h"

//==============================================================================

#include <QtTreePropertyBrowser>

//==============================================================================

namespace OpenCOR {
namespace QtPropertyBrowserSupport {

//==============================================================================

class QTPROPERTYBROWSERSUPPORT_EXPORT QtPropertyBrowserWidget : public QtTreePropertyBrowser
{
public:
    explicit QtPropertyBrowserWidget(QWidget *pParent = 0);
};

//==============================================================================

}   // namespace QtPropertyBrowserSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
