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
    mPropertyEditors(QMap<QString, Core::PropertyEditorWidget *>()),
    mColumnWidths(QList<int>())
{
    // Create a widget that will be shown whenever there are no graphs
    // associated with the current plotting area

    mNoGraphsMessageWidget = new QLabel(pParent);

    mNoGraphsMessageWidget->setAlignment(Qt::AlignCenter);
    mNoGraphsMessageWidget->setAutoFillBackground(true);
    mNoGraphsMessageWidget->setBackgroundRole(QPalette::Base);
    mNoGraphsMessageWidget->setMargin(3);
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

void SingleCellViewInformationGraphsWidget::finishPropertyEditing()
{
    // Retrieve our current property editor, if any

    Core::PropertyEditorWidget *propertyEditor = qobject_cast<Core::PropertyEditorWidget *>(currentWidget());

    if (!propertyEditor)
        return;

    // Finish the editing of our current property editor

    propertyEditor->finishPropertyEditing();
}

//==============================================================================

void SingleCellViewInformationGraphsWidget::requireGraph(CellMLSupport::CellmlFileRuntimeParameter *pParameterX,
                                                         CellMLSupport::CellmlFileRuntimeParameter *pParameterY)
{
//---GRY--- TO BE DONE...
    // Keep track of the graph requirement

qDebug(">>> Graph required for:");
qDebug(">>>  - X: %s", qPrintable(pParameterX->name()));
qDebug(">>>  - Y: %s", qPrintable(pParameterY->name()));
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
