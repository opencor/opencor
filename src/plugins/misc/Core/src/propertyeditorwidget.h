//==============================================================================
// Property editor widget
//==============================================================================

#ifndef PROPERTYEDITORWIDGET_H
#define PROPERTYEDITORWIDGET_H

//==============================================================================

#include "coreglobal.h"
#include "treeviewwidget.h"

//==============================================================================

#include <QLineEdit>
#include <QStandardItem>
#include <QStyledItemDelegate>

//==============================================================================

class QStandardItemModel;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class DoubleEditWidget : public QLineEdit
{
    Q_OBJECT

public:
    explicit DoubleEditWidget(QWidget *pParent = 0);

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

private:
    QStandardItemModel *mModel;

Q_SIGNALS:
    void openEditor(QWidget *pEditor) const;

    void goToPreviousPropertyRequested();
    void goToNextPropertyRequested();
};

//==============================================================================

class PropertyItem : public QStandardItem
{
public:
    enum Type
    {
        String = QStandardItem::UserType,
        Double = QStandardItem::UserType+1
    };

    explicit PropertyItem(const Type &pType, const QString &pValue = QString(),
                          const bool &pEditable = true);

    virtual int type() const;

private:
    Type mType;
};

//==============================================================================

class CORE_EXPORT PropertyEditorWidget : public TreeViewWidget
{
    Q_OBJECT

public:
    explicit PropertyEditorWidget(QWidget *pParent = 0);

    virtual void setModel(QAbstractItemModel *pModel);

    void resizeColumnsToContents();

    static PropertyItem * newNonEditableString(const QString &pValue = QString());
    static PropertyItem * newEditableDouble(const double &pValue);

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);

    virtual void mousePressEvent(QMouseEvent *pEvent);
    virtual void mouseMoveEvent(QMouseEvent *pEvent);

private:
    PropertyItemDelegate *mPropertyItemDelegate;

    QWidget *mPropertyEditor;
    int mPropertyRow;

    void editProperty(const int &pPropertyRow, const bool &pCommitData = true);

    void cancelPropertyEditing();

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
