#ifndef QTMMLWIDGETPLUGIN_H
#define QTMMLWIDGETPLUGIN_H

#include "plugininterface.h"

namespace OpenCOR {
namespace QtMmlWidget {

PLUGININFO_FUNC QtMmlWidgetPluginInfo();

class QtMmlWidgetPlugin : public QObject,
                          public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
};

} }

#endif
