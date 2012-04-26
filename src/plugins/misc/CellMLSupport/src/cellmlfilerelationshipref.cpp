//==============================================================================
// CellML file relationship ref(erence)
//==============================================================================

#include "cellmlfilerelationshipref.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileRelationshipRef::CellmlFileRelationshipRef(iface::cellml_api::RelationshipRef *pRelationshipRef) :
    CellmlFileElement(pRelationshipRef),
    mRelationship(QString::fromStdWString(pRelationshipRef->relationship())),
    mRelationshipNamespace(QString::fromStdWString(pRelationshipRef->relationshipNamespace()))
{
}

//==============================================================================

QString CellmlFileRelationshipRef::relationship() const
{
    // Return the relationship ref(erence)'s relationship

    return mRelationship;
}

//==============================================================================

QString CellmlFileRelationshipRef::relationshipNamespace() const
{
    // Return the relationship ref(erence)'s relationship namespace

    return mRelationshipNamespace;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
