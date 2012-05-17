//==============================================================================
// CellML annotation view widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmlannotationviewlistswidget.h"
#include "cellmlannotationviewdetailswidget.h"
#include "cellmlannotationviewwidget.h"
#include "cellmlfilemanager.h"

//==============================================================================

#include "ui_cellmlannotationviewwidget.h"

//==============================================================================

#include <QSplitter>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewWidget::CellmlAnnotationViewWidget(QWidget *pParent,
                                                       const QString &pFileName) :
    Widget(pParent),
    mGui(new Ui::CellmlAnnotationViewWidget),
    mListsWidget(0),
    mDetailsWidget(0)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Retrieve and load, in case it's necessary, the requested CellML file

    mCellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);

    mCellmlFile->load();

    // Customise our GUI which consists of two main parts:
    //
    //  1) A couple of lists (for CellML elements and metadata, resp.); and
    //  2) Some details (for a CellML element or metadata).
    //
    // These two main parts are widgets of an horizontal splitter and moving the
    // splitter will result in the horizontal splitter of this view for other
    // CellML files to be moved too

    // Create our horizontal splitter

    mHorizontalSplitter = new QSplitter(Qt::Horizontal, this);

    // Create our two main parts

    mListsWidget   = new CellmlAnnotationViewListsWidget(this);
    mDetailsWidget = new CellmlAnnotationViewDetailsWidget(this);

    // Populate our horizontal splitter

    mHorizontalSplitter->addWidget(mListsWidget);
    mHorizontalSplitter->addWidget(new Core::BorderedWidget(mDetailsWidget,
                                                            false, true, false, false));

    // Add our horizontal splitter to our horizontal layout

    mGui->horizontalLayout->addWidget(mHorizontalSplitter);

    // Keep track of our horizontal splitter being moved

    connect(mHorizontalSplitter, SIGNAL(splitterMoved(int,int)),
            this, SLOT(emitHorizontalSplitterMoved()));

    // Select the first CellML node from our CellML list

    mListsWidget->selectFirstCellmlNode();
}

//==============================================================================

CellmlAnnotationViewWidget::~CellmlAnnotationViewWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewWidget::retranslateUi()
{
    // Retranslate our lists and details widgets

    mListsWidget->retranslateUi();
    mDetailsWidget->retranslateUi();
}

//==============================================================================

CellMLSupport::CellmlFile * CellmlAnnotationViewWidget::cellmlFile() const
{
    // Return our pointer to the CellML file

    return mCellmlFile;
}

//==============================================================================

CellmlAnnotationViewListsWidget * CellmlAnnotationViewWidget::listsWidget() const
{
    // Return our lists widget

    return mListsWidget;
}

//==============================================================================

CellmlAnnotationViewDetailsWidget * CellmlAnnotationViewWidget::detailsWidget() const
{
    // Return our details widget

    return mDetailsWidget;
}

//==============================================================================

QList<int> CellmlAnnotationViewWidget::horizontalSplitterSizes() const
{
    // Return our horizontal splitter's sizes

    return mHorizontalSplitter->sizes();
}

//==============================================================================

void CellmlAnnotationViewWidget::updateHorizontalSplitter(const QList<int> &pHorizontalSizes)
{
    // Update our horizontal splitter after the horizontal splitter of another
    // CellmlAnnotationViewWidget object has been moved

    mHorizontalSplitter->setSizes(pHorizontalSizes);
}

//==============================================================================

void CellmlAnnotationViewWidget::emitHorizontalSplitterMoved()
{
    // Let whoever know that our horizontal splitter has been moved

    emit horizontalSplitterMoved(mHorizontalSplitter->sizes());
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
