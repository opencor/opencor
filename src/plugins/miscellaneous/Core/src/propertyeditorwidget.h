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

#pragma once

//==============================================================================

#include "coreglobal.h"
#include "coreguiutils.h"
#include "treeviewwidget.h"

//==============================================================================

#include <QComboBox>
#include <QLineEdit>
#include <QStandardItem>

//==============================================================================

class QStandardItemModel;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

static const auto UnknownValue = QStringLiteral("???");

//==============================================================================

class TextEditorWidget : public QLineEdit
{
    Q_OBJECT

public:
    explicit TextEditorWidget(QWidget *pParent);

protected:
    void keyPressEvent(QKeyEvent *pEvent) override;

signals:
    void goToPreviousPropertyRequested();
    void goToNextPropertyRequested();
};

//==============================================================================

class IntegerEditorWidget : public TextEditorWidget
{
    Q_OBJECT

public:
    explicit IntegerEditorWidget(QWidget *pParent);
};

//==============================================================================

class IntegerGe0EditorWidget : public TextEditorWidget
{
    Q_OBJECT

public:
    explicit IntegerGe0EditorWidget(QWidget *pParent);
};

//==============================================================================

class IntegerGt0EditorWidget : public TextEditorWidget
{
    Q_OBJECT

public:
    explicit IntegerGt0EditorWidget(QWidget *pParent);
};

//==============================================================================

class DoubleEditorWidget : public TextEditorWidget
{
    Q_OBJECT

public:
    explicit DoubleEditorWidget(QWidget *pParent);
};

//==============================================================================

class DoubleGe0EditorWidget : public TextEditorWidget
{
    Q_OBJECT

public:
    explicit DoubleGe0EditorWidget(QWidget *pParent);
};

//==============================================================================

class DoubleGt0EditorWidget : public TextEditorWidget
{
    Q_OBJECT

public:
    explicit DoubleGt0EditorWidget(QWidget *pParent);
};

//==============================================================================

class ListEditorWidget : public QComboBox
{
    Q_OBJECT

public:
    explicit ListEditorWidget(QWidget *pParent);

protected:
    void keyPressEvent(QKeyEvent *pEvent) override;
    void mouseDoubleClickEvent(QMouseEvent *pEvent) override;
    void mousePressEvent(QMouseEvent *pEvent) override;

signals:
    void goToPreviousPropertyRequested();
    void goToNextPropertyRequested();
};

//==============================================================================

class BooleanEditorWidget : public ListEditorWidget
{
    Q_OBJECT

public:
    explicit BooleanEditorWidget(QWidget *pParent);
};

//==============================================================================

class ColorEditorWidget : public TextEditorWidget
{
    Q_OBJECT

public:
    explicit ColorEditorWidget(QWidget *pParent);
};

//==============================================================================

class PropertyEditorWidget;

//==============================================================================

class PropertyItemDelegate : public StyledItemDelegate
{
    Q_OBJECT

public:
    explicit PropertyItemDelegate(PropertyEditorWidget *pParent);

    QWidget *createEditor(QWidget *pParent,
                          const QStyleOptionViewItem &pOption,
                          const QModelIndex &pIndex) const override;

protected:
    bool eventFilter(QObject *pObject, QEvent *pEvent) override;
    void paint(QPainter *pPainter, const QStyleOptionViewItem &pOption,
               const QModelIndex &pIndex) const override;

private:
    PropertyEditorWidget *mPropertyEditorWidget;

signals:
    void openEditor(QWidget *pEditor) const;

    void goToPreviousPropertyRequested();
    void goToNextPropertyRequested();

private slots:
    void listPropertyChanged(const QString &pValue);
    void booleanPropertyChanged(const QString &pValue);
};

//==============================================================================

class Property;

//==============================================================================

class PropertyItem : public QStandardItem
{
public:
    explicit PropertyItem(Property *pOwner);
    ~PropertyItem() override;

    Property *owner() const;

private:
    Property *mOwner;
};

//==============================================================================

using Properties = QList<Property *>;

//==============================================================================

class CORE_EXPORT Property : public QObject
{
    Q_OBJECT

public:
    enum class Type {
        Section,
        String,
        Integer,
        IntegerGe0,
        IntegerGt0,
        Double,
        DoubleGe0,
        DoubleGt0,
        List,
        Boolean,
        Color
    };

    explicit Property(Type pType, PropertyEditorWidget *pParent);

    PropertyEditorWidget * owner() const;

    Type type() const;

    QStandardItem * parent() const;

    int row() const;

    Property * parentProperty() const;
    void setParentProperty(Property *pProperty);

    Properties properties() const;

    void add(Property *pProperty);
    void addTo(QStandardItem *pParent);

    bool hasIndex(const QModelIndex &pIndex) const;
    QModelIndex index() const;

    QString id() const;
    void setId(const QString &pId);

    bool isCheckable() const;
    void setCheckable(bool pCheckable);

    bool isChecked() const;
    void setChecked(bool pChecked);

    bool isEditable() const;
    void setEditable(bool pEditable);

    QIcon icon() const;
    void setIcon(const QIcon &pIcon);

    QString name() const;
    void setName(const QString &pName, bool pUpdateToolTip = true);

    QString value() const;
    void setValue(const QString &pValue, bool pForce = false,
                  bool pEmitSignal = true);

    QVariant variantValue(bool pListValueIndex = true) const;
    QString stringValue() const;

    int integerValue() const;
    void setIntegerValue(int pIntegerValue, bool pEmitSignal = true);

    double doubleValue() const;
    void setDoubleValue(double pDoubleValue, bool pEmitSignal = true);

    QStringList listValues() const;
    void setListValues(const QStringList &pListValues, bool pEmitSignal = true);

    QString listValue() const;
    void setListValue(const QString &pListValue);

    int listValueIndex() const;
    void setListValueIndex(int pListValueIndex);

    QString emptyListValue() const;
    void setEmptyListValue(const QString &pEmptyListValue);

    bool booleanValue() const;
    void setBooleanValue(bool pBooleanValue);

    QColor colorValue() const;
    void setColorValue(const QColor &pColorValue);
    void setColorValue(const QPoint &pPoint = QPoint());

    QString unit() const;
    void setUnit(const QString &pUnit, bool pUpdateToolTip = true);

    QString extraInfo() const;
    void setExtraInfo(const QString &pExtraInfo);

    bool isVisible() const;
    void setVisible(bool pVisible);

    void select() const;

    void edit() const;

    void updateToolTip();

private:
    PropertyEditorWidget *mOwner;

    Type mType;

    QString mId;

    PropertyItem *mName;
    PropertyItem *mValue;
    PropertyItem *mUnit;

    QStringList mListValues;
    QString mEmptyListValue = UnknownValue;

    QString mExtraInfo;

    Property * mParentProperty = nullptr;
    Properties mProperties;

    QList<QStandardItem *> items() const;

signals:
    void visibilityChanged(bool pVisible);
    void valueChanged(const QString &pOldValue, const QString &pNewValue);
};

//==============================================================================

class CORE_EXPORT PropertyEditorWidget : public TreeViewWidget
{
    Q_OBJECT

public:
    explicit PropertyEditorWidget(bool pShowUnits, bool pAutoUpdateHeight,
                                  QWidget *pParent);
    explicit PropertyEditorWidget(bool pAutoUpdateHeight, QWidget *pParent);
    explicit PropertyEditorWidget(QWidget *pParent);
    ~PropertyEditorWidget() override;

    void retranslateUi() override;

    void clear();

    void selectFirstProperty();

    Property * addSectionProperty(const QString &pName, Property *pParent = nullptr);
    Property * addSectionProperty(Property *pParent = nullptr);

    Property * addStringProperty(const QString &pString, Property *pParent = nullptr);
    Property * addStringProperty(Property *pParent = nullptr);

    Property * addIntegerProperty(int pValue, Property *pParent = nullptr);
    Property * addIntegerProperty(Property *pParent = nullptr);

    Property * addIntegerGe0Property(int pValue, Property *pParent = nullptr);
    Property * addIntegerGe0Property(Property *pParent = nullptr);

    Property * addIntegerGt0Property(int pValue, Property *pParent = nullptr);
    Property * addIntegerGt0Property(Property *pParent = nullptr);

    Property * addDoubleProperty(double pValue, Property *pParent = nullptr);
    Property * addDoubleProperty(Property *pParent = nullptr);

    Property * addDoubleGe0Property(double pValue, Property *pParent = nullptr);
    Property * addDoubleGe0Property(Property *pParent = nullptr);

    Property * addDoubleGt0Property(double pValue, Property *pParent = nullptr);
    Property * addDoubleGt0Property(Property *pParent = nullptr);

    Property * addListProperty(const QStringList &pValues,
                               const QString &pDefaultValue,
                               Property *pParent = nullptr);
    Property * addListProperty(const QStringList &pValues,
                               Property *pParent = nullptr);
    Property * addListProperty(Property *pParent = nullptr);

    Property * addBooleanProperty(bool pValue, Property *pParent = nullptr);
    Property * addBooleanProperty(Property *pParent = nullptr);

    Property * addColorProperty(const QColor &pValue, Property *pParent = nullptr);
    Property * addColorProperty(Property *pParent = nullptr);

    bool removeProperty(Property *pProperty);

    Properties properties() const;
    Properties allProperties() const;

    Property * property(const QModelIndex &pIndex) const;
    Property * currentProperty() const;

    bool showUnits() const;

    void finishEditing(bool pCommitData = true);

    void removeAllProperties();

protected:
    void keyPressEvent(QKeyEvent *pEvent) override;
    void mouseDoubleClickEvent(QMouseEvent *pEvent) override;
    void mouseMoveEvent(QMouseEvent *pEvent) override;
    void mousePressEvent(QMouseEvent *pEvent) override;
    void mouseReleaseEvent(QMouseEvent *pEvent) override;
    void resizeEvent(QResizeEvent *pEvent) override;

    QSize sizeHint() const override;

private:
    bool mShowUnits;
    bool mAutoUpdateHeight;

    QStandardItemModel *mModel;

    Properties mProperties;
    Properties mAllProperties;

    Property *mProperty = nullptr;
    QWidget *mPropertyEditor = nullptr;

    bool mRightClicking = false;

    QMap<Property *, bool> mPropertyChecked;

    void retranslateEmptyListProperties(QStandardItem *pItem);

    Property * addProperty(Property::Type pType, Property *pParent);

    void selectProperty(Property *pProperty);
    void editProperty(Property *pProperty, bool pCommitData = true);

    void goToNeighbouringProperty(int pShift);

    int childrenRowHeight(const QStandardItem *pItem) const;

    void deleteProperty(Property *pProperty);

signals:
    void propertyChanged(Property *pProperty);

private slots:
    void updateHeight();

    void checkCheckState(QStandardItem *pItem);

    void updateFocusProxy();

    void editorOpened(QWidget *pEditor);
    void editorClosed();

    void goToPreviousProperty();
    void goToNextProperty();

    void emitPropertyChanged();
};

//==============================================================================

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
