//==============================================================================
// CellML file import component
//==============================================================================

#include "cellmlfileimportcomponent.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileImportComponent::CellmlFileImportComponent(CellmlFile *pCellmlFile,
                                                     iface::cellml_api::ImportComponent *pImportComponent) :
    CellmlFileComponent(pCellmlFile, pImportComponent),
    mComponentReference(QString::fromStdWString(pImportComponent->componentRef()))
{
}

//==============================================================================

QString CellmlFileImportComponent::componentReference() const
{
    // Return the import component's component reference

    return mComponentReference;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
