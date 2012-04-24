//==============================================================================
// CellML file component
//==============================================================================

#include "cellmlfilecomponent.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileComponent::CellmlFileComponent(iface::cellml_api::ImportComponent *pImportComponent) :
    CellmlFileNamedElement(pImportComponent)
{
}

//==============================================================================

CellmlFileComponent::CellmlFileComponent(iface::cellml_api::CellMLComponent *pComponent) :
    CellmlFileNamedElement(pComponent)
{
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
