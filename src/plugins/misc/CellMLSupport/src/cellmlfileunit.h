//==============================================================================
// CellML file unit
//==============================================================================

#ifndef CELLMLFILEUNIT_H
#define CELLMLFILEUNIT_H

//==============================================================================

#include "cellmlfilenamedelement.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QString>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileUnit : public CellmlFileNamedElement
{
public:
    explicit CellmlFileUnit(const QString &pCmetaId, const QString &pName,
                            const bool &pBaseUnit = false);

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
