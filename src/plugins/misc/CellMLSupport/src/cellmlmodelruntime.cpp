//==============================================================================
// CellML model runtime class
//==============================================================================

#include "cellmlmodelruntime.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

bool CellmlModelRuntime::isValid()
{
    // The runtime is valid if no issue was found

    return mIssues.isEmpty();
}

//==============================================================================

QList<CellmlModelIssue> CellmlModelRuntime::issues()
{
    // Return the issue(s)

    return mIssues;
}

//==============================================================================

void CellmlModelRuntime::reset()
{
    // Reset all of the runtime's properties

//---GRY--- TO BE DONE...
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
