//==============================================================================
// CellML file component ref(erence)
//==============================================================================

#ifndef CELLMLFILECOMPONENTREF_H
#define CELLMLFILECOMPONENTREF_H

//==============================================================================

#include "cellmlfilenamedelement.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QList>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileComponentRef;

//==============================================================================

typedef QList<CellmlFileComponentRef *> CellmlFileComponentRefs;

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileComponentRef : public CellmlFileNamedElement
{
public:
    explicit CellmlFileComponentRef(iface::cellml_api::ComponentRef *pComponentRef);
    ~CellmlFileComponentRef();

    CellmlFileComponentRefs componentRefs() const;

private:
    CellmlFileComponentRefs mComponentRefs;
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
