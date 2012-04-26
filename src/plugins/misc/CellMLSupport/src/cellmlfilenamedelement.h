//==============================================================================
// CellML file named element
//==============================================================================

#ifndef CELLMLFILENAMEDELEMENT_H
#define CELLMLFILENAMEDELEMENT_H

//==============================================================================

#include "cellmlfileelement.h"
#include "cellmlsupportglobal.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileNamedElement : public CellmlFileElement
{
public:
    explicit CellmlFileNamedElement(iface::cellml_api::Model *pModel);
    explicit CellmlFileNamedElement(iface::cellml_api::ImportUnits *pImportUnits);
    explicit CellmlFileNamedElement(iface::cellml_api::ImportComponent *pImportComponent);
    explicit CellmlFileNamedElement(iface::cellml_api::Units *pUnits);
    explicit CellmlFileNamedElement(iface::cellml_api::Unit *pUnit);
    explicit CellmlFileNamedElement(iface::cellml_api::CellMLComponent *pComponent);
    explicit CellmlFileNamedElement(iface::cellml_api::CellMLVariable *pVariable);

    QString name() const;

private:
    QString mName;
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
