//==============================================================================
// CellML file component
//==============================================================================

#include "cellmlfilecomponent.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileComponent::CellmlFileComponent(CellmlFile *pCellmlFile,
                                         iface::cellml_api::ImportComponent *pImportComponent) :
    CellmlFileNamedElement(pCellmlFile, pImportComponent),
    mUnits(CellmlFileUnits()),
    mVariables(CellmlFileVariables())
{
}

//==============================================================================

CellmlFileComponent::CellmlFileComponent(CellmlFile *pCellmlFile,
                                         iface::cellml_api::CellMLComponent *pComponent) :
    CellmlFileNamedElement(pCellmlFile, pComponent),
    mUnits(CellmlFileUnits()),
    mVariables(CellmlFileVariables())
{
    // Iterate through the units and add them to our list

    ObjRef<iface::cellml_api::UnitsSet> units = pComponent->units();
    ObjRef<iface::cellml_api::UnitsIterator> unitIterator = units->iterateUnits();

    forever {
        ObjRef<iface::cellml_api::Units> unit = unitIterator->nextUnits();

        if (unit)
            // We have a unit, so add it to our list

            mUnits << new CellmlFileUnit(pCellmlFile, unit);
        else
            // No more units, so...

            break;
    }

    // Iterate through the variables and add them to our list

    ObjRef<iface::cellml_api::CellMLVariableSet> variables = pComponent->variables();
    ObjRef<iface::cellml_api::CellMLVariableIterator> variableIterator = variables->iterateVariables();

    forever {
        ObjRef<iface::cellml_api::CellMLVariable> variable = variableIterator->nextVariable();

        if (variable)
            // We have a variable, so add it to our list

            mVariables << new CellmlFileVariable(pCellmlFile, variable);
        else
            // No more variables, so...

            break;
    }

    // Iterate through the MathML elements and add them to our list

    ObjRef<iface::cellml_api::MathList> mathmlElements = pComponent->math();
    ObjRef<iface::cellml_api::MathMLElementIterator> mathmlElementIterator = mathmlElements->iterate();

    forever {
        ObjRef<iface::mathml_dom::MathMLElement> mathmlElement = mathmlElementIterator->next();

        if (mathmlElement)
            // We have a MathML element, so add it to our list

            mMathmlElements << new CellmlFileMathmlElement(mathmlElement);
        else
            // No more MathML elements, so...

            break;
    }
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

CellmlFileUnits * CellmlFileComponent::units()
{
    // Return the component's units

    return &mUnits;
}

//==============================================================================

CellmlFileVariables * CellmlFileComponent::variables()
{
    // Return the component's variables

    return &mVariables;
}

//==============================================================================

CellmlFileMathmlElements * CellmlFileComponent::mathmlElements()
{
    // Return the component's MathML elements

    return &mMathmlElements;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
