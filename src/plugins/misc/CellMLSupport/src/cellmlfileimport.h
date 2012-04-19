//==============================================================================
// CellML file import
//==============================================================================

#ifndef CELLMLFILEIMPORT_H
#define CELLMLFILEIMPORT_H

//==============================================================================

#include "cellmlfileelement.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QList>
#include <QMap>
#include <QString>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileImport : public CellmlFileElement
{
public:
    explicit CellmlFileImport(const QString &pCmetaId, const QString &pUri);

    void addUnits(const QString &pName, const QString &pReferenceName);
    void addComponent(const QString &pName, const QString &pReferenceName);

    QString uri() const;

    QMap<QString, QString> units() const;
    QMap<QString, QString> components() const;

private:
    QString mUri;

    QMap<QString, QString> mUnits;
    QMap<QString, QString> mComponents;
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
