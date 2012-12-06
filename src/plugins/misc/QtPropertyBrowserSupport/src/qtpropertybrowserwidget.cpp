//==============================================================================
// QtPropertyBrowserWidget class
//==============================================================================

#include "qtpropertybrowserwidget.h"

//==============================================================================

#include <QHeaderView>
#include <QKeyEvent>
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

DoublePropertyManager::DoublePropertyManager(QObject *pParent) :
    QtAbstractPropertyManager(pParent),
    mData(QMap<const QtProperty *, Data>())
{
}

//==============================================================================

QString DoublePropertyManager::unit(QtProperty *pProperty) const
{
    // Make sure that the property exists

    if (!mData.contains(pProperty))
        return QString();

    // Return the property's unit

    return mData[pProperty].unit;
}

//==============================================================================

void DoublePropertyManager::setUnit(QtProperty *pProperty, const QString &pUnit)
{
    // Make sure that the property exists

    if (!mData.contains(pProperty))
        return;

    // Set the property's unit

    Data data = mData[pProperty];

    if (pUnit.compare(data.unit)) {
        data.unit = pUnit;

        mData[pProperty] = data;
    }

    // Let people know that the property's unit has changed

    emit unitChanged(pProperty, pUnit);
}

//==============================================================================

QString DoublePropertyManager::valueText(const QtProperty *pProperty) const
{
    // Make sure that the property exists

    if (!mData.contains(pProperty))
        return QString();

    // Return our property as a string

    return QString::number(mData[pProperty].value);
}

//==============================================================================

void DoublePropertyManager::initializeProperty(QtProperty *pProperty)
{
    // Initialise the property

    mData[pProperty] = Data();
}

//==============================================================================

void DoublePropertyManager::uninitializeProperty(QtProperty *pProperty)
{
    // Uninitialise the property

    mData.remove(pProperty);
}

//==============================================================================

DoubleEditWidget::DoubleEditWidget(QWidget *pParent) :
    QLineEdit(pParent),
    mUnit(QString())
{
#ifdef Q_WS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    // Note: the above removes the focus border since it messes up the look of
    //       our editor
#endif

    // Set a validator which accepts any double

    setValidator(new QRegExpValidator(QRegExp("^[+-]?[0-9]*\\.?[0-9]+([eE][+-]?[0-9]+)?$"), this));
}

//==============================================================================

void DoubleEditWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Check some key combinations

    if (   !(pEvent->modifiers() & Qt::ShiftModifier)
        && !(pEvent->modifiers() & Qt::ControlModifier)
        && !(pEvent->modifiers() & Qt::AltModifier)
        && !(pEvent->modifiers() & Qt::MetaModifier)) {
        // None of the modifiers is selected

        if (pEvent->key() == Qt::Key_Up) {
            // The user wants to go to the previous property

            emit goToPreviousPropertyRequested();

            // Accept the event

            pEvent->accept();
        } else if (pEvent->key() == Qt::Key_Down) {
            // The user wants to go to the next property

            emit goToNextPropertyRequested();

            // Accept the event

            pEvent->accept();
        } else {
            // Default handling of the event

            QLineEdit::keyPressEvent(pEvent);
        }
    } else {
        // Default handling of the event

        QLineEdit::keyPressEvent(pEvent);
    }
}

//==============================================================================

void DoubleEditWidget::setUnit(const QString &pUnit)
{
    // Update our unit, if needed

    if (!pUnit.compare(mUnit))
        return;

    mUnit = pUnit;

    // Let people know that the unit has changed

    emit unitChanged(pUnit);
}

//==============================================================================

DoubleEditFactory::DoubleEditFactory(QObject *pParent) :
    QtAbstractEditorFactory(pParent),
    mEditors(QMap<QtProperty *, QList<DoubleEditWidget *> >()),
    mProperties(QMap<DoubleEditWidget *, QtProperty *>())
{
}

//==============================================================================

void DoubleEditFactory::connectPropertyManager(DoublePropertyManager *pManager)
{
    // Keep track of changes to a property's unit

    connect(pManager, SIGNAL(unitChanged(QtProperty *, const QString &)),
            this, SLOT(unitChanged(QtProperty *, const QString &)));
}

//==============================================================================

void DoubleEditFactory::disconnectPropertyManager(DoublePropertyManager *pManager)
{
    // Stop tracking changes to a property's unit

    disconnect(pManager, SIGNAL(unitChanged(QtProperty *, const QString &)),
               this, SLOT(unitChanged(QtProperty *, const QString &)));
}

//==============================================================================

QWidget * DoubleEditFactory::createEditor(DoublePropertyManager *pManager,
                                          QtProperty *pProperty,
                                          QWidget *pParent)
{
    // Create an editor

    DoubleEditWidget *res = new DoubleEditWidget(pParent);

    // Set its unit

    res->setUnit(pManager->unit(pProperty));

    // Keep track of things

    mEditors[pProperty].append(res);
    mProperties[res] = pProperty;

    // Propagate a few signals

    connect(res, SIGNAL(goToPreviousPropertyRequested()),
            this, SIGNAL(goToPreviousPropertyRequested()));
    connect(res, SIGNAL(goToNextPropertyRequested()),
            this, SIGNAL(goToNextPropertyRequested()));

    // Keep track of when the editor has been destroyed

    connect(res, SIGNAL(destroyed(QObject *)),
            this, SLOT(editorDestroyed(QObject *)));

    // Return our editor

    return res;
}

//==============================================================================

void DoubleEditFactory::editorDestroyed(QObject *pEditor)
{
    // Remove any trace of the given editor

    QMap<DoubleEditWidget *, QtProperty *>::ConstIterator editorsIterator = mProperties.constBegin();

    while (editorsIterator != mProperties.constEnd()) {
        if (editorsIterator.key() == pEditor) {
            DoubleEditWidget *editor = editorsIterator.key();
            QtProperty *property = editorsIterator.value();

            mProperties.remove(editor);

            mEditors[property].removeAll(editor);

            if (mEditors[property].isEmpty())
                mEditors.remove(property);

            return;
        }

        ++editorsIterator;
    }
}

//==============================================================================

void DoubleEditFactory::unitChanged(QtProperty *pProperty, const QString &pUnit)
{
    // Make sure that at least one editor exists for our property

    if (!mEditors.contains(pProperty))
        return;

    // Make sure that our property has a manager

    DoublePropertyManager *manager = propertyManager(pProperty);

    if (!manager)
        return;

    // Update the unit of all our editors

    QList<DoubleEditWidget *> editors = mEditors[pProperty];
    QListIterator<DoubleEditWidget *> editorsIterator(editors);

    while (editorsIterator.hasNext()) {
        DoubleEditWidget *editor = editorsIterator.next();

        editor->setUnit(pUnit);
    }
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

    QTreeWidget *propertyWidget = treeWidget();

#ifdef Q_WS_MAC
    propertyWidget->setAttribute(Qt::WA_MacShowFocusRect, 0);
    // Note: the above removes the focus border since it messes up the look of
    //       our property editor...
#endif
    propertyWidget->setFrameShape(QFrame::NoFrame);

    if (pAutoResizeHeight)
        propertyWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Resize our height in case one of our header's sections gets resized

    connect(propertyWidget->header(), SIGNAL(sectionResized(int, int, int)),
            this, SLOT(resizeHeight()));

    // Make our tree widget our focus proxy

    setFocusProxy(propertyWidget);
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
        // We don't automatically resize our height, so our ideal size is our
        // maximum size...

        return maximumSize();
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

    // Resize our height

    resizeHeight();
}

//==============================================================================

void QtPropertyBrowserWidget::resizeHeight()
{
    // Resize our height, so that we don't have any blank space at the bottom,
    // but only if required

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
