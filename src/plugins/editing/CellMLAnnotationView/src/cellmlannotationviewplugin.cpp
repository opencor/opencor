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

    descriptions.insert("en", QString::fromUtf8("A plugin to annotate CellML files"));
    descriptions.insert("fr", QString::fromUtf8("Une extension pour annoter des fichiers CellML"));

    return new PluginInfo(PluginInfo::FormatVersion001,
                          PluginInfo::Gui,
                          PluginInfo::Editing,
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

bool CellMLAnnotationViewPlugin::hasViewWidget(const QString &pFileName) const
{
    // Return whether a view widget exists for the given file name

    return mViewWidgets.value(pFileName);
}

//==============================================================================

void CellMLAnnotationViewPlugin::deleteViewWidget(const QString &pFileName)
{
    // Remove the view widget from our list, should there be one for the given
    // file name

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

void CellMLAnnotationViewPlugin::retranslateUi()
{
    // Retranslate all of our CellML annotation view widgets

    foreach (CellmlAnnotationViewWidget *viewWidget, mViewWidgets)
        viewWidget->retranslateUi();
}

//==============================================================================

bool CellMLAnnotationViewPlugin::saveFile(const QString &pOldFileName,
                                          const QString &pNewFileName)
{
    // Retrieve the view widget associated with the 'old' file name

    CellmlAnnotationViewWidget *viewWidget = mViewWidgets.value(pOldFileName);

    // Save the CellML file, should we have a valid view widget

    if (viewWidget) {
        // We have a view widget, so we can try to save the file

        if (viewWidget->cellmlFile()->save(pNewFileName)) {
            // The file was properly saved, so check if we need to update our
            // view widgets mapping

            if (pNewFileName.compare(pOldFileName)) {
                // The old and new file names are different, so we do need to
                // update our view widgets mapping

                mViewWidgets.insert(pNewFileName, mViewWidgets.value(pOldFileName));
                mViewWidgets.remove(pOldFileName);
            }

            return true;
        } else {
            // The file couldn't be saved, so...

            return false;
        }
    } else {
        // No view widget exists for the file, so...

        return false;
    }
}

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
