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
    mMainWidget(0),
    mMainLayout(0),
    mFormWidget(0),
    mFormLayout(0),
    mGridWidget(0),
    mGridLayout(0)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create a stacked widget which will contain our GUI

    mWidget = new QStackedWidget(this);

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
    // Note: we are using certain layouts to dislay the contents of our view,
    //       but this unfortunately results in some very bad flickering on Mac
    //       OS X. This can, however, be addressed using a stacked widget, so...

    // Prevent ourselves from being updated (to avoid any flickering)

    setUpdatesEnabled(false);

    // Create a widget which will contain our GUI

    QWidget *newMainWidget = new QWidget(this);
    QVBoxLayout *newMainLayout = new QVBoxLayout(newMainWidget);

//    newMainLayout->setMargin(0);

    newMainWidget->setLayout(newMainLayout);

    // Deal with the form part of our GUI

    // Create a form widget which will contain the qualifier and term fields

    QWidget *newFormWidget = new QWidget(newMainWidget);
    QFormLayout *newFormLayout = new QFormLayout(newFormWidget);

    newFormWidget->setLayout(newFormLayout);

    // Add the qualifier and term fields

    QComboBox *qualifierValue = new QComboBox(newFormWidget);

    qualifierValue->addItems(CellMLSupport::CellmlFileRdfTriple::qualifiersAsStringList());

    newFormLayout->addRow(mParent->newLabel(newFormWidget, tr("Qualifier:"), true),
                          qualifierValue);

    QLineEdit *termValue = new QLineEdit(newFormWidget);

    newFormLayout->addRow(mParent->newLabel(newFormWidget, tr("Term:"), true),
                          termValue);

    // Deal with the grid part of our GUI

    // Create a new widget and layout

    QWidget *newGridWidget = new QWidget(newMainWidget);
    QGridLayout *newGridLayout = new QGridLayout(newGridWidget);

    newGridWidget->setLayout(newGridLayout);

    // Populate our new layout, but only if there is at least one RDF triple

    newGridLayout->addWidget(mParent->newLabel(newMainWidget,
                                               tr("No data available..."),
                                               false, 1.25, Qt::AlignCenter),
                             0, 0);

    // Add our 'internal' widgets to our new main widget

    newMainLayout->addWidget(newFormWidget);
    newMainLayout->addWidget(Core::newLineWidget(mWidget));
    newMainLayout->addWidget(newGridWidget);

    // Add our new widget to our stacked widget

    mWidget->addWidget(newMainWidget);

    // Remove the contents of our old form layout

    if (mFormWidget)
        for (int i = 0, iMax = mFormLayout->count(); i < iMax; ++i) {
            QLayoutItem *item = mFormLayout->takeAt(0);

            delete item->widget();
            delete item;
        }

    // Remove the contents of our old grid layout

    if (mGridWidget)
        for (int i = 0, iMax = mGridLayout->count(); i < iMax; ++i) {
            QLayoutItem *item = mGridLayout->takeAt(0);

            delete item->widget();
            delete item;
        }

    // Get rid of our old main widget and layout (and its contents)

    if (mMainWidget) {
        mWidget->removeWidget(mMainWidget);

        for (int i = 0, iMax = mMainLayout->count(); i < iMax; ++i) {
            QLayoutItem *item = mMainLayout->takeAt(0);

            delete item->widget();
            delete item;
        }

        delete mMainWidget;
    }

    // Keep track of our new main widgets and layouts

    mMainWidget = newMainWidget;
    mMainLayout = newMainLayout;

    mFormWidget = newFormWidget;
    mFormLayout = newFormLayout;

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
