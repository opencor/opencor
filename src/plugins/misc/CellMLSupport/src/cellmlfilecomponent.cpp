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
    mUnits(CellmlFileUnits()),
    mVariables(CellmlFileVariables())
{
}

//==============================================================================

CellmlFileComponent::CellmlFileComponent(iface::cellml_api::CellMLComponent *pComponent) :
    CellmlFileNamedElement(pComponent),
    mUnits(CellmlFileUnits()),
    mVariables(CellmlFileVariables())
{
    // Iterate through the units and add them to our list

    iface::cellml_api::UnitsIterator *unitsIterator = pComponent->units()->iterateUnits();
    iface::cellml_api::Units *units;

    while ((units = unitsIterator->nextUnits()))
        // We have a unit, so add it to our list

        mUnits.append(new CellmlFileUnit(units));

    // Iterate through the variables and add them to our list

    iface::cellml_api::CellMLVariableIterator *variableIterator = pComponent->variables()->iterateVariables();
    iface::cellml_api::CellMLVariable *variable;

    while ((variable = variableIterator->nextVariable()))
        // We have a variable, so add it to our list

        mVariables.append(new CellmlFileVariable(variable));
}

//==============================================================================

CellmlFileComponent::~CellmlFileComponent()
{
    // Delete some internal objects

    clearUnits();
    clearVariables();
}

//==============================================================================

CellmlFileUnits CellmlFileComponent::units() const
{
    // Return the component's units

    return mUnits;
}

//==============================================================================

CellmlFileVariables CellmlFileComponent::variables() const
{
    // Return the component's variables

    return mVariables;
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

void CellmlFileComponent::clearVariables()
{
    // Delete all the variables and clear our list

    foreach (CellmlFileVariable *variable, mVariables)
        delete variable;

    mVariables.clear();
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
