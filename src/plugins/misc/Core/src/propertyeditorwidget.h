//==============================================================================
// Property editor widget
//==============================================================================

#ifndef PROPERTYEDITORWIDGET_H
#define PROPERTYEDITORWIDGET_H

//==============================================================================

#include "coreglobal.h"
#include "treeviewwidget.h"

//==============================================================================

class QStandardItem;
class QStandardItemModel;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT PropertyEditorWidget : public TreeViewWidget
{
    Q_OBJECT

public:
    explicit PropertyEditorWidget(QWidget *pParent = 0);

    void initialize(QStandardItemModel *pModel);

    static QStandardItem * newNonEditableItem();
    static QStandardItem * newEditableItem();

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);

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
