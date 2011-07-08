#ifndef COREPLUGIN_H
#define COREPLUGIN_H

#include "guiinterface.h"

namespace OpenCOR {
namespace Core {

PLUGININFO_FUNC CorePluginInfo();

class CorePlugin : public QObject, public GuiInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::GuiInterface)
};

} }

#endif
