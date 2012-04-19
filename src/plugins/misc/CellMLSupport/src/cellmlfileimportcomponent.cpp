//==============================================================================
// CellML file import component
//==============================================================================

#include "cellmlfileimportcomponent.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileImportComponent::CellmlFileImportComponent(const QString &pCmetaId,
                                                     const QString &pName,
                                                     const QString &pReferenceName) :
    CellmlFileComponent(pCmetaId, pName),
    mReferenceName(pReferenceName)
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
