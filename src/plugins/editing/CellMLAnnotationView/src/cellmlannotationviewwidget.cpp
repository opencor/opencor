//==============================================================================
// CellML annotation view widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmlannotationviewdetailswidget.h"
#include "cellmlannotationviewwidget.h"
#include "cellmlfilemanager.h"
#include "coreutils.h"
#include "treeview.h"

//==============================================================================

#include "ui_cellmlannotationviewwidget.h"

//==============================================================================

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
        || (cellmlElementItem->type() == CellmlElementItem::Category)) {
        // This is an error/warning/category item, so prevent it from being
        // hoverable. Otherwise, show the item enabled since it's actually
        // disabled (so we can't select it), yet we want to see as if it was
        // enabled...

        option.state &= ~QStyle::State_MouseOver;
        option.state |=  QStyle::State_Enabled;

        // In the case of a category, we make the font bold

        if (cellmlElementItem->type() == CellmlElementItem::Category)
            option.font.setBold(true);
    }

    QStyledItemDelegate::paint(pPainter, option, pIndex);
}

//==============================================================================

CellmlElementItem::CellmlElementItem(CellMLSupport::CellmlFileModel *pModel) :
    QStandardItem(pModel->name()),
    mType(Model),
    mSubType(None)
{
    initialize(mType, mType, text(), pModel);
}

//==============================================================================

CellmlElementItem::CellmlElementItem(CellMLSupport::CellmlFileImport *pImport) :
    QStandardItem(pImport->uri()),
    mType(Import),
    mSubType(None)
{
    initialize(mType, mType, text(), pImport);
}

//==============================================================================

CellmlElementItem::CellmlElementItem(CellMLSupport::CellmlFileUnit *pUnit) :
    QStandardItem(pUnit->name()),
    mType(Unit),
    mSubType(None)
{
    initialize(mType, mType, text(), pUnit);
}

//==============================================================================

CellmlElementItem::CellmlElementItem(CellMLSupport::CellmlFileUnitElement *pUnitElement) :
    QStandardItem(pUnitElement->name()),
    mType(UnitElement),
    mSubType(None)
{
    initialize(mType, mType, text(), pUnitElement);
}

//==============================================================================

CellmlElementItem::CellmlElementItem(CellMLSupport::CellmlFileComponent *pComponent) :
    QStandardItem(pComponent->name()),
    mType(Component),
    mSubType(None)
{
    initialize(mType, mType, text(), pComponent);
}

//==============================================================================

CellmlElementItem::CellmlElementItem(const Type &pType, const QString &pText) :
    QStandardItem(pText),
    mType(pType),
    mSubType(None)
{
    initialize(pType, pType, pText);
}

//==============================================================================

CellmlElementItem::CellmlElementItem(const Type &pType, const Type &pSubType,
                                     const QString &pText) :
    QStandardItem(pText),
    mType(pType),
    mSubType(pSubType)
{
    initialize(pType, pSubType, pText);
}

//==============================================================================

void CellmlElementItem::initialize(const Type &pType, const Type &pSubType,
                                   const QString &pText,
                                   CellMLSupport::CellmlFileElement *pElement)
{
    // Some settings for the Error, Warning and Category types. In the case of
    // an Error or Warning type, we use their text as a tooltip (in case it's
    // too long and therefore doesn't fit within the allocated space we have).
    // Otherwise, in all the types we handle, the item is disabled though it
    // will get 're-enabled' by our item delegate...

    switch (pType) {
    case Error:
    case Warning:
        setToolTip(pText);
    case Category:
        setEnabled(false);

        break;
    default:
        // Type which we don't handle

        break;
    }

    // Determine the icon to be used for the item

    switch (pSubType) {
    case Error:
        setIcon(QIcon(":CellMLSupport_errorNode"));

        break;
    case Warning:
        setIcon(QIcon(":CellMLSupport_warningNode"));

        break;
    case Model:
        setIcon(QIcon(":CellMLSupport_modelNode"));

        break;
    case Import:
        setIcon(QIcon(":CellMLSupport_importNode"));

        break;
    case Unit:
        setIcon(QIcon(":CellMLSupport_unitNode"));

        break;
    case UnitElement:
        setIcon(QIcon(":CellMLSupport_unitElementNode"));

        break;
    case Component:
        setIcon(QIcon(":CellMLSupport_componentNode"));

        break;
    case Variable:
        setIcon(QIcon(":CellMLSupport_variableNode"));

        break;
    case MathmlElement:
        setIcon(QIcon(":CellMLSupport_mathmlElementNode"));

        break;
    case Group:
        setIcon(QIcon(":CellMLSupport_groupNode"));

        break;
    case RelationshipRef:
        setIcon(QIcon(":CellMLSupport_relationshipRefNode"));

        break;
    case ComponentRef:
        setIcon(QIcon(":CellMLSupport_componentRefNode"));

        break;
    case Connection:
        setIcon(QIcon(":CellMLSupport_connectionNode"));

        break;
    case Metadata:
        setIcon(QIcon(":CellMLSupport_metadataNode"));

        break;
    default:
        // Sub-type which doesn't require an icon

        break;
    }

    // CellML element

    mElement = pElement;
}

//==============================================================================

CellMLSupport::CellmlFileElement * CellmlElementItem::element() const
{
    // Return the CellML element item's element

    return mElement;
}

//==============================================================================

int CellmlElementItem::type() const
{
    // Return the CellML element item's type

    return mType;
}

//==============================================================================

int CellmlElementItem::subType() const
{
    // Return the CellML element item's sub-type

    return mSubType;
}

//==============================================================================

CellmlAnnotationViewWidget::CellmlAnnotationViewWidget(QWidget *pParent,
                                                       const QString &pFileName) :
    Widget(pParent),
    mUi(new Ui::CellmlAnnotationViewWidget)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create some splitters

    mHorizontalSplitter = new QSplitter(Qt::Horizontal, this);
    mVerticalSplitter   = new QSplitter(Qt::Vertical, mHorizontalSplitter);

    // Create and customise our CellML tree view which will contain all of the
    // imports, units, components, groups and connections from the CellML file

    mCellmlTreeView = new Core::TreeView(mVerticalSplitter);
    mCellmlDataModel = new QStandardItemModel(mCellmlTreeView);
    mCellmlItemDelegate = new CellmlItemDelegate(mCellmlTreeView,
                                                 mCellmlDataModel);

    mCellmlTreeView->setModel(mCellmlDataModel);
    mCellmlTreeView->setItemDelegate(mCellmlItemDelegate);

    initTreeView(mCellmlTreeView);

    // Create and customise our metadata tree view which will contain all of the
    // metadata from the CellML file

    mMetadataTreeView = new Core::TreeView(mVerticalSplitter);
    mMetadataDataModel = new QStandardItemModel(mMetadataTreeView);

    mMetadataTreeView->setModel(mMetadataDataModel);

    initTreeView(mMetadataTreeView);

    // Populate our splitters with the aforementioned tree views, as well as
    // with a details widget

    mVerticalSplitter->addWidget(new Core::BorderedWidget(mCellmlTreeView,
                                                          false, false, true, true));
    mVerticalSplitter->addWidget(new Core::BorderedWidget(mMetadataTreeView,
                                                          true, false, false, true));

    mDetails = new CellmlAnnotationViewDetailsWidget(this);

    mHorizontalSplitter->addWidget(mVerticalSplitter);
    mHorizontalSplitter->addWidget(new Core::BorderedWidget(mDetails,
                                                            false, true, false, false));

    mUi->verticalLayout->addWidget(mHorizontalSplitter);

    // Keep track of our splitters being moved

    connect(mHorizontalSplitter, SIGNAL(splitterMoved(int,int)),
            this, SLOT(emitHorizontalSplitterMoved()));
    connect(mVerticalSplitter, SIGNAL(splitterMoved(int,int)),
            this, SLOT(emitVerticalSplitterMoved()));

    // Check what should be used to represent a right arrow character

    mRightArrow = QChar(0x2192);

    if (!QRawFont::fromFont(mCellmlTreeView->font()).supportsCharacter(mRightArrow))
        mRightArrow = QChar('>');

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

    populateCellmlDataModel(pFileName);
    populateMetadataDataModel(pFileName);

    // Expand our CellML tree view enough so that we can see the meaningful
    // parts of the CellML file

    mCellmlTreeView->expandToDepth(1);

    // Resize our tree views, just to be on the safe side

    resizeCellmlTreeViewToContents();
    mMetadataTreeView->resizeColumnToContents(0);
}

//==============================================================================

CellmlAnnotationViewWidget::~CellmlAnnotationViewWidget()
{
    // Delete the UI

    delete mUi;
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

    // Check whether the current node needs retranslating

    switch (pCellmlElementItem->type()) {
    case CellmlElementItem::Category:
        // We are dealing with a category, so we need to check its sub-type to
        // know how to retranslate it

        switch (pCellmlElementItem->subType()) {
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
        case CellmlElementItem::MathmlElement:
            pCellmlElementItem->setText(tr("MathML Elements"));

            break;
        case CellmlElementItem::Group:
            pCellmlElementItem->setText(tr("Groups"));

            break;
        case CellmlElementItem::RelationshipRef:
            pCellmlElementItem->setText(tr("Relationship References"));

            break;
        case CellmlElementItem::ComponentRef:
            pCellmlElementItem->setText(tr("Component References"));

            break;
        case CellmlElementItem::Connection:
            pCellmlElementItem->setText(tr("Connections"));

            break;
        default:
            // Not a sub-type we can retranslate, so do nothing...

            break;
        }

        break;
    case CellmlElementItem::MathmlElement:
        pCellmlElementItem->setText(tr("MathML element #%1").arg(pCellmlElementItem->text().remove(QRegExp("^[^#]+#"))));

        break;
    case CellmlElementItem::Group:
        pCellmlElementItem->setText(tr("Group #%1").arg(pCellmlElementItem->text().remove(QRegExp("^[^#]+#"))));

        break;
    case CellmlElementItem::Connection:
        pCellmlElementItem->setText(tr("Connection #%1").arg(pCellmlElementItem->text().remove(QRegExp("^[^#]+#"))));

        break;
    default:
        // Not a sub-type we can retranslate, so do nothing...

        break;
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

void CellmlAnnotationViewWidget::initTreeView(Core::TreeView *pTreeView)
{
    pTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pTreeView->setFrameShape(QFrame::NoFrame);
    pTreeView->setHeaderHidden(true);
    pTreeView->setRootIsDecorated(false);
    pTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
}

//==============================================================================

void CellmlAnnotationViewWidget::populateCellmlDataModel(const QString &pFileName)
{
    // Retrieve our CellML file object and load the CellML file

    CellMLSupport::CellmlFile *cellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);

    cellmlFile->load();

    if (cellmlFile->issues().count()) {
        // Something went wrong while trying to load the CellML file, so report
        // it and leave

        mCellmlDataModel->invisibleRootItem()->appendRow(new CellmlElementItem((cellmlFile->issues().first().type() == CellMLSupport::CellmlFileIssue::Error)?CellmlElementItem::Error:CellmlElementItem::Warning,
                                                                               cellmlFile->issues().first().formattedMessage()));

        return;
    }

    // Retrieve the model's root

    CellmlElementItem *modelItem = new CellmlElementItem(cellmlFile->model());

    mCellmlDataModel->invisibleRootItem()->appendRow(modelItem);

    // Retrieve the model's imports

    if (cellmlFile->imports().count()) {
        // Imports category

        CellmlElementItem *importsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                               CellmlElementItem::Import,
                                                               tr("Imports"));

        modelItem->appendRow(importsItem);

        // Retrieve the model's imports themselves

        foreach (CellMLSupport::CellmlFileImport *import,
                 cellmlFile->imports()) {
            // A model import

            CellmlElementItem *importItem = new CellmlElementItem(import);

            importsItem->appendRow(importItem);

            // Retrieve the model's import's units

            if (import->units().count()) {
                // Units category

                CellmlElementItem *unitsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                                     CellmlElementItem::Unit,
                                                                     tr("Units"));

                importItem->appendRow(unitsItem);

                // Retrieve the model's import's units themselves

                foreach (CellMLSupport::CellmlFileImportUnit *unit,
                         import->units())
                    // A model's import's unit

                    unitsItem->appendRow(new CellmlElementItem(unit));
            }

            // Retrieve the model's import's components

            if (import->components().count()) {
                // Components category

                CellmlElementItem *componentsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                                          CellmlElementItem::Component,
                                                                          tr("Components"));

                importItem->appendRow(componentsItem);

                // Retrieve the model's import's components themselves

                foreach (CellMLSupport::CellmlFileImportComponent *component,
                         import->components())
                    // A model's import's component

                    componentsItem->appendRow(new CellmlElementItem(CellmlElementItem::Component,
                                                                    component->name()));
            }
        }
    }

    // Retrieve the model's units

    populateUnitsDataModel(modelItem, cellmlFile->units());

    // Retrieve the model's components

    if (cellmlFile->components().count()) {
        // Components category

        CellmlElementItem *componentsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                                  CellmlElementItem::Component,
                                                                  tr("Components"));

        modelItem->appendRow(componentsItem);

        // Retrieve the model's components themselves

        foreach (CellMLSupport::CellmlFileComponent *component,
                 cellmlFile->components()) {
            // A model's component

            CellmlElementItem *componentItem = new CellmlElementItem(component);

            componentsItem->appendRow(componentItem);

            // Retrieve the model's component's units

            populateUnitsDataModel(componentItem, component->units());

            // Retrieve the model's component's variables

            if (component->variables().count()) {
                // Variables category

                CellmlElementItem *variablesItem = new CellmlElementItem(CellmlElementItem::Category,
                                                                         CellmlElementItem::Variable,
                                                                         tr("Variables"));

                componentItem->appendRow(variablesItem);

                // Retrieve the model's component's variables themselves

                foreach (CellMLSupport::CellmlFileVariable *variable,
                         component->variables())
                    variablesItem->appendRow(new CellmlElementItem(CellmlElementItem::Variable,
                                                                   variable->name()));
            }

            // Retrieve the model's component's MathML elements

            if (component->mathmlElements().count()) {
                // MathML elements category

                CellmlElementItem *mathmlElementsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                                              CellmlElementItem::MathmlElement,
                                                                              tr("MathML Elements"));

                componentItem->appendRow(mathmlElementsItem);

                // Retrieve the model's component's MathML elements themselves

                for (int i = 1, iMax = component->mathmlElements().count(); i <= iMax; ++i)
                    mathmlElementsItem->appendRow(new CellmlElementItem(CellmlElementItem::MathmlElement,
                                                                        tr("MathML element #%1").arg(QString::number(i))));
            }
        }
    }

    // Retrieve the model's groups

    if (cellmlFile->groups().count()) {
        // Groups category

        CellmlElementItem *groupsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                              CellmlElementItem::Group,
                                                              tr("Groups"));

        modelItem->appendRow(groupsItem);

        // Retrieve the model's groups themselves

        int counter = 0;

        foreach (CellMLSupport::CellmlFileGroup *group, cellmlFile->groups()) {
            // A model's group

            CellmlElementItem *groupItem = new CellmlElementItem(CellmlElementItem::Group,
                                                                 tr("Group #%1").arg(QString::number(++counter)));

            groupsItem->appendRow(groupItem);

            // Retrieve the model's group's relationship ref(erence)s

            if (group->relationshipRefs().count()) {
                // Relationship ref(erence)s category

                CellmlElementItem *relationshipRefsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                                                CellmlElementItem::RelationshipRef,
                                                                                tr("Relationship References"));

                groupItem->appendRow(relationshipRefsItem);

                // Retrieve the model's group's relationship ref(erence)s
                // themselves

                foreach (CellMLSupport::CellmlFileRelationshipRef *relationshipRef,
                         group->relationshipRefs())
                    relationshipRefsItem->appendRow(new CellmlElementItem(CellmlElementItem::RelationshipRef,
                                                                          relationshipRef->relationship()));
            }

            // Retrieve the model's group's component ref(erence)s

            if (group->componentRefs().count()) {
                // Component ref(erence)s category

                CellmlElementItem *componentRefsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                                             CellmlElementItem::ComponentRef,
                                                                             tr("Component References"));

                groupItem->appendRow(componentRefsItem);

                // Retrieve the model's group's relationship ref(erence)s
                // themselves

                foreach (CellMLSupport::CellmlFileComponentRef *componentRef,
                         group->componentRefs())
                    populateComponentRefDataModel(componentRefsItem,
                                                  componentRef);
            }
        }
    }

    // Retrieve the model's connections

    if (cellmlFile->connections().count()) {
        // Connections category

        CellmlElementItem *connectionsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                                   CellmlElementItem::Connection,
                                                                   tr("Connections"));

        modelItem->appendRow(connectionsItem);

        // Retrieve the model's connections themselves

        int counter = 0;

        foreach (CellMLSupport::CellmlFileConnection *connection, cellmlFile->connections()) {
            // A model's connection

            CellmlElementItem *connectionItem = new CellmlElementItem(CellmlElementItem::Connection,
                                                                      tr("Connection #%1").arg(QString::number(++counter)));

            connectionsItem->appendRow(connectionItem);

            // Component mapping

            connectionItem->appendRow(new CellmlElementItem(CellmlElementItem::Component,
                                                            QString("%1 %2 %3").arg(connection->componentMapping()->firstComponentName(),
                                                                                    mRightArrow,
                                                                                    connection->componentMapping()->secondComponentName())));

            // Variable mappings

            foreach (CellMLSupport::CellmlFileMapVariablesItem *mapVariablesItem,
                     connection->variableMappings())
                connectionItem->appendRow(new CellmlElementItem(CellmlElementItem::Variable,
                                                                QString("%1 %2 %3").arg(mapVariablesItem->firstVariableName(),
                                                                                        mRightArrow,
                                                                                        mapVariablesItem->secondVariableName())));
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

        CellmlElementItem *unitsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                                   CellmlElementItem::Unit,
                                                                   tr("Units"));

        pCellmlElementItem->appendRow(unitsItem);

        // Retrieve the units themselves

        foreach (CellMLSupport::CellmlFileUnit *unit, pUnits) {
            CellmlElementItem *unitItem = new CellmlElementItem(unit);

            unitsItem->appendRow(unitItem);

            // Retrieve the unit's unit elements

            if (unit->unitElements().count())
                foreach (CellMLSupport::CellmlFileUnitElement *unitElement,
                         unit->unitElements())
                    unitItem->appendRow(new CellmlElementItem(unitElement));

        }
    }
}

//==============================================================================

void CellmlAnnotationViewWidget::populateComponentRefDataModel(CellmlElementItem *pCellmlElementItem,
                                                               CellMLSupport::CellmlFileComponentRef *pComponentRef)
{
    CellmlElementItem *componentRefItem = new CellmlElementItem(CellmlElementItem::ComponentRef,
                                                                pComponentRef->component());

    pCellmlElementItem->appendRow(componentRefItem);

    // Retrieve the component ref(erence)'s children

    foreach (CellMLSupport::CellmlFileComponentRef *componentRef,
             pComponentRef->componentRefs())
        populateComponentRefDataModel(componentRefItem, componentRef);
}

//==============================================================================

void CellmlAnnotationViewWidget::populateMetadataDataModel(const QString &pFileName)
{
    // Retrieve our CellML file object and load the CellML file
    // Note: the CellML file will already be loaded because of our previous call
    //       to populateCellmlDataModel() and that's fine. In fact, should there
    //       have been issues, then they will have been listed in the CellML
    //       tree view, so we don't need to do anything here in case there are
    //       issues, just return...

    CellMLSupport::CellmlFile *cellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);

    cellmlFile->load();

    if (cellmlFile->issues().count())
        // There are issues, so...

        return;

    // Retrieve the name of the different groups of triples

    foreach (const QString &groupName, cellmlFile->metadata().uniqueKeys())
        mMetadataDataModel->invisibleRootItem()->appendRow(new CellmlElementItem(CellmlElementItem::Metadata,
                                                                                 groupName));
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

    CellmlElementItem *cellmlElementItem = static_cast<CellmlElementItem *>(mCellmlDataModel->itemFromIndex(pNewIndex));

    switch (cellmlElementItem->type()) {
    case CellmlElementItem::Model:
        mDetails->setModel(cellmlElementItem->element());

        break;
    case CellmlElementItem::Import:
        mDetails->setImport(cellmlElementItem->element());

        break;
    case CellmlElementItem::Unit:
        mDetails->setUnit(cellmlElementItem->element());

        break;
    case CellmlElementItem::UnitElement:
        mDetails->setUnitElement(cellmlElementItem->element());

        break;
    case CellmlElementItem::Component:
        mDetails->setComponent(cellmlElementItem->element());

        break;
    case CellmlElementItem::Variable:
        mDetails->setVariable();

        break;
    case CellmlElementItem::MathmlElement:
        mDetails->setMathmlElement();

        break;
    case CellmlElementItem::Group:
        mDetails->setGroup();

        break;
    case CellmlElementItem::RelationshipRef:
        mDetails->setRelationshipRef();

        break;
    case CellmlElementItem::ComponentRef:
        mDetails->setComponentRef();

        break;
    case CellmlElementItem::Connection:
        mDetails->setConnection();

        break;
    case CellmlElementItem::Metadata:
        mDetails->setMetadata();

        break;
    default:
        // Either an error, warning or category, so nothing to show...

        mDetails->setEmpty();

        break;
    }
}

//==============================================================================

void CellmlAnnotationViewWidget::updateMetadataNode(const QModelIndex &pNewIndex,
                                                    const QModelIndex &pOldIndex)
{
    Q_UNUSED(pNewIndex);
    Q_UNUSED(pOldIndex);

    mDetails->setMetadata();
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
