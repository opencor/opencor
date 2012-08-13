//==============================================================================
// CellML file group
//==============================================================================

#ifndef CELLMLFILEGROUP_H
#define CELLMLFILEGROUP_H

//==============================================================================

#include "cellmlfilecomponentreference.h"
#include "cellmlfileelement.h"
#include "cellmlfilerelationshipreference.h"
#include "cellmlsupportglobal.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileGroup : public CellmlFileElement
{
public:
    explicit CellmlFileGroup(CellmlFile *pCellmlFile,
                             iface::cellml_api::Group *pCellmlApiGroup);
    ~CellmlFileGroup();

    CellmlFileRelationshipReferences relationshipReferences() const;
    CellmlFileComponentReferences componentReferences() const;

private:
    CellmlFileRelationshipReferences mRelationshipReferences;
    CellmlFileComponentReferences mComponentReferences;
};

//==============================================================================

typedef QList<CellmlFileGroup *> CellmlFileGroups;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
