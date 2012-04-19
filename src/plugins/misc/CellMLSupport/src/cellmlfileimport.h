//==============================================================================
// CellML file import
//==============================================================================

#ifndef CELLMLFILEIMPORT_H
#define CELLMLFILEIMPORT_H

//==============================================================================

#include "cellmlfileelement.h"
#include "cellmlfileimportunit.h"
#include "cellmlfileimportcomponent.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QString>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileImport : public CellmlFileElement
{
public:
    explicit CellmlFileImport(const QString &pCmetaId, const QString &pUri);
    ~CellmlFileImport();

    void addUnits(const QString &pCmetaId, const QString &pName,
                  const QString &pReferenceName);
    void addComponent(const QString &pCmetaId, const QString &pName,
                      const QString &pReferenceName);

    QString uri() const;

    CellmlFileImportUnits units() const;
    CellmlFileImportComponents components() const;

private:
    QString mUri;

    CellmlFileImportUnits mUnits;
    CellmlFileImportComponents mComponents;
};

//==============================================================================

typedef QList<CellmlFileImport *> CellmlFileImports;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
