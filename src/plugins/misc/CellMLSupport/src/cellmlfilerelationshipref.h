//==============================================================================
// CellML file relationship ref(erence)
//==============================================================================

#ifndef CELLMLFILERELATIONSHIPREF_H
#define CELLMLFILERELATIONSHIPREF_H

//==============================================================================

#include "cellmlfileelement.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QList>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileRelationshipRef : public CellmlFileElement
{
public:
    explicit CellmlFileRelationshipRef(iface::cellml_api::RelationshipRef *pRelationshipRef);
};

//==============================================================================

typedef QList<CellmlFileRelationshipRef *> CellmlFileRelationshipRefs;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
