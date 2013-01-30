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
    virtual void mouseDoubleClickEvent(QMouseEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);

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
    virtual void paint(QPainter *pPainter, const QStyleOptionViewItem &pOption,
                       const QModelIndex &pIndex) const;

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
        Section  = QStandardItem::UserType,
        String   = QStandardItem::UserType+1,
        Integer  = QStandardItem::UserType+2,
        Double   = QStandardItem::UserType+3,
        List     = QStandardItem::UserType+4
    };

    explicit PropertyItem(const Type &pType, const bool &pEditable);

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

class CORE_EXPORT Property
{
public:
    explicit Property(const PropertyItem::Type &pType, const bool &pEditable);
    explicit Property(QStandardItem *pName, PropertyItem *pValue,
                      QStandardItem *pUnit);

    QStandardItem * name() const;
    PropertyItem * value() const;
    QStandardItem * unit() const;

    QList<QStandardItem *> items() const;

private:
    QStandardItem *mName;
    PropertyItem *mValue;
    QStandardItem *mUnit;
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
                                  QWidget *pParent = 0);
    explicit PropertyEditorWidget(const bool &pAutoUpdateHeight,
                                  QWidget *pParent = 0);
    explicit PropertyEditorWidget(QWidget *pParent = 0);
    ~PropertyEditorWidget();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual QSize sizeHint() const;

    void selectFirstProperty();

    PropertyEditorWidgetGuiState * guiState();
    void setGuiState(PropertyEditorWidgetGuiState *pGuiState);

    Property * addSectionProperty(Property *pParent = 0);
    Property * addIntegerProperty(const bool &pEditable,
                                  Property *pParent = 0);
    Property * addDoubleProperty(const bool &pEditable,
                                 Property *pParent = 0);
    Property * addListProperty(Property *pParent = 0);

    void setStringPropertyItem(QStandardItem *pPropertyItem,
                               const QString &pValue);

    int integerPropertyItem(PropertyItem *pPropertyItem) const;
    void setIntegerPropertyItem(PropertyItem *pPropertyItem,
                                const int &pValue);

    double doublePropertyItem(PropertyItem *pPropertyItem) const;
    void setDoublePropertyItem(PropertyItem *pPropertyItem,
                               const double &pValue);

    Properties properties() const;

    void cancelPropertyEditing();

    void removeAllProperties();

    void setPropertyVisible(Property *pProperty, const bool &pVisible);

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);
    virtual void mouseMoveEvent(QMouseEvent *pEvent);
    virtual void resizeEvent(QResizeEvent *pEvent);

    Property * property(const QModelIndex &pIndex) const;
    Property * currentProperty() const;

private:
    bool mShowUnits;
    bool mAutoUpdateHeight;

    QStandardItemModel *mModel;

    Properties mProperties;

    Property *mProperty;
    QWidget *mPropertyEditor;

    void constructor(const bool &pShowUnits = true,
                     const bool &pAutoUpdateHeight = false);

    void retranslateEmptyListProperties(QStandardItem *pItem);

    Property * addProperty(const PropertyItem::Type &pType,
                           const bool &pEditable, Property *pParent);

    void editProperty(Property *pProperty, const bool &pCommitData = true);

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
