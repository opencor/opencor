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

    return new PluginInfo(PluginInfo::Editing, true,
                          QStringList() << "CellMLSupport",
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
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

static const auto SettingsCellmlAnnotationViewWidget                           = QStringLiteral("CellmlAnnotationViewWidget");
static const auto SettingsCellmlAnnotationViewWidgetSizes                      = QStringLiteral("Sizes");
static const auto SettingsCellmlAnnotationViewWidgetMetadataDetailsWidgetSizes = QStringLiteral("MetadataDetailsWidgetSizes");

//==============================================================================

void CellMLAnnotationViewPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve the size of the different items that make up our splitters
    // Note: we would normally do this in CellmlAnnotationViewWidget, but we
    //       have one instance of it per CellML file and we want to share some
    //       information between the different instances, so we have to do it
    //       here instead...

    qRegisterMetaTypeStreamOperators< QList<int> >("QList<int>");

    pSettings->beginGroup(SettingsCellmlAnnotationViewWidget);
        QVariant defaultSizes = QVariant::fromValue< QList<int> >(QList<int>() << 0.25*qApp->desktop()->screenGeometry().width()
                                                                               << 0.75*qApp->desktop()->screenGeometry().width());
        QVariant defaultMetadataDetailsWidgetSizes = QVariant::fromValue< QList<int> >(QList<int>() << 0.25*qApp->desktop()->screenGeometry().height()
                                                                                                    << 0.25*qApp->desktop()->screenGeometry().height()
                                                                                                    << 0.50*qApp->desktop()->screenGeometry().height());

        mSizes = pSettings->value(SettingsCellmlAnnotationViewWidgetSizes, defaultSizes).value< QList<int> >();
        mMetadataDetailsWidgetSizes = pSettings->value(SettingsCellmlAnnotationViewWidgetMetadataDetailsWidgetSizes, defaultMetadataDetailsWidgetSizes).value< QList<int> >();
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

    pSettings->beginGroup(SettingsCellmlAnnotationViewWidget);
        pSettings->setValue(SettingsCellmlAnnotationViewWidgetSizes, QVariant::fromValue< QList<int> >(mSizes));
        pSettings->setValue(SettingsCellmlAnnotationViewWidgetMetadataDetailsWidgetSizes, QVariant::fromValue< QList<int> >(mMetadataDetailsWidgetSizes));
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
    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::finalizeView()
{
    // We don't handle this interface...
}

//==============================================================================

bool CellMLAnnotationViewPlugin::hasViewWidget(const QString &pFileName)
{
    // Make sure that we are dealing with a CellML file

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        return false;

    // Return whether we have a view widget associated with the given CellML
    // file

    return mViewWidgets.value(pFileName);
}

//==============================================================================

QWidget * CellMLAnnotationViewPlugin::viewWidget(const QString &pFileName,
                                                 const bool &pCreate)
{
    // Make sure that we are dealing with a CellML file

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        return 0;

    // Retrieve the view widget associated with the given CellML file

    CellmlAnnotationViewWidget *res = mViewWidgets.value(pFileName);

    // Create a new view widget, if none could be retrieved

    if (!res && pCreate) {
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

void CellMLAnnotationViewPlugin::removeViewWidget(const QString &pFileName)
{
    // Make sure that we are dealing with a CellML file

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        return;

    // Remove the view widget from our list, should there be one for the given
    // CellML file

    CellmlAnnotationViewWidget *viewWidget = mViewWidgets.value(pFileName);

    if (viewWidget) {
        // There is a view widget for the given file name, so delete it and
        // remove it from our list

        delete viewWidget;

        mViewWidgets.remove(pFileName);
    }
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

void CellMLAnnotationViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    // The given file has been un/locked, so retranslate ourselves (since some
    // messages may be locked-dependent)
    // Note: our plugin is such that retranslating it will update the GUI (since
    //       it was easier/faster to do it that way), so all we had to do was to
    //       to make those updateGui() methods locked-dependent...

    if (mViewWidgets.value(pFileName))
        retranslateUi();
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileModified(const QString &pFileName,
                                              const bool &pModified)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pModified);

    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so let its corresponding view widget
    // know about it

    CellmlAnnotationViewWidget *viewWidget = mViewWidgets.value(pFileName);

    if (viewWidget)
        viewWidget->fileReloaded();
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileRenamed(const QString &pOldFileName,
                                             const QString &pNewFileName)
{
    // A file has been renamed, so update our view widgets mapping

    CellmlAnnotationViewWidget *viewWidget = mViewWidgets.value(pOldFileName);

    if (viewWidget) {
        mViewWidgets.insert(pNewFileName, viewWidget);
        mViewWidgets.remove(pOldFileName);
    }
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
