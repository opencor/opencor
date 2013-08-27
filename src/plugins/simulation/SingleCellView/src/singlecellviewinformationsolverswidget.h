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
// Single cell view information solvers widget
//==============================================================================

#ifndef SINGLECELLVIEWINFORMATIONSOLVERSWIDGET_H
#define SINGLECELLVIEWINFORMATIONSOLVERSWIDGET_H

//==============================================================================

#include "propertyeditorwidget.h"
#include "solverinterface.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntime;
}   // namespace CellMLSupport

//==============================================================================

namespace SingleCellView {

//==============================================================================

class SingleCellViewSimulation;

//==============================================================================

class SingleCellViewInformationSolversWidgetData
{
public:
    explicit SingleCellViewInformationSolversWidgetData(Core::Property *pSolversProperty,
                                                        Core::Property *pSolversListProperty,
                                                        const QMap<QString, Core::Properties> &pSolversProperties);

    Core::Property * solversProperty() const;
    Core::Property * solversListProperty() const;

    QMap<QString, Core::Properties> solversProperties() const;

private:
    Core::Property *mSolversProperty;
    Core::Property *mSolversListProperty;

    QMap<QString, Core::Properties> mSolversProperties;
};

//==============================================================================

class SingleCellViewInformationSolversWidget : public Core::PropertyEditorWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewInformationSolversWidget(QWidget *pParent = 0);
    ~SingleCellViewInformationSolversWidget();

    virtual void retranslateUi();

    void setSolverInterfaces(const SolverInterfaces &pSolverInterfaces);

    void initialize(const QString &pFileName,
                    CellMLSupport::CellmlFileRuntime *pRuntime,
                    SingleCellViewSimulation *pSimulation);
    void backup(const QString &pFileName);
    void finalize(const QString &pFileName);

    QStringList odeSolvers() const;
    QStringList daeSolvers() const;
    QStringList nlaSolvers() const;

    SingleCellViewInformationSolversWidgetData * odeSolverData() const;
    SingleCellViewInformationSolversWidgetData * daeSolverData() const;
    SingleCellViewInformationSolversWidgetData * nlaSolverData() const;

private:
    SingleCellViewInformationSolversWidgetData *mOdeSolverData;
    SingleCellViewInformationSolversWidgetData *mDaeSolverData;
    SingleCellViewInformationSolversWidgetData *mNlaSolverData;

    QMap<QString, Core::PropertyEditorWidgetGuiState *> mGuiStates;
    Core::PropertyEditorWidgetGuiState *mDefaultGuiState;

    QMap<Core::Property *, Descriptions> mDescriptions;

    void resetAllGuiStates();

    SingleCellViewInformationSolversWidgetData * addSolverProperties(const SolverInterfaces &pSolverInterfaces,
                                                                     const Solver::Type &pSolverType);

    void doSolverChanged(SingleCellViewInformationSolversWidgetData *pSolverData,
                         const QString &pSolverName);

    void setPropertiesUnit(SingleCellViewInformationSolversWidgetData *pSolverData,
                           const QString &pVoiUnit);

private Q_SLOTS:
    void solverChanged(Core::Property *pProperty, const QString &pValue);
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
