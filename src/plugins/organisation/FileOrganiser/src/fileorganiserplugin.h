#ifndef FILEORGANISERPLUGIN_H
#define FILEORGANISERPLUGIN_H

#include "i18ninterface.h"
#include "plugininfo.h"

namespace OpenCOR {
namespace FileOrganiser {

PLUGININFO_FUNC FileOrganiserPluginInfo();

class FileOrganiserPlugin : public QObject, public I18nInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::I18nInterface)
};

} }

#endif
