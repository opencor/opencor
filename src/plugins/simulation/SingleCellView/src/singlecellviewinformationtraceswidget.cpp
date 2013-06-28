//==============================================================================
// Single cell view information traces widget
//==============================================================================

//#include "cellmlfileruntime.h"
#include "propertyeditorwidget.h"
#include "singlecellviewinformationtraceswidget.h"
//#include "singlecellviewsimulation.h"

//==============================================================================

#include <QHeaderView>
//#include <QMenu>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

SingleCellViewInformationTracesWidget::SingleCellViewInformationTracesWidget(QWidget *pParent) :
    QStackedWidget(pParent),
    mPropertyEditors(QMap<QString, Core::PropertyEditorWidget *>()),
    mColumnWidths(QList<int>())
{
    // Determine the default width of each column of our property editors

    Core::PropertyEditorWidget *tempPropertyEditor = new Core::PropertyEditorWidget(this);

    for (int i = 0, iMax = tempPropertyEditor->header()->count(); i < iMax; ++i)
        mColumnWidths.append(tempPropertyEditor->columnWidth(i));

    delete tempPropertyEditor;
}

//==============================================================================

void SingleCellViewInformationTracesWidget::retranslateUi()
{
    // Retranslate all our property editors

    foreach (Core::PropertyEditorWidget *propertyEditor, mPropertyEditors)
        propertyEditor->retranslateUi();
}

//==============================================================================

static const QString SettingsColumnWidth = "ColumnWidth%1";

//==============================================================================

void SingleCellViewInformationTracesWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the width of each column of our property editors

    for (int i = 0, iMax = mColumnWidths.size(); i < iMax; ++i)
        mColumnWidths[i] = pSettings->value(SettingsColumnWidth.arg(i),
                                            mColumnWidths.at(i)).toInt();

}

//==============================================================================

void SingleCellViewInformationTracesWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the width of each column of our current property editor

    for (int i = 0, iMax = mColumnWidths.size(); i < iMax; ++i)
        pSettings->setValue(SettingsColumnWidth.arg(i), mColumnWidths.at(i));
}

//==============================================================================

void SingleCellViewInformationTracesWidget::initialize(const QString &pFileName,
                                                           CellMLSupport::CellmlFileRuntime *pRuntime,
                                                           SingleCellViewSimulationData *pSimulationData)
{
Q_UNUSED(pFileName);
Q_UNUSED(pRuntime);
Q_UNUSED(pSimulationData);
//---GRY--- TO BE DONE...
}

//==============================================================================

void SingleCellViewInformationTracesWidget::finalize(const QString &pFileName)
{
Q_UNUSED(pFileName);
//---GRY--- TO BE DONE...
}

//==============================================================================

void SingleCellViewInformationTracesWidget::finishPropertyEditing()
{
    // Retrieve our current property editor, if any

    Core::PropertyEditorWidget *propertyEditor = qobject_cast<Core::PropertyEditorWidget *>(currentWidget());

    if (!propertyEditor)
        return;

    // Finish the editing of our current property editor

    propertyEditor->finishPropertyEditing();
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
