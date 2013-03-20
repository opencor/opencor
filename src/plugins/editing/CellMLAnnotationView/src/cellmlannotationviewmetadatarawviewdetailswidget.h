//==============================================================================
// CellML annotation view metadata raw view details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWMETADATARAWVIEWDETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWMETADATARAWVIEWDETAILSWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "treeviewwidget.h"

//==============================================================================

class QStandardItemModel;

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewWidget;

//==============================================================================

class CellmlAnnotationViewMetadataRawViewDetailsWidget : public Core::TreeViewWidget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewMetadataRawViewDetailsWidget(CellmlAnnotationViewWidget *pParent);

    virtual void retranslateUi();

    void updateGui(iface::cellml_api::CellMLElement *pCellmlFileElement);

private:
    CellMLSupport::CellmlFile *mCellmlFile;

    QStandardItemModel *mModel;
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
