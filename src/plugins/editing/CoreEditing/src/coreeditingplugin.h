#ifndef COREEDITINGPLUGIN_H
#define COREEDITINGPLUGIN_H

#include "plugininterface.h"

namespace OpenCOR {
namespace CoreEditing {

PLUGININFO_FUNC CoreEditingPluginInfo();

class CoreEditingPlugin : public QObject
{
    Q_OBJECT
};

} }

#endif
