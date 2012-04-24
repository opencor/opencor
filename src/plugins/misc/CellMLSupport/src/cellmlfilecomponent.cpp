//==============================================================================
// CellML file component
//==============================================================================

#include "cellmlfilecomponent.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileComponent::CellmlFileComponent(iface::cellml_api::ImportComponent *pImportComponent) :
    CellmlFileNamedElement(pImportComponent),
    mUnits(CellmlFileUnits())
{
}

//==============================================================================

CellmlFileComponent::CellmlFileComponent(iface::cellml_api::CellMLComponent *pComponent) :
    CellmlFileNamedElement(pComponent),
    mUnits(CellmlFileUnits())
{
    // Iterate through the units and add them to our list

    iface::cellml_api::UnitsIterator *unitsIterator = pComponent->units()->iterateUnits();
    iface::cellml_api::Units *units;

    while ((units = unitsIterator->nextUnits()))
        // We have a unit, so add it to our list

        mUnits.append(new CellmlFileUnit(units));
}

//==============================================================================

CellmlFileComponent::~CellmlFileComponent()
{
    // Delete some internal objects

    clearUnits();
}

//==============================================================================

CellmlFileUnits CellmlFileComponent::units() const
{
    // Return the component's units

    return mUnits;
}

//==============================================================================

void CellmlFileComponent::clearUnits()
{
    // Delete all the units and clear our list

    foreach (CellmlFileUnit *unit, mUnits)
        delete unit;

    mUnits.clear();
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
