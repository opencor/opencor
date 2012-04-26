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

#include <QSplitter>
#include <QStandardItemModel>
#include <QTextEdit>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlElementDelegate::CellmlElementDelegate(QStandardItemModel *pDataModel) :
    QStyledItemDelegate(),
    mDataModel(pDataModel)
{
}

//==============================================================================

void CellmlElementDelegate::paint(QPainter *pPainter,
                                  const QStyleOptionViewItem &pOption,
                                  const QModelIndex &pIndex) const
{
    // Paint the item as normal, except for the items which are not checkable
    // (i.e. plugins which the user cannot decide whether to load) in which case
    // we paint them as if they were disabled

    CellmlElementItem *cellmlElementItem = static_cast<CellmlElementItem *>(mDataModel->itemFromIndex(pIndex));

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
    case Group:
        setIcon(QIcon(":CellMLSupport_groupNode"));

        break;
    case Connection:
        setIcon(QIcon(":CellMLSupport_connectionNode"));

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

    // Create our horizontal splitter

    mHorizontalSplitter = new QSplitter(Qt::Horizontal, this);

    // Create and customise a tree view which will contain all of the units,
    // components, groups and connections from the CellML file

    mTreeView = new Core::TreeView(this);
    mDataModel = new QStandardItemModel(mTreeView);
    mCellmlElementDelegate = new CellmlElementDelegate(mDataModel);

    mTreeView->setModel(mDataModel);
    mTreeView->setItemDelegate(mCellmlElementDelegate);

    mTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mTreeView->setFrameShape(QFrame::NoFrame);
    mTreeView->setHeaderHidden(true);
    mTreeView->setRootIsDecorated(false);

    mTreeView->setSelectionMode(QAbstractItemView::SingleSelection);

    // Populate our horizontal splitter with the aforementioned tree view, as
    // well as with a dummy (for now) widget
    //---GRY--- THE DUMMY WIDGET WILL EVENTUALLY GET REPLACED BY SOMETHING THAT
    //          WILL ALLOW THE USER TO EDIT METADATA, MAKE USE OF RICORDO, ETC.

    mHorizontalSplitter->addWidget(new Core::BorderedWidget(mTreeView,
                                                            Core::BorderedWidget::Right));
    mHorizontalSplitter->addWidget(new Core::BorderedWidget(new QWidget(this),
                                                            Core::BorderedWidget::Left));

    mUi->verticalLayout->addWidget(mHorizontalSplitter);

    // Keep track of our horizontal splitter being moved

    connect(mHorizontalSplitter, SIGNAL(splitterMoved(int,int)),
            this, SLOT(emitHorizontalSplitterMoved()));

    // Create a (temporary) debug output
    //---GRY--- THIS WIDGET IS ONLY FOR US WHILE WE ARE WORKING ON THIS
    //          PLUGIN...

    mDebugOutput = new QTextEdit(this);

    mDebugOutput->setFrameStyle(QFrame::NoFrame);
    mDebugOutput->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    mUi->verticalLayout->addWidget(Core::newLineWidget(this));
    mUi->verticalLayout->addWidget(mDebugOutput);

    // Initialise our tree view

    initTreeView(pFileName);
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

void CellmlAnnotationViewWidget::initTreeView(const QString &pFileName)
{
    // Initialise our tree view with the units, components, groups and
    // connections from the CellML file

    // Retrieve our CellML file object and load the CellML file

    CellMLSupport::CellmlFile *cellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);

    cellmlFile->load();

    mDebugOutput->append(QString("File name: %1").arg(pFileName));

    if (cellmlFile->issues().count()) {
        // Something went wrong while trying to load the CellML file, so report
        // it and leave

        mDataModel->invisibleRootItem()->appendRow(new CellmlElementItem((cellmlFile->issues().first().type() == CellMLSupport::CellmlFileIssue::Error)?CellmlElementItem::Error:CellmlElementItem::Warning,
                                                                         cellmlFile->issues().first().formattedMessage()));

        return;
    }

    // Output the name of the CellML model

    CellmlElementItem *modelItem = new CellmlElementItem(CellmlElementItem::Model,
                                                         cellmlFile->model()->name());

    mDataModel->invisibleRootItem()->appendRow(modelItem);
//    mDebugOutput->append(QString("    Model: %1 [%2]").arg(cellmlFile->model()->name(),
//                                                           cellmlFile->model()->cmetaId()));

    // Retrieve the model's imports

    if (cellmlFile->imports().count()) {
        CellmlElementItem *importsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                                 CellmlElementItem::Import,
                                                                 "Imports");

        modelItem->appendRow(importsItem);

        foreach (CellMLSupport::CellmlFileImport *import,
                 cellmlFile->imports()) {
            CellmlElementItem *importItem = new CellmlElementItem(CellmlElementItem::Import,
                                                                  import->uri());

            importsItem->appendRow(importItem);

            if (import->units().count()) {
                CellmlElementItem *unitsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                                     CellmlElementItem::Unit,
                                                                     "Units");

                importItem->appendRow(unitsItem);
//                mDebugOutput->append(QString("            Units:"));

                foreach (CellMLSupport::CellmlFileImportUnit *unit,
                         import->units())
                    unitsItem->appendRow(new CellmlElementItem(CellmlElementItem::Unit,
                                                               unit->name()));
//                    mDebugOutput->append(QString("                Unit: %1 ---> %2 [%3]").arg(unit->name(), unit->referenceName(), unit->cmetaId()));
            }

            if (import->components().count()) {
                CellmlElementItem *componentsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                                          CellmlElementItem::Component,
                                                                          "Components");

                importItem->appendRow(componentsItem);

//                mDebugOutput->append(QString("            Components:"));

                foreach (CellMLSupport::CellmlFileImportComponent *component,
                         import->components())
                    componentsItem->appendRow(new CellmlElementItem(CellmlElementItem::Component,
                                                                    component->name()));
//                    mDebugOutput->append(QString("                Component: %1 ---> %2 [%3]").arg(component->name(), component->referenceName(), component->cmetaId()));
            }
        }
    }

    // Retrieve the model's unit definitions

    initUnitsTreeView(modelItem, cellmlFile->units());

    // Retrieve the model's components

    if (cellmlFile->components().count()) {
        CellmlElementItem *componentsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                                  CellmlElementItem::Component,
                                                                  "Components");

        modelItem->appendRow(componentsItem);

        foreach (CellMLSupport::CellmlFileComponent *component,
                 cellmlFile->components()) {
            CellmlElementItem *componentItem = new CellmlElementItem(CellmlElementItem::Component,
                                                                     component->name());

            componentsItem->appendRow(componentItem);
//            mDebugOutput->append(QString("        %1 [%2]").arg(component->name(),
//                                                                component->cmetaId()));

            initUnitsTreeView(componentItem, component->units());

            if (component->variables().count()) {
                CellmlElementItem *variablesItem = new CellmlElementItem(CellmlElementItem::Category,
                                                                         CellmlElementItem::Variable,
                                                                         "Variables");

                componentItem->appendRow(variablesItem);
//                mDebugOutput->append(QString("            Variables:"));

                foreach (CellMLSupport::CellmlFileVariable *variable,
                         component->variables()) {
//                    QString variablePublicInterface = (variable->publicInterface() == CellMLSupport::CellmlFileVariable::In)?
//                                                          "in":
//                                                          (variable->publicInterface() == CellMLSupport::CellmlFileVariable::Out)?
//                                                              "out":
//                                                              "none";
//                    QString variablePrivateInterface = (variable->privateInterface() == CellMLSupport::CellmlFileVariable::In)?
//                                                          "in":
//                                                          (variable->privateInterface() == CellMLSupport::CellmlFileVariable::Out)?
//                                                              "out":
//                                                              "none";

                    variablesItem->appendRow(new CellmlElementItem(CellmlElementItem::Variable,
                                                                   variable->name()));
//                    mDebugOutput->append(QString("                %1 | Unit: %2 | Initial value: %3 | Public interface: %4 | Private interface: %5 [%6]").arg(variable->name(),
//                                                                                                                                                              variable->unit(),
//                                                                                                                                                              variable->initialValue(),
//                                                                                                                                                              variablePublicInterface,
//                                                                                                                                                              variablePrivateInterface,
//                                                                                                                                                              variable->cmetaId()));
                }
            }

            if (component->mathmlElements().count()) {
                mDebugOutput->append(QString("            MathML elements:"));

                int counter = 0;

                foreach (CellMLSupport::CellmlFileMathmlElement *mathmlElement,
                         component->mathmlElements())
                    mDebugOutput->append(QString("                MathML element #%1 | Class name: %2 | Math element style: %3 | Id: %4 | Xref: %5 | Href: %6").arg(QString::number(++counter),
                                                                                                                                                                    mathmlElement->className(),
                                                                                                                                                                    mathmlElement->mathElementStyle(),
                                                                                                                                                                    mathmlElement->id(),
                                                                                                                                                                    mathmlElement->xref(),
                                                                                                                                                                    mathmlElement->href()));
            }
        }
    }

    // Retrieve the model's groups

    if (cellmlFile->groups().count()) {
        CellmlElementItem *groupsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                              CellmlElementItem::Group,
                                                              "Groups");

        modelItem->appendRow(groupsItem);

        foreach (CellMLSupport::CellmlFileGroup *group, cellmlFile->groups()) {
            mDebugOutput->append(QString("        Group [%1]").arg(group->cmetaId()));

            if (group->relationshipRefs().count()) {
                mDebugOutput->append(QString("            Relationship ref(erence)s:"));

                foreach (CellMLSupport::CellmlFileRelationshipRef *relationshipRef,
                         group->relationshipRefs()) {
                    mDebugOutput->append(QString("                %1 [%2]").arg(relationshipRef->name(),
                                                                                relationshipRef->cmetaId()));
                    mDebugOutput->append(QString("                    Relationship: %1").arg(relationshipRef->relationship()));
                    mDebugOutput->append(QString("                    Relationship namespace: %1").arg(relationshipRef->relationshipNamespace()));
                }
            }

            if (group->componentRefs().count()) {
                mDebugOutput->append(QString("            Component ref(erence)s:"));

                foreach (CellMLSupport::CellmlFileComponentRef *componentRef,
                         group->componentRefs())
                    initComponentRefTreeView("                ", componentRef);
            }
        }
    }

    // Retrieve the model's connections

    if (cellmlFile->connections().count()) {
        CellmlElementItem *connectionsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                                   CellmlElementItem::Connection,
                                                                   "Connections");

        modelItem->appendRow(connectionsItem);

        foreach (CellMLSupport::CellmlFileConnection *connection, cellmlFile->connections()) {
            mDebugOutput->append(QString("        Connection [%1]").arg(connection->cmetaId()));
            mDebugOutput->append(QString("            Components: %1 ---> %2 [%3]").arg(connection->componentMapping()->firstComponentName(),
                                                                                        connection->componentMapping()->secondComponentName(),
                                                                                        connection->componentMapping()->cmetaId()));

            if (connection->variableMappings().count()) {
                mDebugOutput->append(QString("            Variables:"));

                foreach (CellMLSupport::CellmlFileMapVariablesItem *mapVariablesItem,
                         connection->variableMappings())
                    mDebugOutput->append(QString("                %1 ---> %2 [%3]").arg(mapVariablesItem->firstVariableName(),
                                                                                        mapVariablesItem->secondVariableName(),
                                                                                        mapVariablesItem->cmetaId()));
            }
        }
    }

    // Expand everything so we can see the full contents of the CellML file

    mTreeView->expandAll();
}

//==============================================================================

void CellmlAnnotationViewWidget::initUnitsTreeView(QStandardItem *pItem,
                                                   const CellMLSupport::CellmlFileUnits pUnits)
{
    if (pUnits.count()) {
        CellmlElementItem *unitsItem = new CellmlElementItem(CellmlElementItem::Category,
                                                                   CellmlElementItem::Unit,
                                                                   "Units");

        pItem->appendRow(unitsItem);

        foreach (CellMLSupport::CellmlFileUnit *unit, pUnits) {
            CellmlElementItem *unitItem = new CellmlElementItem(CellmlElementItem::Unit,
                                                                unit->name());

            unitsItem->appendRow(unitItem);
//            mDebugOutput->append(QString("%1 [%2]").arg(unit->name(),
//                                                        unit->cmetaId()));
//            mDebugOutput->append(QString("Base unit: %1").arg(unit->isBaseUnit()?"yes":"no"));

            if (unit->unitElements().count()) {
//                mDebugOutput->append("Unit elements:");

                foreach (CellMLSupport::CellmlFileUnitElement *unitElement,
                         unit->unitElements())
                    unitItem->appendRow(new CellmlElementItem(CellmlElementItem::UnitElement,
                                                              unitElement->name()));
//                    mDebugOutput->append(QString("%1 | Prefix: %2 | Multiplier: %3 | Offset: %4 | Exponent: %5 [%6]").arg(unitElement->name(),
//                                                                                                                          QString::number(unitElement->prefix()),
//                                                                                                                          QString::number(unitElement->multiplier()),
//                                                                                                                          QString::number(unitElement->offset()),
//                                                                                                                          QString::number(unitElement->exponent()),
//                                                                                                                          unitElement->cmetaId()));
            }
        }
    }
}

//==============================================================================

void CellmlAnnotationViewWidget::initComponentRefTreeView(const QString &pLeadingSpace,
                                                          CellMLSupport::CellmlFileComponentRef *pComponentRef)
{
    mDebugOutput->append(QString("%1%2 [%3]").arg(pLeadingSpace,
                                                  pComponentRef->name(),
                                                  pComponentRef->cmetaId()));

    foreach (CellMLSupport::CellmlFileComponentRef *componentRef,
             pComponentRef->componentRefs())
        initComponentRefTreeView(pLeadingSpace+"    ", componentRef);
}

//==============================================================================

void CellmlAnnotationViewWidget::updateHorizontalSplitter(const QList<int> &pSizes)
{
    // Update our horizontal splitter after a the horizontal splitter of another
    // CellmlAnnotationViewWidget object has been moved

    mHorizontalSplitter->setSizes(pSizes);
}

//==============================================================================

void CellmlAnnotationViewWidget::emitHorizontalSplitterMoved()
{
    // Let whoever know that our horizontal splitter has been moved

    emit horizontalSplitterMoved(mHorizontalSplitter->sizes());
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
