//==============================================================================
// CellML annotation view metadata details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWMETADATADETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWMETADATADETAILSWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "widget.h"

//==============================================================================

#include <QScrollArea>
#include <QStandardItem>
#include <QStyledItemDelegate>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewMetadataDetailsWidget;
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

class CellmlAnnotationViewDetailsWidget;

//==============================================================================

class CellmlAnnotationViewMetadataDetailsWidget : public QScrollArea,
                                                  public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewMetadataDetailsWidget(CellmlAnnotationViewDetailsWidget *pParent);
    ~CellmlAnnotationViewMetadataDetailsWidget();

    virtual void retranslateUi();

    void updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples);

private:
    CellmlAnnotationViewDetailsWidget *mParent;

    Ui::CellmlAnnotationViewMetadataDetailsWidget *mGui;

    CellMLSupport::CellmlFileRdfTriples mRdfTriples;

    QWidget *mWidget;
    QVBoxLayout *mLayout;
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
