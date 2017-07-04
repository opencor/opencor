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
#include "treeviewwidget.h"

//==============================================================================

#include <QComboBox>
#include <QLineEdit>
#include <QStandardItem>
#include <QStyledItemDelegate>

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
    virtual void keyPressEvent(QKeyEvent *pEvent);

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

class DoubleEditorWidget : public TextEditorWidget
{
    Q_OBJECT

public:
    explicit DoubleEditorWidget(QWidget *pParent);
};

//==============================================================================

class ListEditorWidget : public QComboBox
{
    Q_OBJECT

public:
    explicit ListEditorWidget(QWidget *pParent);

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);
    virtual void mouseDoubleClickEvent(QMouseEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);

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

class Property;
class PropertyEditorWidget;

//==============================================================================

class PropertyItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit PropertyItemDelegate(PropertyEditorWidget *pParent);

    virtual QWidget *createEditor(QWidget *pParent,
                                  const QStyleOptionViewItem &pOption,
                                  const QModelIndex &pIndex) const;

protected:
    virtual bool eventFilter(QObject *pObject, QEvent *pEvent);
    virtual void paint(QPainter *pPainter, const QStyleOptionViewItem &pOption,
                       const QModelIndex &pIndex) const;

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

class PropertyItem : public QStandardItem
{
public:
    explicit PropertyItem(Property *pOwner);

    Property *owner() const;

private:
    Property *mOwner;
};

//==============================================================================

class CORE_EXPORT Property : public QObject
{
    Q_OBJECT

public:
    enum Type {
        Section  = QStandardItem::UserType,
        String   = QStandardItem::UserType+1,
        Integer  = QStandardItem::UserType+2,
        Double   = QStandardItem::UserType+3,
        List     = QStandardItem::UserType+4,
        Boolean  = QStandardItem::UserType+5
    };

    explicit Property(const Type &pType, PropertyEditorWidget *pParent);

    Type type() const;

    QStandardItem * parent() const;

    int row() const;

    Property * parentProperty() const;
    void setParentProperty(Property *pProperty);

    QList<Property *> properties() const;

    void add(Property *pProperty);
    void addTo(QStandardItem *pParent);

    bool hasIndex(const QModelIndex &pIndex) const;
    QModelIndex index() const;

    QString id() const;
    void setId(const QString &pId);

    bool isCheckable() const;
    void setCheckable(const bool &pCheckable);

    bool isChecked() const;
    void setChecked(const bool &pChecked);

    bool isEditable() const;
    void setEditable(const bool &pEditable);

    QString name() const;
    void setName(const QString &pName, const bool &pUpdateToolTip = true);

    QIcon icon() const;
    void setIcon(const QIcon &pIcon);

    int integerValue() const;
    void setIntegerValue(const int &pIntegerValue);

    double doubleValue() const;
    void setDoubleValue(const double &pDoubleValue,
                        const bool &pEmitSignal = true);

    QString value() const;
    void setValue(const QString &pValue, const bool &pForce = false,
                  const bool &pEmitSignal = true);

    QStringList listValues() const;
    void setListValues(const QStringList &pListValues,
                       const QString &pListValue,
                       const bool &pEmitSignal = true);
    void setListValues(const QStringList &pListValues,
                       const bool &pEmitSignal = true);

    QString listValue() const;
    void setListValue(const QString &pListValue);

    QString emptyListValue() const;
    void setEmptyListValue(const QString &pEmptyListValue);

    bool booleanValue() const;
    void setBooleanValue(const bool &pBooleanValue);

    QString unit() const;
    void setUnit(const QString &pUnit, const bool &pUpdateToolTip = true);

    QString extraInfo() const;
    void setExtraInfo(const QString &pExtraInfo);

    bool isVisible() const;
    void setVisible(const bool &pVisible);

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
    QString mEmptyListValue;

    QString mExtraInfo;

    Property * mParentProperty;
    QList<Property *> mProperties;

    QList<QStandardItem *> items() const;

signals:
    void visibilityChanged(const bool &pVisible);
    void valueChanged(const QString &pOldValue, const QString &pNewValue);
};

//==============================================================================

typedef QList<Property *> Properties;

//==============================================================================

class CORE_EXPORT PropertyEditorWidget : public TreeViewWidget
{
    Q_OBJECT

public:
    explicit PropertyEditorWidget(const bool &pShowUnits,
                                  const bool &pAutoUpdateHeight,
                                  QWidget *pParent);
    explicit PropertyEditorWidget(const bool &pAutoUpdateHeight,
                                  QWidget *pParent);
    explicit PropertyEditorWidget(QWidget *pParent);
    ~PropertyEditorWidget();

    virtual void retranslateUi();

    void clear();

    void selectFirstProperty();

    Property * addSectionProperty(const QString &pName, Property *pParent = 0);
    Property * addSectionProperty(Property *pParent = 0);

    Property * addIntegerProperty(const int &pValue, Property *pParent = 0);
    Property * addIntegerProperty(Property *pParent = 0);

    Property * addDoubleProperty(const double &pValue, Property *pParent = 0);
    Property * addDoubleProperty(Property *pParent = 0);

    Property * addListProperty(const QStringList &pValues,
                               const QString &pValue, Property *pParent = 0);
    Property * addListProperty(const QStringList &pValues,
                               Property *pParent = 0);
    Property * addListProperty(Property *pParent = 0);

    Property * addBooleanProperty(const bool &pValue, Property *pParent = 0);
    Property * addBooleanProperty(Property *pParent = 0);

    Property * addStringProperty(const QString &pString, Property *pParent = 0);
    Property * addStringProperty(Property *pParent = 0);

    bool removeProperty(Property *pProperty);

    Properties properties() const;

    Property * property(const QModelIndex &pIndex) const;
    Property * currentProperty() const;

    bool showUnits() const;

    void finishEditing(const bool &pCommitData = true);

    void removeAllProperties();

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);
    virtual void mouseMoveEvent(QMouseEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);
    virtual void mouseReleaseEvent(QMouseEvent *pEvent);
    virtual void resizeEvent(QResizeEvent *pEvent);

    virtual QSize sizeHint() const;

private:
    bool mShowUnits;
    bool mAutoUpdateHeight;

    QStandardItemModel *mModel;

    Properties mProperties;

    Property *mProperty;
    QWidget *mPropertyEditor;

    bool mRightClicking;

    QMap<Property *, bool> mPropertyChecked;

    void retranslateEmptyListProperties(QStandardItem *pItem);

    Property * addProperty(const Property::Type &pType, Property *pParent);

    void selectProperty(Property *pProperty);
    void editProperty(Property *pProperty, const bool &pCommitData = true);

    void goToNeighbouringProperty(const int &pShift);

    int childrenRowHeight(const QStandardItem *pItem) const;

    void deleteProperty(Property *pProperty);

signals:
    void propertyChanged(Core::Property *pProperty);

private slots:
    void updateHeight();

    void checkCheckState(QStandardItem *pItem);

    void editorOpened(QWidget *pEditor);
    void editorClosed();

    void goToPreviousProperty();
    void goToNextProperty();

    void emitPropertyChanged();
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
