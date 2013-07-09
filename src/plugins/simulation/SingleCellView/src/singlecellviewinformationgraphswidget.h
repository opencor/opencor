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

//==============================================================================

namespace Core {
    class Property;
    class PropertyEditorWidget;
}   // namespace Core

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntimeParameter;
}   // namespace CellMLSupport

//==============================================================================

namespace SingleCellView {

//==============================================================================

class SingleCellViewGraphPanelPlotGraph;
class SingleCellViewGraphPanelWidget;

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
    QMap<SingleCellViewGraphPanelWidget *, Core::PropertyEditorWidget *> mPropertyEditors;

    QList<int> mColumnWidths;

    QLabel *mNoGraphsMessageWidget;
    Core::PropertyEditorWidget *mPropertyEditor;

public Q_SLOTS:
    void initialize(SingleCellViewGraphPanelWidget *pGraphPanel);
    void finalize(SingleCellViewGraphPanelWidget *pGraphPanel);

    void addGraph(SingleCellViewGraphPanelPlotGraph *pGraph);
    void removeGraph(SingleCellViewGraphPanelPlotGraph *pGraph);

private Q_SLOTS:
    void propertyChanged(Core::Property *pProperty);
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
