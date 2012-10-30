//==============================================================================
// CellML annotation view metadata raw view details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWMETADATARAWVIEWDETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWMETADATARAWVIEWDETAILSWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "widget.h"

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewMetadataRawViewDetailsWidget;
}

//==============================================================================

class QStandardItemModel;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class TreeViewWidget;
}   // namespace Core

//==============================================================================

namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewWidget;

//==============================================================================

class CellmlAnnotationViewMetadataRawViewDetailsWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewMetadataRawViewDetailsWidget(CellmlAnnotationViewWidget *pParent);
    ~CellmlAnnotationViewMetadataRawViewDetailsWidget();

    virtual void retranslateUi();

    void updateGui(CellMLSupport::CellmlFileElement *pCellmlElement);

private:
    CellMLSupport::CellmlFile *mCellmlFile;

    Ui::CellmlAnnotationViewMetadataRawViewDetailsWidget *mGui;

    Core::TreeViewWidget *mTreeView;
    QStandardItemModel *mDataModel;
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
