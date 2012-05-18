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
    Core::CommonWidget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewDetailsWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our CellML and metadata details GUIs

    mCellmlDetails   = new CellmlAnnotationViewCellmlDetailsWidget(this);
    mMetadataDetails = new CellmlAnnotationViewMetadataDetailsWidget(this);

    // Make our CellML details GUI the default widget

    addWidget(mCellmlDetails);
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

    if (currentWidget() == mCellmlDetails)
        return mCellmlDetails->retranslateUi();
    else
        return mMetadataDetails->retranslateUi();
}

//==============================================================================

QWidget * CellmlAnnotationViewDetailsWidget::focusProxyWidget() const
{
    // Ask either our CellML our metadata details GUI to tell which widget, if
    // any, should be the focus proxy widget

    if (currentWidget() == mCellmlDetails)
        return mCellmlDetails->focusProxyWidget();
    else
        return mMetadataDetails->focusProxyWidget();
}

//==============================================================================

CellmlAnnotationViewWidget * CellmlAnnotationViewDetailsWidget::parent() const
{
    // Return the details GUI's parent

    return mParent;
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::updateGui(const CellmlAnnotationViewCellmlElementDetailsWidget::Items &pItems,
                                                  const CellMLSupport::CellmlFileRdfTriples &pRdfTriples)
{
    // Hide ourselves (since we may potentially update ourselves quite a bit and
    // we want to avoid any flickering)
    // Note #1: one would normally use setUpdatesEnabled(), but it still results
    //          in bad flickering on Mac OS X, so...
    // Note #2: it's surprising that setVisible() doesn't cause any flickering
    //          on any of the platforms we are targetting, but let's not
    //          complain...

    setVisible(false);

    // Ask the CellML or metadata details GUI to update itself

    if (currentWidget() == mCellmlDetails)
        mCellmlDetails->updateGui(pItems);
    else
        mMetadataDetails->updateGui(pRdfTriples);

    // Re-show ourselves

    setVisible(true);

    // Finalise the update
    // Note: this is for things that can only be done when everything is visible
    //       again...

    if (currentWidget() == mCellmlDetails)
        mCellmlDetails->finalizeGui();
    else
        mMetadataDetails->finalizeGui();
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::updateGui(const CellmlAnnotationViewCellmlElementDetailsWidget::Items &pItems)
{
    // Make our CellML details GUI the default default widget

    removeWidget(mMetadataDetails);
    addWidget(mCellmlDetails);

    // Call our generic updateGui() method

    updateGui(pItems, CellMLSupport::CellmlFileRdfTriples());
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples)
{
    // Make our metadata details GUI the default default widget

    removeWidget(mCellmlDetails);
    addWidget(mMetadataDetails);

    // Call our generic updateGui() method

    updateGui(CellmlAnnotationViewCellmlElementDetailsWidget::Items(), pRdfTriples);
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
