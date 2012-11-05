//==============================================================================
// Property editor widget
//==============================================================================

#ifndef PROPERTYEDITORWIDGET_H
#define PROPERTYEDITORWIDGET_H

//==============================================================================

#include "coreglobal.h"
#include "treeviewwidget.h"

//==============================================================================

#include <QStandardItem>
#include <QStyledItemDelegate>

//==============================================================================

class QStandardItemModel;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class PropertyItemDelegate : public QStyledItemDelegate
{
public:
    explicit PropertyItemDelegate();

    void setModel(QStandardItemModel *pModel);

private:
    QStandardItemModel *mModel;
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

    void initialize(QStandardItemModel *pModel);

    static PropertyItem * newNonEditableString(const QString &pValue = QString());
    static PropertyItem * newEditableDouble(const double &pValue);

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);

private:
    PropertyItemDelegate *mPropertyItemDelegate;

private Q_SLOTS:
    void editProperty(const QModelIndex &pNewItem,
                      const QModelIndex &pOldItem);
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
