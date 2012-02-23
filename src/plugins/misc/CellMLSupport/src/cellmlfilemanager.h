//==============================================================================
// CellML file manager
//==============================================================================

#ifndef CELLMLFILEMANAGER_H
#define CELLMLFILEMANAGER_H

//==============================================================================

#include "cellmlmodel.h"

//==============================================================================

#include <QMap>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

typedef QMap<QString, CellmlModel *> CellmlFiles;

//==============================================================================

class CellmlFileManager : public QObject
{
    Q_OBJECT

public:
    static CellmlFileManager * instance();

    CellmlModel * cellmlFile(const QString &pFileName);

private:
    CellmlFiles mCellmlFiles;

    explicit CellmlFileManager();
    ~CellmlFileManager();

private Q_SLOTS:
    void fileManaged(const QString &pFileName);
    void fileUnmanaged(const QString &pFileName);
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
