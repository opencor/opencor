//==============================================================================
// CellMLAnnotationView plugin
//==============================================================================

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
                          QStringList() << "CoreCellMLEditing" << "RICORDOSupport",
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(CellMLAnnotationView, CellMLAnnotationViewPlugin)

//==============================================================================

CellMLAnnotationViewPlugin::CellMLAnnotationViewPlugin() :
    mSizes(QList<int>()),
    mVerticalSizes(QList<int>())
{
    // Set our settings

    mGuiSettings->addView(GuiViewSettings::Editing, 0);
}

//==============================================================================

static const QString SettingsCellmlAnnotationWidget                   = "CellmlAnnotationWidget";
static const QString SettingsCellmlAnnotationWidgetSizesCount         = "SizesCount";
static const QString SettingsCellmlAnnotationWidgetSizes              = "Sizes%1";
static const QString SettingsCellmlAnnotationWidgetVerticalSizesCount = "VerticalSizesCount";
static const QString SettingsCellmlAnnotationWidgetVerticalSizes      = "VerticalSizes%1";

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

            mSizes << 0.35*qApp->desktop()->screenGeometry().width()
                      << 0.65*qApp->desktop()->screenGeometry().width();
        } else {
            // There are previous sizes, so use them to initialise mSizes

            for (int i = 0; i < sizesCount; ++i)
                mSizes << pSettings->value(SettingsCellmlAnnotationWidgetSizes.arg(QString::number(i))).toInt();
        }

        // Vertical sizes

        int verticalSizesCount = pSettings->value(SettingsCellmlAnnotationWidgetVerticalSizesCount, 0).toInt();

        if (!verticalSizesCount) {
            // There are no previous vertical sizes, so get some default ones

            mVerticalSizes << 0.75*qApp->desktop()->screenGeometry().height()
                           << 0.25*qApp->desktop()->screenGeometry().height();
        } else {
            // There are previous vertical sizes, so use them to initialise
            // mVerticalSizes

            for (int i = 0; i < verticalSizesCount; ++i)
                mVerticalSizes << pSettings->value(SettingsCellmlAnnotationWidgetVerticalSizes.arg(QString::number(i))).toInt();
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
            pSettings->setValue(SettingsCellmlAnnotationWidgetSizes.arg(QString::number(i)), mSizes.at(i));

        // Vertical sizes

        pSettings->setValue(SettingsCellmlAnnotationWidgetVerticalSizesCount, mVerticalSizes.count());

        for (int i = 0, iMax = mVerticalSizes.count(); i < iMax; ++i)
            pSettings->setValue(SettingsCellmlAnnotationWidgetVerticalSizes.arg(QString::number(i)), mVerticalSizes.at(i));
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

    CellmlAnnotationViewWidget *widget = new CellmlAnnotationViewWidget(mMainWindow, pFileName);

    // Initialise our new CellML annotation view widget's sizes

    widget->updateSplitter(mSizes);
    widget->listsWidget()->updateVerticalSplitter(mVerticalSizes);

    // Keep track of the splitter move in our new CellML annotation view widget

    connect(widget, SIGNAL(splitterMoved(const QList<int> &)),
            this, SLOT(splitterMoved(const QList<int> &)));
    connect(widget->listsWidget(), SIGNAL(verticalSplitterMoved(const QList<int> &)),
            this, SLOT(verticalSplitterMoved(const QList<int> &)));

    // Some other connections to handle splitter moves between our CellML
    // annotation view widgets

    for (int i = 0, iMax = mWidgets.count(); i < iMax; ++i) {
        // Make sur that our new CellML annotation view widget is aware of any
        // splitter move occuring in the other CellML annotation view widget

        connect(widget, SIGNAL(splitterMoved(const QList<int> &)),
                mWidgets.at(i), SLOT(updateSplitter(const QList<int> &)));
        connect(widget->listsWidget(), SIGNAL(verticalSplitterMoved(const QList<int> &)),
                mWidgets.at(i)->listsWidget(), SLOT(updateVerticalSplitter(const QList<int> &)));

        // Make sur that the other CellML annotation view widget is aware of any
        // splitter move occuring in our new CellML annotation view widget

        connect(mWidgets.at(i), SIGNAL(splitterMoved(const QList<int> &)),
                widget, SLOT(updateSplitter(const QList<int> &)));
        connect(mWidgets.at(i)->listsWidget(), SIGNAL(verticalSplitterMoved(const QList<int> &)),
                widget->listsWidget(), SLOT(updateVerticalSplitter(const QList<int> &)));
    }

    // Keep track of our new CellML annotation view widget

    mWidgets.append(widget);

    // We are all done, so return our new CellML annotation view widget

    return widget;
}

//==============================================================================

QString CellMLAnnotationViewPlugin::viewName(const int &pViewIndex)
{
    // We have only one view, so return its name otherwise call the GuiInterface
    // implementation of viewName

    switch (pViewIndex) {
    case 0:
        return tr("CellML Annotation");
    default:
        return GuiInterface::viewName(pViewIndex);
    }
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

void CellMLAnnotationViewPlugin::verticalSplitterMoved(const QList<int> &pVerticalSizes)
{
    // The vertical splitter of one of our CellML annotation view widgets has
    // been moved, so update things

    mVerticalSizes = pVerticalSizes;
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
