//==============================================================================
// CellML file unit
//==============================================================================

#include "cellmlfileunit.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileUnit::CellmlFileUnit(iface::cellml_api::ImportUnits *pImportUnits) :
    CellmlFileNamedElement(pImportUnits),
    mBaseUnit(false)
{
}

//==============================================================================

CellmlFileUnit::CellmlFileUnit(iface::cellml_api::Units *pUnits) :
    CellmlFileNamedElement(pUnits),
    mBaseUnit(pUnits->isBaseUnits())
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
