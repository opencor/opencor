//==============================================================================
// CellML file variable
//==============================================================================

#include "cellmlfilevariable.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileVariable::CellmlFileVariable(CellmlFile *pCellmlFile,
                                       iface::cellml_api::CellMLVariable *pVariable) :
    CellmlFileNamedElement(pCellmlFile, pVariable),
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

        break;
    case iface::cellml_api::INTERFACE_OUT:
        pInterface = Out;

        break;
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

QString CellmlFileVariable::interfaceAsString(const CellmlFileVariableInterface &pInterface) const
{
    // Return a variable's interface as a string

    switch (pInterface) {
    case In:
        return "in";
    case Out:
        return "out";
    default:
        // None

        return "none";
    }
}

//==============================================================================

QString CellmlFileVariable::publicInterfaceAsString() const
{
    // Return the variable's public interface as a string

    return interfaceAsString(mPublicInterface);
}

//==============================================================================

QString CellmlFileVariable::privateInterfaceAsString() const
{
    // Return the variable's private interface as a string

    return interfaceAsString(mPrivateInterface);
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
