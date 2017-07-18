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
// Simulation Experiment view information graphs widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "coreguiutils.h"
#include "filemanager.h"
#include "graphpanelwidget.h"
#include "simulationexperimentviewinformationgraphswidget.h"
#include "simulationexperimentviewplugin.h"
#include "simulationexperimentviewsimulation.h"
#include "simulationexperimentviewsimulationwidget.h"
#include "simulationexperimentviewwidget.h"

//==============================================================================

#include <QFileInfo>
#include <QHeaderView>
#include <QMenu>
#include <QScrollBar>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace SimulationExperimentView {

//==============================================================================

SimulationExperimentViewInformationGraphsWidget::SimulationExperimentViewInformationGraphsWidget(SimulationExperimentViewWidget *pViewWidget,
                                                                                                 SimulationExperimentViewSimulationWidget *pSimulationWidget,
                                                                                                 QWidget *pParent) :
    QStackedWidget(pParent),
    Core::CommonWidget(this),
    mViewWidget(pViewWidget),
    mSimulationWidget(pSimulationWidget),
    mGraphPanels(QMap<Core::PropertyEditorWidget *, GraphPanelWidget::GraphPanelWidget *>()),
    mPropertyEditors(QMap<GraphPanelWidget::GraphPanelWidget *, Core::PropertyEditorWidget *>()),
    mPropertyEditor(0),
    mGraphs(QMap<Core::Property *, GraphPanelWidget::GraphPanelPlotGraph *>()),
    mGraphProperties(QMap<GraphPanelWidget::GraphPanelPlotGraph *, Core::Property *>()),
    mParameterActions(QMap<QAction *, CellMLSupport::CellmlFileRuntimeParameter *>()),
    mHorizontalScrollBarValue(0)
{
    // Create our context menus and populate our main context menu

    mContextMenu = new QMenu(this);
    mParametersContextMenu = new QMenu(this);

    mAddGraphAction = Core::newAction(this);
    mRemoveCurrentGraphAction = Core::newAction(this);
    mRemoveAllGraphsAction = Core::newAction(this);
    mSelectAllGraphsAction = Core::newAction(this);
    mUnselectAllGraphsAction = Core::newAction(this);

    connect(mAddGraphAction, SIGNAL(triggered(bool)),
            this, SLOT(addGraph()));
    connect(mRemoveCurrentGraphAction, SIGNAL(triggered(bool)),
            this, SLOT(removeCurrentGraph()));
    connect(mRemoveAllGraphsAction, SIGNAL(triggered(bool)),
            this, SLOT(removeAllGraphs()));
    connect(mSelectAllGraphsAction, SIGNAL(triggered(bool)),
            this, SLOT(selectAllGraphs()));
    connect(mUnselectAllGraphsAction, SIGNAL(triggered(bool)),
            this, SLOT(unselectAllGraphs()));

    mContextMenu->addAction(mAddGraphAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mRemoveCurrentGraphAction);
    mContextMenu->addAction(mRemoveAllGraphsAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mSelectAllGraphsAction);
    mContextMenu->addAction(mUnselectAllGraphsAction);

    // Some further initialisations that are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::retranslateUi()
{
    // Retranslate our actions

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

    // Retranslate all our property editors

    foreach (Core::PropertyEditorWidget *propertyEditor, mPropertyEditors)
        propertyEditor->retranslateUi();

    // Retranslate the information about our graphs properties
    // Note: no need to do this for all our property editors (i.e. call
    //       updateAllGraphsInfo()) since this will automatically be done when
    //       selecting another graph panel. So, instead, we just do this for our
    //       current graph panel...

    updateGraphsInfo();
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::initialize(SimulationExperimentViewSimulation *pSimulation)
{
    // Populate our parameters context menu

    populateParametersContextMenu(pSimulation->runtime());

    // Update our graphs information

    updateAllGraphsInfo();
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::finalize()
{
    // Clear our parameters context menu

    mParametersContextMenu->clear();
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::initialize(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                                                                 const bool &pActive)
{
    // Retrieve the property editor for the given file name or create one, if
    // none exists

    Core::PropertyEditorWidget *oldPropertyEditor = mPropertyEditor;

    mPropertyEditor = mPropertyEditors.value(pGraphPanel);

    if (!mPropertyEditor) {
        // No property editor exists for the given graph panel, so create one

        mPropertyEditor = new Core::PropertyEditorWidget(false, false, this);

        // We want our own context menu for our property editor

        mPropertyEditor->setContextMenuPolicy(Qt::CustomContextMenu);

        connect(mPropertyEditor, SIGNAL(customContextMenuRequested(const QPoint &)),
                this, SLOT(propertyEditorContextMenu(const QPoint &)));

        // Keep track of changes to the horizontal bar's value

        connect(mPropertyEditor->horizontalScrollBar(), SIGNAL(valueChanged(int)),
                this, SLOT(propertyEditorHorizontalScrollBarValueChanged(const int &)));

        // Keep track of changes to columns' width

        connect(mPropertyEditor->header(), SIGNAL(sectionResized(int, int, int)),
                this, SLOT(propertyEditorSectionResized(const int &, const int &, const int &)));

        // Keep track of when the user changes a property value

        connect(mPropertyEditor, SIGNAL(propertyChanged(Core::Property *)),
                this, SLOT(graphChanged(Core::Property *)));

        // Add our new property editor to ourselves

        addWidget(mPropertyEditor);

        // Keep track of the link between our existing graph panel and our new
        // property editor

        mGraphPanels.insert(mPropertyEditor, pGraphPanel);
        mPropertyEditors.insert(pGraphPanel, mPropertyEditor);
    }

    // Set the value of the property editor's horizontal scroll bar

    mPropertyEditor->horizontalScrollBar()->setValue(mHorizontalScrollBarValue);

    // Make sure that our 'new' property editor's columns' width is the same as
    // that of our 'old' property editor

    if (oldPropertyEditor) {
        for (int i = 0, iMax = oldPropertyEditor->header()->count(); i < iMax; ++i)
            mPropertyEditor->setColumnWidth(i, oldPropertyEditor->columnWidth(i));
    }

    // Set our retrieved property editor as our current widget, but only if the
    // corresponding graph panel has been been made active

    if (pActive)
        setCurrentWidget(mPropertyEditor);

    // Update our graphs information
    // Note: this is in case the user changed the locale and then switched to a
    //       different graph panel...

    updateAllGraphsInfo();
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::finalize(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel)
{
    // Remove track of the link betwen our graph panel and our property editor

    mGraphPanels.remove(mPropertyEditors.value(pGraphPanel));
    mPropertyEditors.remove(pGraphPanel);
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::addGraph(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                                                               OpenCOR::GraphPanelWidget::GraphPanelPlotGraph *pGraph)
{
    // Make sure that we have a property editor

    Core::PropertyEditorWidget *propertyEditor = mPropertyEditors.value(pGraphPanel);

    if (!propertyEditor)
        return;

    // Create a section for our newly added graph

    Core::Property *graphProperty = propertyEditor->addSectionProperty();

    graphProperty->setCheckable(true);
    graphProperty->setChecked(true);

    // Keep track of the link between our given graph and our graph property

    mGraphs.insert(graphProperty, pGraph);
    mGraphProperties.insert(pGraph, graphProperty);

    // Create some properties for our graph
    // Note: to add properties will result in the propertyChanged() signal being
    //       emitted, but we don't want to handle that signal (at least, not
    //       when creating a graph since not everyting may be set yet so this
    //       might cause more problems than anything), so we must disconnect
    //       ourselves from it before adding the properties (and then reconnect
    //       ourselves to it once we are done)...

    disconnect(propertyEditor, SIGNAL(propertyChanged(Core::Property *)),
               this, SLOT(graphChanged(Core::Property *)));

    propertyEditor->addListProperty(graphProperty);
    propertyEditor->addStringProperty(pGraph->parameterX()?
                                          static_cast<CellMLSupport::CellmlFileRuntimeParameter *>(pGraph->parameterX())->fullyFormattedName():
                                          Core::UnknownValue,
                                      graphProperty);
    propertyEditor->addStringProperty(pGraph->parameterY()?
                                          static_cast<CellMLSupport::CellmlFileRuntimeParameter *>(pGraph->parameterY())->fullyFormattedName():
                                          Core::UnknownValue,
                                      graphProperty);

    connect(propertyEditor, SIGNAL(propertyChanged(Core::Property *)),
            this, SLOT(graphChanged(Core::Property *)));

    // Update the information about our new graph

    updateGraphsInfo(graphProperty);
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::removeGraphs(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                                                                   const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphs &pGraphs)
{
    // Make sure that we have a property editor

    Core::PropertyEditorWidget *propertyEditor = mPropertyEditors.value(pGraphPanel);

    if (!propertyEditor)
        return;

    // Remove the graph properties associated with the given graphs, as well as
    // their trace

    foreach (GraphPanelWidget::GraphPanelPlotGraph *graph, pGraphs) {
        Core::Property *property = mGraphProperties.value(graph);

        propertyEditor->removeProperty(property);

        mGraphs.remove(property);
        mGraphProperties.remove(graph);
    }
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::addGraph()
{
    // Ask the graph panel associated with our current property editor to add an
    // 'empty' graph

    mGraphPanels.value(mPropertyEditor)->addGraph(new GraphPanelWidget::GraphPanelPlotGraph());
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::removeCurrentGraph()
{
    // Ask the graph panel associated with our current property editor to remove
    // the current graph

    mGraphPanels.value(mPropertyEditor)->removeGraphs(GraphPanelWidget::GraphPanelPlotGraphs() << mGraphs.value(mPropertyEditor->currentProperty()));
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::removeAllGraphs()
{
    // Ask the graph panel associated with our current property editor to remove
    // all the graphs

    mGraphPanels.value(mPropertyEditor)->removeGraphs(mGraphs.values());
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::selectAllGraphs(const bool &pSelect)
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

    disconnect(mPropertyEditor, SIGNAL(propertyChanged(Core::Property *)),
               this, SLOT(graphChanged(Core::Property *)));

    foreach (Core::Property *property, mGraphProperties)
        property->setChecked(pSelect);

    foreach (GraphPanelWidget::GraphPanelPlotGraph *graph, mGraphs)
        graph->setSelected(pSelect);

    if (mGraphs.count()) {
        emit graphsUpdated(qobject_cast<GraphPanelWidget::GraphPanelPlotWidget *>(mGraphs.values().first()->plot()),
                           mGraphs.values());
    }

    connect(mPropertyEditor, SIGNAL(propertyChanged(Core::Property *)),
            this, SLOT(graphChanged(Core::Property *)));
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::selectAllGraphs()
{
    // Select all the graphs

    selectAllGraphs(true);
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::unselectAllGraphs()
{
    // Unselect all the graphs

    selectAllGraphs(false);
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::updateGui()
{
    // Update our graphs information
    // Note: this is useful, for example, if we created a graph for a file that
    //       has not yet been selected, in which case the graph will initially
    //       be invalid, but it should become valid after we have switched to
    //       that file and back...

    updateAllGraphsInfo();
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::finishEditing()
{
    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Ask our property editor to finish editing

    mPropertyEditor->finishEditing();
}

//==============================================================================

static const auto PropertySeparator = QStringLiteral(" | ");

//==============================================================================

Core::Properties SimulationExperimentViewInformationGraphsWidget::graphProperties(GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                                                                                  const QString &pFileName) const
{
    // Retrieve and return all the graph properties associated with the given
    // graph and file name, if any

    Core::Properties res = Core::Properties();
    Core::PropertyEditorWidget *propertyEditor = mPropertyEditors.value(pGraphPanel);

    if (propertyEditor) {
        foreach (Core::Property *property, propertyEditor->properties()) {
            // The property should be returned if it is a section (i.e. a graph
            // property), is checked (i.e. a selected graph) and have its first
            // sub-property (i.e. to which model the graph applies) has either a
            // value of "Current" or that of the given file name

            if (   (property->type() == Core::Property::Section)
                && property->isChecked()) {
                QString modelPropertyValue = property->properties().first()->value();

                if (   !modelPropertyValue.compare(tr("Current"))
                    || !modelPropertyValue.split(PropertySeparator).last().compare(pFileName)) {
                    res << property;
                }
            }
        }
    }

    return res;
}

//==============================================================================

int SimulationExperimentViewInformationGraphsWidget::headerCount() const
{
    // Return the number of headers in our property editors

    return mPropertyEditor?mPropertyEditor->header()->count():0;
}

//==============================================================================

int SimulationExperimentViewInformationGraphsWidget::columnWidth(const int &pIndex) const
{
    // Return the width of the given column

    return mPropertyEditor?mPropertyEditor->columnWidth(pIndex):0;
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::setColumnWidth(const int &pIndex,
                                                                     const int &pColumnWidth)
{
    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Return the width of the given column

    mPropertyEditor->setColumnWidth(pIndex, pColumnWidth);
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::propertyEditorContextMenu(const QPoint &pPosition) const
{
    Q_UNUSED(pPosition);

    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Retrieve our current property, if any

    Core::Property *crtProperty = mPropertyEditor->currentProperty();

    // Update the enabled state of some of our actions

    mRemoveCurrentGraphAction->setEnabled(crtProperty);
    mRemoveAllGraphsAction->setEnabled(!mPropertyEditor->properties().isEmpty());

    bool canSelectAllGraphs = false;
    bool canUnselectAllGraphs = false;

    foreach (Core::Property *property, mGraphProperties) {
        bool graphSelected = property->isChecked();

        canSelectAllGraphs = canSelectAllGraphs || !graphSelected;
        canUnselectAllGraphs = canUnselectAllGraphs || graphSelected;
    }

    mSelectAllGraphsAction->setEnabled(canSelectAllGraphs);
    mUnselectAllGraphsAction->setEnabled(canUnselectAllGraphs);

    // Show the context menu, or not, depending on the type of property we are
    // dealing with, if any

    if (   !crtProperty
        || (crtProperty->type() == Core::Property::Section)
        || (!crtProperty->name().compare(tr("Model")))) {
        mContextMenu->exec(QCursor::pos());
    } else {
        mParametersContextMenu->exec(QCursor::pos());
    }
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::propertyEditorHorizontalScrollBarValueChanged(const int &pValue)
{
    // Keep track of the property editor's horizontal scroll bar value

    mHorizontalScrollBarValue = pValue;
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::propertyEditorSectionResized(const int &pIndex,
                                                                                   const int &pOldSize,
                                                                                   const int &pNewSize)
{
    Q_UNUSED(pOldSize);

    // Let people know that a header section has been resized

    emit headerSectionResized(pIndex, pOldSize, pNewSize);
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::populateParametersContextMenu(CellMLSupport::CellmlFileRuntime *pRuntime)
{
    // Populate our parameters context menu with the contents of our main
    // context menu

    mParametersContextMenu->addActions(mContextMenu->actions());
    mParametersContextMenu->addSeparator();

    // Now, add our model parameters to it

    QString componentHierarchy = QString();
    QMenu *componentMenu = 0;

    foreach (CellMLSupport::CellmlFileRuntimeParameter *parameter, pRuntime->parameters()) {
        // Check whether the current parameter is in the same component
        // hierarchy as the previous one

        QString crtComponentHierarchy = parameter->formattedComponentHierarchy();

        if (crtComponentHierarchy.compare(componentHierarchy)) {
            // The current parameter is in a different component hierarchy, so
            // create a new menu hierarchy for our 'new' component, reusing
            // existing menus, whenever possible

            QMenu *parentComponentMenu = mParametersContextMenu;

            foreach (const QString &component, parameter->componentHierarchy()) {
                // Check whether we already have a menu for our current
                // component

                componentMenu = 0;

                foreach (QObject *object, parentComponentMenu->children()) {
                    QMenu *subMenu = qobject_cast<QMenu *>(object);

                    if (subMenu && !subMenu->menuAction()->text().compare(component)) {
                        componentMenu = subMenu;

                        break;
                    }
                }

                // Create a new menu for our current component, if none could be
                // found

                if (!componentMenu) {
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

        if (!componentMenu)
            continue;

        // Add the current parameter to the 'current' component menu

        QAction *parameterAction = componentMenu->addAction(parameter->icon(),
                                                            parameter->formattedName());

        // Create a connection to handle the parameter value update

        connect(parameterAction, SIGNAL(triggered(bool)),
                this, SLOT(updateParameterValue()));

        // Keep track of the parameter associated with our model parameter
        // action

        mParameterActions.insert(parameterAction, parameter);
    }
}

//==============================================================================

bool SimulationExperimentViewInformationGraphsWidget::checkParameter(CellMLSupport::CellmlFileRuntime *pRuntime,
                                                                     GraphPanelWidget::GraphPanelPlotGraph *pGraph,
                                                                     Core::Property *pParameterProperty,
                                                                     const bool &pParameterX) const
{
    // Check that the information held by the given property corresponds to
    // an existing parameter in our runtime

    CellMLSupport::CellmlFileRuntimeParameter *res = 0;

    if (pRuntime) {
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

        foreach (CellMLSupport::CellmlFileRuntimeParameter *parameter, pRuntime->parameters()) {
            if (   (parameter->componentHierarchy() == componentHierarchy)
                && !parameter->name().compare(parameterName)
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

    pParameterProperty->setIcon(res?BlankIcon:WarningIcon);
    pParameterProperty->setExtraInfo(res?
                                         QString():
                                         pRuntime?
                                             tr("does not exist"):
                                             tr("no runtime"));

    // Keep track of the existing parameter, if any, to which our property
    // corresponds

    if (pParameterX)
        pGraph->setParameterX(res);
    else
        pGraph->setParameterY(res);

    return res;
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::updateGraphInfo(Core::Property *pProperty,
                                                                      const QString &pFileName)
{
    // Make sure that we have a property

    if (!pProperty)
        return;

    // Update the graph information by checking the new value of the given
    // section property

    // Update the model property's icon and graph colour, based on the value of
    // the model property, and determine the file name from which we will have
    // to check our X and Y properties

    static const QIcon LockedIcon   = QIcon(":/oxygen/status/object-locked.png");
    static const QIcon UnlockedIcon = QIcon(":/oxygen/status/object-unlocked.png");

    GraphPanelWidget::GraphPanelPlotGraph *graph = mGraphs.value(pProperty);
    QString fileName = mSimulationWidget->fileName();
    QPen oldPen = graph->pen();
    QPen newPen = oldPen;

    if (!pFileName.compare(tr("Current"))) {
        pProperty->properties()[0]->setIcon(UnlockedIcon);

        newPen.setColor(Qt::darkBlue);
    } else {
        pProperty->properties()[0]->setIcon(LockedIcon);

        newPen.setColor(Qt::darkRed);

        fileName = pFileName.split(PropertySeparator).last();
    }

    graph->setPen(newPen);

    // Check that the parameters represented by the value of the X and Y
    // properties exist for the current/selected model
    // Note: we absolutely want to check the parameter (so that an icon can be
    //       assigned to its corresponding property) , hence the order of our &&
    //       assignment...

    bool graphOk = true;
    CellMLSupport::CellmlFileRuntime *runtime = mViewWidget->runtime(fileName);
    CellMLSupport::CellmlFileRuntimeParameter *oldParameterX = static_cast<CellMLSupport::CellmlFileRuntimeParameter *>(graph->parameterX());
    CellMLSupport::CellmlFileRuntimeParameter *oldParameterY = static_cast<CellMLSupport::CellmlFileRuntimeParameter *>(graph->parameterY());

    graphOk = checkParameter(runtime, graph, pProperty->properties()[1], true) && graphOk;
    graphOk = checkParameter(runtime, graph, pProperty->properties()[2], false) && graphOk;

    // Update our section's name, if possible
    // Note: indeed, when populating ourselves, updateGraphInfo() gets called
    //       (through graphChanged()), yet we don't want to (and can't) do what
    //       follows if not all the properties are available...

    pProperty->setName( pProperty->properties()[1]->value()
                       +PropertySeparator
                       +pProperty->properties()[2]->value());

    // Update the status (i.e. icon) of our (section) property

    static const QIcon BlankIcon   = QIcon(":/SimulationExperimentView/blank.png");
    static const QIcon WarningIcon = QIcon(":/oxygen/status/task-attention.png");

    pProperty->setIcon(graphOk?BlankIcon:WarningIcon);

    // Update the file name with which the graph is associated

    graph->setFileName(fileName);

    // Let people know if we consider that the graph has been updated

    if (   (oldParameterX != graph->parameterX())
        || (oldParameterY != graph->parameterY())
        || (oldPen != newPen)) {
        emit graphsUpdated(qobject_cast<GraphPanelWidget::GraphPanelPlotWidget *>(graph->plot()),
                           GraphPanelWidget::GraphPanelPlotGraphs() << graph);
    }
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::graphChanged(Core::Property *pProperty)
{
    // The graph has changed, which means that either it has been un/selected or
    // that the value of its model, X or Y parameter property has changed

    if (pProperty->type() == Core::Property::Section) {
        // The property associated with the graph is a section, which means that
        // the graph has been un/selected, so update its selected state and let
        // people know that our graph has been updated

        GraphPanelWidget::GraphPanelPlotGraph *graph = mGraphs.value(pProperty);

        if (graph) {
            graph->setSelected(pProperty->isChecked());

            emit graphsUpdated(qobject_cast<GraphPanelWidget::GraphPanelPlotWidget *>(graph->plot()),
                               GraphPanelWidget::GraphPanelPlotGraphs() << graph);
        }
    } else {
        // Either the model, X or Y parameter property of the graph has changed,
        // so update its information
        // Note: updateGraphInfo() will emit the graphsUpdated() signal, if
        //       needed...

        updateGraphInfo(pProperty->parentProperty(),
                        pProperty->parentProperty()->properties()[0]->value());
    }
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::updateGraphsInfo(Core::Property *pSectionProperty)
{
    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Make sure that no editing is in progress

    finishEditing();

    // Use the given section property or retrieve the ones for our current
    // property editor

    QList<Core::Property *> graphProperties = QList<Core::Property *>();

    if (pSectionProperty) {
        graphProperties << pSectionProperty;
    } else {
        foreach (Core::Property *property, mPropertyEditor->properties()) {
            if (property->type() == Core::Property::Section)
                graphProperties << property;
        }
    }

    // Determine the model list values

    QStringList modelListValues = QStringList();

    foreach (const QString &fileName, mViewWidget->fileNames()) {
        Core::File *file = Core::FileManager::instance()->file(fileName);
        QString fileNameOrUrl = file->isLocal()?fileName:file->url();

        modelListValues <<  QFileInfo(fileNameOrUrl).fileName()
                           +PropertySeparator
                           +fileNameOrUrl;
    }

    modelListValues.sort();

    modelListValues.prepend(QString());
    modelListValues.prepend(tr("Current"));

    // Go through our graph properties and update (incl. retranslate) their
    // information

    foreach (Core::Property *graphProperty, graphProperties) {
        graphProperty->properties()[0]->setName(tr("Model"));
        graphProperty->properties()[1]->setName(tr("X"));
        graphProperty->properties()[2]->setName(tr("Y"));

        // Keep track of the current model value, so that we can safely update
        // its list values and then select the correct model value back

        QString oldModelValue = graphProperty->properties()[0]->value();
        QString newModelValue = oldModelValue;

        graphProperty->properties()[0]->setListValues(modelListValues, false);
        // Note: we don't want setListValues() to emit a signal since one will
        //       be emitted as a result of our call to setValue() below...

        if (newModelValue.isEmpty()) {
            // newModelValue is empty, which means that this is the first time
            // we come here, so update our graph info using the current value of
            // our model property

            newModelValue = graphProperty->properties()[0]->value();
        } else if (!modelListValues.contains(oldModelValue)) {
            // Our old model value is not in our new model list values, which
            // means that either the value of the model property was "Current"
            // (and the locale got changed) or the current file got renamed, so
            // we use that instead

            if (oldModelValue.contains(PropertySeparator)) {
                // The current file got renamed

                newModelValue = QFileInfo(mSimulationWidget->fileName()).fileName()+PropertySeparator+mSimulationWidget->fileName();
            } else {
                // The value of the model property was "Current" (and the locale
                // got changed)

                newModelValue = tr("Current");
            }
        }

        // Set the value of our model property to newModelValue (which will
        // result in updateGraphInfo() being called, so we are all fine)

        graphProperty->properties()[0]->setValue(newModelValue, true);
        // Note: we must force the setting of the value since it may very well
        //       be that it's the same as before, yet we want the signal
        //       associated with setValue() to be emitted (so that
        //       updateGraphInfo() can then be called)...
    }
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::updateAllGraphsInfo()
{
    // Update the information about our graphs properties and this for all our
    // property editors, so first keep track of our active property editor

    Core::PropertyEditorWidget *oldPropertyEditor = mPropertyEditor;

    // Go through our property editors and update the information about the
    // graph properties they hold

    foreach (Core::PropertyEditorWidget *propertyEditor, mPropertyEditors) {
        mPropertyEditor = propertyEditor;

        updateGraphsInfo();
    }

    // Retrieve our originally active property editor

    mPropertyEditor = oldPropertyEditor;
}

//==============================================================================

void SimulationExperimentViewInformationGraphsWidget::updateParameterValue()
{
    // Update the current property's value

    mPropertyEditor->currentProperty()->setValue(mParameterActions.value(qobject_cast<QAction *>(sender()))->fullyFormattedName());
}

//==============================================================================

}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
