//==============================================================================
// CellML file component ref(erence)
//==============================================================================

#include "cellmlfilecomponentref.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileComponentRef::CellmlFileComponentRef(iface::cellml_api::ComponentRef *pComponentRef) :
    CellmlFileNamedElement(pComponentRef),
    mComponentRefs(CellmlFileComponentRefs())
{
    // Iterate through the component ref(erence)s and add them to our list

    iface::cellml_api::ComponentRefIterator *componentRefIterator = pComponentRef->componentRefs()->iterateComponentRefs();
    iface::cellml_api::ComponentRef *componentRef;

    while ((componentRef = componentRefIterator->nextComponentRef()))
        // We have a component ref(erence), so add it to our list

        mComponentRefs.append(new CellmlFileComponentRef(componentRef));
}

//==============================================================================

CellmlFileComponentRef::~CellmlFileComponentRef()
{
    // Delete some internal objects

    foreach (CellmlFileComponentRef *componentRef, mComponentRefs)
        delete componentRef;
}

//==============================================================================

CellmlFileComponentRefs CellmlFileComponentRef::componentRefs() const
{
    // Return the component ref(erence)'s list of component ref(erence)s

    return mComponentRefs;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
