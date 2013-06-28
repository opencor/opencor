//==============================================================================
// Single cell view information traces widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "propertyeditorwidget.h"
#include "singlecellviewinformationtraceswidget.h"

//==============================================================================

#include <QHeaderView>
#include <QLabel>
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
    // Create a widget that will be shown whenever there are no traces
    // associated with the current plotting area

    mNoTracesMessageWidget = new QLabel(pParent);

    mNoTracesMessageWidget->setAlignment(Qt::AlignCenter);
    mNoTracesMessageWidget->setAutoFillBackground(true);
    mNoTracesMessageWidget->setBackgroundRole(QPalette::Base);
    mNoTracesMessageWidget->setMargin(3);
    mNoTracesMessageWidget->setWordWrap(true);

    addWidget(mNoTracesMessageWidget);

    // Determine the default width of each column of our property editors

    Core::PropertyEditorWidget *tempPropertyEditor = new Core::PropertyEditorWidget(this);

    for (int i = 0, iMax = tempPropertyEditor->header()->count(); i < iMax; ++i)
        mColumnWidths.append(tempPropertyEditor->columnWidth(i));

    delete tempPropertyEditor;
}

//==============================================================================

void SingleCellViewInformationTracesWidget::retranslateUi()
{
    // Retranslate our no traces message widget

    mNoTracesMessageWidget->setText(tr("There are no traces..."));

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

void SingleCellViewInformationTracesWidget::requireTrace(CellMLSupport::CellmlFileRuntimeParameter *pParameterX,
                                                         CellMLSupport::CellmlFileRuntimeParameter *pParameterY)
{
//---GRY--- TO BE DONE...
    // Keep track of the trace requirement

qDebug(">>> Trace required for:");
qDebug(">>>  - X: %s", qPrintable(pParameterX->name()));
qDebug(">>>  - Y: %s", qPrintable(pParameterY->name()));
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
