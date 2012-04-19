//==============================================================================
// CellML file import unit
//==============================================================================

#include "cellmlfileimportunit.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileImportUnit::CellmlFileImportUnit(const QString &pCmetaId,
                                           const QString &pName,
                                           const QString &pReferenceName) :
    CellmlFileUnit(pCmetaId, pName),
    mReferenceName(pReferenceName)
{
}

//==============================================================================

QString CellmlFileImportUnit::referenceName() const
{
    // Return the import unit's reference name

    return mReferenceName;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
