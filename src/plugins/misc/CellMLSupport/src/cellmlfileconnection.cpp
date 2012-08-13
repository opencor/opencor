//==============================================================================
// CellML file connection
//==============================================================================

#include "cellmlfileconnection.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileConnection::CellmlFileConnection(CellmlFile *pCellmlFile,
                                           iface::cellml_api::Connection *pCellmlApiConnection) :
    CellmlFileElement(pCellmlFile, pCellmlApiConnection),
    mVariableMappings(CellmlFileMapVariables())
{
    // Retrieve the components to map

    mComponentMapping = new CellmlFileMapComponents(pCellmlFile, pCellmlApiConnection->componentMapping());

    // Iterate through the variables to map and add them to our list

    ObjRef<iface::cellml_api::MapVariablesSet> variableMappings = pCellmlApiConnection->variableMappings();
    ObjRef<iface::cellml_api::MapVariablesIterator> mapVariablesIterator = variableMappings->iterateMapVariables();

    forever {
        iface::cellml_api::MapVariables *mapVariables = mapVariablesIterator->nextMapVariables();

        if (!mapVariables)
            break;

        mVariableMappings << new CellmlFileMapVariablesItem(pCellmlFile, mapVariables);
    }
}

//==============================================================================

CellmlFileConnection::~CellmlFileConnection()
{
    // Delete some internal objects

    delete mComponentMapping;

    foreach (CellmlFileMapVariablesItem *mapVariablesItem, mVariableMappings)
        delete mapVariablesItem;
}

//==============================================================================

CellmlFileMapComponents * CellmlFileConnection::componentMapping() const
{
    // Return the connection's component mapping

    return mComponentMapping;
}

//==============================================================================

CellmlFileMapVariables CellmlFileConnection::variableMappings() const
{
    // Return the connection's variable mappings

    return mVariableMappings;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
