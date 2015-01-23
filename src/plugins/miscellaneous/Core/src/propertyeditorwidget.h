/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Property editor widget
//==============================================================================

#ifndef PROPERTYEDITORWIDGET_H
#define PROPERTYEDITORWIDGET_H

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

Q_SIGNALS:
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

Q_SIGNALS:
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

Q_SIGNALS:
    void openEditor(QWidget *pEditor) const;

    void goToPreviousPropertyRequested();
    void goToNextPropertyRequested();

    void listPropertyChanged(Core::Property *pProperty, const QString &pValue);
    void booleanPropertyChanged(Core::Property *pProperty, const QString &pValue);
    // Note: strictly speaking, we don't need to specify the Core namespace, but
    //       then if we were no to specify it, we wouldn't be able to use the
    //       property editor outside of the Core namespace...

private Q_SLOTS:
    void emitListPropertyChanged(const QString &pValue);
    void emitBooleanPropertyChanged(const QString &pValue);
};

//==============================================================================

class CORE_EXPORT PropertyItem : public QStandardItem
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
    void setName(const QString &pName);

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

    QStringList listValue() const;
    void setListValue(const QStringList &pListValue,
                      const bool &pEmitSignal = true);

    QString emptyListValue() const;
    void setEmptyListValue(const QString &pEmptyListValue);

    bool booleanValue() const;
    void setBooleanValue(const bool &pBooleanValue);

    QString unit() const;
    void setUnit(const QString &pUnit);

    QString extraInfo() const;
    void setExtraInfo(const QString &pExtraInfo);

    bool isVisible() const;
    void setVisible(const bool &pVisible);

    void select() const;

    void edit() const;

private:
    PropertyEditorWidget *mOwner;

    Type mType;

    QString mId;

    bool mHasUnit;

    PropertyItem *mName;
    PropertyItem *mValue;
    PropertyItem *mUnit;

    QStringList mListValue;
    QString mEmptyListValue;

    QString mExtraInfo;

    Property * mParentProperty;
    QList<Property *> mProperties;

    QList<QStandardItem *> items() const;

    void updateToolTip();

Q_SIGNALS:
    void visibilityChanged(const bool &pVisible);
    void valueChanged(const QString &pOldValue, const QString &pNewValue);
};

//==============================================================================

typedef QList<Property *> Properties;

//==============================================================================

class PropertyEditorWidgetGuiStateProperty
{
public:
    explicit PropertyEditorWidgetGuiStateProperty(Property *pProperty,
                                                  const bool &pHidden,
                                                  const bool &pExpanded,
                                                  const QString &pValue);

    Property * property() const;

    bool isHidden() const;
    bool isExpanded() const;
    QString value() const;

private:
    Property *mProperty;

    bool mHidden;
    bool mExpanded;
    QString mValue;
};

//==============================================================================

typedef QList<PropertyEditorWidgetGuiStateProperty *> PropertyEditorWidgetGuiStateProperties;

//==============================================================================

class CORE_EXPORT PropertyEditorWidgetGuiState
{
public:
    explicit PropertyEditorWidgetGuiState(const QModelIndex &pCurrentProperty);
    ~PropertyEditorWidgetGuiState();

    PropertyEditorWidgetGuiStateProperties properties() const;
    void addProperty(PropertyEditorWidgetGuiStateProperty *pProperty);

    QModelIndex currentProperty() const;

private:
    PropertyEditorWidgetGuiStateProperties mProperties;

    QModelIndex mCurrentProperty;
};

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

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual QSize sizeHint() const;

    void selectFirstProperty();

    PropertyEditorWidgetGuiState * guiState();
    void setGuiState(PropertyEditorWidgetGuiState *pGuiState);

    Property * addSectionProperty(const QString &pName, Property *pParent = 0);
    Property * addSectionProperty(Property *pParent = 0);

    Property * addIntegerProperty(const int &pValue, Property *pParent = 0);
    Property * addIntegerProperty(Property *pParent = 0);

    Property * addDoubleProperty(const double &pValue, Property *pParent = 0);
    Property * addDoubleProperty(Property *pParent = 0);

    Property * addListProperty(const QStringList &pStringList,
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

private:
    bool mShowUnits;
    bool mAutoUpdateHeight;

    QStandardItemModel *mModel;

    Properties mProperties;

    Property *mProperty;
    QWidget *mPropertyEditor;

    bool mRightClicking;

    QMap<Property *, bool> mPropertyChecked;

    void constructor(const bool &pShowUnits = true,
                     const bool &pAutoUpdateHeight = false);

    void retranslateEmptyListProperties(QStandardItem *pItem);

    Property * addProperty(const Property::Type &pType, Property *pParent);

    void selectProperty(Property *pProperty);
    void editProperty(Property *pProperty, const bool &pCommitData = true);

    void goToNeighbouringProperty(const int &pShift);

    int childrenRowHeight(const QStandardItem *pItem) const;

    void deleteProperty(Property *pProperty);

Q_SIGNALS:
    void propertyChanged(Core::Property *pProperty);
    void listPropertyChanged(Core::Property *pProperty, const QString &pValue);
    // Note: see the comment for the same signal in PropertyItemDelegate...

private Q_SLOTS:
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

#endif

//==============================================================================
// End of file
//==============================================================================
