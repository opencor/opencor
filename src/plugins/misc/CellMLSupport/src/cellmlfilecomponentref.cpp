//==============================================================================
// CellML file component ref(erence)
//==============================================================================

#include "cellmlfilecomponentref.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileComponentRef::CellmlFileComponentRef(iface::cellml_api::ComponentRef *pComponentRef) :
    CellmlFileElement(pComponentRef),
    mComponent(QString::fromStdWString(pComponentRef->componentName())),
    mComponentRefs(CellmlFileComponentRefs())
{
    // Iterate through the component ref(erence)s and add them to our list

    ObjRef<iface::cellml_api::ComponentRefSet> componentRefs = pComponentRef->componentRefs();
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

CellmlFileComponentRef::~CellmlFileComponentRef()
{
    // Delete some internal objects

    foreach (CellmlFileComponentRef *componentRef, mComponentRefs)
        delete componentRef;
}

//==============================================================================

QString CellmlFileComponentRef::component() const
{
    // Return the component ref(erence)'s component

    return mComponent;
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
