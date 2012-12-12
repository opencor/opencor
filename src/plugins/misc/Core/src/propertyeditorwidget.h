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
        String = QStandardItem::UserType,
        Double = QStandardItem::UserType+1,
        List   = QStandardItem::UserType+2
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
    PropertyItem *name;
    PropertyItem *value;
    PropertyItem *unit;

    explicit Property(PropertyItem *pName = 0, PropertyItem *pValue = 0,
                      PropertyItem *pUnit = 0);

    bool operator!=(const Property &pOther) const;

    bool isEmpty() const;
};

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

    Property addDoubleProperty();
    Property addListProperty();

    void setStringProperty(PropertyItem *pPropertyItem, const QString &pValue);

    double doubleProperty(PropertyItem *pPropertyItem) const;
    void setDoubleProperty(PropertyItem *pPropertyItem, const double &pValue);

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

    Property addProperty(const PropertyItem::Type &pType);

    void editProperty(const Property &pProperty,
                      const bool &pCommitData = true);

    void goToNeighbouringProperty(const int &pShift);

    Property property(const int &pRow) const;
    Property currentProperty() const;

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
