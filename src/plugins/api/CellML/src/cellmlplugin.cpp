#include "cellmlplugin.h"

#include "CellMLBootstrap.hpp"
#include "IfaceCellML_APISPEC.hxx"
#include "cellml-api-cxx-support.hpp"

#include <QDebug>

namespace OpenCOR {
namespace CellML {

PLUGININFO_FUNC CellMLPluginInfo()
{
    QMap<QString, QString> descriptions;

    descriptions.insert("en", "A plugin to use the <a href=\"http://www.cellml.org/tools/api/\">CellML API</a>");
    descriptions.insert("fr", "Une extension pour utiliser l'<a href=\"http://www.cellml.org/tools/api/\">API CellML</a>");

    return PluginInfo(PluginInterface::V001,
                      PluginInfo::General,
                      PluginInfo::Api,
                      false,
                      QStringList(),
                      descriptions);
}

Q_EXPORT_PLUGIN2(CellML, CellMLPlugin)

void CellMLPlugin::initialize(const QList<Plugin *> &)
{
    // Fetch a bootstrap object

    RETURN_INTO_OBJREF(cbs, iface::cellml_api::CellMLBootstrap,
                       CreateCellMLBootstrap());

    // Retrieve the model loader

    RETURN_INTO_OBJREF(ml, iface::cellml_api::DOMModelLoader,
                       cbs->modelLoader());

    // Load a modeland return its CMetaId
    // Note: we do this within a try...catch statement since we might get an
    //       exception...

    try {
        RETURN_INTO_OBJREF(model, iface::cellml_api::Model,
                           ml->loadFromURL(L"http://www.cellml.org/models/beeler_reuter_1977_version04/download"));

        // Fetch the models cmeta:id (there obviously lots of other things we could
        // do here!)
        RETURN_INTO_WSTRING(cmid, model->cmetaId());
        qDebug() << "Model's cmeta:id is" << cmid.c_str();
    } catch (iface::cellml_api::CellMLException& e) {
        RETURN_INTO_WSTRING(msg, ml->lastErrorMessage());

        qDebug() << "Got a CellML Exception loading a model. Error was" << msg.c_str();
    }
}

} }
