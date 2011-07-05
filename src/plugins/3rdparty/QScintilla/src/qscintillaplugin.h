#ifndef QSCINTILLAPLUGIN_H
#define QSCINTILLAPLUGIN_H

#include "plugininterface.h"

namespace OpenCOR {

PLUGININFO_FUNC QScintillaPluginInfo();

class QScintillaPlugin : public QObject,
                         public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
};

}

#endif
