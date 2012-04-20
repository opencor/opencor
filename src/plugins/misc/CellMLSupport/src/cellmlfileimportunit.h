//==============================================================================
// CellML file import unit
//==============================================================================

#ifndef CELLMLFILEIMPORTUNIT_H
#define CELLMLFILEIMPORTUNIT_H

//==============================================================================

#include "cellmlfileunit.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QList>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileImportUnit : public CellmlFileUnit
{
public:
    explicit CellmlFileImportUnit(iface::cellml_api::ImportUnits *pImportUnits);

    QString referenceName() const;

private:
    QString mReferenceName;
};

//==============================================================================

typedef QList<CellmlFileImportUnit *> CellmlFileImportUnits;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
