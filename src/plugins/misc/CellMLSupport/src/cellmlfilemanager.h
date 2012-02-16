//==============================================================================
// CellML file manager
//==============================================================================

#ifndef CELLMLFILEMANAGER_H
#define CELLMLFILEMANAGER_H

//==============================================================================

#include "cellmlmodel.h"
#include "filemanager.h"

//==============================================================================

class QTimer;

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CellmlFile : public Core::File
{
};

//==============================================================================

class CellmlFileManager : public QObject
{
    Q_OBJECT

public:
    explicit CellmlFileManager(const int &pTimerInterval = 1000);
    ~CellmlFileManager();

    static CellmlFileManager * instance();

private:
    QTimer *mTimer;
    QList<CellmlFile *> mCellmlFiles;

private Q_SLOTS:
    void checkFiles();
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
