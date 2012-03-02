//==============================================================================
// CellMLSupport plugin
//==============================================================================

#ifndef CELLMLSUPPORTPLUGIN_H
#define CELLMLSUPPORTPLUGIN_H

//==============================================================================

#include "cellmlsupportglobal.h"
#include "coreinterface.h"
#include "plugin.h"
#include "plugininfo.h"
#include "fileinterface.h"
#include "i18ninterface.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

PLUGININFO_FUNC CellMLSupportPluginInfo();

//==============================================================================

static const QString CellmlMimeType = "application/cellml+xml";
static const QString CellmlFileExtension = "cellml";

//==============================================================================

class CellMLSupportPlugin : public QObject, public CoreInterface,
                            public FileInterface, public I18nInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::FileInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    virtual void initializationsDone(const Plugins &);

    virtual QList<FileType> fileTypes() const;
    virtual QString fileTypeDescription(const QString &mMimeType) const;
};

//==============================================================================

bool CELLMLSUPPORT_EXPORT isCellmlFile(const QString &pFileName);

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
