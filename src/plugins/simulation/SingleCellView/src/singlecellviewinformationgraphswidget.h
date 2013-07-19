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
class QMenu;

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

    void fileOpened(const QString &pFileName);
    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);

    void finishPropertyEditing();

private:
    QMap<SingleCellViewGraphPanelWidget *, Core::PropertyEditorWidget *> mPropertyEditors;
    QMap<QString, QMenu *> mContextMenus;

    QMap<QAction *, CellMLSupport::CellmlFileRuntimeParameter *> mParameterActions;

    QList<int> mColumnWidths;

    QLabel *mNoGraphsMessageWidget;
    Core::PropertyEditorWidget *mPropertyEditor;

    QStringList mFileNames;
    QString mFileName;

    QMap<QString, CellMLSupport::CellmlFileRuntime *> mRuntimes;
    QMap<QString, SingleCellViewSimulation *> mSimulations;

    void populateContextMenu(QMenu *pContextMenu,
                             CellMLSupport::CellmlFileRuntime *pRuntime);

    bool checkParameter(const QString &pFileName,
                        OpenCOR::Core::Property *pProperty) const;

    void updateGraphInfo(OpenCOR::Core::Property *pProperty,
                         const QString &pFileName) const;
    void updateGraphsInfo(Core::Property *pSectionProperty = 0);

public Q_SLOTS:
    void initialize(SingleCellViewGraphPanelWidget *pGraphPanel);
    void finalize(SingleCellViewGraphPanelWidget *pGraphPanel);

    void addGraph(SingleCellViewGraphPanelPlotGraph *pGraph);
    void removeGraph(SingleCellViewGraphPanelPlotGraph *pGraph);

private Q_SLOTS:
    void propertyEditorContextMenu(const QPoint &pPosition) const;
    void propertyEditorSectionResized(const int &pLogicalIndex,
                                      const int &pOldSize, const int &pNewSize);

    void modelChanged(Core::Property *pProperty, const QString &pValue);
    void propertyChanged(Core::Property *pProperty);

    void updateParameterValue();
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
