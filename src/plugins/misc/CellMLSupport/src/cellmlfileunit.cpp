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
    mBaseUnit(false),
    mUnitElements(CellmlFileUnitElements())
{
}

//==============================================================================

CellmlFileUnit::CellmlFileUnit(iface::cellml_api::Units *pUnits) :
    CellmlFileNamedElement(pUnits),
    mBaseUnit(pUnits->isBaseUnits()),
    mUnitElements(CellmlFileUnitElements())
{
    // Iterate through the unit elements and add them to our list

    ObjRef<iface::cellml_api::UnitSet> units = pUnits->unitCollection();
    ObjRef<iface::cellml_api::UnitIterator> unitIterator = units->iterateUnits();

    forever {
        ObjRef<iface::cellml_api::Unit> unit = unitIterator->nextUnit();

        if (unit)
            // We have a unit, so add it to our list

            mUnitElements.append(new CellmlFileUnitElement(unit));
        else
            // No more units, so...

            break;
    }
}

//==============================================================================

CellmlFileUnit::~CellmlFileUnit()
{
    // Delete some internal objects

    foreach (CellmlFileUnitElement *unitElement, mUnitElements)
        delete unitElement;
}

//==============================================================================

bool CellmlFileUnit::isBaseUnit() const
{
    // Return whether the unit is a base unit

    return mBaseUnit;
}

//==============================================================================

CellmlFileUnitElements CellmlFileUnit::unitElements() const
{
    // Return the unit's unit elements

    return mUnitElements;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
