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
#include "sedmlsupport.h"
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

static const auto SettingsCategory        = QStringLiteral("Category");
static const auto SettingsPropertiesWidth = QStringLiteral("PropertiesWidth");

//==============================================================================

static const auto SettingsCategoryDefault    = QStringLiteral("Graph Panel");
static const auto SettingsCategoryGraphPanel = SettingsCategoryDefault;
static const auto SettingsCategoryGraph      = QStringLiteral("Graph");

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

    QString category = mSettings->value(SettingsCategory, SettingsCategoryDefault).toString();

    mCategoryTabs->setCurrentIndex(!category.compare(SettingsCategoryGraphPanel)?
                                       0:
                                       1);

    mGui->layout->addWidget(mCategoryTabs);

    connect(mCategoryTabs, &Core::TabBarWidget::currentChanged,
            this, &SimulationExperimentViewPreferencesWidget::updateGui);

    // Retrieve our preferences

    mGraphPanelBackgroundColor = mSettings->value(SettingsPreferencesGraphPanelBackgroundColor, SettingsPreferencesGraphPanelBackgroundColorDefault).value<QColor>();
    mGraphPanelForegroundColor = mSettings->value(SettingsPreferencesGraphPanelForegroundColor, SettingsPreferencesGraphPanelForegroundColorDefault).value<QColor>();
    mGraphPanelTitle = mSettings->value(SettingsPreferencesGraphPanelTitle, SettingsPreferencesGraphPanelTitleDefault).toString();

    mGraphTitle = mSettings->value(SettingsPreferencesGraphTitle, SettingsPreferencesGraphTitleDefault).toString();
    mGraphLineStyle = SEDMLSupport::lineStyle(mSettings->value(SettingsPreferencesGraphLineStyle, SEDMLSupport::stringLineStyle(SettingsPreferencesGraphLineStyleDefault)).toString());
    mGraphLineWidth = mSettings->value(SettingsPreferencesGraphLineWidth, SettingsPreferencesGraphLineWidthDefault).toInt();
    mGraphSymbolStyle = SEDMLSupport::symbolStyle(mSettings->value(SettingsPreferencesGraphSymbolStyle, SEDMLSupport::stringSymbolStyle(SettingsPreferencesGraphSymbolStyleDefault)).toString());
    mGraphSymbolSize = mSettings->value(SettingsPreferencesGraphSymbolSize, SettingsPreferencesGraphSymbolSizeDefault).toInt();
    mGraphSymbolFilled = mSettings->value(SettingsPreferencesGraphSymbolFilled, SettingsPreferencesGraphSymbolFilledDefault).toBool();

    // Create and customise our graph panel property editor

    mGraphPanelProperties = new Core::PropertyEditorWidget(false, false, this);

    mGraphPanelProperties->addColorProperty(mGraphPanelBackgroundColor)->setName(tr("Background colour"));
    mGraphPanelProperties->addColorProperty(mGraphPanelForegroundColor)->setName(tr("Foreground colour"));
    mGraphPanelProperties->addStringProperty(mGraphPanelTitle)->setName(tr("Title"));

    int propertiesWidth = mSettings->value(SettingsPropertiesWidth, int(0.42*width())).toInt();

    mGraphPanelProperties->setColumnWidth(0, propertiesWidth);

    connect(mGraphPanelProperties->header(), &QHeaderView::sectionResized,
            this, &SimulationExperimentViewPreferencesWidget::headerSectionResized);

    // Create and customise our graph property editor

    mGraphProperties = new Core::PropertyEditorWidget(false, false, this);

    mGraphProperties->addStringProperty(mGraphTitle)->setName(tr("Title"));

    Core::Property *lineProperty = mGraphProperties->addSectionProperty(tr("Line"));

    mGraphProperties->addListProperty(SEDMLSupport::lineStyles(),
                                      SEDMLSupport::stringLineStyle(mGraphLineStyle),
                                      lineProperty)->setName(tr("Style"));
    mGraphProperties->addIntegerGt0Property(mGraphLineWidth, lineProperty)->setName(tr("Width"));

    Core::Property *symbolProperty = mGraphProperties->addSectionProperty(tr("Symbol"));

    mGraphProperties->addListProperty(SEDMLSupport::symbolStyles(),
                                      SEDMLSupport::stringSymbolStyle(mGraphSymbolStyle),
                                      symbolProperty)->setName(tr("Style"));
    mGraphProperties->addIntegerGt0Property(mGraphSymbolSize, symbolProperty)->setName(tr("Size"));
    mGraphProperties->addBooleanProperty(mGraphSymbolFilled, symbolProperty)->setName(tr("Filled"));

    mGraphProperties->setColumnWidth(0, propertiesWidth);
    mGraphProperties->expandAll();

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
    // Keep track of which category is selected

    mSettings->setValue(SettingsCategory, (mCategoryTabs->currentIndex() == 0)?
                                              SettingsCategoryGraphPanel:
                                              SettingsCategoryGraph);

    // Keep track of the width of our property editors (using that of our graph
    // panel property editor since they all have the same width)

    mSettings->setValue(SettingsPropertiesWidth, mGraphPanelProperties->columnWidth(0));

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
    Core::Properties graphLineProperties = graphProperties[1]->properties();
    Core::Properties graphSymbolProperties = graphProperties[2]->properties();

    return    // Graph panel preferences
              (graphPanelProperties[0]->colorValue() != mGraphPanelBackgroundColor)
           || (graphPanelProperties[1]->colorValue() != mGraphPanelForegroundColor)
           ||  graphPanelProperties[2]->stringValue().compare(mGraphPanelTitle)
              // Graph preferences
           ||  graphProperties[0]->stringValue().compare(mGraphTitle)
              // Graph line preferences
           ||  (graphLineProperties[0]->listValueIndex() != SEDMLSupport::indexLineStyle(mGraphLineStyle))
           ||  (graphLineProperties[1]->integerValue() != mGraphLineWidth)
              // Graph line preferences
           ||  (graphSymbolProperties[0]->listValueIndex() != SEDMLSupport::indexSymbolStyle(mGraphSymbolStyle))
           ||  (graphSymbolProperties[1]->integerValue() != mGraphSymbolSize)
           ||  (graphSymbolProperties[2]->booleanValue() != mGraphSymbolFilled);
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

    Core::Properties graphLineProperties = graphProperties[1]->properties();

    graphLineProperties[0]->setValue(SEDMLSupport::stringLineStyle(SettingsPreferencesGraphLineStyleDefault));
    graphLineProperties[1]->setIntegerValue(SettingsPreferencesGraphLineWidthDefault);

    Core::Properties graphSymbolProperties = graphProperties[2]->properties();

    graphSymbolProperties[0]->setValue(SEDMLSupport::stringSymbolStyle(SettingsPreferencesGraphSymbolStyleDefault));
    graphSymbolProperties[1]->setIntegerValue(SettingsPreferencesGraphSymbolSizeDefault);
    graphSymbolProperties[2]->setBooleanValue(SettingsPreferencesGraphSymbolFilledDefault);
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

    Core::Properties graphLineProperties = graphProperties[1]->properties();

    mSettings->setValue(SettingsPreferencesGraphLineStyle, graphLineProperties[0]->listValue());
    mSettings->setValue(SettingsPreferencesGraphLineWidth, graphLineProperties[1]->integerValue());

    Core::Properties graphSymbolProperties = graphProperties[2]->properties();

    mSettings->setValue(SettingsPreferencesGraphSymbolStyle, graphSymbolProperties[0]->listValue());
    mSettings->setValue(SettingsPreferencesGraphSymbolSize, graphSymbolProperties[1]->integerValue());
    mSettings->setValue(SettingsPreferencesGraphSymbolFilled, graphSymbolProperties[2]->booleanValue());
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
