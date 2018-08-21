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

    // Retrieve our preferences

    mGraphPanelBackgroundColor = mSettings->value(SettingsPreferencesGraphPanelBackgroundColor, SettingsPreferencesGraphPanelBackgroundColorDefault).value<QColor>();
    mGraphPanelForegroundColor = mSettings->value(SettingsPreferencesGraphPanelForegroundColor, SettingsPreferencesGraphPanelForegroundColorDefault).value<QColor>();
    mGraphPanelTitle = mSettings->value(SettingsPreferencesGraphPanelTitle, SettingsPreferencesGraphPanelTitleDefault).toString();

    mGraphTitle = mSettings->value(SettingsPreferencesGraphTitle, SettingsPreferencesGraphTitleDefault).toString();

    // Create and customise our property editors

    mGraphPanelProperties = new Core::PropertyEditorWidget(false, false, this);
    mGraphProperties = new Core::PropertyEditorWidget(false, false, this);

    mGraphPanelProperties->addColorProperty(mGraphPanelBackgroundColor)->setName(tr("Background colour"));
    mGraphPanelProperties->addColorProperty(mGraphPanelForegroundColor)->setName(tr("Foreground colour"));
    mGraphPanelProperties->addStringProperty(mGraphPanelTitle)->setName(tr("Title"));

    mGraphProperties->addStringProperty(mGraphTitle)->setName(tr("Title"));

    int defaultWidth = int(0.42*width());

    mGraphPanelProperties->setColumnWidth(0, mSettings->value(SettingsGraphPanelPropertiesWidth, defaultWidth).toInt());
    mGraphProperties->setColumnWidth(0, mSettings->value(SettingsGraphPropertiesWidth, defaultWidth).toInt());

    connect(mGraphPanelProperties->header(), &QHeaderView::sectionResized,
            this, &SimulationExperimentViewPreferencesWidget::headerSectionResized);
    connect(mGraphProperties->header(), &QHeaderView::sectionResized,
            this, &SimulationExperimentViewPreferencesWidget::headerSectionResized);

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
    // Finish any pending editing

    mGraphPanelProperties->finishEditing();
    mGraphProperties->finishEditing();

    // Return whether our preferences have changed

    Core::Properties graphPanelProperties = mGraphPanelProperties->properties();
    Core::Properties graphProperties = mGraphProperties->properties();

    return    // Graph panel preferences
              (graphPanelProperties[0]->colorValue() != mGraphPanelBackgroundColor)
           || (graphPanelProperties[1]->colorValue() != mGraphPanelForegroundColor)
           ||  graphPanelProperties[2]->stringValue().compare(mGraphPanelTitle)
              // Graph preferences
           ||  graphProperties[0]->stringValue().compare(mGraphTitle);
}

//==============================================================================

void SimulationExperimentViewPreferencesWidget::resetPreferences()
{
    // Reset our preferences

    Core::Properties graphPanelProperties = mGraphPanelProperties->properties();

    graphPanelProperties[0]->setColorValue(SettingsPreferencesGraphPanelBackgroundColorDefault);
    graphPanelProperties[1]->setColorValue(SettingsPreferencesGraphPanelForegroundColorDefault);
    graphPanelProperties[2]->setValue(SettingsPreferencesGraphPanelTitleDefault);

    Core::Properties graphProperties = mGraphProperties->properties();

    graphProperties[0]->setValue(SettingsPreferencesGraphTitleDefault);
}

//==============================================================================

void SimulationExperimentViewPreferencesWidget::savePreferences()
{
    // Save our preferences

    Core::Properties graphPanelProperties = mGraphPanelProperties->properties();

    mSettings->setValue(SettingsPreferencesGraphPanelBackgroundColor, graphPanelProperties[0]->colorValue());
    mSettings->setValue(SettingsPreferencesGraphPanelForegroundColor, graphPanelProperties[1]->colorValue());
    mSettings->setValue(SettingsPreferencesGraphPanelTitle, graphPanelProperties[2]->stringValue());

    Core::Properties graphProperties = mGraphProperties->properties();

    mSettings->setValue(SettingsPreferencesGraphTitle, graphProperties[0]->stringValue());
}

//==============================================================================

void SimulationExperimentViewPreferencesWidget::updateGui()
{
    // Update our GUI by showing the right property editor

    for (int i = 0, iMax = mCategoryTabs->count(); i < iMax; ++i)
        mPropertyEditors.value(i)->setVisible(i == mCategoryTabs->currentIndex());
}

//==============================================================================

void SimulationExperimentViewPreferencesWidget::headerSectionResized(int pIndex,
                                                                     int pOldSize,
                                                                     int pNewSize)
{
    Q_UNUSED(pOldSize);

    // Update the column width of our property editors

    disconnect(mGraphPanelProperties->header(), &QHeaderView::sectionResized,
               this, &SimulationExperimentViewPreferencesWidget::headerSectionResized);
    disconnect(mGraphProperties->header(), &QHeaderView::sectionResized,
               this, &SimulationExperimentViewPreferencesWidget::headerSectionResized);

    mGraphPanelProperties->setColumnWidth(pIndex, pNewSize);
    mGraphProperties->setColumnWidth(pIndex, pNewSize);

    connect(mGraphPanelProperties->header(), &QHeaderView::sectionResized,
            this, &SimulationExperimentViewPreferencesWidget::headerSectionResized);
    connect(mGraphProperties->header(), &QHeaderView::sectionResized,
            this, &SimulationExperimentViewPreferencesWidget::headerSectionResized);
}

//==============================================================================

}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
