//==============================================================================
// CellML annotation view details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWDETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWDETAILSWIDGET_H

//==============================================================================

#include "widget.h"

//==============================================================================

#include <QStandardItem>
#include <QStyledItemDelegate>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewDetailsWidget;
}

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewDetailsWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewDetailsWidget(QWidget *pParent);
    ~CellmlAnnotationViewDetailsWidget();

private:
    Ui::CellmlAnnotationViewDetailsWidget *mUi;
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
