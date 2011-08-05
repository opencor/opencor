#ifndef CELLMLXMLVIEWPLUGIN_H
#define CELLMLXMLVIEWPLUGIN_H

#include "guiinterface.h"

namespace OpenCOR {
namespace CellMLXMLView {

PLUGININFO_FUNC CellMLXMLViewPluginInfo();

class CellMLXMLViewPlugin : public GuiInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)

public:
    explicit CellMLXMLViewPlugin();
};

} }

#endif
