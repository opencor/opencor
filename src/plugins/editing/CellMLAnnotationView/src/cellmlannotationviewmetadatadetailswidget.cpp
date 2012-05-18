//==============================================================================
// CellML annotation view metadata details widget
//==============================================================================

#include "cellmlannotationviewdetailswidget.h"
#include "cellmlannotationviewmetadatadetailswidget.h"

//==============================================================================

#include "ui_cellmlannotationviewmetadatadetailswidget.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataDetailsWidget::CellmlAnnotationViewMetadataDetailsWidget(CellmlAnnotationViewDetailsWidget *pParent) :
    QSplitter(pParent),
    Core::CommonWidget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewMetadataDetailsWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create and add our Metadata view details widget

    mMetadataViewDetails = new CellmlAnnotationViewMetadataViewDetailsWidget(this);

    addWidget(mMetadataViewDetails);
}

//==============================================================================

CellmlAnnotationViewMetadataDetailsWidget::~CellmlAnnotationViewMetadataDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    mMetadataViewDetails->retranslateUi();
}

//==============================================================================

CellmlAnnotationViewDetailsWidget * CellmlAnnotationViewMetadataDetailsWidget::parent() const
{
    // Return our parent

    return mParent;
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples)
{
    // Update our Metadata view details GUI

    mMetadataViewDetails->updateGui(pRdfTriples);
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::finalizeGui()
{
    // Finalise our Metadata view details GUI

    mMetadataViewDetails->finalizeGui();
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
