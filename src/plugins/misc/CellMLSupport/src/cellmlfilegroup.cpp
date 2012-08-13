//==============================================================================
// CellML file group
//==============================================================================

#include "cellmlfilegroup.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileGroup::CellmlFileGroup(CellmlFile *pCellmlFile,
                                 iface::cellml_api::Group *pCellmlApiGroup) :
    CellmlFileElement(pCellmlFile, pCellmlApiGroup),
    mRelationshipReferences(CellmlFileRelationshipReferences())
{
    // Iterate through the relationship references and add them to our list

    ObjRef<iface::cellml_api::RelationshipRefSet> relationshipReferences = pCellmlApiGroup->relationshipRefs();
    ObjRef<iface::cellml_api::RelationshipRefIterator> relationshipReferenceIterator = relationshipReferences->iterateRelationshipRefs();

    forever {
        iface::cellml_api::RelationshipRef *relationshipReference = relationshipReferenceIterator->nextRelationshipRef();

        if (!relationshipReference)
            break;

        mRelationshipReferences << new CellmlFileRelationshipReference(pCellmlFile, relationshipReference);
    }

    // Iterate through the component references and add them to our list

    ObjRef<iface::cellml_api::ComponentRefSet> componentReferences = pCellmlApiGroup->componentRefs();
    ObjRef<iface::cellml_api::ComponentRefIterator> componentReferenceIterator = componentReferences->iterateComponentRefs();

    forever {
        iface::cellml_api::ComponentRef *componentReference = componentReferenceIterator->nextComponentRef();

        if (!componentReference)
            break;

        mComponentReferences << new CellmlFileComponentReference(pCellmlFile, componentReference);
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
