#include "collapsablewidget.h"
#include "coreutils.h"

#include <QCalendarWidget>
//---GRY--- THE ABOVE IS TO BE REMOVED...

//==============================================================================
// Single cell simulation view information widget
//==============================================================================

#include "singlecellsimulationviewinformationwidget.h"

//==============================================================================

#include <QSettings>

//==============================================================================

#include "ui_singlecellsimulationviewinformationwidget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewInformationWidget::SingleCellSimulationViewInformationWidget(QWidget *pParent) :
    Widget(pParent),
    mGui(new Ui::SingleCellSimulationViewInformationWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

//---GRY--- THE BELOW IS JUST FOR TESTING PURPOSES...
Core::CollapsableWidget *collapsableWidget1 = new Core::CollapsableWidget("Collapsable widget #1", new QCalendarWidget(this), this);
Core::CollapsableWidget *collapsableWidget2 = new Core::CollapsableWidget("Collapsable widget #2", 0, this);
Core::CollapsableWidget *collapsableWidget3 = new Core::CollapsableWidget("Collapsable widget #3", new QCalendarWidget(this), this);

mGui->layout->addWidget(collapsableWidget1);
mGui->layout->addWidget(Core::newLineWidget(this));
mGui->layout->addWidget(collapsableWidget2);
mGui->layout->addWidget(Core::newLineWidget(this));
mGui->layout->addWidget(collapsableWidget3);
mGui->layout->addWidget(Core::newLineWidget(this));
mGui->layout->addStretch();
}

//==============================================================================

SingleCellSimulationViewInformationWidget::~SingleCellSimulationViewInformationWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void SingleCellSimulationViewInformationWidget::loadSettings(QSettings *pSettings)
{
//---GRY--- TO BE DONE...
}

//==============================================================================

void SingleCellSimulationViewInformationWidget::saveSettings(QSettings *pSettings) const
{
    //---GRY--- TO BE DONE...
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
