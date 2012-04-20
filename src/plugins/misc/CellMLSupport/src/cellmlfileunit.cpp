//==============================================================================
// CellML file unit
//==============================================================================

#include "cellmlfileunit.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileUnit::CellmlFileUnit(const QString &pCmetaId, const QString &pName,
                               const bool &pBaseUnit) :
    CellmlFileNamedElement(pCmetaId, pName),
    mBaseUnit(pBaseUnit)
{
}

//==============================================================================

bool CellmlFileUnit::isBaseUnit() const
{
    // Return whether the unit is a base unit

    return mBaseUnit;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
