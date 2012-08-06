//==============================================================================
// CellML file manager
//==============================================================================

#ifndef CELLMLFILEMANAGER_H
#define CELLMLFILEMANAGER_H

//==============================================================================

#include "cellmlfile.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QMap>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

typedef QMap<QString, CellmlFile *> CellmlFiles;

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileManager : public QObject
{
    Q_OBJECT

public:
    static CellmlFileManager * instance();

    CellmlFile * cellmlFile(const QString &pFileName);

private:
    CellmlFiles mCellmlFiles;

    explicit CellmlFileManager();
    ~CellmlFileManager();

Q_SIGNALS:
    void fileModified(const QString &pFileName, const bool &pModified);

private Q_SLOTS:
    void manageFile(const QString &pFileName);
    void unmanageFile(const QString &pFileName);

    void setModified(const QString &pFileName, const bool &pModified);

    void renameFile(const QString &pOldFileName, const QString &pNewFileName);
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
