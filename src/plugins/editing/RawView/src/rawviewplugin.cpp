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
// RawView plugin
//==============================================================================

#include "rawviewplugin.h"
#include "rawviewwidget.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace RawView {

//==============================================================================

PLUGININFO_FUNC RawViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to edit files using a text editor."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour éditer des fichiers à l'aide d'un éditeur de texte."));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::Editing,
                          false,
                          true,
                          QStringList() << "CoreEditing" << "QScintillaSupport",
                          descriptions);
}

//==============================================================================

RawViewPlugin::RawViewPlugin() :
    mViewWidgets(QMap<QString, RawViewWidget *>())
{
    // Set our settings

    mGuiSettings->setView(GuiViewSettings::Editing, QStringList());
}

//==============================================================================

RawViewPlugin::~RawViewPlugin()
{
    // Delete our view widgets

    foreach (QWidget *viewWidget, mViewWidgets)
        delete viewWidget;
}

//==============================================================================
// GUI interface
//==============================================================================

void RawViewPlugin::changeEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);

    // We don't handle this interface...
}

//==============================================================================

void RawViewPlugin::updateGui(Plugin *pViewPlugin, const QString &pFileName)
{
    Q_UNUSED(pViewPlugin);
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void RawViewPlugin::initializeView()
{
    // We don't handle this interface...
}

//==============================================================================

void RawViewPlugin::finalizeView()
{
    // We don't handle this interface...
}

//==============================================================================

QWidget * RawViewPlugin::viewWidget(const QString &pFileName,
                                    const bool &pCreate)
{
    // Retrieve from our list the view widget associated with the file name

    RawViewWidget *res = mViewWidgets.value(pFileName);

    // Create a new view widget, if none could be retrieved

    if (!res && pCreate) {
        res = new RawViewWidget(pFileName, mMainWindow);

        // Keep track of our new view widget

        mViewWidgets.insert(pFileName, res);
    }

    // Return our view widget

    return res;
}

//==============================================================================

void RawViewPlugin::removeViewWidget(const QString &pFileName)
{
    // Remove the view widget from our list, should there be one for the given
    // file name

    RawViewWidget *viewWidget = mViewWidgets.value(pFileName);

    if (viewWidget) {
        // There is a view widget for the given file name, so delete it and
        // remove it from our list

        delete viewWidget;

        mViewWidgets.remove(pFileName);
    }
}

//==============================================================================

QString RawViewPlugin::viewName() const
{
    // Return our raw view's name

    return tr("Raw");
}

//==============================================================================

QIcon RawViewPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return QIcon();
}

//==============================================================================

bool RawViewPlugin::saveFile(const QString &pOldFileName,
                             const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void RawViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void RawViewPlugin::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so let its corresponding view widget
    // know about it

    RawViewWidget *crtViewWidget = mViewWidgets.value(pFileName);

    if (crtViewWidget)
        crtViewWidget->fileReloaded();
}

//==============================================================================

void RawViewPlugin::fileSaved(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void RawViewPlugin::fileRenamed(const QString &pOldFileName,
                                const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...
}

//==============================================================================

void RawViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

bool RawViewPlugin::canClose()
{
    // We don't handle this interface...

    return true;
}

//==============================================================================
// I18n interface
//==============================================================================

void RawViewPlugin::retranslateUi()
{
    // We don't handle this interface...
}

//==============================================================================

}   // namespace RawView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
