//==============================================================================
// CellML annotation view details widget
//==============================================================================

#include "cellmlannotationviewcellmldetailswidget.h"
#include "cellmlannotationviewdetailswidget.h"
#include "cellmlannotationviewmetadatadetailswidget.h"
#include "cellmlannotationviewwidget.h"

//==============================================================================

#include "ui_cellmlannotationviewdetailswidget.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewDetailsWidget::CellmlAnnotationViewDetailsWidget(CellmlAnnotationViewWidget *pParent) :
    QStackedWidget(pParent),
    CommonWidget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewDetailsWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our CellML and metadata details GUIs

    mCellmlDetails   = new CellmlAnnotationViewCellmlDetailsWidget(pParent);
    mMetadataDetails = new CellmlAnnotationViewMetadataDetailsWidget(pParent);

    // Make our CellML details GUI the default widget
    // Note: for the GUI to be properly initialised, we must add and immediately
    //       remove mMetadataDetails. Not to do that may mess things up in our
    //       parent, so...

    addWidget(mCellmlDetails);

    addWidget(mMetadataDetails);
    removeWidget(mMetadataDetails);
}

//==============================================================================

CellmlAnnotationViewDetailsWidget::~CellmlAnnotationViewDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    mCellmlDetails->retranslateUi();
    mMetadataDetails->retranslateUi();
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::updateGui(const CellmlAnnotationViewCellmlElementDetailsWidget::Items &pItems,
                                                  const CellMLSupport::CellmlFileRdfTriples &pRdfTriples)
{
    // The widget which updateGui() method we call must first hide itself before
    // being updated (and then re-show itself once it has been updated), this to
    // avoid any flickering)
    // Note #1: one would normally use setUpdatesEnabled(), but it still results
    //          in bad flickering on Mac OS X, so...
    // Note #2: it's surprising that setVisible() doesn't cause any flickering
    //          on any of the platforms we are targetting, but let's not
    //          complain...

    // Ask the CellML or metadata details GUI to update itself, but only if it
    // is done using new data

    if (currentWidget() == mCellmlDetails)
        mCellmlDetails->updateGui(pItems);
    else
        mMetadataDetails->updateGui(pRdfTriples);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::updateGui(const CellmlAnnotationViewCellmlElementDetailsWidget::Items &pItems)
{
    // Make our CellML details GUI the default widget

    removeWidget(mMetadataDetails);
    addWidget(mCellmlDetails);

    // Call our generic updateGui() method

    updateGui(pItems, CellMLSupport::CellmlFileRdfTriples());
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples)
{
    // Make our metadata details GUI the default widget

    removeWidget(mCellmlDetails);
    addWidget(mMetadataDetails);

    // Call our generic updateGui() method

    updateGui(CellmlAnnotationViewCellmlElementDetailsWidget::Items(), pRdfTriples);
}

//==============================================================================

CellmlAnnotationViewCellmlDetailsWidget * CellmlAnnotationViewDetailsWidget::cellmlDetails() const
{
    // Return our CellML details widget

    return mCellmlDetails;
}

//==============================================================================

CellmlAnnotationViewMetadataDetailsWidget * CellmlAnnotationViewDetailsWidget::metadataDetails() const
{
    // Return our metadata details widget

    return mMetadataDetails;
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
