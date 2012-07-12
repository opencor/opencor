//==============================================================================
// CellML annotation view CellML details widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmlannotationviewcellmldetailswidget.h"
#include "cellmlannotationviewcellmlmetadatadetailswidget.h"
#include "cellmlannotationviewwidget.h"
#include "cellmlannotationviewmetadataviewdetailswidget.h"

//==============================================================================

#include "ui_cellmlannotationviewcellmldetailswidget.h"

//==============================================================================

#include <QComboBox>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewCellmlDetailsWidget::CellmlAnnotationViewCellmlDetailsWidget(CellmlAnnotationViewWidget *pParent) :
    QSplitter(pParent),
    CommonWidget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewCellmlDetailsWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our details widgets

    mCellmlElementDetails  = new CellmlAnnotationViewCellmlElementDetailsWidget(pParent);
    mCellmlMetadataDetails = new CellmlAnnotationViewCellmlMetadataDetailsWidget(pParent);

    // Add our details widgets to our splitter

    addWidget(new Core::BorderedWidget(mCellmlElementDetails,
                                       false, true, true, false));
    addWidget(mCellmlMetadataDetails);

    // Keep track of our splitter being moved

    connect(this, SIGNAL(splitterMoved(int,int)),
            this, SLOT(emitSplitterMoved()));
}

//==============================================================================

CellmlAnnotationViewCellmlDetailsWidget::~CellmlAnnotationViewCellmlDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    mCellmlElementDetails->retranslateUi();
    mCellmlMetadataDetails->retranslateUi();
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::updateGui(const CellmlAnnotationViewCellmlElementDetailsWidget::Items &pItems)
{
    static CellmlAnnotationViewCellmlElementDetailsWidget::Items oldItems = CellmlAnnotationViewCellmlElementDetailsWidget::Items();

    if (pItems == oldItems)
        // We want to show the same items, so...

        return;

    // Keep track of the items

    oldItems = pItems;

    // Update our CellML element details GUI

    mCellmlElementDetails->updateGui(pItems);
}

//==============================================================================

CellmlAnnotationViewCellmlElementDetailsWidget * CellmlAnnotationViewCellmlDetailsWidget::cellmlElementDetails() const
{
    // Return our CellML element details widget

    return mCellmlElementDetails;
}

//==============================================================================

CellmlAnnotationViewCellmlMetadataDetailsWidget * CellmlAnnotationViewCellmlDetailsWidget::cellmlMetadataDetails() const
{
    // Return our CellML metadata details widget

    return mCellmlMetadataDetails;
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::updateSizes(const QList<int> &pSizes)
{
    // The splitter of another CellmlAnnotationViewCellmlDetailsWidget object
    // has been moved, so update our sizes

    setSizes(pSizes);
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::emitSplitterMoved()
{
    // Let people know that our splitter has been moved

    emit splitterMoved(sizes());
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::metadataUpdated()
{
    // Some metadata has been updated, so we need to update the metadata
    // information we show to the user

    mCellmlElementDetails->updateGui();
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
