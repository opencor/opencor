//==============================================================================
// CellML file unit element
//==============================================================================

#include "cellmlfileunitelement.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileUnitElement::CellmlFileUnitElement(iface::cellml_api::Unit *pUnit) :
    CellmlFileNamedElement(pUnit),
    mPrefix(pUnit->prefix()),
    mMultiplier(pUnit->multiplier()),
    mOffset(pUnit->offset()),
    mExponent(pUnit->exponent())
{
}

//==============================================================================

int CellmlFileUnitElement::prefix() const
{
    // Return the unit element's prefix

    return mPrefix;
}

//==============================================================================

double CellmlFileUnitElement::multiplier() const
{
    // Return the unit element's multiplier

    return mMultiplier;
}

//==============================================================================

double CellmlFileUnitElement::offset() const
{
    // Return the unit element's offset

    return mOffset;
}

//==============================================================================

double CellmlFileUnitElement::exponent() const
{
    // Return the unit element's exponent

    return mExponent;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
