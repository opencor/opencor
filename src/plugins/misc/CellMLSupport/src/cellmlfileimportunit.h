//==============================================================================
// CellML file import unit
//==============================================================================

#ifndef CELLMLFILEIMPORTUNIT_H
#define CELLMLFILEIMPORTUNIT_H

//==============================================================================

#include "cellmlfileunit.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QString>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileImportUnit : public CellmlFileUnit
{
public:
    explicit CellmlFileImportUnit(const QString &pCmetaId,
                                  const QString &pName,
                                  const QString &pReferenceName);

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
