//==============================================================================
// CellMLSupport plugin
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlsupportplugin.h"

//==============================================================================

#include <QFileInfo>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

PLUGININFO_FUNC CellMLSupportPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to support <a href=\"http://www.cellml.org/\">CellML</a>");
    descriptions.insert("fr", "Une extension pour supporter <a href=\"http://www.cellml.org/\">CellML</a>");

    return new PluginInfo(PluginInfo::V001,
                          PluginInfo::General,
                          PluginInfo::Miscellaneous,
                          false,
                          QStringList() << "Core" << "CellMLAPI" << "Compiler",
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(CellMLSupport, CellMLSupportPlugin)

//==============================================================================

void CellMLSupportPlugin::initializationsDone(const Plugins &)
{
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
    // Return whether the file is a CellML file based on its file extension

    return !QFileInfo(pFileName).suffix().compare(CellMLSupport::CellmlFileExtension);
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
