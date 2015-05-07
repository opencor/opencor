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
// CVODESolver plugin
//==============================================================================

#include "cvodesolver.h"
#include "cvodesolverplugin.h"

//==============================================================================

namespace OpenCOR {
namespace CVODESolver {

//==============================================================================

PLUGININFO_FUNC CVODESolverPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin that uses <a href=\"http://computation.llnl.gov/casc/sundials/description/description.html#descr_cvode\">CVODE</a> to solve ODEs."));
    descriptions.insert("fr", QString::fromUtf8("une extension qui utilise <a href=\"http://computation.llnl.gov/casc/sundials/description/description.html#descr_cvode\">CVODE</a> pour résoudre des EDOs."));

    return new PluginInfo(PluginInfo::Solver, true, false,
                          QStringList() << "CoreSolver" << "SUNDIALS",
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================

void CVODESolverPlugin::retranslateUi()
{
    // We don't handle this interface...
    // Note: even though we don't handle this interface, we still want to
    //       support it since some other aspects of our plugin are
    //       multilingual...
}

//==============================================================================
// Solver interface
//==============================================================================

void * CVODESolverPlugin::solverInstance() const
{
    // Create and return an instance of the solver

    return new CvodeSolver();
}

//==============================================================================

Solver::Type CVODESolverPlugin::solverType() const
{
    // Return the type of the solver

    return Solver::Ode;
}

//==============================================================================

QString CVODESolverPlugin::solverName() const
{
    // Return the name of the solver

    return "CVODE";
}

//==============================================================================

Solver::Properties CVODESolverPlugin::solverProperties() const
{
    // Return the properties supported by the solver

    Descriptions MaximumStepDescriptions;
    Descriptions MaximumNumberOfStepsDescriptions;
    Descriptions IntegrationMethodDescriptions;
    Descriptions IteratorTypeDescriptions;
    Descriptions LinearSolverDescriptions;
    Descriptions PreconditionerDescriptions;
    Descriptions UpperHalfBandwidthDescriptions;
    Descriptions LowerHalfBandwidthDescriptions;
    Descriptions RelativeToleranceDescriptions;
    Descriptions AbsoluteToleranceDescriptions;
    Descriptions InterpolateSolutionDescriptions;

    MaximumStepDescriptions.insert("en", QString::fromUtf8("Maximum step"));
    MaximumStepDescriptions.insert("fr", QString::fromUtf8("Pas maximum"));

    MaximumNumberOfStepsDescriptions.insert("en", QString::fromUtf8("Maximum number of steps"));
    MaximumNumberOfStepsDescriptions.insert("fr", QString::fromUtf8("Nombre maximum de pas"));

    IntegrationMethodDescriptions.insert("en", QString::fromUtf8("Integration method"));
    IntegrationMethodDescriptions.insert("fr", QString::fromUtf8("Méthode d'intégration"));

    IteratorTypeDescriptions.insert("en", QString::fromUtf8("Iterator type"));
    IteratorTypeDescriptions.insert("fr", QString::fromUtf8("Type d'itérateur"));

    LinearSolverDescriptions.insert("en", QString::fromUtf8("Linear solver"));
    LinearSolverDescriptions.insert("fr", QString::fromUtf8("Solveur linéaire"));

    PreconditionerDescriptions.insert("en", QString::fromUtf8("Preconditioner"));
    PreconditionerDescriptions.insert("fr", QString::fromUtf8("Préconditionneur"));

    UpperHalfBandwidthDescriptions.insert("en", QString::fromUtf8("Upper half-bandwidth"));
    UpperHalfBandwidthDescriptions.insert("fr", QString::fromUtf8("Demi largeur de bande supérieure"));

    LowerHalfBandwidthDescriptions.insert("en", QString::fromUtf8("Lower half-bandwidth"));
    LowerHalfBandwidthDescriptions.insert("fr", QString::fromUtf8("Demi largeur de bande inférieure"));

    RelativeToleranceDescriptions.insert("en", QString::fromUtf8("Relative tolerance"));
    RelativeToleranceDescriptions.insert("fr", QString::fromUtf8("Tolérance relative"));

    AbsoluteToleranceDescriptions.insert("en", QString::fromUtf8("Absolute tolerance"));
    AbsoluteToleranceDescriptions.insert("fr", QString::fromUtf8("Tolérance absolue"));

    InterpolateSolutionDescriptions.insert("en", QString::fromUtf8("Interpolate solution"));
    InterpolateSolutionDescriptions.insert("fr", QString::fromUtf8("Interpoler solution"));

    QStringList IntegrationMethodListValues = QStringList() << AdamsMoultonMethod
                                                            << BdfMethod;

    QStringList IteratorTypeListValues = QStringList() << FunctionalIterator
                                                       << NewtonIterator;

    QStringList LinearSolverListValues = QStringList() << DenseLinearSolver
                                                       << BandedLinearSolver
                                                       << DiagonalLinearSolver
                                                       << GmresLinearSolver
                                                       << BiCgStabLinearSolver
                                                       << TfqmrLinearSolver;

    QStringList PreconditionerListValues = QStringList() << NoPreconditioner
                                                         << BandedPreconditioner;

    return Solver::Properties() << Solver::Property(Solver::Property::Double, MaximumStepId, MaximumStepDescriptions, QStringList(), MaximumStepDefaultValue, true)
                                << Solver::Property(Solver::Property::Integer, MaximumNumberOfStepsId, MaximumNumberOfStepsDescriptions, QStringList(), MaximumNumberOfStepsDefaultValue, false)
                                << Solver::Property(Solver::Property::List, IntegrationMethodId, IntegrationMethodDescriptions, IntegrationMethodListValues, IntegrationMethodDefaultValue, false)
                                << Solver::Property(Solver::Property::List, IteratorTypeId, IteratorTypeDescriptions, IteratorTypeListValues, IteratorTypeDefaultValue, false)
                                << Solver::Property(Solver::Property::List, LinearSolverId, LinearSolverDescriptions, LinearSolverListValues, LinearSolverDefaultValue, false)
                                << Solver::Property(Solver::Property::List, PreconditionerId, PreconditionerDescriptions, PreconditionerListValues, PreconditionerDefaultValue, false)
                                << Solver::Property(Solver::Property::Integer, UpperHalfBandwidthId, UpperHalfBandwidthDescriptions, QStringList(), UpperHalfBandwidthDefaultValue, false)
                                << Solver::Property(Solver::Property::Integer, LowerHalfBandwidthId, LowerHalfBandwidthDescriptions, QStringList(), LowerHalfBandwidthDefaultValue, false)
                                << Solver::Property(Solver::Property::Double, RelativeToleranceId, RelativeToleranceDescriptions, QStringList(), RelativeToleranceDefaultValue, false)
                                << Solver::Property(Solver::Property::Double, AbsoluteToleranceId, AbsoluteToleranceDescriptions, QStringList(), AbsoluteToleranceDefaultValue, false)
                                << Solver::Property(Solver::Property::Boolean, InterpolateSolutionId, InterpolateSolutionDescriptions, QStringList(), InterpolateSolutionDefaultValue, false);
}

//==============================================================================

QMap<QString, bool> CVODESolverPlugin::solverPropertiesVisibility(const QMap<QString, QString> &pSolverPropertiesValues) const
{
    // Return the visibility of our properties based on the given properties
    // values
Q_UNUSED(pSolverPropertiesValues);

    QMap<QString, bool> res = QMap<QString, bool>();

    return res;
}

//==============================================================================

}   // namespace CVODESolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
