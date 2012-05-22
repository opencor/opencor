//==============================================================================
// CellML annotation view lists widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmlannotationviewcellmldetailswidget.h"
#include "cellmlannotationviewcellmlelementitem.h"
#include "cellmlannotationviewcellmlelementitemdelegate.h"
#include "cellmlannotationviewdetailswidget.h"
#include "cellmlannotationviewlistswidget.h"
#include "cellmlannotationviewwidget.h"
#include "treeview.h"

//==============================================================================

#include "ui_cellmlannotationviewlistswidget.h"

//==============================================================================

#include <QMenu>
#include <QStandardItemModel>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewListsWidget::CellmlAnnotationViewListsWidget(CellmlAnnotationViewWidget *pParent) :
    QSplitter(pParent),
    CommonWidget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewListsWidget),
    mOldIndex(QModelIndex())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create and customise our CellML tree view which will contain all of the
    // imports, units, components, groups and connections from a CellML file

    mCellmlTreeView            = new Core::TreeView(pParent);
    mCellmlDataModel           = new QStandardItemModel(mCellmlTreeView);
    mCellmlElementItemDelegate = new CellmlElementItemDelegate(mCellmlTreeView,
                                                               mCellmlDataModel);

    initTreeView(mCellmlTreeView, mCellmlDataModel, mCellmlElementItemDelegate);

    // Create and customise our metadata tree view which will contain all of the
    // metadata from a CellML file

    mMetadataTreeView  = new Core::TreeView(pParent);
    mMetadataDataModel = new QStandardItemModel(mMetadataTreeView);

    initTreeView(mMetadataTreeView, mMetadataDataModel);

    // Populate ourselves

    addWidget(new Core::BorderedWidget(mCellmlTreeView,
                                       false, false, true, true));
    addWidget(new Core::BorderedWidget(mMetadataTreeView,
                                       true, false, false, true));

    // We want a context menu for our CellML tree view

    mCellmlTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(mCellmlTreeView, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(cellmlTreeViewContextMenu(const QPoint &)));

    // Keep track of our splitter being moved

    connect(this, SIGNAL(splitterMoved(int,int)),
            this, SLOT(emitSplitterMoved()));

    // Set an event filter for our tree views

    mCellmlTreeView->installEventFilter(this);
    mMetadataTreeView->installEventFilter(this);

    // Some connections to handle the expansion/collapse of a CellML tree node

    connect(mCellmlTreeView, SIGNAL(expanded(const QModelIndex &)),
            this, SLOT(resizeCellmlTreeViewToContents()));
    connect(mCellmlTreeView, SIGNAL(collapsed(const QModelIndex &)),
            this, SLOT(resizeCellmlTreeViewToContents()));

    // Some connections to handle the change of CellML/metadata node

    connect(mCellmlTreeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(updateCellmlNode(const QModelIndex &, const QModelIndex &)));
    connect(mMetadataTreeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(updateMetadataNode(const QModelIndex &, const QModelIndex &)));

    // Populate our tree views

    populateCellmlDataModel();
    populateMetadataDataModel();

    // Make our CellML tree view our focus proxy

    setFocusProxy(mCellmlTreeView);

    // Expand our CellML tree view enough so that we can see the meaningful
    // parts of the CellML file

    mCellmlTreeView->expandToDepth(1);

    // Resize our tree views, just to be on the safe side

    resizeCellmlTreeViewToContents();
    mMetadataTreeView->resizeColumnToContents(0);
}

//==============================================================================

CellmlAnnotationViewListsWidget::~CellmlAnnotationViewListsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewListsWidget::retranslateUi()
{
    // Retranslate some of the nodes in the CellML tree view

    retranslateCellmlDataItem(static_cast<CellmlElementItem *>(mCellmlDataModel->invisibleRootItem()));
}

//==============================================================================

void CellmlAnnotationViewListsWidget::retranslateCellmlDataItem(CellmlElementItem *pCellmlElementItem)
{
    // Retranslate some of the node's children

    for (int i = 0, iMax = pCellmlElementItem->rowCount(); i < iMax; ++i)
        retranslateCellmlDataItem(static_cast<CellmlElementItem *>(pCellmlElementItem->child(i)));

    // Check whether we are dealing with a category or not

    if (pCellmlElementItem->isCategory())
        // We are dealing with a category, so retranslate its type

        switch (pCellmlElementItem->type()) {
        case CellmlElementItem::Import:
            pCellmlElementItem->setText(tr("Imports"));

            break;
        case CellmlElementItem::Unit:
            pCellmlElementItem->setText(tr("Units"));

            break;
        case CellmlElementItem::Component:
            pCellmlElementItem->setText(tr("Components"));

            break;
        case CellmlElementItem::Variable:
            pCellmlElementItem->setText(tr("Variables"));

            break;
        case CellmlElementItem::Group:
            pCellmlElementItem->setText(tr("Groups"));

            break;
        case CellmlElementItem::RelationshipReference:
            pCellmlElementItem->setText(tr("Relationship References"));

            break;
        case CellmlElementItem::ComponentReference:
            pCellmlElementItem->setText(tr("Component References"));

            break;
        case CellmlElementItem::Connection:
            pCellmlElementItem->setText(tr("Connections"));

            break;
        default:
            // Not a type we can retranslate, so do nothing...

            ;
        }
    else
        // We are not dealing with a category, so check the type and see whether
        // node needs retranslating

        switch (pCellmlElementItem->type()) {
        case CellmlElementItem::Group:
            pCellmlElementItem->setText(tr("Group #%1").arg(pCellmlElementItem->number()));

            break;
        case CellmlElementItem::Connection:
            pCellmlElementItem->setText(tr("Connection #%1").arg(pCellmlElementItem->number()));

            break;
        default:
            // Not a sub-type we can retranslate, so do nothing...

            ;
        }
}

//==============================================================================

bool CellmlAnnotationViewListsWidget::eventFilter(QObject *pObject,
                                                  QEvent *pEvent)
{
    switch (pEvent->type()) {
    case QEvent::FocusIn:
        // Check whether we are dealing with one of our tree views and, if so,
        // update their details

        if (pObject == mCellmlTreeView)
            updateCellmlNode(mCellmlTreeView->currentIndex(), QModelIndex());
        else if (pObject == mMetadataTreeView)
            updateMetadataNode(mMetadataTreeView->currentIndex(), QModelIndex());
        else
            return QWidget::eventFilter(pObject, pEvent);

        return true;
    default:
        // Another type of event which we don't handle, so...

        return QWidget::eventFilter(pObject, pEvent);
    }
}

//==============================================================================

void CellmlAnnotationViewListsWidget::selectFirstCellmlNode()
{
    // Select the first CellML node, if any

    if (mCellmlDataModel->invisibleRootItem()->rowCount())
        mCellmlTreeView->setCurrentIndex(mCellmlDataModel->invisibleRootItem()->child(0)->index());
}

//==============================================================================

void CellmlAnnotationViewListsWidget::initTreeView(Core::TreeView *pTreeView,
                                                   QStandardItemModel *pDataModel,
                                                   CellmlElementItemDelegate *pItemDelegate)
{
    pTreeView->setModel(pDataModel);

    if (pItemDelegate)
        pTreeView->setItemDelegate(pItemDelegate);

    pTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pTreeView->setFrameShape(QFrame::NoFrame);
    pTreeView->setHeaderHidden(true);
    pTreeView->setRootIsDecorated(false);
    pTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Note: the selection mode we are opting for means that there is always a
    //       node which is selected, so that's something we must keep in mind
    //       when showing the context menu...
}

//==============================================================================

void CellmlAnnotationViewListsWidget::populateCellmlDataModel()
{
    // Make sure that the CellML file was properly loaded

    if (mParent->cellmlFile()->issues().count()) {
        // Something went wrong while trying to load the CellML file, so report
        // it and leave

        mCellmlDataModel->invisibleRootItem()->appendRow(new CellmlElementItem(mParent->cellmlFile()->issues().first().type() == CellMLSupport::CellmlFileIssue::Error,
                                                                               mParent->cellmlFile()->issues().first().formattedMessage()));

        return;
    }

    // Retrieve the model's root

    CellmlElementItem *modelItem = new CellmlElementItem(CellmlElementItem::Model,
                                                         mParent->cellmlFile()->model());

    mCellmlDataModel->invisibleRootItem()->appendRow(modelItem);

    // Retrieve the model's imports

    if (mParent->cellmlFile()->imports().count()) {
        // Imports category

        CellmlElementItem *importsItem = new CellmlElementItem(CellmlElementItem::Import,
                                                               tr("Imports"));

        modelItem->appendRow(importsItem);

        // Retrieve the model's imports themselves

        foreach (CellMLSupport::CellmlFileImport *import,
                 mParent->cellmlFile()->imports()) {
            // A model import

            CellmlElementItem *importItem = new CellmlElementItem(CellmlElementItem::Import,
                                                                  import);

            importsItem->appendRow(importItem);

            // Retrieve the model's import's units

            if (import->units().count()) {
                // Units category

                CellmlElementItem *unitsItem = new CellmlElementItem(CellmlElementItem::ImportUnit,
                                                                     tr("Units"));

                importItem->appendRow(unitsItem);

                // Retrieve the model's import's units themselves

                foreach (CellMLSupport::CellmlFileImportUnit *unit,
                         import->units())
                    // A model's import's unit

                    unitsItem->appendRow(new CellmlElementItem(CellmlElementItem::ImportUnit,
                                                               unit));
            }

            // Retrieve the model's import's components

            if (import->components().count()) {
                // Components category

                CellmlElementItem *componentsItem = new CellmlElementItem(CellmlElementItem::ImportComponent,
                                                                          tr("Components"));

                importItem->appendRow(componentsItem);

                // Retrieve the model's import's components themselves

                foreach (CellMLSupport::CellmlFileImportComponent *component,
                         import->components())
                    // A model's import's component

                    componentsItem->appendRow(new CellmlElementItem(CellmlElementItem::ImportComponent,
                                                                    component));
            }
        }
    }

    // Retrieve the model's units

    populateUnitsDataModel(modelItem, mParent->cellmlFile()->units());

    // Retrieve the model's components

    if (mParent->cellmlFile()->components().count()) {
        // Components category

        CellmlElementItem *componentsItem = new CellmlElementItem(CellmlElementItem::Component,
                                                                  tr("Components"));

        modelItem->appendRow(componentsItem);

        // Retrieve the model's components themselves

        foreach (CellMLSupport::CellmlFileComponent *component,
                 mParent->cellmlFile()->components()) {
            // A model's component

            CellmlElementItem *componentItem = new CellmlElementItem(CellmlElementItem::Component,
                                                                     component);

            componentsItem->appendRow(componentItem);

            // Retrieve the model's component's units

            populateUnitsDataModel(componentItem, component->units());

            // Retrieve the model's component's variables

            if (component->variables().count()) {
                // Variables category

                CellmlElementItem *variablesItem = new CellmlElementItem(CellmlElementItem::Variable,
                                                                         tr("Variables"));

                componentItem->appendRow(variablesItem);

                // Retrieve the model's component's variables themselves

                foreach (CellMLSupport::CellmlFileVariable *variable,
                         component->variables())
                    variablesItem->appendRow(new CellmlElementItem(CellmlElementItem::Variable,
                                                                   variable));
            }
        }
    }

    // Retrieve the model's groups

    if (mParent->cellmlFile()->groups().count()) {
        // Groups category

        CellmlElementItem *groupsItem = new CellmlElementItem(CellmlElementItem::Group,
                                                              tr("Groups"));

        modelItem->appendRow(groupsItem);

        // Retrieve the model's groups themselves

        int counter = 0;

        foreach (CellMLSupport::CellmlFileGroup *group, mParent->cellmlFile()->groups()) {
            // A model's group

            CellmlElementItem *groupItem = new CellmlElementItem(CellmlElementItem::Group,
                                                                 group,
                                                                 ++counter);

            groupsItem->appendRow(groupItem);

            // Retrieve the model's group's relationship references

            if (group->relationshipReferences().count()) {
                // Relationship references category

                CellmlElementItem *relationshipReferencesItem = new CellmlElementItem(CellmlElementItem::RelationshipReference,
                                                                                      tr("Relationship References"));

                groupItem->appendRow(relationshipReferencesItem);

                // Retrieve the model's group's relationship references
                // themselves

                foreach (CellMLSupport::CellmlFileRelationshipReference *relationshipReference,
                         group->relationshipReferences())
                    relationshipReferencesItem->appendRow(new CellmlElementItem(CellmlElementItem::RelationshipReference,
                                                                                relationshipReference));
            }

            // Retrieve the model's group's component references

            if (group->componentReferences().count()) {
                // Component references category

                CellmlElementItem *componentReferencesItem = new CellmlElementItem(CellmlElementItem::ComponentReference,
                                                                                   tr("Component References"));

                groupItem->appendRow(componentReferencesItem);

                // Retrieve the model's group's relationship references
                // themselves

                foreach (CellMLSupport::CellmlFileComponentReference *componentReference,
                         group->componentReferences())
                    populateComponentReferenceDataModel(componentReferencesItem,
                                                        componentReference);
            }
        }
    }

    // Retrieve the model's connections

    if (mParent->cellmlFile()->connections().count()) {
        // Connections category

        CellmlElementItem *connectionsItem = new CellmlElementItem(CellmlElementItem::Connection,
                                                                   tr("Connections"));

        modelItem->appendRow(connectionsItem);

        // Retrieve the model's connections themselves

        int counter = 0;

        foreach (CellMLSupport::CellmlFileConnection *connection, mParent->cellmlFile()->connections()) {
            // A model's connection

            CellmlElementItem *connectionItem = new CellmlElementItem(CellmlElementItem::Connection,
                                                                      connection,
                                                                      ++counter);

            connectionsItem->appendRow(connectionItem);

            // Component mapping

            connectionItem->appendRow(new CellmlElementItem(CellmlElementItem::ComponentMapping,
                                                            connection->componentMapping()));

            // Variable mappings

            foreach (CellMLSupport::CellmlFileMapVariablesItem *mapVariablesItem,
                     connection->variableMappings())
                connectionItem->appendRow(new CellmlElementItem(CellmlElementItem::VariableMapping,
                                                                mapVariablesItem));
        }
    }
}

//==============================================================================

void CellmlAnnotationViewListsWidget::populateUnitsDataModel(CellmlElementItem *pCellmlElementItem,
                                                             const CellMLSupport::CellmlFileUnits pUnits)
{
    // Retrieve the units

    if (pUnits.count()) {
        // Units category

        CellmlElementItem *unitsItem = new CellmlElementItem(CellmlElementItem::Unit,
                                                             tr("Units"));

        pCellmlElementItem->appendRow(unitsItem);

        // Retrieve the units themselves

        foreach (CellMLSupport::CellmlFileUnit *unit, pUnits) {
            CellmlElementItem *unitItem = new CellmlElementItem(CellmlElementItem::Unit,
                                                                unit);

            unitsItem->appendRow(unitItem);

            // Retrieve the unit's unit elements

            if (unit->unitElements().count())
                foreach (CellMLSupport::CellmlFileUnitElement *unitElement,
                         unit->unitElements())
                    unitItem->appendRow(new CellmlElementItem(CellmlElementItem::UnitElement,
                                                              unitElement));

        }
    }
}

//==============================================================================

void CellmlAnnotationViewListsWidget::populateComponentReferenceDataModel(CellmlElementItem *pCellmlElementItem,
                                                                          CellMLSupport::CellmlFileComponentReference *pComponentReference)
{
    CellmlElementItem *componentReferenceItem = new CellmlElementItem(CellmlElementItem::ComponentReference,
                                                                      pComponentReference);

    pCellmlElementItem->appendRow(componentReferenceItem);

    // Retrieve the component reference's children

    foreach (CellMLSupport::CellmlFileComponentReference *componentReference,
             pComponentReference->componentReferences())
        populateComponentReferenceDataModel(componentReferenceItem, componentReference);
}

//==============================================================================

void CellmlAnnotationViewListsWidget::populateMetadataDataModel()
{
    // Make sure that the CellML file was properly loaded

    if (mParent->cellmlFile()->issues().count())
        // There are issues, so...

        return;

    // Retrieve the id of the different groups of triples

    QString uriBase = mParent->cellmlFile()->uriBase();
    QStringList ids = QStringList();

    foreach (CellMLSupport::CellmlFileRdfTriple *rdfTriple,
             mParent->cellmlFile()->metadata())
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
        //       <bqbiol:isVersionOf rdf:resource="urn:miriam:uniprot:Q4KLA0"/>
        //       <bqbiol:isVersionOf rdf:resource="urn:miriam:interpro:IPR006670"/>
        //       <bqbiol:isVersionOf rdf:resource="urn:miriam:obo.sbo:sbo%3A0000252"/>
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

                QStandardItem *metadataItem = new QStandardItem(id);

                metadataItem->setIcon(QIcon(":CellMLSupport_metadataNode"));

                mMetadataDataModel->invisibleRootItem()->appendRow(metadataItem);

                ids.append(id);
            }
        }

    // Sort the ids

    mMetadataDataModel->sort(0);
}

//==============================================================================

void CellmlAnnotationViewListsWidget::updateSizes(const QList<int> &pSizes)
{
    // The splitter of another CellmlAnnotationViewListsWidget object has been
    // moved, so update our sizes

    setSizes(pSizes);
}

//==============================================================================

void CellmlAnnotationViewListsWidget::emitSplitterMoved()
{
    // Let whoever know that our splitter has been moved

    emit splitterMoved(sizes());
}

//==============================================================================

void CellmlAnnotationViewListsWidget::resizeCellmlTreeViewToContents()
{
    // Resize our CellML tree view so that its contents is visible

    mCellmlTreeView->resizeColumnToContents(0);
}

//==============================================================================

void CellmlAnnotationViewListsWidget::updateCellmlNode(const QModelIndex &pNewIndex,
                                                       const QModelIndex &pOldIndex)
{
    Q_UNUSED(pOldIndex);

    // Make sure that we are not already updating a CellML node, that we are not
    // trying to update the same CellML node, that the details GUI is valid and
    // that we have a valid pNewIndex

    static bool alreadyUpdatingCellmlNode = false;

    if (   alreadyUpdatingCellmlNode || (pNewIndex == mOldIndex)
        || !mParent->detailsWidget() || (pNewIndex == QModelIndex()))
        return;

    alreadyUpdatingCellmlNode = true;
    mOldIndex = pNewIndex;

    // Retrieve all the CellML items which properties we want to be added to the
    // details GUI

    CellmlAnnotationViewCellmlElementDetailsWidget::Items items = CellmlAnnotationViewCellmlElementDetailsWidget::Items();
    QModelIndex crtIndex = pNewIndex;

    do {
        CellmlElementItem *crtCellmlElementItem = static_cast<CellmlElementItem *>(mCellmlDataModel->itemFromIndex(crtIndex));

        // Add the item based on its type, but only if it's not a category

        if (!crtCellmlElementItem->isCategory())
            switch (crtCellmlElementItem->type()) {
            case CellmlElementItem::Model:
                items << CellmlAnnotationViewCellmlElementDetailsWidget::item(CellmlAnnotationViewCellmlElementDetailsWidget::Model,
                                                                              crtCellmlElementItem->element());

                break;
            case CellmlElementItem::Import:
                items << CellmlAnnotationViewCellmlElementDetailsWidget::item(CellmlAnnotationViewCellmlElementDetailsWidget::Import,
                                                                              crtCellmlElementItem->element());

                break;
            case CellmlElementItem::ImportUnit:
                items << CellmlAnnotationViewCellmlElementDetailsWidget::item(CellmlAnnotationViewCellmlElementDetailsWidget::ImportUnit,
                                                                              crtCellmlElementItem->element());

                break;
            case CellmlElementItem::ImportComponent:
                items << CellmlAnnotationViewCellmlElementDetailsWidget::item(CellmlAnnotationViewCellmlElementDetailsWidget::ImportComponent,
                                                                              crtCellmlElementItem->element());

                break;
            case CellmlElementItem::Unit:
                items << CellmlAnnotationViewCellmlElementDetailsWidget::item(CellmlAnnotationViewCellmlElementDetailsWidget::Unit,
                                                                              crtCellmlElementItem->element());

                break;
            case CellmlElementItem::UnitElement:
                items << CellmlAnnotationViewCellmlElementDetailsWidget::item(CellmlAnnotationViewCellmlElementDetailsWidget::UnitElement,
                                                                              crtCellmlElementItem->element());

                break;
            case CellmlElementItem::Component:
                items << CellmlAnnotationViewCellmlElementDetailsWidget::item(CellmlAnnotationViewCellmlElementDetailsWidget::Component,
                                                                              crtCellmlElementItem->element());

                break;
            case CellmlElementItem::Variable:
                items << CellmlAnnotationViewCellmlElementDetailsWidget::item(CellmlAnnotationViewCellmlElementDetailsWidget::Variable,
                                                                              crtCellmlElementItem->element());

                break;
            case CellmlElementItem::Group:
                items << CellmlAnnotationViewCellmlElementDetailsWidget::item(CellmlAnnotationViewCellmlElementDetailsWidget::Group,
                                                                              crtCellmlElementItem->element(),
                                                                              crtCellmlElementItem->number());

                break;
            case CellmlElementItem::RelationshipReference:
                items << CellmlAnnotationViewCellmlElementDetailsWidget::item(CellmlAnnotationViewCellmlElementDetailsWidget::RelationshipReference,
                                                                              crtCellmlElementItem->element());

                break;
            case CellmlElementItem::ComponentReference:
                items << CellmlAnnotationViewCellmlElementDetailsWidget::item(CellmlAnnotationViewCellmlElementDetailsWidget::ComponentReference,
                                                                              crtCellmlElementItem->element());

                break;
            case CellmlElementItem::Connection:
                items << CellmlAnnotationViewCellmlElementDetailsWidget::item(CellmlAnnotationViewCellmlElementDetailsWidget::Connection,
                                                                              crtCellmlElementItem->element(),
                                                                              crtCellmlElementItem->number());

                break;
            case CellmlElementItem::ComponentMapping:
                items << CellmlAnnotationViewCellmlElementDetailsWidget::item(CellmlAnnotationViewCellmlElementDetailsWidget::ComponentMapping,
                                                                              crtCellmlElementItem->element());

                break;
            case CellmlElementItem::VariableMapping:
                items << CellmlAnnotationViewCellmlElementDetailsWidget::item(CellmlAnnotationViewCellmlElementDetailsWidget::VariableMapping,
                                                                              crtCellmlElementItem->element());

                break;
            default:
                // Either an error, warning, category or metadata, so nothing to
                // show/do...

                ;
            }

        // Go to the next index

        crtIndex = crtIndex.parent();
    } while (crtIndex != QModelIndex());

    // Reverse the list, so that we start from the CellML item's parents and
    // finish with the CellML item itself

    int itemsCount = items.count();

    for (int i = 0, iMax = itemsCount >> 1; i < iMax; ++i)
        items.swap(i, itemsCount-(i+1));

    // Update the details GUI

    mParent->detailsWidget()->updateGui(items);

    // Check that the current index is the one that got us here in the first
    // place and update ourselves, if not

    if (pNewIndex != mCellmlTreeView->currentIndex())
        updateCellmlNode(mCellmlTreeView->currentIndex(), pNewIndex);

    // We are done, so...

    alreadyUpdatingCellmlNode = false;
}

//==============================================================================

void CellmlAnnotationViewListsWidget::updateMetadataNode(const QModelIndex &pNewIndex,
                                                         const QModelIndex &pOldIndex)
{
    Q_UNUSED(pOldIndex);

    // Make sure that we are not already updating a metadata node, that we are
    // not trying to update the same metadata node, that the details GUI is
    // valid and that we have a valid pNewIndex

    static bool alreadyUpdatingMetadataNode = false;

    if (   alreadyUpdatingMetadataNode || (pNewIndex == mOldIndex)
        || !mParent->detailsWidget() || (pNewIndex == QModelIndex()))
        return;

    alreadyUpdatingMetadataNode = true;
    mOldIndex = pNewIndex;

    // Update the details GUI

    mParent->detailsWidget()->updateGui(mParent->rdfTriples(mMetadataDataModel->itemFromIndex(pNewIndex)->text()));

    // Check that the current index is the one that got us here in the first
    // place and update ourselves, if not

    if (pNewIndex != mMetadataTreeView->currentIndex())
        updateMetadataNode(mMetadataTreeView->currentIndex(), pNewIndex);

    // We are done, so...

    alreadyUpdatingMetadataNode = false;
}

//==============================================================================

void CellmlAnnotationViewListsWidget::cellmlTreeViewContextMenu(const QPoint &pPosition) const
{
    Q_UNUSED(pPosition);

    // Determine whether to show context menu or not by checking whether the
    // current item is the same as the one over which we are

    CellmlElementItem *posItem = static_cast<CellmlElementItem *>(mCellmlDataModel->itemFromIndex(mCellmlTreeView->indexAt(mCellmlTreeView->mapFromGlobal(QCursor::pos()-mCellmlTreeView->pos()))));
    CellmlElementItem *crtItem = static_cast<CellmlElementItem *>(mCellmlDataModel->itemFromIndex(mCellmlTreeView->currentIndex()));

    bool showContextMenu = (posItem == crtItem) && crtItem->hasChildren();

    // Create a custom context menu which items match the contents of our
    // toolbar

    if (showContextMenu) {
        // Update the enabled status of our actions

        mGui->actionExpandAll->setEnabled(!indexIsAllExpanded(mCellmlTreeView->currentIndex()));
        mGui->actionCollapseAll->setEnabled(mCellmlTreeView->isExpanded(mCellmlTreeView->currentIndex()));

        // Create and show the context menu

        QMenu menu;

        menu.addAction(mGui->actionExpandAll);
        menu.addAction(mGui->actionCollapseAll);

        menu.exec(QCursor::pos());
    }
}

//==============================================================================

void CellmlAnnotationViewListsWidget::on_actionExpandAll_triggered()
{
    // Expand all the nodes below the current one
    // Note: we disable and then re-enable updates before expanding all the
    //       index since it may end up in quite a few updates...

    mCellmlTreeView->setUpdatesEnabled(false);
        indexExpandAll(mCellmlTreeView->currentIndex());
    mCellmlTreeView->setUpdatesEnabled(true);
}

//==============================================================================

void CellmlAnnotationViewListsWidget::on_actionCollapseAll_triggered()
{
    // Collapse all the nodes below the current one
    // Note: see the note in on_actionExpandAll_triggered() above...

    mCellmlTreeView->setUpdatesEnabled(false);
        indexCollapseAll(mCellmlTreeView->currentIndex());
    mCellmlTreeView->setUpdatesEnabled(true);
}

//==============================================================================

void CellmlAnnotationViewListsWidget::indexExpandAll(const QModelIndex &pIndex) const
{
    // Recursively expand all the nodes below the current one
    // Note: the test with against pIndex.child(0, 0) is to ensure that we are
    //       not trying to expand an index which item has no children. Indeed,
    //       a call to expand() is quite expensive, so the fewer of those we
    //       make the better...

    if (pIndex.child(0, 0) != QModelIndex()) {
        mCellmlTreeView->expand(pIndex);

        QStandardItem *item = mCellmlDataModel->itemFromIndex(pIndex);

        for (int i = 0, iMax = item->rowCount(); i < iMax; ++i)
            indexExpandAll(item->child(i)->index());
    }
}

//==============================================================================

void CellmlAnnotationViewListsWidget::indexCollapseAll(const QModelIndex &pIndex) const
{
    // Recursively collapse all the nodes below the current one
    // Note: see the note in indexExpandAll() above...

    if (pIndex.child(0, 0) != QModelIndex()) {
        QStandardItem *item = mCellmlDataModel->itemFromIndex(pIndex);

        for (int i = 0, iMax = item->rowCount(); i < iMax; ++i)
            indexCollapseAll(item->child(i)->index());

        mCellmlTreeView->collapse(pIndex);
    }
}

//==============================================================================

bool CellmlAnnotationViewListsWidget::indexIsAllExpanded(const QModelIndex &pIndex) const
{
    // Recursively check that the current node and all of its children are
    // expanded
    // Note: see the note in indexExpandAll() above...

    if (pIndex.child(0, 0) != QModelIndex()) {
        QStandardItem *item = mCellmlDataModel->itemFromIndex(pIndex);

        for (int i = 0, iMax = item->rowCount(); i < iMax; ++i)
            if (!indexIsAllExpanded(item->child(i)->index()))
                return false;

        return mCellmlTreeView->isExpanded(pIndex);
    } else {
        return true;
    }
}

//==============================================================================

QStringList CellmlAnnotationViewListsWidget::metadataIds() const
{
    // Return the full list of metadata ids

    QStringList res = QStringList();

    for (int i = 0, iMax = mMetadataDataModel->invisibleRootItem()->rowCount(); i < iMax; ++i)
        res << mMetadataDataModel->invisibleRootItem()->child(i)->text();

    return res;
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
