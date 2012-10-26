#include "collapsiblewidget.h"
#include "coreutils.h"

#include <QCalendarWidget>
#include <QDial>
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
Core::CollapsibleWidget *collapsibleWidget1 = new Core::CollapsibleWidget("Collapsible widget #1", new QCalendarWidget(this), this);
Core::CollapsibleWidget *collapsibleWidget2 = new Core::CollapsibleWidget(this);
Core::CollapsibleWidget *collapsibleWidget3 = new Core::CollapsibleWidget("Collapsible widget #3", 0, this);
Core::CollapsibleWidget *collapsibleWidget4 = new Core::CollapsibleWidget("Collapsible widget #4", new QCalendarWidget(this), this);

collapsibleWidget1->setCollapsed(true);
collapsibleWidget1->setBody(new QDial(this));
collapsibleWidget2->setTitle("Collapsible widget #2");
collapsibleWidget3->setBody(new QCalendarWidget(this));
collapsibleWidget4->body()->hide();
collapsibleWidget4->setBody(0);

mGui->layout->addWidget(collapsibleWidget1);
mGui->layout->addWidget(Core::newLineWidget(this));
mGui->layout->addWidget(collapsibleWidget2);
mGui->layout->addWidget(Core::newLineWidget(this));
mGui->layout->addWidget(collapsibleWidget3);
mGui->layout->addWidget(Core::newLineWidget(this));
mGui->layout->addWidget(collapsibleWidget4);
mGui->layout->addWidget(Core::newLineWidget(this));
mGui->layout->addStretch(1);
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
