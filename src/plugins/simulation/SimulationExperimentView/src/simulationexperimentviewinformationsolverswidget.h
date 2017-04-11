/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Simulation Experiment view information solvers widget
//==============================================================================

#pragma once

//==============================================================================

#include "propertyeditorwidget.h"
#include "solverinterface.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace SimulationExperimentView {

//==============================================================================

class SimulationExperimentViewPlugin;
class SimulationExperimentViewSimulation;

//==============================================================================

class SimulationExperimentViewInformationSolversWidgetData
{
public:
    explicit SimulationExperimentViewInformationSolversWidgetData(const QMap<QString, SolverInterface *> &pSolversInterfaces,
                                                                  Core::Property *pSolversProperty,
                                                                  Core::Property *pSolversListProperty,
                                                                  const QMap<QString, Core::Properties> &pSolversProperties);

    QMap<QString, SolverInterface *> solversInterfaces() const;

    Core::Property * solversProperty() const;
    Core::Property * solversListProperty() const;

    QMap<QString, Core::Properties> solversProperties() const;

private:
    QMap<QString, SolverInterface *> mSolversInterfaces;

    Core::Property *mSolversProperty;
    Core::Property *mSolversListProperty;

    QMap<QString, Core::Properties> mSolversProperties;
};

//==============================================================================

class SimulationExperimentViewInformationSolversWidget : public Core::PropertyEditorWidget
{
    Q_OBJECT

public:
    explicit SimulationExperimentViewInformationSolversWidget(SimulationExperimentViewPlugin *pPlugin,
                                                              QWidget *pParent);
    ~SimulationExperimentViewInformationSolversWidget();

    virtual void retranslateUi();

    void initialize(SimulationExperimentViewSimulation *pSimulation);

    QStringList odeSolvers() const;
    QStringList daeSolvers() const;
    QStringList nlaSolvers() const;

    SimulationExperimentViewInformationSolversWidgetData * odeSolverData() const;
    SimulationExperimentViewInformationSolversWidgetData * daeSolverData() const;
    SimulationExperimentViewInformationSolversWidgetData * nlaSolverData() const;

    void updateGui(SimulationExperimentViewInformationSolversWidgetData *pSolverData);

private:
    SimulationExperimentViewInformationSolversWidgetData *mOdeSolverData;
    SimulationExperimentViewInformationSolversWidgetData *mDaeSolverData;
    SimulationExperimentViewInformationSolversWidgetData *mNlaSolverData;

    QMap<Core::Property *, Descriptions> mDescriptions;

    void updateSolverGui(SimulationExperimentViewInformationSolversWidgetData *pSolverData);

    SimulationExperimentViewInformationSolversWidgetData * addSolverProperties(const SolverInterfaces &pSolverInterfaces,
                                                                               const Solver::Type &pSolverType);

    void doSolverChanged(SimulationExperimentViewInformationSolversWidgetData *pSolverData,
                         const QString &pSolverName);

    void setPropertiesUnit(SimulationExperimentViewInformationSolversWidgetData *pSolverData,
                           const QString &pVoiUnit);

private slots:
    void solverChanged(Core::Property *pProperty);
};

//==============================================================================

}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
