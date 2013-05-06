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

typedef QMap<QString, CellMLFile *> CellMLFiles;

//==============================================================================

class CELLMLSUPPORT_EXPORT CellMLFileManager : public QObject
{
    Q_OBJECT

public:
    static CellMLFileManager * instance();

    CellMLFile * cellmlFile(const QString &pFileName);

private:
    CellMLFiles mCellMLFiles;

    explicit CellMLFileManager();
    ~CellMLFileManager();

private Q_SLOTS:
    void manageFile(const QString &pFileName);
    void unmanageFile(const QString &pFileName);

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
