//==============================================================================
// CellML annotation view CellML details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWCELLMLDETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWCELLMLDETAILSWIDGET_H

//==============================================================================

#include "cellmlannotationviewcellmlelementdetailswidget.h"
#include "commonwidget.h"

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewCellmlDetailsWidget;
}

//==============================================================================

#include <QSplitter>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewDetailsWidget;

//==============================================================================

class CellmlAnnotationViewCellmlDetailsWidget : public QSplitter,
                                                public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewCellmlDetailsWidget(CellmlAnnotationViewDetailsWidget *pParent);
    ~CellmlAnnotationViewCellmlDetailsWidget();

    virtual void retranslateUi();

    virtual QWidget * focusProxyWidget() const;

    void updateGui(const CellmlAnnotationViewCellmlElementDetailsWidget::Items &pItems);
    void finalizeGui();

private:
    CellmlAnnotationViewDetailsWidget *mParent;

    Ui::CellmlAnnotationViewCellmlDetailsWidget *mGui;

    CellmlAnnotationViewCellmlElementDetailsWidget *mCellmlElementDetails;
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
