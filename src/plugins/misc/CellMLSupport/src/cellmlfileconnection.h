//==============================================================================
// CellML file connection
//==============================================================================

#ifndef CELLMLFILECONNECTION_H
#define CELLMLFILECONNECTION_H

//==============================================================================

#include "cellmlfileelement.h"
#include "cellmlfilemapcomponents.h"
#include "cellmlfilemapvariables.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QList>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileConnection : public CellmlFileElement
{
public:
    explicit CellmlFileConnection(CellmlFile *pCellmlFile,
                                  iface::cellml_api::Connection *pCellmlApiConnection);
    ~CellmlFileConnection();

    CellmlFileMapComponents * componentMapping() const;
    CellmlFileMapVariables variableMappings() const;

private:
    CellmlFileMapComponents *mComponentMapping;
    CellmlFileMapVariables mVariableMappings;
};

//==============================================================================

typedef QList<CellmlFileConnection *> CellmlFileConnections;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
