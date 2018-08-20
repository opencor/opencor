/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Simulation Experiment view preferences widget
//==============================================================================

#include "simulationexperimentviewpreferenceswidget.h"
#include "tabbarwidget.h"

//==============================================================================

#include "ui_simulationexperimentviewpreferenceswidget.h"

//==============================================================================

#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace SimulationExperimentView {

//==============================================================================

static const auto TeachingInstance = QStringLiteral("https://teaching.physiomeproject.org");

//==============================================================================

SimulationExperimentViewPreferencesWidget::SimulationExperimentViewPreferencesWidget(QWidget *pParent) :
    Preferences::PreferencesWidget(PluginName, pParent),
    mGui(new Ui::SimulationExperimentViewPreferencesWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create, customise and add our category tab bar widget to our layout

    mCategoryTabs = new Core::TabBarWidget(this);

    mCategoryTabs->addTab(tr("Graph Panel"));
    mCategoryTabs->addTab(tr("Graph"));

    mGui->layout->addWidget(mCategoryTabs);

    connect(mCategoryTabs, &Core::TabBarWidget::currentChanged,
            this, &SimulationExperimentViewPreferencesWidget::updateGui);
}

//==============================================================================

SimulationExperimentViewPreferencesWidget::~SimulationExperimentViewPreferencesWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

bool SimulationExperimentViewPreferencesWidget::preferencesChanged() const
{
    // Return whether our preferences have changed

//---ISSUE1772--- TO BE DONE...
return false;
}

//==============================================================================

void SimulationExperimentViewPreferencesWidget::resetPreferences()
{
    // Reset our preferences

//---ISSUE1772--- TO BE DONE...
}

//==============================================================================

void SimulationExperimentViewPreferencesWidget::savePreferences()
{
    // Save our preferences

//---ISSUE1772--- TO BE DONE...
}

//==============================================================================

void SimulationExperimentViewPreferencesWidget::updateGui()
{
    // Update our GUI

//---ISSUE1772--- TO BE DONE...
}

//==============================================================================

}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
