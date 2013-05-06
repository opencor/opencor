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

CellMLAnnotationViewMetadataViewDetailsWidget::CellMLAnnotationViewMetadataViewDetailsWidget(CellMLAnnotationViewWidget *pParent) :
    QStackedWidget(pParent),
    CommonWidget(pParent),
    mCellMLFile(pParent->cellmlFile())
{
    // Create our different metadata views

    mRawView    = new CellMLAnnotationViewMetadataRawViewDetailsWidget(pParent);
    mNormalView = new CellMLAnnotationViewMetadataNormalViewDetailsWidget(pParent);

    // Make our raw view the default widget
    // Note: for the GUI to be properly initialised, we must add and immediately
    //       remove the views which we don't yet need. Not to do that may mess
    //       things up in our parent, so...

    addWidget(mRawView);

    addWidget(mNormalView);
    removeWidget(mNormalView);
}

//==============================================================================

void CellMLAnnotationViewMetadataViewDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mRawView->retranslateUi();
    mNormalView->retranslateUi();
}

//==============================================================================

void CellMLAnnotationViewMetadataViewDetailsWidget::updateGui(iface::cellml_api::CellMLElement *pElement)
{
    if (!pElement)
        return;

    // Decide on which view to use and update it, if needed

    switch (mCellMLFile->rdfTriples(pElement).type()) {
    case CellMLSupport::CellMLFileRdfTriple::BioModelsDotNetQualifier:
    case CellMLSupport::CellMLFileRdfTriple::Empty:
        removeWidget(mRawView);
        addWidget(mNormalView);

        mNormalView->updateGui(pElement);

        break;
    default:
        // Unknown type, so...

        removeWidget(mNormalView);
        addWidget(mRawView);

        mRawView->updateGui(pElement);
    }
}

//==============================================================================

void CellMLAnnotationViewMetadataViewDetailsWidget::addRdfTriple(CellMLSupport::CellMLFileRdfTriple *pRdfTriple)
{
    if (!pRdfTriple)
        return;

    // Add the given RDF triple to our normal view

    mNormalView->addRdfTriple(pRdfTriple);
}

//==============================================================================

CellMLAnnotationViewMetadataRawViewDetailsWidget * CellMLAnnotationViewMetadataViewDetailsWidget::rawView() const
{
    // Return our raw view

    return mRawView;
}

//==============================================================================

CellMLAnnotationViewMetadataNormalViewDetailsWidget * CellMLAnnotationViewMetadataViewDetailsWidget::normalView() const
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
