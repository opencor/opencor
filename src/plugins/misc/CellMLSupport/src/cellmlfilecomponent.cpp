//==============================================================================
// CellML file component
//==============================================================================

#include "cellmlfilecomponent.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileComponent::CellmlFileComponent(CellmlFile *pCellmlFile,
                                         iface::cellml_api::ImportComponent *pCellmlApiImportComponent) :
    CellmlFileNamedElement(pCellmlFile, pCellmlApiImportComponent),
    mUnits(CellmlFileUnits()),
    mVariables(CellmlFileVariables())
{
}

//==============================================================================

CellmlFileComponent::CellmlFileComponent(CellmlFile *pCellmlFile,
                                         iface::cellml_api::CellMLComponent *pCellmlApiComponent) :
    CellmlFileNamedElement(pCellmlFile, pCellmlApiComponent),
    mUnits(CellmlFileUnits()),
    mVariables(CellmlFileVariables())
{
    // Iterate through the units and add them to our list

    ObjRef<iface::cellml_api::UnitsSet> units = pCellmlApiComponent->units();
    ObjRef<iface::cellml_api::UnitsIterator> unitIterator = units->iterateUnits();

    forever {
        iface::cellml_api::Units *unit = unitIterator->nextUnits();

        if (!unit)
            break;

        mUnits << new CellmlFileUnit(pCellmlFile, unit);
    }

    // Iterate through the variables and add them to our list

    ObjRef<iface::cellml_api::CellMLVariableSet> variables = pCellmlApiComponent->variables();
    ObjRef<iface::cellml_api::CellMLVariableIterator> variableIterator = variables->iterateVariables();

    forever {
        iface::cellml_api::CellMLVariable *variable = variableIterator->nextVariable();

        if (!variable)
            break;

        mVariables << new CellmlFileVariable(pCellmlFile, variable);
    }

    // Iterate through the MathML elements and add them to our list

    ObjRef<iface::cellml_api::MathList> mathmlElements = pCellmlApiComponent->math();
    ObjRef<iface::cellml_api::MathMLElementIterator> mathmlElementIterator = mathmlElements->iterate();

    forever {
        ObjRef<iface::mathml_dom::MathMLElement> mathmlElement = mathmlElementIterator->next();
        // Note: CellmlFileMathmlElement isn't going take ownership of
        //       mathmlElement, hence we declare it using ObjRef<> unlike unit
        //       and variable above...

        if (!mathmlElement)
            break;

        mMathmlElements << new CellmlFileMathmlElement(mathmlElement);
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

CellmlFileVariable * CellmlFileComponent::variable(const QString &pVariableName)
{
    foreach (CellmlFileVariable *variable, mVariables)
        if (!variable->name().compare(pVariableName))
            // We have found the variable we are after

            return variable;

    // The variable we are after couldn't be found, so...

    return 0;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
