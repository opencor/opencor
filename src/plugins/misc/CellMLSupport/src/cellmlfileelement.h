//==============================================================================
// CellML file element
//==============================================================================

#ifndef CELLMLFILEELEMENT_H
#define CELLMLFILEELEMENT_H

//==============================================================================

#include "cellmlsupportglobal.h"

//==============================================================================

#include <QString>

//==============================================================================

#include "cellml-api-cxx-support.hpp"

#include "IfaceCellML_APISPEC.hxx"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileElement
{
public:
    explicit CellmlFileElement(iface::cellml_api::Model *pModel);
    explicit CellmlFileElement(iface::cellml_api::CellMLImport *pCellmlImport);
    explicit CellmlFileElement(iface::cellml_api::ImportUnits *pImportUnits);
    explicit CellmlFileElement(iface::cellml_api::ImportComponent *pImportComponent);
    explicit CellmlFileElement(iface::cellml_api::Units *pUnits);

    QString cmetaId() const;

private:
    QString mCmetaId;
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
