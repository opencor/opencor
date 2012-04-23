//==============================================================================
// CellML file group
//==============================================================================

#ifndef CELLMLFILEGROUP_H
#define CELLMLFILEGROUP_H

//==============================================================================

#include "cellmlfileelement.h"
#include "cellmlfilerelationshipref.h"
#include "cellmlsupportglobal.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileGroup : public CellmlFileElement
{
public:
    explicit CellmlFileGroup(iface::cellml_api::Group *pGroup);
    ~CellmlFileGroup();

    CellmlFileRelationshipRefs relationshipRefs() const;

private:
    CellmlFileRelationshipRefs mRelationshipRefs;
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
