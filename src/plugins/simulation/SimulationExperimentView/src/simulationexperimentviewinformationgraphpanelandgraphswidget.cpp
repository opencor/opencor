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
// Simulation Experiment view information graph panel and graphs widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "coreguiutils.h"
#include "filemanager.h"
#include "i18ninterface.h"
#include "preferencesinterface.h"
#include "sedmlsupport.h"
#include "simulation.h"
#include "simulationexperimentviewinformationgraphpanelandgraphswidget.h"
#include "simulationexperimentviewsimulationwidget.h"
#include "simulationexperimentviewwidget.h"

//==============================================================================

#include <QFileInfo>
#include <QHeaderView>
#include <QMenu>
#include <QScrollBar>
#include <QSettings>

//==============================================================================

#include "qwtbegin.h"
    #include "qwt_symbol.h"
#include "qwtend.h"

//==============================================================================

namespace OpenCOR {
namespace SimulationExperimentView {

//==============================================================================

SimulationExperimentViewInformationGraphPanelAndGraphsWidget::SimulationExperimentViewInformationGraphPanelAndGraphsWidget(SimulationExperimentViewWidget *pViewWidget,
                                                                                                                           SimulationExperimentViewSimulationWidget *pSimulationWidget,
                                                                                                                           QWidget *pParent) :
    QStackedWidget(pParent),
    Core::CommonWidget(this),
    mViewWidget(pViewWidget),
    mSimulationWidget(pSimulationWidget)
{
    // Create our graph panel context menu and populate it

    mGraphPanelContextMenu = new QMenu(this);

    mSelectGraphPanelColorAction = Core::newAction(this);

    connect(mSelectGraphPanelColorAction, &QAction::triggered,
            this, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::selectGraphPanelColor);

    mGraphPanelContextMenu->addAction(mSelectGraphPanelColorAction);

    // Create our graph context menus and populate our main graph context menu

    mGraphContextMenu = new QMenu(this);
    mGraphParametersContextMenu = new QMenu(this);

    mAddGraphAction = Core::newAction(this);
    mRemoveCurrentGraphAction = Core::newAction(this);
    mRemoveAllGraphsAction = Core::newAction(this);
    mSelectAllGraphsAction = Core::newAction(this);
    mUnselectAllGraphsAction = Core::newAction(this);
    mSelectGraphColorAction = Core::newAction(this);

    connect(mAddGraphAction, &QAction::triggered,
            this, QOverload<>::of(&SimulationExperimentViewInformationGraphPanelAndGraphsWidget::addGraph));
    connect(mRemoveCurrentGraphAction, &QAction::triggered,
            this, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::removeCurrentGraph);
    connect(mRemoveAllGraphsAction, &QAction::triggered,
            this, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::removeAllGraphs);
    connect(mSelectAllGraphsAction, &QAction::triggered,
            this, QOverload<>::of(&SimulationExperimentViewInformationGraphPanelAndGraphsWidget::selectAllGraphs));
    connect(mUnselectAllGraphsAction, &QAction::triggered,
            this, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::unselectAllGraphs);
    connect(mSelectGraphColorAction, &QAction::triggered,
            this, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::selectGraphColor);

    mGraphContextMenu->addAction(mAddGraphAction);
    mGraphContextMenu->addSeparator();
    mGraphContextMenu->addAction(mRemoveCurrentGraphAction);
    mGraphContextMenu->addAction(mRemoveAllGraphsAction);
    mGraphContextMenu->addSeparator();
    mGraphContextMenu->addAction(mSelectAllGraphsAction);
    mGraphContextMenu->addAction(mUnselectAllGraphsAction);
    mGraphContextMenu->addSeparator();
    mGraphContextMenu->addAction(mSelectGraphColorAction);

    // Some further initialisations that are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::retranslateGraphPanelPropertyEditor(Core::PropertyEditorWidget *pGraphPanelPropertyEditor)
{
    // Retranslate the contents of the given graph panel property editor

    Core::Properties graphPanelProperties = pGraphPanelPropertyEditor->properties();

    graphPanelProperties[0]->setName(tr("Background colour"));
    graphPanelProperties[1]->setName(tr("Font size"));
    graphPanelProperties[2]->setName(tr("Foreground colour"));

    // Grid lines

    Core::Properties gridLinesProperties = graphPanelProperties[3]->properties();

    graphPanelProperties[3]->setName(tr("Grid lines"));

    gridLinesProperties[0]->setName(tr("Style"));
    gridLinesProperties[1]->setName(tr("Width"));
    gridLinesProperties[2]->setName(tr("Colour"));

    // Legend

    Core::Properties legendProperties = graphPanelProperties[4]->properties();

    graphPanelProperties[4]->setName(tr("Legend"));

    legendProperties[0]->setName(tr("Font size"));
    legendProperties[1]->setName(tr("Visible"));

    // Point coordinates

    Core::Properties pointCoordinatesProperties = graphPanelProperties[5]->properties();

    graphPanelProperties[5]->setName(tr("Point coordinates"));

    pointCoordinatesProperties[0]->setName(tr("Style"));
    pointCoordinatesProperties[1]->setName(tr("Width"));
    pointCoordinatesProperties[2]->setName(tr("Colour"));
    pointCoordinatesProperties[3]->setName(tr("Font colour"));
    pointCoordinatesProperties[4]->setName(tr("Font size"));

    // Surrounding area

    Core::Properties surroundingAreaProperties = graphPanelProperties[6]->properties();

    graphPanelProperties[6]->setName(tr("Surrounding area"));

    surroundingAreaProperties[0]->setName(tr("Background colour"));
    surroundingAreaProperties[1]->setName(tr("Foreground colour"));

    // Title

    graphPanelProperties[7]->setName(tr("Title"));

    // X axis

    Core::Properties xAxisProperties = graphPanelProperties[8]->properties();

    graphPanelProperties[8]->setName(tr("X axis"));

    xAxisProperties[0]->setName(tr("Font size"));
    xAxisProperties[1]->setName(tr("Logarithmic scale"));
    xAxisProperties[2]->setName(tr("Title"));

    // Y axis

    Core::Properties yAxisProperties = graphPanelProperties[9]->properties();

    graphPanelProperties[9]->setName(tr("Y axis"));

    yAxisProperties[0]->setName(tr("Font size"));
    yAxisProperties[1]->setName(tr("Logarithmic scale"));
    yAxisProperties[2]->setName(tr("Title"));

    // Zoom region

    Core::Properties zoomRegionProperties = graphPanelProperties[10]->properties();

    graphPanelProperties[10]->setName(tr("Zoom region"));

    zoomRegionProperties[0]->setName(tr("Style"));
    zoomRegionProperties[1]->setName(tr("Width"));
    zoomRegionProperties[2]->setName(tr("Colour"));
    zoomRegionProperties[3]->setName(tr("Font colour"));
    zoomRegionProperties[4]->setName(tr("Font size"));
    zoomRegionProperties[5]->setName(tr("Filled"));
    zoomRegionProperties[6]->setName(tr("Fill colour"));
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::retranslateUi()
{
    // Retranslate our actions

    I18nInterface::retranslateAction(mSelectGraphPanelColorAction, tr("Select Colour..."),
                                     tr("Select a colour"));

    I18nInterface::retranslateAction(mAddGraphAction, tr("Add Graph"),
                                     tr("Add a graph"));
    I18nInterface::retranslateAction(mRemoveCurrentGraphAction, tr("Remove Current Graph"),
                                     tr("Remove the current graph"));
    I18nInterface::retranslateAction(mRemoveAllGraphsAction, tr("Remove All Graphs"),
                                     tr("Remove all the graphs"));
    I18nInterface::retranslateAction(mSelectAllGraphsAction, tr("Select All Graphs"),
                                     tr("Select all the graphs"));
    I18nInterface::retranslateAction(mUnselectAllGraphsAction, tr("Unselect All Graphs"),
                                     tr("Unselect all the graphs"));
    I18nInterface::retranslateAction(mSelectGraphColorAction, tr("Select Colour..."),
                                     tr("Select a colour"));

    // Retranslate all our property editors

    for (auto graphPanelPropertyEditor : mGraphPanelPropertyEditors) {
        graphPanelPropertyEditor->retranslateUi();
    }

    for (auto graphsPropertyEditor : mGraphsPropertyEditors) {
        graphsPropertyEditor->retranslateUi();
    }

    // Retranslate the contents of our graph panel property editors

    for (auto graphPanelPropertyEditor : mGraphPanelPropertyEditors) {
        retranslateGraphPanelPropertyEditor(graphPanelPropertyEditor);
    }

    // Retranslate the information about our graphs properties
    // Note: no need to do this for all our property editors (i.e. call
    //       updateAllGraphsInfo()) since this will automatically be done when
    //       selecting another graph panel. So, instead, we just do this for our
    //       current graph panel...

    updateGraphsInfo();
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::initialize(SimulationSupport::Simulation *pSimulation)
{
    // Keep track of the simulation

    mSimulation = pSimulation;

    // Populate our graph parameters context menu

    populateGraphParametersContextMenu(pSimulation->runtime());

    // Update our graphs information

    updateAllGraphsInfo();
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::finalize()
{
    // Clear our graph parameters context menu and the the parameters associated
    // with our model parameter actions

    mGraphParametersContextMenu->clear();

    mParameterActions.clear();

    // Remove all our graphs' runs

    for (auto graph : mGraphs) {
        graph->removeRuns();
    }
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::importData(DataStore::DataStoreImportData *pImportData)
{
    // Create our general import menu, if needed

    if (mImportMenu == nullptr) {
        mImportMenu = new QMenu("imports", mGraphParametersContextMenu);

        mGraphParametersContextMenu->addMenu(mImportMenu);
    }

    // Create our import sub-menu

    auto importSubMenu = new QMenu(pImportData->hierarchy().last(), mImportMenu);

    mImportMenu->addMenu(importSubMenu);

    // Populate our import sub-menu with the given data

    for (auto parameter : mSimulation->runtime()->dataParameters(mSimulation->data()->data(pImportData->importDataStore()))) {
        QAction *parameterAction = importSubMenu->addAction(CellMLSupport::CellmlFileRuntimeParameter::icon(parameter->type()),
                                                            parameter->formattedName());

        // Create a connection to handle the parameter value update

        connect(parameterAction, &QAction::triggered,
                this, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::updateParameterValue);

        // Keep track of the parameter associated with our parameter action

        mParameterActions.insert(parameterAction, parameter);
    }
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::initialize(GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                                                                              const GraphPanelWidget::GraphPanelWidgetProperties &pGraphPanelWidgetProperties,
                                                                              bool pActive,
                                                                              bool pCustomize)
{
    // Retrieve the graph panel and graphs property editors for the given file
    // name or create some, if none exists

    Core::PropertyEditorWidget *oldGraphPanelPropertyEditor = mGraphPanelPropertyEditor;
    Core::PropertyEditorWidget *oldGraphsPropertyEditor = mGraphsPropertyEditor;

    mGraphPanelPropertyEditor = mGraphPanelPropertyEditors.value(pGraphPanel);
    mGraphsPropertyEditor = mGraphsPropertyEditors.value(pGraphPanel);

    if ((mGraphPanelPropertyEditor == nullptr) && (mGraphsPropertyEditor == nullptr)) {
        // No graph panel and graphs property editors exist for the given graph
        // panel, so create some

        mGraphPanelPropertyEditor = new Core::PropertyEditorWidget(false, false, this);
        mGraphsPropertyEditor = new Core::PropertyEditorWidget(false, false, this);

        // Keep track of the link between our existing graph panel and our new
        // graph panel and graphs property editors

        mGraphPanels.insert(mGraphPanelPropertyEditor, pGraphPanel);
        mGraphPanels.insert(mGraphsPropertyEditor, pGraphPanel);

        mGraphPanelPropertyEditors.insert(pGraphPanel, mGraphPanelPropertyEditor);
        mGraphsPropertyEditors.insert(pGraphPanel, mGraphsPropertyEditor);

        // Customise our corresponding graph panel with our preferences, if
        // required

        if (pCustomize) {
            GraphPanelWidget::GraphPanelPlotWidget *graphPanelPlot = pGraphPanel->plot();

            graphPanelPlot->setBackgroundColor(pGraphPanelWidgetProperties.backgroundColor());
            graphPanelPlot->setForegroundColor(pGraphPanelWidgetProperties.foregroundColor());
            graphPanelPlot->setLegendVisible(pGraphPanelWidgetProperties.legend());
        }

        // Populate our graph panel property editor

        populateGraphPanelPropertyEditor();

        // We want our own context menu for our graph panel and graphs property
        // editors

        mGraphPanelPropertyEditor->setContextMenuPolicy(Qt::CustomContextMenu);
        mGraphsPropertyEditor->setContextMenuPolicy(Qt::CustomContextMenu);

        connect(mGraphPanelPropertyEditor, &Core::PropertyEditorWidget::customContextMenuRequested,
                this, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::showGraphPanelContextMenu);
        connect(mGraphsPropertyEditor, &Core::PropertyEditorWidget::customContextMenuRequested,
                this, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::showGraphsContextMenu);

        // Keep track of changes to columns' width

        connect(mGraphPanelPropertyEditor->header(), &QHeaderView::sectionResized,
                this, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphPanelHeaderSectionResized);
        connect(mGraphsPropertyEditor->header(), &QHeaderView::sectionResized,
                this, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphsHeaderSectionResized);

        // Keep track of changes to the expanded/collapsed state of a property
        // Note: we only need to do this for our graph panel property editor
        //       since graphs are really unique to a graph panel...

        connect(mGraphPanelPropertyEditor, &Core::PropertyEditorWidget::expanded,
                this, QOverload<const QModelIndex &>::of(&SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphPanelSectionExpanded));
        connect(mGraphPanelPropertyEditor, &Core::PropertyEditorWidget::collapsed,
                this, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphPanelSectionCollapsed);

        // Keep track of when the user changes a property value

        connect(mGraphPanelPropertyEditor, &Core::PropertyEditorWidget::propertyChanged,
                this, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphPanelPropertyChanged);
        connect(mGraphsPropertyEditor, &Core::PropertyEditorWidget::propertyChanged,
                this, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphsPropertyChanged);

        // Add our new graphs property editor to ourselves

        addWidget(mGraphPanelPropertyEditor);
        addWidget(mGraphsPropertyEditor);
    }

    // Make sure that our 'new' property editors' columns' width is the same as
    // that of our 'old' property editors, and that the expanded state of our
    // graph panel editor's sections is properly set
    // Note: the only reason we check mGraphPanelPropertyEditor and
    //       mGraphsPropertyEditor is to keep Xcode happy...

    if (   (oldGraphPanelPropertyEditor != nullptr) && (oldGraphsPropertyEditor != nullptr)
        && (mGraphPanelPropertyEditor != nullptr) && (mGraphsPropertyEditor != nullptr)) {
        for (int i = 0, iMax = oldGraphPanelPropertyEditor->header()->count(); i < iMax; ++i) {
            mGraphPanelPropertyEditor->setColumnWidth(i, oldGraphPanelPropertyEditor->columnWidth(i));
        }

        for (int i = 0, iMax = oldGraphsPropertyEditor->header()->count(); i < iMax; ++i) {
            mGraphsPropertyEditor->setColumnWidth(i, oldGraphsPropertyEditor->columnWidth(i));
        }

        for (int i = 0, iMax = oldGraphPanelPropertyEditor->model()->rowCount(); i < iMax; ++i) {
            mGraphPanelPropertyEditor->setExpanded(mGraphPanelPropertyEditor->model()->index(i, 0),
                                                   oldGraphPanelPropertyEditor->isExpanded(oldGraphPanelPropertyEditor->model()->index(i, 0)));
        }
    }

    // Set our retrieved graph panel or graphs property editor as our current
    // widget, but only if the corresponding graph panel has been been made
    // active

    if (pActive) {
        setCurrentWidget((mMode == Mode::GraphPanel)?mGraphPanelPropertyEditor:mGraphsPropertyEditor);
    }

    // Update our graphs information
    // Note: this is in case the user changed the locale and then switched to a
    //       different graph panel...

    updateAllGraphsInfo();
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::initialize(GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                                                                              const GraphPanelWidget::GraphPanelWidgetProperties &pGraphPanelWidgetProperties,
                                                                              bool pActive)
{
    // Initialise the given graph panel

    initialize(pGraphPanel, pGraphPanelWidgetProperties, pActive, true);
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::initialize(GraphPanelWidget::GraphPanelWidget *pGraphPanel)
{
    // Initialise the given graph panel

    initialize(pGraphPanel, GraphPanelWidget::GraphPanelWidgetProperties(), true, false);
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::finalize(GraphPanelWidget::GraphPanelWidget *pGraphPanel)
{
    // Remove all the graphs associated with the given graph panel

    pGraphPanel->removeAllGraphs();

    // Remove track of the link betwen our graph panel and our graph panel and
    // graphs property editors

    Core::PropertyEditorWidget *graphPanelPropertyEditor = mGraphPanelPropertyEditors.value(pGraphPanel);
    Core::PropertyEditorWidget *graphsPropertyEditor = mGraphsPropertyEditors.value(pGraphPanel);

    if (   (graphPanelPropertyEditor == mGraphPanelPropertyEditor)
        && (graphsPropertyEditor == mGraphsPropertyEditor)) {
        mGraphPanelPropertyEditor = nullptr;
        mGraphsPropertyEditor = nullptr;
    }

    mGraphPanels.remove(graphPanelPropertyEditor);
    mGraphPanels.remove(graphsPropertyEditor);

    mGraphPanelPropertyEditors.remove(pGraphPanel);
    mGraphsPropertyEditors.remove(pGraphPanel);

    removeWidget(graphPanelPropertyEditor);
    removeWidget(graphsPropertyEditor);

    delete graphPanelPropertyEditor;
    delete graphsPropertyEditor;
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::addGraph(GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                                                                            GraphPanelWidget::GraphPanelPlotGraph *pGraph,
                                                                            const GraphPanelWidget::GraphPanelPlotGraphProperties &pGraphProperties)
{
    // Make sure that we have a graphs property editor

    Core::PropertyEditorWidget *graphsPropertyEditor = mGraphsPropertyEditors.value(pGraphPanel);

    if (graphsPropertyEditor == nullptr) {
        return;
    }

    // Create a section for our newly added graph

    Core::Property *graphProperty = graphsPropertyEditor->addSectionProperty();

    graphProperty->setCheckable(true);
    graphProperty->setChecked(pGraphProperties.isSelected());

    // Keep track of the link between our given graph and our graph property

    mGraphs.insert(graphProperty, pGraph);
    mGraphProperties.insert(pGraph, graphProperty);

    // Create some graph properties
    // Note #1: to add properties will result in the propertyChanged() signal
    //          being emitted, but we don't want to handle that signal (at
    //          least, not when creating a graph since not everyting may be set
    //          yet, so this might cause more problems than anything), so we
    //          must disconnect ourselves from it before adding the properties
    //          (and then reconnect ourselves to it once we are done)...
    // Note #2: our graph properties have or can have an icon, but not our title
    //          property, so give it a blank icon so that it is properly aligned
    //          with our other properties...

    disconnect(graphsPropertyEditor, &Core::PropertyEditorWidget::propertyChanged,
               this, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphsPropertyChanged);

    graphsPropertyEditor->addListProperty(graphProperty);
    graphsPropertyEditor->addStringProperty(pGraphProperties.title(), graphProperty);
    graphsPropertyEditor->addStringProperty((pGraph->parameterX() != nullptr)?
                                                static_cast<CellMLSupport::CellmlFileRuntimeParameter *>(pGraph->parameterX())->fullyFormattedName():
                                                Core::UnknownValue,
                                            graphProperty);
    graphsPropertyEditor->addStringProperty((pGraph->parameterY() != nullptr)?
                                                static_cast<CellMLSupport::CellmlFileRuntimeParameter *>(pGraph->parameterY())->fullyFormattedName():
                                                Core::UnknownValue,
                                            graphProperty);

    static const QIcon BlankIcon = QIcon(":/SimulationExperimentView/blank.png");

    graphProperty->properties()[1]->setIcon(BlankIcon);

    // Create some line properties for our graph

    Core::Property *lineProperty = graphsPropertyEditor->addSectionProperty(graphProperty);

    graphsPropertyEditor->addListProperty(SEDMLSupport::lineStyles(),
                                          SEDMLSupport::stringLineStyle(pGraphProperties.lineStyle()),
                                          lineProperty);
    graphsPropertyEditor->addIntegerGt0Property(pGraphProperties.lineWidth(), lineProperty);
    graphsPropertyEditor->addColorProperty(pGraphProperties.lineColor(), lineProperty);

    // Create some symbol properties for our graph

    Core::Property *symbolProperty = graphsPropertyEditor->addSectionProperty(graphProperty);

    graphsPropertyEditor->addListProperty(SEDMLSupport::symbolStyles(),
                                          SEDMLSupport::stringSymbolStyle(pGraphProperties.symbolStyle()),
                                          symbolProperty);
    graphsPropertyEditor->addIntegerGt0Property(pGraphProperties.symbolSize(), symbolProperty);
    graphsPropertyEditor->addColorProperty(pGraphProperties.symbolColor(), symbolProperty);
    graphsPropertyEditor->addBooleanProperty(pGraphProperties.symbolFilled(), symbolProperty);
    graphsPropertyEditor->addColorProperty(pGraphProperties.symbolFillColor(), symbolProperty);

    // Keep track of changes to our new graph

    connect(graphsPropertyEditor, &Core::PropertyEditorWidget::propertyChanged,
            this, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphsPropertyChanged);

    // Update the information about our new graph

    updateGraphsInfo(graphProperty);

    // Make sure that the selected state of our new graph is taken into account
    // Note: this must be done after the call to updateGraphsInfo() above
    //       otherwise our new graph's visible state won't be properly set...

    graphsPropertyChanged(graphProperty);
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::removeGraphs(GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                                                                                const GraphPanelWidget::GraphPanelPlotGraphs &pGraphs)
{
    // Make sure that we have a graphs property editor

    Core::PropertyEditorWidget *graphsPropertyEditor = mGraphsPropertyEditors.value(pGraphPanel);

    if (graphsPropertyEditor == nullptr) {
        return;
    }

    // Remove the graph properties associated with the given graphs, as well as
    // their trace

    for (auto graph : pGraphs) {
        Core::Property *property = mGraphProperties.value(graph);

        graphsPropertyEditor->removeProperty(property);

        mGraphs.remove(property);
        mGraphProperties.remove(graph);
    }
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::toggleGraph(GraphPanelWidget::GraphPanelPlotGraph *pGraph)
{
    // Toggle our graph property

    Core::Property *graphProperty = mGraphProperties.value(pGraph);

    graphProperty->setChecked(!graphProperty->isChecked());
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::toggleLegend()
{
    // Toggle our legend property

    Core::Property *legendProperty = mGraphPanelPropertyEditor->properties()[4];

    legendProperty->setBooleanValue(!legendProperty->booleanValue());
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::toggleLogarithmicXAxis()
{
    // Toggle our logarithmic X axis property

    Core::Property *logarithmicXAxisProperty = mGraphPanelPropertyEditor->properties()[8]->properties()[0];

    logarithmicXAxisProperty->setBooleanValue(!logarithmicXAxisProperty->booleanValue());
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::toggleLogarithmicYAxis()
{
    // Toggle our logarithmic Y axis property

    Core::Property *logarithmicYAxisProperty = mGraphPanelPropertyEditor->properties()[9]->properties()[0];

    logarithmicYAxisProperty->setBooleanValue(!logarithmicYAxisProperty->booleanValue());
}

//==============================================================================

bool SimulationExperimentViewInformationGraphPanelAndGraphsWidget::rootProperty(Core::Property *pProperty) const
{
    // Return whether the given property is a root property

    return    (pProperty->type() == Core::Property::Type::Section)
           && (pProperty->parentProperty() == nullptr) && pProperty->isCheckable();
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::selectGraphPanelColor()
{
    // Select a colour for the current property

    mGraphPanelPropertyEditor->currentProperty()->setColorValue();
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::addGraph()
{
    // Ask the graph panel associated with our current graphs property editor to
    // add an 'empty' graph

    GraphPanelWidget::GraphPanelWidget * graphPanel = mGraphPanels.value(mGraphsPropertyEditor);
    auto graph = new GraphPanelWidget::GraphPanelPlotGraph(graphPanel);

    graphPanel->addGraph(graph, mSimulationWidget->defaultGraphProperties(graph->color()));
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::removeCurrentGraph()
{
    // Ask the graph panel associated with our current graphs property editor to
    // remove the current graph

    mGraphPanels.value(mGraphsPropertyEditor)->removeGraphs(GraphPanelWidget::GraphPanelPlotGraphs() << mGraphs.value(mGraphsPropertyEditor->currentProperty()));
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::removeAllGraphs()
{
    // Ask the graph panel associated with our current graphs property editor to
    // remove all its graphs

    mGraphPanels.value(mGraphsPropertyEditor)->removeAllGraphs();
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::selectAllGraphs(bool pSelect)
{
    // (Un)select all the graphs
    // Note: normally, we would only update the checked state of our graph
    //       properties, which would in turn update the selected state of our
    //       graphs and let people know that they have been updated. Now, the
    //       problem with this is that every single graph is going to be shown/
    //       hidden indivdually. So, if we have loads of graphs with many data
    //       points, then it's going to be slow. So, instead, we disconnect the
    //       propertyChanged() signal, update the checked state of our graph
    //       properties as well as the selected state of our graphs, and then
    //       let people know that all the graphs have been updated...

    disconnect(mGraphsPropertyEditor, &Core::PropertyEditorWidget::propertyChanged,
               this, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphsPropertyChanged);

    GraphPanelWidget::GraphPanelPlotGraphs graphs = GraphPanelWidget::GraphPanelPlotGraphs();

    for (auto property : mGraphsPropertyEditor->properties()) {
        property->setChecked(pSelect);

        GraphPanelWidget::GraphPanelPlotGraph *graph = mGraphs.value(property);

        graphs << graph;

        graph->setSelected(pSelect);
    }

    if (!graphs.isEmpty()) {
        emit graphsUpdated(graphs);
    }

    connect(mGraphsPropertyEditor, &Core::PropertyEditorWidget::propertyChanged,
            this, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphsPropertyChanged);
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::selectAllGraphs()
{
    // Select all the graphs

    selectAllGraphs(true);
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::unselectAllGraphs()
{
    // Unselect all the graphs

    selectAllGraphs(false);
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::selectGraphColor()
{
    // Select a colour for the current property

    mGraphsPropertyEditor->currentProperty()->setColorValue();
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::fileRenamed(const QString &pOldFileName,
                                                                               const QString &pNewFileName)
{
    // Keep track of the renamed model list value (so that we can take advantage
    // of it in our calls to updateGraphsInfo() in updateAllGraphsInfo())

    QString oldModelListValue = modelListValue(pOldFileName);

    if (std::find(mRenamedModelListValues.begin(), mRenamedModelListValues.end(), oldModelListValue) != mRenamedModelListValues.end()) {
        // A previous model list value has been renamed again, so find its
        // original value and update its new renamed version, if needed

        QStringList origModelListValueKeys = mRenamedModelListValues.keys();

        for (const auto &origModelListValue : origModelListValueKeys) {
            QString renamedOrigModelListValue = mRenamedModelListValues.value(origModelListValue);

            if (renamedOrigModelListValue == oldModelListValue) {
                QString newModelListValue = modelListValue(pNewFileName);

                if (origModelListValue == newModelListValue) {
                    // A model list value has been re-renamed to its original
                    // value, so stop tracking it

                    mRenamedModelListValues.remove(origModelListValue);
                } else {
                    mRenamedModelListValues.insert(origModelListValue, newModelListValue);
                }

                break;
            }
        }
    } else {
        // Not a renamed model list value that we know, so just keep track of it

        mRenamedModelListValues.insert(oldModelListValue, modelListValue(pNewFileName));
    }
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::updateGui()
{
    // Update our graphs information
    // Note: this is useful, for example, if we created a graph for a file that
    //       has not yet been selected, in which case the graph will initially
    //       be invalid, but it should become valid after we have switched to
    //       that file and back...

    updateAllGraphsInfo();
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::finishEditing()
{
    // Make sure that we have a graphs property editor

    if (mGraphsPropertyEditor == nullptr) {
        return;
    }

    // Ask our graphs property editor to finish editing

    mGraphsPropertyEditor->finishEditing();
}

//==============================================================================

Core::PropertyEditorWidget * SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphPanelPropertyEditor(GraphPanelWidget::GraphPanelWidget *pGraphPanel) const
{
    // Retrieve and return the graph panel property editor associated with the
    // given graph panel

    return mGraphPanelPropertyEditors.value(pGraphPanel);
}

//==============================================================================

Core::PropertyEditorWidget * SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphsPropertyEditor(GraphPanelWidget::GraphPanelWidget *pGraphPanel) const
{
    // Retrieve and return the graphs property editor associated with the given
    // graph panel

    return mGraphsPropertyEditors.value(pGraphPanel);
}

//==============================================================================

static const char *PropertySeparator = " | ";

//==============================================================================

Core::Properties SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphsProperties(GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                                                                                                const QString &pFileName) const
{
    // Retrieve and return all the graph properties associated with the given
    // graph panel and file name, if any

    Core::Properties res;

    for (auto property : mGraphsPropertyEditors.value(pGraphPanel)->properties()) {
        // The property should be returned if its first sub-property (i.e. to
        // which model the graph applies) has either a value of "Current" or
        // that of the given file name

        QString modelPropertyValue = property->properties().first()->value();

        if (   (modelPropertyValue == tr("Current"))
            || (modelPropertyValue.split(PropertySeparator).last() == pFileName)) {
            res << property;
        }
    }

    return res;
}

//==============================================================================

SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode SimulationExperimentViewInformationGraphPanelAndGraphsWidget::mode() const
{
    // Return our mode

    return mMode;
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::setMode(Mode pMode)
{
    // Set our mode

    if (pMode != mMode) {
        mMode = pMode;

        // Set our graph panel or graphs property editor as our current widget,
        // if we have one (i.e. the model could be compiled)

        Core::PropertyEditorWidget *propertyEditor = (pMode == Mode::GraphPanel)?mGraphPanelPropertyEditor:mGraphsPropertyEditor;

        if (propertyEditor != nullptr) {
            setCurrentWidget(propertyEditor);
        }

        // Let people know about our change of modes

        emit graphPanelGraphsModeChanged(pMode);
    }
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::setGraphPanelColumnWidth(int pIndex,
                                                                                            int pColumnWidth)
{
    // Make sure that we have a graph panel property editor

    if (mGraphPanelPropertyEditor == nullptr) {
        return;
    }

    // Set the width of the given column

    mGraphPanelPropertyEditor->setColumnWidth(pIndex, pColumnWidth);
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::setGraphsColumnWidth(int pIndex,
                                                                                        int pColumnWidth)
{
    // Make sure that we have a graphs property editor

    if (mGraphsPropertyEditor == nullptr) {
        return;
    }

    // Set the width of the given column

    mGraphsPropertyEditor->setColumnWidth(pIndex, pColumnWidth);
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::setGraphPanelSectionExpanded(int pSection,
                                                                                                bool pExpanded)
{
    // Make sure that we have a graph panel property editor

    if (mGraphPanelPropertyEditor == nullptr) {
        return;
    }

    // Expand/collapse the given section

    mGraphPanelPropertyEditor->setExpanded(mGraphPanelPropertyEditor->model()->index(pSection, 0), pExpanded);
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::reinitialize(GraphPanelWidget::GraphPanelWidget *pGraphPanel)
{
    // Reinitialise ourselves

    finalize(pGraphPanel);
    initialize(pGraphPanel);
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::showGraphPanelContextMenu(const QPoint &pPosition) const
{
    Q_UNUSED(pPosition)

    // Make sure that we have a graph panel property editor

    if (mGraphPanelPropertyEditor == nullptr) {
        return;
    }

    // Show the graph panel context menu, or not, depending on the type of
    // property we are dealing with, if any

    Core::Property *crtProperty = mGraphPanelPropertyEditor->currentProperty();

    if (   (crtProperty != nullptr)
        && (crtProperty->type() == Core::Property::Type::Color)) {
        mGraphPanelContextMenu->exec(QCursor::pos());
    }
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::showGraphsContextMenu(const QPoint &pPosition) const
{
    Q_UNUSED(pPosition)

    // Make sure that we have a graphs property editor

    if (mGraphsPropertyEditor == nullptr) {
        return;
    }

    // Update the enabled state of some of our actions

    Core::Property *crtProperty = mGraphsPropertyEditor->currentProperty();

    mRemoveCurrentGraphAction->setEnabled(crtProperty != nullptr);
    mRemoveAllGraphsAction->setEnabled(!mGraphsPropertyEditor->properties().isEmpty());

    bool canSelectAllGraphs = false;
    bool canUnselectAllGraphs = false;

    for (auto property : mGraphsPropertyEditor->properties()) {
        bool graphSelected = property->isChecked();

        canSelectAllGraphs = canSelectAllGraphs || !graphSelected;
        canUnselectAllGraphs = canUnselectAllGraphs || graphSelected;
    }

    mSelectAllGraphsAction->setEnabled(canSelectAllGraphs);
    mUnselectAllGraphsAction->setEnabled(canUnselectAllGraphs);

    mSelectGraphColorAction->setVisible(   (crtProperty != nullptr)
                                        && (crtProperty->type() == Core::Property::Type::Color));

    // Show the graph context menu, or not, depending on the type of property we
    // are dealing with, if any

    if (   (crtProperty != nullptr)
        && (   (crtProperty->name() == tr("X"))
            || (crtProperty->name() == tr("Y")))) {
        mGraphParametersContextMenu->exec(QCursor::pos());
    } else {
        mGraphContextMenu->exec(QCursor::pos());
    }
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::populateGraphPanelPropertyEditor()
{
    // Populate our graph panel property editor

    GraphPanelWidget::GraphPanelPlotWidget *graphPanelPlot = mGraphPanels.value(mGraphPanelPropertyEditor)->plot();

    mGraphPanelPropertyEditor->addColorProperty(graphPanelPlot->backgroundColor());
    mGraphPanelPropertyEditor->addIntegerGt0Property(graphPanelPlot->fontSize());
    mGraphPanelPropertyEditor->addColorProperty(graphPanelPlot->foregroundColor());

    // Grid lines

    Core::Property *gridLinesProperty = mGraphPanelPropertyEditor->addSectionProperty();

    mGraphPanelPropertyEditor->addListProperty(SEDMLSupport::lineStyles(),
                                               SEDMLSupport::stringLineStyle(graphPanelPlot->gridLinesStyle()),
                                               gridLinesProperty);
    mGraphPanelPropertyEditor->addIntegerGt0Property(graphPanelPlot->gridLinesWidth(), gridLinesProperty);
    mGraphPanelPropertyEditor->addColorProperty(graphPanelPlot->gridLinesColor(), gridLinesProperty);

    // Legend

    Core::Property *legendProperty = mGraphPanelPropertyEditor->addSectionProperty();

    mGraphPanelPropertyEditor->addIntegerGt0Property(graphPanelPlot->legendFontSize(), legendProperty);
    mGraphPanelPropertyEditor->addBooleanProperty(graphPanelPlot->isLegendVisible(), legendProperty);

    // Point coordinates

    Core::Property *pointCoordinatesProperty = mGraphPanelPropertyEditor->addSectionProperty();

    mGraphPanelPropertyEditor->addListProperty(SEDMLSupport::lineStyles(),
                                               SEDMLSupport::stringLineStyle(graphPanelPlot->pointCoordinatesStyle()),
                                               pointCoordinatesProperty);
    mGraphPanelPropertyEditor->addIntegerGt0Property(graphPanelPlot->pointCoordinatesWidth(), pointCoordinatesProperty);
    mGraphPanelPropertyEditor->addColorProperty(graphPanelPlot->pointCoordinatesColor(), pointCoordinatesProperty);
    mGraphPanelPropertyEditor->addColorProperty(graphPanelPlot->pointCoordinatesFontColor(), pointCoordinatesProperty);
    mGraphPanelPropertyEditor->addIntegerGt0Property(graphPanelPlot->pointCoordinatesFontSize(), pointCoordinatesProperty);

    // Surrounding area

    Core::Property *surroundingAreaProperty = mGraphPanelPropertyEditor->addSectionProperty();

    mGraphPanelPropertyEditor->addColorProperty(graphPanelPlot->surroundingAreaBackgroundColor(), surroundingAreaProperty);
    mGraphPanelPropertyEditor->addColorProperty(graphPanelPlot->surroundingAreaForegroundColor(), surroundingAreaProperty);

    // Title

    mGraphPanelPropertyEditor->addStringProperty(graphPanelPlot->title().text());

    // X axis

    Core::Property *xAxisProperty = mGraphPanelPropertyEditor->addSectionProperty();

    mGraphPanelPropertyEditor->addIntegerGt0Property(graphPanelPlot->fontSizeAxisX(), xAxisProperty);
    mGraphPanelPropertyEditor->addBooleanProperty(graphPanelPlot->logAxisX(), xAxisProperty);
    mGraphPanelPropertyEditor->addStringProperty(graphPanelPlot->titleAxisX(), xAxisProperty);

    // Y axis

    Core::Property *yAxisProperty = mGraphPanelPropertyEditor->addSectionProperty();

    mGraphPanelPropertyEditor->addIntegerGt0Property(graphPanelPlot->fontSizeAxisY(), yAxisProperty);
    mGraphPanelPropertyEditor->addBooleanProperty(graphPanelPlot->logAxisY(), yAxisProperty);
    mGraphPanelPropertyEditor->addStringProperty(graphPanelPlot->titleAxisY(), yAxisProperty);

    // Zoom region

    Core::Property *zoomRegionProperty = mGraphPanelPropertyEditor->addSectionProperty();

    mGraphPanelPropertyEditor->addListProperty(SEDMLSupport::lineStyles(),
                                               SEDMLSupport::stringLineStyle(graphPanelPlot->zoomRegionStyle()),
                                               zoomRegionProperty);
    mGraphPanelPropertyEditor->addIntegerGt0Property(graphPanelPlot->zoomRegionWidth(), zoomRegionProperty);
    mGraphPanelPropertyEditor->addColorProperty(graphPanelPlot->zoomRegionColor(), zoomRegionProperty);
    mGraphPanelPropertyEditor->addColorProperty(graphPanelPlot->zoomRegionFontColor(), zoomRegionProperty);
    mGraphPanelPropertyEditor->addIntegerGt0Property(graphPanelPlot->zoomRegionFontSize(), zoomRegionProperty);
    mGraphPanelPropertyEditor->addBooleanProperty(graphPanelPlot->zoomRegionFilled(), zoomRegionProperty);
    mGraphPanelPropertyEditor->addColorProperty(graphPanelPlot->zoomRegionFillColor(), zoomRegionProperty);

    // Retranslate the contents of our graph panel property editor

    retranslateGraphPanelPropertyEditor(mGraphPanelPropertyEditor);
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::populateGraphParametersContextMenu(CellMLSupport::CellmlFileRuntime *pRuntime)
{
    // Populate our graph parameters context menu with the contents of our main
    // graph context menu

    mGraphParametersContextMenu->addActions(mGraphContextMenu->actions());
    mGraphParametersContextMenu->addSeparator();

    // Now, add our model parameters to it

    QString componentHierarchy;
    QMenu *componentMenu = nullptr;

    for (auto parameter : pRuntime->parameters()) {
        // Check whether the current parameter is in the same component
        // hierarchy as the previous one

        QString crtComponentHierarchy = parameter->formattedComponentHierarchy();

        if (crtComponentHierarchy != componentHierarchy) {
            // The current parameter is in a different component hierarchy, so
            // create a new menu hierarchy for our 'new' component, reusing
            // existing menus, whenever possible

            QMenu *parentComponentMenu = mGraphParametersContextMenu;

            for (const auto &component : parameter->componentHierarchy()) {
                // Check whether we already have a menu for our current
                // component

                componentMenu = nullptr;

                for (auto object : parentComponentMenu->children()) {
                    auto subMenu = qobject_cast<QMenu *>(object);

                    if ((subMenu != nullptr) && (subMenu->menuAction()->text() == component)) {
                        componentMenu = subMenu;

                        break;
                    }
                }

                // Create a new menu for our current component, if none could be
                // found

                if (componentMenu == nullptr) {
                    componentMenu = new QMenu(component, parentComponentMenu);

                    parentComponentMenu->addMenu(componentMenu);
                }

                // Get ready for the next component in our component hierarchy

                parentComponentMenu = componentMenu;
            }

            // Keep track of the new component hierarchy

            componentHierarchy = crtComponentHierarchy;
        }

        // Make sure that we have a 'current' component menu
        // Note: this should never happen, but we never know...

        if (componentMenu == nullptr) {
            continue;
        }

        // Add the current parameter to the 'current' component menu

        QAction *parameterAction = componentMenu->addAction(CellMLSupport::CellmlFileRuntimeParameter::icon(parameter->type()),
                                                            parameter->formattedName());

        // Create a connection to handle the parameter value update

        connect(parameterAction, &QAction::triggered,
                this, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::updateParameterValue);

        // Keep track of the parameter associated with our parameter action

        mParameterActions.insert(parameterAction, parameter);
    }
}

//==============================================================================

bool SimulationExperimentViewInformationGraphPanelAndGraphsWidget::checkParameter(CellMLSupport::CellmlFileRuntime *pRuntime,
                                                                                  GraphPanelWidget::GraphPanelPlotGraph *pGraph,
                                                                                  Core::Property *pParameterProperty,
                                                                                  bool pParameterX) const
{
    // Check that the information held by the given property corresponds to
    // an existing parameter in our runtime

    CellMLSupport::CellmlFileRuntimeParameter *res = nullptr;

    if (pRuntime != nullptr) {
        // Retrieve the component and parameter of the property

        QStringList info = pParameterProperty->value().split('.');
        QStringList componentHierarchy = info;

        componentHierarchy.removeLast();

        // Determine the degree of our parameter, if any

        QString parameterName = info.last();
        int parameterDegree = parameterName.length();

        parameterName.replace('\'', QString());

        parameterDegree -= parameterName.length();

        // Check whether we can find our property among our runtime's parameters

        for (auto parameter : pRuntime->parameters()) {
            if (   (parameter->componentHierarchy() == componentHierarchy)
                && (parameter->name() == parameterName)
                && (parameter->degree() == parameterDegree)) {
                res = parameter;

                break;
            }
        }
    }

    // Update our parameter property based on whether it corresponds to an
    // existing parameter in our runtime

    static const QIcon BlankIcon   = QIcon(":/SimulationExperimentView/blank.png");
    static const QIcon WarningIcon = QIcon(":/oxygen/status/task-attention.png");

    pParameterProperty->setIcon((res != nullptr)?BlankIcon:WarningIcon);
    pParameterProperty->setExtraInfo((res != nullptr)?
                                         QString():
                                         (pRuntime != nullptr)?
                                             tr("does not exist"):
                                             tr("no runtime"));

    // Keep track of the existing parameter, if any, to which our property
    // corresponds

    if (pParameterX) {
        pGraph->setParameterX(res);
    } else {
        pGraph->setParameterY(res);
    }

    return res != nullptr;
}

//==============================================================================

QString SimulationExperimentViewInformationGraphPanelAndGraphsWidget::modelListValue(const QString &pFileName) const
{
    // Return the model list value for the given file name

    return QFileInfo(pFileName).fileName()+PropertySeparator+pFileName;
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::updateGraphInfo(Core::Property *pProperty)
{
    // Make sure that we have a property

    if (pProperty == nullptr) {
        return;
    }

    // Update the model property's icon, based on the value of the model
    // property, and determine the file name against which we have to check our
    // X and Y properties

    static const QIcon LockedIcon   = QIcon(":/oxygen/status/object-locked.png");
    static const QIcon UnlockedIcon = QIcon(":/oxygen/status/object-unlocked.png");

    GraphPanelWidget::GraphPanelPlotGraph *graph = mGraphs.value(pProperty);
    Core::Properties properties = pProperty->properties();
    QString propertyFileName = properties[0]->value();
    QString fileName = mSimulationWidget->simulation()->fileName();

    if (propertyFileName == tr("Current")) {
        properties[0]->setIcon(UnlockedIcon);
    } else {
        properties[0]->setIcon(LockedIcon);

        fileName = propertyFileName.split(PropertySeparator).last();
    }

    // Check that the parameters represented by the value of the X and Y
    // properties exist for the current/selected model
    // Note: we absolutely want to check the parameter (so that an icon can be
    //       assigned to its corresponding property) , hence the order of our &&
    //       tests when setting graphOk...

    bool graphOk = true;
    CellMLSupport::CellmlFileRuntime *runtime = mViewWidget->runtime(fileName);
    auto oldParameterX = static_cast<CellMLSupport::CellmlFileRuntimeParameter *>(graph->parameterX());
    auto oldParameterY = static_cast<CellMLSupport::CellmlFileRuntimeParameter *>(graph->parameterY());

    graphOk = checkParameter(runtime, graph, properties[2], true) && graphOk;
    graphOk = checkParameter(runtime, graph, properties[3], false) && graphOk;

    // Update our section's name, if possible
    // Note: indeed, when populating ourselves, updateGraphInfo() gets called
    //       (through graphsPropertyChanged()), yet we don't want to (and can't)
    //       do what follows if not all the properties are available...

    pProperty->setName( properties[2]->value()
                       +PropertySeparator
                       +properties[3]->value());

    // Update the graph's title

    QString oldTitle = graph->title();
    QString newTitle = properties[1]->value();
    auto newParameterY = static_cast<CellMLSupport::CellmlFileRuntimeParameter *>(graph->parameterY());

    if (   (newParameterY != nullptr) && (newParameterY != oldParameterY)
        && oldTitle.isEmpty() && newTitle.isEmpty()) {
        // The graph didn't and still doesn't have a title, and we have a valid
        // (and different) Y property, so use its formatted name as a default
        // value for our graph's title

        newTitle = newParameterY->formattedName();

        disconnect(pProperty->owner(), &Core::PropertyEditorWidget::propertyChanged,
                   this, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphsPropertyChanged);

        properties[1]->setValue(newTitle);

        connect(pProperty->owner(), &Core::PropertyEditorWidget::propertyChanged,
                this, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphsPropertyChanged);
    }

    graph->setTitle(newTitle);

    // Update the status (i.e. icon) of our (section) property

    static const QIcon BlankIcon   = QIcon(":/SimulationExperimentView/blank.png");
    static const QIcon WarningIcon = QIcon(":/oxygen/status/task-attention.png");

    pProperty->setIcon(graphOk?BlankIcon:WarningIcon);

    // Update the file name with which the graph is associated

    graph->setFileName(fileName);

    // Update the graph line settings

    QPen oldLinePen = graph->pen();
    QPen newLinePen = oldLinePen;
    Core::Properties lineProperties = properties[4]->properties();

    newLinePen.setStyle(SEDMLSupport::lineStyle(lineProperties[0]->listValueIndex()));
    newLinePen.setWidth(lineProperties[1]->integerValue());
    newLinePen.setColor(lineProperties[2]->colorValue());

    graph->setPen(newLinePen);

    // Update the graph symbol settings

    const QwtSymbol *oldGraphSymbol = graph->symbol();
    bool graphSymbolUpdated = oldGraphSymbol == nullptr;
    Core::Properties symbolProperties = properties[5]->properties();
    QwtSymbol::Style symbolStyle = SEDMLSupport::symbolStyle(symbolProperties[0]->listValueIndex());
    int symbolSizeValue = symbolProperties[1]->integerValue();
    QSize symbolSize = QSize(symbolSizeValue, symbolSizeValue);
    QPen symbolColor = QPen(symbolProperties[2]->colorValue());
    bool symbolFill = symbolProperties[3]->booleanValue();
    QBrush symbolFillColor = symbolFill?QBrush(symbolProperties[4]->colorValue()):QBrush();

    if (oldGraphSymbol != nullptr) {
        graphSymbolUpdated =    (oldGraphSymbol->style() != symbolStyle)
                             || (oldGraphSymbol->size() != symbolSize)
                             || (oldGraphSymbol->pen() != symbolColor)
                             || (oldGraphSymbol->brush() != symbolFillColor);
    }

    graph->setSymbol(symbolStyle, symbolFillColor, symbolColor, symbolSize);

    // Update our graph's GUI if the title of our graph or its symbol is
    // different (both of which may result in our legend's width being updated),
    // let people know if the X and/or Y parameters of our graph have changed,
    // or replot it if its settings have changed
    // Note: we want several if statements rather than if...elseif...elseif...
    //       Indeed, to change the Y property may, for example, result in the
    //       title being also changed...

    if ((newTitle != oldTitle) || graphSymbolUpdated) {
        graph->plot()->updateGui(false, graph->plot()->isLegendVisible());
    }

    if (   (oldParameterX != graph->parameterX())
        || (oldParameterY != graph->parameterY())) {
        emit graphUpdated(graph);
    }

    if ((oldLinePen != newLinePen) || graphSymbolUpdated) {
        graph->plot()->replot();

        processEvents();
        // Note: this ensures that our plot is updated at once...
    }
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphPanelSectionExpanded(const QModelIndex &pIndex)
{
    // Let people know that the given section has been expanded

    emit graphPanelSectionExpanded(pIndex.row(), true);
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphPanelSectionCollapsed(const QModelIndex &pIndex)
{
    // Let people know that the given section has been collapsed

    emit graphPanelSectionExpanded(pIndex.row(), false);
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphPanelPropertyChanged(Core::Property *pProperty)
{
    // Update our graph panel settings

    Core::PropertyEditorWidget *propertyEditor = pProperty->owner();
    GraphPanelWidget::GraphPanelPlotWidget *graphPanelPlot = mGraphPanels.value(propertyEditor)->plot();
    Core::Properties properties = propertyEditor->properties();
    Core::Properties gridLinesProperties = properties[3]->properties();
    Core::Properties legendProperties = properties[4]->properties();
    Core::Properties pointCoordinatesProperties = properties[5]->properties();
    Core::Properties surroundingAreaProperties = properties[6]->properties();
    Core::Properties xAxisProperties = properties[8]->properties();
    Core::Properties yAxisProperties = properties[9]->properties();
    Core::Properties zoomRegionProperties = properties[10]->properties();

    // Graph panel

    if (pProperty == properties[0]) {
        graphPanelPlot->setBackgroundColor(pProperty->colorValue());
    } else if (pProperty == properties[1]) {
        graphPanelPlot->setFontSize(pProperty->integerValue());
    } else if (pProperty == properties[2]) {
        graphPanelPlot->setForegroundColor(pProperty->colorValue());

    // Grid lines

    } else if (pProperty == gridLinesProperties[0]) {
        graphPanelPlot->setGridLinesStyle(SEDMLSupport::lineStyle(pProperty->listValueIndex()));
    } else if (pProperty == gridLinesProperties[1]) {
        graphPanelPlot->setGridLinesWidth(pProperty->integerValue());
    } else if (pProperty == gridLinesProperties[2]) {
        graphPanelPlot->setGridLinesColor(pProperty->colorValue());

    // Legend

    } else if (pProperty == legendProperties[0]) {
        graphPanelPlot->setLegendFontSize(pProperty->integerValue());
    } else if (pProperty == legendProperties[1]) {
        graphPanelPlot->setLegendVisible(pProperty->booleanValue());

    // Point coordinates

    } else if (pProperty == pointCoordinatesProperties[0]) {
        graphPanelPlot->setPointCoordinatesStyle(SEDMLSupport::lineStyle(pProperty->listValueIndex()));
    } else if (pProperty == pointCoordinatesProperties[1]) {
        graphPanelPlot->setPointCoordinatesWidth(pProperty->integerValue());
    } else if (pProperty == pointCoordinatesProperties[2]) {
        graphPanelPlot->setPointCoordinatesColor(pProperty->colorValue());
    } else if (pProperty == pointCoordinatesProperties[3]) {
        graphPanelPlot->setPointCoordinatesFontColor(pProperty->colorValue());
    } else if (pProperty == pointCoordinatesProperties[4]) {
        graphPanelPlot->setPointCoordinatesFontSize(pProperty->integerValue());

    // Surrounding area

    } else if (pProperty == surroundingAreaProperties[0]) {
        graphPanelPlot->setSurroundingAreaBackgroundColor(pProperty->colorValue());
    } else if (pProperty == surroundingAreaProperties[1]) {
        graphPanelPlot->setSurroundingAreaForegroundColor(pProperty->colorValue());

    // Title

    } else if (pProperty == properties[7]) {
        graphPanelPlot->setTitle(pProperty->value());

    // X axis

    } else if (pProperty == xAxisProperties[0]) {
        graphPanelPlot->setFontSizeAxisX(pProperty->integerValue());
    } else if (pProperty == xAxisProperties[1]) {
        graphPanelPlot->setLogAxisX(pProperty->booleanValue());
    } else if (pProperty == xAxisProperties[2]) {
        graphPanelPlot->setTitleAxisX(pProperty->value());

    // Y axis

    } else if (pProperty == yAxisProperties[0]) {
        graphPanelPlot->setFontSizeAxisY(pProperty->integerValue());
    } else if (pProperty == yAxisProperties[1]) {
        graphPanelPlot->setLogAxisY(pProperty->booleanValue());
    } else if (pProperty == yAxisProperties[2]) {
        graphPanelPlot->setTitleAxisY(pProperty->value());

    // Zoom region

    } else if (pProperty == zoomRegionProperties[0]) {
        graphPanelPlot->setZoomRegionStyle(SEDMLSupport::lineStyle(pProperty->listValueIndex()));
    } else if (pProperty == zoomRegionProperties[1]) {
        graphPanelPlot->setZoomRegionWidth(pProperty->integerValue());
    } else if (pProperty == zoomRegionProperties[2]) {
        graphPanelPlot->setZoomRegionColor(pProperty->colorValue());
    } else if (pProperty == zoomRegionProperties[3]) {
        graphPanelPlot->setZoomRegionFontColor(pProperty->colorValue());
    } else if (pProperty == zoomRegionProperties[4]) {
        graphPanelPlot->setZoomRegionFontSize(pProperty->integerValue());
    } else if (pProperty == zoomRegionProperties[5]) {
        graphPanelPlot->setZoomRegionFilled(pProperty->booleanValue());
    } else if (pProperty == zoomRegionProperties[6]) {
        graphPanelPlot->setZoomRegionFillColor(pProperty->colorValue());
    }
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphsPropertyChanged(Core::Property *pProperty)
{
    // Our graph has changed, which means that either it has been un/selected or
    // that the value of one of its properties has changed

    if (rootProperty(pProperty)) {
        // The property associated with the graph is our root property, which
        // means that the graph has been un/selected, so update its selected
        // state and let people know that our graph has been updated

        GraphPanelWidget::GraphPanelPlotGraph *graph = mGraphs.value(pProperty);

        if (graph != nullptr) {
            graph->setSelected(pProperty->isChecked());

            emit graphUpdated(graph);
        }
    } else {
        // One of our graph properties has changed, so update its information
        // Note: updateGraphInfo() will emit the graphUpdated() signal, if
        //       needed...

        Core::Property *graphProperty = pProperty->parentProperty();

        while (graphProperty->parentProperty() != nullptr) {
            graphProperty = graphProperty->parentProperty();
        }

        updateGraphInfo(graphProperty);
    }
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::updateGraphsInfo(Core::Property *pSectionProperty)
{
    // Make sure that we have a graphs property editor

    if (mGraphsPropertyEditor == nullptr) {
        return;
    }

    // Make sure that no editing is in progress

    finishEditing();

    // Use the given section property or retrieve the ones for our current
    // graphs property editor

    Core::Properties graphProperties;

    if (pSectionProperty != nullptr) {
        graphProperties << pSectionProperty;
    } else {
        for (auto property : mGraphsPropertyEditor->properties()) {
            if (rootProperty(property)) {
                graphProperties << property;
            }
        }
    }

    // Determine the model list values, but only if needed, i.e. if we have some
    // graph properties

    QStringList modelListValues;

    if (!graphProperties.isEmpty()) {
        for (const auto &fileName : mViewWidget->fileNames()) {
            Core::File *file = Core::FileManager::instance()->file(fileName);
            QString fileNameOrUrl = file->isLocal()?fileName:file->url();

            modelListValues << modelListValue(fileNameOrUrl);
        }

        modelListValues.sort(Qt::CaseInsensitive);

        modelListValues.prepend({});
        modelListValues.prepend(tr("Current"));
    }

    // Go through our graph properties and update (incl. retranslate) their
    // information

    for (auto graphProperty : graphProperties) {
        // Set the label of our graph properties

        Core::Properties graphPropertyProperties = graphProperty->properties();

        graphPropertyProperties[0]->setName(tr("Model"));
        graphPropertyProperties[1]->setName(tr("Title"));
        graphPropertyProperties[2]->setName(tr("X"));
        graphPropertyProperties[3]->setName(tr("Y"));

        // Set the label of our graph line properties

        Core::Properties lineProperties = graphPropertyProperties[4]->properties();

        graphPropertyProperties[4]->setName(tr("Line"));

        lineProperties[0]->setName(tr("Style"));
        lineProperties[0]->setListValues(SEDMLSupport::formattedLineStyles(), false);
        lineProperties[1]->setName(tr("Width"));
        lineProperties[2]->setName(tr("Colour"));

        // Set the label of our graph symbol properties

        Core::Properties symbolProperties = graphPropertyProperties[5]->properties();

        graphPropertyProperties[5]->setName(tr("Symbol"));

        symbolProperties[0]->setName(tr("Style"));
        symbolProperties[0]->setListValues(SEDMLSupport::formattedSymbolStyles(), false);
        symbolProperties[1]->setName(tr("Size"));
        symbolProperties[2]->setName(tr("Colour"));
        symbolProperties[3]->setName(tr("Filled"));
        symbolProperties[4]->setName(tr("Fill colour"));

        // Keep track of the current model value, so that we can safely update
        // its list values and then select the correct model value back

        QString oldModelValue = graphPropertyProperties[0]->value();
        QString newModelValue = oldModelValue;

        graphPropertyProperties[0]->setListValues(modelListValues, false);
        // Note: we don't want setListValues() to emit a signal since one will
        //       be emitted as a result of our call to setValue() below...

        if (newModelValue.isEmpty()) {
            // newModelValue is empty, which means that this is the first time
            // we come here, so update our graph info using the current value of
            // our model property

            newModelValue = graphPropertyProperties[0]->value();
        } else if (!modelListValues.contains(oldModelValue)) {
            // Our old model value is not in our new model list values anymore,
            // which means that either the value of the model property was
            // "Current" (and the locale got changed) or the current file got
            // renamed

            if (oldModelValue.contains(PropertySeparator)) {
                // The current file got renamed

                newModelValue = mRenamedModelListValues.value(oldModelValue);
            } else {
                // The value of the model property was "Current" (and the locale
                // got changed)

                newModelValue = tr("Current");
            }
        }

        // Set the value of our model property to newModelValue (which will
        // result in updateGraphInfo() being called, so we are all fine)

        graphPropertyProperties[0]->setValue(newModelValue, true);
        // Note: we must force the setting of the value since it may very well
        //       be that it's the same as before, yet we want the signal
        //       associated with setValue() to be emitted (so that
        //       updateGraphInfo() can then be called)...
    }
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::updateAllGraphsInfo()
{
    // Update the information about our graphs properties and this for all our
    // property editors, so first keep track of our active graphs property
    // editor

    Core::PropertyEditorWidget *oldGraphsPropertyEditor = mGraphsPropertyEditor;

    // Go through our graphs property editors and update the information about
    // the graph properties they hold

    for (auto graphsPropertyEditor : mGraphsPropertyEditors) {
        mGraphsPropertyEditor = graphsPropertyEditor;

        updateGraphsInfo();
    }

    // Our list of renamed model list values was used in our different calls to
    // updateGraphsInfo() above, so it can now be reset

    mRenamedModelListValues.clear();

    // Retrieve our originally active graphs property editor

    mGraphsPropertyEditor = oldGraphsPropertyEditor;
}

//==============================================================================

void SimulationExperimentViewInformationGraphPanelAndGraphsWidget::updateParameterValue()
{
    // Update the current property's value

    mGraphsPropertyEditor->currentProperty()->setValue(mParameterActions.value(qobject_cast<QAction *>(sender()))->fullyFormattedName());
}

//==============================================================================

} // namespace SimulationExperimentView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
