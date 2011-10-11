#ifndef RAWCELLMLVIEWPLUGIN_H
#define RAWCELLMLVIEWPLUGIN_H

#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

namespace OpenCOR {
namespace RawCellMLView {

PLUGININFO_FUNC RawCellMLViewPluginInfo();

class RawCellMLViewPlugin : public QObject, public GuiInterface,
                            public I18nInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    explicit RawCellMLViewPlugin();

    virtual QString viewName(const int &pViewIndex);
};

} }

#endif
