//==============================================================================
// CellML annotation view metadata view details widget
//==============================================================================

#include "cellmlannotationviewmetadataviewdetailswidget.h"
#include "cellmlannotationviewwidget.h"
#include "treeview.h"

//==============================================================================

#include "ui_cellmlannotationviewmetadataviewdetailswidget.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataViewDetailsWidget::CellmlAnnotationViewMetadataViewDetailsWidget(CellmlAnnotationViewWidget *pParent) :
    QSplitter(pParent),
    CommonWidget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewMetadataViewDetailsWidget),
    mRdfTriples(CellMLSupport::CellmlFileRdfTriples())
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

    addWidget(mTreeView);
}

//==============================================================================

CellmlAnnotationViewMetadataViewDetailsWidget::~CellmlAnnotationViewMetadataViewDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewMetadataViewDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    updateGui(mRdfTriples);
}

//==============================================================================

void CellmlAnnotationViewMetadataViewDetailsWidget::updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples)
{
    // Hide ourselves (to avoid any flickering during the updaate)

    setVisible(false);

    // Keep track of the RDF triples

    mRdfTriples = pRdfTriples;

    // Update the header labels
    // Note: we do it here (rather than in the constructor) in case the user
    //       decides to change languages...

    mDataModel->setHorizontalHeaderLabels(QStringList() << tr("#")
                                                        << tr("Subject")
                                                        << tr("Predicate")
                                                        << tr("Object"));

    // Remove all previous RDF triples from our tree view

    while (mDataModel->rowCount())
        foreach (QStandardItem *item, mDataModel->takeRow(0))
            delete item;

    // Add the 'new' RDF triples to our tree view
    // Note: for the RDF triple's subject, we try to remove the CellML file's
    //       URI base, thus only leaving the equivalent of a CellML element
    //       cmeta:id which will speak more to the user than a possibly long URI
    //       reference...

    QString uriBase = mParent->cellmlFile()->uriBase();
    int rdfTripleCounter = 0;

    foreach (CellMLSupport::CellmlFileRdfTriple *rdfTriple, pRdfTriples)
        mDataModel->invisibleRootItem()->appendRow(QList<QStandardItem *>() << new QStandardItem(QString::number(++rdfTripleCounter))
                                                                            << new QStandardItem(rdfTriple->subject()->asString().remove(QRegExp("^"+QRegExp::escape(uriBase)+"#?")))
                                                                            << new QStandardItem(rdfTriple->predicate()->asString())
                                                                            << new QStandardItem(rdfTriple->object()->asString()));

    // Make sure that all the columns have their contents fit

    mTreeView->resizeColumnToContents(0);
    mTreeView->resizeColumnToContents(1);
    mTreeView->resizeColumnToContents(2);
    mTreeView->resizeColumnToContents(3);

    // Re-show ourselves

    setVisible(true);
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
