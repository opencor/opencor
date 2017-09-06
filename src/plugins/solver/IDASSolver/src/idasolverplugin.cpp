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
// IDA solver plugin
//==============================================================================

#include "idasolver.h"
#include "idasolverplugin.h"

//==============================================================================

namespace OpenCOR {
namespace IDASolver {

//==============================================================================

PLUGININFO_FUNC IDASolverPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin that uses <a href=\"http://computation.llnl.gov/projects/sundials/ida\">IDA</a> to solve DAEs."));
    descriptions.insert("fr", QString::fromUtf8("une extension qui utilise <a href=\"http://computation.llnl.gov/projects/sundials/ida\">IDA</a> pour résoudre des EADs."));

    return new PluginInfo(PluginInfo::Solver, true, false,
                          QStringList() << "SUNDIALS",
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================


void IDASolverPlugin::retranslateUi()
{
    // We don't handle this interface...
    // Note: even though we don't handle this interface, we still want to
    //       support it since some other aspects of our plugin are
    //       multilingual...
}

//==============================================================================
// Solver interface
//==============================================================================


Solver::Solver * IDASolverPlugin::solverInstance() const
{
    // Create and return an instance of the solver

    return new IdaSolver();
}

//==============================================================================

QString IDASolverPlugin::id(const QString &pKisaoId) const
{
    // Return the id for the given KiSAO id

    if (!pKisaoId.compare("KISAO:0000283"))
        return solverName();
    else if (!pKisaoId.compare("KISAO:0000467"))
        return MaximumStepId;
    else if (!pKisaoId.compare("KISAO:0000415"))
        return MaximumNumberOfStepsId;
    else if (!pKisaoId.compare("KISAO:0000477"))
        return LinearSolverId;
    else if (!pKisaoId.compare("KISAO:0000479"))
        return UpperHalfBandwidthId;
    else if (!pKisaoId.compare("KISAO:0000480"))
        return LowerHalfBandwidthId;
    else if (!pKisaoId.compare("KISAO:0000209"))
        return RelativeToleranceId;
    else if (!pKisaoId.compare("KISAO:0000211"))
        return AbsoluteToleranceId;
    else if (!pKisaoId.compare("KISAO:0000481"))
        return InterpolateSolutionId;

    return QString();
}

//==============================================================================

QString IDASolverPlugin::kisaoId(const QString &pId) const
{
    // Return the KiSAO id for the given id

    if (!pId.compare(solverName()))
        return "KISAO:0000283";
    else if (!pId.compare(MaximumStepId))
        return "KISAO:0000467";
    else if (!pId.compare(MaximumNumberOfStepsId))
        return "KISAO:0000415";
    else if (!pId.compare(LinearSolverId))
        return "KISAO:0000477";
    else if (!pId.compare(UpperHalfBandwidthId))
        return "KISAO:0000479";
    else if (!pId.compare(LowerHalfBandwidthId))
        return "KISAO:0000480";
    else if (!pId.compare(RelativeToleranceId))
        return "KISAO:0000209";
    else if (!pId.compare(AbsoluteToleranceId))
        return "KISAO:0000211";
    else if (!pId.compare(InterpolateSolutionId))
        return "KISAO:0000481";

    return QString();
}

//==============================================================================

Solver::Type IDASolverPlugin::solverType() const
{
    // Return the type of the solver

    return Solver::Dae;
}

//==============================================================================

QString IDASolverPlugin::solverName() const
{
    // Return the name of the solver

    return "IDA";
}

//==============================================================================

Solver::Properties IDASolverPlugin::solverProperties() const
{
    // Return the properties supported by the solver

    Descriptions MaximumStepDescriptions;
    Descriptions MaximumNumberOfStepsDescriptions;
    Descriptions LinearSolverDescriptions;
    Descriptions UpperHalfBandwidthDescriptions;
    Descriptions LowerHalfBandwidthDescriptions;
    Descriptions RelativeToleranceDescriptions;
    Descriptions AbsoluteToleranceDescriptions;
    Descriptions InterpolateSolutionDescriptions;

    MaximumStepDescriptions.insert("en", QString::fromUtf8("Maximum step"));
    MaximumStepDescriptions.insert("fr", QString::fromUtf8("Pas maximum"));

    MaximumNumberOfStepsDescriptions.insert("en", QString::fromUtf8("Maximum number of steps"));
    MaximumNumberOfStepsDescriptions.insert("fr", QString::fromUtf8("Nombre maximum de pas"));

    LinearSolverDescriptions.insert("en", QString::fromUtf8("Linear solver"));
    LinearSolverDescriptions.insert("fr", QString::fromUtf8("Solveur linéaire"));

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

    QStringList LinearSolverListValues = QStringList() << DenseLinearSolver
                                                       << BandedLinearSolver
                                                       << GmresLinearSolver
                                                       << BiCgStabLinearSolver
                                                       << TfqmrLinearSolver;

    return Solver::Properties() << Solver::Property(Solver::Property::Double, MaximumStepId, MaximumStepDescriptions, QStringList(), MaximumStepDefaultValue, true)
                                << Solver::Property(Solver::Property::Integer, MaximumNumberOfStepsId, MaximumNumberOfStepsDescriptions, QStringList(), MaximumNumberOfStepsDefaultValue, false)
                                << Solver::Property(Solver::Property::List, LinearSolverId, LinearSolverDescriptions, LinearSolverListValues, LinearSolverDefaultValue, false)
                                << Solver::Property(Solver::Property::Integer, UpperHalfBandwidthId, UpperHalfBandwidthDescriptions, QStringList(), UpperHalfBandwidthDefaultValue, false)
                                << Solver::Property(Solver::Property::Integer, LowerHalfBandwidthId, LowerHalfBandwidthDescriptions, QStringList(), LowerHalfBandwidthDefaultValue, false)
                                << Solver::Property(Solver::Property::Double, RelativeToleranceId, RelativeToleranceDescriptions, QStringList(), RelativeToleranceDefaultValue, false)
                                << Solver::Property(Solver::Property::Double, AbsoluteToleranceId, AbsoluteToleranceDescriptions, QStringList(), AbsoluteToleranceDefaultValue, false)
                                << Solver::Property(Solver::Property::Boolean, InterpolateSolutionId, InterpolateSolutionDescriptions, QStringList(), InterpolateSolutionDefaultValue, false);
}

//==============================================================================

QMap<QString, bool> IDASolverPlugin::solverPropertiesVisibility(const QMap<QString, QString> &pSolverPropertiesValues) const
{
    // Return the visibility of our properties based on the given properties
    // values

    QMap<QString, bool> res = QMap<QString, bool>();

    QString linearSolver = pSolverPropertiesValues.value(LinearSolverId);

    if (!linearSolver.compare(BandedLinearSolver)) {
        // Banded linear solver

        res.insert(UpperHalfBandwidthId, true);
        res.insert(LowerHalfBandwidthId, true);
    } else {
        // Dense/GMRES/Bi-CGStab/TFQMR linear solver

        res.insert(UpperHalfBandwidthId, false);
        res.insert(LowerHalfBandwidthId, false);
    }

    return res;
}

//==============================================================================

}   // namespace IDASolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
