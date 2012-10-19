//==============================================================================
// Single cell simulation view contents widget
//==============================================================================

#include "singlecellsimulationviewcontentswidget.h"
#include "singlecellsimulationviewgraphpanelswidget.h"

//==============================================================================

#include <QSettings>
#include <QWheelEvent>

//==============================================================================

#include "ui_singlecellsimulationviewcontentswidget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewContentsWidget::SingleCellSimulationViewContentsWidget(QWidget *pParent) :
    QSplitter(pParent),
    CommonWidget(pParent),
    mGui(new Ui::SingleCellSimulationViewContentsWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create a splitter for our graph panels and create a connection to keep
    // track of whether we can remove graph panels

    mGraphPanels = new SingleCellSimulationViewGraphPanelsWidget(this);

    mGraphPanels->setObjectName("GraphPanels");

//---GRY---
//    connect(mGraphPanels, SIGNAL(removeGraphPanelsEnabled(const bool &)),
//            mGui->actionRemove, SLOT(setEnabled(bool)));
}

//==============================================================================

SingleCellSimulationViewContentsWidget::~SingleCellSimulationViewContentsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void SingleCellSimulationViewContentsWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the settings of our graph panels widget

    pSettings->beginGroup(mGraphPanels->objectName());
        mGraphPanels->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SingleCellSimulationViewContentsWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the settings of our graph panels widget

    pSettings->beginGroup(mGraphPanels->objectName());
        mGraphPanels->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
