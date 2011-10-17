//==============================================================================
// CellML plugin
//==============================================================================

#ifndef CELLMLPLUGIN_H
#define CELLMLPLUGIN_H

//==============================================================================

#include "coreinterface.h"
#include "fileinterface.h"
#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace CellML {

//==============================================================================

PLUGININFO_FUNC CellMLPluginInfo();

//==============================================================================

static const QString CellmlMimeType = "application/cellml+xml";

//==============================================================================

class CellMLPlugin : public QObject, public CoreInterface, public FileInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::FileInterface)

public:
    virtual void initialize();

    virtual QList<FileType> fileTypes() const;
    virtual QString fileTypeDescription(const QString &mMimeType) const;

private:
    void usePrecomputedTestCellmlResults();
};

//==============================================================================

}   // namespace CellML
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
