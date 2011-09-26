#ifndef CORECELLMLEDITINGPLUGIN_H
#define CORECELLMLEDITINGPLUGIN_H

#include "plugininfo.h"

namespace OpenCOR {
namespace CoreCellMLEditing {

PLUGININFO_FUNC CoreCellMLEditingPluginInfo();

class CoreCellMLEditingPlugin : public QObject
{
    Q_OBJECT
};

} }

#endif
