//==============================================================================
// CellML file component reference
//==============================================================================

#include "cellmlfilecomponentreference.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileComponentReference::CellmlFileComponentReference(iface::cellml_api::ComponentRef *pComponentReference) :
    CellmlFileElement(pComponentReference),
    mComponent(QString::fromStdWString(pComponentReference->componentName())),
    mComponentReferences(CellmlFileComponentReferences())
{
    // Iterate through the component references and add them to our list

    ObjRef<iface::cellml_api::ComponentRefSet> componentReferences = pComponentReference->componentRefs();
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

CellmlFileComponentReference::~CellmlFileComponentReference()
{
    // Delete some internal objects

    foreach (CellmlFileComponentReference *componentReference,
             mComponentReferences)
        delete componentReference;
}

//==============================================================================

QString CellmlFileComponentReference::component() const
{
    // Return the component reference's component

    return mComponent;
}

//==============================================================================

CellmlFileComponentReferences CellmlFileComponentReference::componentReferences() const
{
    // Return the component reference's list of component references

    return mComponentReferences;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
