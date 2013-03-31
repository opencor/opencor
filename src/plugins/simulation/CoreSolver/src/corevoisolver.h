//==============================================================================
// Core VOI solver class
//==============================================================================

#ifndef COREVOISOLVER_H
#define COREVOISOLVER_H

//==============================================================================

#include "coresolver.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

class CORESOLVER_EXPORT CoreVoiSolver : public CoreSolver
{
public:
    explicit CoreVoiSolver();

    virtual void solve(double &pVoi, const double &pVoiEnd) const = 0;

protected:
    int mStatesCount;

    double *mConstants;
    double *mStates;
    double *mRates;
    double *mAlgebraic;
};

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
