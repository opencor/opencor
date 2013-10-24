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

#include "cellmlannotationviewmetadatadetailswidget.h"
#include "cellmlannotationviewplugin.h"
#include "cellmlannotationviewwidget.h"
#include "cellmlfilemanager.h"
#include "cellmlsupportplugin.h"
#include "corecellmleditingplugin.h"
#include "coreeditingplugin.h"

//==============================================================================

#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

PLUGININFO_FUNC CellMLAnnotationViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to annotate CellML files."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour annoter des fichiers CellML."));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::Editing,
                          false,
                          true,
                          QStringList() << "CoreCellMLEditing",
                          descriptions);
}

//==============================================================================

CellMLAnnotationViewPlugin::CellMLAnnotationViewPlugin() :
    mSizes(QList<int>()),
    mMetadataDetailsWidgetSizes(QList<int>()),
    mViewWidgets(QMap<QString, CellmlAnnotationViewWidget *>())
{
    // Set our settings

    mGuiSettings->setView(GuiViewSettings::Editing,
                          QStringList() << CellMLSupport::CellmlMimeType);
}

//==============================================================================

CellMLAnnotationViewPlugin::~CellMLAnnotationViewPlugin()
{
    // Delete our view widgets

    foreach (QWidget *viewWidget, mViewWidgets)
        delete viewWidget;
}

//==============================================================================
// Core interface
//==============================================================================

void CellMLAnnotationViewPlugin::initialize()
{
    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::finalize()
{
    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::initialized(const Plugins &pLoadedPlugins)
{
    // Keep track of the CoreEditing and CoreCellMLEditing plugin instances

    foreach (Plugin *loadedPlugin, pLoadedPlugins)
        if (!loadedPlugin->name().compare("CoreEditing"))
            mCoreEditingPlugin = static_cast<CoreEditing::CoreEditingPlugin *>(loadedPlugin->instance());
        else if (!loadedPlugin->name().compare("CoreCellMLEditing"))
            mCoreCellmlEditingPlugin = static_cast<CoreCellMLEditing::CoreCellMLEditingPlugin *>(loadedPlugin->instance());
}

//==============================================================================

static const QString SettingsCellmlAnnotationWidget                                = "CellmlAnnotationWidget";
static const QString SettingsCellmlAnnotationWidgetSizesCount                      = "SizesCount";
static const QString SettingsCellmlAnnotationWidgetSizes                           = "Sizes%1";
static const QString SettingsCellmlAnnotationWidgetMetadataDetailsWidgetSizesCount = "MetadataDetailsWidgetSizesCount";
static const QString SettingsCellmlAnnotationWidgetMetadataDetailsWidgetSizes      = "MetadataDetailsWidgetSizes%1";

//==============================================================================

void CellMLAnnotationViewPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve the size of the different items that make up our splitters
    // Note: we would normally do this in CellmlAnnotationViewWidget, but we
    //       have one instance of it per CellML file and we want to share some
    //       information between the different instances, so we have to do it
    //       here instead...

    pSettings->beginGroup(SettingsCellmlAnnotationWidget);
        // Sizes

        int sizesCount = pSettings->value(SettingsCellmlAnnotationWidgetSizesCount, 0).toInt();

        if (!sizesCount) {
            // There are no previous sizes, so get some default ones

            mSizes << 0.25*qApp->desktop()->screenGeometry().width()
                   << 0.75*qApp->desktop()->screenGeometry().width();
        } else {
            // There are previous sizes, so use them to initialise mSizes

            for (int i = 0; i < sizesCount; ++i)
                mSizes << pSettings->value(SettingsCellmlAnnotationWidgetSizes.arg(i)).toInt();
        }

        // Metadata details view widget sizes

        int metadataDetailsWidgetSizesCount = pSettings->value(SettingsCellmlAnnotationWidgetMetadataDetailsWidgetSizesCount, 0).toInt();

        if (!metadataDetailsWidgetSizesCount) {
            // There are no previous view widget sizes, so get some default ones

            mMetadataDetailsWidgetSizes << 0.25*qApp->desktop()->screenGeometry().height()
                                        << 0.25*qApp->desktop()->screenGeometry().height()
                                        << 0.50*qApp->desktop()->screenGeometry().height();
        } else {
            // There are previous view widget sizes, so use them to initialise
            // mMetadataDetailsWidgetSizes

            for (int i = 0; i < metadataDetailsWidgetSizesCount; ++i)
                mMetadataDetailsWidgetSizes << pSettings->value(SettingsCellmlAnnotationWidgetMetadataDetailsWidgetSizes.arg(i)).toInt();
        }
    pSettings->endGroup();
}

//==============================================================================

void CellMLAnnotationViewPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of the tree view widget's and CellML annotation's width
    // Note: we must also keep track of the CellML annotation's width because
    //       when loading our settings (see above), the view widget doesn't yet
    //       have a 'proper' width, so we couldn't simply assume that the Cellml
    //       annotation's initial width is this view widget's width minus the
    //       tree view widget's initial width, so...

    pSettings->beginGroup(SettingsCellmlAnnotationWidget);
        // Sizes

        pSettings->setValue(SettingsCellmlAnnotationWidgetSizesCount, mSizes.count());

        for (int i = 0, iMax = mSizes.count(); i < iMax; ++i)
            pSettings->setValue(SettingsCellmlAnnotationWidgetSizes.arg(i), mSizes[i]);

        // Metadata details view widget sizes

        pSettings->setValue(SettingsCellmlAnnotationWidgetMetadataDetailsWidgetSizesCount, mMetadataDetailsWidgetSizes.count());

        for (int i = 0, iMax = mMetadataDetailsWidgetSizes.count(); i < iMax; ++i)
            pSettings->setValue(SettingsCellmlAnnotationWidgetMetadataDetailsWidgetSizes.arg(i), mMetadataDetailsWidgetSizes[i]);
    pSettings->endGroup();
}

//==============================================================================

void CellMLAnnotationViewPlugin::settingsLoaded(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::handleArguments(const QStringList &pArguments)
{
    Q_UNUSED(pArguments);

    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::runCliCommand(const QString &pCommand,
                                               const QStringList &pArguments,
                                               int *pRes)
{
    Q_UNUSED(pCommand);
    Q_UNUSED(pArguments);

    // We don't handle this interface...

    *pRes = 0;
}

//==============================================================================
// GUI interface
//==============================================================================

void CellMLAnnotationViewPlugin::changeEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);

    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::updateGui(Plugin *pViewPlugin,
                                           const QString &pFileName)
{
    Q_UNUSED(pViewPlugin);
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::initializeView()
{
    // Hide our inherited File|New actions and Edit menu

    mCoreEditingPlugin->setEditMenuVisible(false);
    mCoreCellmlEditingPlugin->setFileNewActionsVisible(false);
}

//==============================================================================

void CellMLAnnotationViewPlugin::finalizeView()
{
    // Re-show our inherited File|New actions and Edit menu

    mCoreEditingPlugin->setEditMenuVisible(true);
    mCoreCellmlEditingPlugin->setFileNewActionsVisible(true);
}

//==============================================================================

QWidget * CellMLAnnotationViewPlugin::viewWidget(const QString &pFileName)
{
    // Check that we are dealing with a CellML file

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        // We are not dealing with a CellML file, so...

        return 0;

    // Retrieve the view widget associated with the file name

    CellmlAnnotationViewWidget *res = mViewWidgets.value(pFileName);

    // Create a new view widget, if none could be retrieved

    if (!res) {
        res = new CellmlAnnotationViewWidget(this, pFileName, mMainWindow);

        // Initialise our new view widget's sizes

        res->setSizes(mSizes);
        res->metadataDetails()->splitter()->setSizes(mMetadataDetailsWidgetSizes);

        // Keep track of the splitter move in our new view widget

        connect(res, SIGNAL(splitterMoved(const QList<int> &)),
                this, SLOT(splitterMoved(const QList<int> &)));
        connect(res->metadataDetails(), SIGNAL(splitterMoved(const QList<int> &)),
                this, SLOT(metadataDetailsWidgetSplitterMoved(const QList<int> &)));

        // Some other connections to handle splitter moves between our view
        // widgets

        foreach (CellmlAnnotationViewWidget *viewWidget, mViewWidgets) {
            // Make sur that our new view widget is aware of any splitter move
            // occuring in the other view widget

            connect(res, SIGNAL(splitterMoved(const QList<int> &)),
                    viewWidget, SLOT(updateSizes(const QList<int> &)));
            connect(res->metadataDetails(), SIGNAL(splitterMoved(const QList<int> &)),
                    viewWidget->metadataDetails(), SLOT(updateSizes(const QList<int> &)));

            // Make sur that the other view widget is aware of any splitter move
            // occuring in our new view widget

            connect(viewWidget, SIGNAL(splitterMoved(const QList<int> &)),
                    res, SLOT(updateSizes(const QList<int> &)));
            connect(viewWidget->metadataDetails(), SIGNAL(splitterMoved(const QList<int> &)),
                    res->metadataDetails(), SLOT(updateSizes(const QList<int> &)));
        }

        // Keep track of our new view widget

        mViewWidgets.insert(pFileName, res);
    }

    // Return our view widget

    return res;
}

//==============================================================================

QWidget * CellMLAnnotationViewPlugin::removeViewWidget(const QString &pFileName)
{
    // Remove the view widget from our list, should there be one for the given
    // file name

    CellmlAnnotationViewWidget *res = mViewWidgets.value(pFileName);

    if (res) {
        // There is a view widget for the given file name, so delete it and
        // remove it from our list

        delete res;

        mViewWidgets.remove(pFileName);
    }

    return res;
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

bool CellMLAnnotationViewPlugin::saveFile(const QString &pOldFileName,
                                          const QString &pNewFileName)
{
    // Retrieve the view widget associated with the 'old' file name and, if any,
    // save it

    CellmlAnnotationViewWidget *viewWidget = mViewWidgets.value(pOldFileName);

    return viewWidget?viewWidget->cellmlFile()->save(pNewFileName):false;
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileSaved(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileRenamed(const QString &pOldFileName,
                                             const QString &pNewFileName)
{
    // Update our view widgets mapping

    mViewWidgets.insert(pNewFileName, mViewWidgets.value(pOldFileName));
    mViewWidgets.remove(pOldFileName);
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

bool CellMLAnnotationViewPlugin::canClose()
{
    // We don't handle this interface...

    return true;
}

//==============================================================================
// I18n interface
//==============================================================================

void CellMLAnnotationViewPlugin::retranslateUi()
{
    // Retranslate all of our CellML annotation view widgets

    foreach (CellmlAnnotationViewWidget *viewWidget, mViewWidgets)
        viewWidget->retranslateUi();
}

//==============================================================================
// Plugin specific
//==============================================================================

void CellMLAnnotationViewPlugin::splitterMoved(const QList<int> &pSizes)
{
    // The splitter of one of our CellML annotation view widgets has been moved,
    // so update things

    mSizes = pSizes;
}

//==============================================================================

void CellMLAnnotationViewPlugin::metadataDetailsWidgetSplitterMoved(const QList<int> &pSizes)
{
    // The splitter of one of our CellML annotation view's metadata details
    // widgets has been moved, so update things

    mMetadataDetailsWidgetSizes = pSizes;
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
