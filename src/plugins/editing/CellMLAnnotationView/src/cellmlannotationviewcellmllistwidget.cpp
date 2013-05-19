//==============================================================================
// CellML annotation view CellML list widget
//==============================================================================

#include "cellmlannotationviewcellmllistwidget.h"
#include "cellmlannotationviewwidget.h"
#include "treeviewwidget.h"

//==============================================================================

#include "ui_cellmlannotationviewcellmllistwidget.h"

//==============================================================================

#include <QDir>
#include <QFileInfo>
#include <QMenu>
#include <QStandardItemModel>

//==============================================================================

#include <QtSingleApplication>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

void CellMLAnnotationViewCellMLElementItemDelegate::paint(QPainter *pPainter,
                                                          const QStyleOptionViewItem &pOption,
                                                          const QModelIndex &pIndex) const
{
    // Paint the item as normal, except for error/warning/category items

    CellMLAnnotationViewCellMLElementItem *cellmlElementItem = static_cast<CellMLAnnotationViewCellMLElementItem *>(qobject_cast<const QStandardItemModel *>(pIndex.model())->itemFromIndex(pIndex));

    QStyleOptionViewItemV4 option(pOption);

    initStyleOption(&option, pIndex);

    if (   (cellmlElementItem->type() == CellMLAnnotationViewCellMLElementItem::Error)
        || (cellmlElementItem->type() == CellMLAnnotationViewCellMLElementItem::Warning)) {
        // This is an error/warning item, so prevent it from hoverable and make
        // it look enabled since it's actually disabled (so we can't select it),
        // yet we want to see as if it was enabled, so...

        option.state &= ~QStyle::State_MouseOver;
        option.state |=  QStyle::State_Enabled;
    } else if (cellmlElementItem->isCategory()) {
        // We are dealing with a category so show it bold

        option.font.setBold(cellmlElementItem->isCategory());
    }

    QStyledItemDelegate::paint(pPainter, option, pIndex);
}

//==============================================================================

void CellMLAnnotationViewCellMLElementItem::constructor(const bool &pCategory,
                                                        const Type &pType,
                                                        iface::cellml_api::CellMLElement *pElement,
                                                        const int &pNumber)
{
    // Some initialisations

    mCategory = pCategory;
    mType = pType;

    mElement = pElement;

    mNumber = pNumber;
}

//==============================================================================

CellMLAnnotationViewCellMLElementItem::CellMLAnnotationViewCellMLElementItem(const bool &pError, const QString &pText) :
    QStandardItem(pText)
{
    // Constructor for either an error or a warning

    constructor(false, pError?Error:Warning, 0, -1);

    // Disable the item and use its text as a tooltip (in case it's too long and
    // doesn't fit within the allocated space we have)
    // Note: the item will get 're-enabled' by our item delegate...

    setEnabled(false);
    setToolTip(pText);

    // Set the icon for the item

    setIcon(mType);
}

//==============================================================================

CellMLAnnotationViewCellMLElementItem::CellMLAnnotationViewCellMLElementItem(const Type &pType,
                                                                             const QString &pText) :
    QStandardItem(pText)
{
    // Constructor for a category

    constructor(true, pType, 0, -1);

    // Use its text as a tooltip (in case it's too long and doesn't fit within
    // the allocated space we have)

    setToolTip(pText);

    // Set the icon for the item

    setIcon(pType);
}

//==============================================================================

CellMLAnnotationViewCellMLElementItem::CellMLAnnotationViewCellMLElementItem(const Type &pType,
                                                                             iface::cellml_api::CellMLElement *pElement,
                                                                             const int pNumber) :
    QStandardItem()
{
    // Constructor for a CellML element

    constructor(false, pType, pElement, pNumber);

    // Set the text for some types

    static const QChar RightArrow = QChar(0x2192);

    switch (pType) {
    case Import: {
        ObjRef<iface::cellml_api::URI> xlinkHref = dynamic_cast<iface::cellml_api::CellMLImport *>(pElement)->xlinkHref();

        setText(QString::fromStdWString(xlinkHref->asText()));

        break;
    }
    case UnitElement:
        setText(QString::fromStdWString(dynamic_cast<iface::cellml_api::Unit *>(pElement)->units()));

        break;
    case Group:
        setText(QObject::tr("Group #%1").arg(QString::number(pNumber)));

        break;
    case RelationshipReference:
        setText(QString::fromStdWString(dynamic_cast<iface::cellml_api::RelationshipRef *>(pElement)->relationship()));

        break;
    case ComponentReference:
        setText(QString::fromStdWString(dynamic_cast<iface::cellml_api::ComponentRef *>(pElement)->componentName()));

        break;
    case Connection:
        setText(QObject::tr("Connection #%1").arg(QString::number(pNumber)));

        break;
    case ComponentMapping: {
        ObjRef<iface::cellml_api::MapComponents> mapComponents = dynamic_cast<iface::cellml_api::MapComponents *>(pElement);

        setText(QString("%1 %2 %3").arg(QString::fromStdWString(mapComponents->firstComponentName()),
                                        RightArrow,
                                        QString::fromStdWString(mapComponents->secondComponentName())));

        break;
    }
    case VariableMapping: {
        ObjRef<iface::cellml_api::MapVariables> mapVariables = dynamic_cast<iface::cellml_api::MapVariables *>(pElement);

        setText(QString("%1 %2 %3").arg(QString::fromStdWString(mapVariables->firstVariableName()),
                                        RightArrow,
                                        QString::fromStdWString(mapVariables->secondVariableName())));

        break;
    }
    default:
        // Another type of element which has a name

        setText(QString::fromStdWString(dynamic_cast<iface::cellml_api::NamedCellMLElement *>(pElement)->name()));
    }

    // Use its text as a tooltip (in case it's too long and doesn't fit within
    // the allocated space we have)

    setToolTip(text());

    // Set the icon for the item

    setIcon(pType);
}

//==============================================================================

void CellMLAnnotationViewCellMLElementItem::setIcon(const Type &pType)
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
        QStandardItem::setIcon(QIcon(":CellMLSupport_relationshipNode"));

        break;
    case ComponentReference:
        QStandardItem::setIcon(QIcon(":CellMLSupport_componentNode"));

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

bool CellMLAnnotationViewCellMLElementItem::isCategory() const
{
    // Return whether the CellML element item is a category

    return mCategory;
}

//==============================================================================

int CellMLAnnotationViewCellMLElementItem::type() const
{
    // Return the CellML element item's type

    return mType;
}

//==============================================================================

int CellMLAnnotationViewCellMLElementItem::number() const
{
    // Return the CellML element item's number

    return mNumber;
}

//==============================================================================

iface::cellml_api::CellMLElement * CellMLAnnotationViewCellMLElementItem::element() const
{
    // Return the CellML element item's element

    return mElement;
}

//==============================================================================

CellMLAnnotationViewCellMLListWidget::CellMLAnnotationViewCellMLListWidget(CellMLAnnotationViewWidget *pParent) :
    Widget(pParent),
    mCellMLFile(pParent->cellmlFile()),
    mGui(new Ui::CellMLAnnotationViewCellMLListWidget),
    mIndexes(QList<QModelIndex>())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create and customise our tree view widget which will contain all of the
    // imports, units, components, groups and connections from a CellML file

    mTreeViewWidget = new Core::TreeViewWidget(pParent);
    mModel          = new QStandardItemModel(mTreeViewWidget);
    mItemDelegate   = new CellMLAnnotationViewCellMLElementItemDelegate();

    mTreeViewWidget->setModel(mModel);
    mTreeViewWidget->setItemDelegate(mItemDelegate);

    mTreeViewWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mTreeViewWidget->setHeaderHidden(true);
    mTreeViewWidget->setRootIsDecorated(false);
    mTreeViewWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    // Note: the selection mode we are opting for means that there is always
    //       going to be a CellML element which is selected, so it's something
    //       that we must keep in mind when showing the context menu...

    // Populate ourselves

    mGui->layout->addWidget(mTreeViewWidget);

    // We want a context menu for our tree view widget

    mTreeViewWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(mTreeViewWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu(const QPoint &)));

    // Some connections to handle the expansion/collapse of a CellML element

    connect(mTreeViewWidget, SIGNAL(expanded(const QModelIndex &)),
            this, SLOT(resizeTreeViewToContents()));
    connect(mTreeViewWidget, SIGNAL(collapsed(const QModelIndex &)),
            this, SLOT(resizeTreeViewToContents()));

    // Some connections to handle the change of CellML element

    connect(mTreeViewWidget->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(updateMetadataDetails(const QModelIndex &, const QModelIndex &)));

    // Populate our tree view widget

    populateModel();

    // Make our tree view widget our focus proxy

    setFocusProxy(mTreeViewWidget);

    // Expand our tree view widget enough so that we can see the meaningful
    // parts of the CellML file

    mTreeViewWidget->expandToDepth(1);

    // Resize our tree view widget, just to be on the safe side

    resizeTreeViewToContents();
}

//==============================================================================

CellMLAnnotationViewCellMLListWidget::~CellMLAnnotationViewCellMLListWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellMLAnnotationViewCellMLListWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    // Retranslate some of the CellML elements in our tree view widget

    retranslateDataItem(static_cast<CellMLAnnotationViewCellMLElementItem *>(mModel->invisibleRootItem()));
}

//==============================================================================

void CellMLAnnotationViewCellMLListWidget::retranslateDataItem(CellMLAnnotationViewCellMLElementItem *pCellMLElementItem)
{
    // Retranslate some of the CellML element's children

    for (int i = 0, iMax = pCellMLElementItem->rowCount(); i < iMax; ++i)
        retranslateDataItem(static_cast<CellMLAnnotationViewCellMLElementItem *>(pCellMLElementItem->child(i)));

    // Check whether we are dealing with a category

    if (pCellMLElementItem->isCategory())
        // We are dealing with a category, so retranslate its type

        switch (pCellMLElementItem->type()) {
        case CellMLAnnotationViewCellMLElementItem::Import:
            pCellMLElementItem->setText(tr("Imports"));

            break;
        case CellMLAnnotationViewCellMLElementItem::Unit:
            pCellMLElementItem->setText(tr("Units"));

            break;
        case CellMLAnnotationViewCellMLElementItem::Component:
            pCellMLElementItem->setText(tr("Components"));

            break;
        case CellMLAnnotationViewCellMLElementItem::Variable:
            pCellMLElementItem->setText(tr("Variables"));

            break;
        case CellMLAnnotationViewCellMLElementItem::Group:
            pCellMLElementItem->setText(tr("Groups"));

            break;
        case CellMLAnnotationViewCellMLElementItem::RelationshipReference:
            pCellMLElementItem->setText(tr("Relationship References"));

            break;
        case CellMLAnnotationViewCellMLElementItem::ComponentReference:
            pCellMLElementItem->setText(tr("Component References"));

            break;
        case CellMLAnnotationViewCellMLElementItem::Connection:
            pCellMLElementItem->setText(tr("Connections"));

            break;
        default:
            // Not a type we can retranslate, so do nothing...

            ;
        }
    else
        // We are not dealing with a category, so check the type and see whether
        // a CellML element needs retranslating

        switch (pCellMLElementItem->type()) {
        case CellMLAnnotationViewCellMLElementItem::Group:
            pCellMLElementItem->setText(tr("Group #%1").arg(pCellMLElementItem->number()));

            break;
        case CellMLAnnotationViewCellMLElementItem::Connection:
            pCellMLElementItem->setText(tr("Connection #%1").arg(pCellMLElementItem->number()));

            break;
        default:
            // Not a sub-type we can retranslate, so do nothing...

            ;
        }
}

//==============================================================================

void CellMLAnnotationViewCellMLListWidget::populateModel()
{
    // Make sure that the CellML file was properly loaded

    CellMLSupport::CellMLFileIssues issues = mCellMLFile->issues();
    int issuesCount = issues.count();

    if (issuesCount) {
        // Something went wrong while trying to load the CellML file, so report
        // the issue(s) and leave

        for (int i = 0; i < issuesCount; ++i)
            mModel->invisibleRootItem()->appendRow(new CellMLAnnotationViewCellMLElementItem(issues[i].type() == CellMLSupport::CellMLFileIssue::Error,
                                                                                             QString("[%1:%2] %3").arg(QString::number(issues[i].line()),
                                                                                                                       QString::number(issues[i].column()),
                                                                                                                       issues[i].formattedMessage())));

        return;
    }

    // Retrieve the model's root

    CellMLAnnotationViewCellMLElementItem *modelItem = new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::Model,
                                                                                                 mCellMLFile->model());

    mModel->invisibleRootItem()->appendRow(modelItem);

    // Retrieve the model's imports

    ObjRef<iface::cellml_api::CellMLImportSet> imports = mCellMLFile->model()->imports();

    if (imports->length()) {
        // Imports category

        CellMLAnnotationViewCellMLElementItem *importsItem = new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::Import,
                                                                                                       tr("Imports"));

        modelItem->appendRow(importsItem);

        // Retrieve the model's imports themselves

        ObjRef<iface::cellml_api::CellMLImportIterator> importsIterator = imports->iterateImports();

        forever {
            ObjRef<iface::cellml_api::CellMLImport> import = importsIterator->nextImport();

            if (!import)
                break;

            // A model import

            CellMLAnnotationViewCellMLElementItem *importItem = new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::Import,
                                                                                                          import);

            importsItem->appendRow(importItem);

            // Retrieve the model's import's units

            ObjRef<iface::cellml_api::ImportUnitsSet> importUnits = import->units();

            if (importUnits->length()) {
                // Units category

                CellMLAnnotationViewCellMLElementItem *unitsItem = new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::ImportUnit,
                                                                                                             tr("Units"));

                importItem->appendRow(unitsItem);

                // Retrieve the model's import's units themselves

                ObjRef<iface::cellml_api::ImportUnitsIterator> importUnitsIterator = importUnits->iterateImportUnits();

                forever {
                    ObjRef<iface::cellml_api::ImportUnits> importUnit = importUnitsIterator->nextImportUnits();

                    if (!importUnit)
                        break;

                    // A model's import's unit

                    unitsItem->appendRow(new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::ImportUnit,
                                                                                   importUnit));
                }
            }

            // Retrieve the model's import's components

            ObjRef<iface::cellml_api::ImportComponentSet> importComponents = import->components();

            if (importComponents->length()) {
                // Components category

                CellMLAnnotationViewCellMLElementItem *componentsItem = new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::ImportComponent,
                                                                                                                  tr("Components"));

                importItem->appendRow(componentsItem);

                // Retrieve the model's import's components themselves

                ObjRef<iface::cellml_api::ImportComponentIterator> importComponentsIterator = importComponents->iterateImportComponents();

                forever {
                    ObjRef<iface::cellml_api::ImportComponent> importComponent = importComponentsIterator->nextImportComponent();

                    if (!importComponent)
                        break;

                    // A model's import's component

                    componentsItem->appendRow(new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::ImportComponent,
                                                                                        importComponent));
                }
            }
        }
    }

    // Retrieve the model's units

    ObjRef<iface::cellml_api::UnitsSet> units = mCellMLFile->model()->localUnits();

    populateUnitsModel(modelItem, units);

    // Retrieve the model's components

    ObjRef<iface::cellml_api::CellMLComponentSet> components = mCellMLFile->model()->localComponents();

    if (components->length()) {
        // Components category

        CellMLAnnotationViewCellMLElementItem *componentsItem = new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::Component,
                                                                                                          tr("Components"));

        modelItem->appendRow(componentsItem);

        // Retrieve the model's components themselves

        ObjRef<iface::cellml_api::CellMLComponentIterator> componentsIterator = components->iterateComponents();

        forever {
            ObjRef<iface::cellml_api::CellMLComponent> component = componentsIterator->nextComponent();

            if (!component)
                break;

            // A model's component

            CellMLAnnotationViewCellMLElementItem *componentItem = new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::Component,
                                                                                                             component);

            componentsItem->appendRow(componentItem);

            // Retrieve the model's component's units

            ObjRef<iface::cellml_api::UnitsSet> componentUnits = component->units();

            populateUnitsModel(componentItem, componentUnits);

            // Retrieve the model's component's variables

            ObjRef<iface::cellml_api::CellMLVariableSet> componentVariables = component->variables();

            if (componentVariables->length()) {
                // Variables category

                CellMLAnnotationViewCellMLElementItem *variablesItem = new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::Variable,
                                                                                                                 tr("Variables"));

                // Retrieve the model's component's variables themselves, but
                // only add a variable if neither its public nor its private
                // interface is equal to "in"

                bool atLeastOneVariable = false;
                ObjRef<iface::cellml_api::CellMLVariableIterator> componentVariablesIterator = componentVariables->iterateVariables();

                forever {
                    ObjRef<iface::cellml_api::CellMLVariable> componentVariable = componentVariablesIterator->nextVariable();

                    if (!componentVariable)
                        break;

                    if (   (componentVariable->publicInterface()  != iface::cellml_api::INTERFACE_IN)
                        && (componentVariable->privateInterface() != iface::cellml_api::INTERFACE_IN)) {
                        variablesItem->appendRow(new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::Variable,
                                                                                           componentVariable));

                        atLeastOneVariable = true;
                    }
                }

                // Check whether some variables were actually added

                if (atLeastOneVariable)
                    componentItem->appendRow(variablesItem);
                else
                    delete variablesItem;
            }
        }
    }

    // Retrieve the model's groups

    ObjRef<iface::cellml_api::GroupSet> groups = mCellMLFile->model()->groups();

    if (groups->length()) {
        // Groups category

        CellMLAnnotationViewCellMLElementItem *groupsItem = new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::Group,
                                                                                                      tr("Groups"));

        modelItem->appendRow(groupsItem);

        // Retrieve the model's groups themselves

        int counter = 0;
        ObjRef<iface::cellml_api::GroupIterator> groupsIterator = groups->iterateGroups();

        forever {
            ObjRef<iface::cellml_api::Group> group = groupsIterator->nextGroup();

            if (!group)
                break;

            // A model's group

            CellMLAnnotationViewCellMLElementItem *groupItem = new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::Group,
                                                                                                         group, ++counter);

            groupsItem->appendRow(groupItem);

            // Retrieve the model's group's relationship references

            ObjRef<iface::cellml_api::RelationshipRefSet> groupRelationshipReferences = group->relationshipRefs();

            if (groupRelationshipReferences->length()) {
                // Relationship references category

                CellMLAnnotationViewCellMLElementItem *groupRelationshipReferencesItem = new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::RelationshipReference,
                                                                                                                                   tr("Relationship References"));

                groupItem->appendRow(groupRelationshipReferencesItem);

                // Retrieve the model's group's relationship references
                // themselves

                ObjRef<iface::cellml_api::RelationshipRefIterator> groupRelationshipReferencesIterator = groupRelationshipReferences->iterateRelationshipRefs();

                forever {
                    ObjRef<iface::cellml_api::RelationshipRef> groupRelationshipReference = groupRelationshipReferencesIterator->nextRelationshipRef();

                    if (!groupRelationshipReference)
                        break;

                    groupRelationshipReferencesItem->appendRow(new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::RelationshipReference,
                                                                                                         groupRelationshipReference));
                }
            }

            // Retrieve the model's group's component references

            ObjRef<iface::cellml_api::ComponentRefSet> groupComponentReferences = group->componentRefs();

            if (groupComponentReferences->length()) {
                // Component references category

                CellMLAnnotationViewCellMLElementItem *groupComponentReferencesItem = new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::ComponentReference,
                                                                                                                                tr("Component References"));

                groupItem->appendRow(groupComponentReferencesItem);

                // Retrieve the model's group's relationship references
                // themselves

                ObjRef<iface::cellml_api::ComponentRefIterator> groupComponentReferencesIterator = groupComponentReferences->iterateComponentRefs();

                forever {
                    ObjRef<iface::cellml_api::ComponentRef> groupComponentReference = groupComponentReferencesIterator->nextComponentRef();

                    if (!groupComponentReference)
                        break;

                    populateGroupComponentReferenceModel(groupComponentReferencesItem, groupComponentReference);
                }
            }
        }
    }

    // Retrieve the model's connections

    ObjRef<iface::cellml_api::ConnectionSet> connections = mCellMLFile->model()->connections();

    if (connections->length()) {
        // Connections category

        CellMLAnnotationViewCellMLElementItem *connectionsItem = new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::Connection,
                                                                                                           tr("Connections"));

        modelItem->appendRow(connectionsItem);

        // Retrieve the model's connections themselves

        int counter = 0;
        ObjRef<iface::cellml_api::ConnectionIterator> connectionsIterator = connections->iterateConnections();

        forever {
            ObjRef<iface::cellml_api::Connection> connection = connectionsIterator->nextConnection();

            if (!connection)
                break;

            // A model's connection

            CellMLAnnotationViewCellMLElementItem *connectionItem = new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::Connection,
                                                                                                              connection, ++counter);

            connectionsItem->appendRow(connectionItem);

            // Component mapping

            connectionItem->appendRow(new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::ComponentMapping,
                                                                                connection->componentMapping()));

            // Variable mappings

            ObjRef<iface::cellml_api::MapVariablesSet> connectionVariableMappings = connection->variableMappings();
            ObjRef<iface::cellml_api::MapVariablesIterator> connectionVariableMappingsIterator = connectionVariableMappings->iterateMapVariables();

            forever {
                ObjRef<iface::cellml_api::MapVariables> connectionVariableMapping = connectionVariableMappingsIterator->nextMapVariables();

                if (!connectionVariableMapping)
                    break;

                connectionItem->appendRow(new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::VariableMapping,
                                                                                    connectionVariableMapping));
            }
        }
    }
}

//==============================================================================

void CellMLAnnotationViewCellMLListWidget::populateUnitsModel(CellMLAnnotationViewCellMLElementItem *pCellMLElementItem,
                                                              iface::cellml_api::UnitsSet *pUnits)
{
    // Retrieve the units

    if (pUnits->length()) {
        // Units category

        CellMLAnnotationViewCellMLElementItem *unitsItem = new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::Unit,
                                                                                                     tr("Units"));

        pCellMLElementItem->appendRow(unitsItem);

        // Retrieve the units themselves

        ObjRef<iface::cellml_api::UnitsIterator> unitsIterator = pUnits->iterateUnits();

        forever {
            ObjRef<iface::cellml_api::Units> unit = unitsIterator->nextUnits();

            if (!unit)
                break;

            // A unit

            CellMLAnnotationViewCellMLElementItem *unitItem = new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::Unit,
                                                                                                        unit);

            unitsItem->appendRow(unitItem);

            // Retrieve the unit's unit elements

            ObjRef<iface::cellml_api::UnitSet> unitElements = unit->unitCollection();

            if (unitElements->length()) {
                ObjRef<iface::cellml_api::UnitIterator> unitElementsIterator = unitElements->iterateUnits();

                forever {
                    ObjRef<iface::cellml_api::Unit> unitElement = unitElementsIterator->nextUnit();

                    if (!unitElement)
                        break;

                    unitItem->appendRow(new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::UnitElement,
                                                                                  unitElement));
                }
            }
        }
    }
}

//==============================================================================

void CellMLAnnotationViewCellMLListWidget::populateGroupComponentReferenceModel(CellMLAnnotationViewCellMLElementItem *pCellMLElementItem,
                                                                                iface::cellml_api::ComponentRef *pGroupComponentReference)
{
    CellMLAnnotationViewCellMLElementItem *groupComponentReferencesItem = new CellMLAnnotationViewCellMLElementItem(CellMLAnnotationViewCellMLElementItem::ComponentReference,
                                                                                                                    pGroupComponentReference);

    pCellMLElementItem->appendRow(groupComponentReferencesItem);

    // Retrieve the component reference's children

    ObjRef<iface::cellml_api::ComponentRefSet> groupComponentReferences = pGroupComponentReference->componentRefs();
    ObjRef<iface::cellml_api::ComponentRefIterator> groupComponentReferencesIterator = groupComponentReferences->iterateComponentRefs();

    forever {
        ObjRef<iface::cellml_api::ComponentRef> groupComponentReference = groupComponentReferencesIterator->nextComponentRef();

        if (!groupComponentReference)
            break;

        populateGroupComponentReferenceModel(groupComponentReferencesItem, groupComponentReference);
    }
}

//==============================================================================

void CellMLAnnotationViewCellMLListWidget::resizeTreeViewToContents()
{
    // Resize our tree view widget so that its contents is visible

    mTreeViewWidget->resizeColumnToContents(0);
}

//==============================================================================

void CellMLAnnotationViewCellMLListWidget::updateMetadataDetails(const QModelIndex &pNewIndex,
                                                                 const QModelIndex &pOldIndex)
{
    Q_UNUSED(pOldIndex);

    // Make sure that we have a valid new index

    if (!pNewIndex.isValid())
        return;

    // Keep track of the fact that there is a CellML element to update

    mIndexes << pNewIndex;

    // Make sure that we are not already updating a CellML element by checking
    // that the CellML file for which we want to update an element is not in our
    // list of CellML files being updated

    static QStringList cellmlFileBeingUpdated;

    QString cellmlFileName = mCellMLFile->fileName();

    if (cellmlFileBeingUpdated.contains(cellmlFileName))
        return;

    cellmlFileBeingUpdated << cellmlFileName;

    // Loop while there are CellML elements to update
    // Note: this is done because a CellML element may take time to update and
    //       we may end up in a situation where several CellML elements need
    //       updating, so...

    while (mIndexes.count()) {
        // Retrieve the first CellML element to update

        QModelIndex crtIndex = mIndexes.first();

        mIndexes.removeFirst();

        // Let people know that we request to see some metadata details

        emit metadataDetailsRequested(static_cast<CellMLAnnotationViewCellMLElementItem *>(mModel->itemFromIndex(crtIndex))->element());
    }

    // We are done, so...

    cellmlFileBeingUpdated.removeAt(cellmlFileBeingUpdated.indexOf(cellmlFileName));
}

//==============================================================================

void CellMLAnnotationViewCellMLListWidget::showCustomContextMenu(const QPoint &pPosition) const
{
    // Determine whether to show the context menu based on whether we are over
    // an item

    CellMLAnnotationViewCellMLElementItem *posItem = static_cast<CellMLAnnotationViewCellMLElementItem *>(mModel->itemFromIndex(mTreeViewWidget->indexAt(pPosition)));

    if (posItem) {
        // We are over an item, so create a custom context menu for our current
        // item

        // Update the enabled status of our actions

        mGui->actionExpandAll->setEnabled(posItem->hasChildren() && !indexIsAllExpanded(mTreeViewWidget->currentIndex()));
        mGui->actionCollapseAll->setEnabled(posItem->hasChildren() && mTreeViewWidget->isExpanded(mTreeViewWidget->currentIndex()));

        mGui->actionRemoveCurrentMetadata->setEnabled(!posItem->isCategory() && mCellMLFile->rdfTriples(posItem->element()).count());
        mGui->actionRemoveAllMetadata->setEnabled(!posItem->isCategory() && mCellMLFile->rdfTriples().count());

        mGui->actionOpenImport->setEnabled(posItem->type() == CellMLAnnotationViewCellMLElementItem::Import);

        // Create and show the context menu, if it isn't empty

        QMenu menu;

        if (posItem->hasChildren()) {
            menu.addAction(mGui->actionExpandAll);
            menu.addAction(mGui->actionCollapseAll);
            menu.addSeparator();
        }

        if (!posItem->isCategory()) {
            menu.addAction(mGui->actionRemoveCurrentMetadata);
            menu.addAction(mGui->actionRemoveAllMetadata);
        }

        if (posItem->type() == CellMLAnnotationViewCellMLElementItem::Import) {
            menu.addSeparator();
            menu.addAction(mGui->actionOpenImport);
        }

        if (!menu.isEmpty())
            menu.exec(QCursor::pos());
    }
}

//==============================================================================

void CellMLAnnotationViewCellMLListWidget::on_actionExpandAll_triggered()
{
    // Expand all the CellML elements below the current one
    // Note: we disable and then re-enable updates before expanding all the
    //       index since it may end up in quite a few updates...

    mTreeViewWidget->setUpdatesEnabled(false);
        qApp->setOverrideCursor(Qt::WaitCursor);

        indexExpandAll(mTreeViewWidget->currentIndex());

        qApp->restoreOverrideCursor();
    mTreeViewWidget->setUpdatesEnabled(true);
}

//==============================================================================

void CellMLAnnotationViewCellMLListWidget::on_actionCollapseAll_triggered()
{
    // Collapse all the CellML elements below the current one
    // Note: see the note in on_actionExpandAll_triggered() above...

    mTreeViewWidget->setUpdatesEnabled(false);
        qApp->setOverrideCursor(Qt::WaitCursor);

        indexCollapseAll(mTreeViewWidget->currentIndex());

        qApp->restoreOverrideCursor();
    mTreeViewWidget->setUpdatesEnabled(true);
}

//==============================================================================

void CellMLAnnotationViewCellMLListWidget::on_actionRemoveCurrentMetadata_triggered()
{
    // Remove all the metadata associated with the current node

    mCellMLFile->rdfTriples().remove(currentCellMLElementItem()->element());

    // Re-update the metadata details view now that the current node doesn't
    // have any metadata associated with it

    updateMetadataDetails(mTreeViewWidget->currentIndex(), QModelIndex());
}

//==============================================================================

void CellMLAnnotationViewCellMLListWidget::on_actionRemoveAllMetadata_triggered()
{
    // Remove all the metadata associated with the CellML file

    mCellMLFile->rdfTriples().removeAll();

    // Re-update the metadata details view now that the CellML file doesn't have
    // any metadata associated with it

    updateMetadataDetails(mTreeViewWidget->currentIndex(), QModelIndex());
}

//==============================================================================

void CellMLAnnotationViewCellMLListWidget::indexExpandAll(const QModelIndex &pIndex) const
{
    // Recursively expand all the CellML elements below the current one
    // Note: the test with against pIndex.child(0, 0) is to ensure that we are
    //       not trying to expand an index which item has no children. Indeed,
    //       a call to expand() is quite expensive, so the fewer of those we
    //       make the better...

    if (pIndex.child(0, 0).isValid()) {
        mTreeViewWidget->expand(pIndex);

        QStandardItem *item = mModel->itemFromIndex(pIndex);

        for (int i = 0, iMax = item->rowCount(); i < iMax; ++i)
            indexExpandAll(item->child(i)->index());
    }
}

//==============================================================================

void CellMLAnnotationViewCellMLListWidget::indexCollapseAll(const QModelIndex &pIndex) const
{
    // Recursively collapse all the CellML elements below the current one
    // Note: see the note in indexExpandAll() above...

    if (pIndex.child(0, 0).isValid()) {
        QStandardItem *item = mModel->itemFromIndex(pIndex);

        for (int i = 0, iMax = item->rowCount(); i < iMax; ++i)
            indexCollapseAll(item->child(i)->index());

        mTreeViewWidget->collapse(pIndex);
    }
}

//==============================================================================

bool CellMLAnnotationViewCellMLListWidget::indexIsAllExpanded(const QModelIndex &pIndex) const
{
    // Recursively check that the current CellML element and all of its children
    // are expanded
    // Note: see the note in indexExpandAll() above...

    if (pIndex.child(0, 0).isValid()) {
        QStandardItem *item = mModel->itemFromIndex(pIndex);

        for (int i = 0, iMax = item->rowCount(); i < iMax; ++i)
            if (!indexIsAllExpanded(item->child(i)->index()))
                return false;

        return mTreeViewWidget->isExpanded(pIndex);
    } else {
        return true;
    }
}

//==============================================================================

Core::TreeViewWidget * CellMLAnnotationViewCellMLListWidget::treeViewWidget() const
{
    // Return our tree view widget

    return mTreeViewWidget;
}

//==============================================================================

CellMLAnnotationViewCellMLElementItem * CellMLAnnotationViewCellMLListWidget::currentCellMLElementItem() const
{
    // Return the current CellML element item

    return static_cast<CellMLAnnotationViewCellMLElementItem *>(mModel->itemFromIndex(mTreeViewWidget->currentIndex()));
}

//==============================================================================

void CellMLAnnotationViewCellMLListWidget::on_actionOpenImport_triggered()
{
    // Ask OpenCOR to open the imported file

    static_cast<SharedTools::QtSingleApplication *>(qApp)->handleAction("gui://openFile/"+QFileInfo(QFileInfo(mCellMLFile->fileName()).canonicalPath()+QDir::separator()+currentCellMLElementItem()->text()).canonicalFilePath());
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
