//==============================================================================
// CellML annotation view widget
//==============================================================================

#include "cellmlannotationviewwidget.h"

//==============================================================================

#include "ui_cellmlannotationviewwidget.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewWidget::CellmlAnnotationViewWidget(const QString &pFileName,
                                                       QWidget *pParent) :
    Widget(pParent),
    mUi(new Ui::CellmlAnnotationViewWidget)
{
    // Set up the UI

    mUi->setupUi(this);
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
