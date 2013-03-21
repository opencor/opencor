//==============================================================================
// CellML annotation view metadata view details widget
//==============================================================================

#include "cellmlannotationviewmetadatanormalviewdetailswidget.h"
#include "cellmlannotationviewmetadatarawviewdetailswidget.h"
#include "cellmlannotationviewmetadataviewdetailswidget.h"
#include "cellmlannotationviewwidget.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataViewDetailsWidget::CellmlAnnotationViewMetadataViewDetailsWidget(CellmlAnnotationViewWidget *pParent) :
    QStackedWidget(pParent),
    CommonWidget(pParent)
{
    // Create our different metadata views

    mRawView    = new CellmlAnnotationViewMetadataRawViewDetailsWidget(pParent);
    mNormalView = new CellmlAnnotationViewMetadataNormalViewDetailsWidget(pParent);

    // Make our raw view the default widget
    // Note: for the GUI to be properly initialised, we must add and immediately
    //       remove the views which we don't yet need. Not to do that may mess
    //       things up in our parent, so...

    addWidget(mRawView);

    addWidget(mNormalView);
    removeWidget(mNormalView);
}

//==============================================================================

void CellmlAnnotationViewMetadataViewDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mRawView->retranslateUi();
    mNormalView->retranslateUi();
}

//==============================================================================

void CellmlAnnotationViewMetadataViewDetailsWidget::updateGui(iface::cellml_api::CellMLElement *pElement)
{
    if (!pElement)
        return;

    // Decide on which view to use and update it, if needed

//---GRY---
//    switch (pElement->rdfTriples().type()) {
//    case CellMLSupport::CellmlFileRdfTriple::BioModelsDotNetQualifier:
//    case CellMLSupport::CellmlFileRdfTriple::Empty:
//        removeWidget(mRawView);
//        addWidget(mNormalView);

//        mNormalView->updateGui(pElement);

//        break;
//    default:
//        // Unknown type, so...

//        removeWidget(mNormalView);
//        addWidget(mRawView);

//        mRawView->updateGui(pElement);
//    }
}

//==============================================================================

void CellmlAnnotationViewMetadataViewDetailsWidget::addRdfTriple(CellMLSupport::CellmlFileRdfTriple *pRdfTriple)
{
    if (!pRdfTriple)
        return;

    // Add the given RDF triple to our normal view

    mNormalView->addRdfTriple(pRdfTriple);
}

//==============================================================================

CellmlAnnotationViewMetadataRawViewDetailsWidget * CellmlAnnotationViewMetadataViewDetailsWidget::rawView() const
{
    // Return our raw view

    return mRawView;
}

//==============================================================================

CellmlAnnotationViewMetadataNormalViewDetailsWidget * CellmlAnnotationViewMetadataViewDetailsWidget::normalView() const
{
    // Return our normal view

    return mNormalView;
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
