//==============================================================================
// Property editor widget
//==============================================================================

#include "propertyeditorwidget.h"

//==============================================================================

#include <float.h>

//==============================================================================

#include <QHeaderView>
#include <QKeyEvent>
#include <QScrollBar>
#include <QSettings>
#include <QStandardItem>
#include <QVariant>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

DoubleEditorWidget::DoubleEditorWidget(QWidget *pParent) :
    QLineEdit(pParent)
{
#ifdef Q_WS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    // Note: the above removes the focus border since it messes up the look of
    //       our editor
#endif
    setFrame(QFrame::NoFrame);

    // Set a validator which accepts any double

    setValidator(new QRegExpValidator(QRegExp("^[+-]?[0-9]*\\.?[0-9]+([eE][+-]?[0-9]+)?$"), this));
}

//==============================================================================

void DoubleEditorWidget::keyPressEvent(QKeyEvent *pEvent)
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

ListEditorWidget::ListEditorWidget(QWidget *pParent) :
    QComboBox(pParent)
{
}

//==============================================================================

void ListEditorWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Default handling of the event

    QComboBox::keyPressEvent(pEvent);
}

//==============================================================================

QWidget * PropertyItemDelegate::createEditor(QWidget *pParent,
                                             const QStyleOptionViewItem &pOption,
                                             const QModelIndex &pIndex) const
{
    Q_UNUSED(pOption);
    Q_UNUSED(pIndex);

    // Create and return an editor for our item, based on its type

    QWidget *editor;
    PropertyItem *propertyItem = static_cast<PropertyItem *>(static_cast<const QStandardItemModel *>(pIndex.model())->itemFromIndex(pIndex));

    switch (propertyItem->type()) {
    case PropertyItem::Double:
        editor = new DoubleEditorWidget(pParent);

        break;
    case PropertyItem::List: {
        ListEditorWidget *listEditor = new ListEditorWidget(pParent);

        listEditor->addItems(propertyItem->list());

        editor = listEditor;

        break;
    }
    default:
        // PropertyItem::String

        return 0;
    }

    // Propagate a few signals

    connect(editor, SIGNAL(goToPreviousPropertyRequested()),
            this, SIGNAL(goToPreviousPropertyRequested()));
    connect(editor, SIGNAL(goToNextPropertyRequested()),
            this, SIGNAL(goToNextPropertyRequested()));

    // Let people know that there is a new editor

    emit openEditor(editor);

    // Return the editor

    return editor;
}

//==============================================================================

bool PropertyItemDelegate::eventFilter(QObject *pObject, QEvent *pEvent)
{
    // We want to handle key events ourselves, so...

    if(pEvent->type()==QEvent::KeyPress)
        return false;
    else
        return QStyledItemDelegate::eventFilter(pObject, pEvent);
}

//==============================================================================

PropertyItem::PropertyItem(const Type &pType) :
    QStandardItem(),
    mType(pType),
    mList(QStringList()),
    mEmptyListValue(QString("???"))
{
    // If the property item is of string type, then it should not be editable

    if (pType == String)
        setFlags(flags() & ~Qt::ItemIsEditable);
}

//==============================================================================

int PropertyItem::type() const
{
    // Return the property item's type

    return mType;
}

//==============================================================================

QStringList PropertyItem::list() const
{
    // Return the property item's list

    return mList;
}

//==============================================================================

void PropertyItem::setList(const QStringList &pList)
{
    // Set the value of our list, if appropriate

    if ((mType == List) && (pList != mList)) {
        // Keep track of the list

        mList = pList;

        // Use the first item of our list as the default value, assuming the
        // list is not empty

        if (pList.isEmpty())
            // The list is empty, so...

            setText(mEmptyListValue);
        else
            setText(pList.first());
    }
}

//==============================================================================

QString PropertyItem::emptyListValue() const
{
    // Return the property item's empty list value

    return mEmptyListValue;
}

//==============================================================================

void PropertyItem::setEmptyListValue(const QString &pEmptyListValue)
{
    // Set the value of our empty list value, if needed

    if ((mType == List) && pEmptyListValue.compare(mEmptyListValue))
        mEmptyListValue = pEmptyListValue;
}

//==============================================================================

Property::Property(PropertyItem *pName, PropertyItem *pValue,
                   PropertyItem *pUnit) :
    name(pName),
    value(pValue),
    unit(pUnit)
{
}

//==============================================================================

bool Property::operator!=(const Property &pOther) const
{
    // Return whether the two properties are different

    return (name != pOther.name) || (value != pOther.value) || (unit != pOther.unit);
}

//==============================================================================

bool Property::isEmpty() const
{
    // Return whether the property is empty

    return !name && !value && !unit;
}

//==============================================================================

void PropertyEditorWidget::constructor(const bool &pAutoUpdateHeight)
{
    // Some initialisations

    mAutoUpdateHeight = pAutoUpdateHeight;

    mProperty       = Property();
    mPropertyEditor = 0;

    // Customise ourselves

    setRootIsDecorated(false);

    // Create and set our data model

    mModel = new QStandardItemModel(this);

    setModel(mModel);

    // Update our height following data changes

    connect(mModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(updateHeight()));

    // Create our item delegate and set it, after making sure that we handle a
    // few of its signals

    mPropertyItemDelegate = new PropertyItemDelegate();

    connect(mPropertyItemDelegate, SIGNAL(openEditor(QWidget *)),
            this, SLOT(editorOpened(QWidget *)));
    connect(mPropertyItemDelegate, SIGNAL(closeEditor(QWidget *, QAbstractItemDelegate::EndEditHint)),
            this, SLOT(editorClosed()));

    connect(mPropertyItemDelegate, SIGNAL(goToPreviousPropertyRequested()),
            this, SLOT(goToPreviousProperty()));
    connect(mPropertyItemDelegate, SIGNAL(goToNextPropertyRequested()),
            this, SLOT(goToNextProperty()));

    setItemDelegate(mPropertyItemDelegate);

    // Resize our height in case one of our header's sections gets resized

    connect(header(), SIGNAL(sectionResized(int, int, int)),
            this, SLOT(updateHeight()));

    // Further customise ourselves

    setSelectionMode(QAbstractItemView::SingleSelection);

    header()->setMovable(false);

    // Initialise our height

    updateHeight();
}

//==============================================================================

PropertyEditorWidget::PropertyEditorWidget(const bool &pAutoUpdateHeight,
                                           QWidget *pParent) :
    TreeViewWidget(pParent)
{
    // Construct our object

    constructor(pAutoUpdateHeight);
}

//==============================================================================

PropertyEditorWidget::PropertyEditorWidget(QWidget *pParent) :
    TreeViewWidget(pParent)
{
    // Construct our object

    constructor();
}

//==============================================================================

void PropertyEditorWidget::retranslateUi()
{
    // Retranslate our header labels

    mModel->setHorizontalHeaderLabels(QStringList() << tr("Property")
                                                    << tr("Value")
                                                    << tr("Unit"));

    // 'Retranslate' the value of empty list properties

    for (int i = 0, iMax = mModel->rowCount(); i < iMax; ++i) {
        PropertyItem *propertyValue = property(i).value;

        if (   (propertyValue->type() == PropertyItem::List)
               && (propertyValue->list().isEmpty()))
            propertyValue->setText(propertyValue->emptyListValue());
    }
}

//==============================================================================

static const QString SettingsColumnWidth = "ColumnWidth%1";

//==============================================================================

void PropertyEditorWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the width of each column

    for (int i = 0, iMax = header()->count(); i < iMax; ++i)
        setColumnWidth(i, pSettings->value(SettingsColumnWidth.arg(i),
                                           columnWidth(i)).toInt());
}

//==============================================================================

void PropertyEditorWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the width of each column

    for (int i = 0, iMax = header()->count(); i < iMax; ++i)
        pSettings->setValue(SettingsColumnWidth.arg(i), columnWidth(i));
}

//==============================================================================

QSize PropertyEditorWidget::sizeHint() const
{
    // Return either our default/ideal size, depending on the case

    if (mAutoUpdateHeight) {
        // We automatically resize our height, so determine our ideal size which
        // is based on the width of our different columns, and the height of our
        // header and our different rows

        int hintWidth  = 0;
        int hintHeight = header()->height();

        for (int i = 0, iMax = header()->count(); i < iMax; ++i)
            hintWidth += columnWidth(i);

        for (int i = 0, iMax = mModel->rowCount(); i < iMax; ++i)
            hintHeight += rowHeight(mModel->index(i, 0));

        return QSize(hintWidth, hintHeight);
    } else {
        // We don't automatically resize our height, so our ideal size is our
        // maximum size...

        return maximumSize();
    }
}

//==============================================================================

void PropertyEditorWidget::selectFirstProperty()
{
    // Convenience function to select the first property, i.e. the first item

    selectFirstItem();
}

//==============================================================================

Property PropertyEditorWidget::addProperty(const PropertyItem::Type &pType)
{
    // Determine our new property's information

    Property res = Property(new PropertyItem(PropertyItem::String),
                            new PropertyItem(pType),
                            new PropertyItem(PropertyItem::String));

    // Populate our data model with our new property

    mModel->invisibleRootItem()->appendRow(QList<QStandardItem *>() << res.name
                                                                    << res.value
                                                                    << res.unit);

    // Return our new property's information

    return res;
}

//==============================================================================

Property PropertyEditorWidget::addDoubleProperty()
{
    // Add a double property and return its information

    return addProperty(PropertyItem::Double);
}

//==============================================================================

Property PropertyEditorWidget::addListProperty()
{
    // Add a list property and return its information

    return addProperty(PropertyItem::List);
}

//==============================================================================

void PropertyEditorWidget::setStringProperty(PropertyItem *pPropertyItem,
                                             const QString &pValue)
{
    // Set the value of the given string property, if valid

    if (pPropertyItem && (pPropertyItem->type() == PropertyItem::String))
        pPropertyItem->setText(pValue);
}

//==============================================================================

double PropertyEditorWidget::doubleProperty(PropertyItem *pPropertyItem) const
{
    // Return the value of the given double property, if valid

    if (pPropertyItem && (pPropertyItem->type() == PropertyItem::Double))
        return pPropertyItem->text().toDouble();
    else
        // The property item is either not valid or not of double type, so...

        return 0.0;
}

//==============================================================================

void PropertyEditorWidget::setDoubleProperty(PropertyItem *pPropertyItem,
                                             const double &pValue)
{
    // Set the value of the given double property, if valid

    if (pPropertyItem && (pPropertyItem->type() == PropertyItem::Double))
        pPropertyItem->setText(QString::number(pValue));
}

//==============================================================================

void PropertyEditorWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Check some key combinations

    if (   (pEvent->modifiers() & Qt::ControlModifier)
        && (pEvent->key() == Qt::Key_A)) {
        // The user wants to select everything which we don't want to allow,
        // so just accept the event...

        pEvent->accept();
    } else if (   (pEvent->key() == Qt::Key_Return)
               || (pEvent->key() == Qt::Key_Enter)) {
        // The user wants to start/stop editing the property

        if (mPropertyEditor)
            // We are currently editing a property, so stop editing it by
            // pretending to edit an empty property

            editProperty(Property());
        else
            // We are not currently editing a property, so start editing the
            // current one
            // Note: we could use mProperty, but if it was to be empty then we
            //       would have to use currentIndex().row(), so we might as well
            //       use the latter all the time...

            editProperty(property(currentIndex().row()));

        // Accept the event

        pEvent->accept();
    } else if (pEvent->key() == Qt::Key_Escape) {
        // The user wants to cancel the editing of the property

        cancelPropertyEditing();

        // Accept the event

        pEvent->accept();
    } else if (   !(pEvent->modifiers() & Qt::ShiftModifier)
               && !(pEvent->modifiers() & Qt::ControlModifier)
               && !(pEvent->modifiers() & Qt::AltModifier)
               && !(pEvent->modifiers() & Qt::MetaModifier)) {
        // None of the modifiers is selected

        if (pEvent->key() == Qt::Key_Up) {
            // The user wants to go the previous property

            goToPreviousProperty();

            // Accept the event

            pEvent->accept();
        } else if (pEvent->key() == Qt::Key_Down) {
            // The user wants to go to the next property

            goToNextProperty();

            // Accept the event

            pEvent->accept();
        } else {
            // Default handling of the event

            TreeViewWidget::keyPressEvent(pEvent);
        }
    } else {
        // Default handling of the event

        TreeViewWidget::keyPressEvent(pEvent);
    }
}

//==============================================================================

void PropertyEditorWidget::mousePressEvent(QMouseEvent *pEvent)
{
    // Start/stop the editing of the property

    Property mouseProperty = property(indexAt(pEvent->pos()).row());

    if (mPropertyEditor) {
        // We are already editing a property, so either stop its editing or
        // start editing anoher property

        if (mouseProperty != mProperty)
            // We want to edit another property

            editProperty(mouseProperty);
        else
            // We want to stop editing the property, so pretend we want to edit
            // an empty property

            editProperty(Property());
    } else {
        // We are not currently editing a property, so start editing the current
        // one

        editProperty(mouseProperty);
    }

    // Accept the event

    pEvent->accept();
}

//==============================================================================

void PropertyEditorWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    // Edit the property, but only if we want to edit a new one

    Property mouseProperty = property(indexAt(pEvent->pos()).row());

    if (!mouseProperty.isEmpty() && (mouseProperty != mProperty))
        editProperty(mouseProperty);

    // Accept the event

    pEvent->accept();
}

//==============================================================================

void PropertyEditorWidget::resizeEvent(QResizeEvent *pEvent)
{
    // Default handling of the event

    TreeViewWidget::resizeEvent(pEvent);

    // Update our height

    updateHeight();
}

//==============================================================================

void PropertyEditorWidget::updateHeight()
{
    // Update our height, so that we don't have any blank space at the bottom,
    // but only if required

    if (mAutoUpdateHeight) {
        QSize idealSize = sizeHint();

        setFixedHeight( idealSize.height()
                       +((width() < idealSize.width())?
#if defined Q_WS_WIN || defined Q_WS_MAC
                            horizontalScrollBar()->height():
#else
                            horizontalScrollBar()->height()+3:
                            // Note: why on earth does Linux require three more
                            //       pixels?!... Indeed, if don't add them, then
                            //       there won't be enough space to show the
                            //       last property (upon selecting it) and the
                            //       widget will increase its height, resulting
                            //       in some blank space at the bottom and a
                            //       vertical bar being shown! We could prevent
                            //       the vertical bar from ever being shown, but
                            //       not sure what could be done about the blank
                            //       space, hence we 'manually' add those three
                            //       pixels...
#endif
                            0));
        // Note: the new height consists of our ideal height to which we add the
        //       height of our horizontal scroll bar, should it be shown (i.e.
        //       if our width is smaller than that of our ideal size)...
    }
}

//==============================================================================

void PropertyEditorWidget::editorOpened(QWidget *pEditor)
{
    // Keep track of some information about the property

    mPropertyEditor = pEditor;
    mProperty       = currentProperty();

    // We are starting the editing of a property, so make sure that if we are to
    // edit a list item, then its original value gets properly set
    // Note: indeed, by default the first list item will be selected...

    if (mProperty.value->type() == PropertyItem::List) {
        ListEditorWidget *propertyEditor = static_cast<ListEditorWidget *>(mPropertyEditor);

        for (int i = 0, iMax = mProperty.value->list().count(); i < iMax; ++i)
            if (!mProperty.value->text().compare(mProperty.value->list()[i])) {
                propertyEditor->setCurrentIndex(i);

                break;
            }
    }

    // Next, we need to use the property's editor as our focus proxy and make
    // sure that it immediately gets the focus
    // Note: if we were not to immediately give the editor the focus, then the
    //       central widget would give the focus to the previously focused
    //       widget (see CentralWidget::updateGui()), so...

    setFocusProxy(pEditor);

    pEditor->setFocus();
}

//==============================================================================

void PropertyEditorWidget::editorClosed()
{
    // We have stopped editing a property, so make sure that if we were editing
    // a list item, then its value gets properly set
    // Note: indeed, by default the value will be the index of the selected item
    //       in the list while we want the actual text corresponding to the
    //       selected item...

    if (mProperty.value->type() == PropertyItem::List)
        mProperty.value->setText(static_cast<ListEditorWidget *>(mPropertyEditor)->currentText());

    // Next, we need to reset our focus proxy and make sure that we get the
    // focus (see editorOpened() above for the reason)

    setFocusProxy(0);

    setFocus();

    // Reset some information about the property

    mPropertyEditor = 0;
    mProperty       = Property();
}

//==============================================================================

void PropertyEditorWidget::editProperty(const Property &pProperty,
                                        const bool &pCommitData)
{
    // We want to edit a new property, so first stop the editing of the current
    // one, if any

    if (mPropertyEditor) {
        // A property is currently being edited, so commit its data and then
        // close its corresponding editor

        if (pCommitData)
            commitData(mPropertyEditor);

        closeEditor(mPropertyEditor,
                    pCommitData?
                        QAbstractItemDelegate::SubmitModelCache:
                        QAbstractItemDelegate::RevertModelCache);

        // Update our state
        // Note: this is very important otherwise our state will remain that of
        //       editing...

        setState(NoState);

        // Call editorClosed() to reset a few things

        editorClosed();
    }

    // Now that the editing of our old property has finished, we can start the
    // editing of our new property, if any

    if (!pProperty.isEmpty()) {
        // There is a new property to edit, so first make sure that it is
        // selected

        QModelIndex propertyIndex = pProperty.value->index();

        setCurrentIndex(propertyIndex);

        // Now, we can 'properly' edit the property's value

        edit(propertyIndex);
    }
}

//==============================================================================

void PropertyEditorWidget::cancelPropertyEditing()
{
    // The user wants to cancel the editing of the property

    editProperty(Property(), false);
}

//==============================================================================

void PropertyEditorWidget::removeAllProperties()
{
    // Remove all the properties we currently hold

    mModel->removeRows(0, mModel->rowCount());

    // By default, we don't want root to be decorated

    setRootIsDecorated(false);
}

//==============================================================================

void PropertyEditorWidget::setPropertyVisible(const Property &pProperty,
                                              const bool &pVisible)
{
    // Show/hide the property, if not empty

    if (pProperty.isEmpty())
        return;

    setRowHidden(pProperty.name->row(),
                 pProperty.name->parent()?
                     pProperty.name->parent()->index():
                     mModel->invisibleRootItem()->index(),
                 !pVisible);

    // Make sure that our height is correct

    updateHeight();
}

//==============================================================================

void PropertyEditorWidget::goToNeighbouringProperty(const int &pShift)
{
    // Go to the previous/next property

    int row = currentIndex().row()+pShift;

    if ((row >= 0) && (row < model()->rowCount()))
        // The previous/next property exists, so we can edit it

        editProperty(property(row));
}

//==============================================================================

void PropertyEditorWidget::goToPreviousProperty()
{
    // Go to the previous property

    goToNeighbouringProperty(-1);
}

//==============================================================================

void PropertyEditorWidget::goToNextProperty()
{
    // Go to the next property

    goToNeighbouringProperty(1);
}

//==============================================================================

Property PropertyEditorWidget::property(const int &pRow) const
{
    // Return some information about the property at the given row

    return Property(static_cast<PropertyItem *>(mModel->itemFromIndex(mModel->index(pRow, 0))),
                    static_cast<PropertyItem *>(mModel->itemFromIndex(mModel->index(pRow, 1))),
                    static_cast<PropertyItem *>(mModel->itemFromIndex(mModel->index(pRow, 2))));
}

//==============================================================================

Property PropertyEditorWidget::currentProperty() const
{
    // Return some information about the current property

    return property(currentIndex().row());
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
