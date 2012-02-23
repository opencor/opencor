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

class CellmlFile
{
public:
    explicit CellmlFile(const QString &pFileName);

    QString fileName() const;

private:
    QString mFileName;
};

//==============================================================================

typedef QMap<QString, CellmlFile *> CellmlFiles;

//==============================================================================

class CellmlFileManager : public QObject
{
    Q_OBJECT

public:
    static CellmlFileManager * instance();

    CellmlFile * cellmlFile(const QString &pFileName);

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
