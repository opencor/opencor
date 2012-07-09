//==============================================================================
// CellML annotation view metadata edit details widget
//==============================================================================

#include "cellmlannotationviewmetadataeditdetailswidget.h"
#include "cellmlannotationviewwidget.h"
#include "coreutils.h"

//==============================================================================

#include "ui_cellmlannotationviewmetadataeditdetailswidget.h"

//==============================================================================

#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QStackedWidget>
#include <QVBoxLayout>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataEditDetailsWidget::CellmlAnnotationViewMetadataEditDetailsWidget(CellmlAnnotationViewWidget *pParent) :
    QScrollArea(pParent),
    CommonWidget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewMetadataEditDetailsWidget),
    mGridWidget(0),
    mGridLayout(0)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create a widget which will contain our GUI

    mWidget = new QWidget(this);

    QVBoxLayout *layout = new QVBoxLayout(mWidget);

    layout->setMargin(0);

    mWidget->setLayout(layout);

    // Create a form widget which will contain the qualifier and term fields

    mFormWidget = new QWidget(mWidget);
    mFormLayout = new QFormLayout(mFormWidget);

    mFormWidget->setLayout(mFormLayout);

    // Create a stacked widget which will contain the suggested ontological
    // terms

    mStackedWidget = new QStackedWidget(mWidget);

    updateGui();

    // Add our 'internal' widgets to our main widget

    layout->addWidget(mFormWidget);
    layout->addWidget(Core::newLineWidget(mWidget));
    layout->addWidget(mStackedWidget);

    // Add our stacked widget to our scroll area

    setWidget(mWidget);
}

//==============================================================================

CellmlAnnotationViewMetadataEditDetailsWidget::~CellmlAnnotationViewMetadataEditDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    // For the rest of our GUI, it's easier to just update it, so...

    updateGui();
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::updateGui()
{
    // Note: we are using a grid layout to dislay the contents of our view, but
    //       to update the contents unfortunately results in some very bad
    //       flickering on Mac OS X. This can, however, be addressed using a
    //       stacked widget with a grid-based widget...

    // Prevent ourselves from being updated (to avoid any flickering)

    setUpdatesEnabled(false);

    // Deal with the form part of our GUI

    // Remove everything from our form layout

    for (int i = 0, iMax = mFormLayout->count(); i < iMax; ++i) {
        QLayoutItem *item = mFormLayout->takeAt(0);

        delete item->widget();
        delete item;
    }

    // Add the qualifier and term fields

    QComboBox *qualifierValue = new QComboBox(mFormWidget);

    qualifierValue->addItems(CellMLSupport::CellmlFileRdfTriple::qualifiersAsStringList());

    mFormLayout->addRow(mParent->newLabel(mFormWidget, tr("Qualifier:"), true),
                        qualifierValue);

    QLineEdit *termValue = new QLineEdit(mFormWidget);

    mFormLayout->addRow(mParent->newLabel(mFormWidget, tr("Term:"), true),
                        termValue);

    // Deal with the grid part of our GUI

    // Create a new widget and layout

    QWidget *newGridWidget = new QWidget(mStackedWidget);
    QGridLayout *newGridLayout = new QGridLayout(newGridWidget);

    newGridWidget->setLayout(newGridLayout);

    // Populate our new layout, but only if there is at least one RDF triple

    newGridLayout->addWidget(mParent->newLabel(mStackedWidget,
                                               tr("No data available..."),
                                               false, 1.25, Qt::AlignCenter),
                             0, 0);

    // Add our new widget to our stacked widget

    mStackedWidget->addWidget(newGridWidget);

    // Get rid of our old widget and layout (and of its contents)

    if (mGridWidget) {
        mStackedWidget->removeWidget(mGridWidget);

        for (int i = 0, iMax = mGridLayout->count(); i < iMax; ++i) {
            QLayoutItem *item = mGridLayout->takeAt(0);

            delete item->widget();
            delete item;
        }

        delete mGridLayout;
        delete mGridWidget;
    }

    // Keep track of our new widget and layout

    mGridWidget = newGridWidget;
    mGridLayout = newGridLayout;

    // Allow ourselves to be updated again

    setUpdatesEnabled(true);
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
