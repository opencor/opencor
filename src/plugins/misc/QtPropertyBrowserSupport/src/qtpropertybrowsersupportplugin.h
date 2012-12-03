//==============================================================================
// QtPropertyBrowserSupport plugin
//==============================================================================

#ifndef QTPROPERTYBROWSERSUPPORTPLUGIN_H
#define QTPROPERTYBROWSERSUPPORTPLUGIN_H

//==============================================================================

#include "i18ninterface.h"
#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace QtPropertyBrowserSupport {

//==============================================================================

PLUGININFO_FUNC QtPropertyBrowserSupportPluginInfo();

//==============================================================================

class QtPropertyBrowserSupportPlugin : public QObject, public I18nInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::I18nInterface)
};

//==============================================================================

}   // namespace QtPropertyBrowserSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
