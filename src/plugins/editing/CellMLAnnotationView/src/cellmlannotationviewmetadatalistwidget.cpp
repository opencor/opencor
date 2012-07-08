//==============================================================================
// CellML annotation view metadata list widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmlannotationviewcellmldetailswidget.h"
#include "cellmlannotationviewcellmlelementitem.h"
#include "cellmlannotationviewcellmlelementitemdelegate.h"
#include "cellmlannotationviewcellmllistwidget.h"
#include "cellmlannotationviewdetailswidget.h"
#include "cellmlannotationviewlistswidget.h"
#include "cellmlannotationviewmetadatalistwidget.h"
#include "cellmlannotationviewwidget.h"
#include "coreutils.h"
#include "toolbar.h"
#include "treeview.h"

//==============================================================================

#include "ui_cellmlannotationviewmetadatalistwidget.h"

//==============================================================================

#include <QMenu>
#include <QStandardItemModel>
#include <QToolButton>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataListWidget::CellmlAnnotationViewMetadataListWidget(CellmlAnnotationViewWidget *pParent) :
    Widget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewMetadataListWidget),
    mIndexes(QList<QModelIndex>())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create a toolbar with different buttons

    Core::ToolBar *toolbar = new Core::ToolBar(this);

    // Create a menu button for our removal of metadata

    QToolButton *removeMetadataButton = new QToolButton(this);
    QMenu *removeMetadataMenu = new QMenu(this);

    removeMetadataMenu->addAction(mGui->actionRemoveCurrentMetadata);
    removeMetadataMenu->addAction(mGui->actionRemoveAllMetadata);

    removeMetadataButton->setDefaultAction(mGui->actionRemoveMetadata);
    removeMetadataButton->setMenu(removeMetadataMenu);
    removeMetadataButton->setPopupMode(QToolButton::MenuButtonPopup);

    // Create a menu button for our clearing up of metadata

    QToolButton *clearMetadataButton = new QToolButton(this);
    QMenu *clearMetadataMenu = new QMenu(this);

    clearMetadataMenu->addAction(mGui->actionClearCurrentMetadata);
    clearMetadataMenu->addAction(mGui->actionClearAllMetadata);

    clearMetadataButton->setDefaultAction(mGui->actionClearMetadata);
    clearMetadataButton->setMenu(clearMetadataMenu);
    clearMetadataButton->setPopupMode(QToolButton::MenuButtonPopup);

    // Populate our toolbar

    toolbar->addAction(mGui->actionAddMetadata);
    toolbar->addWidget(removeMetadataButton);
    toolbar->addSeparator();
    toolbar->addWidget(clearMetadataButton);

    // Create and customise our tree view which will contain all of the metadata
    // from a CellML file

    mTreeView  = new Core::TreeView(pParent);
    mDataModel = new QStandardItemModel(mTreeView);

    mTreeView->setModel(mDataModel);

    mTreeView->setEditTriggers(QAbstractItemView::EditKeyPressed);
    mTreeView->setFrameShape(QFrame::NoFrame);
    mTreeView->setHeaderHidden(true);
    mTreeView->setRootIsDecorated(false);
    mTreeView->setSelectionMode(QAbstractItemView::SingleSelection);

    // Populate ourselves

    mGui->layout->addWidget(toolbar);
    mGui->layout->addWidget(Core::newLineWidget(this));
    mGui->layout->addWidget(mTreeView);

    // Set an event filter for our tree view

    mTreeView->installEventFilter(this);

    // A couple of connections to handle the change of node

    connect(mTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(updateActions()));
    connect(mTreeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(updateNode(const QModelIndex &, const QModelIndex &)));

    // A couple of connections to handle the addition/removal of a node

    connect(mTreeView->model(), SIGNAL(rowsInserted(const QModelIndex &, int, int)),
            this, SLOT(updateActions()));
    connect(mTreeView->model(), SIGNAL(rowsRemoved(const QModelIndex &, int, int)),
            this, SLOT(updateActions()));

    // A connection to handle the editing of a node

    connect(mDataModel, SIGNAL(itemChanged(QStandardItem *)),
            this, SLOT(itemChanged(QStandardItem *)));

    // A connection to make sure that our tree view has the focus whenever some
    // metadata has been updated

    connect(this, SIGNAL(metadataUpdated()),
            mTreeView, SLOT(setFocus()));

    // Populate our tree view

    populateDataModel();

    // Resize our tree view, just to be on the safe side

    mTreeView->resizeColumnToContents(0);
}

//==============================================================================

CellmlAnnotationViewMetadataListWidget::~CellmlAnnotationViewMetadataListWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewMetadataListWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);
}

//==============================================================================

bool CellmlAnnotationViewMetadataListWidget::eventFilter(QObject *pObject,
                                                         QEvent *pEvent)
{
    Q_UNUSED(pObject);

    switch (pEvent->type()) {
    case QEvent::FocusIn: {
        // Note: to override focusInEvent() is not what we want since it won't
        //       be triggered when selecting a node in the metadata list after
        //       having selecting a node in the CellML list. Indeed, it's
        //       already 'in focus', so...

        updateNode(mTreeView->currentIndex(), QModelIndex());

        return true;
    }
    default:
        // Another type of event which we don't handle ourselves, so...

        return QWidget::eventFilter(pObject, pEvent);
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataListWidget::populateDataModel()
{
    // Make sure that the CellML file was properly loaded

    if (mParent->cellmlFile()->issues().count())
        // There are issues, so...

        return;

    // Retrieve the id of the different groups of triples

    QString uriBase = mParent->cellmlFile()->uriBase();
    QStringList ids = QStringList();

    foreach (CellMLSupport::CellmlFileRdfTriple *rdfTriple,
             *mParent->cellmlFile()->rdfTriples())
        // Retrieve the RDF triple's subject so we can determine to which group
        // of RDF triples it should be added
        // Note: the RDF triple is part of an rdf:Description element which is
        //       itself part of an rdf:RDF element. The rdf:Description element
        //       has an rdf:about attribute which may or not have a value
        //       assigned to it. If no value was assigned, then the RDF triple
        //       is valid at the whole CellML model level. On the other hand, if
        //       a value was assigned (and of the format #<id>), then it will be
        //       associated to any CellML element which cmeta:id value is <id>.
        //       A couple of examples:
        //
        // <rdf:RDF xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:vCard="http://www.w3.org/2001/vcard-rdf/3.0#">
        //   <rdf:Description rdf:about="">
        //     <dc:creator rdf:parseType="Resource">
        //       <vCard:EMAIL rdf:parseType="Resource">
        //         <rdf:type rdf:resource="http://imc.org/vCard/3.0#internet"/>
        //         <rdf:value>someone@somewhere.com</rdf:value>
        //       </vCard:EMAIL>
        //     </dc:creator>
        //   </rdf:Description>
        // </rdf:RDF>
        //
        // <variable units="micromolar" public_interface="out" cmeta:id="C_C" name="C" initial_value="0.01">
        //   <rdf:RDF xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:bqbiol="http://biomodels.net/biology-qualifiers/">
        //     <rdf:Description rdf:about="#C_C">
        //       <bqbiol:isVersionOf rdf:resource="http://identifiers.org/uniprot/Q4KLA0"/>
        //       <bqbiol:isVersionOf rdf:resource="http://identifiers.org/interpro/IPR006670"/>
        //       <bqbiol:isVersionOf rdf:resource="http://identifiers.org/obo.sbo/sbo:0000252"/>
        //     </rdf:Description>
        //   </rdf:RDF>
        // </variable>

        if (rdfTriple->subject()->type() == CellMLSupport::CellmlFileRdfTripleElement::UriReference) {
            // We have an RDF triple of which we can make sense, so add it to
            // the correct group of RDF triples
            // Note: we want the id of the group to be the same as that of the
            //       cmeta:id of a CellML element. This means that we must
            //       remove the URI base (and hash character) which makes the
            //       beginning of the RDF triple's subject's URI reference...

            QString id = rdfTriple->subject()->uriReference().remove(QRegExp("^"+QRegExp::escape(uriBase)+"#?"));

            if (!ids.contains(id)) {
                // The id hasn't already been added, so add it and keep track of
                // it

                QStandardItem *item = new QStandardItem(id);

                item->setIcon(QIcon(":CellMLSupport_metadataNode"));

                mDataModel->invisibleRootItem()->appendRow(item);

                ids << id;
            }
        }

    // Sort the ids

    mDataModel->sort(0);
}

//==============================================================================

void CellmlAnnotationViewMetadataListWidget::updateActions()
{
    // Enable/disable the removal/clearing of metadata

    bool enabled = mTreeView->selectionModel()->selectedIndexes().count();

    mGui->actionRemoveMetadata->setEnabled(enabled);
    mGui->actionRemoveCurrentMetadata->setEnabled(enabled);
    mGui->actionRemoveAllMetadata->setEnabled(enabled);

    mGui->actionClearMetadata->setEnabled(enabled);
    mGui->actionClearCurrentMetadata->setEnabled(enabled);
    mGui->actionClearAllMetadata->setEnabled(enabled);
}

//==============================================================================

void CellmlAnnotationViewMetadataListWidget::updateNode(const QModelIndex &pNewIndex,
                                                        const QModelIndex &pOldIndex)
{
    Q_UNUSED(pOldIndex);

    // Make sure that the details GUI is valid and that we have a valid
    // pNewIndex

    if (!mParent->detailsWidget() || (pNewIndex == QModelIndex()))
        return;

    // Keep track of the fact that there is a node to update

    mIndexes << pNewIndex;

    // Make sure that we are not already updating a node

    static bool alreadyUpdatingNode = false;

    if (alreadyUpdatingNode)
        return;

    alreadyUpdatingNode = true;

    // Loop while there are nodes to update
    // Note: this is done because a node may take time to update, so we may end
    //       up in a situation where several nodes need updating...

    while (mIndexes.count()) {
        // Retrieve the first node to update

        QModelIndex crtIndex = mIndexes.first();

        mIndexes.removeFirst();

        // Keep track of the id

        mId = mDataModel->itemFromIndex(crtIndex)->text();

        // Update the details GUI

        mParent->detailsWidget()->updateGui(mParent->cellmlFile()->rdfTriples(mDataModel->itemFromIndex(crtIndex)->text()));
    }

    // We are done, so...

    alreadyUpdatingNode = false;
}

//==============================================================================

void CellmlAnnotationViewMetadataListWidget::itemChanged(QStandardItem * pItem)
{
    // An id has been renamed, update the metadata in the CellML file

    QString newId = pItem->text();

    mParent->cellmlFile()->rdfTriples()->renameMetadataId(mId, newId);

    // Resort our list

    mDataModel->sort(0);

    // Make sure that the new id is selected

    setCurrentId(newId);

    // Let people know that some metadata has been renamed

    emit metadataUpdated();
}

//==============================================================================

Core::TreeView * CellmlAnnotationViewMetadataListWidget::treeView() const
{
    // Return our tree view

    return mTreeView;
}

//==============================================================================

QString CellmlAnnotationViewMetadataListWidget::currentId() const
{
    // Return the current id

    QModelIndex currentIndex = mTreeView->currentIndex();

    if (currentIndex.isValid())
        return mDataModel->itemFromIndex(currentIndex)->text();
    else
        return QString();
}

//==============================================================================

void CellmlAnnotationViewMetadataListWidget::setCurrentId(const QString &pId)
{
    // Set the given id as our new current id

    for (int i = 0, iMax = mDataModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
        QStandardItem *standardItem = mDataModel->invisibleRootItem()->child(i);

        if (!pId.compare(standardItem->text())) {
            // We have found our new current id in our list, so select it and
            // force its details to be shown
            // Note: we need to force its details to be shown (by 'resetting'
            //       the current index) because it may already be the current id
            //       (e.g. we were viewing some CellML element information and
            //       decided to edit the metadata associated with the CellML
            //       element's cmeta:id), so...

            QModelIndex index = mDataModel->indexFromItem(standardItem);

            mTreeView->setCurrentIndex(QModelIndex());
            mTreeView->setCurrentIndex(index);

            break;
        }
    }
}

//==============================================================================

QStringList CellmlAnnotationViewMetadataListWidget::ids() const
{
    // Return the full list of ids

    QStringList res = QStringList();

    for (int i = 0, iMax = mDataModel->invisibleRootItem()->rowCount(); i < iMax; ++i)
        res << mDataModel->invisibleRootItem()->child(i)->text();

    return res;
}

//==============================================================================

void CellmlAnnotationViewMetadataListWidget::on_actionAddMetadata_triggered()
{
    // Generate a metadata id, making sure that it's not already taken

    QStringList crtIds = ids();

    QString baseMetadataId = tr("Metadata");
    QString templateMetadataId = baseMetadataId+" (%1)";

    int metadataIdNb = 1;
    QString metadataId = baseMetadataId;

    while (crtIds.contains(metadataId))
        metadataId = templateMetadataId.arg(++metadataIdNb);

    // Create our new metadata item

    QStandardItem *metadataItem = new QStandardItem(metadataId);

    metadataItem->setIcon(QIcon(":CellMLSupport_metadataNode"));

    mDataModel->invisibleRootItem()->appendRow(metadataItem);

    // Sort our metadata items

    mTreeView->sortByColumn(0, Qt::AscendingOrder);

    // Scroll down to our current metadata item

    mTreeView->scrollTo(mTreeView->currentIndex(),
                        QAbstractItemView::PositionAtCenter);

    // Edit our new metadata item, but only if our current metadata item is not
    // already being edited
    // Note: whether a metadata item is being edited or not, we are going to end
    //       up editing a metadata item and upon editing, we emit the
    //       metadataUpdated signal (see itemChanged()) which is exactly what we
    //       want...

    if (!mTreeView->isEditing())
        mTreeView->edit(metadataItem->index());
}

//==============================================================================

void CellmlAnnotationViewMetadataListWidget::on_actionRemoveMetadata_triggered()
{
    // Remove the current metadata

    on_actionRemoveCurrentMetadata_triggered();
}

//==============================================================================

void CellmlAnnotationViewMetadataListWidget::on_actionRemoveCurrentMetadata_triggered()
{
    // Remove the current metadata, i.e. all the RDF triples which subject is
    // the same as the cmeta:id

    QString cmetaId = mDataModel->itemFromIndex(mTreeView->currentIndex())->text();

    mParent->cellmlFile()->rdfTriples()->remove(cmetaId);

    // Remove the entry for the cmeta:id from our data model

    for (int i = 0, iMax = mDataModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
        QStandardItem *item = mDataModel->invisibleRootItem()->child(i);

        if (!item->text().compare(cmetaId)) {
            delete item;

            mDataModel->removeRow(i);

            break;
        }
    }

    // Let people know that some metadata has been removed

    emit metadataUpdated();
}

//==============================================================================

void CellmlAnnotationViewMetadataListWidget::on_actionRemoveAllMetadata_triggered()
{
    // Remove all the metadata, i.e. all the RDF triples

    mParent->cellmlFile()->rdfTriples()->removeAll();

    // Clean the data model

    for (int i = 0, iMax = mDataModel->invisibleRootItem()->rowCount(); i < iMax; ++i)
        delete mDataModel->invisibleRootItem()->child(i);

    mDataModel->clear();

    // Let people know that all the metadata have been removed

    emit metadataUpdated();
}

//==============================================================================

void CellmlAnnotationViewMetadataListWidget::on_actionClearMetadata_triggered()
{
    // Clear the current metadata

    on_actionClearCurrentMetadata_triggered();
}

//==============================================================================

void CellmlAnnotationViewMetadataListWidget::on_actionClearCurrentMetadata_triggered()
{
    // Clear the current metadata, i.e. all the RDF triples which subject is the
    // same as the cmeta:id

    mParent->cellmlFile()->rdfTriples()->remove(mDataModel->itemFromIndex(mTreeView->currentIndex())->text());

    // Let people know that some metadata has been removed

    emit metadataUpdated();
}

//==============================================================================

void CellmlAnnotationViewMetadataListWidget::on_actionClearAllMetadata_triggered()
{
    // Clear all the metadata, i.e. all the RDF triples

    mParent->cellmlFile()->rdfTriples()->removeAll();

    // Let people know that all the metadata have been removed

    emit metadataUpdated();
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
