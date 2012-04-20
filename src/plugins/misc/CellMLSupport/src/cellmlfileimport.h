//==============================================================================
// CellML file import
//==============================================================================

#ifndef CELLMLFILEIMPORT_H
#define CELLMLFILEIMPORT_H

//==============================================================================

#include "cellmlfileelement.h"
#include "cellmlfileimportunit.h"
#include "cellmlfileimportcomponent.h"
#include "cellmlsupportglobal.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileImport : public CellmlFileElement
{
public:
    explicit CellmlFileImport(iface::cellml_api::CellMLImport *pCellmlImport);
    ~CellmlFileImport();

    QString uri() const;

    CellmlFileImportUnits units() const;
    CellmlFileImportComponents components() const;

private:
    QString mUri;

    CellmlFileImportUnits mUnits;
    CellmlFileImportComponents mComponents;
};

//==============================================================================

typedef QList<CellmlFileImport *> CellmlFileImports;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
