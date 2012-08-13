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
    explicit CellmlFileImportComponent(CellmlFile *pCellmlFile,
                                       iface::cellml_api::ImportComponent *pCellmlApiImportComponent);

    QString componentReference() const;

private:
    QString mComponentReference;
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
