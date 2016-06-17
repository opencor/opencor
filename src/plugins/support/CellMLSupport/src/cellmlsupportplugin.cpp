/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// CellML support plugin
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlsupportplugin.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "filemanager.h"

//==============================================================================

#include <QAction>
#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

PLUGININFO_FUNC CellMLSupportPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to support <a href=\"http://www.cellml.org/\">CellML</a>."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour supporter <a href=\"http://www.cellml.org/\">CellML</a>."));

    return new PluginInfo("Support", false, false,
                          QStringList() << "CellMLAPI" << "Compiler" << "StandardSupport",
                          descriptions);
}

//==============================================================================

CellMLSupportPlugin::CellMLSupportPlugin()
{
    // The file types that we support
    // Note: they will get deleted by FileTypeInterface...

    mFileTypes = FileTypes() << new FileType(qobject_cast<FileTypeInterface *>(this),
                                             CellmlMimeType, CellmlFileExtension);

    mDefaultViews = QStringList() << "CellMLTextView" << "RawCellMLView" << "RawTextView";
}

//==============================================================================
// File interface
//==============================================================================

bool CellMLSupportPlugin::isFile(const QString &pFileName) const
{
    // Return whether the given file is of the type that we support

    return CellmlFileManager::instance()->isFile(pFileName);
}

//==============================================================================

QString CellMLSupportPlugin::fileTypeDescription(const QString &pMimeType) const
{
    // Return the description for the requested MIME type, that is as long as it
    // is for the MIME type that we support

    if (!pMimeType.compare(CellmlMimeType)) {
        return tr("CellML File");
    } else {
        // Not a MIME type that we can recognise

        return QString();
    }
}

//==============================================================================
// GUI interface
//==============================================================================

void CellMLSupportPlugin::updateGui(Plugin *pViewPlugin, const QString &pFileName)
{
    Q_UNUSED(pViewPlugin);
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

Gui::Menus CellMLSupportPlugin::guiMenus() const
{
    // We don't handle this interface...

    return Gui::Menus();
}

//==============================================================================

Gui::MenuActions CellMLSupportPlugin::guiMenuActions() const
{
    // Return our menu actions

    return Gui::MenuActions() << Gui::MenuAction(Gui::MenuAction::FileNew, mFileNewCellmlFileAction);
}

//==============================================================================
// I18n interface
//==============================================================================

void CellMLSupportPlugin::retranslateUi()
{
    // Retranslate our different actions

    retranslateAction(mFileNewCellmlFileAction, tr("CellML File"),
                      tr("Create a new CellML file"));
}

//==============================================================================
// Plugin interface
//==============================================================================

void CellMLSupportPlugin::initializePlugin()
{
    // Create our different actions

    mFileNewCellmlFileAction = Core::newAction(Core::mainWindow());

    // Some connections to handle our different actions

    connect(mFileNewCellmlFileAction, SIGNAL(triggered(bool)),
            this, SLOT(newCellmlFile()));
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

void CellMLSupportPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Plugin specific
//==============================================================================

void CellMLSupportPlugin::newCellmlFile()
{
    // Ask our file manager to create a new file

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();
#ifdef QT_DEBUG
    Core::FileManager::Status createStatus =
#endif
    fileManagerInstance->create(QString(),
                                "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                "<model xmlns=\"http://www.cellml.org/cellml/"+QString(Cellml_Latest).toUtf8()+"#\" name=\"my_model\">\n"
                                "    <!-- Your code goes here-->\n"
                                "</model>\n");

#ifdef QT_DEBUG
    // Make sure that the file has indeed been created

    if (createStatus != Core::FileManager::Created)
        qFatal("FATAL ERROR | %s:%d: the file was not created.", __FILE__, __LINE__);
#endif
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
