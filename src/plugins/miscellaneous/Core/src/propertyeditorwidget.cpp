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

#include <cfloat>

//==============================================================================

#include <QAbstractItemDelegate>
#include <QAbstractItemView>
#include <QColorDialog>
#include <QEvent>
#include <QHeaderView>
#include <QKeyEvent>
#include <QMainWindow>
#include <QModelIndex>
#include <QPainter>
#include <QRegularExpressionValidator>
#include <QScrollBar>
#include <QSettings>
#include <QStandardItem>
#include <QStyle>
#include <QStyleOptionViewItem>
#include <QTimer>
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
    setFrame(false);
}

//==============================================================================

void TextEditorWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Check some key combinations

    if (   (pEvent->modifiers() == Qt::KeypadModifier)
        && (pEvent->key() == Qt::Key_Up)) {
        // The user wants to go to the previous property

        emit goToPreviousPropertyRequested();

        pEvent->accept();
    } else if (   (pEvent->modifiers() == Qt::KeypadModifier)
               && (pEvent->key() == Qt::Key_Down)) {
        // The user wants to go to the next property

        emit goToNextPropertyRequested();

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

    static const QRegularExpression IntegerRegEx = QRegularExpression("^[+-]?\\d+$");

    setValidator(new QRegularExpressionValidator(IntegerRegEx, this));
}

//==============================================================================

IntegerGe0EditorWidget::IntegerGe0EditorWidget(QWidget *pParent) :
    TextEditorWidget(pParent)
{
    // Set a validator that accepts zero or any strictly positive integer

    static const QRegularExpression IntegerGe0RegEx = QRegularExpression("^[+]?\\d+$");

    setValidator(new QRegularExpressionValidator(IntegerGe0RegEx, this));
}

//==============================================================================

IntegerGt0EditorWidget::IntegerGt0EditorWidget(QWidget *pParent) :
    TextEditorWidget(pParent)
{
    // Set a validator that accepts any strictly positive integer

    static const QRegularExpression IntegerGt0RegEx = QRegularExpression("^[+]?[1-9]\\d*$");

    setValidator(new QRegularExpressionValidator(IntegerGt0RegEx, this));
}

//==============================================================================

DoubleEditorWidget::DoubleEditorWidget(QWidget *pParent) :
    TextEditorWidget(pParent)
{
    // Set a validator that accepts any double

    static const QRegularExpression DoubleRegEx = QRegularExpression(R"(^[+-]?(\d+(\.\d*)?|\.\d+)([eE][+-]?\d+)?$)");

    setValidator(new QRegularExpressionValidator(DoubleRegEx, this));
}

//==============================================================================

DoubleGe0EditorWidget::DoubleGe0EditorWidget(QWidget *pParent) :
    TextEditorWidget(pParent)
{
    // Set a validator that accepts zero or any strictly positive double

    static const QRegularExpression DoubleGe0RegEx = QRegularExpression(R"(^[+]?(\d+(\.\d*)?|\.\d+)([eE][+-]?\d+)?$)");

    setValidator(new QRegularExpressionValidator(DoubleGe0RegEx, this));
}

//==============================================================================

DoubleGt0EditorWidget::DoubleGt0EditorWidget(QWidget *pParent) :
    TextEditorWidget(pParent)
{
    // Set a validator that accepts any strictly positive double

    static const QRegularExpression DoubleGt0RegEx = QRegularExpression(R"(^[+]?([1-9]\d*(\.\d*)?|[0]?\.\d+)([eE][+-]?\d+)?$)");

    setValidator(new QRegularExpressionValidator(DoubleGt0RegEx, this));
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

    if (   (pEvent->modifiers() == Qt::KeypadModifier)
        && (pEvent->key() == Qt::Key_Up)) {
        // The user wants to go to the previous property

        emit goToPreviousPropertyRequested();

        pEvent->accept();
    } else if (   (pEvent->modifiers() == Qt::KeypadModifier)
               && (pEvent->key() == Qt::Key_Down)) {
        // The user wants to go to the next property

        emit goToNextPropertyRequested();

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

    if (count() == 0) {
        return;
    }

    // We want to go to the next item in our list (and go back to the first one
    // if we are at the end of our list), so determine the new current index

    int newCurrentIndex = currentIndex();

    do {
        ++newCurrentIndex;

        if (newCurrentIndex == count()) {
            newCurrentIndex = 0;
        }
    } while (itemText(newCurrentIndex).isEmpty());

    // Set the new current index and accept the event

    setCurrentIndex(newCurrentIndex);

    pEvent->accept();
}

//==============================================================================

void ListEditorWidget::mousePressEvent(QMouseEvent *pEvent)
{
    // Check whether the user clicked on the arrow and, if so, allow the default
    // handling of the event (so that our list of items gets displayed) while do
    // nothing if the user clicked somewhere else (this so that if the user
    // double clicks on the widget, then we can select the next item)
    // Note: we would normally call style()->hitTestComplexControl() and, if it
    //       returns QStyle::SC_ComboBoxArrow, then allow the default handling
    //       of the event, but although this works fine on Windows and Linux, it
    //       just doesn't work on macOS. Indeed, no matter where we are over the
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

    while (subControl != 0) {
        // Retrieve the sub control's region

        subControlRect = style()->subControlRect(QStyle::CC_ComboBox,
                                                 &styleOption,
                                                 QStyle::SubControl(subControl),
                                                 this);

        // Check whether the sub control exists (i.e. its region is valid) and,
        // if so, whether we ore over it

        if (subControlRect.isValid() && subControlRect.contains(pEvent->pos())) {
            break;
        }

        // Either the sub control doesn't exist or we are not over it, so try
        // the next sub control

        subControl /= 2;
    }

    // Check whether the 'current' sub control is the arrow we are after

    if (QStyle::SubControl(subControl) == QStyle::SC_ComboBoxArrow) {
        QComboBox::mousePressEvent(pEvent);
    } else {
        pEvent->accept();
    }
}

//==============================================================================

static const char *TrueValue  = "True";
static const char *FalseValue = "False";

//==============================================================================

BooleanEditorWidget::BooleanEditorWidget(QWidget *pParent) :
    ListEditorWidget(pParent)
{
    // Add "True" and "False" to ourselves

    addItem(TrueValue);
    addItem(FalseValue);
}

//==============================================================================

ColorEditorWidget::ColorEditorWidget(QWidget *pParent) :
    TextEditorWidget(pParent)
{
    // Set a validator that accepts any colour of the form #aarrggbb or #rrggbb

    static const QRegularExpression ColorRegEx = QRegularExpression("^#([[:xdigit:]]{6}|[[:xdigit:]]{8})$");

    setValidator(new QRegularExpressionValidator(ColorRegEx, this));
}

//==============================================================================

PropertyItemDelegate::PropertyItemDelegate(PropertyEditorWidget *pParent) :
    StyledItemDelegate(pParent),
    mPropertyEditorWidget(pParent)
{
}

//==============================================================================

QWidget * PropertyItemDelegate::createEditor(QWidget *pParent,
                                             const QStyleOptionViewItem &pOption,
                                             const QModelIndex &pIndex) const
{
    Q_UNUSED(pOption)

    // Create and return an editor for our item, based on its type

    QWidget *editor = nullptr;
    TextEditorWidget *textEditor = nullptr;
    ListEditorWidget *listEditor = nullptr;
    Property *property = static_cast<PropertyItem *>(qobject_cast<const QStandardItemModel *>(pIndex.model())->itemFromIndex(pIndex))->owner();

    switch (property->type()) {
    case Property::Type::Section:
        // A section, so no editor

        return nullptr;
    case Property::Type::String:
        editor = textEditor = new TextEditorWidget(pParent);

        break;
    case Property::Type::Integer:
        editor = textEditor = new IntegerEditorWidget(pParent);

        break;
    case Property::Type::IntegerGe0:
        editor = textEditor = new IntegerGe0EditorWidget(pParent);

        break;
    case Property::Type::IntegerGt0:
        editor = textEditor = new IntegerGt0EditorWidget(pParent);

        break;
    case Property::Type::Double:
        editor = textEditor = new DoubleEditorWidget(pParent);

        break;
    case Property::Type::DoubleGe0:
        editor = textEditor = new DoubleGe0EditorWidget(pParent);

        break;
    case Property::Type::DoubleGt0:
        editor = textEditor = new DoubleGt0EditorWidget(pParent);

        break;
    case Property::Type::List: {
        editor = listEditor = new ListEditorWidget(pParent);

        // Add the value items to our list, keeping in mind separators

        for (const auto &valueItem : property->listValues()) {
            if (valueItem.isEmpty()) {
                listEditor->insertSeparator(listEditor->count());
            } else {
                listEditor->addItem(valueItem);
            }
        }

        break;
    }
    case Property::Type::Boolean: {
        editor = listEditor = new BooleanEditorWidget(pParent);

        break;
    }
    case Property::Type::Color:
        editor = textEditor = new ColorEditorWidget(pParent);

        break;
    }

    // Propagate a few signals

    switch (property->type()) {
    case Property::Type::String:
    case Property::Type::Integer:
    case Property::Type::IntegerGe0:
    case Property::Type::IntegerGt0:
    case Property::Type::Double:
    case Property::Type::DoubleGe0:
    case Property::Type::DoubleGt0:
    case Property::Type::Color:
        connect(textEditor, &TextEditorWidget::goToPreviousPropertyRequested,
                this, &PropertyItemDelegate::goToPreviousPropertyRequested);
        connect(textEditor, &TextEditorWidget::goToNextPropertyRequested,
                this, &PropertyItemDelegate::goToNextPropertyRequested);

        break;
    case Property::Type::List:
    case Property::Type::Boolean:
        connect(listEditor, QOverload<const QString &>::of(&ListEditorWidget::currentIndexChanged),
                this, &PropertyItemDelegate::listPropertyChanged);

        connect(listEditor, &ListEditorWidget::goToPreviousPropertyRequested,
                this, &PropertyItemDelegate::goToPreviousPropertyRequested);
        connect(listEditor, &ListEditorWidget::goToNextPropertyRequested,
                this, &PropertyItemDelegate::goToNextPropertyRequested);

        break;
    case Property::Type::Section:
        // Not a relevant type, so do nothing

        ;
    }

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

    if (property->type() == Property::Type::Section) {
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
    mOwner(pOwner)
{
}

//==============================================================================

PropertyItem::~PropertyItem() = default;

//==============================================================================

Property * PropertyItem::owner() const
{
    // Return our owner

    return mOwner;
}

//==============================================================================

Property::Property(Type pType, PropertyEditorWidget *pParent) :
    mOwner(pParent),
    mType(pType),
    mName(new PropertyItem(this)),
    mValue(new PropertyItem(this)),
    mUnit(pParent->showUnits()?new PropertyItem(this):nullptr)
{
    // Note: mName, mValue and mUnit get owned by our property editor widget, so
    //       no need to delete them afterwards...

    // Make sure that our name and unit can't be edited

    mName->setEditable(false);

    if (mUnit != nullptr) {
        mUnit->setEditable(false);
    }
}

//==============================================================================

PropertyEditorWidget * Property::owner() const
{
    // Return our owner

    return mOwner;
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

Properties Property::properties() const
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

    // Let the property know that we are its parent

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

    if (mUnit != nullptr) {
        res << mUnit;
    }

    return res;
}

//==============================================================================

bool Property::hasIndex(const QModelIndex &pIndex) const
{
    // Return whether the given index is that of our name, value or unit item

    bool res = (mName->index() == pIndex) || (mValue->index() == pIndex);

    if (mUnit != nullptr) {
        res = res || (mUnit->index() == pIndex);
    }

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

void Property::setCheckable(bool pCheckable)
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

void Property::setChecked(bool pChecked)
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

void Property::setEditable(bool pEditable)
{
    // Make our value item (non-)editable

    mValue->setEditable(pEditable);
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

QString Property::name() const
{
    // Return our name

    return mName->text();
}

//==============================================================================

void Property::setName(const QString &pName, bool pUpdateToolTip)
{
    // Set our name

    if (pName != mName->text()) {
        mName->setText(pName);

        if (pUpdateToolTip) {
            updateToolTip();
        }
    }
}

//==============================================================================

QString Property::value() const
{
    // Return our value

    return mValue->text();
}

//==============================================================================

void Property::setValue(const QString &pValue, bool pForce, bool pEmitSignal)
{
    // Set our value (and value icon, if we are a colour property)

    QString oldValue = value();

    if ((pValue != oldValue) || pForce) {
        mValue->setText(pValue);

        if (mType == Type::Color) {
            QPixmap colorPixmap = QPixmap(48, 48);
            QColor color;

            color.setNamedColor(pValue);

            colorPixmap.fill(color);

            mValue->setIcon(colorPixmap);
        }

        updateToolTip();

        // Let people know if we have a new value

        if (pEmitSignal) {
            emit valueChanged(oldValue, pValue);
        }
    }
}

//==============================================================================

QVariant Property::variantValue(bool pListValueIndex) const
{
    // Return our property value as a variant

    switch (mType) {
    case Type::Section:
    case Type::String:
        return value();
    case Type::Integer:
    case Type::IntegerGe0:
    case Type::IntegerGt0:
        return integerValue();
    case Type::Double:
    case Type::DoubleGe0:
    case Type::DoubleGt0:
        return doubleValue();
    case Type::List:
        if (pListValueIndex) {
            return listValueIndex();
        } else {
            return listValue();
        }
    case Type::Boolean:
        return booleanValue();
    case Type::Color:
        return colorValue();
    }

    return {};
    // Note: we can't reach this point, but without it we may, at compilation
    //       time, be told that not all control paths return a value...
}

//==============================================================================

QString Property::stringValue() const
{
    // Return our property value as a string

    switch (mType) {
    case Type::Section:
    case Type::String:
    case Type::Integer:
    case Type::IntegerGe0:
    case Type::IntegerGt0:
    case Type::Double:
    case Type::DoubleGe0:
    case Type::DoubleGt0:
    case Type::List:
    case Type::Color:
        return value();
    case Type::Boolean:
        return QVariant(booleanValue()).toString();
    }

    return {};
    // Note: we can't reach this point, but without it we may, at compilation
    //       time, be told that not all control paths return a value...
}

//==============================================================================

int Property::integerValue() const
{
    // Return our value as an integer

    return value().toInt();
}

//==============================================================================

void Property::setIntegerValue(int pIntegerValue, bool pEmitSignal)
{
    // Set our value, should it be of integer type

    if (mType == Type::Integer) {
        setValue(QString::number(pIntegerValue), false, pEmitSignal);
    } else if (mType == Type::IntegerGe0) {
        setValue(QString::number((pIntegerValue >= 0)?pIntegerValue:1), false, pEmitSignal);
    } else if (mType == Type::IntegerGt0) {
        setValue(QString::number((pIntegerValue > 0)?pIntegerValue:1), false, pEmitSignal);
    }
}

//==============================================================================

double Property::doubleValue() const
{
    // Return our value as a double

    return value().toDouble();
}

//==============================================================================

void Property::setDoubleValue(double pDoubleValue, bool pEmitSignal)
{
    // Set our value, should it be of double type
    // Note: we want as much precision as possible, hence we use a precision of
    //       15 decimals (see http://en.wikipedia.org/wiki/Double_precision)...

    if (mType == Type::Double) {
        setValue(QString::number(pDoubleValue, 'g', 15), false, pEmitSignal);
    } else if (mType == Type::DoubleGe0) {
        setValue(QString::number((pDoubleValue >= 0.0)?pDoubleValue:1.0, 'g', 15), false, pEmitSignal);
    } else if (mType == Type::DoubleGt0) {
        setValue(QString::number((pDoubleValue > 0.0)?pDoubleValue:1.0, 'g', 15), false, pEmitSignal);
    }
}

//==============================================================================

QStringList Property::listValues() const
{
    // Return our list values, if any

    return (mType == Type::List)?mListValues:QStringList();
}

//==============================================================================

void Property::setListValues(const QStringList &pListValues, bool pEmitSignal)
{
    // Make sure that there would be a point in setting the list values

    if (mType != Type::List) {
        return;
    }

    // Set our list values, if appropriate

    if (pListValues != mListValues) {
        int listValueIndex = mListValues.indexOf(value());

        mListValues = pListValues;

        // Update our value using the item at the same index as before, unless
        // our new list of values is empty or the index is not valid anymore

        setValue(mListValues.isEmpty()?
                     mEmptyListValue:
                     ((listValueIndex == -1) || (listValueIndex >= mListValues.count()))?
                         mListValues.first():
                         mListValues[listValueIndex],
                 false, pEmitSignal);
    }
}

//==============================================================================

QString Property::listValue() const
{
    // Return our list value

    return value();
}

//==============================================================================

void Property::setListValue(const QString &pListValue)
{
    // Set our list value, if appropriate

    if (    (mType == Type::List)
        && !mListValues.isEmpty() && (listValue() != pListValue)) {
        int listValueIndex = mListValues.indexOf(pListValue);

        if (listValueIndex != -1) {
            setValue(mListValues[listValueIndex]);
        }
    }
}

//==============================================================================

int Property::listValueIndex() const
{
    // Return our list value index

    return listValues().indexOf(listValue());
}

//==============================================================================

void Property::setListValueIndex(int pListValueIndex)
{
    // Set our list value, if appropriate

    if (    (mType == Type::List)
        && !mListValues.isEmpty()
        && (pListValueIndex >= 0)
        && (pListValueIndex < mListValues.count())
        && (pListValueIndex != listValueIndex())) {
        setValue(mListValues[pListValueIndex]);
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

    if ((mType == Type::List) && (pEmptyListValue != mEmptyListValue)) {
        mEmptyListValue = pEmptyListValue;

        // Keep our current value, if our list is not empty, otherwise update it
        // with our new empty list value

        setValue(mListValues.isEmpty()?mEmptyListValue:value());
    }
}

//==============================================================================

bool Property::booleanValue() const
{
    // Return our value as a boolean

    return value() == TrueValue;
}

//==============================================================================

void Property::setBooleanValue(bool pBooleanValue)
{
    // Set our value, should it be of boolean type

    if (mType == Type::Boolean) {
        setValue(pBooleanValue?TrueValue:FalseValue);
    }
}

//==============================================================================

QColor Property::colorValue() const
{
    // Return our value as a color

    QColor res;

    res.setNamedColor(value());

    return res;
}

//==============================================================================

void Property::setColorValue(const QColor &pColorValue)
{
    // Set our value, should it be of color type

    if (mType == Type::Color) {
        QString colorValueName = pColorValue.name(QColor::HexArgb);

        if (colorValueName.startsWith("#ff")) {
            setValue(pColorValue.name());
        } else {
            setValue(colorValueName);
        }
    }
}

//==============================================================================

void Property::setColorValue(const QPoint &pPoint)
{
    // Make sure that we are of colour type and that the given point is either
    // null or over our colour value icon

    if (mType == Type::Color) {
        if (!pPoint.isNull()) {
            // The given point is not null, so determine the position and size
            // of our colour value icon
            // Note: this very much relies on an empirical approach...

            QRect iconRect = mOwner->visualRect(mValue->index());

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
            iconRect.translate(3, 1);
#else
            iconRect.translate(5, 1);
#endif

            iconRect.setSize(QSize(16, 16));

            // Check whether the given point is over our colour value icon

            if (!iconRect.contains(pPoint)) {
                return;
            }
        }

        // Select a colour and assign it to ourselves

        QColorDialog colorDialog(colorValue(), mainWindow());

        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setWindowTitle(tr("Select Colour"));

        if (colorDialog.exec() != 0) {
            setColorValue(colorDialog.currentColor());
        }

        // Make sure that the widget that owns this property gets the focus back
        // straightaway
        // Note: indeed, if we come here as a result of a double click (see
        //       PropertyEditorWidget::mouseDoubleClickEvent()), then the first
        //       time round, we will have to click anywhere in OpenCOR for
        //       OpenCOR to get the focus back...

        mOwner->processEvents();
    }
}

//==============================================================================

QString Property::unit() const
{
    // Return our unit

    return (mUnit != nullptr)?mUnit->text():QString();
}

//==============================================================================

void Property::setUnit(const QString &pUnit, bool pUpdateToolTip)
{
    // Set our unit, if it's not of section type

    if ((mUnit != nullptr) && (mType != Type::Section) && (pUnit != mUnit->text())) {
        mUnit->setText(pUnit);

        if (pUpdateToolTip) {
            updateToolTip();
        }
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

    if (pExtraInfo != mExtraInfo) {
        mExtraInfo = pExtraInfo;

        updateToolTip();
    }
}

//==============================================================================

bool Property::isVisible() const
{
    // Return our visibility

    return mOwner->isRowHidden(mName->row(),
                               (mName->parent() != nullptr)?
                                   mName->parent()->index():
                                   qobject_cast<QStandardItemModel *>(mOwner->model())->invisibleRootItem()->index());
}

//==============================================================================

void Property::setVisible(bool pVisible)
{
    // Set our visibility

    mOwner->setRowHidden(mName->row(),
                         (mName->parent() != nullptr)?
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
    // of our horizontal scroll bar

    int horizontalScrollBarValue = mOwner->horizontalScrollBar()->value();

    mOwner->setCurrentIndex(mValue->index());
    mOwner->horizontalScrollBar()->setValue(horizontalScrollBarValue);
}

//==============================================================================

void Property::edit() const
{
    // Have our owner edit our value

    if (mValue->isEditable()) {
        mOwner->edit(mValue->index());
    }
}

//==============================================================================

void Property::updateToolTip()
{
    // Update our tool tip based on the values of our name, value and unit

    QString toolTip = mName->text();

    if (mType != Type::Section) {
        toolTip += tr(": ");

        if (value().isEmpty() && (mType != Type::String)) {
            toolTip += UnknownValue;
        } else {
            toolTip += value();
        }

        if ((mUnit != nullptr) && !mUnit->text().isEmpty()) {
            toolTip += " "+mUnit->text();
        }
    }

    if (!mExtraInfo.isEmpty()) {
        toolTip += " ("+mExtraInfo+")";
    }

    mName->setToolTip(toolTip);

    if (mType != Type::Section) {
        mValue->setToolTip(toolTip);

        if (mUnit != nullptr) {
            mUnit->setToolTip(toolTip);
        }
    }
}

//==============================================================================

PropertyEditorWidget::PropertyEditorWidget(bool pShowUnits,
                                           bool pAutoUpdateHeight,
                                           QWidget *pParent) :
    TreeViewWidget(pParent),
    mShowUnits(pShowUnits),
    mAutoUpdateHeight(pAutoUpdateHeight)
{
    // Customise ourselves

    mModel = new QStandardItemModel(this);

    setModel(mModel);
    setRootIsDecorated(false);

    // Create our item delegate and set it, after making sure that we handle a
    // few of its signals

    auto propertyItemDelegate = new PropertyItemDelegate(this);

    connect(selectionModel(), &QItemSelectionModel::currentChanged,
            this, &PropertyEditorWidget::editorClosed);

    connect(mModel, &QStandardItemModel::itemChanged,
            this, &PropertyEditorWidget::checkCheckState);

    connect(propertyItemDelegate, &PropertyItemDelegate::openEditor,
            this, &PropertyEditorWidget::editorOpened);
    connect(propertyItemDelegate, &PropertyItemDelegate::closeEditor,
            this, &PropertyEditorWidget::editorClosed);

    connect(propertyItemDelegate, &PropertyItemDelegate::goToPreviousPropertyRequested,
            this, &PropertyEditorWidget::goToPreviousProperty);
    connect(propertyItemDelegate, &PropertyItemDelegate::goToNextPropertyRequested,
            this, &PropertyEditorWidget::goToNextProperty);

    setItemDelegate(propertyItemDelegate);

    // Resize our height in case some data has changed or one of the properties
    // gets expanded/collapsed

    connect(mModel, &QStandardItemModel::itemChanged,
            this, &PropertyEditorWidget::updateHeight);

    connect(this, &PropertyEditorWidget::expanded,
            this, &PropertyEditorWidget::updateHeight);
    connect(this, &PropertyEditorWidget::collapsed,
            this, &PropertyEditorWidget::updateHeight);

    header()->setSectionsMovable(false);

    // Retranslate ourselves, so that we are properly initialised

    retranslateUi();

    // Initialise our height

    updateHeight();
}

//==============================================================================

PropertyEditorWidget::PropertyEditorWidget(bool pAutoUpdateHeight,
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

    if (   (currentProperty != nullptr)
        && (currentProperty->type() == Property::Type::List)
        && currentProperty->listValues().isEmpty()) {
        currentProperty->setValue(currentProperty->emptyListValue());
    }

    // Retranslate the current item's children, if any

    for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i) {
        retranslateEmptyListProperties(pItem->child(i));
    }
}

//==============================================================================

void PropertyEditorWidget::retranslateUi()
{
    // Retranslate our header labels

    QStringList horizontalHeaderLabels = QStringList() << tr("Property")
                                                       << tr("Value");

    if (mShowUnits) {
        horizontalHeaderLabels << tr("Unit");
    }

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

            if (childIndexHeight != 0) {
                res += childIndexHeight+childrenRowHeight(childItem);
            }
        }
    }

    return res;
}

//==============================================================================

QSize PropertyEditorWidget::sizeHint() const
{
    // Return our default/ideal size, depending on the case

    if (mAutoUpdateHeight) {
        // We automatically resize our height, so determine our ideal size which
        // is based on the width of our different columns, and the height of our
        // header and our different rows

        int hintWidth = 0;
        int hintHeight = header()->height();

        for (int i = 0, iMax = header()->count(); i < iMax; ++i) {
            hintWidth += columnWidth(i);
        }

        for (int i = 0, iMax = mModel->rowCount(); i < iMax; ++i) {
            QStandardItem *rowItem = mModel->item(i);
            int rowItemHeight = rowHeight(rowItem->index());

            if (rowItemHeight != 0) {
                // Our current row has some height, meaning that it is visible,
                // so we can set its height to our hintHeight, as well as
                // retrieve the total height of our row's children

                hintHeight += rowItemHeight+childrenRowHeight(rowItem);
            }
        }

        return { hintWidth, hintHeight };
    }

    // We don't automatically resize our height, so our ideal size is our
    // maximum size

    return maximumSize();
}

//==============================================================================

void PropertyEditorWidget::clear()
{
    // Clear our data model

    mModel->clear();

    // Delete some internal objects

    for (auto property : mAllProperties) {
        delete property;
    }

    mProperties.clear();
    mAllProperties.clear();
}

//==============================================================================

void PropertyEditorWidget::selectFirstProperty()
{
    // Convenience function to select the first property, i.e. the first item

    selectFirstItem();
}

//==============================================================================

Property * PropertyEditorWidget::addProperty(Property::Type pType,
                                             Property *pParent)
{
    // Create our property

    auto res = new Property(pType, this);

    // Populate our data model with our property

    if (pParent != nullptr) {
        // We want to add a child property

        pParent->add(res);

        // If we want to see the child property, we need root decoration

        setRootIsDecorated(true);
    } else {
        // We want to add a root property

        res->addTo(mModel->invisibleRootItem());

        // Keep track of our root property

        mProperties << res;
    }

    // Span ourselves if we are of section type

    if (pType == Property::Type::Section) {
        setFirstColumnSpanned(res->row(),
                              (pParent != nullptr)?pParent->index():mModel->invisibleRootItem()->index(),
                              true);
    }

    // Keep track of our property's change of visibility

    connect(res, &Property::visibilityChanged,
            this, &PropertyEditorWidget::updateHeight);

    // Keep track of our property's change of value

    connect(res, &Property::valueChanged,
            this, &PropertyEditorWidget::emitPropertyChanged);

    // Keep track of our property's check state

    mPropertyChecked.insert(res, res->isChecked());

    // Keep track of our property and return it

    mAllProperties << res;

    return res;
}

//==============================================================================

Property * PropertyEditorWidget::addSectionProperty(const QString &pName,
                                                    Property *pParent)
{
    // Add a section property and return its information

    Property *res = addProperty(Property::Type::Section, pParent);

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

Property * PropertyEditorWidget::addStringProperty(const QString &pString,
                                                   Property *pParent)
{
    // Add a string property and return its information

    Property *res = addProperty(Property::Type::String, pParent);

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

Property * PropertyEditorWidget::addIntegerProperty(int pValue,
                                                    Property *pParent)
{
    // Add an integer property and return its information

    Property *res = addProperty(Property::Type::Integer, pParent);

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

Property * PropertyEditorWidget::addIntegerGe0Property(int pValue,
                                                       Property *pParent)
{
    // Add a zero or strictly positive integer property and return its
    // information

    Property *res = addProperty(Property::Type::IntegerGe0, pParent);

    res->setIntegerValue(pValue);

    return res;
}

//==============================================================================

Property * PropertyEditorWidget::addIntegerGe0Property(Property *pParent)
{
    // Add a zero or strictly positive integer property and return its
    // information

    return addIntegerGe0Property(1, pParent);
}

//==============================================================================

Property * PropertyEditorWidget::addIntegerGt0Property(int pValue,
                                                       Property *pParent)
{
    // Add a strictly positive integer property and return its information

    Property *res = addProperty(Property::Type::IntegerGt0, pParent);

    res->setIntegerValue(pValue);

    return res;
}

//==============================================================================

Property * PropertyEditorWidget::addIntegerGt0Property(Property *pParent)
{
    // Add a strictly positive integer property and return its information

    return addIntegerGt0Property(1, pParent);
}

//==============================================================================

Property * PropertyEditorWidget::addDoubleProperty(double pValue,
                                                   Property *pParent)
{
    // Add a double property and return its information

    Property *res = addProperty(Property::Type::Double, pParent);

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

Property * PropertyEditorWidget::addDoubleGe0Property(double pValue,
                                                      Property *pParent)
{
    // Add a zero or strictly positive double property and return its
    // information

    Property *res = addProperty(Property::Type::DoubleGe0, pParent);

    res->setDoubleValue(pValue);

    return res;
}

//==============================================================================

Property * PropertyEditorWidget::addDoubleGe0Property(Property *pParent)
{
    // Add a zero or strictly positive double property and return its
    // information

    return addDoubleGe0Property(1.0, pParent);
}

//==============================================================================

Property * PropertyEditorWidget::addDoubleGt0Property(double pValue,
                                                      Property *pParent)
{
    // Add a strictly positive double property and return its information

    Property *res = addProperty(Property::Type::DoubleGt0, pParent);

    res->setDoubleValue(pValue);

    return res;
}

//==============================================================================

Property * PropertyEditorWidget::addDoubleGt0Property(Property *pParent)
{
    // Add a strictly positive double property and return its information

    return addDoubleGt0Property(1.0, pParent);
}

//==============================================================================

Property * PropertyEditorWidget::addListProperty(const QStringList &pValues,
                                                 const QString &pDefaultValue,
                                                 Property *pParent)
{
    // Add a list property and return its information
    // Note: a list property is necessarily editable...

    Property *res = addProperty(Property::Type::List, pParent);

    res->setListValues(pValues);
    res->setValue(pDefaultValue);

    return res;
}

//==============================================================================

Property * PropertyEditorWidget::addListProperty(const QStringList &pValues,
                                                 Property *pParent)
{
    // Add a list property and return its information

    return addListProperty(pValues, QString(), pParent);
}

//==============================================================================

Property * PropertyEditorWidget::addListProperty(Property *pParent)
{
    // Add a list property and return its information

    return addListProperty(QStringList(), QString(), pParent);
}

//==============================================================================

Property * PropertyEditorWidget::addBooleanProperty(bool pValue,
                                                    Property *pParent)
{
    // Add a boolean property and return its information

    Property *res = addProperty(Property::Type::Boolean, pParent);

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

Property * PropertyEditorWidget::addColorProperty(const QColor &pValue,
                                                  Property *pParent)
{
    // Add a colour property and return its information

    Property *res = addProperty(Property::Type::Color, pParent);

    res->setColorValue(pValue);

    return res;
}

//==============================================================================

Property * PropertyEditorWidget::addColorProperty(Property *pParent)
{
    // Add a colour property and return its information

    return addColorProperty(QColor(), pParent);
}

//==============================================================================

void PropertyEditorWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Check some key combinations

    if (   (pEvent->modifiers() == Qt::ControlModifier)
        && (pEvent->key() == Qt::Key_A)) {
        // The user wants to select everything, which we don't want to allow,
        // so just accept the event...

        pEvent->accept();
    } else if (   (   (pEvent->modifiers() == Qt::NoModifier)
                   && (pEvent->key() == Qt::Key_Return))
               || (   (pEvent->modifiers() == Qt::KeypadModifier)
                   && (pEvent->key() == Qt::Key_Enter))) {
        // The user wants to start/stop editing the property

        if (mPropertyEditor != nullptr) {
            editProperty(nullptr);
        } else {
            editProperty(currentProperty());
        }

        pEvent->accept();
    } else if (   isEditing()
               && (pEvent->modifiers() == Qt::NoModifier)
               && (pEvent->key() == Qt::Key_Escape)) {
        // The user is editing and wants to cancel it

        finishEditing(false);

        pEvent->accept();
    } else if (   (pEvent->modifiers() == Qt::KeypadModifier)
               && (pEvent->key() == Qt::Key_Up)) {
        // The user wants to go the previous property

        goToPreviousProperty();

        pEvent->accept();
    } else if (   (pEvent->modifiers() == Qt::KeypadModifier)
               && (pEvent->key() == Qt::Key_Down)) {
        // The user wants to go to the next property

        goToNextProperty();

        pEvent->accept();
    } else {
        // Default handling of the event

        TreeViewWidget::keyPressEvent(pEvent);
    }
}

//==============================================================================

void PropertyEditorWidget::mouseDoubleClickEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    TreeViewWidget::mouseDoubleClickEvent(pEvent);

    // We want to select a colour when double clicking on the icon of a colour
    // value, so do just that

    Property *crtProperty = property(indexAt(pEvent->pos()));

    if ((crtProperty != nullptr) && (crtProperty->type() == Property::Type::Color)) {
        crtProperty->setColorValue(pEvent->pos());
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

    mRightClicking = pEvent->button() == Qt::RightButton;

    if (mRightClicking) {
        finishEditing(false);
    } else {
        Property *crtProperty = property(indexAt(pEvent->pos()));

        if ((crtProperty != nullptr) && (crtProperty != oldProperty)) {
            editProperty(crtProperty);
        }
    }
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
#else
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

void PropertyEditorWidget::updateFocusProxy()
{
    // Set our property's editor as our focus proxy

    setFocusProxy(mPropertyEditor);
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

    if (mProperty->type() == Property::Type::List) {
        QString propertyValue = mProperty->value();
        QStringList propertyListValues = mProperty->listValues();
        auto propertyEditor = static_cast<ListEditorWidget *>(mPropertyEditor);

        for (int i = 0, iMax = propertyListValues.count(); i < iMax; ++i) {
            if (propertyValue == propertyListValues[i]) {
                propertyEditor->setCurrentIndex(i);

                break;
            }
        }
    }

    // Next, we need to use the property's editor as our focus proxy and make
    // sure that it immediately gets the focus
    // Note #1: if we were not to immediately give the focus to our editor, then
    //          the central widget would give the focus to the previously
    //          focused widget (see CentralWidget::updateGui()), which is
    //          clearly not what we want...
    // Note #2: starting with Qt 5.12.5, we can't immediately set the property's
    //          editor as our focus proxy (!?). This will indeed crash OpenCOR!
    //          So, instead, we set it through a single shot...

    pEditor->setFocus();

    QTimer::singleShot(0, this, &PropertyEditorWidget::updateFocusProxy);
}

//==============================================================================

void PropertyEditorWidget::editorClosed()
{
    // Make sure that we are editing a property

    if (mPropertyEditor == nullptr) {
        return;
    }

    // We have stopped editing a property, so make sure that its value is
    // updated
    // Note #1: we don't need to do this for a list property or a boolean
    //          property since such a property will have already been updated
    //          (see listPropertyChanged() and booleanPropertyChanged())...
    // Note #2: we should always set (and force) the value of the property since
    //          only the text of the property item will have been updated
    //          (through QTreeView) while Property::setValue() will do a few
    //          more things (e.g. update the tool tip)...

    if (   (mProperty->type() != Property::Type::List)
        && (mProperty->type() != Property::Type::Boolean)) {
        mProperty->setValue(mProperty->value(), true);
    }

    // Reset our focus proxy and make sure that we get the focus (see
    // editorOpened() above for the reason)

    setFocusProxy(nullptr);
    setFocus();

    // Reset some information about the property

    mProperty = nullptr;
    mPropertyEditor = nullptr;
}

//==============================================================================

void PropertyEditorWidget::selectProperty(Property *pProperty)
{
    // Select the property, if one is provided

    if (pProperty == nullptr) {
        return;
    }

    pProperty->select();
}

//==============================================================================

void PropertyEditorWidget::editProperty(Property *pProperty, bool pCommitData)
{
    // We want to edit a new property, so first stop the editing of the current
    // one, if any

    if (mPropertyEditor != nullptr) {
        // A property is currently being edited, so commit its data and then
        // close its corresponding editor

        bool canCommitData = pCommitData;

        if (canCommitData) {
            // Make sure that the value of some of our properties is valid
            // Note: indeed, in the case of a DoubleGt0 property, we allow
            //       "0.3", but the user might enter "0." and then decide to
            //       move onto the next property, in which case we should ignore
            //       the 'new' value...

            Property::Type propertyType = mProperty->type();

            if (   (propertyType == Property::Type::Integer)
                || (propertyType == Property::Type::IntegerGe0)
                || (propertyType == Property::Type::IntegerGt0)
                || (propertyType == Property::Type::Double)
                || (propertyType == Property::Type::DoubleGe0)
                || (propertyType == Property::Type::DoubleGt0)
                || (propertyType == Property::Type::Color)) {
                auto propertyEditor = static_cast<TextEditorWidget *>(mPropertyEditor);
                QString propertyValue = propertyEditor->text();
                int dummy;

                canCommitData = propertyEditor->validator()->validate(propertyValue, dummy) == QValidator::Acceptable;
            }

            if (canCommitData) {
                commitData(mPropertyEditor);
            }
        }

        closeEditor(mPropertyEditor,
                    canCommitData?
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

    if (pProperty != nullptr) {
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

    if (!mAllProperties.contains(pProperty)) {
        return false;
    }

    // Stop tracking the property, if it is a root one

    if (pProperty->parent() == nullptr) {
        mProperties.removeOne(pProperty);
    }

    // Remove the property from our model
    // Note: the below will remove the given property and any of its children

    mModel->removeRow(pProperty->row(),
                      (pProperty->parent() != nullptr)?
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

    for (auto property : pProperty->properties()) {
        deleteProperty(property);
    }

    mAllProperties.removeOne(pProperty);

    delete pProperty;
}

//==============================================================================

Properties PropertyEditorWidget::properties() const
{
    // Return our properties

    return mProperties;
}

//==============================================================================

Properties PropertyEditorWidget::allProperties() const
{
    // Return all of our properties

    return mAllProperties;
}

//==============================================================================

void PropertyEditorWidget::finishEditing(bool pCommitData)
{
    // The user wants to finish the editing

    editProperty(nullptr, pCommitData);
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

void PropertyEditorWidget::goToNeighbouringProperty(int pShift)
{
    // Determine the index of the current index's neighbour

    QModelIndex neighbouringIndex = QModelIndex();

    if (pShift == 1) {
        neighbouringIndex = indexBelow(currentIndex());
    } else if (pShift == -1) {
        neighbouringIndex = indexAbove(currentIndex());
    }

    // Edit the neighbouring property, if any

    if (neighbouringIndex.isValid()) {
        editProperty(property(neighbouringIndex));
    }
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

    if (!pIndex.isValid()) {
        return nullptr;
    }

    // Return our information about the property at the given index

    for (auto property : mAllProperties) {
        if (property->hasIndex(pIndex)) {
            return property;
        }
    }

    return nullptr;
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

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
