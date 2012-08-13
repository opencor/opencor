//==============================================================================
// CellML file unit
//==============================================================================

#ifndef CELLMLFILEUNIT_H
#define CELLMLFILEUNIT_H

//==============================================================================

#include "cellmlfileunitelement.h"
#include "cellmlsupportglobal.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileUnit : public CellmlFileNamedElement
{
public:
    explicit CellmlFileUnit(CellmlFile *pCellmlFile,
                            iface::cellml_api::ImportUnits *pCellmlApiImportUnits);
    explicit CellmlFileUnit(CellmlFile *pCellmlFile,
                            iface::cellml_api::Units *pCellmlApiUnits);
    ~CellmlFileUnit();

    bool isBaseUnit() const;

    CellmlFileUnitElements unitElements() const;

private:
    bool mBaseUnit;

    CellmlFileUnitElements mUnitElements;
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
