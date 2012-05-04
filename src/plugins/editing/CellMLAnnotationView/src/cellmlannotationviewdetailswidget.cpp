//==============================================================================
// CellML annotation view details widget
//==============================================================================

#include "cellmlannotationviewdetailswidget.h"

//==============================================================================

#include "ui_cellmlannotationviewdetailswidget.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewDetailsWidget::CellmlAnnotationViewDetailsWidget(QWidget *pParent) :
    Widget(pParent),
    mUi(new Ui::CellmlAnnotationViewDetailsWidget)
{
    // Set up the UI

    mUi->setupUi(this);
}

//==============================================================================

CellmlAnnotationViewDetailsWidget::~CellmlAnnotationViewDetailsWidget()
{
    // Delete the UI

    delete mUi;
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
