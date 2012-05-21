//==============================================================================
// CellML annotation view metadata view details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWMETADATAVIEWDETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWMETADATAVIEWDETAILSWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "commonwidget.h"

//==============================================================================

#include <QSplitter>
#include <QStandardItem>
#include <QStyledItemDelegate>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewMetadataViewDetailsWidget;
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

class CellmlAnnotationViewMetadataViewDetailsWidget : public QSplitter,
                                                      public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewMetadataViewDetailsWidget(CellmlAnnotationViewWidget *pParent);
    ~CellmlAnnotationViewMetadataViewDetailsWidget();

    virtual void retranslateUi();

    void updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples);

private:
    CellmlAnnotationViewWidget *mParent;

    Ui::CellmlAnnotationViewMetadataViewDetailsWidget *mGui;

    CellMLSupport::CellmlFileRdfTriples mRdfTriples;

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
