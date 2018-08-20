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

#include "borderedwidget.h"
#include "propertyeditorwidget.h"
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

static const auto SettingsGraphPanelPropertiesWidth = QStringLiteral("GraphPanelPropertiesWidth");
static const auto SettingsGraphPropertiesWidth      = QStringLiteral("GraphPropertiesWidth");

//==============================================================================

SimulationExperimentViewPreferencesWidget::SimulationExperimentViewPreferencesWidget(QWidget *pParent) :
    Preferences::PreferencesWidget(PluginName, pParent),
    mGui(new Ui::SimulationExperimentViewPreferencesWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    mGui->layout->setSpacing(0);

    // Create, customise and add our category tab bar widget to our layout

    mCategoryTabs = new Core::TabBarWidget(this);

    int graphPanelIndex = mCategoryTabs->addTab(tr("Graph Panel"));
    int graphIndex = mCategoryTabs->addTab(tr("Graph"));

    mGui->layout->addWidget(mCategoryTabs);

    connect(mCategoryTabs, &Core::TabBarWidget::currentChanged,
            this, &SimulationExperimentViewPreferencesWidget::updateGui);

    // Create and customise our property editors

    mGraphPanelProperties = new Core::PropertyEditorWidget(false, false, this);
    mGraphProperties = new Core::PropertyEditorWidget(false, false, this);

    mGraphPanelProperties->addColorProperty()->setName(tr("Background colour"));
    mGraphPanelProperties->addColorProperty()->setName(tr("Foreground colour"));
    mGraphPanelProperties->addStringProperty()->setName(tr("Title"));

    mGraphProperties->addStringProperty()->setName(tr("Title"));

    mGraphPanelProperties->setColumnWidth(0, mSettings->value(SettingsGraphPanelPropertiesWidth, defaultWidth()).toInt());
    mGraphProperties->setColumnWidth(0, mSettings->value(SettingsGraphPropertiesWidth, defaultWidth()).toInt());

    // Add our property editors (with a border) to our layout

    Core::BorderedWidget *graphPanelBorderedWidget = new Core::BorderedWidget(mGraphPanelProperties, true, true, true, true);
    Core::BorderedWidget *graphBorderedWidget = new Core::BorderedWidget(mGraphProperties, true, true, true, true);

    mPropertyEditors.insert(graphPanelIndex, graphPanelBorderedWidget);
    mPropertyEditors.insert(graphIndex, graphBorderedWidget);

    mGui->layout->addWidget(graphPanelBorderedWidget);
    mGui->layout->addWidget(graphBorderedWidget);

    // Show the right property editor by "updating" our GUI

    updateGui();

    // Give focus to our graph panel property editor

    setFocusProxy(mGraphPanelProperties);
}

//==============================================================================

SimulationExperimentViewPreferencesWidget::~SimulationExperimentViewPreferencesWidget()
{
    // Keep track of the sizes of our property editors

    mSettings->setValue(SettingsGraphPanelPropertiesWidth, mGraphPanelProperties->columnWidth(0));
    mSettings->setValue(SettingsGraphPropertiesWidth, mGraphProperties->columnWidth(0));

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
    // Reset our preferences and GUI settings

//---ISSUE1772--- TO BE DONE...

    mGraphPanelProperties->setColumnWidth(0, defaultWidth());
    mGraphProperties->setColumnWidth(0, defaultWidth());
}

//==============================================================================

void SimulationExperimentViewPreferencesWidget::savePreferences()
{
    // Save our preferences

//---ISSUE1772--- TO BE DONE...
}

//==============================================================================

int SimulationExperimentViewPreferencesWidget::defaultWidth()
{
    // Return the default width we want to use for the first column of a
    // property editor

    return int(0.35*width());
}

//==============================================================================

void SimulationExperimentViewPreferencesWidget::updateGui()
{
    // Update our GUI by showing the right property editor

    for (int i = 0, iMax = mCategoryTabs->count(); i < iMax; ++i)
        mPropertyEditors.value(i)->setVisible(i == mCategoryTabs->currentIndex());
}

//==============================================================================

}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
