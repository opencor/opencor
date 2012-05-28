//==============================================================================
// CellML annotation view metadata BioModels.Net view details widget
//==============================================================================

#include "cellmlannotationviewmetadatabiomodelsdotnetviewdetailswidget.h"
#include "cellmlannotationviewwidget.h"
#include "treeview.h"

//==============================================================================

#include "ui_cellmlannotationviewmetadatabiomodelsdotnetviewdetailswidget.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget(CellmlAnnotationViewWidget *pParent) :
    Widget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    mTreeView  = new Core::TreeView(this);
    mDataModel = new QStandardItemModel(mTreeView);

    mTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mTreeView->setFrameShape(QFrame::NoFrame);
    mTreeView->setModel(mDataModel);
    mTreeView->setRootIsDecorated(false);
    mTreeView->setSelectionMode(QAbstractItemView::SingleSelection);

    mGui->layout->addWidget(mTreeView);
}

//==============================================================================

CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::~CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);
}

//==============================================================================

void CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples)
{
    // Hide ourselves (to avoid any flickering during the updaate)

    setVisible(false);

    // Update the header labels
    // Note: we do it here (rather than in the constructor) in case the user
    //       decides to change languages...

    mDataModel->setHorizontalHeaderLabels(QStringList() << tr("Qualifier")
                                                        << tr("Resource")
                                                        << tr("Id"));

    // Remove all previous RDF triples from our tree view

    while (mDataModel->rowCount())
        foreach (QStandardItem *item, mDataModel->takeRow(0))
            delete item;

    // Add the 'new' RDF triples to our tree view
    // Note: for the RDF triple's subject, we try to remove the CellML file's
    //       URI base, thus only leaving the equivalent of a CellML element
    //       cmeta:id which will speak more to the user than a possibly long URI
    //       reference...

    foreach (CellMLSupport::CellmlFileRdfTriple *rdfTriple, pRdfTriples)
        mDataModel->invisibleRootItem()->appendRow(QList<QStandardItem *>() << new QStandardItem((rdfTriple->modelQualifierType() != CellMLSupport::CellmlFileRdfTriple::ModelUnknown)?
                                                                                                     rdfTriple->modelQualifierTypeAsString():
                                                                                                     rdfTriple->bioQualifierTypeAsString())
                                                                            << new QStandardItem(rdfTriple->resource())
                                                                            << new QStandardItem(rdfTriple->id()));

    // Make sure that all the columns have their contents fit

    mTreeView->resizeColumnToContents(0);
    mTreeView->resizeColumnToContents(1);
    mTreeView->resizeColumnToContents(2);

    // Re-show ourselves

    setVisible(true);
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
