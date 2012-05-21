//==============================================================================
// CellML annotation view metadata view details widget
//==============================================================================

#include "cellmlannotationviewmetadatarawviewdetailswidget.h"
#include "cellmlannotationviewmetadataviewdetailswidget.h"
#include "cellmlannotationviewwidget.h"

//==============================================================================

#include "ui_cellmlannotationviewmetadataviewdetailswidget.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataViewDetailsWidget::CellmlAnnotationViewMetadataViewDetailsWidget(CellmlAnnotationViewWidget *pParent) :
    QStackedWidget(pParent),
    CommonWidget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewMetadataViewDetailsWidget),
    mRdfTriples(CellMLSupport::CellmlFileRdfTriples())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our different metadata views

    mRawView = new CellmlAnnotationViewMetadataRawViewDetailsWidget(pParent);

    // Make our raw the default widget

    addWidget(mRawView);
}

//==============================================================================

CellmlAnnotationViewMetadataViewDetailsWidget::~CellmlAnnotationViewMetadataViewDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewMetadataViewDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    mRawView->retranslateUi();

    updateGui(mRdfTriples);
}

//==============================================================================

void CellmlAnnotationViewMetadataViewDetailsWidget::updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples)
{
    // Keep track of the RDF triples

    mRdfTriples = pRdfTriples;

    // Update our raw view

    mRawView->updateGui(pRdfTriples);
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
