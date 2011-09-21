#include "cellmlplugin.h"
#include "coreutils.h"

#include "CellMLBootstrap.hpp"
#include "IfaceCellML_APISPEC.hxx"
#include "cellml-api-cxx-support.hpp"

namespace OpenCOR {
namespace CellML {

PLUGININFO_FUNC CellMLPluginInfo()
{
    PluginInfoDescriptions descriptions;

    descriptions.insert("en", "A plugin to use the <a href=\"http://www.cellml.org/tools/api/\">CellML API</a>");
    descriptions.insert("fr", "Une extension pour utiliser l'<a href=\"http://www.cellml.org/tools/api/\">API CellML</a>");

    return PluginInfo(PluginInterface::V001,
                      PluginInfo::General,
                      PluginInfo::Api,
                      false,
                      QStringList() << "Core",
                      descriptions);
}

Q_EXPORT_PLUGIN2(CellML, CellMLPlugin)

QList<FileType> CellMLPlugin::fileTypes()
{
    FileDescriptions descriptions;

    descriptions.insert("en", "CellML File");
    descriptions.insert("fr", "Ficher CellML");

    return QList<FileType>() << FileType("application/cellml+xml", ".cellml", descriptions);
}

} }
