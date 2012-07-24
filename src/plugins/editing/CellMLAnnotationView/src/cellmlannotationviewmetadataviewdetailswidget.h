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

class CellmlAnnotationViewMetadataNormalViewDetailsWidget;
class CellmlAnnotationViewMetadataRawViewDetailsWidget;
class CellmlAnnotationViewWidget;

//==============================================================================

class CellmlAnnotationViewMetadataViewDetailsWidget : public QStackedWidget,
                                                      public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewMetadataViewDetailsWidget(CellmlAnnotationViewWidget *pParent,
                                                           const bool &pEditingMode);
    ~CellmlAnnotationViewMetadataViewDetailsWidget();

    virtual void retranslateUi();

    void updateGui(CellMLSupport::CellmlFileElement *pCellmlElement);

    CellmlAnnotationViewMetadataRawViewDetailsWidget *rawView() const;
    CellmlAnnotationViewMetadataNormalViewDetailsWidget *normalView() const;

private:
    Ui::CellmlAnnotationViewMetadataViewDetailsWidget *mGui;

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
