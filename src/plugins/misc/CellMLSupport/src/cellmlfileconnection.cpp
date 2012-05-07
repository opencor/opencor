//==============================================================================
// CellML file connection
//==============================================================================

#include "cellmlfileconnection.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileConnection::CellmlFileConnection(iface::cellml_api::Connection *pConnection) :
    CellmlFileElement(pConnection),
    mVariableMappings(CellmlFileMapVariables())
{
    // Retrieve the components to map

    mComponentMapping = new CellmlFileMapComponents(pConnection->componentMapping());

    // Iterate through the variables to map and add them to our list

    ObjRef<iface::cellml_api::MapVariablesSet> variableMappings = pConnection->variableMappings();
    ObjRef<iface::cellml_api::MapVariablesIterator> mapVariablesIterator = variableMappings->iterateMapVariables();

    forever {
        ObjRef<iface::cellml_api::MapVariables> mapVariables = mapVariablesIterator->nextMapVariables();

        if (mapVariables)
            // We have variables to map, so add them to our list

            mVariableMappings.append(new CellmlFileMapVariablesItem(mapVariables));
        else
            // No more variables to map, so...

            break;
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
