//==============================================================================
// CellML file import component
//==============================================================================

#include "cellmlfileimportcomponent.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileImportComponent::CellmlFileImportComponent(iface::cellml_api::ImportComponent *pImportComponent) :
    CellmlFileComponent(pImportComponent),
    mReferenceName(QString::fromStdWString(pImportComponent->componentRef()))
{
}

//==============================================================================

QString CellmlFileImportComponent::referenceName() const
{
    // Return the import component's reference name

    return mReferenceName;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
