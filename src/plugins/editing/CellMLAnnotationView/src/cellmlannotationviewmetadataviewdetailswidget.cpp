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

    mEmptyView           = new QWidget(pParent);
    mRawView             = new CellmlAnnotationViewMetadataRawViewDetailsWidget(pParent);
    mBioModelsDotNetView = new CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget(pParent);

    // Make our empty view the default widget
    // Note: for the GUI to be properly initialised, we must add and immediately
    //       remove the views which we don't yet need. Not to do that may mess
    //       things up in our parent, so...

    addWidget(mEmptyView);

    addWidget(mRawView);
    removeWidget(mRawView);

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

    updateGui(mRdfTriples);
}

//==============================================================================

void CellmlAnnotationViewMetadataViewDetailsWidget::updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples)
{
    // Keep track of the RDF triples

    mRdfTriples = pRdfTriples;

    // Decide on which view to use

    removeWidget(mEmptyView);
    removeWidget(mRawView);
    removeWidget(mBioModelsDotNetView);

    if (pRdfTriples.isEmpty())
        addWidget(mEmptyView);
    else
        switch (pRdfTriples.type()) {
        case CellMLSupport::CellmlFileRdfTriple::BioModelNetQualifier:
            addWidget(mBioModelsDotNetView);

            break;
        default:
            // Unknown type, so...

            addWidget(mRawView);
        }

    // Update our non-empty view, if needed

    if (currentWidget() == mRawView)
        mRawView->updateGui(pRdfTriples);
    else if (currentWidget() == mBioModelsDotNetView)
        mBioModelsDotNetView->updateGui(pRdfTriples);
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
