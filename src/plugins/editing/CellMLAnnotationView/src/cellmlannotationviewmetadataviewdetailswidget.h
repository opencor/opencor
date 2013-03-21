//==============================================================================
// CellML annotation view metadata view details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWMETADATAVIEWDETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWMETADATAVIEWDETAILSWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "commonwidget.h"

//==============================================================================

#include <QStackedWidget>

//==============================================================================

class QVBoxLayout;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewMetadataNormalViewDetailsWidget;
class CellmlAnnotationViewMetadataRawViewDetailsWidget;
class CellmlAnnotationViewWidget;

//==============================================================================

class CellmlAnnotationViewMetadataViewDetailsWidget : public QStackedWidget,
                                                      public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewMetadataViewDetailsWidget(CellmlAnnotationViewWidget *pParent);

    virtual void retranslateUi();

    void updateGui(iface::cellml_api::CellMLElement *pElement);

    void addRdfTriple(CellMLSupport::CellmlFileRdfTriple *pRdfTriple);

    CellmlAnnotationViewMetadataRawViewDetailsWidget *rawView() const;
    CellmlAnnotationViewMetadataNormalViewDetailsWidget *normalView() const;

private:
    CellmlAnnotationViewMetadataRawViewDetailsWidget *mRawView;
    CellmlAnnotationViewMetadataNormalViewDetailsWidget *mNormalView;
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
