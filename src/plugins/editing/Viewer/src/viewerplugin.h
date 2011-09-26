#ifndef VIEWERPLUGIN_H
#define VIEWERPLUGIN_H

#include "i18ninterface.h"
#include "plugininfo.h"

namespace OpenCOR {
namespace Viewer {

PLUGININFO_FUNC ViewerPluginInfo();

class ViewerPlugin : public QObject, public I18nInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::I18nInterface)
};

} }

#endif
