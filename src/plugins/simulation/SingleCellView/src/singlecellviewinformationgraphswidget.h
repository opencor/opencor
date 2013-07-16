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
    class Property;
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

class SingleCellViewGraphPanelPlotGraph;
class SingleCellViewGraphPanelWidget;
class SingleCellViewSimulation;

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

    void initialize(const QString &pFileName,
                    CellMLSupport::CellmlFileRuntime *pRuntime,
                    SingleCellViewSimulation *pSimulation);
    void finalize(const QString &pFileName);

    void finishPropertyEditing();

private:
    QMap<SingleCellViewGraphPanelWidget *, Core::PropertyEditorWidget *> mPropertyEditors;

    QList<int> mColumnWidths;

    QLabel *mNoGraphsMessageWidget;
    Core::PropertyEditorWidget *mPropertyEditor;

    QString mFileName;
    QStringList mFileNames;

    QMap<QString, CellMLSupport::CellmlFileRuntime *> mRuntimes;
    QMap<QString, SingleCellViewSimulation *> mSimulations;

    void updateGraphsInfo(Core::Property *pSectionProperty = 0);

public Q_SLOTS:
    void initialize(SingleCellViewGraphPanelWidget *pGraphPanel);
    void finalize(SingleCellViewGraphPanelWidget *pGraphPanel);

    void addGraph(SingleCellViewGraphPanelPlotGraph *pGraph);
    void removeGraph(SingleCellViewGraphPanelPlotGraph *pGraph);

private Q_SLOTS:
    void propertyEditorSectionResized(const int &pLogicalIndex,
                                      const int &pOldSize, const int &pNewSize);

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
