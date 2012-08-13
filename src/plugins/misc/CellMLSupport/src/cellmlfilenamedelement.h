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
    explicit CellmlFileNamedElement(CellmlFile *pCellmlFile,
                                    iface::cellml_api::Model *pCellmlApiModel);
    explicit CellmlFileNamedElement(CellmlFile *pCellmlFile,
                                    iface::cellml_api::ImportUnits *pCellmlApiImportUnits);
    explicit CellmlFileNamedElement(CellmlFile *pCellmlFile,
                                    iface::cellml_api::ImportComponent *pCellmlApiImportComponent);
    explicit CellmlFileNamedElement(CellmlFile *pCellmlFile,
                                    iface::cellml_api::Units *pCellmlApiUnits);
    explicit CellmlFileNamedElement(CellmlFile *pCellmlFile,
                                    iface::cellml_api::Unit *pCellmlApiUnit);
    explicit CellmlFileNamedElement(CellmlFile *pCellmlFile,
                                    iface::cellml_api::CellMLComponent *pCellmlApiComponent);
    explicit CellmlFileNamedElement(CellmlFile *pCellmlFile,
                                    iface::cellml_api::CellMLVariable *pCellmlApiVariable);

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
