//==============================================================================
// CellML file relationship reference
//==============================================================================

#ifndef CELLMLFILERELATIONSHIPREFERENCE_H
#define CELLMLFILERELATIONSHIPREFERENCE_H

//==============================================================================

#include "cellmlfileelement.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QList>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileRelationshipReference : public CellmlFileElement
{
public:
    explicit CellmlFileRelationshipReference(CellmlFile *pCellmlFile,
                                             iface::cellml_api::RelationshipRef *pCellmlApiRelationshipReference);

    QString relationship() const;
    QString relationshipNamespace() const;

private:
    QString mRelationship;
    QString mRelationshipNamespace;
};

//==============================================================================

typedef QList<CellmlFileRelationshipReference *> CellmlFileRelationshipReferences;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
