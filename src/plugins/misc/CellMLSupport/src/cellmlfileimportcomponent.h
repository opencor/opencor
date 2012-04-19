//==============================================================================
// CellML file import component
//==============================================================================

#ifndef CELLMLFILEIMPORTCOMPONENT_H
#define CELLMLFILEIMPORTCOMPONENT_H

//==============================================================================

#include "cellmlfilecomponent.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QList>
#include <QString>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileImportComponent : public CellmlFileComponent
{
public:
    explicit CellmlFileImportComponent(const QString &pCmetaId,
                                       const QString &pName,
                                       const QString &pReferenceName);

    QString referenceName() const;

private:
    QString mReferenceName;
};

//==============================================================================

typedef QList<CellmlFileImportComponent *> CellmlFileImportComponentList;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
