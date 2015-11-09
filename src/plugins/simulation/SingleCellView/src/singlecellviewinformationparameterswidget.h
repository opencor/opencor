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
// Single cell view information parameters widget
//==============================================================================

#ifndef SINGLECELLVIEWINFORMATIONPARAMETERSWIDGET_H
#define SINGLECELLVIEWINFORMATIONPARAMETERSWIDGET_H

//==============================================================================

#include "commonwidget.h"
#include "corecliutils.h"

//==============================================================================

#include <QList>
#include <QMap>
#include <QPoint>
#include <QStackedWidget>

//==============================================================================

class QAction;
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

class SingleCellViewSimulation;

//==============================================================================

class SingleCellViewInformationParametersWidget : public QStackedWidget,
                                                  public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewInformationParametersWidget(QWidget *pParent);

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void initialize(const QString &pFileName,
                    CellMLSupport::CellmlFileRuntime *pRuntime,
                    SingleCellViewSimulation *pSimulation);
    void finalize(const QString &pFileName);

    void finishEditing();

    QMap<Core::Property *, CellMLSupport::CellmlFileRuntimeParameter *> parameters() const;

private:
    QMap<QString, Core::PropertyEditorWidget *> mPropertyEditors;
    Core::PropertyEditorWidget *mPropertyEditor;

    QMap<QString, QMenu *> mContextMenus;
    QMenu *mContextMenu;

    QMap<QString, QMap<Core::Property *, CellMLSupport::CellmlFileRuntimeParameter *> *> mParametersMapping;
    QMap<Core::Property *, CellMLSupport::CellmlFileRuntimeParameter *> *mParameters;

    QMap<QString, QMap<QAction *, CellMLSupport::CellmlFileRuntimeParameter *> *> mParameterActionsMapping;
    QMap<QAction *, CellMLSupport::CellmlFileRuntimeParameter *> *mParameterActions;

    QIntList mColumnWidths;

    QString mFileName;

    SingleCellViewSimulation *mSimulation;

    int mHorizontalScrollBarValue;

    void populateModel(CellMLSupport::CellmlFileRuntime *pRuntime);
    void populateContextMenu(QMenu *pContextMenu,
                             CellMLSupport::CellmlFileRuntime *pRuntime);

    void updateExtraInfos(const bool &pUpdateToolTips = true);

    void retranslateContextMenu(QMenu *pContextMenu);

Q_SIGNALS:
    void graphRequired(CellMLSupport::CellmlFileRuntimeParameter *pParameterX,
                       CellMLSupport::CellmlFileRuntimeParameter *pParameterY);

public Q_SLOTS:
    void updateParameters(const double &pCurrentPoint,
                          const bool &pProcessEvents = false);

private Q_SLOTS:
    void propertyEditorContextMenu(const QPoint &pPosition) const;

    void propertyEditorHorizontalScrollBarValueChanged(const int &pValue);

    void propertyEditorSectionResized(const int &pLogicalIndex,
                                      const int &pOldSize, const int &pNewSize);

    void propertyChanged(Core::Property *pProperty);

    void emitGraphRequired();
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
