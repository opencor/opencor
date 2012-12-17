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

NumberEditorWidget::NumberEditorWidget(QWidget *pParent) :
    QLineEdit(pParent)
{
#ifdef Q_WS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    // Note: the above removes the focus border since it messes up the look of
    //       our editor
#endif
    setFrame(QFrame::NoFrame);
}

//==============================================================================

void NumberEditorWidget::keyPressEvent(QKeyEvent *pEvent)
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

IntegerEditorWidget::IntegerEditorWidget(QWidget *pParent) :
    NumberEditorWidget(pParent)
{
    // Set a validator which accepts any integer

    setValidator(new QRegExpValidator(QRegExp("^[+-]?[0-9]*([eE][+-]?[0-9]+)?$"), this));
}

//==============================================================================

DoubleEditorWidget::DoubleEditorWidget(QWidget *pParent) :
    NumberEditorWidget(pParent)
{
    // Set a validator which accepts any double

    setValidator(new QRegExpValidator(QRegExp("^[+-]?[0-9]*\\.?[0-9]+([eE][+-]?[0-9]+)?$"), this));
}

//==============================================================================

ListEditorWidget::ListEditorWidget(QWidget *pParent) :
    QComboBox(pParent)
{
}

//==============================================================================

void ListEditorWidget::keyPressEvent(QKeyEvent *pEvent)
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

            QComboBox::keyPressEvent(pEvent);
        }
    } else {
        // Default handling of the event

        QComboBox::keyPressEvent(pEvent);
    }
}

//==============================================================================

void ListEditorWidget::mousePressEvent(QMouseEvent *pEvent)
{
    // We don't want to call the default handler of the event (since this would
    // the list of items), but we don't want to ignore the event either (since
    // our parent might then handle it and mess things up for us), so just do
    // nothing and accept the event

    pEvent->accept();
}

//==============================================================================

void ListEditorWidget::mouseDoubleClickEvent(QMouseEvent *pEvent)
{
    // We want to go to the next item in the list (and go back to the first one
    // if we are at the end of the list), so determine the new current index

    int newCurrentIndex = currentIndex()+1;

    if (newCurrentIndex == count())
        newCurrentIndex = 0;

    // Set the new current index

    setCurrentIndex(newCurrentIndex);

    // Accept the event

    pEvent->accept();
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
    case PropertyItem::Integer:
        editor = new IntegerEditorWidget(pParent);

        break;
    case PropertyItem::Double:
        editor = new DoubleEditorWidget(pParent);

        break;
    case PropertyItem::List: {
        ListEditorWidget *listEditor = new ListEditorWidget(pParent);

        listEditor->addItems(propertyItem->list());

        editor = listEditor;

        // Propagate the signal telling us about the list property value having
        // changed

        connect(listEditor, SIGNAL(currentIndexChanged(const QString &)),
                this, SIGNAL(listPropertyChanged(const QString &)));

        break;
    }
    default:
        // PropertyItem::Category

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

    if(pEvent->type() == QEvent::KeyPress)
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

Property::Property() :
    name(0),
    value(0),
    unit(0)
{
}

//==============================================================================

Property::Property(const PropertyItem::Type &pType) :
    name(nonEditableItem()),
    value(new PropertyItem(pType)),
    unit(nonEditableItem())
{
}

//==============================================================================

Property::Property(QStandardItem *pName, PropertyItem *pValue,
                   QStandardItem *pUnit) :
    name(pName),
    value(pValue),
    unit(pUnit)
{
}

//==============================================================================

QStandardItem * Property::nonEditableItem() const
{
    // Create and return a non-editable item

    QStandardItem *res = new QStandardItem();

    res->setFlags(res->flags() & ~Qt::ItemIsEditable);

    return res;
}

//==============================================================================

bool Property::operator!=(const Property &pOther) const
{
    // Return whether the two properties are different

    return (name != pOther.name) || (value != pOther.value) || (unit != pOther.unit);
}

//==============================================================================

bool Property::operator==(const Property &pOther) const
{
    // Return whether the two properties are the same

    return (name == pOther.name) && (value == pOther.value) && (unit == pOther.unit);
}

//==============================================================================

bool Property::isEmpty() const
{
    // Return whether the property is empty

    return !name && !value && !unit;
}

//==============================================================================

QList<QStandardItem *> Property::items() const
{
    // Return our items as a list

    return QList<QStandardItem *>() << name << value << unit;
}

//==============================================================================

PropertyEditorWidgetGuiState::PropertyEditorWidgetGuiState() :
    currentIndex(QModelIndex())
{
}

//==============================================================================

void PropertyEditorWidget::constructor(const bool &pShowUnits,
                                       const bool &pAutoUpdateHeight)
{
    // Some initialisations

    mShowUnits = pShowUnits;
    mAutoUpdateHeight = pAutoUpdateHeight;

    mProperty       = Property();
    mPropertyEditor = 0;

    // Customise ourselves

    setRootIsDecorated(false);

    // Create and set our data model

    mModel = new QStandardItemModel(this);

    setModel(mModel);

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

    connect(mPropertyItemDelegate, SIGNAL(listPropertyChanged(const QString &)),
            this, SIGNAL(listPropertyChanged(const QString &)));

    setItemDelegate(mPropertyItemDelegate);

    // Resize our height in case data have been changed or one the properties
    // gets expanded/collapsed

    connect(mModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(updateHeight()));

    connect(this, SIGNAL(collapsed(const QModelIndex &)),
            this, SLOT(updateHeight()));
    connect(this, SIGNAL(expanded(const QModelIndex &)),
            this, SLOT(updateHeight()));

    // Further customise ourselves

    setSelectionMode(QAbstractItemView::SingleSelection);

    header()->setMovable(false);

    // Initialise our height

    updateHeight();
}

//==============================================================================

PropertyEditorWidget::PropertyEditorWidget(const bool &pShowUnits,
                                           const bool &pAutoUpdateHeight,
                                           QWidget *pParent) :
    TreeViewWidget(pParent)
{
    // Construct our object

    constructor(pShowUnits, pAutoUpdateHeight);
}

//==============================================================================

PropertyEditorWidget::PropertyEditorWidget(const bool &pAutoUpdateHeight,
                                           QWidget *pParent) :
    TreeViewWidget(pParent)
{
    // Construct our object

    constructor(true, pAutoUpdateHeight);
}

//==============================================================================

PropertyEditorWidget::PropertyEditorWidget(QWidget *pParent) :
    TreeViewWidget(pParent)
{
    // Construct our object

    constructor();
}

//==============================================================================

void PropertyEditorWidget::retranslateEmptyListProperties(QStandardItem *pItem)
{
    // Retranslate the current item, should it be an empty list

    QModelIndex index = pItem->index();

    if (index.isValid()) {
        // The index is valid (i.e. it's not our invisible root item), so
        // retrieve the corresponding property item

        PropertyItem *propertyValue = property(index).value;

        // Check whether the property value is of list type and whether its list
        // is empty and, if so, then set its text value accordingly

        if (   (propertyValue->type() == PropertyItem::List)
               && (propertyValue->list().isEmpty()))
            propertyValue->setText(propertyValue->emptyListValue());
    }

    // Retranslate the current item's children, if any

    for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i)
        retranslateEmptyListProperties(pItem->child(i));
}

//==============================================================================

void PropertyEditorWidget::retranslateUi()
{
    // Retranslate our header labels

    if (mShowUnits)
        mModel->setHorizontalHeaderLabels(QStringList() << tr("Property")
                                                        << tr("Value")
                                                        << tr("Unit"));
    else
        mModel->setHorizontalHeaderLabels(QStringList() << tr("Property")
                                                        << tr("Value"));

    // 'Retranslate' the value of all empty list properties

    retranslateEmptyListProperties(mModel->invisibleRootItem());
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

int PropertyEditorWidget::childrenRowHeight(const QStandardItem *pItem) const
{
    // Return the total height of the given index's children

    int res = 0;

    if (pItem->hasChildren())
        for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i) {
            QStandardItem *childItem = pItem->child(i, 0);
            int childIndexHeight     = rowHeight(childItem->index());

            if (childIndexHeight)
                res += childIndexHeight+childrenRowHeight(childItem);
        }

    return res;
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

        for (int i = 0, iMax = mModel->rowCount(); i < iMax; ++i) {
            QStandardItem *rowItem = mModel->item(i, 0);
            int rowItemHeight      = rowHeight(rowItem->index());

            if (rowItemHeight)
                // Our current row has some height, meaning that it is visible,
                // so we can its height to ou hintHeight, as well as retrieve
                // the total height of our row's children

                hintHeight += rowItemHeight+childrenRowHeight(rowItem);
        }

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

PropertyEditorWidgetGuiState PropertyEditorWidget::guiState() const
{
    // Retrieve and return our GUI state

    PropertyEditorWidgetGuiState guiState = PropertyEditorWidgetGuiState();

    guiState.currentIndex = currentIndex();

    return guiState;
}

//==============================================================================

void PropertyEditorWidget::setGuiState(const PropertyEditorWidgetGuiState &pGuiState)
{
    // Set our GUI state

    if (pGuiState.currentIndex.isValid())
        setCurrentIndex(pGuiState.currentIndex);
}

//==============================================================================

Property PropertyEditorWidget::addProperty(const Property &pParent,
                                           const PropertyItem::Type &pType)
{
    // Determine our new property's information

    Property res = Property(pType);

    // Populate our data model with our new property

    if (pParent.isEmpty()) {
        // We want to add a root property

        mModel->invisibleRootItem()->appendRow(res.items());
    } else {
        // We want to add a child property

        pParent.name->appendRow(res.items());

        // If we want to see the child property, we need root decoration

        setRootIsDecorated(true);
    }

    // Return our new property's information

    return res;
}

//==============================================================================

Property PropertyEditorWidget::addCategoryProperty(const Property &pParent)
{
    // Add a category property and return its information

    return addProperty(pParent, PropertyItem::Category);
}

//==============================================================================

Property PropertyEditorWidget::addIntegerProperty(const Property &pParent)
{
    // Add an integer property and return its information

    return addProperty(pParent, PropertyItem::Integer);
}

//==============================================================================

Property PropertyEditorWidget::addDoubleProperty(const Property &pParent)
{
    // Add a double property and return its information

    return addProperty(pParent, PropertyItem::Double);
}

//==============================================================================

Property PropertyEditorWidget::addListProperty(const Property &pParent)
{
    // Add a list property and return its information

    return addProperty(pParent, PropertyItem::List);
}

//==============================================================================

void PropertyEditorWidget::setNonEditablePropertyItem(QStandardItem *pPropertyItem,
                                                      const QString &pValue)
{
    // Set the value of the given non-editable property item, if it exists

    if (pPropertyItem)
        pPropertyItem->setText(pValue);
}

//==============================================================================

int PropertyEditorWidget::integerPropertyItem(PropertyItem *pPropertyItem) const
{
    // Return the value of the given double property item, if it exists and is
    // valid

    if (pPropertyItem && (pPropertyItem->type() == PropertyItem::Integer))
        return pPropertyItem->text().toInt();
    else
        // The property item is either not valid or not of double type, so...

        return 0.0;
}

//==============================================================================

void PropertyEditorWidget::setIntegerPropertyItem(PropertyItem *pPropertyItem,
                                                  const int &pValue)
{
    // Set the value of the given double property item, if it exists and is
    // valid

    if (pPropertyItem && (pPropertyItem->type() == PropertyItem::Integer))
        pPropertyItem->setText(QString::number(pValue));
}

//==============================================================================

double PropertyEditorWidget::doublePropertyItem(PropertyItem *pPropertyItem) const
{
    // Return the value of the given double property item, if it exists and is
    // valid

    if (pPropertyItem && (pPropertyItem->type() == PropertyItem::Double))
        return pPropertyItem->text().toDouble();
    else
        // The property item is either not valid or not of double type, so...

        return 0.0;
}

//==============================================================================

void PropertyEditorWidget::setDoublePropertyItem(PropertyItem *pPropertyItem,
                                                 const double &pValue)
{
    // Set the value of the given double property item, if it exists and is
    // valid

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

            editProperty(property(currentIndex()));

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

    Property mouseProperty = property(indexAt(pEvent->pos()));

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

    Property mouseProperty = property(indexAt(pEvent->pos()));

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

    mProperty       = currentProperty();
    mPropertyEditor = pEditor;

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

    if (mProperty.value->type() == PropertyItem::List) {
        if (mProperty.value->list().isEmpty())
            mProperty.value->setText(mProperty.value->emptyListValue());
        else
            mProperty.value->setText(static_cast<ListEditorWidget *>(mPropertyEditor)->currentText());
    }

    // Reset our focus proxy and make sure that we get the focus (see
    // editorOpened() above for the reason)

    setFocusProxy(0);

    setFocus();

    // Reset some information about the property

    mProperty       = Property();
    mPropertyEditor = 0;
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

    // By default, we don't want root decoration

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
    // Determine the index of the current index's neighbour

    QModelIndex neighbouringIndex = QModelIndex();

    if (pShift == 1)
        neighbouringIndex = indexBelow(currentIndex());
    else if (pShift == -1)
        neighbouringIndex = indexAbove(currentIndex());

    // Edit the neighbouring property, if any

    if (neighbouringIndex.isValid())
        editProperty(property(neighbouringIndex));
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

Property PropertyEditorWidget::property(const QModelIndex &pIndex) const
{
    // Make sure that the given index is valid

    if (!pIndex.isValid())
        return Property();

    // Return some information about the property at the given index

    QModelIndex parent = pIndex.parent();
    int row = pIndex.row();

    return Property(mModel->itemFromIndex(mModel->index(row, 0, parent)),
                    static_cast<PropertyItem *>(mModel->itemFromIndex(mModel->index(row, 1, parent))),
                    mModel->itemFromIndex(mModel->index(row, 2, parent)));
}

//==============================================================================

Property PropertyEditorWidget::currentProperty() const
{
    // Return some information about the current property

    return property(currentIndex());
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
