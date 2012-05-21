//==============================================================================
// CellML annotation view metadata raw view details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWMETADATARAWVIEWDETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWMETADATARAWVIEWDETAILSWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "widget.h"

//==============================================================================

#include <QStandardItem>
#include <QStyledItemDelegate>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewMetadataRawViewDetailsWidget;
}

//==============================================================================

class QVBoxLayout;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class TreeView;
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

    void updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples);

private:
    CellmlAnnotationViewWidget *mParent;

    Ui::CellmlAnnotationViewMetadataRawViewDetailsWidget *mGui;

    Core::TreeView *mTreeView;
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
