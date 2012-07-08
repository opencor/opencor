//==============================================================================
// CellML annotation view metadata view details widget
//==============================================================================

#include "cellmlannotationviewmetadatabiomodelsdotnetviewdetailswidget.h"
#include "cellmlannotationviewmetadatarawviewdetailswidget.h"
#include "cellmlannotationviewmetadataviewdetailswidget.h"
#include "cellmlannotationviewwidget.h"

//==============================================================================

#include "ui_cellmlannotationviewmetadataviewdetailswidget.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataViewDetailsWidget::CellmlAnnotationViewMetadataViewDetailsWidget(CellmlAnnotationViewWidget *pParent,
                                                                                             const bool &pEditingMode) :
    QStackedWidget(pParent),
    CommonWidget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewMetadataViewDetailsWidget),
    mRdfTriples(CellMLSupport::CellmlFileRdfTriples(mParent->cellmlFile()))
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our different metadata views

    mRawView             = new CellmlAnnotationViewMetadataRawViewDetailsWidget(pParent);
    mBioModelsDotNetView = new CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget(pParent, pEditingMode);

    // Make our raw view the default widget
    // Note: for the GUI to be properly initialised, we must add and immediately
    //       remove the views which we don't yet need. Not to do that may mess
    //       things up in our parent, so...

    addWidget(mRawView);

    addWidget(mBioModelsDotNetView);
    removeWidget(mBioModelsDotNetView);
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
    mBioModelsDotNetView->retranslateUi();
}

//==============================================================================

void CellmlAnnotationViewMetadataViewDetailsWidget::updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples)
{
    // Keep track of the RDF triples

    mRdfTriples = pRdfTriples;

    // Decide on which view to use and update it, if needed

    switch (pRdfTriples.type()) {
    case CellMLSupport::CellmlFileRdfTriple::BioModelsDotNetQualifier:
    case CellMLSupport::CellmlFileRdfTriple::Empty:
        removeWidget(mRawView);
        addWidget(mBioModelsDotNetView);

        mBioModelsDotNetView->updateGui(pRdfTriples);

        break;
    default:
        // Unknown type, so...

        removeWidget(mBioModelsDotNetView);
        addWidget(mRawView);

        mRawView->updateGui(pRdfTriples);
    }
}

//==============================================================================

CellmlAnnotationViewMetadataRawViewDetailsWidget * CellmlAnnotationViewMetadataViewDetailsWidget::rawView() const
{
    // Return our raw view

    return mRawView;
}

//==============================================================================

CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget * CellmlAnnotationViewMetadataViewDetailsWidget::bioModelsDotNetView() const
{
    // Return our BioModels.Net view

    return mBioModelsDotNetView;
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
