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
    explicit TreeViewWidget(QWidget *pParent = 0);

    void resizeColumnsToContents();

    void selectItem(const int &pRow = 0, const int &pColumn = 0);
    void selectFirstItem();

    bool isEditing() const;

protected:
    virtual QSize sizeHint() const;

    virtual void mouseDoubleClickEvent(QMouseEvent *pEvent);

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
