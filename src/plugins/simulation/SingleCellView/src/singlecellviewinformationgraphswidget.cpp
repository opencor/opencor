//==============================================================================
// Single cell view information graphs widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "propertyeditorwidget.h"
#include "singlecellviewinformationgraphswidget.h"

//==============================================================================

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
    mPropertyEditor(0)
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

void SingleCellViewInformationGraphsWidget::initialize(SingleCellViewGraphPanelWidget *pGraphPanel)
{
//---GRY--- THE BELOW IS JUST FOR DEVELOPMENT PURPOSES...
mNoGraphsMessageWidget->setText(tr("There are no graphs..."));
QString p;
p.sprintf("%p", pGraphPanel);
mNoGraphsMessageWidget->setText(mNoGraphsMessageWidget->text()+" ["+p+"]");
    // Retrieve the property editor for the given file name or create one, if
    // none exists

    mPropertyEditor = mPropertyEditors.value(pGraphPanel);

    if (!mPropertyEditor) {
        // No property editor exists for the given graph panel, so create one

        mPropertyEditor = new Core::PropertyEditorWidget(this);

        // Initialise our property editor's columns' width

        for (int i = 0, iMax = mColumnWidths.size(); i < iMax; ++i)
            mPropertyEditor->setColumnWidth(i, mColumnWidths.at(i));

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
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::finalize(SingleCellViewGraphPanelWidget *pGraphPanel)
{
    // Remove any track of our property editor

    mPropertyEditors.remove(pGraphPanel);
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::addGraph(SingleCellViewGraphPanelPlotGraph *pGraph)
{
//---GRY--- TO BE DONE...
qDebug(">>> Added graph: %p", pGraph);
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::removeGraph(SingleCellViewGraphPanelPlotGraph *pGraph)
{
//---GRY--- TO BE DONE...
qDebug(">>> Removed graph: %p", pGraph);
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

void SingleCellViewInformationGraphsWidget::propertyChanged(Core::Property *pProperty)
{
//---GRY--- TO BE DONE...

Q_UNUSED(pProperty);
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
