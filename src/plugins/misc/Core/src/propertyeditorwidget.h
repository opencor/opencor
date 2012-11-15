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

class LineEditWidget : public QLineEdit
{
    Q_OBJECT

public:
    explicit LineEditWidget(QWidget *pParent = 0);

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);

Q_SIGNALS:
    void goToPreviousPropertyRequested();
    void goToNextPropertyRequested();
};

//==============================================================================

class DoubleEditWidget : public LineEditWidget
{
    Q_OBJECT

public:
    explicit DoubleEditWidget(QWidget *pParent = 0);
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

class PropertyItem : public QStandardItem
{
public:
    enum Type
    {
        String = QStandardItem::UserType,
        Double = QStandardItem::UserType+1
    };

    explicit PropertyItem(const Type &pType, const bool &pEditable = true);

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

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void setModel(QAbstractItemModel *pModel);

    static PropertyItem * newString(const bool &pEditable = true);
    static PropertyItem * newDouble(const bool &pEditable = true);

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
