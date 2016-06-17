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
// Single Cell view information solvers widget
//==============================================================================

#pragma once

//==============================================================================

#include "propertyeditorwidget.h"
#include "solverinterface.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace SingleCellView {

//==============================================================================

class SingleCellViewPlugin;
class SingleCellViewSimulation;

//==============================================================================

class SingleCellViewInformationSolversWidgetData
{
public:
    explicit SingleCellViewInformationSolversWidgetData(const QMap<QString, SolverInterface *> &pSolversInterfaces,
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

class SingleCellViewInformationSolversWidget : public Core::PropertyEditorWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewInformationSolversWidget(SingleCellViewPlugin *pPlugin,
                                                    QWidget *pParent);
    ~SingleCellViewInformationSolversWidget();

    virtual void retranslateUi();

    void initialize(SingleCellViewSimulation *pSimulation);

    QStringList odeSolvers() const;
    QStringList daeSolvers() const;
    QStringList nlaSolvers() const;

    SingleCellViewInformationSolversWidgetData * odeSolverData() const;
    SingleCellViewInformationSolversWidgetData * daeSolverData() const;
    SingleCellViewInformationSolversWidgetData * nlaSolverData() const;

    void updateGui(SingleCellViewInformationSolversWidgetData *pSolverData);

private:
    SingleCellViewInformationSolversWidgetData *mOdeSolverData;
    SingleCellViewInformationSolversWidgetData *mDaeSolverData;
    SingleCellViewInformationSolversWidgetData *mNlaSolverData;

    QMap<Core::Property *, Descriptions> mDescriptions;

    void updateSolverGui(SingleCellViewInformationSolversWidgetData *pSolverData);

    SingleCellViewInformationSolversWidgetData * addSolverProperties(const SolverInterfaces &pSolverInterfaces,
                                                                     const Solver::Type &pSolverType);

    void doSolverChanged(SingleCellViewInformationSolversWidgetData *pSolverData,
                         const QString &pSolverName);

    void setPropertiesUnit(SingleCellViewInformationSolversWidgetData *pSolverData,
                           const QString &pVoiUnit);

private Q_SLOTS:
    void solverChanged(Core::Property *pProperty);
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
