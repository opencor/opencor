//==============================================================================
// CellML file manager
//==============================================================================

#ifndef CELLMLFILEMANAGER_H
#define CELLMLFILEMANAGER_H

//==============================================================================

#include "cellmlmodel.h"

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

class CellmlFileManager : public QObject
{
    Q_OBJECT

public:
    static CellmlFileManager * instance();

private:
    QList<CellmlFile *> mCellmlFiles;

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
