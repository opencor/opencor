#ifndef CELLMLXMLVIEWPLUGIN_H
#define CELLMLXMLVIEWPLUGIN_H

#include "plugininterface.h"

namespace OpenCOR {
namespace CellMLXMLView {

PLUGININFO_FUNC CellMLXMLViewPluginInfo();

class CellMLXMLViewPlugin : public QObject
{
    Q_OBJECT
};

} }

#endif
