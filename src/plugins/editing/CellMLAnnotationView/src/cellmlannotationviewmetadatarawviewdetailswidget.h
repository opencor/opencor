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

class CellMLAnnotationViewWidget;

//==============================================================================

class CellMLAnnotationViewMetadataRawViewDetailsWidget : public Core::TreeViewWidget
{
    Q_OBJECT

public:
    explicit CellMLAnnotationViewMetadataRawViewDetailsWidget(CellMLAnnotationViewWidget *pParent);

    virtual void retranslateUi();

    void updateGui(iface::cellml_api::CellMLElement *pElement);

private:
    CellMLSupport::CellMLFile *mCellMLFile;

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
