//==============================================================================
// CellML file import units
//==============================================================================

#ifndef CELLMLFILEIMPORTUNITS_H
#define CELLMLFILEIMPORTUNITS_H

//==============================================================================

#include "cellmlfileunits.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QString>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileImportUnits : public CellmlFileUnits
{
public:
    explicit CellmlFileImportUnits(const QString &pCmetaId,
                                   const QString &pName,
                                   const QString &pReferenceName);

    QString referenceName() const;

private:
    QString mReferenceName;
};

//==============================================================================

typedef QList<CellmlFileImportUnits *> CellmlFileImportUnitsList;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
