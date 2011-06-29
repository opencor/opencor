#ifndef QTMMLWIDGETPLUGIN_H
#define QTMMLWIDGETPLUGIN_H

#include "plugininterface.h"

namespace OpenCOR {

PLUGININFO_FUNC QtMmlWidgetPluginInfo();

class QtMmlWidgetPlugin : public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
};

}

#endif
