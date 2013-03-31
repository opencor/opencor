//==============================================================================
// QScintillaSupport plugin
//==============================================================================

#ifndef QSCINTILLASUPPORTPLUGIN_H
#define QSCINTILLASUPPORTPLUGIN_H

//==============================================================================

#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace QScintillaSupport {

//==============================================================================

PLUGININFO_FUNC QScintillaSupportPluginInfo();

//==============================================================================

class QScintillaSupportPlugin : public QObject
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.QScintillaSupportPlugin" FILE "qscintillasupportplugin.json")
};

//==============================================================================

}   // namespace QScintillaSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
