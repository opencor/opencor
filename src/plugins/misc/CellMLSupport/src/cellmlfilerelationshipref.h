//==============================================================================
// CellML file relationship ref(erence)
//==============================================================================

#ifndef CELLMLFILERELATIONSHIPREF_H
#define CELLMLFILERELATIONSHIPREF_H

//==============================================================================

#include "cellmlfilenamedelement.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QList>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileRelationshipRef : public CellmlFileNamedElement
{
public:
    explicit CellmlFileRelationshipRef(iface::cellml_api::RelationshipRef *pRelationshipRef);

    QString relationship() const;
    QString relationshipNamespace() const;

private:
    QString mRelationship;
    QString mRelationshipNamespace;
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
