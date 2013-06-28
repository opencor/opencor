//==============================================================================
// Single cell view information traces widget
//==============================================================================

#ifndef SINGLECELLVIEWINFORMATIONTRACESWIDGET_H
#define SINGLECELLVIEWINFORMATIONTRACESWIDGET_H

//==============================================================================

#include "commonwidget.h"

//==============================================================================

#include <QStackedWidget>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class PropertyEditorWidget;
}   // namespace Core

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntime;
}   // namespace CellMLSupport

//==============================================================================

namespace SingleCellView {

//==============================================================================

class SingleCellViewSimulationData;

//==============================================================================

class SingleCellViewInformationTracesWidget : public QStackedWidget,
                                              public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewInformationTracesWidget(QWidget *pParent = 0);

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void initialize(const QString &pFileName,
                    CellMLSupport::CellmlFileRuntime *pRuntime,
                    SingleCellViewSimulationData *pSimulationData);
    void finalize(const QString &pFileName);

    void finishPropertyEditing();

private:
    QMap<QString, Core::PropertyEditorWidget *> mPropertyEditors;

    QList<int> mColumnWidths;
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
