//==============================================================================
// CellML file map variables
//==============================================================================

#include "cellmlfilemapvariables.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileMapVariablesItem::CellmlFileMapVariablesItem(iface::cellml_api::MapVariables *pMapVariables) :
    CellmlFileElement(pMapVariables),
    mFirstVariable(QString::fromStdWString(pMapVariables->firstVariableName())),
    mSecondVariable(QString::fromStdWString(pMapVariables->secondVariableName()))
{
}

//==============================================================================

QString CellmlFileMapVariablesItem::firstVariable() const
{
    // Return the map variables' first variable

    return mFirstVariable;
}

//==============================================================================

QString CellmlFileMapVariablesItem::secondVariable() const
{
    // Return the map variables' second variable

    return mSecondVariable;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
