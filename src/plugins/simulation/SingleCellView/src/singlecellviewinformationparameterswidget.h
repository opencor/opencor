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

#pragma once

//==============================================================================

#include "propertyeditorwidget.h"

//==============================================================================

namespace OpenCOR {

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

class SingleCellViewInformationParametersWidget : public Core::PropertyEditorWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewInformationParametersWidget(QWidget *pParent);

    virtual void retranslateUi();

    void initialize(SingleCellViewSimulation *pSimulation,
                    const bool &pReloadingView = false);
    void finalize();

    QMap<Core::Property *, CellMLSupport::CellmlFileRuntimeParameter *> parameters() const;

private:
    QMenu *mContextMenu;

    QMap<Core::Property *, CellMLSupport::CellmlFileRuntimeParameter *> mParameters;
    QMap<QAction *, CellMLSupport::CellmlFileRuntimeParameter *> mParameterActions;

    SingleCellViewSimulation *mSimulation;

    void populateModel(CellMLSupport::CellmlFileRuntime *pRuntime);
    void populateContextMenu(CellMLSupport::CellmlFileRuntime *pRuntime);

    void updateExtraInfos();

    void retranslateContextMenu();

Q_SIGNALS:
    void graphRequired(CellMLSupport::CellmlFileRuntimeParameter *pParameterX,
                       CellMLSupport::CellmlFileRuntimeParameter *pParameterY);

public Q_SLOTS:
    void updateParameters(const double &pCurrentPoint,
                          const bool &pProcessEvents = false);

private Q_SLOTS:
    void propertyEditorContextMenu(const QPoint &pPosition) const;

    void propertyChanged(Core::Property *pProperty);

    void emitGraphRequired();
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
