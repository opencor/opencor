/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Single cell view information graphs widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "propertyeditorwidget.h"
#include "singlecellviewgraphpanelplotwidget.h"
#include "singlecellviewgraphpanelwidget.h"
#include "singlecellviewinformationgraphswidget.h"
#include "singlecellviewwidget.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QAction>
#include <QFileInfo>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QMetaType>
#include <QSettings>

//==============================================================================

#include "ui_singlecellviewinformationgraphswidget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

SingleCellViewInformationGraphsWidget::SingleCellViewInformationGraphsWidget(QWidget *pParent) :
    QStackedWidget(pParent),
    Core::CommonWidget(pParent),
    mGui(new Ui::SingleCellViewInformationGraphsWidget),
    mGraphPanels(QMap<Core::PropertyEditorWidget *, SingleCellViewGraphPanelWidget *>()),
    mPropertyEditors(QMap<SingleCellViewGraphPanelWidget *, Core::PropertyEditorWidget *>()),
    mPropertyEditor(0),
    mGraphs(QMap<Core::Property *, SingleCellViewGraphPanelPlotGraph *>()),
    mGraphProperties(QMap<SingleCellViewGraphPanelPlotGraph *, Core::Property *>()),
    mContextMenus(QMap<QString, QMenu *>()),
    mParameterActions(QMap<QAction *, CellMLSupport::CellmlFileRuntimeParameter *>()),
    mColumnWidths(QIntList()),
    mFileNames(QStringList()),
    mFileName(QString()),
    mRuntimes(QMap<QString, CellMLSupport::CellmlFileRuntime *>()),
    mSimulations(QMap<QString, SingleCellViewSimulation *>()),
    mGraphPropertiesSelected(QMap<QString, QMap<Core::Property *, bool>>()),
    mCanEmitGraphsUpdatedSignal(true)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Determine the default width of each column of our property editors

    Core::PropertyEditorWidget *tempPropertyEditor = new Core::PropertyEditorWidget(this);

    for (int i = 0, iMax = tempPropertyEditor->header()->count(); i < iMax; ++i)
        mColumnWidths.append(tempPropertyEditor->columnWidth(i));

    delete tempPropertyEditor;

    // Create our context menu

    mContextMenu = new QMenu(this);

    mContextMenu->addAction(mGui->actionAddGraph);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mGui->actionRemoveCurrentGraph);
    mContextMenu->addAction(mGui->actionRemoveAllGraphs);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mGui->actionSelectAllGraphs);
    mContextMenu->addAction(mGui->actionUnselectAllGraphs);
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

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

static const auto SettingsColumnWidths = QStringLiteral("ColumnWidths");

//==============================================================================

void SingleCellViewInformationGraphsWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the width of each column of our property editors

    mColumnWidths = qVariantListToIntList(pSettings->value(SettingsColumnWidths, qIntListToVariantList(mColumnWidths)).toList());
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the width of each column of our current property editor

    pSettings->setValue(SettingsColumnWidths, qIntListToVariantList(mColumnWidths));
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::initialize(const QString &pFileName,
                                                       CellMLSupport::CellmlFileRuntime *pRuntime,
                                                       SingleCellViewSimulation *pSimulation)
{
    // Keep track of the context menu, file name, runtime and simulation

    mFileName = pFileName;

    mRuntimes.insert(pFileName, pRuntime);
    mSimulations.insert(pFileName, pSimulation);

    // Create and populate our context menu

    QMenu *contextMenu = mContextMenus.value(pFileName);

    if (!contextMenu) {
        QMenu *contextMenu = new QMenu(this);

        populateContextMenu(contextMenu, pRuntime);

        // Keep track of our new context menu

        mContextMenus.insert(pFileName, contextMenu);
    }

    // Update the information about our graphs properties and this for all our
    // property editors

    updateAllGraphsInfo(true);

    // Specify which graphs should be selected

    QMap<Core::Property *, bool> graphsPropertiesSelected = mGraphPropertiesSelected.value(pFileName);

    foreach (Core::Property *graphProperty, mGraphProperties)
        graphProperty->setChecked(graphsPropertiesSelected.value(graphProperty, true));
        // Note: by default, we want graphs to be selected, hence our use of
        //       true as the default value...
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::backup(const QString &pFileName)
{
    // Keep track of which graphs are selected

    QMap<Core::Property *, bool> graphsPropertiesSelected = QMap<Core::Property *, bool>();

    foreach (Core::Property *graphProperty, mGraphProperties)
        graphsPropertiesSelected.insert(graphProperty, graphProperty->isChecked());

    mGraphPropertiesSelected.insert(pFileName, graphsPropertiesSelected);
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::finalize(const QString &pFileName)
{
    // Remove track of various information

    mContextMenus.remove(pFileName);

    mFileNames.removeOne(pFileName);

    mRuntimes.remove(pFileName);
    mSimulations.remove(pFileName);

    mGraphPropertiesSelected.remove(pFileName);
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::fileOpened(const QString &pFileName)
{
    // Keep track of the file name

    mFileNames << pFileName;

    updateAllGraphsInfo(true);
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::fileRenamed(const QString &pOldFileName,
                                                        const QString &pNewFileName)
{
    // Replace the old file name with the new one in our various trackers

    mFileName = pNewFileName;

    mFileNames << pNewFileName;

    mRuntimes.insert(pNewFileName, mRuntimes.value(pOldFileName));
    mSimulations.insert(pNewFileName, mSimulations.value(pOldFileName));

    finalize(pOldFileName);

    updateAllGraphsInfo(true);
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // Update the information about our graphs properties
    // Note: our various trackers (e.g. mFileNames) will have been updated
    //       through finalize(), so we are fine...

    updateAllGraphsInfo(true);
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::initialize(SingleCellViewGraphPanelWidget *pGraphPanel)
{
    // Retrieve the property editor for the given file name or create one, if
    // none exists

    mPropertyEditor = mPropertyEditors.value(pGraphPanel);

    if (!mPropertyEditor) {
        // No property editor exists for the given graph panel, so create one

        mPropertyEditor = new Core::PropertyEditorWidget(false, false, this);

        // Initialise our property editor's columns' width

        for (int i = 0, iMax = mColumnWidths.size(); i < iMax; ++i)
            mPropertyEditor->setColumnWidth(i, mColumnWidths[i]);

        // We want our own context menu for our property editor

        mPropertyEditor->setContextMenuPolicy(Qt::CustomContextMenu);

        connect(mPropertyEditor, SIGNAL(customContextMenuRequested(const QPoint &)),
                this, SLOT(propertyEditorContextMenu(const QPoint &)));

        // Keep track of changes to columns' width

        connect(mPropertyEditor->header(), SIGNAL(sectionResized(int, int, int)),
                this, SLOT(propertyEditorSectionResized(const int &, const int &, const int &)));

        // Keep track of changes to list properties

        connect(mPropertyEditor, SIGNAL(listPropertyChanged(Core::Property *, const QString &)),
                this, SLOT(modelChanged(Core::Property *, const QString &)));

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

    // Set our retrieved property editor as our current widget

    setCurrentWidget(mPropertyEditor);

    // Update the information about our graphs properties and this for all our
    // property editors
    // Note: this is in case the user changed the locale and then switched to a
    //       different graph panel...

    updateAllGraphsInfo();
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::finalize(SingleCellViewGraphPanelWidget *pGraphPanel)
{
    // Remove track of the link betwen our graph panel and our property editor

    mGraphPanels.remove(mPropertyEditors.value(pGraphPanel));
    mPropertyEditors.remove(pGraphPanel);
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::addGraph(SingleCellViewGraphPanelPlotWidget *pPlot,
                                                     SingleCellViewGraphPanelPlotGraph *pGraph)
{
    Q_UNUSED(pPlot);

    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Prevent ourselves from being updated (to avoid flickering)

    mPropertyEditor->setUpdatesEnabled(false);

    // Create a section for our newly added graph

    Core::Property *graphProperty = mPropertyEditor->addSectionProperty();

    graphProperty->setCheckable(true);
    graphProperty->setChecked(true);

    // Keep track of the link between our given graph and our graph property

    mGraphs.insert(graphProperty, pGraph);
    mGraphProperties.insert(pGraph, graphProperty);

    // Create some properties for our graph
    // Note: to add properties will result in some signals being emitted, but we
    //       don't want to handle them (at least, not when creating a graph
    //       since not everyting may be set yet so this might cause more
    //       problems than anything), so we must disconnect ourselves from them,
    //       before adding the properties (and then reconnect ourselves to
    //       them)...

    disconnect(mPropertyEditor, SIGNAL(listPropertyChanged(Core::Property *, const QString &)),
               this, SLOT(modelChanged(Core::Property *, const QString &)));
    disconnect(mPropertyEditor, SIGNAL(propertyChanged(Core::Property *)),
               this, SLOT(graphChanged(Core::Property *)));

    mPropertyEditor->addListProperty(graphProperty);
    Core::Property *xProperty = mPropertyEditor->addStringProperty(pGraph->parameterX()?pGraph->parameterX()->fullyFormattedName():Core::UnknownValue, graphProperty);
    Core::Property *yProperty = mPropertyEditor->addStringProperty(pGraph->parameterY()?pGraph->parameterY()->fullyFormattedName():Core::UnknownValue, graphProperty);

    xProperty->setEditable(true);
    yProperty->setEditable(true);

    connect(mPropertyEditor, SIGNAL(listPropertyChanged(Core::Property *, const QString &)),
            this, SLOT(modelChanged(Core::Property *, const QString &)));
    connect(mPropertyEditor, SIGNAL(propertyChanged(Core::Property *)),
            this, SLOT(graphChanged(Core::Property *)));

    // Update the information about our new graph

    updateGraphsInfo(graphProperty);

    // Allow ourselves to be updated again

    mPropertyEditor->setUpdatesEnabled(true);

    // Make sure that our property editor is our current widget

    setCurrentWidget(mPropertyEditor);
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::removeGraphs(SingleCellViewGraphPanelPlotWidget *pPlot,
                                                         const QList<SingleCellViewGraphPanelPlotGraph *> &pGraphs)
{
    Q_UNUSED(pPlot);

    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Prevent ourselves from being updated (to avoid flickering)

    mPropertyEditor->setUpdatesEnabled(false);

    // Remove the graph properties associated with the given graphs, as well as
    // their trace

    foreach (SingleCellViewGraphPanelPlotGraph *graph, pGraphs) {
        Core::Property *property = mGraphProperties.value(graph);

        mPropertyEditor->removeProperty(property);

        mGraphs.remove(property);
        mGraphProperties.remove(graph);
    }

    // Allow ourselves to be updated again

    mPropertyEditor->setUpdatesEnabled(true);
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::on_actionAddGraph_triggered()
{
    // Ask the graph panel associated with our current property editor to add an
    // 'empty' graph

    mGraphPanels.value(mPropertyEditor)->addGraph(new SingleCellViewGraphPanelPlotGraph());
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::on_actionRemoveCurrentGraph_triggered()
{
    // Ask the graph panel associated with our current property editor to remove
    // the current graph

    mGraphPanels.value(mPropertyEditor)->removeGraphs(QList<SingleCellViewGraphPanelPlotGraph *>() << mGraphs.value(mPropertyEditor->currentProperty()));
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::on_actionRemoveAllGraphs_triggered()
{
    // Ask the graph panel associated with our current property editor to remove
    // all the graphs

    mGraphPanels.value(mPropertyEditor)->removeGraphs(mGraphs.values());
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::selectAllGraphs(const bool &pSelect)
{
    // (Un)select all the graphs
    // Note: normall, we would only update the checked state of our graph
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

    foreach (SingleCellViewGraphPanelPlotGraph *graph, mGraphs)
        graph->setSelected(pSelect);

    if (mGraphs.count())
        emit graphsUpdated(qobject_cast<SingleCellViewGraphPanelPlotWidget *>(mGraphs.values().first()->plot()),
                           mGraphs.values());

    connect(mPropertyEditor, SIGNAL(propertyChanged(Core::Property *)),
            this, SLOT(graphChanged(Core::Property *)));
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::on_actionSelectAllGraphs_triggered()
{
    // Select all the graphs

    selectAllGraphs(true);
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::on_actionUnselectAllGraphs_triggered()
{
    // Unselect all the graphs

    selectAllGraphs(false);
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::finishEditing()
{
    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Ask our property editor to finish editing

    mPropertyEditor->finishEditing();
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::propertyEditorContextMenu(const QPoint &pPosition) const
{
    Q_UNUSED(pPosition);

    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Retrieve our current property, if any

    Core::Property *currentProperty = mPropertyEditor->currentProperty();

    // Update the enabled state of some of our actions

    mGui->actionRemoveCurrentGraph->setEnabled(currentProperty);
    mGui->actionRemoveAllGraphs->setEnabled(!mPropertyEditor->properties().isEmpty());

    bool canSelectAllGraphs = false;
    bool canUnselectAllGraphs = false;

    foreach (Core::Property *property, mGraphProperties) {
        bool graphSelected = property->isChecked();

        canSelectAllGraphs   = canSelectAllGraphs   || !graphSelected;
        canUnselectAllGraphs = canUnselectAllGraphs ||  graphSelected;
    }

    mGui->actionSelectAllGraphs->setEnabled(canSelectAllGraphs);
    mGui->actionUnselectAllGraphs->setEnabled(canUnselectAllGraphs);

    // Show the context menu, or not, depending ont the type of property we are
    // dealing with, if any

    if (   !currentProperty
        || (currentProperty->type() == Core::Property::Section)
        || (!currentProperty->name().compare(tr("Model")))) {
        mContextMenu->exec(QCursor::pos());
    } else {
        mContextMenus.value(mFileName)->exec(QCursor::pos());
    }
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::propertyEditorSectionResized(const int &pLogicalIndex,
                                                                             const int &pOldSize,
                                                                             const int &pNewSize)
{
    Q_UNUSED(pOldSize);

    // Prevent all our property editors from responding to an updating of their
    // columns' width

    foreach (Core::PropertyEditorWidget *propertyEditor, mPropertyEditors)
        disconnect(propertyEditor->header(), SIGNAL(sectionResized(int, int, int)),
                   this, SLOT(propertyEditorSectionResized(const int &, const int &, const int &)));

    // Update the column width of all our property editors

    foreach (Core::PropertyEditorWidget *propertyEditor, mPropertyEditors)
        propertyEditor->header()->resizeSection(pLogicalIndex, pNewSize);

    // Keep track of the new column width

    mColumnWidths[pLogicalIndex] = pNewSize;

    // Re-allow all our property editors to respond to an updating of their
    // columns' width

    foreach (Core::PropertyEditorWidget *propertyEditor, mPropertyEditors)
        connect(propertyEditor->header(), SIGNAL(sectionResized(int, int, int)),
                this, SLOT(propertyEditorSectionResized(const int &, const int &, const int &)));
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::populateContextMenu(QMenu *pContextMenu,
                                                                CellMLSupport::CellmlFileRuntime *pRuntime)
{
    // Populate our context menu with the contents of our context menu

    pContextMenu->addActions(mContextMenu->actions());
    pContextMenu->addSeparator();

    // Now, add our model parameters to it

    QString componentHierarchy = QString();
    QMenu *componentMenu = 0;

    foreach (CellMLSupport::CellmlFileRuntimeParameter *parameter, pRuntime->parameters()) {
        // Check whether the current parameter is in the same component
        // hierarchy as the previous one

        QString currentComponentHierarchy = parameter->formattedComponentHierarchy();

        if (currentComponentHierarchy.compare(componentHierarchy)) {
            // The current parameter is in a different component hierarchy, so
            // create a new menu hierarchy for our 'new' component, reusing
            // existing menus, whenever possible

            QMenu *menu = pContextMenu;

            foreach (const QString &component, parameter->componentHierarchy()) {
                // Check whether we already have a menu for our current
                // component

                componentMenu = 0;

                foreach (QObject *object, menu->children()) {
                    QMenu *subMenu = dynamic_cast<QMenu *>(object);

                    if (    subMenu
                        && !subMenu->menuAction()->text().compare(component)) {
                        componentMenu = subMenu;

                        break;
                    }
                }

                // Create a new menu for our current component, if none could be
                // found

                if (!componentMenu) {
                    componentMenu = new QMenu(component, menu);

                    menu->addMenu(componentMenu);
                }

                // Get ready for the next component in our component hierarchy

                menu = componentMenu;
            }

            // Keep track of the new component hierarchy

            componentHierarchy = currentComponentHierarchy;
        }

        // Make sure that we have a 'current' component menu
        // Note: this should never happen, but we never know...

        if (!componentMenu)
            continue;

        // Add the current parameter to the 'current' component menu

        QAction *parameterAction = componentMenu->addAction(SingleCellViewWidget::parameterIcon(parameter->type()),
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

bool SingleCellViewInformationGraphsWidget::checkParameter(CellMLSupport::CellmlFileRuntime *pRuntime,
                                                           SingleCellViewGraphPanelPlotGraph *pGraph,
                                                           Core::Property *pParameterProperty,
                                                           const bool &pParameterX) const
{
    // Check that the information held by the given property corresponds to
    // an existing parameter in our runtime

    CellMLSupport::CellmlFileRuntimeParameter *res = 0;

    if (pRuntime) {
        // Retrieve the component and parameter of the property

        QStringList info = pParameterProperty->value().split(".");
        QStringList componentHierarchy = info;
        QString parameterName = info.last();
        int parameterDegree = parameterName.size();

        componentHierarchy.removeLast();

        // Determine the degree of our parameter, if any

        parameterName.replace("'", QString());

        parameterDegree -= parameterName.size();

        // Check whether we can find our property among our runtime's parameters

        foreach (CellMLSupport::CellmlFileRuntimeParameter *parameter, pRuntime->parameters())
            if (   (parameter->componentHierarchy() == componentHierarchy)
                && !parameter->name().compare(parameterName)
                && (parameter->degree() == parameterDegree)) {
                res = parameter;

                break;
            }
    }

    // Update our parameter property based on whether it corresponds to an
    // existing parameter in our runtime

    pParameterProperty->setIcon(res?
                                    QIcon(":/blank.png"):
                                    QIcon(":/oxygen/status/task-attention.png"));
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

static const auto PropertySeparator = QStringLiteral(" | ");

//==============================================================================

void SingleCellViewInformationGraphsWidget::updateGraphInfo(Core::Property *pProperty,
                                                            const QString &pFileName)
{
    // Update the graph information by checking the new value of the given
    // section property

    // Update the model property's icon and graph colour, based on the value of
    // the model property, and determine the file name from which we will have
    // to check our X and Y properties

    SingleCellViewGraphPanelPlotGraph *graph = mGraphs.value(pProperty);
    QString fileName = mFileName;
    QPen oldPen = graph->pen();
    QPen newPen = oldPen;

    if (!pFileName.compare(tr("Current"))) {
        pProperty->properties()[0]->setIcon(QIcon(":/oxygen/status/object-unlocked.png"));

        newPen.setColor(Qt::darkBlue);
    } else {
        pProperty->properties()[0]->setIcon(QIcon(":/oxygen/status/object-locked.png"));

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
    CellMLSupport::CellmlFileRuntime *runtime = mRuntimes.value(fileName);
    CellMLSupport::CellmlFileRuntimeParameter *oldParameterX = graph->parameterX();
    CellMLSupport::CellmlFileRuntimeParameter *oldParameterY = graph->parameterY();

    if (pProperty->properties().count() >= 2)
        graphOk = checkParameter(runtime, graph, pProperty->properties()[1], true) && graphOk;

    if (pProperty->properties().count() == 3)
        graphOk = checkParameter(runtime, graph, pProperty->properties()[2], false) && graphOk;

    // Update our section's name, if possible
    // Note: indeed, when populating ourselves, updateGraphInfo() gets called
    //       (through graphChanged()), yet we don't want to (and can't) do what
    //       follows if not all the properties are available...

    if (pProperty->properties().count() == 3)
        pProperty->setName( pProperty->properties()[1]->value()
                           +PropertySeparator
                           +pProperty->properties()[2]->value());

    // Update the status (i.e. icon) of our (section) property

    pProperty->setIcon(graphOk?
                           QIcon(":/blank.png"):
                           QIcon(":/oxygen/status/task-attention.png"));

    // Update the file name with which the graph is associated

    graph->setFileName(fileName);

    // Let people know if we consider that the graph has been updated

    if (   mCanEmitGraphsUpdatedSignal
        && (   (oldParameterX != graph->parameterX())
            || (oldParameterY != graph->parameterY())
            || (oldPen != newPen))) {
        emit graphsUpdated(qobject_cast<SingleCellViewGraphPanelPlotWidget *>(graph->plot()),
                           QList<SingleCellViewGraphPanelPlotGraph *>() << graph);
    }
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::modelChanged(Core::Property *pProperty,
                                                         const QString &pValue)
{
    // Update the graph information associated with the given property's
    // corresponding section property and the given value

    updateGraphInfo(pProperty->parentProperty(), pValue);
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::graphChanged(Core::Property *pProperty)
{
    // The graph has changed, which means that either it has been un/selected or
    // that the value of its model, X or Y parameter property has changed

    if (pProperty->type() == Core::Property::Section) {
        // The property associated with the graph is a section, which means that
        // the graph has been un/selected, so update its selected state and let
        // people know that our graph has been updated

        SingleCellViewGraphPanelPlotGraph *graph = mGraphs.value(pProperty);

        if (graph) {
            graph->setSelected(pProperty->isChecked());

            emit graphsUpdated(qobject_cast<SingleCellViewGraphPanelPlotWidget *>(graph->plot()),
                               QList<SingleCellViewGraphPanelPlotGraph *>() << graph);
        }
    } else {
        // Either the model, X or Y parameter property of the graph has changed,
        // so update its information
        // Note: updateGraphInfo() will emit the graphUpdated() signal, if
        //       needed...

        updateGraphInfo(pProperty->parentProperty(),
                        pProperty->parentProperty()->properties()[0]->value());
    }
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::updateGraphsInfo(Core::Property *pSectionProperty,
                                                             const bool &pGlobalGraphsUpdate)
{
    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Make sure that no editing is in progress

    finishEditing();

    // Use the given section property or retrieve the ones for our current
    // property editor

    QList<Core::Property *> graphProperties = QList<Core::Property *>();

    if (pSectionProperty)
        graphProperties << pSectionProperty;
    else
        foreach (Core::Property *property, mPropertyEditor->properties())
            if (property->type() == Core::Property::Section)
                graphProperties << property;

    // Determine the model list value

    QStringList modelListValue = QStringList();

    foreach (const QString &fileName, mFileNames)
        modelListValue << QFileInfo(fileName).fileName()+PropertySeparator+fileName;

    modelListValue.sort();

    modelListValue.prepend(QString());
    modelListValue.prepend(tr("Current"));

    // Prevent updateGraphInfo(), which is going to be called when updating the
    // information of our graph properies below, from emitting the
    // graphsUpdated() signal, in case we came here with the intention of
    // updating several graphs, thus making it faster to update them
    // Note: indeed, say that you have several graphs, each with several data
    //       points, and then switch to another file and back, then it each
    //       graph will hidden and reshown individually, which can be slow. So,
    //       instead we 'manually' let people know that some graphs have been
    //       updated...

    mCanEmitGraphsUpdatedSignal = !pGlobalGraphsUpdate;

    // Go through our graph properties and update (incl. retranslate) their
    // information

    foreach (Core::Property *graphProperty, graphProperties) {
        graphProperty->properties()[0]->setName(tr("Model"));
        graphProperty->properties()[1]->setName(tr("X"));
        graphProperty->properties()[2]->setName(tr("Y"));

        // Keep track of the current model value, so that we can safely update
        // its list value and then select the correct model value back

        QString oldModelValue = graphProperty->properties()[0]->value();
        QString newModelValue = oldModelValue;

        graphProperty->properties()[0]->setListValue(modelListValue, false);
        // Note: we don't want setListValue() to emit a signal since one will be
        //       emitted as a result of our call to setValue() below...

        if (newModelValue.isEmpty()) {
            // newModelValue is empty, which means that this is the first time
            // we come here, so update our graph info using the current value of
            // our model property

            newModelValue = graphProperty->properties()[0]->value();
        } else if (!modelListValue.contains(oldModelValue)) {
            // Our old model value is not in our new model list value, which
            // means that either the value of the model property was "Current"
            // (and the locale got changed) or the current file got renamed, so
            // we use that instead

            if (oldModelValue.contains(PropertySeparator))
                // The current file got renamed

                newModelValue = QFileInfo(mFileName).fileName()+PropertySeparator+mFileName;
            else
                // The value of the model property was "Current" (and the locale
                // got changed)

                newModelValue = tr("Current");
        }

        // Set the value of our model property to newModelValue (which will
        // result in updateGraphInfo() being called, so we are all fine)

        graphProperty->properties()[0]->setValue(newModelValue, true);
        // Note: we must force the setting of the value since it may very well
        //       be that it's the same as before, yet we want the signal
        //       associated with setValue() to be emitted (so that
        //       updateGraphInfo() can then be called)...
    }

    // Reset mCanEmitGraphsUpdatedSignal and let people know that we have
    // updated some graphs, if needed

    if (pGlobalGraphsUpdate) {
        // Re-allow updateGraphInfo() to emit the graphsUpdated() signal

        mCanEmitGraphsUpdatedSignal = true;

        // Let people know about some graphs having been updaated

        QList<SingleCellViewGraphPanelPlotGraph *> graphs = QList<SingleCellViewGraphPanelPlotGraph *>();

        foreach (Core::Property *graphProperty, graphProperties)
            graphs << mGraphs.value(graphProperty);

        if (graphs.count())
            emit graphsUpdated(qobject_cast<SingleCellViewGraphPanelPlotWidget *>(graphs.first()->plot()),
                               graphs);
    }
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::updateAllGraphsInfo(const bool &pGlobalGraphsUpdate)
{
    // Update the information about our graphs properties and this for all our
    // property editors, so first keep track of our active property editor

    Core::PropertyEditorWidget *oldPropertyEditor = mPropertyEditor;

    // Go through our property editors and update the information about the
    // graph properties they hold

    foreach (Core::PropertyEditorWidget *propertyEditor, mPropertyEditors) {
        mPropertyEditor = propertyEditor;

        updateGraphsInfo(0, pGlobalGraphsUpdate);
    }

    // Retrieve our originally active property editor

    mPropertyEditor = oldPropertyEditor;
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::updateParameterValue()
{
    // Update the current property's value

    mPropertyEditor->currentProperty()->setValue(mParameterActions.value(qobject_cast<QAction *>(sender()))->fullyFormattedName());
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
