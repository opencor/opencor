//==============================================================================
// Single cell view information graphs widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "propertyeditorwidget.h"
#include "singlecellviewgraphpanelplotwidget.h"
#include "singlecellviewinformationgraphswidget.h"
#include "singlecellviewwidget.h"

//==============================================================================

#include <QFileInfo>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

SingleCellViewInformationGraphsWidget::SingleCellViewInformationGraphsWidget(QWidget *pParent) :
    QStackedWidget(pParent),
    mPropertyEditors(QMap<SingleCellViewGraphPanelWidget *, Core::PropertyEditorWidget *>()),
    mPropertyEditor(0),
    mContextMenus(QMap<QString, QMenu *>()),
    mParameterActions(QMap<QAction *, CellMLSupport::CellmlFileRuntimeParameter *>()),
    mColumnWidths(QList<int>()),
    mFileNames(QStringList()),
    mFileName(QString()),
    mRuntimes(QMap<QString, CellMLSupport::CellmlFileRuntime *>()),
    mSimulations(QMap<QString, SingleCellViewSimulation *>())
{
    // Create a widget that will be shown whenever there are no graphs
    // associated with the current plotting area

    mNoGraphsMessageWidget = new QLabel(pParent);

    mNoGraphsMessageWidget->setAlignment(Qt::AlignCenter);
    mNoGraphsMessageWidget->setAutoFillBackground(true);
    mNoGraphsMessageWidget->setBackgroundRole(QPalette::Base);
    mNoGraphsMessageWidget->setWordWrap(true);

    addWidget(mNoGraphsMessageWidget);

    // Determine the default width of each column of our property editors

    Core::PropertyEditorWidget *tempPropertyEditor = new Core::PropertyEditorWidget(this);

    for (int i = 0, iMax = tempPropertyEditor->header()->count(); i < iMax; ++i)
        mColumnWidths.append(tempPropertyEditor->columnWidth(i));

    delete tempPropertyEditor;
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::retranslateUi()
{
    // Retranslate our no graphs message widget

    mNoGraphsMessageWidget->setText(tr("There are no graphs..."));

    // Retranslate all our property editors

    foreach (Core::PropertyEditorWidget *propertyEditor, mPropertyEditors)
        propertyEditor->retranslateUi();

    // Retranslate the information about our graphs properties

    updateGraphsInfo();
}

//==============================================================================

static const QString SettingsColumnWidth = "ColumnWidth%1";

//==============================================================================

void SingleCellViewInformationGraphsWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the width of each column of our property editors

    for (int i = 0, iMax = mColumnWidths.size(); i < iMax; ++i)
        mColumnWidths[i] = pSettings->value(SettingsColumnWidth.arg(i),
                                            mColumnWidths.at(i)).toInt();

}

//==============================================================================

void SingleCellViewInformationGraphsWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the width of each column of our current property editor

    for (int i = 0, iMax = mColumnWidths.size(); i < iMax; ++i)
        pSettings->setValue(SettingsColumnWidth.arg(i), mColumnWidths.at(i));
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

    // Update the information about our graphs properties

    updateGraphsInfo();
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::finalize(const QString &pFileName)
{
    // Remove track of the context menu, file name, runtime and simulation

    mContextMenus.remove(pFileName);

    mFileNames.removeOne(pFileName);

    mRuntimes.remove(pFileName);
    mSimulations.remove(pFileName);

    // Update the information about our graphs properties

    updateGraphsInfo();
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::fileOpened(const QString &pFileName)
{
    // Keep track of the file name

    if (!mFileNames.contains(pFileName))
        mFileNames << pFileName;

    mFileNames.sort();

    // Update the information about our graphs properties

    updateGraphsInfo();
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
            mPropertyEditor->setColumnWidth(i, mColumnWidths.at(i));

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
                this, SLOT(propertyChanged(Core::Property *)));

        // Add our new property editor to ourselves

        addWidget(mPropertyEditor);

        // Keep track of our new property editor

        mPropertyEditors.insert(pGraphPanel, mPropertyEditor);
    }

    // Set our retrieved property editor as our current widget, but only if it
    // isn't empty

    if (mPropertyEditor->properties().count())
        setCurrentWidget(mPropertyEditor);
    else
        setCurrentWidget(mNoGraphsMessageWidget);

    // Update the information about our graphs properties
    // Note: this is in case the user changed the locale and then switched to a
    //       different graph panel...

    updateGraphsInfo();
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::finalize(SingleCellViewGraphPanelWidget *pGraphPanel)
{
    // Remove track of our property editor

    mPropertyEditors.remove(pGraphPanel);
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::addGraph(SingleCellViewGraphPanelPlotGraph *pGraph)
{
    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Prevent ourselves from being updated (to avoid any flickering)

    mPropertyEditor->setUpdatesEnabled(false);

    // Create a section for our newly added graph

    Core::Property *sectionProperty = mPropertyEditor->addSectionProperty();

    sectionProperty->setCheckable(true);
    sectionProperty->setChecked(true);

    // Create some properties for our graph

    mPropertyEditor->addListProperty(sectionProperty);
    Core::Property *xProperty = mPropertyEditor->addStringProperty(pGraph->parameterX()->fullyFormattedName(), sectionProperty);
    Core::Property *yProperty = mPropertyEditor->addStringProperty(pGraph->parameterY()->fullyFormattedName(), sectionProperty);

    xProperty->setEditable(true);
    yProperty->setEditable(true);

    // Update the information about our new graph

    updateGraphsInfo(sectionProperty);

    // Allow ourselves to be updated again

    mPropertyEditor->setUpdatesEnabled(true);

    // Make sure that our property editor is our current widget

    setCurrentWidget(mPropertyEditor);
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::removeGraph(SingleCellViewGraphPanelPlotGraph *pGraph)
{
//---GRY--- TO BE DONE...
Q_UNUSED(pGraph);
    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Prevent ourselves from being updated (to avoid any flickering)

    mPropertyEditor->setUpdatesEnabled(false);

    // Allow ourselves to be updated again

    mPropertyEditor->setUpdatesEnabled(true);

    // Hide our property editor if it doesn't contain any graph anymore

    if (!mPropertyEditor->properties().count())
        setCurrentWidget(mNoGraphsMessageWidget);
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::finishPropertyEditing()
{
    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Finish the editing of our property editor

    mPropertyEditor->finishPropertyEditing();
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::propertyEditorContextMenu(const QPoint &pPosition) const
{
    Q_UNUSED(pPosition);

    // Create a custom context menu for our property editor, based on what is
    // underneath our mouse pointer

    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Retrieve our current property, if any

    Core::Property *currentProperty = mPropertyEditor->currentProperty();

    if (!currentProperty)
        return;

    // Make sure that our current property is not a section or the model
    // property

    if (   (currentProperty->type() == Core::Property::Section)
        || (!currentProperty->name().compare(tr("Model"))))
        return;

    // Generate and show the context menu

    mContextMenus.value(mFileName)->exec(QCursor::pos());
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
    // Populate our property editor with the parameters

    QMenu *componentMenu = 0;

    foreach (CellMLSupport::CellmlFileRuntimeParameter *parameter, pRuntime->parameters()) {
        // Check whether the current parameter is in the same component as the
        // previous one

        QString currentComponent = parameter->component();

        if (   !componentMenu
            ||  currentComponent.compare(componentMenu->menuAction()->text())) {
            // The current parameter is in a different component, so create a
            // new menu for the 'new' component

            componentMenu = new QMenu(currentComponent, pContextMenu);

            pContextMenu->addMenu(componentMenu);
        }

        // Add the current parameter to the 'current' component menu

        QAction *parameterAction = componentMenu->addAction(SingleCellViewWidget::parameterIcon(parameter->type()),
                                                            parameter->formattedName());

        // Create a connection to handle the parameter value update

        connect(parameterAction, SIGNAL(triggered()),
                this, SLOT(updateParameterValue()));

        // Keep track of the parameter associated with our model parameter
        // action

        mParameterActions.insert(parameterAction, parameter);
    }
}

//==============================================================================

bool SingleCellViewInformationGraphsWidget::checkParameter(const QString &pFileName,
                                                           Core::Property *pProperty) const
{
    bool res = false;

    // Retrieve the runtime associated with the given file name

    CellMLSupport::CellmlFileRuntime *runtime = mRuntimes.value(pFileName);

    // Check that the information held by the given property corresponds to
    // an existing parameter in our runtime

    if (runtime) {
        // Retrieve the component and parameter of the property

        QStringList info = pProperty->value().split(".");
        QString componentName = info.first();
        QString parameterName = info.last();
        int parameterDegree = parameterName.size();

        // Determine the degree of our parameter, if any

        parameterName.replace("'", QString());

        parameterDegree -= parameterName.size();

        // Check whether we can find our property among our runtime's parameters

        foreach (CellMLSupport::CellmlFileRuntimeParameter *parameter, runtime->parameters())
            if (   !parameter->component().compare(componentName)
                && !parameter->name().compare(parameterName)
                && (parameter->degree() == parameterDegree)) {
                res = true;

                break;
            }
    }

    pProperty->setIcon(res?
                           QIcon(":Core_blankIcon"):
                           QIcon(":/oxygen/status/task-attention.png"));
    pProperty->setExtraInfo(res?
                                QString():
                                runtime?
                                    tr("does not exist"):
                                    tr("no runtime"));

    return res;
}

//==============================================================================

static const QString PropertySeparator = " | ";

void SingleCellViewInformationGraphsWidget::updateGraphInfo(Core::Property *pProperty,
                                                            const QString &pFileName) const
{
    // Update the graph information by checking the new value of the given
    // section property

    // Update the model property's icon based on its value and determine the
    // file name from which we will have to check our X and Y properties

    QString fileName = mFileName;

    if (!pFileName.compare(tr("Current"))) {
        pProperty->properties()[0]->setIcon(QIcon(":/oxygen/status/object-unlocked.png"));
    } else {
        pProperty->properties()[0]->setIcon(QIcon(":/oxygen/status/object-locked.png"));

        fileName = pFileName.split(PropertySeparator).last();
    }

    // Check that the parameters represented by the value of the X and Y
    // properties exist for the current/selected model
    // Note: we absolutely want to check the parameter (so that an icon can be
    //       assigned to its corresponding property) , hence the order of our &&
    //       assignment...

    bool graphOk = true;

    if (pProperty->properties().count() >= 2)
        graphOk = checkParameter(fileName, pProperty->properties()[1]) && graphOk;

    if (pProperty->properties().count() == 3)
        graphOk = checkParameter(fileName, pProperty->properties()[2]) && graphOk;

    // Update our section's name, if possible
    // Note: indeed, when populating ourselves, updateGraphInfo() gets called
    //       (through propertyChanged()), yet we don't want to (and can't) do
    //       what follows if not all the properties are available...

    if (pProperty->properties().count() == 3)
        pProperty->setName( pProperty->properties()[1]->value()
                           +PropertySeparator
                           +pProperty->properties()[2]->value());

    // Update the status (i.e. icon) of our (section) property

    pProperty->setIcon(graphOk?
                           QIcon(":Core_blankIcon"):
                           QIcon(":/oxygen/status/task-attention.png"));
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

void SingleCellViewInformationGraphsWidget::propertyChanged(Core::Property *pProperty)
{
    // Update the graph information associated with the given property's
    // corresponding section property

    if (pProperty->value().isEmpty())
        // The property value is empty, so...

        pProperty->setValue("???");
    else
        updateGraphInfo(pProperty->parentProperty(),
                        pProperty->parentProperty()->properties()[0]->value());
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::updateGraphsInfo(Core::Property *pSectionProperty)
{
    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Make sure that no editing is in progress

    finishPropertyEditing();

    // Use the given section property or retrieve the ones for our current
    // property editor

    QList<Core::Property *> sectionProperties = QList<Core::Property *>();

    if (pSectionProperty)
        sectionProperties << pSectionProperty;
    else
        foreach (Core::Property *property, mPropertyEditor->properties())
            if (property->type() == Core::Property::Section)
                sectionProperties << property;

    // Determine the model list value

    QStringList modelListValue = QStringList();

    foreach (const QString &fileName, mFileNames)
        modelListValue << QFileInfo(fileName).fileName()+PropertySeparator+fileName;

    modelListValue.sort();

    modelListValue.prepend(QString());
    modelListValue.prepend(tr("Current"));

    // Go through our section properties and update (incl. retranslate) their
    // information

    foreach (Core::Property *sectionProperty, sectionProperties) {
        sectionProperty->properties()[0]->setName(tr("Model"));
        sectionProperty->properties()[1]->setName(tr("X"));
        sectionProperty->properties()[2]->setName(tr("Y"));

        // Keep track of the current model value, so that we can safely update
        // its list value and then select the correct model value back

        QString oldModelValue = sectionProperty->properties()[0]->value();
        QString newModelValue = oldModelValue;

        sectionProperty->properties()[0]->setListValue(modelListValue);

        if (!modelListValue.contains(oldModelValue)) {
            // Our old model value is not in our new model list value, which
            // means that either the value of the model property was "Current"
            // (and the locale got changed) or the current file got renamed, so
            // we use that instead

            if (oldModelValue.contains(PropertySeparator))
                // The current file got renamed

                newModelValue = QFileInfo(mFileName).fileName()+PropertySeparator+mFileName;
            else
                // The value of the model property was "Current"

                newModelValue = tr("Current");
        }

        // Check whether newModelValue is empty (which is the case when we first
        // come here) and, if so, update our graph info using the current value
        // of our model property (which will have been set through our call to
        // Property::setListValue() above), otherwise set the value of our model
        // property to newModelValue (which will result in updateGraphInfo()
        // being called, so we are all fine)

        if (newModelValue.isEmpty())
            updateGraphInfo(sectionProperty, sectionProperty->properties()[0]->value());
        else
            sectionProperty->properties()[0]->setValue(newModelValue, true);
            // Note: we must force the setting of the value since it may very
            //       well be that it's the same as before, yet we want the
            //       signal associated with setValue() to be emitted (so that
            //       updateGraphInfo() can then be called)...
    }
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
