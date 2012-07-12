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

    CellmlAnnotationViewCellmlDetailsWidget * cellmlDetails() const;
    CellmlAnnotationViewMetadataDetailsWidget * metadataDetails() const;

private:
    Ui::CellmlAnnotationViewDetailsWidget *mGui;

    CellmlAnnotationViewCellmlDetailsWidget *mCellmlDetails;
    CellmlAnnotationViewMetadataDetailsWidget *mMetadataDetails;

    void updateGui(const CellmlAnnotationViewCellmlElementDetailsWidget::Items &pItems,
                   const CellMLSupport::CellmlFileRdfTriples &pRdfTriples);

public Q_SLOTS:
    void updateGui(const CellmlAnnotationViewCellmlElementDetailsWidget::Items &pItems);
    void updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples);
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
