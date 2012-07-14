//==============================================================================
// CellMLAnnotationView plugin
//==============================================================================

#include "cellmlannotationviewcellmldetailswidget.h"
#include "cellmlannotationviewcellmlmetadatadetailswidget.h"
#include "cellmlannotationviewdetailswidget.h"
#include "cellmlannotationviewlistswidget.h"
#include "cellmlannotationviewmetadatadetailswidget.h"
#include "cellmlannotationviewplugin.h"
#include "cellmlannotationviewwidget.h"
#include "cellmlfilemanager.h"

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

    descriptions.insert("en", "A plugin to annotate CellML files");
    descriptions.insert("fr", "Une extension pour annoter des fichiers CellML");

    return new PluginInfo(PluginInfo::V001,
                          PluginInfo::Gui,
                          PluginInfo::Editing,
                          true,
                          QStringList() << "CoreCellMLEditing" << "QJson" << "RICORDOSupport",
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(CellMLAnnotationView, CellMLAnnotationViewPlugin)

//==============================================================================

CellMLAnnotationViewPlugin::CellMLAnnotationViewPlugin() :
    mSizes(QList<int>()),
    mListsWidgetSizes(QList<int>()),
    mCellmlDetailsWidgetSizes(QList<int>()),
    mCellmlMetadataDetailsWidgetSizes(QList<int>()),
    mMetadataDetailsWidgetSizes(QList<int>()),
    mViewWidgets(QMap<QString, CellmlAnnotationViewWidget *>())
{
    // Set our settings

    mGuiSettings->setView(GuiViewSettings::Editing);
}

//==============================================================================

CellMLAnnotationViewPlugin::~CellMLAnnotationViewPlugin()
{
    // Delete our view widgets

    foreach (QWidget *viewWidget, mViewWidgets)
        delete viewWidget;
}

//==============================================================================

static const QString SettingsCellmlAnnotationWidget                                      = "CellmlAnnotationWidget";
static const QString SettingsCellmlAnnotationWidgetSizesCount                            = "SizesCount";
static const QString SettingsCellmlAnnotationWidgetSizes                                 = "Sizes%1";
static const QString SettingsCellmlAnnotationWidgetListsWidgetSizesCount                 = "ListsWidgetSizesCount";
static const QString SettingsCellmlAnnotationWidgetListsWidgetSizes                      = "ListsWidgetSizes%1";
static const QString SettingsCellmlAnnotationWidgetCellmlDetailsWidgetSizesCount         = "CellmlDetailsWidgetSizesCount";
static const QString SettingsCellmlAnnotationWidgetCellmlDetailsWidgetSizes              = "CellmlDetailsWidgetSizes%1";
static const QString SettingsCellmlAnnotationWidgetCellmlMetadataDetailsWidgetSizesCount = "CellmlMetadataDetailsWidgetSizesCount";
static const QString SettingsCellmlAnnotationWidgetCellmlMetadataDetailsWidgetSizes      = "CellmlMetadataDetailsWidgetSizes%1";
static const QString SettingsCellmlAnnotationWidgetMetadataDetailsWidgetSizesCount       = "MetadataDetailsWidgetSizesCount";
static const QString SettingsCellmlAnnotationWidgetMetadataDetailsWidgetSizes            = "MetadataDetailsWidgetSizes%1";

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
                mSizes << pSettings->value(SettingsCellmlAnnotationWidgetSizes.arg(QString::number(i))).toInt();
        }

        // View widget sizes

        int listsWidgetSizesCount = pSettings->value(SettingsCellmlAnnotationWidgetListsWidgetSizesCount, 0).toInt();

        if (!listsWidgetSizesCount) {
            // There are no previous view widget sizes, so get some default ones

            mListsWidgetSizes << 0.65*qApp->desktop()->screenGeometry().height()
                              << 0.35*qApp->desktop()->screenGeometry().height();
        } else {
            // There are previous view widget sizes, so use them to initialise
            // mListsWidgetSizes

            for (int i = 0; i < listsWidgetSizesCount; ++i)
                mListsWidgetSizes << pSettings->value(SettingsCellmlAnnotationWidgetListsWidgetSizes.arg(QString::number(i))).toInt();
        }

        // CellML details view widget sizes

        int cellmlDetailsWidgetSizesCount = pSettings->value(SettingsCellmlAnnotationWidgetCellmlDetailsWidgetSizesCount, 0).toInt();

        if (!cellmlDetailsWidgetSizesCount) {
            // There are no previous view widget sizes, so get some default ones

            mCellmlDetailsWidgetSizes << 0.25*qApp->desktop()->screenGeometry().height()
                                      << 0.75*qApp->desktop()->screenGeometry().height();
        } else {
            // There are previous view widget sizes, so use them to initialise
            // mCellmlDetailsWidgetSizes

            for (int i = 0; i < cellmlDetailsWidgetSizesCount; ++i)
                mCellmlDetailsWidgetSizes << pSettings->value(SettingsCellmlAnnotationWidgetCellmlDetailsWidgetSizes.arg(QString::number(i))).toInt();
        }

        // CellML metadata details view widget sizes

        int cellmlMetadataDetailsWidgetSizesCount = pSettings->value(SettingsCellmlAnnotationWidgetCellmlMetadataDetailsWidgetSizesCount, 0).toInt();

        if (!cellmlMetadataDetailsWidgetSizesCount) {
            // There are no previous view widget sizes, so get some default ones

            mCellmlMetadataDetailsWidgetSizes << 0.25*qApp->desktop()->screenGeometry().height()
                                              << 0.75*qApp->desktop()->screenGeometry().height();
        } else {
            // There are previous view widget sizes, so use them to initialise
            // mCellmlMetadataDetailsWidgetSizes

            for (int i = 0; i < cellmlMetadataDetailsWidgetSizesCount; ++i)
                mCellmlMetadataDetailsWidgetSizes << pSettings->value(SettingsCellmlAnnotationWidgetCellmlMetadataDetailsWidgetSizes.arg(QString::number(i))).toInt();
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
                mMetadataDetailsWidgetSizes << pSettings->value(SettingsCellmlAnnotationWidgetMetadataDetailsWidgetSizes.arg(QString::number(i))).toInt();
        }
    pSettings->endGroup();
}

//==============================================================================

void CellMLAnnotationViewPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of the tree view's and CellML annotation's width
    // Note: we must also keep track of the CellML annotation's width because
    //       when loading our settings (see above), the view widget doesn't yet
    //       have a 'proper' width, so we couldn't simply assume that the Cellml
    //       annotation's initial width is this view widget's width minus the
    //       tree view's initial width, so...

    pSettings->beginGroup(SettingsCellmlAnnotationWidget);
        // Sizes

        pSettings->setValue(SettingsCellmlAnnotationWidgetSizesCount, mSizes.count());

        for (int i = 0, iMax = mSizes.count(); i < iMax; ++i)
            pSettings->setValue(SettingsCellmlAnnotationWidgetSizes.arg(QString::number(i)), mSizes[i]);

        // View widget sizes

        pSettings->setValue(SettingsCellmlAnnotationWidgetListsWidgetSizesCount, mListsWidgetSizes.count());

        for (int i = 0, iMax = mListsWidgetSizes.count(); i < iMax; ++i)
            pSettings->setValue(SettingsCellmlAnnotationWidgetListsWidgetSizes.arg(QString::number(i)), mListsWidgetSizes[i]);

        // CellML details view widget sizes

        pSettings->setValue(SettingsCellmlAnnotationWidgetCellmlDetailsWidgetSizesCount, mCellmlDetailsWidgetSizes.count());

        for (int i = 0, iMax = mCellmlDetailsWidgetSizes.count(); i < iMax; ++i)
            pSettings->setValue(SettingsCellmlAnnotationWidgetCellmlDetailsWidgetSizes.arg(QString::number(i)), mCellmlDetailsWidgetSizes[i]);

        // CellML metadata details view widget sizes

        pSettings->setValue(SettingsCellmlAnnotationWidgetCellmlMetadataDetailsWidgetSizesCount, mCellmlMetadataDetailsWidgetSizes.count());

        for (int i = 0, iMax = mCellmlMetadataDetailsWidgetSizes.count(); i < iMax; ++i)
            pSettings->setValue(SettingsCellmlAnnotationWidgetCellmlMetadataDetailsWidgetSizes.arg(QString::number(i)), mCellmlMetadataDetailsWidgetSizes[i]);

        // Metadata details view widget sizes

        pSettings->setValue(SettingsCellmlAnnotationWidgetMetadataDetailsWidgetSizesCount, mMetadataDetailsWidgetSizes.count());

        for (int i = 0, iMax = mMetadataDetailsWidgetSizes.count(); i < iMax; ++i)
            pSettings->setValue(SettingsCellmlAnnotationWidgetMetadataDetailsWidgetSizes.arg(QString::number(i)), mMetadataDetailsWidgetSizes[i]);
    pSettings->endGroup();
}

//==============================================================================

QWidget * CellMLAnnotationViewPlugin::viewWidget(const QString &pFileName)
{
    // Check that we are dealing with a CellML file

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        // We are not dealing with a CellML file, so...

        return 0;

    // Retrieve from our list the view widget associated with the file name

    CellmlAnnotationViewWidget *res = mViewWidgets.value(pFileName);

    // Create a new view widget, if none could be retrieved

    if (!res) {
        res = new CellmlAnnotationViewWidget(mMainWindow, this, pFileName);

        // Initialise our new view widget's sizes

        res->setSizes(mSizes);
        res->listsWidget()->setSizes(mListsWidgetSizes);
        res->detailsWidget()->cellmlDetails()->setSizes(mCellmlDetailsWidgetSizes);
        res->detailsWidget()->cellmlDetails()->cellmlMetadataDetails()->splitter()->setSizes(mCellmlMetadataDetailsWidgetSizes);
        res->detailsWidget()->metadataDetails()->splitter()->setSizes(mMetadataDetailsWidgetSizes);

        // Keep track of the splitter move in our new view widget

        connect(res, SIGNAL(splitterMoved(const QList<int> &)),
                this, SLOT(splitterMoved(const QList<int> &)));
        connect(res->listsWidget(), SIGNAL(splitterMoved(const QList<int> &)),
                this, SLOT(listsWidgetSplitterMoved(const QList<int> &)));
        connect(res->detailsWidget()->cellmlDetails(), SIGNAL(splitterMoved(const QList<int> &)),
                this, SLOT(cellmlDetailsWidgetSplitterMoved(const QList<int> &)));
        connect(res->detailsWidget()->cellmlDetails()->cellmlMetadataDetails(), SIGNAL(splitterMoved(const QList<int> &)),
                this, SLOT(cellmlMetadataDetailsWidgetSplitterMoved(const QList<int> &)));
        connect(res->detailsWidget()->metadataDetails(), SIGNAL(splitterMoved(const QList<int> &)),
                this, SLOT(metadataDetailsWidgetSplitterMoved(const QList<int> &)));

        // Some other connections to handle splitter moves between our view
        // widgets

        foreach (CellmlAnnotationViewWidget *viewWidget, mViewWidgets) {
            // Make sur that our new view widget is aware of any splitter move
            // occuring in the other view widget

            connect(res, SIGNAL(splitterMoved(const QList<int> &)),
                    viewWidget, SLOT(updateSizes(const QList<int> &)));
            connect(res->listsWidget(), SIGNAL(splitterMoved(const QList<int> &)),
                    viewWidget->listsWidget(), SLOT(updateSizes(const QList<int> &)));
            connect(res->detailsWidget()->cellmlDetails(), SIGNAL(splitterMoved(const QList<int> &)),
                    viewWidget->detailsWidget()->cellmlDetails(), SLOT(updateSizes(const QList<int> &)));
            connect(res->detailsWidget()->cellmlDetails()->cellmlMetadataDetails(), SIGNAL(splitterMoved(const QList<int> &)),
                    viewWidget->detailsWidget()->cellmlDetails()->cellmlMetadataDetails(), SLOT(updateSizes(const QList<int> &)));
            connect(res->detailsWidget()->metadataDetails(), SIGNAL(splitterMoved(const QList<int> &)),
                    viewWidget->detailsWidget()->metadataDetails(), SLOT(updateSizes(const QList<int> &)));

            // Make sur that the other view widget is aware of any splitter move
            // occuring in our new view widget

            connect(viewWidget, SIGNAL(splitterMoved(const QList<int> &)),
                    res, SLOT(updateSizes(const QList<int> &)));
            connect(viewWidget->listsWidget(), SIGNAL(splitterMoved(const QList<int> &)),
                    res->listsWidget(), SLOT(updateSizes(const QList<int> &)));
            connect(viewWidget->detailsWidget()->cellmlDetails(), SIGNAL(splitterMoved(const QList<int> &)),
                    res->detailsWidget()->cellmlDetails(), SLOT(updateSizes(const QList<int> &)));
            connect(viewWidget->detailsWidget()->cellmlDetails()->cellmlMetadataDetails(), SIGNAL(splitterMoved(const QList<int> &)),
                    res->detailsWidget()->cellmlDetails()->cellmlMetadataDetails(), SLOT(updateSizes(const QList<int> &)));
            connect(viewWidget->detailsWidget()->metadataDetails(), SIGNAL(splitterMoved(const QList<int> &)),
                    res->detailsWidget()->metadataDetails(), SLOT(updateSizes(const QList<int> &)));
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
    // Return whether a view widget exists or not for the given file name

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

QString CellMLAnnotationViewPlugin::viewName()
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

void CellMLAnnotationViewPlugin::splitterMoved(const QList<int> &pSizes)
{
    // The splitter of one of our CellML annotation view widgets has been moved,
    // so update things

    mSizes = pSizes;
}

//==============================================================================

void CellMLAnnotationViewPlugin::listsWidgetSplitterMoved(const QList<int> &pSizes)
{
    // The splitter of one of our CellML annotation view's lists widgets has
    // been moved, so update things

    mListsWidgetSizes = pSizes;
}

//==============================================================================

void CellMLAnnotationViewPlugin::cellmlDetailsWidgetSplitterMoved(const QList<int> &pSizes)
{
    // The splitter of one of our CellML annotation view's CellML details
    // widgets has been moved, so update things

    mCellmlDetailsWidgetSizes = pSizes;
}

//==============================================================================

void CellMLAnnotationViewPlugin::cellmlMetadataDetailsWidgetSplitterMoved(const QList<int> &pSizes)
{
    // The splitter of one of our CellML annotation view's CellML metadata
    // details widgets has been moved, so update things

    mCellmlMetadataDetailsWidgetSizes = pSizes;
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
