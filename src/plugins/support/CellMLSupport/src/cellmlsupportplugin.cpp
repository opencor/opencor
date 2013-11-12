/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

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

    descriptions.insert("en", QString::fromUtf8("a plugin to support <a href=\"http://www.cellml.org/\">CellML</a>."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour supporter <a href=\"http://www.cellml.org/\">CellML</a>."));

    return new PluginInfo(PluginInfo::Support,
                          QStringList() << "Core" << "CellMLAPI" << "Compiler" << "CoreSolver",
                          descriptions);
}

//==============================================================================
// Core interface
//==============================================================================

void CellMLSupportPlugin::initialize()
{
    // We don't handle this interface...
}

//==============================================================================

void CellMLSupportPlugin::finalize()
{
    // We don't handle this interface...
}

//==============================================================================

void CellMLSupportPlugin::initialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // Make a call to the instance of the CellML file manager so that it gets
    // properly set up before it actually gets used
    // Note: we do it here rather than in initialize() since we need the Core
    //       plugin to be initialised (so we can get access to our 'global' file
    //       manager)...

    CellmlFileManager::instance();
}

//==============================================================================

void CellMLSupportPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void CellMLSupportPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void CellMLSupportPlugin::settingsLoaded(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void CellMLSupportPlugin::handleArguments(const QStringList &pArguments)
{
    Q_UNUSED(pArguments);

    // We don't handle this interface...
}

//==============================================================================

void CellMLSupportPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// File interface
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

        return QString();
}

//==============================================================================
// GUI interface
//==============================================================================

void CellMLSupportPlugin::retranslateUi()
{
    // We don't handle this interface...
}

//==============================================================================
// Plugin specific
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
                &&  (   (!xml.namespaceUri().toString().compare("http://www.cellml.org/cellml/1.0#"))
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
