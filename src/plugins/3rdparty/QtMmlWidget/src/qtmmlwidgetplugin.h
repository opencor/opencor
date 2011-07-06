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

public:
    // Plugin interface

    virtual void initialize();
    virtual void finalize();
};

} }

#endif
