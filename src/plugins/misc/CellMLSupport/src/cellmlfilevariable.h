//==============================================================================
// CellML file variable
//==============================================================================

#ifndef CELLMLFILEVARIABLE_H
#define CELLMLFILEVARIABLE_H

//==============================================================================

#include "cellmlfilenamedelement.h"
#include "cellmlsupportglobal.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileVariable : public CellmlFileNamedElement
{
public:
    explicit CellmlFileVariable(iface::cellml_api::CellMLVariable *pVariable);
};

//==============================================================================

typedef QList<CellmlFileVariable *> CellmlFileVariables;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
