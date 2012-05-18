//==============================================================================
// CellML annotation view metadata details widget
//==============================================================================

#include "cellmlannotationviewdetailswidget.h"
#include "cellmlannotationviewmetadatadetailswidget.h"
#include "cellmlannotationviewwidget.h"
#include "treeview.h"

//==============================================================================

#include "ui_cellmlannotationviewmetadatadetailswidget.h"

//==============================================================================

#include <QVBoxLayout>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataDetailsWidget::CellmlAnnotationViewMetadataDetailsWidget(CellmlAnnotationViewDetailsWidget *pParent) :
    QScrollArea(pParent),
    Core::CommonWidget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewMetadataDetailsWidget),
    mRdfTriples(CellMLSupport::CellmlFileRdfTriples()),
    mWidget(0),
    mLayout(0),
    mTreeView(0),
    mDataModel(0)
{
    // Set up the GUI

    mGui->setupUi(this);

    mWidget = new QWidget(this);
    mLayout = new QVBoxLayout(mWidget);

    mLayout->setMargin(0);

    mWidget->setLayout(mLayout);

    mTreeView  = new Core::TreeView(mWidget);
    mDataModel = new QStandardItemModel(mTreeView);

    mTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mTreeView->setFrameShape(QFrame::NoFrame);
    mTreeView->setModel(mDataModel);
    mTreeView->setRootIsDecorated(false);
    mTreeView->setSelectionMode(QAbstractItemView::SingleSelection);

    mDataModel->setHorizontalHeaderLabels(QStringList() << tr("#")
                                                        << tr("Subject")
                                                        << tr("Predicate")
                                                        << tr("Object"));

    mLayout->addWidget(mTreeView);

    setWidget(mWidget);
}

//==============================================================================

CellmlAnnotationViewMetadataDetailsWidget::~CellmlAnnotationViewMetadataDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    updateGui(mRdfTriples);
    finalizeGui();
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples)
{
    // Keep track of the RDF triples

    mRdfTriples = pRdfTriples;

    // Remove all previous RDF triples from our tree view

    while (mDataModel->rowCount())
        foreach (QStandardItem *item, mDataModel->takeRow(0))
            delete item;

    // Add the 'new' RDF triples to our tree view
    // Note: for the RDF triple's subject, we try to remove the CellML file's
    //       URI base, thus only leaving the equivalent of a CellML element
    //       cmeta:id which will speak more to the user than a possibly long
    //       URI reference...

    QString uriBase = mParent->parent()->cellmlFile()->uriBase();
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
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::finalizeGui()
{
    // Nothing to do at this stage...
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
