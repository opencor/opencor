//==============================================================================
// CellML file relationship ref(erence)
//==============================================================================

#include "cellmlfilerelationshipref.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileRelationshipRef::CellmlFileRelationshipRef(iface::cellml_api::RelationshipRef *pRelationshipRef) :
    CellmlFileElement(pRelationshipRef)
{
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
