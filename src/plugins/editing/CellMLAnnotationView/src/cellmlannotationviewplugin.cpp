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
// CellMLAnnotationView plugin
//==============================================================================

#include "cellmlannotationvieweditingwidget.h"
#include "cellmlannotationviewplugin.h"
#include "cellmlannotationviewwidget.h"
#include "cellmlfilemanager.h"
#include "cellmlsupportplugin.h"

//==============================================================================

#include <QMainWindow>
#include <QSettings>
#include <QStatusBar>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

PLUGININFO_FUNC CellMLAnnotationViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to annotate CellML files."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour annoter des fichiers CellML."));

    return new PluginInfo(PluginInfo::Editing, true, false,
                          QStringList() << "CellMLSupport",
                          descriptions);
}

//==============================================================================
// File handling interface
//==============================================================================

bool CellMLAnnotationViewPlugin::saveFile(const QString &pOldFileName,
                                          const QString &pNewFileName)
{
    // Ask our CellML annotation view widget to save the given file

    CellmlAnnotationViewEditingWidget *editingWidget = mViewWidget->editingWidget(pOldFileName);

    return editingWidget?editingWidget->cellmlFile()->save(pNewFileName):false;
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    // The given file has been un/locked, so let our view widget know about it

    mViewWidget->filePermissionsChanged(pFileName);
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so let our view widget know about it

    mViewWidget->fileReloaded(pFileName);
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileRenamed(const QString &pOldFileName,
                                             const QString &pNewFileName)
{
    // The given file has been renamed, so let our view widget know about it

    mViewWidget->fileRenamed(pOldFileName, pNewFileName);
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================
// I18n interface
//==============================================================================

void CellMLAnnotationViewPlugin::retranslateUi()
{
    // Retranslate our CellML annotation view widget

    mViewWidget->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

void CellMLAnnotationViewPlugin::initializePlugin(QMainWindow *pMainWindow)
{
    // Keep track of our main window

    mMainWindow = pMainWindow;

    // Create our CellML annotation view widget

    mViewWidget = new CellmlAnnotationViewWidget(this, pMainWindow);

    // Hide our CellML annotation view widget since it may not initially be
    // shown in our central widget

    mViewWidget->setVisible(false);
}

//==============================================================================

void CellMLAnnotationViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our CellML annotation view widget settings

    pSettings->beginGroup(mViewWidget->objectName());
        mViewWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void CellMLAnnotationViewPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our CellML annotation view widget settings

    pSettings->beginGroup(mViewWidget->objectName());
        mViewWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void CellMLAnnotationViewPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// View interface
//==============================================================================

ViewInterface::Mode CellMLAnnotationViewPlugin::viewMode() const
{
    // Return our mode

    return ViewInterface::Editing;
}

//==============================================================================

QStringList CellMLAnnotationViewPlugin::viewMimeTypes() const
{
    // Return the MIME types we support

    return QStringList() << CellMLSupport::CellmlMimeType;
}

//==============================================================================

QWidget * CellMLAnnotationViewPlugin::viewWidget(const QString &pFileName)
{
    // Make sure that we are dealing with a CellML file

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        return 0;

    // Update and return our CellML annotation view widget using the given
    // CellML file
    // Note: to show/hide the status bar is to avoid some of the flickering that
    //       results from switching from one file to another (both using the
    //       same view) with the status bar visible and the mouse pointer over a
    //       button-like widget within the current view (see
    //       https://github.com/opencor/opencor/issues/405). It's not neat, but
    //       it seems like it might be an issue with Qt itself...

    bool statusBarVisible = mMainWindow->statusBar()->isVisible();

    mMainWindow->statusBar()->setVisible(false);

    mViewWidget->initialize(pFileName);

    mMainWindow->statusBar()->setVisible(statusBarVisible);

    return mViewWidget;
}

//==============================================================================

void CellMLAnnotationViewPlugin::removeViewWidget(const QString &pFileName)
{
    // Ask our CellML annotation view widget to finalise the given CellML file

    mViewWidget->finalize(pFileName);
}

//==============================================================================

QString CellMLAnnotationViewPlugin::viewName() const
{
    // Return our CellML annotation view's name

    return tr("CellML Annotation");
}

//==============================================================================

QIcon CellMLAnnotationViewPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return QIcon();
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
