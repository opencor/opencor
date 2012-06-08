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
    mRelationshipReferences(CellmlFileRelationshipReferences())
{
    // Iterate through the relationship references and add them to our list

    ObjRef<iface::cellml_api::RelationshipRefSet> relationshipReferences = pGroup->relationshipRefs();
    ObjRef<iface::cellml_api::RelationshipRefIterator> relationshipReferenceIterator = relationshipReferences->iterateRelationshipRefs();

    forever {
        ObjRef<iface::cellml_api::RelationshipRef> relationshipReference = relationshipReferenceIterator->nextRelationshipRef();

        if (relationshipReference)
            // We have a relationship reference, so add it to our list

            mRelationshipReferences << new CellmlFileRelationshipReference(relationshipReference);
        else
            // No more relationship references, so...

            break;
    }

    // Iterate through the component references and add them to our list

    ObjRef<iface::cellml_api::ComponentRefSet> componentReferences = pGroup->componentRefs();
    ObjRef<iface::cellml_api::ComponentRefIterator> componentReferenceIterator = componentReferences->iterateComponentRefs();

    forever {
        ObjRef<iface::cellml_api::ComponentRef> componentReference = componentReferenceIterator->nextComponentRef();

        if (componentReference)
            // We have a component reference, so add it to our list

            mComponentReferences << new CellmlFileComponentReference(componentReference);
        else
            // No more component references, so...

            break;
    }
}

//==============================================================================

CellmlFileGroup::~CellmlFileGroup()
{
    // Delete some internal objects

    foreach (CellmlFileRelationshipReference *relationshipReference,
             mRelationshipReferences)
        delete relationshipReference;
}

//==============================================================================

CellmlFileRelationshipReferences CellmlFileGroup::relationshipReferences() const
{
    // Return the group's relationship references

    return mRelationshipReferences;
}

//==============================================================================

CellmlFileComponentReferences CellmlFileGroup::componentReferences() const
{
    // Return the group's component references

    return mComponentReferences;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
