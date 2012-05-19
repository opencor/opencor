//==============================================================================
// CellML annotation view metadata details widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmlannotationviewmetadatadetailswidget.h"
#include "cellmlannotationviewmetadataviewdetailswidget.h"
#include "cellmlannotationviewwidget.h"

//==============================================================================

#include "ui_cellmlannotationviewmetadatadetailswidget.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataDetailsWidget::CellmlAnnotationViewMetadataDetailsWidget(CellmlAnnotationViewWidget *pParent) :
    QSplitter(pParent),
    CommonWidget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewMetadataDetailsWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our details widget

    mMetadataViewDetails = new CellmlAnnotationViewMetadataViewDetailsWidget(pParent);

    // Add our details widget to our splitter

    addWidget(new Core::BorderedWidget(mMetadataViewDetails,
                                       false, true, false, false));
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
