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

class CellMLAnnotationViewMetadataNormalViewDetailsWidget;
class CellMLAnnotationViewMetadataRawViewDetailsWidget;
class CellMLAnnotationViewWidget;

//==============================================================================

class CellMLAnnotationViewMetadataViewDetailsWidget : public QStackedWidget,
                                                      public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit CellMLAnnotationViewMetadataViewDetailsWidget(CellMLAnnotationViewWidget *pParent);

    virtual void retranslateUi();

    void updateGui(iface::cellml_api::CellMLElement *pElement);

    void addRdfTriple(CellMLSupport::CellMLFileRdfTriple *pRdfTriple);

    CellMLAnnotationViewMetadataRawViewDetailsWidget *rawView() const;
    CellMLAnnotationViewMetadataNormalViewDetailsWidget *normalView() const;

private:
    CellMLAnnotationViewMetadataRawViewDetailsWidget *mRawView;
    CellMLAnnotationViewMetadataNormalViewDetailsWidget *mNormalView;

    CellMLSupport::CellMLFile *mCellMLFile;
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
