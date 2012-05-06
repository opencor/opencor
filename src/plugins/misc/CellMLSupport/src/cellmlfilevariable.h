//==============================================================================
// CellML file variable
//==============================================================================

#ifndef CELLMLFILEVARIABLE_H
#define CELLMLFILEVARIABLE_H

//==============================================================================

#include "cellmlfilenamedelement.h"
#include "cellmlsupportglobal.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileVariable : public CellmlFileNamedElement
{
public:
    enum CellmlFileVariableInterface {
        None,
        In,
        Out
    };

    explicit CellmlFileVariable(iface::cellml_api::CellMLVariable *pVariable);

    QString unit() const;
    QString initialValue() const;
    CellmlFileVariableInterface publicInterface() const;
    CellmlFileVariableInterface privateInterface() const;

    QString publicInterfaceAsString() const;
    QString privateInterfaceAsString() const;

private:
    QString mUnit;
    QString mInitialValue;
    CellmlFileVariableInterface mPublicInterface;
    CellmlFileVariableInterface mPrivateInterface;

    void retrieveInterfaceType(CellmlFileVariableInterface &pInterface,
                               const iface::cellml_api::VariableInterface &pVariableInterface);

    QString interfaceAsString(const CellmlFileVariableInterface &pInterface) const;
};

//==============================================================================

typedef QList<CellmlFileVariable *> CellmlFileVariables;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
