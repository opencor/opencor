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

namespace Ui {
    class CellmlAnnotationViewMetadataViewDetailsWidget;
}

//==============================================================================

class QVBoxLayout;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget;
class CellmlAnnotationViewMetadataRawViewDetailsWidget;
class CellmlAnnotationViewWidget;

//==============================================================================

class CellmlAnnotationViewMetadataViewDetailsWidget : public QStackedWidget,
                                                      public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewMetadataViewDetailsWidget(CellmlAnnotationViewWidget *pParent,
                                                           CellMLSupport::CellmlFile *pCellmlFile,
                                                           const bool &pEditingMode);
    ~CellmlAnnotationViewMetadataViewDetailsWidget();

    virtual void retranslateUi();

    void updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples = CellMLSupport::CellmlFileRdfTriples());

    CellmlAnnotationViewMetadataRawViewDetailsWidget *rawView() const;
    CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget *bioModelsDotNetView() const;

private:
    CellmlAnnotationViewWidget *mParent;
    CellMLSupport::CellmlFile *mCellmlFile;

    Ui::CellmlAnnotationViewMetadataViewDetailsWidget *mGui;

    CellmlAnnotationViewMetadataRawViewDetailsWidget *mRawView;
    CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget *mBioModelsDotNetView;
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
