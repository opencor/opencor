//==============================================================================
// Single cell view information graphs widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "propertyeditorwidget.h"
#include "singlecellviewgraphpanelplotwidget.h"
#include "singlecellviewinformationgraphswidget.h"

//==============================================================================

#include <QFileInfo>
#include <QHeaderView>
#include <QLabel>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

SingleCellViewInformationGraphsWidget::SingleCellViewInformationGraphsWidget(QWidget *pParent) :
    QStackedWidget(pParent),
    mPropertyEditors(QMap<SingleCellViewGraphPanelWidget *, Core::PropertyEditorWidget *>()),
    mColumnWidths(QList<int>()),
    mPropertyEditor(0),
    mFileName(QString()),
    mFileNames(QStringList()),
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
    // Keep track of the file name, runtime and simulation

    mFileName = pFileName;

    mRuntimes.insert(pFileName, pRuntime);
    mSimulations.insert(pFileName, pSimulation);

    // Update the information about our graphs properties

    updateGraphsInfo();
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::finalize(const QString &pFileName)
{
    // Remove track of the file name, runtime and simulation

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

bool SingleCellViewInformationGraphsWidget::checkParameter(const QString &pFileName,
                                                           Core::Property *pProperty) const
{
    bool res = false;

    // Retrieve the runtime associated with the given file name

    CellMLSupport::CellmlFileRuntime *runtime = mRuntimes.value(pFileName);

    // Check that the information held by the given property corresponds to
    // an existing parameter in our runtime

    if (runtime) {
        QStringList info = pProperty->value().split(".");
        QString componentName = info.first();
        QString parameterName = info.last();

        foreach (CellMLSupport::CellmlFileRuntimeParameter *parameter, runtime->parameters())
            if (   !parameter->component().compare(componentName)
                && !parameter->name().compare(parameterName)) {
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

void SingleCellViewInformationGraphsWidget::updateGraphInfo(Core::Property *pProperty,
                                                            const QString &pFileName) const
{
    // Update the graph information by checking the new value of the given
    // section property

    // Update the model property's icon based on its value and determine the
    // file name from which we will have to check our X and Y properties

    QString oldFileName = pProperty->properties()[0]->value();
    QString fileName = mFileName;

    if (!pFileName.compare(tr("Current"))) {
        pProperty->properties()[0]->setIcon(QIcon(":/oxygen/status/object-unlocked.png"));
    } else {
        pProperty->properties()[0]->setIcon(QIcon(":/oxygen/status/object-locked.png"));

        fileName = pFileName.split(" | ").last();
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
        pProperty->setName(QString("%1 | %2").arg(pProperty->properties()[1]->value(),
                                                  pProperty->properties()[2]->value()));

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
    // corresponding section property, but only if the property is not the model
    // one (since we will have been updating the graph information in
    // modelChanged()) above)

    if (pProperty != pProperty->parentProperty()->properties()[0])
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
        modelListValue << QString("%1 | %2").arg(QFileInfo(fileName).fileName(), fileName);

    modelListValue.sort();

    modelListValue.prepend(QString());
    modelListValue.prepend(tr("Current"));

    // Go through our section properties and update (incl. retranslate) their
    // information

    foreach (Core::Property *sectionProperty, sectionProperties) {
        sectionProperty->properties()[0]->setName(tr("Model"));
        sectionProperty->properties()[1]->setName(tr("X"));
        sectionProperty->properties()[2]->setName(tr("Y"));

        sectionProperty->properties()[0]->setListValue(modelListValue);

        updateGraphInfo(sectionProperty, sectionProperty->properties()[0]->value());
    }
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
