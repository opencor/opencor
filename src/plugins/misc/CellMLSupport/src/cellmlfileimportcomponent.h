//==============================================================================
// CellML file import component
//==============================================================================

#ifndef CELLMLFILEIMPORTCOMPONENT_H
#define CELLMLFILEIMPORTCOMPONENT_H

//==============================================================================

#include "cellmlfilecomponent.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QList>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileImportComponent : public CellmlFileComponent
{
public:
    explicit CellmlFileImportComponent(iface::cellml_api::ImportComponent *pImportComponent);

    QString referenceName() const;

private:
    QString mReferenceName;
};

//==============================================================================

typedef QList<CellmlFileImportComponent *> CellmlFileImportComponents;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
