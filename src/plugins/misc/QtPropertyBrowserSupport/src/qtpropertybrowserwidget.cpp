//==============================================================================
// QtPropertyBrowserWidget class
//==============================================================================

#include "qtpropertybrowserwidget.h"

//==============================================================================

#include <QHeaderView>
#include <QSettings>
#include <QTreeWidget>

//==============================================================================

#include <QtVariantPropertyManager>

//==============================================================================

namespace OpenCOR {
namespace QtPropertyBrowserSupport {

//==============================================================================

QtPropertyBrowserWidget::QtPropertyBrowserWidget(QWidget *pParent) :
    QtTreePropertyBrowser(pParent)
{
    // Customise ourselves

    mPropertyManager = new QtVariantPropertyManager(this);
    QtVariantEditorFactory *editorFactory = new QtVariantEditorFactory(this);

    setAlternatingRowColors(false);
    setFactoryForManager(mPropertyManager, editorFactory);
    setPropertiesWithoutValueMarked(true);
    setResizeMode(QtTreePropertyBrowser::Interactive);
    setRootIsDecorated(false);

#ifdef Q_WS_MAC
    treeWidget()->setAttribute(Qt::WA_MacShowFocusRect, 0);
    // Note: the above removes the focus border since it messes up the look of
    //       our property editor...
#endif
    treeWidget()->setFrameShape(QFrame::NoFrame);

    // Make our tree widget our focus proxy

    setFocusProxy(treeWidget());
}

//==============================================================================

void QtPropertyBrowserWidget::retranslateUi()
{
    // Retranslate our header labels

    treeWidget()->setHeaderLabels(QStringList() << tr("Property")
                                                << tr("Value"));
}

//==============================================================================

static const QString SettingsColumnWidth = "ColumnWidth%1";

//==============================================================================

void QtPropertyBrowserWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the width of each column
    // Note: to use splitterPosition()/setSplitterPosition() is not sufficient
    //       since we also need to retrieve the width of the value column
    //       otherwise we may get a horizontal scroll bar when we wouldn't, had
    //       we been able to retrieve the width of the value column...

    QTreeWidget *propertyWidget = treeWidget();

    for (int i = 0, iMax = propertyWidget->header()->count(); i < iMax; ++i)
        propertyWidget->setColumnWidth(i, pSettings->value(SettingsColumnWidth.arg(i),
                                                           propertyWidget->columnWidth(i)).toInt());
}

//==============================================================================

void QtPropertyBrowserWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the width of each column
    // Note: see note above in loadSettings()...

    QTreeWidget *propertyWidget = treeWidget();

    for (int i = 0, iMax = propertyWidget->header()->count(); i < iMax; ++i)
        pSettings->setValue(SettingsColumnWidth.arg(i),
                            propertyWidget->columnWidth(i));
}

//==============================================================================

QtVariantProperty * QtPropertyBrowserWidget::addProperty(const int pType,
                                                         const QString &pName)
{
    // Create a property

    QtVariantProperty *res = mPropertyManager->addProperty(pType, pName);

    // Add it to our GUI

    QtAbstractPropertyBrowser::addProperty(res);

    // Return it

    return res;
}

//==============================================================================

void QtPropertyBrowserWidget::selectFirstProperty()
{
    // Select the first property, if any

    setCurrentItem(items(properties().first()).first());
}

//==============================================================================

}   // namespace QScintillaSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
