//==============================================================================
// CellML file map variables
//==============================================================================

#include "cellmlfilemapvariables.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileMapVariablesItem::CellmlFileMapVariablesItem(CellmlFile *pCellmlFile,
                                                       iface::cellml_api::MapVariables *pCellmlApiMapVariables) :
    CellmlFileElement(pCellmlFile, pCellmlApiMapVariables),
    mFirstVariable(QString::fromStdWString(pCellmlApiMapVariables->firstVariableName())),
    mSecondVariable(QString::fromStdWString(pCellmlApiMapVariables->secondVariableName()))
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
