//==============================================================================
// CVODE solver class
//==============================================================================

#include "cvodesolver.h"

//==============================================================================

namespace OpenCOR {
namespace CVODESolver {

//==============================================================================

// Default CVODE parameter values
// Note: a maximum step of 0 means that there is no maximum step as such and
//       that CVODE can use whatever step it sees fit...

static const double DefaultMaximumStep = 0;
static const double DefaultRelativeTolerance = 1e-6;
static const double DefaultAbsoluteTolerance = 1e-6;

//==============================================================================

CVODESolver::CVODESolver() :
    mMaximumStep(DefaultMaximumStep),
    mRelativeTolerance(DefaultRelativeTolerance),
    mAbsoluteTolerance(DefaultAbsoluteTolerance)
{
}

//==============================================================================

void CVODESolver::initialize(const int &pNbOfStates, double *pConstants,
                                    double *pRates, double *pStates,
                                    double *pAlgebraic)
{
    // Initialise the CVODE solver by first initialising the ODE solver itself

    OpenCOR::CoreSolver::CoreOdeSolver::initialize(pNbOfStates, pConstants,
                                                   pRates, pStates, pAlgebraic);

    // Next, we need to retrieve some of the CVODE properties

    mMaximumStep = mProperties.contains(MaximumStepProperty)?
                       mProperties.value(MaximumStepProperty).toDouble():
                       DefaultMaximumStep;

    mRelativeTolerance = mProperties.contains(RelativeToleranceProperty)?
                             mProperties.value(RelativeToleranceProperty).toDouble():
                             DefaultRelativeTolerance;
    mAbsoluteTolerance = mProperties.contains(AbsoluteToleranceProperty)?
                             mProperties.value(AbsoluteToleranceProperty).toDouble():
                             DefaultAbsoluteTolerance;
}

//==============================================================================

void CVODESolver::solve(double &pVoi, const double &pVoiEnd,
                        OpenCOR::CoreSolver::CoreOdeSolver::ComputeRatesFunction pComputeRates) const
{
    Q_ASSERT(mStatesCount > 0);
    Q_ASSERT(mConstants);
    Q_ASSERT(mRates);
    Q_ASSERT(mStates);
    Q_ASSERT(mAlgebraic);

    pVoi = pVoiEnd;
}

//==============================================================================

bool CVODESolver::isValidProperty(const QString &pName) const
{
    // Check whether the property name is known to us

    return    !pName.compare(MaximumStepProperty)
            || !pName.compare(RelativeToleranceProperty)
            || !pName.compare(AbsoluteToleranceProperty);
}

//==============================================================================

}   // namespace CVODESolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
