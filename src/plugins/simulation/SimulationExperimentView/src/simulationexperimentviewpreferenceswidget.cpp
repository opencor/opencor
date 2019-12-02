/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Simulation Experiment view preferences widget
//==============================================================================

#include "borderedwidget.h"
#include "propertyeditorwidget.h"
#include "sedmlsupport.h"
#include "simulationexperimentviewpreferenceswidget.h"
#include "tabbarwidget.h"

//==============================================================================

#include "ui_simulationexperimentviewpreferenceswidget.h"

//==============================================================================

#include <QHeaderView>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace SimulationExperimentView {

//==============================================================================

static const char *SettingsCategory        = "Category";
static const char *SettingsPropertiesWidth = "PropertiesWidth";

//==============================================================================

static const char *SettingsCategoryDefault    = "Graph Panel";
static const char *SettingsCategoryGraphPanel = SettingsCategoryDefault;
static const char *SettingsCategoryGraph      = "Graph";

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

    QString category = mSettings.value(SettingsCategory, SettingsCategoryDefault).toString();

    mCategoryTabs->setCurrentIndex(int(category != SettingsCategoryGraphPanel));

    mGui->layout->addWidget(mCategoryTabs);

    connect(mCategoryTabs, &Core::TabBarWidget::currentChanged,
            this, &SimulationExperimentViewPreferencesWidget::updateGui);

    // Retrieve our preferences

    mGraphPanelBackgroundColor = mSettings.value(SettingsPreferencesGraphPanelBackgroundColor, SettingsPreferencesGraphPanelBackgroundColorDefault).value<QColor>();
    mGraphPanelForegroundColor = mSettings.value(SettingsPreferencesGraphPanelForegroundColor, SettingsPreferencesGraphPanelForegroundColorDefault).value<QColor>();
    mGraphPanelLegend = mSettings.value(SettingsPreferencesGraphPanelLegend, SettingsPreferencesGraphPanelLegendDefault).toBool();

    mGraphLineStyle = SEDMLSupport::lineStyle(mSettings.value(SettingsPreferencesGraphLineStyle, SEDMLSupport::stringLineStyle(SettingsPreferencesGraphLineStyleDefault)).toString());
    mGraphLineWidth = mSettings.value(SettingsPreferencesGraphLineWidth, SettingsPreferencesGraphLineWidthDefault).toInt();
    mGraphSymbolStyle = SEDMLSupport::symbolStyle(mSettings.value(SettingsPreferencesGraphSymbolStyle, SEDMLSupport::stringSymbolStyle(SettingsPreferencesGraphSymbolStyleDefault)).toString());
    mGraphSymbolSize = mSettings.value(SettingsPreferencesGraphSymbolSize, SettingsPreferencesGraphSymbolSizeDefault).toInt();
    mGraphSymbolFilled = mSettings.value(SettingsPreferencesGraphSymbolFilled, SettingsPreferencesGraphSymbolFilledDefault).toBool();
    mGraphSymbolFillColor = mSettings.value(SettingsPreferencesGraphSymbolFillColor, SettingsPreferencesGraphSymbolFillColorDefault).value<QColor>();

    // Create and customise our graph panel property editor

    mGraphPanelProperties = new Core::PropertyEditorWidget(false, false, this);

    mGraphPanelProperties->addColorProperty(mGraphPanelBackgroundColor)->setName(tr("Background colour"));
    mGraphPanelProperties->addColorProperty(mGraphPanelForegroundColor)->setName(tr("Foreground colour"));
    mGraphPanelProperties->addBooleanProperty(mGraphPanelLegend)->setName(tr("Legend"));

    int propertiesWidth = mSettings.value(SettingsPropertiesWidth, int(0.42*width())).toInt();

    mGraphPanelProperties->setColumnWidth(0, propertiesWidth);

    connect(mGraphPanelProperties->header(), &QHeaderView::sectionResized,
            this, &SimulationExperimentViewPreferencesWidget::headerSectionResized);

    // Create and customise our graph property editor

    mGraphProperties = new Core::PropertyEditorWidget(false, false, this);

    Core::Property *lineProperty = mGraphProperties->addSectionProperty(tr("Line"));

    mGraphProperties->addListProperty(SEDMLSupport::formattedLineStyles(),
                                      SEDMLSupport::stringLineStyle(mGraphLineStyle, true),
                                      lineProperty)->setName(tr("Style"));
    mGraphProperties->addIntegerGt0Property(mGraphLineWidth, lineProperty)->setName(tr("Width"));

    Core::Property *symbolProperty = mGraphProperties->addSectionProperty(tr("Symbol"));

    mGraphProperties->addListProperty(SEDMLSupport::formattedSymbolStyles(),
                                      SEDMLSupport::stringSymbolStyle(mGraphSymbolStyle, true),
                                      symbolProperty)->setName(tr("Style"));
    mGraphProperties->addIntegerGt0Property(mGraphSymbolSize, symbolProperty)->setName(tr("Size"));
    mGraphProperties->addBooleanProperty(mGraphSymbolFilled, symbolProperty)->setName(tr("Filled"));
    mGraphProperties->addColorProperty(mGraphSymbolFillColor, symbolProperty)->setName(tr("Fill colour"));

    mGraphProperties->setColumnWidth(0, propertiesWidth);
    mGraphProperties->expandAll();

    connect(mGraphProperties->header(), &QHeaderView::sectionResized,
            this, &SimulationExperimentViewPreferencesWidget::headerSectionResized);

    // Add our property editors (with a border) to our layout

    auto graphPanelBorderedWidget = new Core::BorderedWidget(mGraphPanelProperties, true, true, true, true);
    auto graphBorderedWidget = new Core::BorderedWidget(mGraphProperties, true, true, true, true);

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
    // Keep track of which category is selected

    mSettings.setValue(SettingsCategory, (mCategoryTabs->currentIndex() == 0)?
                                             SettingsCategoryGraphPanel:
                                             SettingsCategoryGraph);

    // Keep track of the width of our property editors (using that of our graph
    // panel property editor since they all have the same width)

    mSettings.setValue(SettingsPropertiesWidth, mGraphPanelProperties->columnWidth(0));

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
    Core::Properties graphLineProperties = graphProperties[0]->properties();
    Core::Properties graphSymbolProperties = graphProperties[1]->properties();

    return    // Graph panel preferences
              (graphPanelProperties[0]->colorValue() != mGraphPanelBackgroundColor)
           || (graphPanelProperties[1]->colorValue() != mGraphPanelForegroundColor)
           || (graphPanelProperties[2]->booleanValue() != mGraphPanelLegend)
              // Graph line preferences
           ||  (graphLineProperties[0]->listValueIndex() != SEDMLSupport::indexLineStyle(mGraphLineStyle))
           ||  (graphLineProperties[1]->integerValue() != mGraphLineWidth)
              // Graph line preferences
           ||  (graphSymbolProperties[0]->listValueIndex() != SEDMLSupport::indexSymbolStyle(mGraphSymbolStyle))
           ||  (graphSymbolProperties[1]->integerValue() != mGraphSymbolSize)
           ||  (graphSymbolProperties[2]->booleanValue() != mGraphSymbolFilled)
           ||  (graphSymbolProperties[3]->colorValue() != mGraphSymbolFillColor);
}

//==============================================================================

void SimulationExperimentViewPreferencesWidget::resetPreferences()
{
    // Reset our preferences

    Core::Properties graphPanelProperties = mGraphPanelProperties->properties();

    graphPanelProperties[0]->setColorValue(SettingsPreferencesGraphPanelBackgroundColorDefault);
    graphPanelProperties[1]->setColorValue(SettingsPreferencesGraphPanelForegroundColorDefault);
    graphPanelProperties[2]->setBooleanValue(SettingsPreferencesGraphPanelLegendDefault);

    Core::Properties graphProperties = mGraphProperties->properties();
    Core::Properties graphLineProperties = graphProperties[0]->properties();

    graphLineProperties[0]->setValue(SEDMLSupport::stringLineStyle(SettingsPreferencesGraphLineStyleDefault, true));
    graphLineProperties[1]->setIntegerValue(SettingsPreferencesGraphLineWidthDefault);

    Core::Properties graphSymbolProperties = graphProperties[1]->properties();

    graphSymbolProperties[0]->setValue(SEDMLSupport::stringSymbolStyle(SettingsPreferencesGraphSymbolStyleDefault, true));
    graphSymbolProperties[1]->setIntegerValue(SettingsPreferencesGraphSymbolSizeDefault);
    graphSymbolProperties[2]->setBooleanValue(SettingsPreferencesGraphSymbolFilledDefault);
    graphSymbolProperties[3]->setColorValue(SettingsPreferencesGraphSymbolFillColorDefault);
}

//==============================================================================

void SimulationExperimentViewPreferencesWidget::savePreferences()
{
    // Save our preferences

    Core::Properties graphPanelProperties = mGraphPanelProperties->properties();

    mSettings.setValue(SettingsPreferencesGraphPanelBackgroundColor, graphPanelProperties[0]->colorValue());
    mSettings.setValue(SettingsPreferencesGraphPanelForegroundColor, graphPanelProperties[1]->colorValue());
    mSettings.setValue(SettingsPreferencesGraphPanelLegend, graphPanelProperties[2]->booleanValue());

    Core::Properties graphProperties = mGraphProperties->properties();
    Core::Properties graphLineProperties = graphProperties[0]->properties();

    mSettings.setValue(SettingsPreferencesGraphLineStyle, SEDMLSupport::stringLineStyle(graphLineProperties[0]->listValueIndex()));
    mSettings.setValue(SettingsPreferencesGraphLineWidth, graphLineProperties[1]->integerValue());

    Core::Properties graphSymbolProperties = graphProperties[1]->properties();

    mSettings.setValue(SettingsPreferencesGraphSymbolStyle, SEDMLSupport::stringSymbolStyle(graphSymbolProperties[0]->listValueIndex()));
    mSettings.setValue(SettingsPreferencesGraphSymbolSize, graphSymbolProperties[1]->integerValue());
    mSettings.setValue(SettingsPreferencesGraphSymbolFilled, graphSymbolProperties[2]->booleanValue());
    mSettings.setValue(SettingsPreferencesGraphSymbolFillColor, graphSymbolProperties[3]->colorValue());
}

//==============================================================================

void SimulationExperimentViewPreferencesWidget::updateGui()
{
    // Update our GUI by showing the right property editor

    for (int i = 0, iMax = mCategoryTabs->count(); i < iMax; ++i) {
        mPropertyEditors.value(i)->setVisible(i == mCategoryTabs->currentIndex());
    }
}

//==============================================================================

void SimulationExperimentViewPreferencesWidget::headerSectionResized(int pIndex,
                                                                     int pOldSize,
                                                                     int pNewSize)
{
    Q_UNUSED(pOldSize)

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

} // namespace SimulationExperimentView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
