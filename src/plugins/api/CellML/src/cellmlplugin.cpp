#include "cellmlplugin.h"
#include "coreutils.h"
#include "plugin.h"

#include "CellMLBootstrap.hpp"
#include "IfaceCellML_APISPEC.hxx"
#include "cellml-api-cxx-support.hpp"

#include <QDir>

#include <QMessageBox>

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

void CellMLPlugin::loadLibrary(const QString pLibName)
{
    QString libFileName = QDir::tempPath()+QDir::separator()+"lib"+pLibName+PluginExtension;

    // Keep track of the library filename

    mLibFileNames.append(libFileName);

    // Extract the library

    Core::saveResourceAs(":"+pLibName, libFileName);

    // Load the library

    QLibrary *lib = new QLibrary(libFileName);

    mLibs.append(lib);

    lib->load();
}

void CellMLPlugin::initialize(const QList<Plugin *> &)
{
    // Load all the CellML libraries

    loadLibrary("annotools");
    loadLibrary("ccgs");
    loadLibrary("celedsexporter");
    loadLibrary("celeds");
    loadLibrary("cellml");
    loadLibrary("cevas");
    loadLibrary("cis");
    loadLibrary("cuses");
    loadLibrary("malaes");
    loadLibrary("spros");
    loadLibrary("srus");
    loadLibrary("telicems");
    loadLibrary("vacss");
    loadLibrary("xpath");

    // Fetch a bootstrap object

    RETURN_INTO_OBJREF(cbs, iface::cellml_api::CellMLBootstrap,
                       CreateCellMLBootstrap());

    // Retrieve the model loader

    RETURN_INTO_OBJREF(ml, iface::cellml_api::DOMModelLoader,
                       cbs->modelLoader());

    // Load a model and return its cmeta:id
    // Note: we do this within a try...catch statement since we might get an
    //       exception...

    try {
        RETURN_INTO_OBJREF(model, iface::cellml_api::Model,
                           ml->loadFromURL(L"http://www.cellml.org/models/beeler_reuter_1977_version04/download"));
        RETURN_INTO_WSTRING(cmid, model->cmetaId());

        QMessageBox::information(0, "CellML", QString("The model's cmeta:id is '%1'.").arg(QString::fromStdWString(cmid)));
    } catch (iface::cellml_api::CellMLException& e) {
        RETURN_INTO_WSTRING(msg, ml->lastErrorMessage());

        QMessageBox::information(0, "CellML", QString("An error occurred while loading the mode: %1.").arg(QString::fromStdWString(msg)));
    }
}

void CellMLPlugin::finalize()
{
    // Unload all the CellML libraries

    foreach(QLibrary *lib, mLibs) {
        lib->unload();

        delete lib;
    }

    // Delete all the CellML libraries

    foreach(const QString &libFileName, mLibFileNames)
        QFile(libFileName).remove();
}

} }
