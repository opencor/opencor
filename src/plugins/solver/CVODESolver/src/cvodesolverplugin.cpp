/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// CVODE solver plugin
//==============================================================================

#include "cvodesolver.h"
#include "cvodesolverplugin.h"

//==============================================================================

namespace OpenCOR {
namespace CVODESolver {

//==============================================================================

PLUGININFO_FUNC CVODESolverPluginInfo()
{
    static const Descriptions descriptions = {
                                                 { "en", QString::fromUtf8(R"(a plugin that uses <a href="https://computing.llnl.gov/projects/sundials/cvode">CVODE</a> to solve <a href="https://en.wikipedia.org/wiki/Ordinary_differential_equation">ODEs</a>.)") },
                                                 { "fr", QString::fromUtf8(R"(une extension qui utilise <a href="https://computing.llnl.gov/projects/sundials/cvode">CVODE</a> pour résoudre des <a href="https://en.wikipedia.org/wiki/Ordinary_differential_equation">EDOs</a>.)") }
                                             };

    return new PluginInfo(PluginInfo::Category::Solver, true, false,
                          { "SUNDIALS" },
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

Solver::Solver * CVODESolverPlugin::solverInstance() const
{
    // Create and return an instance of the solver

    return new CvodeSolver();
}

//==============================================================================

QString CVODESolverPlugin::id(const QString &pKisaoId) const
{
    // Return the id for the given KiSAO id

    static const QString Kisao0000019 = "KISAO:0000019";
    static const QString Kisao0000467 = "KISAO:0000467";
    static const QString Kisao0000415 = "KISAO:0000415";
    static const QString Kisao0000475 = "KISAO:0000475";
    static const QString Kisao0000476 = "KISAO:0000476";
    static const QString Kisao0000477 = "KISAO:0000477";
    static const QString Kisao0000478 = "KISAO:0000478";
    static const QString Kisao0000479 = "KISAO:0000479";
    static const QString Kisao0000480 = "KISAO:0000480";
    static const QString Kisao0000209 = "KISAO:0000209";
    static const QString Kisao0000211 = "KISAO:0000211";
    static const QString Kisao0000481 = "KISAO:0000481";

    if (pKisaoId == Kisao0000019) {
        return solverName();
    }

    if (pKisaoId == Kisao0000467) {
        return MaximumStepId;
    }

    if (pKisaoId == Kisao0000415) {
        return MaximumNumberOfStepsId;
    }

    if (pKisaoId == Kisao0000475) {
        return IntegrationMethodId;
    }

    if (pKisaoId == Kisao0000476) {
        return IterationTypeId;
    }

    if (pKisaoId == Kisao0000477) {
        return LinearSolverId;
    }

    if (pKisaoId == Kisao0000478) {
        return PreconditionerId;
    }

    if (pKisaoId == Kisao0000479) {
        return UpperHalfBandwidthId;
    }

    if (pKisaoId == Kisao0000480) {
        return LowerHalfBandwidthId;
    }

    if (pKisaoId == Kisao0000209) {
        return RelativeToleranceId;
    }

    if (pKisaoId == Kisao0000211) {
        return AbsoluteToleranceId;
    }

    if (pKisaoId == Kisao0000481) {
        return InterpolateSolutionId;
    }

    return {};
}

//==============================================================================

QString CVODESolverPlugin::kisaoId(const QString &pId) const
{
    // Return the KiSAO id for the given id

    if (pId == solverName()) {
        return "KISAO:0000019";
    }

    if (pId == MaximumStepId) {
        return "KISAO:0000467";
    }

    if (pId == MaximumNumberOfStepsId) {
        return "KISAO:0000415";
    }

    if (pId == IntegrationMethodId) {
        return "KISAO:0000475";
    }

    if (pId == IterationTypeId) {
        return "KISAO:0000476";
    }

    if (pId == LinearSolverId) {
        return "KISAO:0000477";
    }

    if (pId == PreconditionerId) {
        return "KISAO:0000478";
    }

    if (pId == UpperHalfBandwidthId) {
        return "KISAO:0000479";
    }

    if (pId == LowerHalfBandwidthId) {
        return "KISAO:0000480";
    }

    if (pId == RelativeToleranceId) {
        return "KISAO:0000209";
    }

    if (pId == AbsoluteToleranceId) {
        return "KISAO:0000211";
    }

    if (pId == InterpolateSolutionId) {
        return "KISAO:0000481";
    }

    return {};
}

//==============================================================================

Solver::Type CVODESolverPlugin::solverType() const
{
    // Return the type of the solver

    return Solver::Type::Ode;
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

    static const Descriptions MaximumStepDescriptions = {
                                                            { "en", QString::fromUtf8("Maximum step") },
                                                            { "fr", QString::fromUtf8("Pas maximum") }
                                                        };
    static const Descriptions MaximumNumberOfStepsDescriptions = {
                                                                     { "en", QString::fromUtf8("Maximum number of steps") },
                                                                     { "fr", QString::fromUtf8("Nombre maximum de pas") }
                                                                 };
    static const Descriptions IntegrationMethodDescriptions = {
                                                                  { "en", QString::fromUtf8("Integration method") },
                                                                  { "fr", QString::fromUtf8("Méthode d'intégration") }
                                                              };
    static const Descriptions IterationTypeDescriptions = {
                                                              { "en", QString::fromUtf8("Iteration type") },
                                                              { "fr", QString::fromUtf8("Type d'itération") }
                                                          };
    static const Descriptions LinearSolverDescriptions = {
                                                             { "en", QString::fromUtf8("Linear solver") },
                                                             { "fr", QString::fromUtf8("Solveur linéaire") }
                                                         };
    static const Descriptions PreconditionerDescriptions = {
                                                               { "en", QString::fromUtf8("Preconditioner") },
                                                               { "fr", QString::fromUtf8("Préconditionneur") }
                                                           };
    static const Descriptions UpperHalfBandwidthDescriptions = {
                                                                   { "en", QString::fromUtf8("Upper half-bandwidth") },
                                                                   { "fr", QString::fromUtf8("Demi largeur de bande supérieure") }
                                                               };
    static const Descriptions LowerHalfBandwidthDescriptions = {
                                                                   { "en", QString::fromUtf8("Lower half-bandwidth") },
                                                                   { "fr", QString::fromUtf8("Demi largeur de bande inférieure") }
                                                               };
    static const Descriptions RelativeToleranceDescriptions = {
                                                                  { "en", QString::fromUtf8("Relative tolerance") },
                                                                  { "fr", QString::fromUtf8("Tolérance relative") }
                                                              };
    static const Descriptions AbsoluteToleranceDescriptions = {
                                                                  { "en", QString::fromUtf8("Absolute tolerance") },
                                                                  { "fr", QString::fromUtf8("Tolérance absolue") }
                                                              };
    static const Descriptions InterpolateSolutionDescriptions = {
                                                                    { "en", QString::fromUtf8("Interpolate solution") },
                                                                    { "fr", QString::fromUtf8("Interpoler solution") }
                                                                };
    static const QStringList IntegrationMethodListValues = {
                                                               AdamsMoultonMethod,
                                                               BdfMethod
                                                           };
    static const QStringList IterationTypeListValues = {
                                                           FunctionalIteration,
                                                           NewtonIteration
                                                       };
    static const QStringList LinearSolverListValues = {
                                                          DenseLinearSolver,
                                                          BandedLinearSolver,
                                                          DiagonalLinearSolver,
                                                          GmresLinearSolver,
                                                          BiCgStabLinearSolver,
                                                          TfqmrLinearSolver
                                                      };
    static const QStringList PreconditionerListValues = {
                                                            NoPreconditioner,
                                                            BandedPreconditioner
                                                        };

    return { Solver::Property(Solver::Property::Type::DoubleGe0, MaximumStepId, MaximumStepDescriptions, {}, MaximumStepDefaultValue, true),
             Solver::Property(Solver::Property::Type::IntegerGt0, MaximumNumberOfStepsId, MaximumNumberOfStepsDescriptions, {}, MaximumNumberOfStepsDefaultValue, false),
             Solver::Property(Solver::Property::Type::List, IntegrationMethodId, IntegrationMethodDescriptions, IntegrationMethodListValues, IntegrationMethodDefaultValue, false),
             Solver::Property(Solver::Property::Type::List, IterationTypeId, IterationTypeDescriptions, IterationTypeListValues, IterationTypeDefaultValue, false),
             Solver::Property(Solver::Property::Type::List, LinearSolverId, LinearSolverDescriptions, LinearSolverListValues, LinearSolverDefaultValue, false),
             Solver::Property(Solver::Property::Type::List, PreconditionerId, PreconditionerDescriptions, PreconditionerListValues, PreconditionerDefaultValue, false),
             Solver::Property(Solver::Property::Type::IntegerGe0, UpperHalfBandwidthId, UpperHalfBandwidthDescriptions, {}, UpperHalfBandwidthDefaultValue, false),
             Solver::Property(Solver::Property::Type::IntegerGe0, LowerHalfBandwidthId, LowerHalfBandwidthDescriptions, {}, LowerHalfBandwidthDefaultValue, false),
             Solver::Property(Solver::Property::Type::DoubleGe0, RelativeToleranceId, RelativeToleranceDescriptions, {}, RelativeToleranceDefaultValue, false),
             Solver::Property(Solver::Property::Type::DoubleGe0, AbsoluteToleranceId, AbsoluteToleranceDescriptions, {}, AbsoluteToleranceDefaultValue, false),
             Solver::Property(Solver::Property::Type::Boolean, InterpolateSolutionId, InterpolateSolutionDescriptions, {}, InterpolateSolutionDefaultValue, false) };
}

//==============================================================================

QMap<QString, bool> CVODESolverPlugin::solverPropertiesVisibility(const QMap<QString, QString> &pSolverPropertiesValues) const
{
    // Return the visibility of our properties based on the given properties
    // values

    QMap<QString, bool> res;

    if (pSolverPropertiesValues.value(IterationTypeId) == NewtonIteration) {
        // Newton iteration

        res.insert(LinearSolverId, true);

        QString linearSolver = pSolverPropertiesValues.value(LinearSolverId);

        if (   (linearSolver == DenseLinearSolver)
            || (linearSolver == DiagonalLinearSolver)) {
            // Dense/diagonal linear solver

            res.insert(PreconditionerId, false);
            res.insert(UpperHalfBandwidthId, false);
            res.insert(LowerHalfBandwidthId, false);
        } else if (linearSolver == BandedLinearSolver) {
            // Banded linear solver

            res.insert(PreconditionerId, false);
            res.insert(UpperHalfBandwidthId, true);
            res.insert(LowerHalfBandwidthId, true);
        } else {
            // GMRES/Bi-CGStab/TFQMR linear solver

            res.insert(PreconditionerId, true);

            if (pSolverPropertiesValues.value(PreconditionerId) == BandedPreconditioner) {
                // Banded preconditioner

                res.insert(UpperHalfBandwidthId, true);
                res.insert(LowerHalfBandwidthId, true);
            } else {
                // No preconditioner

                res.insert(UpperHalfBandwidthId, false);
                res.insert(LowerHalfBandwidthId, false);
            }
        }
    } else {
        // Functional iteration

        res.insert(LinearSolverId, false);
        res.insert(PreconditionerId, false);
        res.insert(UpperHalfBandwidthId, false);
        res.insert(LowerHalfBandwidthId, false);
    }

    return res;
}

//==============================================================================

} // namespace CVODESolver
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
