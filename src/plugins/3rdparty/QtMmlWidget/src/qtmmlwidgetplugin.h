#ifndef QTMMLWIDGETPLUGIN_H
#define QTMMLWIDGETPLUGIN_H

#include "plugininfo.h"

namespace OpenCOR {
namespace QtMmlWidget {

PLUGININFO_FUNC QtMmlWidgetPluginInfo();

class QtMmlWidgetPlugin : public QObject
{
    Q_OBJECT
};

} }

#endif
