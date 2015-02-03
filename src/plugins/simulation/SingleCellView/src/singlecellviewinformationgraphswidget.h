/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Single cell view information graphs widget
//==============================================================================

#ifndef SINGLECELLVIEWINFORMATIONGRAPHSWIDGET_H
#define SINGLECELLVIEWINFORMATIONGRAPHSWIDGET_H

//==============================================================================

#include "commonwidget.h"
#include "corecliutils.h"

//==============================================================================

#include <QList>
#include <QMap>
#include <QStackedWidget>

//==============================================================================

class QAction;
class QLabel;
class QMenu;

//==============================================================================

namespace Ui {
    class SingleCellViewInformationGraphsWidget;
}

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
class SingleCellViewGraphPanelPlotWidget;
class SingleCellViewGraphPanelWidget;
class SingleCellViewSimulation;

//==============================================================================

class SingleCellViewInformationGraphsWidget : public QStackedWidget,
                                              public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewInformationGraphsWidget(QWidget *pParent);

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void initialize(const QString &pFileName,
                    CellMLSupport::CellmlFileRuntime *pRuntime,
                    SingleCellViewSimulation *pSimulation);
    void backup(const QString &pFileName);
    void finalize(const QString &pFileName);

    void fileOpened(const QString &pFileName);
    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);
    void fileClosed(const QString &pFileName);

    void finishEditing();

private:
    Ui::SingleCellViewInformationGraphsWidget *mGui;

    QMap<Core::PropertyEditorWidget *, SingleCellViewGraphPanelWidget *> mGraphPanels;
    QMap<SingleCellViewGraphPanelWidget *, Core::PropertyEditorWidget *> mPropertyEditors;
    Core::PropertyEditorWidget *mPropertyEditor;

    QMap<Core::Property *, SingleCellViewGraphPanelPlotGraph *> mGraphs;
    QMap<SingleCellViewGraphPanelPlotGraph *, Core::Property *> mGraphProperties;

    QMap<QString, QMenu *> mContextMenus;
    QMenu *mContextMenu;

    QMap<QAction *, CellMLSupport::CellmlFileRuntimeParameter *> mParameterActions;

    QIntList mColumnWidths;

    QStringList mFileNames;
    QString mFileName;

    QMap<QString, CellMLSupport::CellmlFileRuntime *> mRuntimes;
    QMap<QString, SingleCellViewSimulation *> mSimulations;
    QMap<QString, QMap<Core::Property *, bool>> mGraphPropertiesSelected;

    bool mCanEmitGraphsUpdatedSignal;

    void populateContextMenu(QMenu *pContextMenu,
                             CellMLSupport::CellmlFileRuntime *pRuntime);

    bool checkParameter(CellMLSupport::CellmlFileRuntime *pRuntime,
                        SingleCellViewGraphPanelPlotGraph *pGraph,
                        Core::Property *pParameterProperty,
                        const bool &pParameterX) const;

    void updateGraphInfo(Core::Property *pProperty, const QString &pFileName);
    void updateGraphsInfo(Core::Property *pSectionProperty = 0,
                          const bool &pGlobalGraphsUpdate = false);
    void updateAllGraphsInfo(const bool &pGlobalGraphsUpdate = false);

    void selectAllGraphs(const bool &pSelect);

Q_SIGNALS:
    void graphsUpdated(SingleCellViewGraphPanelPlotWidget *pPlot,
                       const QList<SingleCellViewGraphPanelPlotGraph *> &pGraphs);

public Q_SLOTS:
    void initialize(SingleCellViewGraphPanelWidget *pGraphPanel);
    void finalize(SingleCellViewGraphPanelWidget *pGraphPanel);

    void addGraph(SingleCellViewGraphPanelPlotWidget *pPlot,
                  SingleCellViewGraphPanelPlotGraph *pGraph);
    void removeGraphs(SingleCellViewGraphPanelPlotWidget *pPlot,
                      const QList<SingleCellViewGraphPanelPlotGraph *> &pGraphs);

private Q_SLOTS:
    void on_actionAddGraph_triggered();

    void on_actionRemoveCurrentGraph_triggered();
    void on_actionRemoveAllGraphs_triggered();

    void on_actionSelectAllGraphs_triggered();
    void on_actionUnselectAllGraphs_triggered();

    void propertyEditorContextMenu(const QPoint &pPosition) const;
    void propertyEditorSectionResized(const int &pLogicalIndex,
                                      const int &pOldSize, const int &pNewSize);

    void modelChanged(Core::Property *pProperty, const QString &pValue);
    void graphChanged(Core::Property *pProperty);

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
