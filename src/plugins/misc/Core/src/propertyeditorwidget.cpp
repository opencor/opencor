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

PropertyItem::PropertyItem(const Type &pType, const QStringList &pList) :
    QStandardItem(),
    mType(pType),
    mList(pList),
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

QString PropertyItem::emptyListValue() const
{
    // Return the property item's empty list value

    return mEmptyListValue;
}

//==============================================================================

void PropertyItem::setEmptyListValue(const QString &pEmptyListValue)
{
    // Set the value of our empty list value, if needed

    if (pEmptyListValue.compare(mEmptyListValue))
        mEmptyListValue = pEmptyListValue;
}

//==============================================================================

void PropertyEditorWidget::constructor(const bool &pAutoUpdateHeight)
{
    // Some initialisations

    mAutoUpdateHeight = pAutoUpdateHeight;

    mPropertyEditor =  0;
    mPropertyIndex  = -1;

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
        PropertyItem *propertyItem = propertyValue(i);

        if (   (propertyItem->type() == PropertyItem::List)
               && (propertyItem->list().isEmpty()))
            propertyItem->setText(propertyItem->emptyListValue());
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

int PropertyEditorWidget::addProperty(const PropertyItem::Type &pType,
                                      const QStringList &pList)
{
    // Determine the index of our new property

    int res = mModel->rowCount();

    // Populate our data model with our new property

    mModel->invisibleRootItem()->setChild(res, 0, new PropertyItem(PropertyItem::String));
    mModel->invisibleRootItem()->setChild(res, 1, new PropertyItem(pType, pList));
    mModel->invisibleRootItem()->setChild(res, 2, new PropertyItem(PropertyItem::String));

    // Return the index of our new property

    return res;
}

//==============================================================================

PropertyItem * PropertyEditorWidget::propertyValue(const int &pIndex) const
{
    // Return a pointer to our property value

    return static_cast<PropertyItem *>(mModel->invisibleRootItem()->child(pIndex, 1));
}

//==============================================================================

int PropertyEditorWidget::addDoubleProperty()
{
    // Add a double property and return its index

    return addProperty(PropertyItem::Double);
}

//==============================================================================

int PropertyEditorWidget::addListProperty(const QStringList &pList)
{
    // Add a list property and retrieve its index

    int res = addProperty(PropertyItem::List, pList);

    // Use the first item of our list as the default value, assuming the list is
    // not empty

    if (pList.isEmpty())
        // The list is empty, so...

        mModel->invisibleRootItem()->child(res, 1)->setText(static_cast<PropertyItem *>(mModel->invisibleRootItem()->child(res, 1))->emptyListValue());
    else
        mModel->invisibleRootItem()->child(res, 1)->setText(pList.first());

    // Return the index of our list property

    return res;
}

//==============================================================================

void PropertyEditorWidget::setPropertyName(const int &pIndex,
                                           const QString &pName)
{
    // Set the name of the given property, if valid

    if ((pIndex >= 0) && (pIndex < mModel->rowCount()))
        mModel->invisibleRootItem()->child(pIndex, 0)->setText(pName);
}

//==============================================================================

double PropertyEditorWidget::doublePropertyValue(const int &pIndex) const
{
    // Return the value of the given double property, if valid

    if ((pIndex >= 0) && (pIndex < mModel->rowCount())) {
        PropertyItem *doublePropertyItem = static_cast<PropertyItem *>(mModel->invisibleRootItem()->child(pIndex, 1));

        if (doublePropertyItem->type() == PropertyItem::Double)
            return doublePropertyItem->text().toDouble();
        else
            // The property item is not of double type, so...

            return 0.0;
    } else {
        // The property index is no valid, so...

        return 0.0;
    }
}

//==============================================================================

void PropertyEditorWidget::setDoublePropertyValue(const int &pIndex,
                                                  const double &pValue)
{
    // Set the value of the given double property, if valid

    if ((pIndex >= 0) && (pIndex < mModel->rowCount())) {
        PropertyItem *doublePropertyItem = static_cast<PropertyItem *>(mModel->invisibleRootItem()->child(pIndex, 1));

        if (doublePropertyItem->type() == PropertyItem::Double)
            doublePropertyItem->setText(QString::number(pValue));
    }
}

//==============================================================================

void PropertyEditorWidget::setPropertyUnit(const int &pIndex,
                                           const QString &pUnit)
{
    // Set the name of the given property, if valid

    if ((pIndex >= 0) && (pIndex < mModel->rowCount()))
        mModel->invisibleRootItem()->child(pIndex, 2)->setText(pUnit);
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
            // We are currently editing a property, so stop editing it

            editProperty(-1);
        else
            // We are not currently editing a property, so start editing it
            // Note: we could use mPropertyIndex, but if it was to be equal to
            //       -1 we would have to use currentIndex().row(), so we might
            //       as well use the latter all the time...

            editProperty(currentIndex().row());

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

    int propertyIndex = indexAt(pEvent->pos()).row();

    if (mPropertyEditor) {
        // We are already editing a property, so either stop its editing or
        // start editing anoher property

        if (propertyIndex != mPropertyIndex)
            // We want to edit another property

            editProperty(propertyIndex);
        else
            // We want to stop editing the property

            editProperty(-1);
    } else {
        // We are not currently editing a property, so start editing the current
        // one

        editProperty(propertyIndex);
    }

    // Accept the event

    pEvent->accept();
}

//==============================================================================

void PropertyEditorWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    // Edit the property, but only if we want to edit a new one

    int propertyIndex = indexAt(pEvent->pos()).row();

    if ((propertyIndex != -1) && (propertyIndex != mPropertyIndex))
        editProperty(propertyIndex);

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
    mPropertyIndex  = currentIndex().row();

    // We are starting the editing of a property, so make sure that if we are to
    // edit a list item, then its original value gets properly set
    // Note: indeed, by default the first list item will be selected...

    PropertyItem *propertyItem = propertyValue(mPropertyIndex);

    if (propertyItem->type() == PropertyItem::List) {
        ListEditorWidget *propertyEditor = static_cast<ListEditorWidget *>(mPropertyEditor);

        for (int i = 0, iMax = propertyItem->list().count(); i < iMax; ++i)
            if (!propertyItem->text().compare(propertyItem->list().at(i))) {
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

    PropertyItem *propertyItem = propertyValue(mPropertyIndex);

    if (propertyItem->type() == PropertyItem::List)
        propertyItem->setText(static_cast<ListEditorWidget *>(mPropertyEditor)->currentText());

    // Next, we need to reset our focus proxy and make sure that we get the
    // focus (see editorOpened() above for the reason)

    setFocusProxy(0);

    setFocus();

    // Reset some information about the property

    mPropertyEditor =  0;
    mPropertyIndex  = -1;
}

//==============================================================================

void PropertyEditorWidget::editProperty(const int &pIndex,
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

    if (pIndex != -1) {
        // There is a new property to edit, so first make sure that it is
        // selected

        selectItem(pIndex);

        // Now, we can 'properly' edit the property's value
        // Note: the property's value's column is always 1...

        edit(model()->index(pIndex, 1));
    }
}

//==============================================================================

void PropertyEditorWidget::cancelPropertyEditing()
{
    // The user wants to cancel the editing of the property

    editProperty(-1, false);
}

//==============================================================================

void PropertyEditorWidget::removeAllProperties()
{
    // Keep track of our columns' width
    // Note: indeed, everything gets reset after we call mModel->clear() below,
    //       so...

    QList<int> columnWidths = QList<int>();

    for (int i = 0, iMax = header()->count(); i < iMax; ++i)
        columnWidths << columnWidth(i);

    // Remove all the properties we currently hold

    mModel->clear();

    // Get the headers reset

    retranslateUi();

    // Set our 'new' columns' width

    for (int i = 0, iMax = header()->count(); i < iMax; ++i)
        setColumnWidth(i, columnWidths[i]);
}

//==============================================================================

void PropertyEditorWidget::goToNeighbouringProperty(const int &pShift)
{
    // Go to the previous/next property

    int row = currentIndex().row()+pShift;

    if ((row >= 0) && (row < model()->rowCount()))
        // The previous/next property exists, so we can edit it

        editProperty(row);
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

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
