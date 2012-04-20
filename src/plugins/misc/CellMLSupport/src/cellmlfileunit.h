//==============================================================================
// CellML file unit
//==============================================================================

#ifndef CELLMLFILEUNIT_H
#define CELLMLFILEUNIT_H

//==============================================================================

#include "cellmlfilenamedelement.h"
#include "cellmlsupportglobal.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileUnit : public CellmlFileNamedElement
{
public:
    explicit CellmlFileUnit(iface::cellml_api::ImportUnits *pImportUnits);
    explicit CellmlFileUnit(iface::cellml_api::Units *pUnits);

    bool isBaseUnit() const;

private:
    bool mBaseUnit;
};

//==============================================================================

typedef QList<CellmlFileUnit *> CellmlFileUnits;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
