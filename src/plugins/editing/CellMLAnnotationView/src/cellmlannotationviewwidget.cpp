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
    mElement(0),
    mMathmlElement(0)
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
    mElement(0),
    mMathmlElement(0)
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
    mElement(pElement),
    mMathmlElement(0)
{
    // Check what should be used to represent a right arrow character

    QChar rightArrow = QChar(0x2192);

    if (!QRawFont::fromFont(font()).supportsCharacter(rightArrow))
        rightArrow = QChar('>');

    // Set the text for some types

    switch (pType) {
    case Import:
        setText(static_cast<CellMLSupport::CellmlFileImport *>(pElement)->uri());

        break;
    case RelationshipRef:
        setText(static_cast<CellMLSupport::CellmlFileRelationshipRef *>(pElement)->relationship());

        break;
    case ComponentRef:
        setText(static_cast<CellMLSupport::CellmlFileComponentRef *>(pElement)->component());

        break;
    case ComponentMapping: {
        CellMLSupport::CellmlFileMapComponents *mapComponents = static_cast<CellMLSupport::CellmlFileMapComponents *>(pElement);

        setText(QString("%1 %2 %3").arg(mapComponents->firstComponentName(),
                                        rightArrow,
                                        mapComponents->secondComponentName()));

        break;
    }
    case VariableMapping: {
        CellMLSupport::CellmlFileMapVariablesItem *mapVariables = static_cast<CellMLSupport::CellmlFileMapVariablesItem *>(pElement);

        setText(QString("%1 %2 %3").arg(mapVariables->firstVariableName(),
                                        rightArrow,
                                        mapVariables->secondVariableName()));

        break;
    }
    default:
        // Another type of element, so either pText was used or it has a name
        // which we can use

        if (pText.isEmpty())
            setText(static_cast<CellMLSupport::CellmlFileNamedElement *>(pElement)->name());

        break;
    }

    // Set the icon for the item

    setIcon(pType);
}

//==============================================================================

CellmlElementItem::CellmlElementItem(CellMLSupport::CellmlFileMathmlElement *pMathmlElement,
                                     const QString &pText) :
    QStandardItem(pText),
    mCategory(false),
    mType(MathmlElement),
    mElement(0),
    mMathmlElement(pMathmlElement)
{
    // Constructor for some MathML element

    // Set the icon for the item

    setIcon(mType);
}

//==============================================================================

CellmlElementItem::CellmlElementItem(const QString &pText) :
    QStandardItem(pText),
    mCategory(false),
    mType(Metadata),
    mElement(0),
    mMathmlElement(0)
{
    // Constructor for some metadata

    // Set the icon for the item

    setIcon(mType);
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
    case MathmlElement:
        QStandardItem::setIcon(QIcon(":CellMLSupport_mathmlElementNode"));

        break;
    case Group:
        QStandardItem::setIcon(QIcon(":CellMLSupport_groupNode"));

        break;
    case RelationshipRef:
        QStandardItem::setIcon(QIcon(":CellMLSupport_relationshipRefNode"));

        break;
    case ComponentRef:
        QStandardItem::setIcon(QIcon(":CellMLSupport_componentRefNode"));

        break;
    case Connection:
        QStandardItem::setIcon(QIcon(":CellMLSupport_connectionNode"));

        break;
    case Metadata:
        QStandardItem::setIcon(QIcon(":CellMLSupport_metadataNode"));

        break;
    default:
        // Type which doesn't require an icon

        break;
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

CellMLSupport::CellmlFileMathmlElement * CellmlElementItem::mathmlElement() const
{
    // Return the CellML element item's MathML element

    return mMathmlElement;
}

//==============================================================================

CellmlAnnotationViewWidget::CellmlAnnotationViewWidget(QWidget *pParent,
                                                       const QString &pFileName) :
    Widget(pParent),
    mGui(new Ui::CellmlAnnotationViewWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

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

    mGui->verticalLayout->addWidget(mHorizontalSplitter);

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

    populateCellmlDataModel(pFileName);
    populateMetadataDataModel(pFileName);

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
            // Not a type we can retranslate, so do nothing...

            break;
        }
    else
        // We are not dealing with a category, so check the type and see whether
        // node needs retranslating

        switch (pCellmlElementItem->type()) {
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

        mCellmlDataModel->invisibleRootItem()->appendRow(new CellmlElementItem(cellmlFile->issues().first().type() == CellMLSupport::CellmlFileIssue::Error,
                                                                               cellmlFile->issues().first().formattedMessage()));

        return;
    }

    // Retrieve the model's root

    CellmlElementItem *modelItem = new CellmlElementItem(CellmlElementItem::Model,
                                                         cellmlFile->model());

    mCellmlDataModel->invisibleRootItem()->appendRow(modelItem);

    // Retrieve the model's imports

    if (cellmlFile->imports().count()) {
        // Imports category

        CellmlElementItem *importsItem = new CellmlElementItem(CellmlElementItem::Import,
                                                               tr("Imports"));

        modelItem->appendRow(importsItem);

        // Retrieve the model's imports themselves

        foreach (CellMLSupport::CellmlFileImport *import,
                 cellmlFile->imports()) {
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

    populateUnitsDataModel(modelItem, cellmlFile->units());

    // Retrieve the model's components

    if (cellmlFile->components().count()) {
        // Components category

        CellmlElementItem *componentsItem = new CellmlElementItem(CellmlElementItem::Component,
                                                                  tr("Components"));

        modelItem->appendRow(componentsItem);

        // Retrieve the model's components themselves

        foreach (CellMLSupport::CellmlFileComponent *component,
                 cellmlFile->components()) {
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

            // Retrieve the model's component's MathML elements

            if (component->mathmlElements().count()) {
                // MathML elements category

                CellmlElementItem *mathmlElementsItem = new CellmlElementItem(CellmlElementItem::MathmlElement,
                                                                              tr("MathML Elements"));

                componentItem->appendRow(mathmlElementsItem);

                // Retrieve the model's component's MathML elements themselves

                for (int i = 0, iMax = component->mathmlElements().count(); i < iMax; ++i)
                    mathmlElementsItem->appendRow(new CellmlElementItem(component->mathmlElements().at(i),
                                                                        tr("MathML element #%1").arg(QString::number(i+1))));
            }
        }
    }

    // Retrieve the model's groups

    if (cellmlFile->groups().count()) {
        // Groups category

        CellmlElementItem *groupsItem = new CellmlElementItem(CellmlElementItem::Group,
                                                              tr("Groups"));

        modelItem->appendRow(groupsItem);

        // Retrieve the model's groups themselves

        int counter = 0;

        foreach (CellMLSupport::CellmlFileGroup *group, cellmlFile->groups()) {
            // A model's group

            CellmlElementItem *groupItem = new CellmlElementItem(CellmlElementItem::Group,
                                                                 group,
                                                                 tr("Group #%1").arg(QString::number(++counter)));

            groupsItem->appendRow(groupItem);

            // Retrieve the model's group's relationship ref(erence)s

            if (group->relationshipRefs().count()) {
                // Relationship ref(erence)s category

                CellmlElementItem *relationshipRefsItem = new CellmlElementItem(CellmlElementItem::RelationshipRef,
                                                                                tr("Relationship References"));

                groupItem->appendRow(relationshipRefsItem);

                // Retrieve the model's group's relationship ref(erence)s
                // themselves

                foreach (CellMLSupport::CellmlFileRelationshipRef *relationshipRef,
                         group->relationshipRefs())
                    relationshipRefsItem->appendRow(new CellmlElementItem(CellmlElementItem::RelationshipRef,
                                                                          relationshipRef));
            }

            // Retrieve the model's group's component ref(erence)s

            if (group->componentRefs().count()) {
                // Component ref(erence)s category

                CellmlElementItem *componentRefsItem = new CellmlElementItem(CellmlElementItem::ComponentRef,
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

        CellmlElementItem *connectionsItem = new CellmlElementItem(CellmlElementItem::Connection,
                                                                   tr("Connections"));

        modelItem->appendRow(connectionsItem);

        // Retrieve the model's connections themselves

        int counter = 0;

        foreach (CellMLSupport::CellmlFileConnection *connection, cellmlFile->connections()) {
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

void CellmlAnnotationViewWidget::populateComponentRefDataModel(CellmlElementItem *pCellmlElementItem,
                                                               CellMLSupport::CellmlFileComponentRef *pComponentRef)
{
    CellmlElementItem *componentRefItem = new CellmlElementItem(CellmlElementItem::ComponentRef,
                                                                pComponentRef);

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
        mMetadataDataModel->invisibleRootItem()->appendRow(new CellmlElementItem(groupName));
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
        mDetails->update(CellmlAnnotationViewDetailsWidget::Model,
                         cellmlElementItem->element());

        break;
    case CellmlElementItem::Import:
        mDetails->update(CellmlAnnotationViewDetailsWidget::Import,
                         cellmlElementItem->element());

        break;
    case CellmlElementItem::ImportUnit:
        mDetails->update(CellmlAnnotationViewDetailsWidget::ImportUnit,
                         cellmlElementItem->element());

        break;
    case CellmlElementItem::ImportComponent:
        mDetails->update(CellmlAnnotationViewDetailsWidget::ImportComponent,
                         cellmlElementItem->element());

        break;
    case CellmlElementItem::Unit:
        mDetails->update(CellmlAnnotationViewDetailsWidget::Unit,
                         cellmlElementItem->element());

        break;
    case CellmlElementItem::UnitElement:
        mDetails->update(CellmlAnnotationViewDetailsWidget::UnitElement,
                         cellmlElementItem->element());

        break;
    case CellmlElementItem::Component:
        mDetails->update(CellmlAnnotationViewDetailsWidget::Component,
                         cellmlElementItem->element());

        break;
    case CellmlElementItem::Variable:
        mDetails->update(CellmlAnnotationViewDetailsWidget::Variable,
                         cellmlElementItem->element());

        break;
    case CellmlElementItem::MathmlElement:
        mDetails->update(CellmlAnnotationViewDetailsWidget::MathmlElement,
                         cellmlElementItem->mathmlElement());

        break;
    case CellmlElementItem::Group:
        mDetails->update(CellmlAnnotationViewDetailsWidget::Group,
                         cellmlElementItem->element());

        break;
    case CellmlElementItem::RelationshipRef:
        mDetails->update(CellmlAnnotationViewDetailsWidget::RelationshipRef,
                         cellmlElementItem->element());

        break;
    case CellmlElementItem::ComponentRef:
        mDetails->update(CellmlAnnotationViewDetailsWidget::ComponentRef,
                         cellmlElementItem->element());

        break;
    case CellmlElementItem::Connection:
        mDetails->update(CellmlAnnotationViewDetailsWidget::Connection,
                         cellmlElementItem->element());

        break;
    case CellmlElementItem::ComponentMapping:
        mDetails->update(CellmlAnnotationViewDetailsWidget::ComponentMapping,
                         cellmlElementItem->element());

        break;
    case CellmlElementItem::VariableMapping:
        mDetails->update(CellmlAnnotationViewDetailsWidget::VariableMapping,
                         cellmlElementItem->element());

        break;
    case CellmlElementItem::Metadata:
        mDetails->update(CellmlAnnotationViewDetailsWidget::Metadata,
                         cellmlElementItem->element());

        break;
    default:
        // Either an error, warning or category, so nothing to show...

        mDetails->update(CellmlAnnotationViewDetailsWidget::Empty);

        break;
    }
}

//==============================================================================

void CellmlAnnotationViewWidget::updateMetadataNode(const QModelIndex &pNewIndex,
                                                    const QModelIndex &pOldIndex)
{
    Q_UNUSED(pNewIndex);
    Q_UNUSED(pOldIndex);

    mDetails->update(CellmlAnnotationViewDetailsWidget::Metadata);
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
