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

class DoubleEditorWidget : public QLineEdit
{
    Q_OBJECT

public:
    explicit DoubleEditorWidget(QWidget *pParent = 0);

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);

Q_SIGNALS:
    void goToPreviousPropertyRequested();
    void goToNextPropertyRequested();
};

//==============================================================================

class ListEditorWidget : public QComboBox
{
    Q_OBJECT

public:
    explicit ListEditorWidget(QWidget *pParent = 0);

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);

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

    bool isEmpty() const;

    QList<QStandardItem *> items() const;

private:
    QStandardItem * nonEditableItem() const;
};

//==============================================================================

typedef QList<Property> Properties;

//==============================================================================

class CORE_EXPORT PropertyEditorWidget : public TreeViewWidget
{
    Q_OBJECT

public:
    explicit PropertyEditorWidget(const bool &pAutoUpdateHeight,
                                  QWidget *pParent = 0);
    explicit PropertyEditorWidget(QWidget *pParent = 0);

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual QSize sizeHint() const;

    void selectFirstProperty();

    Property addCategoryProperty(const Property &pParent = Property());
    Property addIntegerProperty(const Property &pParent = Property());
    Property addDoubleProperty(const Property &pParent = Property());
    Property addListProperty(const Property &pParent = Property());

    void setNonEditablePropertyItem(QStandardItem *pPropertyItem,
                                    const QString &pValue);

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

private:
    bool mAutoUpdateHeight;

    QStandardItemModel *mModel;

    PropertyItemDelegate *mPropertyItemDelegate;

    QWidget *mPropertyEditor;
    Property mProperty;

    void constructor(const bool &pAutoUpdateHeight = false);

    Property addProperty(const Property &pParent,
                         const PropertyItem::Type &pType);

    void editProperty(const Property &pProperty,
                      const bool &pCommitData = true);

    void goToNeighbouringProperty(const int &pShift);

    Property property(const int &pRow) const;
    Property currentProperty() const;

    int childrenRowHeight(const QStandardItem *pItem) const;

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
