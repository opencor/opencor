//==============================================================================
// CellML file component reference
//==============================================================================

#include "cellmlfilecomponentreference.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileComponentReference::CellmlFileComponentReference(CellmlFile *pCellmlFile,
                                                           iface::cellml_api::ComponentRef *pCellmlApiComponentReference) :
    CellmlFileElement(pCellmlFile, pCellmlApiComponentReference),
    mComponent(QString::fromStdWString(pCellmlApiComponentReference->componentName())),
    mComponentReferences(CellmlFileComponentReferences())
{
    // Iterate through the component references and add them to our list

    ObjRef<iface::cellml_api::ComponentRefSet> componentReferences = pCellmlApiComponentReference->componentRefs();
    ObjRef<iface::cellml_api::ComponentRefIterator> componentReferenceIterator = componentReferences->iterateComponentRefs();

    forever {
        iface::cellml_api::ComponentRef *componentReference = componentReferenceIterator->nextComponentRef();

        if (!componentReference)
            break;

        mComponentReferences << new CellmlFileComponentReference(pCellmlFile, componentReference);
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
