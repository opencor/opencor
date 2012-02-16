//==============================================================================
// CellML file manager
//==============================================================================

#include "cellmlfilemanager.h"

//==============================================================================

#include <QTimer>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileManager::CellmlFileManager(const int &pTimerInterval)
{
    //

    // Create our timer

    mTimer = new QTimer(this);

    // A connection to handle the timing out of our timer

    connect(mTimer, SIGNAL(timeout()), this, SLOT(checkFiles()));

    // Start our timer

    mTimer->start(pTimerInterval);
}

//==============================================================================

CellmlFileManager::~CellmlFileManager()
{
    // Delete the timer

    delete mTimer;
}

//==============================================================================

CellmlFileManager * CellmlFileManager::instance()
{
    // Return our 'global' CellML model manager

    static CellmlFileManager instance;

    return &instance;
}

//==============================================================================

void CellmlFileManager::checkFiles()
{
    // Check that our various CellML models are still referenced in our 'global'
    // file manager

    for (int i = mCellmlFiles.count()-1; i >= 0; --i) {
        CellmlFile *cellmlFile = mCellmlFiles.at(i);

        if (!Core::FileManager::instance()->isManaged(cellmlFile->fileName())) {
            // The CellML file is not managed by our 'global' file manager, so
            // remove it

            mCellmlFiles.removeAt(i);

            delete cellmlFile;
        }
    }
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
