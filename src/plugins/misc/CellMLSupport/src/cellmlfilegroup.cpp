//==============================================================================
// CellML file group
//==============================================================================

#include "cellmlfilegroup.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileGroup::CellmlFileGroup(iface::cellml_api::Group *pGroup) :
    CellmlFileElement(pGroup),
    mRelationshipRefs(CellmlFileRelationshipRefs())
{
    // Iterate through the unit elements and add them to our list

    iface::cellml_api::RelationshipRefIterator *relationshipRefIterator = pGroup->relationshipRefs()->iterateRelationshipRefs();
    iface::cellml_api::RelationshipRef *relationshipRef;

    while ((relationshipRef = relationshipRefIterator->nextRelationshipRef()))
        // We have a relationship ref(erence), so add it to our list

        mRelationshipRefs.append(new CellmlFileRelationshipRef(relationshipRef));
}

//==============================================================================

CellmlFileGroup::~CellmlFileGroup()
{
    // Delete some internal objects

    foreach (CellmlFileRelationshipRef *relationshipRef, mRelationshipRefs)
        delete relationshipRef;
}

//==============================================================================

CellmlFileRelationshipRefs CellmlFileGroup::relationshipRefs() const
{
    // Return the group's relationship ref(erence)s

    return mRelationshipRefs;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
