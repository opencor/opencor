//==============================================================================
// CellML annotation view metadata edit details widget
//==============================================================================

#include "cellmlannotationviewmetadataeditdetailswidget.h"
#include "cellmlannotationviewwidget.h"

//==============================================================================

#include "ui_cellmlannotationviewmetadataeditdetailswidget.h"

//==============================================================================

#include <QGridLayout>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataEditDetailsWidget::CellmlAnnotationViewMetadataEditDetailsWidget(CellmlAnnotationViewWidget *pParent) :
    QScrollArea(pParent),
    CommonWidget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewMetadataEditDetailsWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create the widget (and its layout) which will contain our GUI

    mWidget = new QWidget(this);

    QVBoxLayout *widgetLayout = new QVBoxLayout(mWidget);

    widgetLayout->setMargin(0);

    mWidget->setLayout(widgetLayout);

    // Create another (grid) widget (and its layout) which will contain the
    // BioModels.Net information and add it to our main widget together with
    // some stretch
    // Note: the stretch is to ensure that the contents of our grid widget
    //       doesn't take all the vertical space, but only the vertical space
    //       which is actually needed to display its contents...

    QWidget *gridWidget = new QWidget(mWidget);

    mLayout = new QGridLayout(gridWidget);

    gridWidget->setLayout(mLayout);

    widgetLayout->addWidget(gridWidget);
    widgetLayout->addStretch();

    // Add our widget to our scroll area

    setWidget(mWidget);
}

//==============================================================================

CellmlAnnotationViewMetadataEditDetailsWidget::~CellmlAnnotationViewMetadataEditDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
