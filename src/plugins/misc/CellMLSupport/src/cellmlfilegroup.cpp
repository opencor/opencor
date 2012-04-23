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
    // Iterate through the relationship ref(erence)s and add them to our list

    iface::cellml_api::RelationshipRefIterator *relationshipRefIterator = pGroup->relationshipRefs()->iterateRelationshipRefs();
    iface::cellml_api::RelationshipRef *relationshipRef;

    while ((relationshipRef = relationshipRefIterator->nextRelationshipRef()))
        // We have a relationship ref(erence), so add it to our list

        mRelationshipRefs.append(new CellmlFileRelationshipRef(relationshipRef));

    // Iterate through the component ref(erence)s and add them to our list

    iface::cellml_api::ComponentRefIterator *componentRefIterator = pGroup->componentRefs()->iterateComponentRefs();
    iface::cellml_api::ComponentRef *componentRef;

    while ((componentRef = componentRefIterator->nextComponentRef()))
        // We have a component ref(erence), so add it to our list

        mComponentRefs.append(new CellmlFileComponentRef(componentRef));
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

CellmlFileComponentRefs CellmlFileGroup::componentRefs() const
{
    // Return the group's component ref(erence)s

    return mComponentRefs;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
