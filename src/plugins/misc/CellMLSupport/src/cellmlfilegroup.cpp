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

    ObjRef<iface::cellml_api::RelationshipRefSet> relationshipRefs = pGroup->relationshipRefs();
    ObjRef<iface::cellml_api::RelationshipRefIterator> relationshipRefIterator = relationshipRefs->iterateRelationshipRefs();

    while (true) {
        ObjRef<iface::cellml_api::RelationshipRef> relationshipRef = relationshipRefIterator->nextRelationshipRef();

        if (relationshipRef)
            // We have a relationship ref(erence), so add it to our list

            mRelationshipRefs.append(new CellmlFileRelationshipRef(relationshipRef));
        else
            // No more relationship ref(erence)s, so...

            break;
    }

    // Iterate through the component ref(erence)s and add them to our list

    ObjRef<iface::cellml_api::ComponentRefSet> componentRefs = pGroup->componentRefs();
    ObjRef<iface::cellml_api::ComponentRefIterator> componentRefIterator = componentRefs->iterateComponentRefs();

    while (true) {
        ObjRef<iface::cellml_api::ComponentRef> componentRef = componentRefIterator->nextComponentRef();

        if (componentRef)
            // We have a component ref(erence), so add it to our list

            mComponentRefs.append(new CellmlFileComponentRef(componentRef));
        else
            // No more component ref(erence)s, so...

            break;
    }
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
