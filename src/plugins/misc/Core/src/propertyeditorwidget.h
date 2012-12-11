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

    explicit PropertyItem(const Type &pType,
                          const QStringList &pList = QStringList());

    virtual int type() const;

    QStringList list() const;

    QString emptyListValue() const;
    void setEmptyListValue(const QString &pEmptyListValue);

private:
    Type mType;
    QStringList mList;
    QString mEmptyListValue;
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

    PropertyItem * propertyValue(const int &pIndex) const;

    int addDoubleProperty();
    int addListProperty(const QStringList &pList);

    void setPropertyName(const int &pIndex, const QString &pName);

    double doublePropertyValue(const int &pIndex) const;
    void setDoublePropertyValue(const int &pIndex, const double &pValue);

    void setPropertyUnit(const int &pIndex, const QString &pUnit);

    void cancelPropertyEditing();

    void removeAllProperties();

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
    int mPropertyIndex;

    void constructor(const bool &pAutoUpdateHeight = false);

    int addProperty(const PropertyItem::Type &pType,
                    const QStringList &pList = QStringList());
    void editProperty(const int &pIndex, const bool &pCommitData = true);

    void goToNeighbouringProperty(const int &pShift);

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
