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
    mFirstVariableName(QString::fromStdWString(pMapVariables->firstVariableName())),
    mSecondVariableName(QString::fromStdWString(pMapVariables->secondVariableName()))
{
}

//==============================================================================

QString CellmlFileMapVariablesItem::firstVariableName() const
{
    // Return the map variables' first variable's name

    return mFirstVariableName;
}

//==============================================================================

QString CellmlFileMapVariablesItem::secondVariableName() const
{
    // Return the map variables' second variable's name

    return mSecondVariableName;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
