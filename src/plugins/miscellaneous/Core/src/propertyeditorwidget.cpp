/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Property editor widget
//==============================================================================

#include "corecliutils.h"
#include "propertyeditorwidget.h"

//==============================================================================

#include <float.h>

//==============================================================================

#include <QAbstractItemDelegate>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QEvent>
#include <QKeyEvent>
#include <QModelIndex>
#include <QRegularExpressionValidator>
#include <QScrollBar>
#include <QSettings>
#include <QStandardItem>
#include <QStyle>
#include <QStyleOptionViewItem>
#include <QVariant>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

TextEditorWidget::TextEditorWidget(QWidget *pParent) :
    QLineEdit(pParent)
{
#ifdef Q_OS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, false);
#endif
    setFrame(QFrame::NoFrame);
}

//==============================================================================

void TextEditorWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Check some key combinations

    bool noModifiers =    !(pEvent->modifiers() & Qt::ShiftModifier)
                       && !(pEvent->modifiers() & Qt::ControlModifier)
                       && !(pEvent->modifiers() & Qt::AltModifier)
                       && !(pEvent->modifiers() & Qt::MetaModifier);

    if (noModifiers && (pEvent->key() == Qt::Key_Up)) {
        // The user wants to go to the previous property

        emit goToPreviousPropertyRequested();

        // Accept the event

        pEvent->accept();
    } else if (noModifiers && (pEvent->key() == Qt::Key_Down)) {
        // The user wants to go to the next property

        emit goToNextPropertyRequested();

        // Accept the event

        pEvent->accept();
    } else {
        // Default handling of the event

        QLineEdit::keyPressEvent(pEvent);
    }
}

//==============================================================================

IntegerEditorWidget::IntegerEditorWidget(QWidget *pParent) :
    TextEditorWidget(pParent)
{
    // Set a validator that accepts any integer

    static const QRegularExpression IntegerRegEx = QRegularExpression("^[+-]?\\d*$");

    setValidator(new QRegularExpressionValidator(IntegerRegEx, this));
}

//==============================================================================

DoubleEditorWidget::DoubleEditorWidget(QWidget *pParent) :
    TextEditorWidget(pParent)
{
    // Set a validator that accepts any double

    static const QRegularExpression DoubleRegEx = QRegularExpression("^[+-]?(\\d+(\\.\\d*)?|\\.\\d+)([eE][+-]?\\d+)?$");

    setValidator(new QRegularExpressionValidator(DoubleRegEx, this));
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

    bool noModifiers =    !(pEvent->modifiers() & Qt::ShiftModifier)
                       && !(pEvent->modifiers() & Qt::ControlModifier)
                       && !(pEvent->modifiers() & Qt::AltModifier)
                       && !(pEvent->modifiers() & Qt::MetaModifier);

    if (noModifiers && (pEvent->key() == Qt::Key_Up)) {
        // The user wants to go to the previous property

        emit goToPreviousPropertyRequested();

        // Accept the event

        pEvent->accept();
    } else if (noModifiers && (pEvent->key() == Qt::Key_Down)) {
        // The user wants to go to the next property

        emit goToNextPropertyRequested();

        // Accept the event

        pEvent->accept();
    } else {
        // Default handling of the event

        QComboBox::keyPressEvent(pEvent);
    }
}

//==============================================================================

void ListEditorWidget::mouseDoubleClickEvent(QMouseEvent *pEvent)
{
    // Make sure that we have at least one item

    if (!count())
        return;

    // We want to go to the next item in the list (and go back to the first one
    // if we are at the end of the list), so determine the new current index

    int newCurrentIndex = currentIndex();

    do {
        ++newCurrentIndex;

        if (newCurrentIndex == count())
            newCurrentIndex = 0;
    } while (itemText(newCurrentIndex).isEmpty());

    // Set the new current index

    setCurrentIndex(newCurrentIndex);

    // Accept the event

    pEvent->accept();
}

//==============================================================================

void ListEditorWidget::mousePressEvent(QMouseEvent *pEvent)
{
    // Check whether the user clicked on the arrow and, if so, allow the default
    // handling of the event (so that the list of items gets displayed) while do
    // nothing if the user clicked somewhere else (this to so that if the user
    // double clicks on the widget, then we can select the next item)
    // Note: we would normally call style()->hitTestComplexControl() and, if it
    //       returns QStyle::SC_ComboBoxArrow, then allow the default handling
    //       of the event, but if this works fine on Windows and Linux, it just
    //       doesn't work on macOS. Indeed, no matter where we are over the
    //       widget, style()->hitTestComplexControl() will always (and as
    //       expected; [QtSources]/qtbase/src/widgets/styles/qmacstyle_mac.mm)
    //       return QStyle::SC_ComboBoxArrow. So, to get the behaviour we are
    //       after, we do what is done in
    //       [QtSources]/qtbase/src/widgets/styles/qcommonstyle.cpp...

    // Retrieve our style option

    QStyleOptionComboBox styleOption;

    initStyleOption(&styleOption);

    // Go through the different sub controls (starting with the arrow) and
    // determine over which one we are

    uint subControl = QStyle::SC_ComboBoxArrow;
    QRect subControlRect;

    while (subControl) {
        // Retrieve the sub control's region

        subControlRect = style()->subControlRect(QStyle::CC_ComboBox,
                                                 &styleOption,
                                                 QStyle::SubControl(subControl),
                                                 this);

        // Check whether the sub control exists (i.e. its region is valid) and,
        // if so, whether we ore over it

        if (subControlRect.isValid() && subControlRect.contains(pEvent->pos()))
            break;

        // Either the sub control doesn't exist or we are not over it, so try
        // the next sub control

        subControl >>= 1;
    }

    // Check whether the 'current' sub control is the arrow we are after

    if (QStyle::SubControl(subControl) == QStyle::SC_ComboBoxArrow)
        QComboBox::mousePressEvent(pEvent);
    else
        pEvent->accept();
}

//==============================================================================

static const auto TrueValue  = QStringLiteral("True");
static const auto FalseValue = QStringLiteral("False");

//==============================================================================

BooleanEditorWidget::BooleanEditorWidget(QWidget *pParent) :
    ListEditorWidget(pParent)
{
    // Add "True" and "False" to ourselves

    addItem(TrueValue);
    addItem(FalseValue);
}

//==============================================================================

PropertyItemDelegate::PropertyItemDelegate(OpenCOR::Core::PropertyEditorWidget *pParent) :
    QStyledItemDelegate(pParent),
    mPropertyEditorWidget(pParent)
{
}

//==============================================================================

QWidget * PropertyItemDelegate::createEditor(QWidget *pParent,
                                             const QStyleOptionViewItem &pOption,
                                             const QModelIndex &pIndex) const
{
    Q_UNUSED(pOption);

    // Create and return an editor for our item, based on its type

    QWidget *editor = 0;
    Property *property = static_cast<PropertyItem *>(qobject_cast<const QStandardItemModel *>(pIndex.model())->itemFromIndex(pIndex))->owner();

    switch (property->type()) {
    case Property::Section:
        // A section, so no editor

        return 0;
    case Property::String:
        editor = new TextEditorWidget(pParent);

        break;
    case Property::Integer:
        editor = new IntegerEditorWidget(pParent);

        break;
    case Property::Double:
        editor = new DoubleEditorWidget(pParent);

        break;
    case Property::List: {
        ListEditorWidget *listEditor = new ListEditorWidget(pParent);

        editor = listEditor;

        // Add the value items to the list, keeping in mind separators

        foreach (const QString &valueItem, property->listValues()) {
            if (valueItem.isEmpty())
                listEditor->insertSeparator(listEditor->count());
            else
                listEditor->addItem(valueItem);
        }

        // Propagate the signal telling us about the list property value having
        // changed

        connect(listEditor, SIGNAL(currentIndexChanged(const QString &)),
                this, SLOT(listPropertyChanged(const QString &)));

        break;
    }
    case Property::Boolean: {
        BooleanEditorWidget *booleanEditor = new BooleanEditorWidget(pParent);

        editor = booleanEditor;

        // Propagate the signal telling us about the list property value having
        // changed

        connect(booleanEditor, SIGNAL(currentIndexChanged(const QString &)),
                this, SLOT(booleanPropertyChanged(const QString &)));

        break;
    }
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
    // Ignore events resulting from a key being pressed

    return (pEvent->type() == QEvent::KeyPress)?
               false:
               QStyledItemDelegate::eventFilter(pObject, pEvent);
}

//==============================================================================

void PropertyItemDelegate::paint(QPainter *pPainter,
                                 const QStyleOptionViewItem &pOption,
                                 const QModelIndex &pIndex) const
{
    // Paint the item as normal, except if it is a section item

    Property *property = static_cast<PropertyItem *>(qobject_cast<const QStandardItemModel *>(pIndex.model())->itemFromIndex(pIndex))->owner();

    QStyleOptionViewItem option(pOption);

    initStyleOption(&option, pIndex);

    if (property->type() == Property::Section) {
        // Make our section item bold

        option.font.setBold(true);
    }

    QStyledItemDelegate::paint(pPainter, option, pIndex);
}

//==============================================================================

void PropertyItemDelegate::listPropertyChanged(const QString &pValue)
{
    // Force the updating of our list property value

    mPropertyEditorWidget->currentProperty()->setValue(pValue);
}

//==============================================================================

void PropertyItemDelegate::booleanPropertyChanged(const QString &pValue)
{
    // Force the updating of our boolean property value

    mPropertyEditorWidget->currentProperty()->setValue(pValue);
}

//==============================================================================

PropertyItem::PropertyItem(Property *pOwner) :
    QStandardItem(),
    mOwner(pOwner)
{
}

//==============================================================================

Property * PropertyItem::owner() const
{
    // Return our owner

    return mOwner;
}

//==============================================================================

Property::Property(const Type &pType, PropertyEditorWidget *pParent) :
    mOwner(pParent),
    mType(pType),
    mId(QString()),
    mName(new PropertyItem(this)),
    mValue(new PropertyItem(this)),
    mUnit(pParent->showUnits()?new PropertyItem(this):0),
    mListValues(QStringList()),
    mEmptyListValue(UnknownValue),
    mExtraInfo(QString()),
    mParentProperty(0),
    mProperties(QList<Property *>())
{
    // Note: mName, mValue and mUnit get owned by our property editor widget, so
    //       no need to delete them afterwards...

    // Make sure that our name and unit can't be edited

    mName->setEditable(false);

    if (mUnit)
        mUnit->setEditable(false);
}

//==============================================================================

Property::Type Property::type() const
{
    // Return our type

    return mType;
}

//==============================================================================

QStandardItem * Property::parent() const
{
    // Return our parent

    return mName->parent();
}

//==============================================================================

int Property::row() const
{
    // Return our row

    return mName->row();
}

//==============================================================================

Property * Property::parentProperty() const
{
    // Return our parent property

    return mParentProperty;
}

//==============================================================================

void Property::setParentProperty(Property *pProperty)
{
    // Set our parent property

    mParentProperty = pProperty;
}

//==============================================================================

QList<Property *> Property::properties() const
{
    // Return our properties

    return mProperties;
}

//==============================================================================

void Property::add(Property *pProperty)
{
    // Add the property to ourselves

    mProperties << pProperty;

    mName->appendRow(pProperty->items());

    // Let the property that we are its parent

    pProperty->setParentProperty(this);
}

//==============================================================================

void Property::addTo(QStandardItem *pParent)
{
    // Add ourselves to the given parent

    pParent->appendRow(items());
}

//==============================================================================

QList<QStandardItem *> Property::items() const
{
    // Return our items as a list

    QList<QStandardItem *> res = QList<QStandardItem *>() << mName << mValue;

    if (mUnit)
        res << mUnit;

    return res;
}

//==============================================================================

bool Property::hasIndex(const QModelIndex &pIndex) const
{
    // Return whether the given index is that of our name, value or unit item

    bool res = (mName->index() == pIndex) || (mValue->index() == pIndex);

    if (mUnit)
        res = res || (mUnit->index() == pIndex);

    return res;
}

//==============================================================================

QModelIndex Property::index() const
{
    // Return our index

    return mName->index();
}

//==============================================================================

QString Property::id() const
{
    // Return our id

    return mId;
}

//==============================================================================

void Property::setId(const QString &pId)
{
    // Set our id

    mId = pId;
}

//==============================================================================

bool Property::isCheckable() const
{
    // Return whether our name item is checkable

    return mName->isCheckable();
}

//==============================================================================

void Property::setCheckable(const bool &pCheckable)
{
    // Make our name item (un)checkable

    mName->setCheckable(pCheckable);
}

//==============================================================================

bool Property::isChecked() const
{
    // Return whether our name item is checked

    return mName->checkState() == Qt::Checked;
}

//==============================================================================

void Property::setChecked(const bool &pChecked)
{
    // Make our name item (un)checked

    mName->setCheckState(pChecked?Qt::Checked:Qt::Unchecked);
}

//==============================================================================

bool Property::isEditable() const
{
    // Return whether our value item is editable

    return mValue->isEditable();
}

//==============================================================================

void Property::setEditable(const bool &pEditable)
{
    // Make our value item (non-)editable

    mValue->setEditable(pEditable);
}

//==============================================================================

QString Property::name() const
{
    // Return our name

    return mName->text();
}

//==============================================================================

void Property::setName(const QString &pName, const bool &pUpdateToolTip)
{
    // Set our name

    if (pName.compare(mName->text())) {
        mName->setText(pName);

        if (pUpdateToolTip)
            updateToolTip();
    }
}

//==============================================================================

QIcon Property::icon() const
{
    // Return our icon

    return mName->icon();
}

//==============================================================================

void Property::setIcon(const QIcon &pIcon)
{
    // Set our icon

    mName->setIcon(pIcon);
}

//==============================================================================

int Property::integerValue() const
{
    // Return our value as an integer, if it is of that type

    return (mType == Integer)?mValue->text().toInt():0;
}

//==============================================================================

void Property::setIntegerValue(const int &pIntegerValue)
{
    // Set our value, should it be of integer type

    if (mType == Integer)
        setValue(QString::number(pIntegerValue));
}

//==============================================================================

double Property::doubleValue() const
{
    // Return our value as a double, if it is of that type

    return (mType == Double)?mValue->text().toDouble():0.0;
}

//==============================================================================

void Property::setDoubleValue(const double &pDoubleValue,
                              const bool &pEmitSignal)
{
    // Set our value, should it be of double type

    if (mType == Double) {
        setValue(QString::number(pDoubleValue, 'g', 15), false, pEmitSignal);
        // Note: we want as much precision as possible, hence we use 15 (see
        //       http://en.wikipedia.org/wiki/Double_precision)...
    }
}

//==============================================================================

QString Property::value() const
{
    // Return our value

    return mValue->text();
}

//==============================================================================

void Property::setValue(const QString &pValue, const bool &pForce,
                        const bool &pEmitSignal)
{
    // Set our value

    if (pValue.compare(mValue->text()) || pForce) {
        QString oldValue = mValue->text();

        mValue->setText(pValue);

        updateToolTip();

        // Let people know if we have a new value

        if (pEmitSignal)
            emit valueChanged(oldValue, pValue);
    }
}

//==============================================================================

QStringList Property::listValues() const
{
    // Return our list values, if any

    return (mType == List)?mListValues:QStringList();
}

//==============================================================================

void Property::setListValues(const QStringList &pListValues,
                             const QString &pListValue, const bool &pEmitSignal)
{
    // Make sure that there would be a point in setting the list values

    if (mType != List)
        return;

    // Clean up the list values we were given:
    //  - Remove leading empty items
    //  - Add items, making sure that only one empty item (i.e. separator) can
    //    be used at once
    //  - Remove the trailing empty item, if any

    QStringList listValues = QStringList();
    int i = 0;
    int iMax = pListValues.count();

    for (; i < iMax; ++i) {
        if (!pListValues[i].isEmpty())
            break;
    }

    bool prevItemIsSeparator = false;

    for (; i < iMax; ++i) {
        QString listValue = pListValues[i];

        if (!listValue.isEmpty()) {
            listValues << listValue;

            prevItemIsSeparator = false;
        } else if (!prevItemIsSeparator) {
            listValues << listValue;

            prevItemIsSeparator = true;
        }
    }

    if (!listValues.isEmpty() && listValues.last().isEmpty())
        listValues.removeLast();

    // Set our list values, if appropriate

    if (listValues != mListValues) {
        mListValues = listValues;

        // Update our value using the requested item from our new list, if it
        // isn't empty, otherwise use our empty list value

        int listValueIndex = mListValues.indexOf(pListValue);

        setValue(mListValues.isEmpty()?
                     mEmptyListValue:
                     (pListValue.isEmpty() || (listValueIndex == -1))?
                         mListValues.first():
                         mListValues[listValueIndex],
                 false, pEmitSignal);
    }
}

//==============================================================================

void Property::setListValues(const QStringList &pListValues,
                             const bool &pEmitSignal)
{
    // Set our list values with no default value

    setListValues(pListValues, QString(), pEmitSignal);
}

//==============================================================================

QString Property::listValue() const
{
    // Return our list value

    return mValue->text();
}

//==============================================================================

void Property::setListValue(const QString &pListValue)
{
    // Set our list value, if appropriate

    if (    (mType == List)
        && !mListValues.isEmpty() && mValue->text().compare(pListValue)) {
        int listValueIndex = mListValues.indexOf(pListValue);

        if (listValueIndex != -1)
            setValue(mListValues[listValueIndex]);
    }
}

//==============================================================================

QString Property::emptyListValue() const
{
    // Return our empty list value

    return mEmptyListValue;
}

//==============================================================================

void Property::setEmptyListValue(const QString &pEmptyListValue)
{
    // Set our empty list value, if appropriate

    if ((mType == List) && pEmptyListValue.compare(mEmptyListValue)) {
        mEmptyListValue = pEmptyListValue;

        // Keep our current value, if the list is not empty, otherwise update it
        // with our new empty list value

        setValue(mListValues.isEmpty()?mEmptyListValue:mValue->text());
    }
}

//==============================================================================

bool Property::booleanValue() const
{
    // Return our value as a boolean, if it is of that type

    return (mType == Boolean)?!mValue->text().compare(TrueValue):false;
}

//==============================================================================

void Property::setBooleanValue(const bool &pBooleanValue)
{
    // Set our value, should it be of boolean type

    if (mType == Boolean)
        setValue(pBooleanValue?TrueValue:FalseValue);
}

//==============================================================================

QString Property::unit() const
{
    // Return our unit

    return mUnit?mUnit->text():QString();
}

//==============================================================================

void Property::setUnit(const QString &pUnit, const bool &pUpdateToolTip)
{
    // Set our unit, if it's not of section type

    if (mUnit && (mType != Section) && pUnit.compare(mUnit->text())) {
        mUnit->setText(pUnit);

        if (pUpdateToolTip)
            updateToolTip();
    }
}

//==============================================================================

QString Property::extraInfo() const
{
    // Return our extra info

    return mExtraInfo;
}

//==============================================================================

void Property::setExtraInfo(const QString &pExtraInfo)
{
    // Set our extra info

    if (pExtraInfo.compare(mExtraInfo)) {
        mExtraInfo = pExtraInfo;

        updateToolTip();
    }
}

//==============================================================================

bool Property::isVisible() const
{
    // Return our visibility

    return mOwner->isRowHidden(mName->row(),
                               mName->parent()?
                                   mName->parent()->index():
                                   qobject_cast<QStandardItemModel *>(mOwner->model())->invisibleRootItem()->index());
}

//==============================================================================

void Property::setVisible(const bool &pVisible)
{
    // Set our visibility

    mOwner->setRowHidden(mName->row(),
                         mName->parent()?
                             mName->parent()->index():
                             qobject_cast<QStandardItemModel *>(mOwner->model())->invisibleRootItem()->index(),
                         !pVisible);

    // Let people know that our visibility has changed

    emit visibilityChanged(pVisible);
}

//==============================================================================

void Property::select() const
{
    // Have our owner select ourselves (i.e. our value), preserving the position
    // of our horizontal scrollbar

    int horizontalScrollBarValue = mOwner->horizontalScrollBar()->value();

    mOwner->setCurrentIndex(mValue->index());
    mOwner->horizontalScrollBar()->setValue(horizontalScrollBarValue);
}

//==============================================================================

void Property::edit() const
{
    // Have our owner edit our value

    if (mValue->isEditable())
        mOwner->edit(mValue->index());
}

//==============================================================================

void Property::updateToolTip()
{
    // Update our tool tip based on the values of our name, value and unit

    QString toolTip = mName->text();

    if (mType != Section) {
        toolTip += QObject::tr(": ");

        if (mValue->text().isEmpty())
            toolTip += UnknownValue;
        else
            toolTip += mValue->text();

        if (mUnit && !mUnit->text().isEmpty())
            toolTip += " "+mUnit->text();
    }

    if (!mExtraInfo.isEmpty())
        toolTip += " ("+mExtraInfo+")";

    mName->setToolTip(toolTip);

    if (mType != Section) {
        mValue->setToolTip(toolTip);

        if (mUnit)
            mUnit->setToolTip(toolTip);
    }
}

//==============================================================================

PropertyEditorWidget::PropertyEditorWidget(const bool &pShowUnits,
                                           const bool &pAutoUpdateHeight,
                                           QWidget *pParent) :
    TreeViewWidget(pParent)
{
    // Some initialisations

    mShowUnits = pShowUnits;
    mAutoUpdateHeight = pAutoUpdateHeight;

    mProperties = Properties();

    mProperty = 0;
    mPropertyEditor = 0;

    mRightClicking = false;

    mPropertyChecked.clear();

    // Customise ourselves

    setRootIsDecorated(false);

    // Create and set our data model

    mModel = new QStandardItemModel(this);

    setModel(mModel);

    // Create our item delegate and set it, after making sure that we handle a
    // few of its signals

    PropertyItemDelegate *propertyItemDelegate = new PropertyItemDelegate(this);

    connect(selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(editorClosed()));

    connect(mModel, SIGNAL(itemChanged(QStandardItem *)),
            this, SLOT(checkCheckState(QStandardItem *)));

    connect(propertyItemDelegate, SIGNAL(openEditor(QWidget *)),
            this, SLOT(editorOpened(QWidget *)));
    connect(propertyItemDelegate, SIGNAL(closeEditor(QWidget *, QAbstractItemDelegate::EndEditHint)),
            this, SLOT(editorClosed()));

    connect(propertyItemDelegate, SIGNAL(goToPreviousPropertyRequested()),
            this, SLOT(goToPreviousProperty()));
    connect(propertyItemDelegate, SIGNAL(goToNextPropertyRequested()),
            this, SLOT(goToNextProperty()));

    setItemDelegate(propertyItemDelegate);

    // Resize our height in case some data has changed or one of the properties
    // gets expanded/collapsed

    connect(mModel, SIGNAL(itemChanged(QStandardItem *)),
            this, SLOT(updateHeight()));

    connect(this, SIGNAL(collapsed(const QModelIndex &)),
            this, SLOT(updateHeight()));
    connect(this, SIGNAL(expanded(const QModelIndex &)),
            this, SLOT(updateHeight()));

    header()->setSectionsMovable(false);

    // Retranslate ourselves, so that we are properly initialised

    retranslateUi();

    // Initialise our height

    updateHeight();
}

//==============================================================================

PropertyEditorWidget::PropertyEditorWidget(const bool &pAutoUpdateHeight,
                                           QWidget *pParent) :
    PropertyEditorWidget(true, pAutoUpdateHeight, pParent)
{
}

//==============================================================================

PropertyEditorWidget::PropertyEditorWidget(QWidget *pParent) :
    PropertyEditorWidget(true, false, pParent)
{
}

//==============================================================================

PropertyEditorWidget::~PropertyEditorWidget()
{
    // Clear ourselves

    clear();
}

//==============================================================================

void PropertyEditorWidget::retranslateEmptyListProperties(QStandardItem *pItem)
{
    // Retranslate the current item, should it be an empty list

    Property *currentProperty = property(pItem->index());

    if (    currentProperty
        && (currentProperty->type() == Property::List)
        &&  currentProperty->listValues().isEmpty()) {
        currentProperty->setValue(currentProperty->emptyListValue());
    }

    // Retranslate the current item's children, if any

    for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i)
        retranslateEmptyListProperties(pItem->child(i));
}

//==============================================================================

void PropertyEditorWidget::retranslateUi()
{
    // Retranslate our header labels

    QStringList horizontalHeaderLabels = QStringList() << tr("Property")
                                                       << tr("Value");

    if (mShowUnits)
        horizontalHeaderLabels << tr("Unit");

    mModel->setHorizontalHeaderLabels(horizontalHeaderLabels);

    // 'Retranslate' the value of all empty list properties

    retranslateEmptyListProperties(mModel->invisibleRootItem());
}

//==============================================================================

int PropertyEditorWidget::childrenRowHeight(const QStandardItem *pItem) const
{
    // Return the total height of the given index's children

    int res = 0;

    if (pItem->hasChildren()) {
        for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i) {
            QStandardItem *childItem = pItem->child(i);
            int childIndexHeight = rowHeight(childItem->index());

            if (childIndexHeight)
                res += childIndexHeight+childrenRowHeight(childItem);
        }
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

        int hintWidth = 0;
        int hintHeight = header()->height();

        for (int i = 0, iMax = header()->count(); i < iMax; ++i)
            hintWidth += columnWidth(i);

        for (int i = 0, iMax = mModel->rowCount(); i < iMax; ++i) {
            QStandardItem *rowItem = mModel->item(i);
            int rowItemHeight = rowHeight(rowItem->index());

            if (rowItemHeight) {
                // Our current row has some height, meaning that it is visible,
                // so we can set its height to our hintHeight, as well as
                // retrieve the total height of our row's children

                hintHeight += rowItemHeight+childrenRowHeight(rowItem);
            }
        }

        return QSize(hintWidth, hintHeight);
    } else {
        // We don't automatically resize our height, so our ideal size is our
        // maximum size

        return maximumSize();
    }
}

//==============================================================================

void PropertyEditorWidget::clear()
{
    // Clear our data model

    mModel->clear();

    // Delete some internal objects

    foreach (Property *property, mProperties)
        delete property;

    mProperties.clear();
}

//==============================================================================

void PropertyEditorWidget::selectFirstProperty()
{
    // Convenience function to select the first property, i.e. the first item

    selectFirstItem();
}

//==============================================================================

Property * PropertyEditorWidget::addProperty(const Property::Type &pType,
                                             Property *pParent)
{
    // Create our property

    Property *res = new Property(pType, this);

    // Populate our data model with our property

    if (pParent) {
        // We want to add a child property

        pParent->add(res);

        // If we want to see the child property, we need root decoration

        setRootIsDecorated(true);
    } else {
        // We want to add a root property

        res->addTo(mModel->invisibleRootItem());
    }

    // Span ourselves if we are of section type

    if (pType == Property::Section) {
        setFirstColumnSpanned(res->row(),
                              pParent?pParent->index():mModel->invisibleRootItem()->index(),
                              true);
    }

    // Keep track of our property's change of visibility

    connect(res, SIGNAL(visibilityChanged(const bool &)),
            this, SLOT(updateHeight()));

    // Keep track of our property's change of value

    connect(res, SIGNAL(valueChanged(const QString &, const QString &)),
            this, SLOT(emitPropertyChanged()));

    // Keep track of our property's check state

    mPropertyChecked.insert(res, res->isChecked());

    // Keep track of our property and return it

    mProperties << res;

    return res;
}

//==============================================================================

Property * PropertyEditorWidget::addSectionProperty(const QString &pName,
                                                    Property *pParent)
{
    // Add a section property and return its information

    Property *res = addProperty(Property::Section, pParent);

    res->setName(pName);

    return res;
}

//==============================================================================

Property * PropertyEditorWidget::addSectionProperty(Property *pParent)
{
    // Add a section property and return its information

    return addSectionProperty(QString(), pParent);
}

//==============================================================================

Property * PropertyEditorWidget::addIntegerProperty(const int &pValue,
                                                    Property *pParent)
{
    // Add an integer property and return its information

    Property *res = addProperty(Property::Integer, pParent);

    res->setIntegerValue(pValue);

    return res;
}

//==============================================================================

Property * PropertyEditorWidget::addIntegerProperty(Property *pParent)
{
    // Add an integer property and return its information

    return addIntegerProperty(0, pParent);
}

//==============================================================================

Property * PropertyEditorWidget::addDoubleProperty(const double &pValue,
                                                   Property *pParent)
{
    // Add a double property and return its information

    Property *res = addProperty(Property::Double, pParent);

    res->setDoubleValue(pValue);

    return res;
}

//==============================================================================

Property * PropertyEditorWidget::addDoubleProperty(Property *pParent)
{
    // Add a double property and return its information

    return addDoubleProperty(0.0, pParent);
}

//==============================================================================

Property * PropertyEditorWidget::addListProperty(const QStringList &pValues,
                                                 const QString &pValue,
                                                 Property *pParent)
{
    // Add a list property and return its information
    // Note: a list property is necessarily editable...

    Property *res = addProperty(Property::List, pParent);

    res->setListValues(pValues, pValue);

    return res;
}

//==============================================================================

Property * PropertyEditorWidget::addListProperty(const QStringList &pValues,
                                                 Property *pParent)
{
    // Add a list property and return its information

    return addListProperty(pValues, 0, pParent);
}

//==============================================================================

Property * PropertyEditorWidget::addListProperty(Property *pParent)
{
    // Add a list property and return its information

    return addListProperty(QStringList(), 0, pParent);
}

//==============================================================================

Property * PropertyEditorWidget::addBooleanProperty(const bool &pValue,
                                                    Property *pParent)
{
    // Add a boolean property and return its information

    Property *res = addProperty(Property::Boolean, pParent);

    res->setBooleanValue(pValue);

    return res;
}

//==============================================================================

Property * PropertyEditorWidget::addBooleanProperty(Property *pParent)
{
    // Add a boolean property and return its information

    return addBooleanProperty(false, pParent);
}

//==============================================================================

Property * PropertyEditorWidget::addStringProperty(const QString &pString,
                                                   Property *pParent)
{
    // Add a string property and return its information

    Property *res = addProperty(Property::String, pParent);

    res->setValue(pString);

    return res;
}

//==============================================================================

Property * PropertyEditorWidget::addStringProperty(Property *pParent)
{
    // Add a string property and return its information

    return addStringProperty(QString(), pParent);
}

//==============================================================================

void PropertyEditorWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Check some key combinations

    bool noModifiers =    !(pEvent->modifiers() & Qt::ShiftModifier)
                       && !(pEvent->modifiers() & Qt::ControlModifier)
                       && !(pEvent->modifiers() & Qt::AltModifier)
                       && !(pEvent->modifiers() & Qt::MetaModifier);
    bool controlModifier =    !(pEvent->modifiers() & Qt::ShiftModifier)
                           &&  (pEvent->modifiers() & Qt::ControlModifier)
                           && !(pEvent->modifiers() & Qt::AltModifier)
                           && !(pEvent->modifiers() & Qt::MetaModifier);

    if (controlModifier && (pEvent->key() == Qt::Key_A)) {
        // The user wants to select everything which we don't want to allow,
        // so just accept the event...

        pEvent->accept();
    } else if (noModifiers && (   (pEvent->key() == Qt::Key_Return)
                               || (pEvent->key() == Qt::Key_Enter))) {
        // The user wants to start/stop editing the property

        if (mPropertyEditor)
            editProperty(0);
        else
            editProperty(currentProperty());

        // Accept the event

        pEvent->accept();
    } else if (noModifiers && (pEvent->key() == Qt::Key_Escape)) {
        // The user wants to cancel the editing

        finishEditing(false);

        // Accept the event

        pEvent->accept();
    } else if (noModifiers && (pEvent->key() == Qt::Key_Up)) {
        // The user wants to go the previous property

        goToPreviousProperty();

        // Accept the event

        pEvent->accept();
    } else if (noModifiers && (pEvent->key() == Qt::Key_Down)) {
        // The user wants to go to the next property

        goToNextProperty();

        // Accept the event

        pEvent->accept();
    } else {
        // Default handling of the event

        TreeViewWidget::keyPressEvent(pEvent);
    }
}

//==============================================================================

void PropertyEditorWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    // Default handling of the event
    // Note: this will finish the editing of our 'old' property, if any...

    TreeViewWidget::mouseMoveEvent(pEvent);

    // Edit our 'new' property, but only if we are not right-clicking
    // Note: pEvent->button() is always equal to Qt::NoButton for mouse move
    //       events, hence we rely on mRightClicking instead...

    if (mRightClicking) {
        // We are right clicking, so we just make sure that the 'new' property
        // is selected

        selectProperty(property(indexAt(pEvent->pos())));
    } else {
        // We are not right clicking, so we can edit the 'new' property (which
        // will, as a result, also select it)

        editProperty(property(indexAt(pEvent->pos())));
    }
}

//==============================================================================

void PropertyEditorWidget::mousePressEvent(QMouseEvent *pEvent)
{
    // Keep track of our 'old' property, if any

    Property *oldProperty = mProperty;

    // Default handling of the event
    // Note: this will finish the editing of our 'old' property, if any...

    TreeViewWidget::mousePressEvent(pEvent);

    // Edit our 'new' property, but only if we are not right-clicking and if
    // there is a 'new' property and it is different from our 'old' property,
    // otherwise cancel any editing if we are right-clicking

    Property *newProperty = property(indexAt(pEvent->pos()));

    mRightClicking = pEvent->button() == Qt::RightButton;

    if (mRightClicking)
        finishEditing(false);
    else if (newProperty && (newProperty != oldProperty))
        editProperty(newProperty);
}

//==============================================================================

void PropertyEditorWidget::mouseReleaseEvent(QMouseEvent *pEvent)
{
    // Reset our right clicking state

    mRightClicking = false;

    // Default handling of the event
    // Note: this will finish the editing of our 'old' property, if any...

    TreeViewWidget::mouseReleaseEvent(pEvent);
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
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
                            horizontalScrollBar()->height():
#elif defined(Q_OS_LINUX)
                            horizontalScrollBar()->height()+3:
                            // Note: why on earth does Linux require three more
                            //       pixels?!... Indeed, if we don't add them,
                            //       then there won't be enough space to show
                            //       the last property (upon selecting it) and
                            //       the widget will increase its height,
                            //       resulting in some blank space at the bottom
                            //       and a vertical bar being shown! We could
                            //       prevent the vertical bar from ever being
                            //       shown, but not sure what could be done
                            //       about the blank space, hence we 'manually'
                            //       add those three pixels...
#else
    #error Unsupported platform
#endif
                            0));
        // Note: the new height consists of our ideal height to which we add the
        //       height of our horizontal scroll bar, should it be shown (i.e.
        //       if our width is smaller than that of our ideal size)...
    }
}

//==============================================================================

void PropertyEditorWidget::checkCheckState(QStandardItem *pItem)
{
    // Let people know whether the check state of the given property has changed

    Property *currentProperty = property(pItem->index());
    bool oldPropertyChecked = mPropertyChecked.value(currentProperty, false);
    bool newPropertyChecked = currentProperty->isChecked();

    if (oldPropertyChecked != newPropertyChecked) {
        // Keep track of the property's new check state

        mPropertyChecked.insert(currentProperty, newPropertyChecked);

        // The property's check state has changed, so let people know about it

        emit propertyChanged(currentProperty);
    }
}

//==============================================================================

void PropertyEditorWidget::editorOpened(QWidget *pEditor)
{
    // Keep track of some information about the property

    mProperty = currentProperty();
    mPropertyEditor = pEditor;

    // We are starting the editing of a property, so make sure that if we are to
    // edit a list item, then its original value gets properly set
    // Note: indeed, by default the first list item will be selected...

    if (mProperty->type() == Property::List) {
        QString propertyValue = mProperty->value();
        QStringList propertyListValues = mProperty->listValues();
        ListEditorWidget *propertyEditor = static_cast<ListEditorWidget *>(mPropertyEditor);

        for (int i = 0, iMax = propertyListValues.count(); i < iMax; ++i) {
            if (!propertyValue.compare(propertyListValues[i])) {
                propertyEditor->setCurrentIndex(i);

                break;
            }
        }
    }

    // Next, we need to use the property's editor as our focus proxy and make
    // sure that it immediately gets the focus
    // Note: if we were not to immediately give the focus to our editor, then
    //       the central widget would give the focus to the previously focused
    //       widget (see CentralWidget::updateGui()), which is clearly not what
    //       we want...

    setFocusProxy(pEditor);

    pEditor->setFocus();
}

//==============================================================================

void PropertyEditorWidget::editorClosed()
{
    // Make sure that we are editing a property

    if (!mPropertyEditor)
        return;

    // We have stopped editing a property, so make sure that its value is
    // updated
    // Note #1: we don't need to do this for a list property or a boolean
    //          property since such a property will have already been updated
    //          (see listPropertyChanged() and booleanPropertyChanged())...
    // Note #2: we should always set (and force) the value of the property, even
    //          if we are not dealing with an 'empty' integer or double property
    //          since only the text of the property item will have been updated
    //          (through QTreeView) while Property::setValue() will do a few
    //          more things (e.g. update the tool tip)...

    if (   (mProperty->type() != Property::List)
        && (mProperty->type() != Property::Boolean)) {
        // Not a list or boolean item, so set its value

        QString value = mProperty->value();

        if (    value.isEmpty()
            && (   (mProperty->type() == Property::Integer)
                || (mProperty->type() == Property::Double))) {
            // We are dealing with an 'empty' integer or double property, so set
            // its value to zero

            value = "0";
        }

        mProperty->setValue(value, true);
    }

    // Reset our focus proxy and make sure that we get the focus (see
    // editorOpened() above for the reason)

    setFocusProxy(0);
    setFocus();

    // Reset some information about the property

    mProperty = 0;
    mPropertyEditor = 0;
}

//==============================================================================

void PropertyEditorWidget::selectProperty(Property *pProperty)
{
    // Select the property, if one is provided

    if (!pProperty)
        return;

    pProperty->select();
}

//==============================================================================

void PropertyEditorWidget::editProperty(Property *pProperty,
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

    if (pProperty) {
        // There is a new property to edit, so first make sure that it is
        // selected

        selectProperty(pProperty);

        // Now, we can 'properly' edit the property's value (assuming it's
        // editable)

        pProperty->edit();
    }
}

//==============================================================================

bool PropertyEditorWidget::removeProperty(Property *pProperty)
{
    // Remove the given property and any of its children, but first make sure
    // that we know about the given property

    if (!mProperties.contains(pProperty))
        return false;

    // Stop tracking the property

    mProperties.removeOne(pProperty);

    // Remove the property from our model
    // Note: the below will remove the given property and any of its children

    mModel->removeRow(pProperty->row(),
                      pProperty->parent()?
                          pProperty->parent()->index():
                          mModel->invisibleRootItem()->index());

    // Delete the given property and any of its children

    deleteProperty(pProperty);

    return true;
}

//==============================================================================

void PropertyEditorWidget::deleteProperty(Property *pProperty)
{
    // Recursively delete the memory associated with the given property and any
    // of its children

    foreach (Property *property, pProperty->properties())
        deleteProperty(property);

    mProperties.removeOne(pProperty);

    delete pProperty;
}

//==============================================================================

Properties PropertyEditorWidget::properties() const
{
    // Return our properties

    return mProperties;
}

//==============================================================================

void PropertyEditorWidget::finishEditing(const bool &pCommitData)
{
    // The user wants to finish the editing

    editProperty(0, pCommitData);
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

void PropertyEditorWidget::emitPropertyChanged()
{
    // Let people know that one of our properties has changed

    emit propertyChanged(qobject_cast<Property *>(sender()));
}

//==============================================================================

Property * PropertyEditorWidget::property(const QModelIndex &pIndex) const
{
    // Don't waste time if the given index isn't valid

    if (!pIndex.isValid())
        return 0;

    // Return our information about the property at the given index

    foreach (Property *property, mProperties) {
        if (property->hasIndex(pIndex))
            return property;
    }

    return 0;
}

//==============================================================================

Property * PropertyEditorWidget::currentProperty() const
{
    // Return some information about the current property

    return property(currentIndex());
}

//==============================================================================

bool PropertyEditorWidget::showUnits() const
{
    // Return whether we show units

    return mShowUnits;
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
