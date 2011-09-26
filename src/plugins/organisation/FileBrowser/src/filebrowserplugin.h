#ifndef FILEBROWSERPLUGIN_H
#define FILEBROWSERPLUGIN_H

#include "i18ninterface.h"
#include "plugininfo.h"

namespace OpenCOR {
namespace FileBrowser {

PLUGININFO_FUNC FileBrowserPluginInfo();

class FileBrowserPlugin : public QObject, public I18nInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::I18nInterface)
};

} }

#endif
