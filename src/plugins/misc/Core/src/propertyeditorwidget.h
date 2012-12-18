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

class NumberEditorWidget : public QLineEdit
{
    Q_OBJECT

public:
    explicit NumberEditorWidget(QWidget *pParent = 0);

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);

Q_SIGNALS:
    void goToPreviousPropertyRequested();
    void goToNextPropertyRequested();
};

//==============================================================================

class IntegerEditorWidget : public NumberEditorWidget
{
    Q_OBJECT

public:
    explicit IntegerEditorWidget(QWidget *pParent = 0);
};

//==============================================================================

class DoubleEditorWidget : public NumberEditorWidget
{
    Q_OBJECT

public:
    explicit DoubleEditorWidget(QWidget *pParent = 0);
};

//==============================================================================

class ListEditorWidget : public QComboBox
{
    Q_OBJECT

public:
    explicit ListEditorWidget(QWidget *pParent = 0);

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);
    virtual void mouseDoubleClickEvent(QMouseEvent *pEvent);

Q_SIGNALS:
    void goToPreviousPropertyRequested();
    void goToNextPropertyRequested();
};

//==============================================================================

class PropertyItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    virtual QWidget *createEditor(QWidget *pParent,
                                  const QStyleOptionViewItem &pOption,
                                  const QModelIndex &pIndex) const;

protected:
    virtual bool eventFilter(QObject *pObject, QEvent *pEvent);

Q_SIGNALS:
    void openEditor(QWidget *pEditor) const;

    void goToPreviousPropertyRequested();
    void goToNextPropertyRequested();

    void listPropertyChanged(const QString &pValue);
};

//==============================================================================

class CORE_EXPORT PropertyItem : public QStandardItem
{
public:
    enum Type {
        Category = QStandardItem::UserType,
        Integer  = QStandardItem::UserType+1,
        Double   = QStandardItem::UserType+2,
        List     = QStandardItem::UserType+3
    };

    explicit PropertyItem(const Type &pType);

    virtual int type() const;

    QStringList list() const;
    void setList(const QStringList &pList);

    QString emptyListValue() const;
    void setEmptyListValue(const QString &pEmptyListValue);

private:
    Type mType;
    QStringList mList;
    QString mEmptyListValue;
};

//==============================================================================

typedef QList<PropertyItem *> PropertyItems;

//==============================================================================

struct CORE_EXPORT Property
{
    QStandardItem *name;
    PropertyItem *value;
    QStandardItem *unit;

    explicit Property();
    explicit Property(const PropertyItem::Type &pType);
    explicit Property(QStandardItem *pName, PropertyItem *pValue,
                      QStandardItem *pUnit);

    bool operator!=(const Property &pOther) const;
    bool operator==(const Property &pOther) const;

    bool isEmpty() const;

    QList<QStandardItem *> items() const;

private:
    QStandardItem * nonEditableItem() const;
};

//==============================================================================

typedef QList<Property> Properties;

//==============================================================================

struct PropertyEditorWidgetGuiStateProperty
{
    PropertyItem *item;

    bool isHidden;
    QString value;

    explicit PropertyEditorWidgetGuiStateProperty(PropertyItem *pItem,
                                                  const bool &pIsHidden,
                                                  const QString &pValue);
};

//==============================================================================

typedef QList<PropertyEditorWidgetGuiStateProperty> PropertyEditorWidgetGuiStateProperties;

//==============================================================================

struct CORE_EXPORT PropertyEditorWidgetGuiState
{
    PropertyEditorWidgetGuiStateProperties properties;

    QModelIndex currentProperty;

    explicit PropertyEditorWidgetGuiState();
};

//==============================================================================

class CORE_EXPORT PropertyEditorWidget : public TreeViewWidget
{
    Q_OBJECT

public:
    explicit PropertyEditorWidget(const bool &pShowUnits,
                                  const bool &pAutoUpdateHeight,
                                  QWidget *pParent = 0);
    explicit PropertyEditorWidget(const bool &pAutoUpdateHeight,
                                  QWidget *pParent = 0);
    explicit PropertyEditorWidget(QWidget *pParent = 0);

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual QSize sizeHint() const;

    void selectFirstProperty();

    PropertyEditorWidgetGuiState guiState();
    void setGuiState(const PropertyEditorWidgetGuiState &pGuiState);

    Property addCategoryProperty(const Property &pParent = Property());
    Property addIntegerProperty(const Property &pParent = Property());
    Property addDoubleProperty(const Property &pParent = Property());
    Property addListProperty(const Property &pParent = Property());

    void setNonEditablePropertyItem(QStandardItem *pPropertyItem,
                                    const QString &pValue);

    int integerPropertyItem(PropertyItem *pPropertyItem) const;
    void setIntegerPropertyItem(PropertyItem *pPropertyItem,
                                const int &pValue);

    double doublePropertyItem(PropertyItem *pPropertyItem) const;
    void setDoublePropertyItem(PropertyItem *pPropertyItem,
                               const double &pValue);

    void cancelPropertyEditing();

    void removeAllProperties();

    void setPropertyVisible(const Property &pProperty, const bool &pVisible);

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);

    virtual void mousePressEvent(QMouseEvent *pEvent);
    virtual void mouseMoveEvent(QMouseEvent *pEvent);

    virtual void resizeEvent(QResizeEvent *pEvent);

    Property property(const QModelIndex &pIndex) const;
    Property currentProperty() const;

private:
    bool mShowUnits;
    bool mAutoUpdateHeight;

    QStandardItemModel *mModel;

    PropertyItemDelegate *mPropertyItemDelegate;

    Property mProperty;
    QWidget *mPropertyEditor;

    PropertyItems mPropertyValues;

    void constructor(const bool &pShowUnits = true,
                     const bool &pAutoUpdateHeight = false);

    void retranslateEmptyListProperties(QStandardItem *pItem);

    Property addProperty(const Property &pParent,
                         const PropertyItem::Type &pType);

    void editProperty(const Property &pProperty,
                      const bool &pCommitData = true);

    void goToNeighbouringProperty(const int &pShift);

    int childrenRowHeight(const QStandardItem *pItem) const;

Q_SIGNALS:
    void listPropertyChanged(const QString &pValue) const;

private Q_SLOTS:
    void updateHeight();

    void editorOpened(QWidget *pEditor);
    void editorClosed();

    void goToPreviousProperty();
    void goToNextProperty();
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
