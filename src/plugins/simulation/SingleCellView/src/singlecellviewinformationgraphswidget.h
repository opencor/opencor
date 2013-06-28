//==============================================================================
// Single cell view information graphs widget
//==============================================================================

#ifndef SINGLECELLVIEWINFORMATIONGRAPHSWIDGET_H
#define SINGLECELLVIEWINFORMATIONGRAPHSWIDGET_H

//==============================================================================

#include "commonwidget.h"

//==============================================================================

#include <QStackedWidget>

//==============================================================================

class QLabel;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class PropertyEditorWidget;
}   // namespace Core

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntime;
    class CellmlFileRuntimeParameter;
}   // namespace CellMLSupport

//==============================================================================

namespace SingleCellView {

//==============================================================================

class SingleCellViewSimulationData;

//==============================================================================

class SingleCellViewInformationGraphsWidget : public QStackedWidget,
                                              public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewInformationGraphsWidget(QWidget *pParent = 0);

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void finishPropertyEditing();

private:
    QMap<QString, Core::PropertyEditorWidget *> mPropertyEditors;

    QList<int> mColumnWidths;

    QLabel *mNoGraphsMessageWidget;

public Q_SLOTS:
    void requireGraph(CellMLSupport::CellmlFileRuntimeParameter *pParameterX,
                      CellMLSupport::CellmlFileRuntimeParameter *pParameterY);
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
