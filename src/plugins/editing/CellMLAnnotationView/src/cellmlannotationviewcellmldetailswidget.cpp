//==============================================================================
// CellML annotation view CellML details widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmlannotationviewcellmldetailswidget.h"
#include "cellmlannotationviewwidget.h"
#include "cellmlannotationviewmetadataviewdetailswidget.h"

//==============================================================================

#include "ui_cellmlannotationviewcellmldetailswidget.h"

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

    mCellmlElementDetails = new CellmlAnnotationViewCellmlElementDetailsWidget(pParent);
    mMetadataViewDetails  = new CellmlAnnotationViewMetadataViewDetailsWidget(pParent);

    // Add our details widgets to our splitter

    addWidget(new Core::BorderedWidget(mCellmlElementDetails,
                                       false, true, true, false));
    addWidget(new Core::BorderedWidget(mMetadataViewDetails,
                                       true, true, false, false));

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
    mMetadataViewDetails->retranslateUi();
}

//==============================================================================

QWidget * CellmlAnnotationViewCellmlDetailsWidget::focusProxyWidget() const
{
    // We want our cmeta:id widget to be a focus proxy widget

    return mCellmlElementDetails->focusProxyWidget();
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::updateGui(const CellmlAnnotationViewCellmlElementDetailsWidget::Items &pItems)
{
    // Update our CellML element details GUI

    mCellmlElementDetails->updateGui(pItems);
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::finalizeGui()
{
    // Finalise our CellML element and metadata view details GUI

    mCellmlElementDetails->finalizeGui();
    mMetadataViewDetails->finalizeGui();
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::updateSplitter(const QList<int> &pSizes)
{
    // The splitter of another CellmlAnnotationViewCellmlDetailsWidget object
    // has been moved, so update ours

    setSizes(pSizes);
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::emitSplitterMoved()
{
    // Let whoever know that our splitter has been moved

    emit splitterMoved(sizes());
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
