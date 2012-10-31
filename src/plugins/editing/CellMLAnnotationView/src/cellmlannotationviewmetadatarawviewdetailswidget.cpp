//==============================================================================
// CellML annotation view metadata raw view details widget
//==============================================================================

#include "cellmlannotationviewmetadatarawviewdetailswidget.h"
#include "cellmlannotationviewwidget.h"

//==============================================================================

#include <QStandardItemModel>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataRawViewDetailsWidget::CellmlAnnotationViewMetadataRawViewDetailsWidget(CellmlAnnotationViewWidget *pParent) :
    TreeViewWidget(pParent),
    mCellmlFile(pParent->cellmlFile())
{
    // Customise ourselves

    mDataModel = new QStandardItemModel(this);

    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setFrameShape(QFrame::NoFrame);
    setModel(mDataModel);
    setRootIsDecorated(false);
    setSelectionMode(QAbstractItemView::SingleSelection);

    // Some further initialisations which are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();
}

//==============================================================================

void CellmlAnnotationViewMetadataRawViewDetailsWidget::retranslateUi()
{
    // Update our header labels

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

    resizeColumnToContents(0);
    resizeColumnToContents(1);
    resizeColumnToContents(2);
    resizeColumnToContents(3);

    // Allow ourselves to be updated again

    setUpdatesEnabled(true);
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
