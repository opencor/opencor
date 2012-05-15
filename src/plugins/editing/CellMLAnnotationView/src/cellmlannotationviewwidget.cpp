//==============================================================================
// CellML annotation view widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmlannotationviewdetailswidget.h"
#include "cellmlannotationviewwidget.h"
#include "cellmlfilemanager.h"
#include "treeview.h"

//==============================================================================

#include "ui_cellmlannotationviewwidget.h"

//==============================================================================

#include <QLineEdit>
#include <QMenu>
#include <QRawFont>
#include <QSplitter>
#include <QStandardItemModel>
#include <QTextEdit>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlItemDelegate::CellmlItemDelegate(QWidget *pParent,
                                       QStandardItemModel *pDataModel) :
    QStyledItemDelegate(pParent),
    mCellmlDataModel(pDataModel)
{
}

//==============================================================================

void CellmlItemDelegate::paint(QPainter *pPainter,
                               const QStyleOptionViewItem &pOption,
                               const QModelIndex &pIndex) const
{
    // Paint the item as normal, except for the items which are not checkable
    // (i.e. plugins which the user cannot decide whether to load) in which case
    // we paint them as if they were disabled

    CellmlElementItem *cellmlElementItem = static_cast<CellmlElementItem *>(mCellmlDataModel->itemFromIndex(pIndex));

    QStyleOptionViewItemV4 option(pOption);

    initStyleOption(&option, pIndex);

    if (   (cellmlElementItem->type() == CellmlElementItem::Error)
        || (cellmlElementItem->type() == CellmlElementItem::Warning)
        || cellmlElementItem->isCategory()) {
        // This is an error/warning/category item, so prevent it from being
        // hoverable. Otherwise, show the item enabled since it's actually
        // disabled (so we can't select it), yet we want to see as if it was
        // enabled...

        option.state &= ~QStyle::State_MouseOver;
        option.state |=  QStyle::State_Enabled;

        // In the case of a category, we make the font bold

        option.font.setBold(cellmlElementItem->isCategory());
    }

    QStyledItemDelegate::paint(pPainter, option, pIndex);
}

//==============================================================================

CellmlElementItem::CellmlElementItem(const bool &pError, const QString &pText) :
    QStandardItem(pText),
    mCategory(false),
    mType(pError?Error:Warning),
    mElement(0)
{
    // Constructor for either an error or a warning

    // Disable the item and use its text as a tooltip (in case it's too long and
    // therefore doesn't fit within the allocated space we have)
    // Note: the item will get 're-enabled' by our item delegate...

    setEnabled(false);
    setToolTip(pText);

    // Set the icon for the item

    setIcon(mType);
}

//==============================================================================

CellmlElementItem::CellmlElementItem(const Type &pType,
                                     const QString &pText) :
    QStandardItem(pText),
    mCategory(true),
    mType(pType),
    mElement(0)
{
    // Constructor for a category

    // Disable the item
    // Note: the item will get 're-enabled' by our item delegate...

    setEnabled(false);

    // Set the icon for the item

    setIcon(pType);
}

//==============================================================================

CellmlElementItem::CellmlElementItem(const Type &pType,
                                     CellMLSupport::CellmlFileElement *pElement,
                                     const QString &pText) :
    QStandardItem(pText),
    mCategory(false),
    mType(pType),
    mElement(pElement)
{
    static const QChar rightArrow = QChar(0x2192);

    // Set the text for some types

    switch (pType) {
    case Import:
        setText(static_cast<CellMLSupport::CellmlFileImport *>(pElement)->xlinkHref());

        break;
    case RelationshipReference:
        setText(static_cast<CellMLSupport::CellmlFileRelationshipReference *>(pElement)->relationship());

        break;
    case ComponentReference:
        setText(static_cast<CellMLSupport::CellmlFileComponentReference *>(pElement)->component());

        break;
    case ComponentMapping: {
        CellMLSupport::CellmlFileMapComponents *mapComponents = static_cast<CellMLSupport::CellmlFileMapComponents *>(pElement);

        setText(QString("%1 %2 %3").arg(mapComponents->firstComponent(),
                                        rightArrow,
                                        mapComponents->secondComponent()));

        break;
    }
    case VariableMapping: {
        CellMLSupport::CellmlFileMapVariablesItem *mapVariables = static_cast<CellMLSupport::CellmlFileMapVariablesItem *>(pElement);

        setText(QString("%1 %2 %3").arg(mapVariables->firstVariable(),
                                        rightArrow,
                                        mapVariables->secondVariable()));

        break;
    }
    default:
        // Another type of element, so either pText was used or it has a name
        // which we can use

        if (pText.isEmpty())
            setText(static_cast<CellMLSupport::CellmlFileNamedElement *>(pElement)->name());
    }

    // Set the icon for the item

    setIcon(pType);
}

//==============================================================================

void CellmlElementItem::setIcon(const Type &pType)
{
    // Determine the icon to be used for the item

    switch (pType) {
    case Error:
        QStandardItem::setIcon(QIcon(":CellMLSupport_errorNode"));

        break;
    case Warning:
        QStandardItem::setIcon(QIcon(":CellMLSupport_warningNode"));

        break;
    case Model:
        QStandardItem::setIcon(QIcon(":CellMLSupport_modelNode"));

        break;
    case Import:
        QStandardItem::setIcon(QIcon(":CellMLSupport_importNode"));

        break;
    case ImportUnit:
    case Unit:
        QStandardItem::setIcon(QIcon(":CellMLSupport_unitNode"));

        break;
    case UnitElement:
        QStandardItem::setIcon(QIcon(":CellMLSupport_unitElementNode"));

        break;
    case ImportComponent:
    case Component:
    case ComponentMapping:
        QStandardItem::setIcon(QIcon(":CellMLSupport_componentNode"));

        break;
    case Variable:
    case VariableMapping:
        QStandardItem::setIcon(QIcon(":CellMLSupport_variableNode"));

        break;
    case Group:
        QStandardItem::setIcon(QIcon(":CellMLSupport_groupNode"));

        break;
    case RelationshipReference:
        QStandardItem::setIcon(QIcon(":CellMLSupport_relationshipReferenceNode"));

        break;
    case ComponentReference:
        QStandardItem::setIcon(QIcon(":CellMLSupport_componentReferenceNode"));

        break;
    case Connection:
        QStandardItem::setIcon(QIcon(":CellMLSupport_connectionNode"));

        break;
    default:
        // Type which doesn't require an icon, so...

        ;
    }
}

//==============================================================================

bool CellmlElementItem::isCategory() const
{
    // Return wether the CellML element item is a category or not

    return mCategory;
}

//==============================================================================

int CellmlElementItem::type() const
{
    // Return the CellML element item's type

    return mType;
}

//==============================================================================

CellMLSupport::CellmlFileElement * CellmlElementItem::element() const
{
    // Return the CellML element item's element

    return mElement;
}

//==============================================================================

CellmlAnnotationViewWidget::CellmlAnnotationViewWidget(QWidget *pParent,
                                                       const QString &pFileName) :
    Widget(pParent),
    mGui(new Ui::CellmlAnnotationViewWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Retrieve and load the corresponding CellML file

    mCellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);

    mCellmlFile->load();

    // Create some splitters

    mHorizontalSplitter = new QSplitter(Qt::Horizontal, this);
    mVerticalSplitter   = new QSplitter(Qt::Vertical, mHorizontalSplitter);

    // Create and customise our CellML tree view which will contain all of the
    // imports, units, components, groups and connections from the CellML file

    mCellmlTreeView     = new Core::TreeView(mVerticalSplitter);
    mCellmlDataModel    = new QStandardItemModel(mCellmlTreeView);
    mCellmlItemDelegate = new CellmlItemDelegate(mCellmlTreeView,
                                                 mCellmlDataModel);

    initTreeView(mCellmlTreeView, mCellmlDataModel, mCellmlItemDelegate);

    // Create and customise our metadata tree view which will contain all of the
    // metadata from the CellML file

    mMetadataTreeView  = new Core::TreeView(mVerticalSplitter);
    mMetadataDataModel = new QStandardItemModel(mMetadataTreeView);

    initTreeView(mMetadataTreeView, mMetadataDataModel);

    // Populate our splitters with the aforementioned tree views, as well as
    // with a details widget

    mVerticalSplitter->addWidget(new Core::BorderedWidget(mCellmlTreeView,
                                                          false, false, true, true));
    mVerticalSplitter->addWidget(new Core::BorderedWidget(mMetadataTreeView,
                                                          true, false, false, true));

    mDetails = new CellmlAnnotationViewDetailsWidget(this, mCellmlFile);

    mHorizontalSplitter->addWidget(mVerticalSplitter);
    mHorizontalSplitter->addWidget(new Core::BorderedWidget(mDetails,
                                                            false, true, false, false));

    mGui->verticalLayout->addWidget(mHorizontalSplitter);

    // We want a context menu for our CellML tree view

    mCellmlTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(mCellmlTreeView, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(cellmlTreeViewContextMenu(const QPoint &)));

    // Keep track of our splitters being moved

    connect(mHorizontalSplitter, SIGNAL(splitterMoved(int,int)),
            this, SLOT(emitHorizontalSplitterMoved()));
    connect(mVerticalSplitter, SIGNAL(splitterMoved(int,int)),
            this, SLOT(emitVerticalSplitterMoved()));

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

    // Expand our CellML tree view enough so that we can see the meaningful
    // parts of the CellML file

    mCellmlTreeView->expandToDepth(1);

    // Resize our tree views, just to be on the safe side

    resizeCellmlTreeViewToContents();
    mMetadataTreeView->resizeColumnToContents(0);

    // Select the first CellML node, if any

    if (mCellmlDataModel->invisibleRootItem()->rowCount())
        mCellmlTreeView->setCurrentIndex(mCellmlDataModel->invisibleRootItem()->child(0)->index());
}

//==============================================================================

CellmlAnnotationViewWidget::~CellmlAnnotationViewWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewWidget::retranslateUi()
{
    // Retranslate some of the nodes in the CellML tree view

    retranslateCellmlDataItem(static_cast<CellmlElementItem *>(mCellmlDataModel->invisibleRootItem()));

    // Retranslate the details widget

    mDetails->retranslateUi();
}

//==============================================================================

void CellmlAnnotationViewWidget::retranslateCellmlDataItem(CellmlElementItem *pCellmlElementItem)
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
            pCellmlElementItem->setText(tr("Group #%1").arg(pCellmlElementItem->text().remove(QRegExp("^[^#]+#"))));

            break;
        case CellmlElementItem::Connection:
            pCellmlElementItem->setText(tr("Connection #%1").arg(pCellmlElementItem->text().remove(QRegExp("^[^#]+#"))));

            break;
        default:
            // Not a sub-type we can retranslate, so do nothing...

            ;
        }
}

//==============================================================================

QList<int> CellmlAnnotationViewWidget::horizontalSplitterSizes() const
{
    // Return our horizontal splitter's sizes

    return mHorizontalSplitter->sizes();
}

//==============================================================================

QList<int> CellmlAnnotationViewWidget::verticalSplitterSizes() const
{
    // Return our vertical splitter's sizes

    return mVerticalSplitter->sizes();
}

//==============================================================================

void CellmlAnnotationViewWidget::initTreeView(Core::TreeView *pTreeView,
                                              QStandardItemModel *pDataModel,
                                              CellmlItemDelegate *pItemDelegate)
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

void CellmlAnnotationViewWidget::populateCellmlDataModel()
{
    // Make sure that the CellML file was properly loaded

    if (mCellmlFile->issues().count()) {
        // Something went wrong while trying to load the CellML file, so report
        // it and leave

        mCellmlDataModel->invisibleRootItem()->appendRow(new CellmlElementItem(mCellmlFile->issues().first().type() == CellMLSupport::CellmlFileIssue::Error,
                                                                               mCellmlFile->issues().first().formattedMessage()));

        return;
    }

    // Retrieve the model's root

    CellmlElementItem *modelItem = new CellmlElementItem(CellmlElementItem::Model,
                                                         mCellmlFile->model());

    mCellmlDataModel->invisibleRootItem()->appendRow(modelItem);

    // Retrieve the model's imports

    if (mCellmlFile->imports().count()) {
        // Imports category

        CellmlElementItem *importsItem = new CellmlElementItem(CellmlElementItem::Import,
                                                               tr("Imports"));

        modelItem->appendRow(importsItem);

        // Retrieve the model's imports themselves

        foreach (CellMLSupport::CellmlFileImport *import,
                 mCellmlFile->imports()) {
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

    populateUnitsDataModel(modelItem, mCellmlFile->units());

    // Retrieve the model's components

    if (mCellmlFile->components().count()) {
        // Components category

        CellmlElementItem *componentsItem = new CellmlElementItem(CellmlElementItem::Component,
                                                                  tr("Components"));

        modelItem->appendRow(componentsItem);

        // Retrieve the model's components themselves

        foreach (CellMLSupport::CellmlFileComponent *component,
                 mCellmlFile->components()) {
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

    if (mCellmlFile->groups().count()) {
        // Groups category

        CellmlElementItem *groupsItem = new CellmlElementItem(CellmlElementItem::Group,
                                                              tr("Groups"));

        modelItem->appendRow(groupsItem);

        // Retrieve the model's groups themselves

        int counter = 0;

        foreach (CellMLSupport::CellmlFileGroup *group, mCellmlFile->groups()) {
            // A model's group

            CellmlElementItem *groupItem = new CellmlElementItem(CellmlElementItem::Group,
                                                                 group,
                                                                 tr("Group #%1").arg(QString::number(++counter)));

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

    if (mCellmlFile->connections().count()) {
        // Connections category

        CellmlElementItem *connectionsItem = new CellmlElementItem(CellmlElementItem::Connection,
                                                                   tr("Connections"));

        modelItem->appendRow(connectionsItem);

        // Retrieve the model's connections themselves

        int counter = 0;

        foreach (CellMLSupport::CellmlFileConnection *connection, mCellmlFile->connections()) {
            // A model's connection

            CellmlElementItem *connectionItem = new CellmlElementItem(CellmlElementItem::Connection,
                                                                      connection,
                                                                      tr("Connection #%1").arg(QString::number(++counter)));

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

void CellmlAnnotationViewWidget::populateUnitsDataModel(CellmlElementItem *pCellmlElementItem,
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

void CellmlAnnotationViewWidget::populateComponentReferenceDataModel(CellmlElementItem *pCellmlElementItem,
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

void CellmlAnnotationViewWidget::populateMetadataDataModel()
{
    // Make sure that the CellML file was properly loaded

    if (mCellmlFile->issues().count())
        // There are issues, so...

        return;

    // Retrieve the name of the different groups of triples

    QString uriBase = mCellmlFile->uriBase();
    QList<QString> groupNames = QList<QString>();

    foreach (CellMLSupport::CellmlFileRdfTriple *rdfTriple,
             mCellmlFile->metadata())
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
            // Note: we want the name of the group to be the same as that of the
            //       cmeta:id of a CellML element. This means that we must
            //       remove the URI base (and hash character) which makes the
            //       beginning of the RDF triple's subject's URI reference...

            QString groupName = rdfTriple->subject()->uriReference().remove(QRegExp("^"+QRegExp::escape(uriBase)+"#?"));

            if (!groupNames.contains(groupName)) {
                // The group hasn't already been added, so add it and keep track
                // of it

                QStandardItem *metadataItem = new QStandardItem(groupName);

                metadataItem->setIcon(QIcon(":CellMLSupport_metadataNode"));

                mMetadataDataModel->invisibleRootItem()->appendRow(metadataItem);

                groupNames.append(groupName);
            }
        }

    // Sort the group names

    mMetadataDataModel->sort(0);
}

//==============================================================================

void CellmlAnnotationViewWidget::updateHorizontalSplitter(const QList<int> &pHorizontalSizes)
{
    // Update our horizontal splitter after the horizontal splitter of another
    // CellmlAnnotationViewWidget object has been moved

    mHorizontalSplitter->setSizes(pHorizontalSizes);
}

//==============================================================================

void CellmlAnnotationViewWidget::updateVerticalSplitter(const QList<int> &pVerticalSizes)
{
    // Update our vertical splitter after the vertical splitter of another
    // CellmlAnnotationViewWidget object has been moved

    mVerticalSplitter->setSizes(pVerticalSizes);
}

//==============================================================================

void CellmlAnnotationViewWidget::emitHorizontalSplitterMoved()
{
    // Let whoever know that our horizontal splitter has been moved

    emit horizontalSplitterMoved(mHorizontalSplitter->sizes());
}

//==============================================================================

void CellmlAnnotationViewWidget::emitVerticalSplitterMoved()
{
    // Let whoever know that our vertical splitter has been moved

    emit verticalSplitterMoved(mVerticalSplitter->sizes());
}

//==============================================================================

void CellmlAnnotationViewWidget::resizeCellmlTreeViewToContents()
{
    // Resize our CellML tree view so that its contents is visible

    mCellmlTreeView->resizeColumnToContents(0);
}

//==============================================================================

void CellmlAnnotationViewWidget::updateCellmlNode(const QModelIndex &pNewIndex,
                                                  const QModelIndex &pOldIndex)
{
    Q_UNUSED(pOldIndex);

    // Retrieve all the CellML items which properties we want to add to the
    // details GUI

    CellmlAnnotationViewDetailsWidget::CellmlItems cellmlItems = CellmlAnnotationViewDetailsWidget::CellmlItems();
    QModelIndex crtIndex = pNewIndex;

    do {
        CellmlElementItem *crtCellmlElementItem = static_cast<CellmlElementItem *>(mCellmlDataModel->itemFromIndex(crtIndex));

        // Add the item based on its type, but only if it's not a category

        if (!crtCellmlElementItem->isCategory())
            switch (crtCellmlElementItem->type()) {
            case CellmlElementItem::Model:
                cellmlItems << CellmlAnnotationViewDetailsWidget::cellmlItem(CellmlAnnotationViewDetailsWidget::Model,
                                                                             crtCellmlElementItem->element());

                break;
            case CellmlElementItem::Import:
                cellmlItems << CellmlAnnotationViewDetailsWidget::cellmlItem(CellmlAnnotationViewDetailsWidget::Import,
                                                                             crtCellmlElementItem->element());

                break;
            case CellmlElementItem::ImportUnit:
                cellmlItems << CellmlAnnotationViewDetailsWidget::cellmlItem(CellmlAnnotationViewDetailsWidget::ImportUnit,
                                                                             crtCellmlElementItem->element());

                break;
            case CellmlElementItem::ImportComponent:
                cellmlItems << CellmlAnnotationViewDetailsWidget::cellmlItem(CellmlAnnotationViewDetailsWidget::ImportComponent,
                                                                             crtCellmlElementItem->element());

                break;
            case CellmlElementItem::Unit:
                cellmlItems << CellmlAnnotationViewDetailsWidget::cellmlItem(CellmlAnnotationViewDetailsWidget::Unit,
                                                                             crtCellmlElementItem->element());

                break;
            case CellmlElementItem::UnitElement:
                cellmlItems << CellmlAnnotationViewDetailsWidget::cellmlItem(CellmlAnnotationViewDetailsWidget::UnitElement,
                                                                             crtCellmlElementItem->element());

                break;
            case CellmlElementItem::Component:
                cellmlItems << CellmlAnnotationViewDetailsWidget::cellmlItem(CellmlAnnotationViewDetailsWidget::Component,
                                                                             crtCellmlElementItem->element());

                break;
            case CellmlElementItem::Variable:
                cellmlItems << CellmlAnnotationViewDetailsWidget::cellmlItem(CellmlAnnotationViewDetailsWidget::Variable,
                                                                             crtCellmlElementItem->element());

                break;
            case CellmlElementItem::Group:
                cellmlItems << CellmlAnnotationViewDetailsWidget::cellmlItem(CellmlAnnotationViewDetailsWidget::Group,
                                                                             crtCellmlElementItem->element(),
                                                                             crtCellmlElementItem->text());

                break;
            case CellmlElementItem::RelationshipReference:
                cellmlItems << CellmlAnnotationViewDetailsWidget::cellmlItem(CellmlAnnotationViewDetailsWidget::RelationshipReference,
                                                                             crtCellmlElementItem->element());

                break;
            case CellmlElementItem::ComponentReference:
                cellmlItems << CellmlAnnotationViewDetailsWidget::cellmlItem(CellmlAnnotationViewDetailsWidget::ComponentReference,
                                                                             crtCellmlElementItem->element());

                break;
            case CellmlElementItem::Connection:
                cellmlItems << CellmlAnnotationViewDetailsWidget::cellmlItem(CellmlAnnotationViewDetailsWidget::Connection,
                                                                             crtCellmlElementItem->element(),
                                                                             crtCellmlElementItem->text());

                break;
            case CellmlElementItem::ComponentMapping:
                cellmlItems << CellmlAnnotationViewDetailsWidget::cellmlItem(CellmlAnnotationViewDetailsWidget::ComponentMapping,
                                                                             crtCellmlElementItem->element());

                break;
            case CellmlElementItem::VariableMapping:
                cellmlItems << CellmlAnnotationViewDetailsWidget::cellmlItem(CellmlAnnotationViewDetailsWidget::VariableMapping,
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

    int cellmlItemsCount = cellmlItems.count();

    for (int i = 0, iMax = cellmlItemsCount >> 1; i < iMax; ++i)
        cellmlItems.swap(i, cellmlItemsCount-(i+1));

    // Update the details GUI

    mDetails->updateGui(cellmlItems);

    // Set the view widget's focus proxy to the widget which the details GUI
    // thinks should be the focus proxy widget

    setFocusProxy(mDetails->focusProxyWidget());
}

//==============================================================================

void CellmlAnnotationViewWidget::addRdfTriple(CellMLSupport::CellmlFileRdfTriples &pRdfTriples,
                                              CellMLSupport::CellmlFileRdfTriple *pRdfTriple)
{
    // Add pRdfTriple to pRdfTriples

    pRdfTriples.insertMulti(pRdfTriple->subject()->asString(), pRdfTriple);

    // Recursively add all the RDF triples which subject matches that of
    // pRdfTriple's object

    foreach (CellMLSupport::CellmlFileRdfTriple *rdfTriple,
             mCellmlFile->metadata().values(pRdfTriple->object()->asString()))
        addRdfTriple(pRdfTriples, rdfTriple);
}

//==============================================================================

void CellmlAnnotationViewWidget::updateMetadataNode(const QModelIndex &pNewIndex,
                                                    const QModelIndex &pOldIndex)
{
    Q_UNUSED(pOldIndex);

    // Determine the RDF triples that we need to show in the details GUI

    CellMLSupport::CellmlFileRdfTriples rdfTriples = CellMLSupport::CellmlFileRdfTriples();

    QString metadataGroupName = mMetadataDataModel->itemFromIndex(pNewIndex)->text();
    QString uriBase = mCellmlFile->uriBase();

    foreach (CellMLSupport::CellmlFileRdfTriple *rdfTriple,
             mCellmlFile->metadata())
        // Retrieve the RDF triple's subject so we can determine whether it's
        // from the group of RDF triples in which we are interested

        if (rdfTriple->subject()->type() == CellMLSupport::CellmlFileRdfTripleElement::UriReference)
            // We have an RDF triple of which we can make sense, so retrieve its
            // group name

            if (!metadataGroupName.compare(rdfTriple->subject()->uriReference().remove(QRegExp("^"+QRegExp::escape(uriBase)+"#?"))))
                // It's the correct group name, so add it to our list

                addRdfTriple(rdfTriples, rdfTriple);

    // Update the details GUI

    mDetails->updateGui(rdfTriples);
}

//==============================================================================

void CellmlAnnotationViewWidget::cellmlTreeViewContextMenu(const QPoint &pPosition) const
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

void CellmlAnnotationViewWidget::on_actionExpandAll_triggered()
{
    // Expand all the nodes below the current one
    // Note: we disable and then re-enable updates before expanding all the
    //       index since it may end up in quite a few updates...

    mCellmlTreeView->setUpdatesEnabled(false);
        indexExpandAll(mCellmlTreeView->currentIndex());
    mCellmlTreeView->setUpdatesEnabled(true);
}

//==============================================================================

void CellmlAnnotationViewWidget::on_actionCollapseAll_triggered()
{
    // Collapse all the nodes below the current one
    // Note: see the note in on_actionExpandAll_triggered() above...

    mCellmlTreeView->setUpdatesEnabled(false);
        indexCollapseAll(mCellmlTreeView->currentIndex());
    mCellmlTreeView->setUpdatesEnabled(true);
}

//==============================================================================

void CellmlAnnotationViewWidget::indexExpandAll(const QModelIndex &pIndex) const
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

void CellmlAnnotationViewWidget::indexCollapseAll(const QModelIndex &pIndex) const
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

bool CellmlAnnotationViewWidget::indexIsAllExpanded(const QModelIndex &pIndex) const
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

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
