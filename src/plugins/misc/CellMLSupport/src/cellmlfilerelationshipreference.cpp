//==============================================================================
// CellML file relationship reference
//==============================================================================

#include "cellmlfilerelationshipreference.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileRelationshipReference::CellmlFileRelationshipReference(CellmlFile *pCellmlFile,
                                                                 iface::cellml_api::RelationshipRef *pRelationshipReference) :
    CellmlFileElement(pCellmlFile, pRelationshipReference),
    mRelationship(QString::fromStdWString(pRelationshipReference->relationship())),
    mRelationshipNamespace(QString::fromStdWString(pRelationshipReference->relationshipNamespace()))
{
}

//==============================================================================

QString CellmlFileRelationshipReference::relationship() const
{
    // Return the relationship reference's relationship

    return mRelationship;
}

//==============================================================================

QString CellmlFileRelationshipReference::relationshipNamespace() const
{
    // Return the relationship reference's relationship namespace

    return mRelationshipNamespace;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
