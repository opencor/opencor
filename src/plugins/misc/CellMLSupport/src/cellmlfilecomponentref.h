//==============================================================================
// CellML file component ref(erence)
//==============================================================================

#ifndef CELLMLFILECOMPONENTREF_H
#define CELLMLFILECOMPONENTREF_H

//==============================================================================

#include "cellmlfileelement.h"
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

class CELLMLSUPPORT_EXPORT CellmlFileComponentRef : public CellmlFileElement
{
public:
    explicit CellmlFileComponentRef(iface::cellml_api::ComponentRef *pComponentRef);
    ~CellmlFileComponentRef();

    QString component() const;
    CellmlFileComponentRefs componentRefs() const;

private:
    QString mComponent;
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
