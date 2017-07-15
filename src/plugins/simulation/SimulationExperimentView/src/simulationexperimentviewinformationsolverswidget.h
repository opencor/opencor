/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
    explicit SimulationExperimentViewInformationSolversWidget(QWidget *pParent);
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

    SimulationExperimentViewInformationSolversWidgetData * addSolverProperties(const Solver::Type &pSolverType);

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
