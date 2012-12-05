//==============================================================================
// QtPropertyBrowserWidget class
//==============================================================================

#include "qtpropertybrowserwidget.h"

//==============================================================================

#include <QHeaderView>
#include <QScrollBar>
#include <QSettings>
#include <QTreeWidget>

//==============================================================================

#include <QtVariantPropertyManager>

//==============================================================================

namespace OpenCOR {
namespace QtPropertyBrowserSupport {

//==============================================================================

int QtPropertyBrowserTreeWidget::rowHeight(const QModelIndex &pIndex) const
{
    // Return the row's height (using the protected version of rowHeight()
    // defined in QTreeWidget)

    return QTreeWidget::rowHeight(pIndex);
}

//==============================================================================

void QtPropertyBrowserWidget::constructor(const bool &pAutoResizeHeight)
{
    // Customise ourselves

    mAutoResizeHeight = pAutoResizeHeight;

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

    if (pAutoResizeHeight)
        treeWidget()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Make our tree widget our focus proxy

    setFocusProxy(treeWidget());
}

//==============================================================================

QtPropertyBrowserWidget::QtPropertyBrowserWidget(const bool &pAutoResizeHeight,
                                                 QWidget *pParent) :
    QtTreePropertyBrowser(pParent)
{
    // Construct our object

    constructor(pAutoResizeHeight);
}

//==============================================================================

QtPropertyBrowserWidget::QtPropertyBrowserWidget(QWidget *pParent) :
    QtTreePropertyBrowser(pParent)
{
    // Construct our object

    constructor();
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

QSize QtPropertyBrowserWidget::sizeHint() const
{
    // Return either our default/ideal size, depending on the case

    if (mAutoResizeHeight) {
        // We automatically resize our height, so determine our ideal size which
        // is based on the width of our different columns, and the height of our
        // header and our different rows

        QtPropertyBrowserTreeWidget *propertyWidget = static_cast<QtPropertyBrowserTreeWidget *>(treeWidget());
        int hintWidth  = 0;
        int hintHeight = propertyWidget->header()->height();

        for (int i = 0, iMax = propertyWidget->header()->count(); i < iMax; ++i)
            hintWidth += propertyWidget->columnWidth(i);

        for (int i = 0, iMax = properties().count(); i < iMax; ++i)
            hintHeight += propertyWidget->rowHeight(propertyWidget->model()->index(i, 0));

        return QSize(hintWidth, hintHeight);
    } else {
        // We don't automatically resize our height, so...

        return QtTreePropertyBrowser::sizeHint();
    }
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

void QtPropertyBrowserWidget::resizeEvent(QResizeEvent *pEvent)
{
    // Default handling of the event

    QtTreePropertyBrowser::resizeEvent(pEvent);

    // Resize our height, so that we don't have any blank space at the bottom

    if (mAutoResizeHeight) {
        QSize idealSize = sizeHint();
        QTreeWidget *propertyWidget = treeWidget();

        propertyWidget->setFixedHeight( idealSize.height()
                                       +((width() < idealSize.width())?
                                             propertyWidget->horizontalScrollBar()->height():
                                             0));
        // Note: the new height consists of our ideal height to which we add the
        //       height of our horizontal scroll bar, should it be shown (i.e.
        //       if our width is smaller than that of our ideal size)...
    }
}

//==============================================================================

}   // namespace QScintillaSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
