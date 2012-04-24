//==============================================================================
// CellML file variable
//==============================================================================

#include "cellmlfilevariable.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileVariable::CellmlFileVariable(iface::cellml_api::CellMLVariable *pVariable) :
    CellmlFileNamedElement(pVariable),
    mUnit(QString::fromStdWString(pVariable->unitsName())),
    mInitialValue(QString::fromStdWString(pVariable->initialValue()))
{
    // Retrieve the type of public and private interfaces the variable has

    retrieveInterfaceType(mPublicInterface, pVariable->publicInterface());
    retrieveInterfaceType(mPrivateInterface, pVariable->privateInterface());
}

//==============================================================================

void CellmlFileVariable::retrieveInterfaceType(CellmlFileVariableInterface &pInterface,
                                               const iface::cellml_api::VariableInterface &pVariableInterface)
{
    // Retrieve the type of interface we are dealing with

    switch (pVariableInterface) {
    case iface::cellml_api::INTERFACE_IN:
        pInterface = In;
    case iface::cellml_api::INTERFACE_OUT:
        pInterface = Out;
    default:
        pInterface = None;
    }
}

//==============================================================================

QString CellmlFileVariable::unit() const
{
    // Return the variable's unit

    return mUnit;
}

//==============================================================================

QString CellmlFileVariable::initialValue() const
{
    // Return the variable's initial value

    return mInitialValue;
}

//==============================================================================

CellmlFileVariable::CellmlFileVariableInterface CellmlFileVariable::publicInterface() const
{
    // Return the variable's public interface

    return mPublicInterface;
}

//==============================================================================

CellmlFileVariable::CellmlFileVariableInterface CellmlFileVariable::privateInterface() const
{
    // Return the variable's private interface

    return mPrivateInterface;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
