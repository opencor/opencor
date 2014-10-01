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
#include "cliutils.h"
#include "filemanager.h"

//==============================================================================

#include <QFile>
#include <QFileInfo>
#include <QIODevice>
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

    return new PluginInfo(PluginInfo::Support, false, false,
                          QStringList() << "Core" << "CellMLAPI" << "Compiler" << "CoreSolver",
                          descriptions);
}

//==============================================================================

CellMLSupportPlugin::CellMLSupportPlugin()
{
    // The file types that we support

    mFileTypes = FileTypes() << new FileType(qobject_cast<FileTypeInterface *>(this),
                                             CellmlMimeType, CellmlFileExtension);
}

//==============================================================================

CellMLSupportPlugin::~CellMLSupportPlugin()
{
    // Delete some internal objects

    foreach (FileType *fileType, mFileTypes)
        delete fileType;
}

//==============================================================================
// File interface
//==============================================================================

FileTypes CellMLSupportPlugin::fileTypes() const
{
    // Return the file types that we support

    return mFileTypes;
}

//==============================================================================

QString CellMLSupportPlugin::fileTypeDescription(const QString &pMimeType) const
{
    // Return the description for the requested MIME type, that is as long as it
    // is for the MIME type that we support

    if (!pMimeType.compare(CellmlMimeType))
        return tr("CellML File");
    else
        // Not a MIME type that we can recognise

        return QString();
}

//==============================================================================
// I18n interface
//==============================================================================

void CellMLSupportPlugin::retranslateUi()
{
    // We don't handle this interface...
    // Note: even though we don't handle this interface, we still want to
    //       support it since some other aspects of our plugin are
    //       multilingual...
}

//==============================================================================
// Plugin interface
//==============================================================================

void CellMLSupportPlugin::initializePlugin(QMainWindow *pMainWindow)
{
    Q_UNUSED(pMainWindow);

    // We don't handle this interface...
}

//==============================================================================

void CellMLSupportPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void CellMLSupportPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // Make a call to the instance of the CellML file manager so that it gets
    // properly set up (and therefore can start managing CellML files) before it
    // actually gets used by other plugins
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

void CellMLSupportPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Plugin specific
//==============================================================================

bool isCellmlFile(const QString &pFileName)
{
    // Return whether the file is a CellML file

    if (!QFileInfo(pFileName).completeSuffix().compare(CellmlFileExtension))
        return true;

    // The file doesn't have the 'correct' file extension, so check whether it's
    // a new file

    if (Core::FileManager::instance()->isNew(pFileName))
        return true;

    // Check whether we are dealing with an empty file or a file that contains
    // spaces of sorts

    QString fileContents;

    if (   Core::readTextFromFile(pFileName, fileContents)
        && fileContents.trimmed().isEmpty())
        return true;

    // The file neither has the 'correct' file extension nor is a new file, so
    // quickly check its contents

    QFile file(pFileName);

    if (!file.open(QIODevice::ReadOnly))
        return false;

    // Try to read the file as if it was an XML file

    QXmlStreamReader xml(&file);

    bool res = false;

    while (!xml.atEnd()) {
        xml.readNext();

        if (xml.isStartElement()) {
            // This is our root element, so for the file to be considered a
            // CellML file it should be a model element in either the CellML 1.0
            // or 1.1 namespace

            if (   !xml.name().toString().compare("model")
                &&  (   (!xml.namespaceUri().toString().compare(Cellml_1_0_Namespace))
                     || (!xml.namespaceUri().toString().compare(Cellml_1_1_Namespace)))) {
                res = true;
            }

            break;
        }
    }

    file.close();

    return res;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
