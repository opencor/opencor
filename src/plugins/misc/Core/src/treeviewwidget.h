//==============================================================================
// Enhanced tree view widget
//==============================================================================

#ifndef TREEVIEWWIDGET_H
#define TREEVIEWWIDGET_H

//==============================================================================

#include "commonwidget.h"
#include "coreglobal.h"

//==============================================================================

#include <QTreeView>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT TreeViewWidget : public QTreeView, public CommonWidget
{
    Q_OBJECT

public:
    explicit TreeViewWidget(QWidget *pParent);

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
