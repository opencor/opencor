//==============================================================================
// CellML annotation view CellML details widget
//==============================================================================

#include "cellmlannotationviewcellmldetailswidget.h"
#include "cellmlannotationviewdetailswidget.h"

//==============================================================================

#include "ui_cellmlannotationviewcellmldetailswidget.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewCellmlDetailsWidget::CellmlAnnotationViewCellmlDetailsWidget(CellmlAnnotationViewDetailsWidget *pParent) :
    QSplitter(pParent),
    Core::CommonWidget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewCellmlDetailsWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create and add our CellML element details widget

    mCellmlElementDetails = new CellmlAnnotationViewCellmlElementDetailsWidget(this);

    addWidget(mCellmlElementDetails);
}

//==============================================================================

CellmlAnnotationViewCellmlDetailsWidget::~CellmlAnnotationViewCellmlDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    mCellmlElementDetails->retranslateUi();
}

//==============================================================================

QWidget * CellmlAnnotationViewCellmlDetailsWidget::focusProxyWidget() const
{
    // We want our cmeta:id widget to be a focus proxy widget

    return mCellmlElementDetails->focusProxyWidget();
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::updateGui(const CellmlAnnotationViewCellmlElementDetailsWidget::Items &pItems)
{
    // Update our CellML element details GUI

    mCellmlElementDetails->updateGui(pItems);
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::finalizeGui()
{
    // Finalise our CellML element details GUI

    mCellmlElementDetails->finalizeGui();
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
