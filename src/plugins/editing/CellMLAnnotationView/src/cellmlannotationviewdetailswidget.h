//==============================================================================
// CellML annotation view details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWDETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWDETAILSWIDGET_H

//==============================================================================

#include "cellmlannotationviewcellmlelementdetailswidget.h"
#include "commonwidget.h"

//==============================================================================

#include <QStackedWidget>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewDetailsWidget;
}

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewCellmlDetailsWidget;
class CellmlAnnotationViewMetadataDetailsWidget;
class CellmlAnnotationViewWidget;

//==============================================================================

class CellmlAnnotationViewDetailsWidget : public QStackedWidget,
                                          public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewDetailsWidget(CellmlAnnotationViewWidget *pParent);
    ~CellmlAnnotationViewDetailsWidget();

    virtual void retranslateUi();

    void updateGui(const CellmlAnnotationViewCellmlElementDetailsWidget::Items &pItems);
    void updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples);

    CellmlAnnotationViewCellmlDetailsWidget * cellmlDetails() const;

private:
    CellmlAnnotationViewWidget *mParent;

    Ui::CellmlAnnotationViewDetailsWidget *mGui;

    CellmlAnnotationViewCellmlDetailsWidget *mCellmlDetails;
    CellmlAnnotationViewMetadataDetailsWidget *mMetadataDetails;

    void updateGui(const CellmlAnnotationViewCellmlElementDetailsWidget::Items &pItems,
                   const CellMLSupport::CellmlFileRdfTriples &pRdfTriples);
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
