//==============================================================================
// CellML model runtime class
//==============================================================================

#ifndef CELLMLMODELRUNTIME_H
#define CELLMLMODELRUNTIME_H

//==============================================================================

#include "cellmlmodelcommon.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QList>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlModelRuntime
{
    friend class CellmlModel;

public:
    bool isValid();

    QList<CellmlModelIssue> issues();

private:
    void reset();

    QList<CellmlModelIssue> mIssues;
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
