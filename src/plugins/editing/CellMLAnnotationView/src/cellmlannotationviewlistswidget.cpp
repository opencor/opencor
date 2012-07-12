//==============================================================================
// CellML annotation view lists widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmlannotationviewcellmllistwidget.h"
#include "cellmlannotationviewlistswidget.h"
#include "cellmlannotationviewmetadatalistwidget.h"
#include "cellmlannotationviewwidget.h"

//==============================================================================

#include "ui_cellmlannotationviewlistswidget.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewListsWidget::CellmlAnnotationViewListsWidget(CellmlAnnotationViewWidget *pParent,
                                                                 CellMLSupport::CellmlFile *pCellmlFile) :
    QSplitter(pParent),
    CommonWidget(pParent),
    mGui(new Ui::CellmlAnnotationViewListsWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our CellML and metadata lists

    mCellmlList   = new CellmlAnnotationViewCellmlListWidget(pParent, pCellmlFile);
    mMetadataList = new CellmlAnnotationViewMetadataListWidget(pParent, pCellmlFile);

    // Populate ourselves

    addWidget(new Core::BorderedWidget(mCellmlList,
                                       false, false, true, true));
    addWidget(new Core::BorderedWidget(mMetadataList,
                                       true, false, false, true));

    // Keep track of our splitter being moved

    connect(this, SIGNAL(splitterMoved(int,int)),
            this, SLOT(emitSplitterMoved()));

    // Make our CellML list our focus proxy

    setFocusProxy(mCellmlList);
}

//==============================================================================

CellmlAnnotationViewListsWidget::~CellmlAnnotationViewListsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewListsWidget::retranslateUi()
{
    // Retranslate our CellML and metadata lists

    mCellmlList->retranslateUi();
    mMetadataList->retranslateUi();
}

//==============================================================================

CellmlAnnotationViewCellmlListWidget * CellmlAnnotationViewListsWidget::cellmlList() const
{
    // Return our CellML list

    return mCellmlList;
}

//==============================================================================

CellmlAnnotationViewMetadataListWidget * CellmlAnnotationViewListsWidget::metadataList() const
{
    // Return our metadata list

    return mMetadataList;
}

//==============================================================================

void CellmlAnnotationViewListsWidget::updateSizes(const QList<int> &pSizes)
{
    // The splitter of another CellmlAnnotationViewListsWidget object has been
    // moved, so update our sizes

    setSizes(pSizes);
}

//==============================================================================

void CellmlAnnotationViewListsWidget::emitSplitterMoved()
{
    // Let people know that our splitter has been moved

    emit splitterMoved(sizes());
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
