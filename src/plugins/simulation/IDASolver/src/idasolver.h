//==============================================================================
// IDA solver class
//==============================================================================

#ifndef IDASOLVER_H
#define IDASOLVER_H

//==============================================================================

#include "coredaesolver.h"

//==============================================================================

#include "nvector/nvector_serial.h"

//==============================================================================

namespace OpenCOR {
namespace IDASolver {

//==============================================================================

static const QString MaximumStepProperty = "Maximum step";
static const QString MaximumNumberOfStepsProperty = "Maximum number of steps";
static const QString RelativeToleranceProperty = "Relative tolerance";
static const QString AbsoluteToleranceProperty = "Absolute tolerance";

//==============================================================================

// Default IDA parameter values
// Note #1: a maximum step of 0 means that there is no maximum step as such and
//          that IDA can use whatever step it sees fit...
// Note #2: IDA's default maximum number of steps is 500 which ought to be big
//          enough in most cases...

static const double DefaultMaximumStep = 0.0;
static const int DefaultMaximumNumberOfSteps = 500;
static const double DefaultRelativeTolerance = 1.0e-7;
static const double DefaultAbsoluteTolerance = 1.0e-7;

//==============================================================================

struct IdaSolverUserData
{
    double *constants;
    double *algebraic;
    double *condVar;

    OpenCOR::CoreSolver::CoreDaeSolver::ComputeEssentialVariablesFunction computeEssentialVariables;
    OpenCOR::CoreSolver::CoreDaeSolver::ComputeResidualsFunction computeResiduals;
    OpenCOR::CoreSolver::CoreDaeSolver::ComputeRootInformationFunction computeRootInformation;
};

//==============================================================================

class IdaSolver : public CoreSolver::CoreDaeSolver
{
public:
    explicit IdaSolver();
    ~IdaSolver();

    virtual void initialize(const double &pVoiStart, const double &pVoiEnd,
                            const int &pStatesCount, const int &pCondVarCount,
                            double *pConstants, double *pRates, double *pStates,
                            double *pAlgebraic, double *pCondVar,
                            ComputeEssentialVariablesFunction pComputeEssentialVariables,
                            ComputeResidualsFunction pComputeResiduals,
                            ComputeRootInformationFunction pComputeRootInformation,
                            ComputeStateInformationFunction pComputeStateInformation);

    virtual void solve(double &pVoi, const double &pVoiEnd) const;

private:
    void *mSolver;
    N_Vector mStatesVector;
    N_Vector mRatesVector;
    IdaSolverUserData mUserData;

    double mMaximumStep;
    int mMaximumNumberOfSteps;
    double mRelativeTolerance;
    double mAbsoluteTolerance;

    virtual bool isValidProperty(const QString &pName) const;
};

//==============================================================================

}   // namespace IDASolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
