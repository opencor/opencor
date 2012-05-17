//==============================================================================
// CellML annotation view details widget
//==============================================================================

#include "cellmlannotationviewdetailswidget.h"
#include "cellmlannotationviewmetadatadetailswidget.h"
#include "cellmlannotationviewwidget.h"

//==============================================================================

#include "ui_cellmlannotationviewdetailswidget.h"

//==============================================================================

#include <QScrollBar>

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

    this->addWidget(mCellmlDetails);
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

void CellmlAnnotationViewDetailsWidget::updateGui(const CellmlAnnotationViewCellmlDetailsWidget::Items &pItems,
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

    QScrollArea *scrollArea;

    if (currentWidget() == mCellmlDetails) {
        scrollArea = mCellmlDetails;

        mCellmlDetails->updateGui(pItems);
    } else {
        scrollArea = mMetadataDetails;

        mMetadataDetails->updateGui(pRdfTriples);
    }

    // Re-show ourselves

    setVisible(true);

    // Scroll down to the bottom of ourselves, just in case things don't fit
    // within the viewport
    // Note: for this, we need to be up-to-date, hence we make a call to
    //       qApp->processEvents() and this can only be done when we are once
    //       again visible...

    qApp->processEvents();

    scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->maximum());
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::updateGui(const CellmlAnnotationViewCellmlDetailsWidget::Items &pItems)
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

    updateGui(CellmlAnnotationViewCellmlDetailsWidget::Items(), pRdfTriples);
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
