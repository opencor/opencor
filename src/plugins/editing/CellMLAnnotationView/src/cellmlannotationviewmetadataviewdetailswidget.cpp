//==============================================================================
// CellML annotation view metadata view details widget
//==============================================================================

#include "cellmlannotationviewmetadatanormalviewdetailswidget.h"
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
    mGui(new Ui::CellmlAnnotationViewMetadataViewDetailsWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our different metadata views

    mRawView    = new CellmlAnnotationViewMetadataRawViewDetailsWidget(pParent);
    mNormalView = new CellmlAnnotationViewMetadataNormalViewDetailsWidget(pParent, pEditingMode);

    // Make our raw view the default widget
    // Note: for the GUI to be properly initialised, we must add and immediately
    //       remove the views which we don't yet need. Not to do that may mess
    //       things up in our parent, so...

    addWidget(mRawView);

    addWidget(mNormalView);
    removeWidget(mNormalView);
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
    mNormalView->retranslateUi();
}

//==============================================================================

void CellmlAnnotationViewMetadataViewDetailsWidget::updateGui(CellMLSupport::CellmlFileElement *pCellmlElement)
{
    if (!pCellmlElement)
        return;

    // Decide on which view to use and update it, if needed

    switch (pCellmlElement->rdfTriples().type()) {
    case CellMLSupport::CellmlFileRdfTriple::BioModelsDotNetQualifier:
    case CellMLSupport::CellmlFileRdfTriple::Empty:
        removeWidget(mRawView);
        addWidget(mNormalView);

        mNormalView->updateGui(pCellmlElement);

        break;
    default:
        // Unknown type, so...

        removeWidget(mNormalView);
        addWidget(mRawView);

        mRawView->updateGui(pCellmlElement);
    }
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
