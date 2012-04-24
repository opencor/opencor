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

    // Iterate through the MathML elements and add them to our list

    iface::cellml_api::MathMLElementIterator *mathmlElementIterator = pComponent->math()->iterate();
    iface::mathml_dom::MathMLElement *mathmlElement;

    while ((mathmlElement = mathmlElementIterator->next()))
        // We have a MathML element, so add it to our list

        mMathmlElements.append(new CellmlFileMathmlElement(mathmlElement));
}

//==============================================================================

CellmlFileComponent::~CellmlFileComponent()
{
    // Delete some internal objects

    foreach (CellmlFileUnit *unit, mUnits)
        delete unit;

    foreach (CellmlFileVariable *variable, mVariables)
        delete variable;

    foreach (CellmlFileMathmlElement *mathmlElement, mMathmlElements)
        delete mathmlElement;
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

CellmlFileMathmlElements CellmlFileComponent::mathmlElements() const
{
    // Return the component's MathML elements

    return mMathmlElements;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
