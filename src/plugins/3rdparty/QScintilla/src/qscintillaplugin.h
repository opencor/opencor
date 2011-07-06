#ifndef QSCINTILLAPLUGIN_H
#define QSCINTILLAPLUGIN_H

#include "plugininterface.h"

namespace OpenCOR {
namespace QScintilla {

PLUGININFO_FUNC QScintillaPluginInfo();

class QScintillaPlugin : public QObject,
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
