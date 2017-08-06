/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// CellML support plugin
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlinterface.h"
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

    return new PluginInfo(PluginInfo::Support, false, false,
                          QStringList() << "CellMLAPI" << "Compiler" << "StandardSupport",
                          descriptions);
}

//==============================================================================

CellMLSupportPlugin::CellMLSupportPlugin()
{
    // Keep track of our file type interface

    static CellmlInterfaceData data(qobject_cast<FileTypeInterface *>(this));

    Core::globalInstance(CellmlInterfaceDataSignature, &data);
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

QString CellMLSupportPlugin::mimeType() const
{
    // Return the MIME type we support

    return CellmlMimeType;
}

//==============================================================================

QString CellMLSupportPlugin::fileExtension() const
{
    // Return the extension of the type of file we support

    return CellmlFileExtension;
}

//==============================================================================

QString CellMLSupportPlugin::fileTypeDescription() const
{
    // Return the description of the type of file we support

    return tr("CellML File");
}

//==============================================================================

QStringList CellMLSupportPlugin::fileTypeDefaultViews() const
{
    // Return the default views to use for the type of file we support

    return QStringList() << "CellMLTextView" << "RawCellMLView" << "RawTextView";
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

bool CellMLSupportPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool CellMLSupportPlugin::pluginInterfacesOk(const QString &pFileName,
                                             QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

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
    // Note: we do it here rather than in initializePlugin() since we need the
    //       Core plugin to be initialised (so we can get access to our 'global'
    //       file manager)...

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
                                QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                        "<model xmlns=\"http://www.cellml.org/cellml/%1#\" name=\"my_model\">\n"
                                        "    <!-- Your code goes here-->\n"
                                        "</model>\n").arg(Cellml_Latest));

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
