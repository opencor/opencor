//==============================================================================
// CellML file unit
//==============================================================================

#include "cellmlfileunit.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileUnit::CellmlFileUnit(CellmlFile *pCellmlFile,
                               iface::cellml_api::ImportUnits *pCellmlApiImportUnits) :
    CellmlFileNamedElement(pCellmlFile, pCellmlApiImportUnits),
    mBaseUnit(false),
    mUnitElements(CellmlFileUnitElements())
{
}

//==============================================================================

CellmlFileUnit::CellmlFileUnit(CellmlFile *pCellmlFile,
                               iface::cellml_api::Units *pCellmlApiUnits) :
    CellmlFileNamedElement(pCellmlFile, pCellmlApiUnits),
    mBaseUnit(pCellmlApiUnits->isBaseUnits()),
    mUnitElements(CellmlFileUnitElements())
{
    // Iterate through the unit elements and add them to our list

    ObjRef<iface::cellml_api::UnitSet> units = pCellmlApiUnits->unitCollection();
    ObjRef<iface::cellml_api::UnitIterator> unitIterator = units->iterateUnits();

    forever {
        iface::cellml_api::Unit *unit = unitIterator->nextUnit();

        if (!unit)
            break;

        mUnitElements << new CellmlFileUnitElement(pCellmlFile, unit);
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
