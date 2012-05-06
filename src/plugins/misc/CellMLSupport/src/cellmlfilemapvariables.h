//==============================================================================
// CellML file map variables
//==============================================================================

#ifndef CELLMLFILEMAPVARIABLES_H
#define CELLMLFILEMAPVARIABLES_H

//==============================================================================

#include "cellmlfileelement.h"
#include "cellmlsupportglobal.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileMapVariablesItem : public CellmlFileElement
{
public:
    explicit CellmlFileMapVariablesItem(iface::cellml_api::MapVariables *pMapVariables);

    QString firstVariable() const;
    QString secondVariable() const;

private:
    QString mFirstVariable;
    QString mSecondVariable;
};

//==============================================================================

typedef QList<CellmlFileMapVariablesItem *> CellmlFileMapVariables;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
