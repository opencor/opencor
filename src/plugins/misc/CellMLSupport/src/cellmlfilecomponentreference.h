//==============================================================================
// CellML file component reference
//==============================================================================

#ifndef CELLMLFILECOMPONENTREFERENCE_H
#define CELLMLFILECOMPONENTREFERENCE_H

//==============================================================================

#include "cellmlfileelement.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QList>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileComponentReference;

//==============================================================================

typedef QList<CellmlFileComponentReference *> CellmlFileComponentReferences;

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileComponentReference : public CellmlFileElement
{
public:
    explicit CellmlFileComponentReference(CellmlFile *pCellmlFile,
                                          iface::cellml_api::ComponentRef *pComponentReference);
    ~CellmlFileComponentReference();

    QString component() const;
    CellmlFileComponentReferences componentReferences() const;

private:
    QString mComponent;
    CellmlFileComponentReferences mComponentReferences;
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
