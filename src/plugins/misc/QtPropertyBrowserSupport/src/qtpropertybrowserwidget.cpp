//==============================================================================
// QtPropertyBrowserWidget class
//==============================================================================

#include "qtpropertybrowserwidget.h"

//==============================================================================

#include <QTreeWidget>

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
    setFactoryForManager(mPropertyManager, editorFactory);
    setPropertiesWithoutValueMarked(true);
    setResizeMode(QtTreePropertyBrowser::Interactive);
    setRootIsDecorated(false);

#ifdef Q_WS_MAC
    treeWidget()->setAttribute(Qt::WA_MacShowFocusRect, 0);
    // Note: the above removes the focus border since it messes up the look of
    //       our property editor...
    treeWidget()->setFrameShape(QFrame::NoFrame);
#endif
}

//==============================================================================

void QtPropertyBrowserWidget::retranslateUi()
{
    // Retranslate our header labels

    treeWidget()->setHeaderLabels(QStringList() << tr("Property")
                                                << tr("Value"));
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
