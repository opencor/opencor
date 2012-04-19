//==============================================================================
// CellML file import units
//==============================================================================

#include "cellmlfileimportunits.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileImportUnits::CellmlFileImportUnits(const QString &pCmetaId,
                                             const QString &pName,
                                             const QString &pReferenceName) :
    CellmlFileUnits(pCmetaId, pName),
    mReferenceName(pReferenceName)
{
}

//==============================================================================

QString CellmlFileImportUnits::referenceName() const
{
    // Return the import units' reference name

    return mReferenceName;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
