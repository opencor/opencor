#ifndef CELLMLPLUGIN_H
#define CELLMLPLUGIN_H

#include "apiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

namespace OpenCOR {
namespace CellML {

PLUGININFO_FUNC CellMLPluginInfo();

static const QString CellmlMimeType = "application/cellml+xml";

class CellMLPlugin : public ApiInterface, public I18nInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::ApiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    virtual QList<FileType> fileTypes() const;
    virtual QString fileTypeDescription(const QString &mMimeType) const;
};

} }

#endif
