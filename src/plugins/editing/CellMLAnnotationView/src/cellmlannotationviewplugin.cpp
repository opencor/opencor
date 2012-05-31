//==============================================================================
// CellMLAnnotationView plugin
//==============================================================================

#include "cellmlannotationviewcellmldetailswidget.h"
#include "cellmlannotationviewdetailswidget.h"
#include "cellmlannotationviewlistswidget.h"
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
    mCellmlDetailsWidgetSizes(QList<int>())
{
    // Set our settings

    mGuiSettings->setView(GuiViewSettings::Editing);
}

//==============================================================================

static const QString SettingsCellmlAnnotationWidget                              = "CellmlAnnotationWidget";
static const QString SettingsCellmlAnnotationWidgetSizesCount                    = "SizesCount";
static const QString SettingsCellmlAnnotationWidgetSizes                         = "Sizes%1";
static const QString SettingsCellmlAnnotationWidgetListsWidgetSizesCount         = "ListsWidgetSizesCount";
static const QString SettingsCellmlAnnotationWidgetListsWidgetSizes              = "ListsWidgetSizes%1";
static const QString SettingsCellmlAnnotationWidgetCellmlDetailsWidgetSizesCount = "CellmlDetailsWidgetSizesCount";
static const QString SettingsCellmlAnnotationWidgetCellmlDetailsWidgetSizes      = "CellmlDetailsWidgetSizes%1";

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

        // Lists widget sizes

        int listsWidgetSizesCount = pSettings->value(SettingsCellmlAnnotationWidgetListsWidgetSizesCount, 0).toInt();

        if (!listsWidgetSizesCount) {
            // There are no previous lists widget sizes, so get some default
            // ones

            mListsWidgetSizes << 0.75*qApp->desktop()->screenGeometry().height()
                              << 0.25*qApp->desktop()->screenGeometry().height();
        } else {
            // There are previous lists widget sizes, so use them to initialise
            // mListsWidgetSizes

            for (int i = 0; i < listsWidgetSizesCount; ++i)
                mListsWidgetSizes << pSettings->value(SettingsCellmlAnnotationWidgetListsWidgetSizes.arg(QString::number(i))).toInt();
        }

        // CellML details widget sizes

        int cellmlDetailsWidgetSizesCount = pSettings->value(SettingsCellmlAnnotationWidgetCellmlDetailsWidgetSizesCount, 0).toInt();

        if (!cellmlDetailsWidgetSizesCount) {
            // There are no previous lists widget sizes, so get some default
            // ones

            mCellmlDetailsWidgetSizes << 0.25*qApp->desktop()->screenGeometry().height()
                                      << 0.25*qApp->desktop()->screenGeometry().height()
                                      << 0.50*qApp->desktop()->screenGeometry().height();
        } else {
            // There are previous lists widget sizes, so use them to initialise
            // mCellmlDetailsWidgetSizes

            for (int i = 0; i < cellmlDetailsWidgetSizesCount; ++i)
                mCellmlDetailsWidgetSizes << pSettings->value(SettingsCellmlAnnotationWidgetCellmlDetailsWidgetSizes.arg(QString::number(i))).toInt();
        }
    pSettings->endGroup();
}

//==============================================================================

void CellMLAnnotationViewPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of the tree view's and CellML annotation's width
    // Note: we must also keep track of the CellML annotation's width because
    //       when loading our settings (see above), the widget doesn't yet have
    //       a 'proper' width, so we couldn't simply assume that the Cellml
    //       annotation's initial width is this widget's width minus the tree
    //       view's initial width, so...

    pSettings->beginGroup(SettingsCellmlAnnotationWidget);
        // Sizes

        pSettings->setValue(SettingsCellmlAnnotationWidgetSizesCount, mSizes.count());

        for (int i = 0, iMax = mSizes.count(); i < iMax; ++i)
            pSettings->setValue(SettingsCellmlAnnotationWidgetSizes.arg(QString::number(i)), mSizes[i]);

        // Lists widget sizes

        pSettings->setValue(SettingsCellmlAnnotationWidgetListsWidgetSizesCount, mListsWidgetSizes.count());

        for (int i = 0, iMax = mListsWidgetSizes.count(); i < iMax; ++i)
            pSettings->setValue(SettingsCellmlAnnotationWidgetListsWidgetSizes.arg(QString::number(i)), mListsWidgetSizes[i]);

        // CellML details widget sizes

        pSettings->setValue(SettingsCellmlAnnotationWidgetCellmlDetailsWidgetSizesCount, mCellmlDetailsWidgetSizes.count());

        for (int i = 0, iMax = mCellmlDetailsWidgetSizes.count(); i < iMax; ++i)
            pSettings->setValue(SettingsCellmlAnnotationWidgetCellmlDetailsWidgetSizes.arg(QString::number(i)), mCellmlDetailsWidgetSizes[i]);
    pSettings->endGroup();
}

//==============================================================================

QWidget * CellMLAnnotationViewPlugin::newViewWidget(const QString &pFileName)
{
    // Check that we are dealing with a CellML file

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        // We are not dealing with a CellML file, so...

        return 0;

    // We are dealing with a CellML file, so create a new CellML annotation view
    // widget

    CellmlAnnotationViewWidget *widget = new CellmlAnnotationViewWidget(mMainWindow, this, pFileName);

    // Initialise our new CellML annotation view widget's sizes

    widget->setSizes(mSizes);
    widget->listsWidget()->setSizes(mListsWidgetSizes);
    widget->detailsWidget()->cellmlDetails()->setSizes(mCellmlDetailsWidgetSizes);

    // Keep track of the splitter move in our new CellML annotation view widget

    connect(widget, SIGNAL(splitterMoved(const QList<int> &)),
            this, SLOT(splitterMoved(const QList<int> &)));
    connect(widget->listsWidget(), SIGNAL(splitterMoved(const QList<int> &)),
            this, SLOT(listsWidgetSplitterMoved(const QList<int> &)));
    connect(widget->detailsWidget()->cellmlDetails(), SIGNAL(splitterMoved(const QList<int> &)),
            this, SLOT(cellmlDetailsWidgetSplitterMoved(const QList<int> &)));

    // Some other connections to handle splitter moves between our CellML
    // annotation view widgets

    for (int i = 0, iMax = mWidgets.count(); i < iMax; ++i) {
        // Make sur that our new CellML annotation view widget is aware of any
        // splitter move occuring in the other CellML annotation view widget

        connect(widget, SIGNAL(splitterMoved(const QList<int> &)),
                mWidgets[i], SLOT(updateSizes(const QList<int> &)));
        connect(widget->listsWidget(), SIGNAL(splitterMoved(const QList<int> &)),
                mWidgets[i]->listsWidget(), SLOT(updateSizes(const QList<int> &)));
        connect(widget->detailsWidget()->cellmlDetails(), SIGNAL(splitterMoved(const QList<int> &)),
                mWidgets[i]->detailsWidget()->cellmlDetails(), SLOT(updateSizes(const QList<int> &)));

        // Make sur that the other CellML annotation view widget is aware of any
        // splitter move occuring in our new CellML annotation view widget

        connect(mWidgets[i], SIGNAL(splitterMoved(const QList<int> &)),
                widget, SLOT(updateSizes(const QList<int> &)));
        connect(mWidgets[i]->listsWidget(), SIGNAL(splitterMoved(const QList<int> &)),
                widget->listsWidget(), SLOT(updateSizes(const QList<int> &)));
        connect(mWidgets[i]->detailsWidget()->cellmlDetails(), SIGNAL(splitterMoved(const QList<int> &)),
                widget->detailsWidget()->cellmlDetails(), SLOT(updateSizes(const QList<int> &)));
    }

    // Keep track of our new CellML annotation view widget

    mWidgets.append(widget);

    // We are all done, so return our new CellML annotation view widget

    return widget;
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

    foreach (CellmlAnnotationViewWidget *widget, mWidgets)
        widget->retranslateUi();
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

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
