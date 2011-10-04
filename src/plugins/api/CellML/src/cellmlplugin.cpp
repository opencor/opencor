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
    // Fetch a bootstrap object

    RETURN_INTO_OBJREF(cbs, iface::cellml_api::CellMLBootstrap,
                       CreateCellMLBootstrap());

    // Retrieve the model loader

    RETURN_INTO_OBJREF(ml, iface::cellml_api::DOMModelLoader,
                       cbs->modelLoader());

    // Load our test CellML model and return its cmeta:id
    // Note: we do this within a try...catch statement since we might get an
    //       exception...

    QString testCellmlModelFileName = QDir::tempPath()+QDir::separator()+"test_cellml_model.cellml";

    Core::saveResourceAs(":test_cellml_model", testCellmlModelFileName);

    try {
        qDebug("Loading the model...");

        RETURN_INTO_OBJREF(model, iface::cellml_api::Model,
                           ml->loadFromURL(QUrl::fromLocalFile(testCellmlModelFileName).toString().toStdWString().c_str()));

        qDebug();
        qDebug("Retrieving some model properties...");

        RETURN_INTO_WSTRING(name, model->name());

        qDebug("    Model: %s",
               QString::fromStdWString(name).toLatin1().constData());

        RETURN_INTO_OBJREF(modelComponents,
                           iface::cellml_api::CellMLComponentSet,
                           model->modelComponents());
        RETURN_INTO_OBJREF(compIter,
                           iface::cellml_api::CellMLComponentIterator,
                           modelComponents->iterateComponents());

        for (int i = 0; i < modelComponents->length(); i++) {
            RETURN_INTO_OBJREF(comp,
                               iface::cellml_api::CellMLComponent,
                               compIter->nextComponent());
            RETURN_INTO_WSTRING(compName, comp->name());

            qDebug("        Component: %s",
                   QString::fromStdWString(compName).toLatin1().constData());

            RETURN_INTO_OBJREF(variables,
                               iface::cellml_api::CellMLVariableSet,
                               comp->variables());
            RETURN_INTO_OBJREF(varIter,
                               iface::cellml_api::CellMLVariableIterator,
                               variables->iterateVariables());

            for(int j = 0; j < variables->length(); ++j) {
                RETURN_INTO_OBJREF(var,
                                   iface::cellml_api::CellMLVariable,
                                   varIter->nextVariable());
                RETURN_INTO_WSTRING(varName, var->name());

                qDebug("            Variable: %s",
                       QString::fromStdWString(varName).toLatin1().constData());
            }
        }

        qDebug();
        qDebug("All done...");
    } catch (iface::cellml_api::CellMLException& e) {
        RETURN_INTO_WSTRING(msg, ml->lastErrorMessage());

        qDebug("ERROR: %s.", QString::fromStdWString(msg).toLatin1().constData());
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
