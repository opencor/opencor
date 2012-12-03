//==============================================================================
// QtPropertyBrowserWidget class
//==============================================================================

#include "qtpropertybrowserwidget.h"

//==============================================================================

#include <QtVariantPropertyManager>

//==============================================================================

namespace OpenCOR {
namespace QtPropertyBrowserSupport {

//==============================================================================

QtPropertyBrowserWidget::QtPropertyBrowserWidget(QWidget *pParent) :
    QtTreePropertyBrowser(pParent)
{
    // Customise ourselves

    mPropertyManager = new QtVariantPropertyManager(this);
    QtVariantEditorFactory *editorFactory = new QtVariantEditorFactory(this);

    setAlternatingRowColors(false);
#ifdef Q_WS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    // Note: the above removes the focus border since it messes up the look of
    //       our property editor...
//---GRY--- FOR SOME REASON, IT DOESN'T WORK, BUT WE STILL LEAVE IT HERE FOR
//          NOW...
#endif
    setFactoryForManager(mPropertyManager, editorFactory);
    setHeaderVisible(false);
    setPropertiesWithoutValueMarked(true);
    setRootIsDecorated(false);
}

//==============================================================================

QtVariantProperty * QtPropertyBrowserWidget::addProperty(const int pType,
                                                         const QString &pName)
{
    // Create a property

    QtVariantProperty *res = mPropertyManager->addProperty(pType, pName);

    // Add it to our GUI

    QtAbstractPropertyBrowser::addProperty(res);

    // Return it

    return res;
}

//==============================================================================

}   // namespace QScintillaSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
