//==============================================================================
// CellMLSupport plugin
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlsupportplugin.h"

//==============================================================================

#include <QFileInfo>
#include <QXmlStreamReader>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

PLUGININFO_FUNC CellMLSupportPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("A plugin to support <a href=\"http://www.cellml.org/\">CellML</a>"));
    descriptions.insert("fr", QString::fromUtf8("Une extension pour supporter <a href=\"http://www.cellml.org/\">CellML</a>"));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::General,
                          PluginInfo::Miscellaneous,
                          false,
                          QStringList() << "Core" << "CellMLAPI" << "Compiler",
                          descriptions);
}

//==============================================================================

void CellMLSupportPlugin::initializationsDone(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // Make a call to the instance of the CellML file manager so that it gets
    // set up and can then do its job

    CellmlFileManager::instance();
}

//==============================================================================

FileTypes CellMLSupportPlugin::fileTypes() const
{
    // Return the CellML file type that the CellMLSupport plugin supports

    return FileTypes() << FileType(qobject_cast<FileInterface *>(this),
                                   CellmlMimeType, CellmlFileExtension);
}

//==============================================================================

QString CellMLSupportPlugin::fileTypeDescription(const QString &pMimeType) const
{
    // Return the description for the requested MIME type, that is as long as it
    // is for the CellML MIME type

    if (!pMimeType.compare(CellmlMimeType))
        return tr("CellML File");
    else
        // Not a MIME type that we can recognise, so...

        return FileInterface::fileTypeDescription(pMimeType);
}

//==============================================================================

bool isCellmlFile(const QString &pFileName)
{
    // Return whether the file is a CellML file

    if (!QFileInfo(pFileName).suffix().compare(CellMLSupport::CellmlFileExtension))
        // We are dealing with a file which file extension is that of a CellML
        // file, so...

        return true;

    // The file doesn't have the 'correct' file extension, so quickly check its
    // contents

    QFile file(pFileName);

    if (!file.open(QIODevice::ReadOnly))
        // We can't open the file, so...

        return false;

    // Try to read the file as if it was an XML file

    QXmlStreamReader xml(&file);

    bool res = false;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();

        if (xml.isStartElement()) {
            // This is our root element, so for the file to be considered a
            // CellML file it should be a model element in either the CellML 1.0
            // or 1.1 namespace

            if (   !xml.name().toString().compare("model")
                && (   (!xml.namespaceUri().toString().compare("http://www.cellml.org/cellml/1.0#"))
                    || (!xml.namespaceUri().toString().compare("http://www.cellml.org/cellml/1.1#"))))
                // All the requirements are gathered for the file to be
                // considered a CellML file, so...

                res = true;

            break;
        }
    }

    file.close();

    // We are done, so...

    return res;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
