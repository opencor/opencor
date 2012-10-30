//==============================================================================
// CellML annotation view metadata raw view details widget
//==============================================================================

#include "cellmlannotationviewmetadatarawviewdetailswidget.h"
#include "cellmlannotationviewwidget.h"
#include "treeviewwidget.h"

//==============================================================================

#include "ui_cellmlannotationviewmetadatarawviewdetailswidget.h"

//==============================================================================

#include <QStandardItemModel>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataRawViewDetailsWidget::CellmlAnnotationViewMetadataRawViewDetailsWidget(CellmlAnnotationViewWidget *pParent) :
    Widget(pParent),
    mCellmlFile(pParent->cellmlFile()),
    mGui(new Ui::CellmlAnnotationViewMetadataRawViewDetailsWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    mTreeView  = new Core::TreeViewWidget(this);
    mDataModel = new QStandardItemModel(mTreeView);

    mTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mTreeView->setFrameShape(QFrame::NoFrame);
    mTreeView->setModel(mDataModel);
    mTreeView->setRootIsDecorated(false);
    mTreeView->setSelectionMode(QAbstractItemView::SingleSelection);

    mGui->layout->addWidget(mTreeView);

    // Some further initialisations which are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();
}

//==============================================================================

CellmlAnnotationViewMetadataRawViewDetailsWidget::~CellmlAnnotationViewMetadataRawViewDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewMetadataRawViewDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    // Update the header labels

    mDataModel->setHorizontalHeaderLabels(QStringList() << tr("#")
                                                        << tr("Subject")
                                                        << tr("Predicate")
                                                        << tr("Object"));
}

//==============================================================================

void CellmlAnnotationViewMetadataRawViewDetailsWidget::updateGui(CellMLSupport::CellmlFileElement *pCellmlElement)
{
    if (!pCellmlElement)
        return;

    // Prevent ourselves from being updated (to avoid any flickering)

    setUpdatesEnabled(false);

    // Remove all previous RDF triples from our tree view widget

    while (mDataModel->rowCount())
        foreach (QStandardItem *item, mDataModel->takeRow(0))
            delete item;

    // Add the 'new' RDF triples to our tree view widget
    // Note: for the RDF triple's subject, we try to remove the CellML file's
    //       URI base, thus only leaving the equivalent of a CellML element
    //       cmeta:id which will speak more to the user than a possibly long URI
    //       reference...

    int rdfTripleCounter = 0;

    foreach (CellMLSupport::CellmlFileRdfTriple *rdfTriple,
             pCellmlElement->rdfTriples())
        mDataModel->invisibleRootItem()->appendRow(QList<QStandardItem *>() << new QStandardItem(QString::number(++rdfTripleCounter))
                                                                            << new QStandardItem((rdfTriple->subject()->type() == CellMLSupport::CellmlFileRdfTripleElement::UriReference)?
                                                                                                     rdfTriple->metadataId():
                                                                                                     rdfTriple->subject()->asString())
                                                                            << new QStandardItem(rdfTriple->predicate()->asString())
                                                                            << new QStandardItem(rdfTriple->object()->asString()));

    // Make sure that all the columns have their contents fit

    mTreeView->resizeColumnToContents(0);
    mTreeView->resizeColumnToContents(1);
    mTreeView->resizeColumnToContents(2);
    mTreeView->resizeColumnToContents(3);

    // Allow ourselves to be updated again

    setUpdatesEnabled(true);
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
