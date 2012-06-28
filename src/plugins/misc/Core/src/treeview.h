//==============================================================================
// Enhanced tree view
//==============================================================================

#ifndef TREEVIEW_H
#define TREEVIEW_H

//==============================================================================

#include "commonwidget.h"
#include "coreglobal.h"

//==============================================================================

#include <QTreeView>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT TreeView : public QTreeView, public CommonWidget
{
    Q_OBJECT

public:
    explicit TreeView(QWidget *pParent);

    void selectFirstNode();

    bool isEditing() const;

protected:
    virtual QSize sizeHint() const;

    virtual void startDrag(Qt::DropActions pSupportedActions);
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
