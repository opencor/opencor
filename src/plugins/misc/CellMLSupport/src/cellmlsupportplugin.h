//==============================================================================
// CellMLSupport plugin
//==============================================================================

#ifndef CELLMLSUPPORTPLUGIN_H
#define CELLMLSUPPORTPLUGIN_H

//==============================================================================

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

//==============================================================================

class CellMLSupportPlugin : public QObject, public FileInterface,
                            public I18nInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::FileInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    virtual QList<FileType> fileTypes() const;
    virtual QString fileTypeDescription(const QString &mMimeType) const;
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
