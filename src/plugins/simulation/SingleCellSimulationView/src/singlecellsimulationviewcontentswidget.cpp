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

    mGraphPanelsWidget = new SingleCellSimulationViewGraphPanelsWidget(this);

    mGraphPanelsWidget->setObjectName("GraphPanels");

    connect(mGraphPanelsWidget, SIGNAL(removeGraphPanelsEnabled(const bool &)),
            this, SIGNAL(removeGraphPanelsEnabled(const bool &)));
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

    pSettings->beginGroup(mGraphPanelsWidget->objectName());
        mGraphPanelsWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SingleCellSimulationViewContentsWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the settings of our graph panels widget

    pSettings->beginGroup(mGraphPanelsWidget->objectName());
        mGraphPanelsWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SingleCellSimulationViewContentsWidget::addGraphPanel()
{
    // Ask our graph panels widget to add a new graph panel

    mGraphPanelsWidget->addGraphPanel();
}

//==============================================================================

void SingleCellSimulationViewContentsWidget::removeGraphPanel()
{
    // Ask our graph panels widget to remove the current graph panel

    mGraphPanelsWidget->removeGraphPanel();
}

//==============================================================================

SingleCellSimulationViewGraphPanelWidget * SingleCellSimulationViewContentsWidget::activeGraphPanel()
{
    // Ask our graph panels widget to retrieve the active graph panel

    return mGraphPanelsWidget->activeGraphPanel();
}

//==============================================================================

void SingleCellSimulationViewContentsWidget::clearGraphPanels()
{
    // Ask our graph panels widget to clear all the graph panels

    mGraphPanelsWidget->clearGraphPanels();
}

//==============================================================================

void SingleCellSimulationViewContentsWidget::clearActiveGraphPanel()
{
    // Ask our graph panels widget to clear the current graph panel

    mGraphPanelsWidget->clearActiveGraphPanel();
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
