#ifndef RAWVIEWPLUGIN_H
#define RAWVIEWPLUGIN_H

#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

namespace OpenCOR {
namespace RawView {

PLUGININFO_FUNC RawViewPluginInfo();

class RawViewPlugin : public QObject, public GuiInterface, public I18nInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    explicit RawViewPlugin();

protected:
    virtual void retranslateUi();
};

} }

#endif
