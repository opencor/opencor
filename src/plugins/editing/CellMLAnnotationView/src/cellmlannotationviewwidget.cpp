//==============================================================================
// CellML annotation view widget
//==============================================================================

#include "borderedwidget.h"
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

CellmlElementItem::CellmlElementItem(const Type &pType, const QString &pText) :
    QStandardItem(pText),
    mType(pType)
{
    initialize(pType, pType, pText);
}

//==============================================================================

CellmlElementItem::CellmlElementItem(const Type &pType, const Type &pSubType,
                                     const QString &pText) :
    QStandardItem(pText),
    mType(pType)
{
    initialize(pType, pSubType, pText);
}

//==============================================================================

void CellmlElementItem::initialize(const Type &pType, const Type &pSubType,
                                   const QString &pText)
{
    // Disable the item in case it's an error/warning item and also use its text
    // as a tooltip (in case it's too long and therefore doesn't fit within the
    // allocated space we have)
    // Note: it will get 're-enabled' by our item delegate

    if ((pType == Error) || (pType == Warning)) {
        setEnabled(false);
        setToolTip(pText);
    }

    // Disable the item in case it's a category item
    // Note: it will get 're-enabled' by our item delegate

    if (pType == Category)
        setEnabled(false);

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
        // Anything that doesn't require an icon

        break;
    }
}

//==============================================================================

int CellmlElementItem::type() const
{
    // Return the CellML element item's type

    return mType;
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
    // with a dummy (for now) widget
    //---GRY--- THE DUMMY WIDGET WILL EVENTUALLY GET REPLACED BY SOMETHING THAT
    //          WILL ALLOW THE USER TO EDIT METADATA, MAKE USE OF RICORDO, ETC.

    mVerticalSplitter->addWidget(new Core::BorderedWidget(mCellmlTreeView,
                                                          false, false, true, true));
    mVerticalSplitter->addWidget(new Core::BorderedWidget(mMetadataTreeView,
                                                          true, false, false, true));

    mHorizontalSplitter->addWidget(mVerticalSplitter);
    mHorizontalSplitter->addWidget(new Core::BorderedWidget(new QWidget(this),
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

    // Some connections

    connect(mCellmlTreeView, SIGNAL(expanded(const QModelIndex &)),
            this, SLOT(resizeCellmlTreeViewToContents()));
    connect(mCellmlTreeView, SIGNAL(collapsed(const QModelIndex &)),
            this, SLOT(resizeCellmlTreeViewToContents()));

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
    // Delete some internal objects

    delete mUi;
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

    CellmlElementItem *modelItem = new CellmlElementItem(CellmlElementItem::Model,
                                                         cellmlFile->model()->name());

    mCellmlDataModel->invisibleRootItem()->appendRow(modelItem);

    // Retrieve the model's imports

    if (cellmlFile->imports().count()) {
        // Imports category

        CellmlElementItem *importsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                                 CellmlElementItem::Import,
                                                                 "Imports");

        modelItem->appendRow(importsItem);

        // Retrieve the model's imports themselves

        foreach (CellMLSupport::CellmlFileImport *import,
                 cellmlFile->imports()) {
            // A model import

            CellmlElementItem *importItem = new CellmlElementItem(CellmlElementItem::Import,
                                                                  import->uri());

            importsItem->appendRow(importItem);

            // Retrieve the model's import's units

            if (import->units().count()) {
                // Units category

                CellmlElementItem *unitsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                                     CellmlElementItem::Unit,
                                                                     "Units");

                importItem->appendRow(unitsItem);

                // Retrieve the model's import's units themselves

                foreach (CellMLSupport::CellmlFileImportUnit *unit,
                         import->units())
                    // A model's import's unit

                    unitsItem->appendRow(new CellmlElementItem(CellmlElementItem::Unit,
                                                               unit->name()));
            }

            // Retrieve the model's import's components

            if (import->components().count()) {
                // Components category

                CellmlElementItem *componentsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                                          CellmlElementItem::Component,
                                                                          "Components");

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
                                                                  "Components");

        modelItem->appendRow(componentsItem);

        // Retrieve the model's components themselves

        foreach (CellMLSupport::CellmlFileComponent *component,
                 cellmlFile->components()) {
            // A model's component

            CellmlElementItem *componentItem = new CellmlElementItem(CellmlElementItem::Component,
                                                                     component->name());

            componentsItem->appendRow(componentItem);

            // Retrieve the model's component's units

            populateUnitsDataModel(componentItem, component->units());

            // Retrieve the model's component's variables

            if (component->variables().count()) {
                // Variables category

                CellmlElementItem *variablesItem = new CellmlElementItem(CellmlElementItem::Category,
                                                                         CellmlElementItem::Variable,
                                                                         "Variables");

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
                                                                              "MathML Elements");

                componentItem->appendRow(mathmlElementsItem);

                // Retrieve the model's component's MathML elements themselves

                for (int i = 1, iMax = component->mathmlElements().count(); i <= iMax; ++i)
                    mathmlElementsItem->appendRow(new CellmlElementItem(CellmlElementItem::MathmlElement,
                                                                        QString("MathML element #%1").arg(QString::number(i))));
            }
        }
    }

    // Retrieve the model's groups

    if (cellmlFile->groups().count()) {
        // Groups category

        CellmlElementItem *groupsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                              CellmlElementItem::Group,
                                                              "Groups");

        modelItem->appendRow(groupsItem);

        // Retrieve the model's groups themselves

        int counter = 0;

        foreach (CellMLSupport::CellmlFileGroup *group, cellmlFile->groups()) {
            // A model's group

            CellmlElementItem *groupItem = new CellmlElementItem(CellmlElementItem::Group,
                                                                 QString("Group #%1").arg(QString::number(++counter)));

            groupsItem->appendRow(groupItem);

            // Retrieve the model's group's relationship ref(erence)s

            if (group->relationshipRefs().count()) {
                // Relationship ref(erence)s category

                CellmlElementItem *relationshipRefsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                                                CellmlElementItem::RelationshipRef,
                                                                                "Relationship References");

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
                                                                             "Component References");

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
                                                                   "Connections");

        modelItem->appendRow(connectionsItem);

        // Retrieve the model's connections themselves

        int counter = 0;

        foreach (CellMLSupport::CellmlFileConnection *connection, cellmlFile->connections()) {
            // A model's connection

            CellmlElementItem *connectionItem = new CellmlElementItem(CellmlElementItem::Connection,
                                                                      QString("Connection #%1").arg(QString::number(++counter)));

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
                                                                   "Units");

        pCellmlElementItem->appendRow(unitsItem);

        // Retrieve the units themselves

        foreach (CellMLSupport::CellmlFileUnit *unit, pUnits) {
            CellmlElementItem *unitItem = new CellmlElementItem(CellmlElementItem::Unit,
                                                                unit->name());

            unitsItem->appendRow(unitItem);

            // Retrieve the unit's unit elements

            if (unit->unitElements().count())
                foreach (CellMLSupport::CellmlFileUnitElement *unitElement,
                         unit->unitElements())
                    unitItem->appendRow(new CellmlElementItem(CellmlElementItem::UnitElement,
                                                              unitElement->name()));

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

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
