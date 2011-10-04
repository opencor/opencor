#include "cellmlplugin.h"
#include "coreutils.h"

#include "CellMLBootstrap.hpp"
#include "IfaceCellML_APISPEC.hxx"
#include "cellml-api-cxx-support.hpp"

#include <QDebug>
#include <QDir>
#include <QUrl>

namespace OpenCOR {
namespace CellML {

PLUGININFO_FUNC CellMLPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to use the <a href=\"http://www.cellml.org/tools/api/\">CellML API</a>");
    descriptions.insert("fr", "Une extension pour utiliser l'<a href=\"http://www.cellml.org/tools/api/\">API CellML</a>");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::General,
                      PluginInfo::Api,
                      false,
                      QStringList(),
                      descriptions);
}

Q_EXPORT_PLUGIN2(CellML, CellMLPlugin)

void CellMLPlugin::initialize()
{
    using namespace iface::cellml_api;

    // Fetch a bootstrap object

    CellMLBootstrap *cbs = CreateCellMLBootstrap();

    // Retrieve the model loader

    DOMModelLoader *ml = cbs->modelLoader();

    // Load our test CellML model and return its cmeta:id
    // Note: we do this within a try...catch statement since we might get an
    //       exception...

    QString testCellmlModelFileName = QDir::tempPath()+QDir::separator()+"test_cellml_model.cellml";

    Core::saveResourceAs(":test_cellml_model", testCellmlModelFileName);

    try {
        qDebug("Loading the model...");

        Model *model = ml->loadFromURL(QUrl::fromLocalFile(testCellmlModelFileName).toString().toStdWString().c_str());

        qDebug();
        qDebug("Retrieving some model properties...");

        // Retrieve the model's name

        qDebug("    Model '%s'",
               QString::fromWCharArray(model->name()).toLatin1().constData());

        // Go through the model's components and their respective variables

        CellMLComponentSet *comps = model->modelComponents();
        CellMLComponentIterator *compsIter = comps->iterateComponents();

        for (int i = 0; i < comps->length(); i++) {
            CellMLComponent *comp = compsIter->nextComponent();

            qDebug("        Component '%s'",
                   QString::fromWCharArray(comp->name()).toLatin1().constData());

            CellMLVariableSet *vars = comp->variables();
            CellMLVariableIterator *varsIter = vars->iterateVariables();

            for(int j = 0; j < vars->length(); ++j)
                qDebug("            Variable '%s'",
                       QString::fromStdWString(varsIter->nextVariable()->name()).toLatin1().constData());
        }

        // Go through the model's connections and information

        ConnectionSet *conns = model->connections();
        ConnectionIterator *connsIter = conns->iterateConnections();

        for (int i = 0; i < conns->length(); i++) {
            Connection *conn = connsIter->nextConnection();
            MapComponents *compMap = conn->componentMapping();

            qDebug("        Connection between '%s' and '%s'",
                   QString::fromStdWString(compMap->firstComponentName()).toLatin1().constData(),
                   QString::fromStdWString(compMap->secondComponentName()).toLatin1().constData());

            MapVariablesSet *varMaps = conn->variableMappings();
            MapVariablesIterator *varMapsIter = varMaps->iterateMapVariables();

            for(int j = 0; j < varMaps->length(); ++j) {
                MapVariables *mapVars = varMapsIter->nextMapVariables();

                qDebug("            For variables '%s' and '%s'",
                       QString::fromStdWString(mapVars->firstVariableName()).toLatin1().constData(),
                       QString::fromStdWString(mapVars->secondVariableName()).toLatin1().constData());
            }
        }

        // All done...

        qDebug();
        qDebug("All done...");
    } catch (CellMLException &) {
        qDebug("ERROR: %s.",
               QString::fromWCharArray(ml->lastErrorMessage()).toLatin1().constData());
    }

    QFile::remove(testCellmlModelFileName);
}

QList<FileType> CellMLPlugin::fileTypes() const
{
    // Return the CellML file type that the CellML plugin supports

    return QList<FileType>() << FileType(qobject_cast<FileInterface *>(this),
                                         CellmlMimeType, "cellml");
}

QString CellMLPlugin::fileTypeDescription(const QString &mMimeType) const
{
    // Return the description for the requested Mime type, that is as long as it
    // is for the CellML Mime type

    if (!mMimeType.compare(CellmlMimeType))
        return tr("CellML File");
    else
        // Not a Mime type that we can recognise, so...

        return FileInterface::fileTypeDescription(mMimeType);
}

} }
