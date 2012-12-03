//==============================================================================
// QtPropertyBrowserWidget class
//==============================================================================

#ifndef QTPROPERTYBROWSERWIDGET_H
#define QTPROPERTYBROWSERWIDGET_H

//==============================================================================

#include "commonwidget.h"
#include "qtpropertybrowsersupportglobal.h"

//==============================================================================

#include <QtTreePropertyBrowser>

//==============================================================================

class QtVariantProperty;
class QtVariantPropertyManager;

//==============================================================================

namespace OpenCOR {
namespace QtPropertyBrowserSupport {

//==============================================================================

class QTPROPERTYBROWSERSUPPORT_EXPORT QtPropertyBrowserWidget : public QtTreePropertyBrowser,
                                                                public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit QtPropertyBrowserWidget(QWidget *pParent = 0);

    virtual void retranslateUi();

    QtVariantProperty * addProperty(const int pType, const QString &pName);

private:
    QtVariantPropertyManager *mPropertyManager;
};

//==============================================================================

}   // namespace QtPropertyBrowserSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
