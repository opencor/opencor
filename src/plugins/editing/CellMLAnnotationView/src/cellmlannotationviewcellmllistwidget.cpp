/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// CellML Annotation view CellML list widget
//==============================================================================

#include "cellmlannotationviewcellmllistwidget.h"
#include "cellmlannotationvieweditingwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "filemanager.h"
#include "i18ninterface.h"
#include "treeviewwidget.h"

//==============================================================================

#include <QAbstractItemView>
#include <QApplication>
#include <QDesktopServices>
#include <QFont>
#include <QLayout>
#include <QMenu>
#include <QPoint>
#include <QStandardItemModel>
#include <QStyle>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewCellmlElementItemDelegate::CellmlAnnotationViewCellmlElementItemDelegate(QObject *pParent) :
    QStyledItemDelegate(pParent)
{
}

//==============================================================================

void CellmlAnnotationViewCellmlElementItemDelegate::paint(QPainter *pPainter,
                                                          const QStyleOptionViewItem &pOption,
                                                          const QModelIndex &pIndex) const
{
    // Paint the item as normal, except for error/warning/category items

    CellmlAnnotationViewCellmlElementItem *cellmlElementItem = static_cast<CellmlAnnotationViewCellmlElementItem *>(qobject_cast<const QStandardItemModel *>(pIndex.model())->itemFromIndex(pIndex));

    QStyleOptionViewItem option(pOption);

    initStyleOption(&option, pIndex);

    if (   (cellmlElementItem->type() == CellmlAnnotationViewCellmlElementItem::Error)
        || (cellmlElementItem->type() == CellmlAnnotationViewCellmlElementItem::Warning)) {
        // This is an error/warning item, so prevent it from being hoverable and
        // make it look enabled since it's actually disabled (so that it can't
        // be selected) and yet we want to see it as if it wasn't

        option.state &= ~QStyle::State_MouseOver;
        option.state |=  QStyle::State_Enabled;
    } else if (cellmlElementItem->isCategory()) {
        // We are dealing with a category so show it bold

        option.font.setBold(cellmlElementItem->isCategory());
    }

    QStyledItemDelegate::paint(pPainter, option, pIndex);
}

//==============================================================================

CellmlAnnotationViewCellmlElementItem::CellmlAnnotationViewCellmlElementItem(const bool &pCategory,
                                                                             const Type &pType,
                                                                             const QString &pText,
                                                                             iface::cellml_api::CellMLElement *pElement,
                                                                             const int &pNumber) :
    QStandardItem(pText)
{
    // Some initialisations

    mCategory = pCategory;
    mType = pType;

    mElement = pElement;

    mNumber = pNumber;
}

//==============================================================================

CellmlAnnotationViewCellmlElementItem::CellmlAnnotationViewCellmlElementItem(const bool &pError,
                                                                             const QString &pText) :
    CellmlAnnotationViewCellmlElementItem(false, pError?Error:Warning, pText, 0, -1)
{
    // Disable the item and use its text as a tooltip (in case it's too long and
    // doesn't fit within the allocated space we have)
    // Note: the item will get 're-enabled' by our item delegate...

    setEnabled(false);
    setToolTip(pText);

    // Set the icon for the item

    setIcon(mType);
}

//==============================================================================

CellmlAnnotationViewCellmlElementItem::CellmlAnnotationViewCellmlElementItem(const Type &pType,
                                                                             const QString &pText) :
    CellmlAnnotationViewCellmlElementItem(true, pType, pText, 0, -1)
{
    // Use its text as a tooltip (in case it's too long and doesn't fit within
    // the allocated space we have)

    setToolTip(pText);

    // Set the icon for the item

    setIcon(pType);
}

//==============================================================================

CellmlAnnotationViewCellmlElementItem::CellmlAnnotationViewCellmlElementItem(const Type &pType,
                                                                             iface::cellml_api::CellMLElement *pElement,
                                                                             const int pNumber) :
    CellmlAnnotationViewCellmlElementItem(false, pType, QString(), pElement, pNumber)
{
    // Set the text for some types

    enum {
        RightArrow = 0x2192
    };

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
        setText(QObject::tr("Group #%1").arg(pNumber));

        break;
    case RelationshipReference: {
        QString name = QString::fromStdWString(dynamic_cast<iface::cellml_api::RelationshipRef *>(pElement)->name());

        setText( QString::fromStdWString(dynamic_cast<iface::cellml_api::RelationshipRef *>(pElement)->relationship())
                +(name.isEmpty()?QString():" "+name));

        break;
    }
    case ComponentReference:
        setText(QString::fromStdWString(dynamic_cast<iface::cellml_api::ComponentRef *>(pElement)->componentName()));

        break;
    case Connection:
        setText(QObject::tr("Connection #%1").arg(pNumber));

        break;
    case ComponentMapping: {
        ObjRef<iface::cellml_api::MapComponents> mapComponents = dynamic_cast<iface::cellml_api::MapComponents *>(pElement);

        setText(QString("%1 %2 %3").arg(QString::fromStdWString(mapComponents->firstComponentName()),
                                        QChar(RightArrow),
                                        QString::fromStdWString(mapComponents->secondComponentName())));

        break;
    }
    case VariableMapping: {
        ObjRef<iface::cellml_api::MapVariables> mapVariables = dynamic_cast<iface::cellml_api::MapVariables *>(pElement);

        setText(QString("%1 %2 %3").arg(QString::fromStdWString(mapVariables->firstVariableName()),
                                        QChar(RightArrow),
                                        QString::fromStdWString(mapVariables->secondVariableName())));

        break;
    }
    default:
        // Another type of element that has a name

        setText(QString::fromStdWString(dynamic_cast<iface::cellml_api::NamedCellMLElement *>(pElement)->name()));
    }

    // Use its text as a tooltip (in case it's too long and doesn't fit within
    // the allocated space we have)

    setToolTip(text());

    // Set the icon for the item

    setIcon(pType);
}

//==============================================================================

void CellmlAnnotationViewCellmlElementItem::setIcon(const Type &pType)
{
    // Set our icon based on our type

    static const QIcon ErrorIcon        = QIcon(":/oxygen/emblems/emblem-important.png");
    static const QIcon WarningIcon      = QIcon(":/oxygen/status/task-attention.png");
    static const QIcon ModelIcon        = QIcon(":/CellMLSupport/modelNode.png");
    static const QIcon ImportIcon       = QIcon(":/CellMLSupport/importNode.png");
    static const QIcon UnitIcon         = QIcon(":/CellMLSupport/unitNode.png");
    static const QIcon UnitElementIcon  = QIcon(":/CellMLSupport/unitElementNode.png");
    static const QIcon ComponentIcon    = QIcon(":/CellMLSupport/componentNode.png");
    static const QIcon VariableIcon     = QIcon(":/CellMLSupport/variableNode.png");
    static const QIcon GroupIcon        = QIcon(":/CellMLSupport/groupNode.png");
    static const QIcon RelationshipIcon = QIcon(":/CellMLSupport/relationshipNode.png");
    static const QIcon ConnectionIcon   = QIcon(":/CellMLSupport/connectionNode.png");

    switch (pType) {
    case Error:
        QStandardItem::setIcon(ErrorIcon);

        break;
    case Warning:
        QStandardItem::setIcon(WarningIcon);

        break;
    case Model:
        QStandardItem::setIcon(ModelIcon);

        break;
    case Import:
        QStandardItem::setIcon(ImportIcon);

        break;
    case ImportUnit:
    case Unit:
        QStandardItem::setIcon(UnitIcon);

        break;
    case UnitElement:
        QStandardItem::setIcon(UnitElementIcon);

        break;
    case ImportComponent:
    case Component:
    case ComponentMapping:
        QStandardItem::setIcon(ComponentIcon);

        break;
    case Variable:
    case VariableMapping:
        QStandardItem::setIcon(VariableIcon);

        break;
    case Group:
        QStandardItem::setIcon(GroupIcon);

        break;
    case RelationshipReference:
        QStandardItem::setIcon(RelationshipIcon);

        break;
    case ComponentReference:
        QStandardItem::setIcon(ComponentIcon);

        break;
    case Connection:
        QStandardItem::setIcon(ConnectionIcon);

        break;
    default:
        // A type that doesn't require an icon

        ;
    }
}

//==============================================================================

bool CellmlAnnotationViewCellmlElementItem::isCategory() const
{
    // Return whether we are a category

    return mCategory;
}

//==============================================================================

int CellmlAnnotationViewCellmlElementItem::type() const
{
    // Return our type

    return mType;
}

//==============================================================================

int CellmlAnnotationViewCellmlElementItem::number() const
{
    // Return our number

    return mNumber;
}

//==============================================================================

iface::cellml_api::CellMLElement * CellmlAnnotationViewCellmlElementItem::element() const
{
    // Return our element

    return mElement;
}

//==============================================================================

CellmlAnnotationViewCellmlListWidget::CellmlAnnotationViewCellmlListWidget(CellmlAnnotationViewEditingWidget *pParent) :
    Widget(pParent),
    mCellmlFile(pParent->cellmlFile())
{
    // Create and customise our tree view widget which will contain all of the
    // imports, units, components, groups and connections from a CellML file

    mTreeViewModel = new QStandardItemModel(this);
    mTreeViewWidget = new Core::TreeViewWidget(this);

    mTreeViewWidget->setModel(mTreeViewModel);
    mTreeViewWidget->setItemDelegate(new CellmlAnnotationViewCellmlElementItemDelegate(this));

    mTreeViewWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mTreeViewWidget->setHeaderHidden(true);
    mTreeViewWidget->setRootIsDecorated(false);

    // Populate ourselves

    createLayout()->addWidget(mTreeViewWidget);

    // We want a context menu for our tree view widget

    mTreeViewWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(mTreeViewWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu(const QPoint &)));

    // Create our actions

    mExpandAllAction = Core::newAction(QIcon(":/oxygen/actions/list-add.png"),
                                       this);
    mCollapseAllAction = Core::newAction(QIcon(":/oxygen/actions/list-remove.png"),
                                         this);
    mRemoveCurrentMetadataAction = Core::newAction(this);
    mRemoveAllMetadataAction = Core::newAction(this);
    mOpenImportAction = Core::newAction(this);

    connect(mExpandAllAction, SIGNAL(triggered(bool)),
            this, SLOT(expandAll()));
    connect(mCollapseAllAction, SIGNAL(triggered(bool)),
            this, SLOT(collapseAll()));
    connect(mRemoveCurrentMetadataAction, SIGNAL(triggered(bool)),
            this, SLOT(removeCurrentMetadata()));
    connect(mRemoveAllMetadataAction, SIGNAL(triggered(bool)),
            this, SLOT(removeAllMetadata()));
    connect(mOpenImportAction, SIGNAL(triggered(bool)),
            this, SLOT(openImport()));

    // Some connections to handle the expansion/collapse of a CellML element

    connect(mTreeViewWidget, SIGNAL(expanded(const QModelIndex &)),
            this, SLOT(resizeTreeViewToContents()));
    connect(mTreeViewWidget, SIGNAL(collapsed(const QModelIndex &)),
            this, SLOT(resizeTreeViewToContents()));

    // Some connections to handle the change of CellML element

    connect(mTreeViewWidget->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(updateMetadataDetails(const QModelIndex &, const QModelIndex &)));

    // Initialise our tree view widget
    // Note: we don't want to select first item of our tree view widget just yet
    //       since we need a connection between ourselves and the metadata
    //       details widget to be set first (see the constructor for
    //       CellmlAnnotationViewEditingWidget)...

    initializeTreeViewWidget(false);

    // Make our tree view widget our focus proxy

    setFocusProxy(mTreeViewWidget);

    // Some further initialisations that are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();
}

//==============================================================================

void CellmlAnnotationViewCellmlListWidget::retranslateUi()
{
    // Retranslate our actions

    I18nInterface::retranslateAction(mExpandAllAction, tr("Expand All"),
                                     tr("Expand all the children nodes"));
    I18nInterface::retranslateAction(mCollapseAllAction, tr("Collapse All"),
                                     tr("Collapse all the children nodes"));
    I18nInterface::retranslateAction(mRemoveCurrentMetadataAction, tr("Remove Current Metadata"),
                                     tr("Remove the metadata associated with the current node"));
    I18nInterface::retranslateAction(mRemoveAllMetadataAction, tr("Remove All Metadata"),
                                     tr("Remove all the metadata associated with the different nodes"));
    I18nInterface::retranslateAction(mOpenImportAction, tr("Open Import"),
                                     tr("Open the imported file"));

    // Retranslate some of the CellML elements in our tree view widget

    retranslateDataItem(static_cast<CellmlAnnotationViewCellmlElementItem *>(mTreeViewModel->invisibleRootItem()));
}

//==============================================================================

void CellmlAnnotationViewCellmlListWidget::retranslateDataItem(CellmlAnnotationViewCellmlElementItem *pCellmlElementItem)
{
    // Retranslate some of the CellML element's children

    for (int i = 0, iMax = pCellmlElementItem->rowCount(); i < iMax; ++i)
        retranslateDataItem(static_cast<CellmlAnnotationViewCellmlElementItem *>(pCellmlElementItem->child(i)));

    // Check whether we are dealing with a category

    if (pCellmlElementItem->isCategory()) {
        // We are dealing with a category, so retranslate its type

        switch (pCellmlElementItem->type()) {
        case CellmlAnnotationViewCellmlElementItem::Import:
            pCellmlElementItem->setText(tr("Imports"));

            break;
        case CellmlAnnotationViewCellmlElementItem::Unit:
            pCellmlElementItem->setText(tr("Units"));

            break;
        case CellmlAnnotationViewCellmlElementItem::Component:
            pCellmlElementItem->setText(tr("Components"));

            break;
        case CellmlAnnotationViewCellmlElementItem::Variable:
            pCellmlElementItem->setText(tr("Variables"));

            break;
        case CellmlAnnotationViewCellmlElementItem::Group:
            pCellmlElementItem->setText(tr("Groups"));

            break;
        case CellmlAnnotationViewCellmlElementItem::RelationshipReference:
            pCellmlElementItem->setText(tr("Relationship References"));

            break;
        case CellmlAnnotationViewCellmlElementItem::ComponentReference:
            pCellmlElementItem->setText(tr("Component References"));

            break;
        case CellmlAnnotationViewCellmlElementItem::Connection:
            pCellmlElementItem->setText(tr("Connections"));

            break;
        default:
            // Not a type that we can retranslate, so do nothing...

            ;
        }
    } else {
        // We are not dealing with a category, so check the type and see whether
        // a CellML element needs retranslating

        switch (pCellmlElementItem->type()) {
        case CellmlAnnotationViewCellmlElementItem::Group:
            pCellmlElementItem->setText(tr("Group #%1").arg(pCellmlElementItem->number()));

            break;
        case CellmlAnnotationViewCellmlElementItem::Connection:
            pCellmlElementItem->setText(tr("Connection #%1").arg(pCellmlElementItem->number()));

            break;
        default:
            // Not a sub-type that we can retranslate, so do nothing...

            ;
        }
    }
}

//==============================================================================

void CellmlAnnotationViewCellmlListWidget::initializeTreeViewWidget(const bool &pSelectFirstItem)
{
    // Populate our tree view widget

    populateModel();

    // Expand our tree view widget enough so that we can see the meaningful
    // parts of the CellML file

    mTreeViewWidget->expandToDepth(1);

    // Resize our tree view widget, just to be on the safe side

    resizeTreeViewToContents();

    // Select the first item of our tree view widget

    if (pSelectFirstItem)
        mTreeViewWidget->selectFirstItem();
}

//==============================================================================

void CellmlAnnotationViewCellmlListWidget::populateModel()
{
    // Make sure that the CellML file was properly loaded

    if (mCellmlFile->issues().count()) {
        // Something went wrong while trying to load the CellML file, so report
        // the issue(s) and leave

        foreach (const CellMLSupport::CellmlFileIssue &issue, mCellmlFile->issues()) {
            mTreeViewModel->invisibleRootItem()->appendRow(new CellmlAnnotationViewCellmlElementItem(issue.type() == CellMLSupport::CellmlFileIssue::Error,
                                                                                                     QString("[%1:%2] %3").arg(QString::number(issue.line()),
                                                                                                                               QString::number(issue.column()),
                                                                                                                               issue.formattedMessage())));
        }

        return;
    }

    // Retrieve the model's root

    CellmlAnnotationViewCellmlElementItem *modelItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Model,
                                                                                                 mCellmlFile->model());

    mTreeViewModel->invisibleRootItem()->appendRow(modelItem);

    // Retrieve the model's imports

    ObjRef<iface::cellml_api::CellMLImportSet> imports = mCellmlFile->model()->imports();

    if (imports->length()) {
        // Imports category

        CellmlAnnotationViewCellmlElementItem *importsItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Import,
                                                                                                       tr("Imports"));

        modelItem->appendRow(importsItem);

        // Retrieve the model's imports themselves

        ObjRef<iface::cellml_api::CellMLImportIterator> importsIter = imports->iterateImports();

        for (ObjRef<iface::cellml_api::CellMLImport> import = importsIter->nextImport();
             import; import = importsIter->nextImport()) {
            CellmlAnnotationViewCellmlElementItem *importItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Import,
                                                                                                          import);

            importsItem->appendRow(importItem);

            // Retrieve the model's import's units

            ObjRef<iface::cellml_api::ImportUnitsSet> importUnitsSet = import->units();

            if (importUnitsSet->length()) {
                // Units category

                CellmlAnnotationViewCellmlElementItem *unitsItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::ImportUnit,
                                                                                                             tr("Units"));

                importItem->appendRow(unitsItem);

                // Retrieve the model's import's units themselves

                ObjRef<iface::cellml_api::ImportUnitsIterator> importUnitsIter = importUnitsSet->iterateImportUnits();

                for (ObjRef<iface::cellml_api::ImportUnits> importUnit = importUnitsIter->nextImportUnits();
                     importUnit; importUnit = importUnitsIter->nextImportUnits()) {
                    unitsItem->appendRow(new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::ImportUnit,
                                                                                   importUnit));
                }
            }

            // Retrieve the model's import's components

            ObjRef<iface::cellml_api::ImportComponentSet> importComponents = import->components();

            if (importComponents->length()) {
                // Components category

                CellmlAnnotationViewCellmlElementItem *componentsItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::ImportComponent,
                                                                                                                  tr("Components"));

                importItem->appendRow(componentsItem);

                // Retrieve the model's import's components themselves

                ObjRef<iface::cellml_api::ImportComponentIterator> importComponentsIter = importComponents->iterateImportComponents();

                for (ObjRef<iface::cellml_api::ImportComponent> importComponent = importComponentsIter->nextImportComponent();
                     importComponent; importComponent = importComponentsIter->nextImportComponent()) {
                    componentsItem->appendRow(new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::ImportComponent,
                                                                                        importComponent));
                }
            }
        }
    }

    // Retrieve the model's units

    ObjRef<iface::cellml_api::UnitsSet> unitsSet = mCellmlFile->model()->localUnits();

    populateUnitsModel(modelItem, unitsSet);

    // Retrieve the model's components

    ObjRef<iface::cellml_api::CellMLComponentSet> components = mCellmlFile->model()->localComponents();

    if (components->length()) {
        // Components category

        CellmlAnnotationViewCellmlElementItem *componentsItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Component,
                                                                                                          tr("Components"));

        modelItem->appendRow(componentsItem);

        // Retrieve the model's components themselves

        ObjRef<iface::cellml_api::CellMLComponentIterator> componentsIter = components->iterateComponents();

        for (ObjRef<iface::cellml_api::CellMLComponent> component = componentsIter->nextComponent();
             component; component = componentsIter->nextComponent()) {
            CellmlAnnotationViewCellmlElementItem *componentItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Component,
                                                                                                             component);

            componentsItem->appendRow(componentItem);

            // Retrieve the model's component's units

            ObjRef<iface::cellml_api::UnitsSet> componentUnitsSet = component->units();

            populateUnitsModel(componentItem, componentUnitsSet);

            // Retrieve the model's component's variables

            ObjRef<iface::cellml_api::CellMLVariableSet> componentVariables = component->variables();

            if (componentVariables->length()) {
                // Variables category

                CellmlAnnotationViewCellmlElementItem *variablesItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Variable,
                                                                                                                 tr("Variables"));

                componentItem->appendRow(variablesItem);

                // Retrieve the model's component's variables themselves

                ObjRef<iface::cellml_api::CellMLVariableIterator> componentVariablesIter = componentVariables->iterateVariables();

                for (ObjRef<iface::cellml_api::CellMLVariable> componentVariable = componentVariablesIter->nextVariable();
                     componentVariable; componentVariable = componentVariablesIter->nextVariable()) {
                        variablesItem->appendRow(new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Variable,
                                                                                           componentVariable));
                }
            }
        }
    }

    // Retrieve the model's groups

    ObjRef<iface::cellml_api::GroupSet> groups = mCellmlFile->model()->groups();

    if (groups->length()) {
        // Groups category

        CellmlAnnotationViewCellmlElementItem *groupsItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Group,
                                                                                                      tr("Groups"));

        modelItem->appendRow(groupsItem);

        // Retrieve the model's groups themselves

        int counter = 0;
        ObjRef<iface::cellml_api::GroupIterator> groupsIter = groups->iterateGroups();

        for (ObjRef<iface::cellml_api::Group> group = groupsIter->nextGroup();
             group; group = groupsIter->nextGroup()) {
            CellmlAnnotationViewCellmlElementItem *groupItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Group,
                                                                                                         group, ++counter);

            groupsItem->appendRow(groupItem);

            // Retrieve the model's group's relationship references

            ObjRef<iface::cellml_api::RelationshipRefSet> groupRelationshipReferences = group->relationshipRefs();

            if (groupRelationshipReferences->length()) {
                // Relationship references category

                CellmlAnnotationViewCellmlElementItem *groupRelationshipReferencesItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::RelationshipReference,
                                                                                                                                   tr("Relationship References"));

                groupItem->appendRow(groupRelationshipReferencesItem);

                // Retrieve the model's group's relationship references
                // themselves

                ObjRef<iface::cellml_api::RelationshipRefIterator> groupRelationshipReferencesIter = groupRelationshipReferences->iterateRelationshipRefs();

                for (ObjRef<iface::cellml_api::RelationshipRef> groupRelationshipReference = groupRelationshipReferencesIter->nextRelationshipRef();
                     groupRelationshipReference; groupRelationshipReference = groupRelationshipReferencesIter->nextRelationshipRef()) {
                    groupRelationshipReferencesItem->appendRow(new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::RelationshipReference,
                                                                                                         groupRelationshipReference));
                }
            }

            // Retrieve the model's group's component references

            ObjRef<iface::cellml_api::ComponentRefSet> groupComponentReferences = group->componentRefs();

            if (groupComponentReferences->length()) {
                // Component references category

                CellmlAnnotationViewCellmlElementItem *groupComponentReferencesItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::ComponentReference,
                                                                                                                                tr("Component References"));

                groupItem->appendRow(groupComponentReferencesItem);

                // Retrieve the model's group's relationship references
                // themselves

                ObjRef<iface::cellml_api::ComponentRefIterator> groupComponentReferencesIter = groupComponentReferences->iterateComponentRefs();

                for (ObjRef<iface::cellml_api::ComponentRef> groupComponentReference = groupComponentReferencesIter->nextComponentRef();
                     groupComponentReference; groupComponentReference = groupComponentReferencesIter->nextComponentRef()) {
                    populateGroupComponentReferenceModel(groupComponentReferencesItem, groupComponentReference);
                }
            }
        }
    }

    // Retrieve the model's connections

    ObjRef<iface::cellml_api::ConnectionSet> connections = mCellmlFile->model()->connections();

    if (connections->length()) {
        // Connections category

        CellmlAnnotationViewCellmlElementItem *connectionsItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Connection,
                                                                                                           tr("Connections"));

        modelItem->appendRow(connectionsItem);

        // Retrieve the model's connections themselves

        int counter = 0;
        ObjRef<iface::cellml_api::ConnectionIterator> connectionsIter = connections->iterateConnections();

        for (ObjRef<iface::cellml_api::Connection> connection = connectionsIter->nextConnection();
             connection; connection = connectionsIter->nextConnection()) {
            CellmlAnnotationViewCellmlElementItem *connectionItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Connection,
                                                                                                              connection, ++counter);

            connectionsItem->appendRow(connectionItem);

            // Component mapping

            ObjRef<iface::cellml_api::MapComponents> connectionComponentMapping = connection->componentMapping();
            QString connectionComponentMappingFirstComponentName = QString::fromStdWString(connectionComponentMapping->firstComponentName());
            QString connectionComponentMappingSecondComponentName = QString::fromStdWString(connectionComponentMapping->secondComponentName());

            if (   !connectionComponentMappingFirstComponentName.isEmpty()
                && !connectionComponentMappingSecondComponentName.isEmpty()) {
                connectionItem->appendRow(new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::ComponentMapping,
                                                                                    connectionComponentMapping));
            }

            // Variable mappings

            ObjRef<iface::cellml_api::MapVariablesSet> connectionVariableMappings = connection->variableMappings();
            ObjRef<iface::cellml_api::MapVariablesIterator> connectionVariableMappingsIter = connectionVariableMappings->iterateMapVariables();

            for (ObjRef<iface::cellml_api::MapVariables> connectionVariableMapping = connectionVariableMappingsIter->nextMapVariables();
                 connectionVariableMapping; connectionVariableMapping = connectionVariableMappingsIter->nextMapVariables()) {
                connectionItem->appendRow(new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::VariableMapping,
                                                                                    connectionVariableMapping));
            }
        }
    }
}

//==============================================================================

void CellmlAnnotationViewCellmlListWidget::populateUnitsModel(CellmlAnnotationViewCellmlElementItem *pCellmlElementItem,
                                                              iface::cellml_api::UnitsSet *pUnitsSet)
{
    // Retrieve the units

    if (pUnitsSet->length()) {
        // Units category

        CellmlAnnotationViewCellmlElementItem *unitsItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Unit,
                                                                                                     tr("Units"));

        pCellmlElementItem->appendRow(unitsItem);

        // Retrieve the units themselves

        ObjRef<iface::cellml_api::UnitsIterator> unitsIter = pUnitsSet->iterateUnits();

        for (ObjRef<iface::cellml_api::Units> units = unitsIter->nextUnits();
             units; units = unitsIter->nextUnits()) {
            CellmlAnnotationViewCellmlElementItem *unitItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Unit,
                                                                                                        units);

            unitsItem->appendRow(unitItem);

            // Retrieve the units' unit references

            ObjRef<iface::cellml_api::UnitSet> unitSet = units->unitCollection();
            ObjRef<iface::cellml_api::UnitIterator> unitIter = unitSet->iterateUnits();

            for (ObjRef<iface::cellml_api::Unit> unit = unitIter->nextUnit();
                 unit; unit = unitIter->nextUnit()) {
                unitItem->appendRow(new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::UnitElement,
                                                                              unit));
            }
        }
    }
}

//==============================================================================

void CellmlAnnotationViewCellmlListWidget::populateGroupComponentReferenceModel(CellmlAnnotationViewCellmlElementItem *pCellmlElementItem,
                                                                                iface::cellml_api::ComponentRef *pGroupComponentReference)
{
    CellmlAnnotationViewCellmlElementItem *groupComponentReferencesItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::ComponentReference,
                                                                                                                    pGroupComponentReference);

    pCellmlElementItem->appendRow(groupComponentReferencesItem);

    // Retrieve the component reference's children

    ObjRef<iface::cellml_api::ComponentRefSet> groupComponentReferences = pGroupComponentReference->componentRefs();
    ObjRef<iface::cellml_api::ComponentRefIterator> groupComponentReferencesIter = groupComponentReferences->iterateComponentRefs();

    for (ObjRef<iface::cellml_api::ComponentRef> groupComponentReference = groupComponentReferencesIter->nextComponentRef();
         groupComponentReference; groupComponentReference = groupComponentReferencesIter->nextComponentRef()) {
        populateGroupComponentReferenceModel(groupComponentReferencesItem, groupComponentReference);
    }
}

//==============================================================================

void CellmlAnnotationViewCellmlListWidget::resizeTreeViewToContents()
{
    // Resize our tree view widget so that all of its contents is visible

    mTreeViewWidget->resizeColumnToContents(0);
}

//==============================================================================

void CellmlAnnotationViewCellmlListWidget::updateMetadataDetails(const QModelIndex &pNewIndex,
                                                                 const QModelIndex &pOldIndex)
{
    Q_UNUSED(pOldIndex);

    // Make sure that we have a valid new index

    if (!pNewIndex.isValid())
        return;

    // Let people know that we want to see the metadata associated with the
    // CellML element

    emit metadataDetailsRequested(static_cast<CellmlAnnotationViewCellmlElementItem *>(mTreeViewModel->itemFromIndex(pNewIndex))->element());
}

//==============================================================================

void CellmlAnnotationViewCellmlListWidget::showCustomContextMenu(const QPoint &pPosition) const
{
    // Make sure that we are dealing with a CellML files that doesn't have any
    // issues

    if (mCellmlFile->issues().count())
        return;

    // Determine whether to show the context menu based on whether we are over
    // an item

    CellmlAnnotationViewCellmlElementItem *item = static_cast<CellmlAnnotationViewCellmlElementItem *>(mTreeViewModel->itemFromIndex(mTreeViewWidget->indexAt(pPosition)));

    if (item) {
        // We are over an item, so create a custom context menu for our current
        // item

        // Update the enabled status of our actions

        bool fileReadableAndWritable = Core::FileManager::instance()->isReadableAndWritable(mCellmlFile->fileName());

        mExpandAllAction->setEnabled(item->hasChildren() && !indexIsAllExpanded(mTreeViewWidget->currentIndex()));
        mCollapseAllAction->setEnabled(item->hasChildren() && mTreeViewWidget->isExpanded(mTreeViewWidget->currentIndex()));

        mRemoveCurrentMetadataAction->setEnabled(fileReadableAndWritable && !item->isCategory() && mCellmlFile->rdfTriples(item->element()).count());
        mRemoveAllMetadataAction->setEnabled(fileReadableAndWritable && !item->isCategory() && mCellmlFile->rdfTriples().count());

        mOpenImportAction->setEnabled(item->type() == CellmlAnnotationViewCellmlElementItem::Import);

        // Create and show the context menu, if it isn't empty

        QMenu menu;

        if (item->hasChildren()) {
            menu.addAction(mExpandAllAction);
            menu.addAction(mCollapseAllAction);
            menu.addSeparator();
        }

        if (!item->isCategory()) {
            menu.addAction(mRemoveCurrentMetadataAction);
            menu.addAction(mRemoveAllMetadataAction);
        }

        if (item->type() == CellmlAnnotationViewCellmlElementItem::Import) {
            menu.addSeparator();
            menu.addAction(mOpenImportAction);
        }

        if (!menu.isEmpty())
            menu.exec(QCursor::pos());
    }
}

//==============================================================================

void CellmlAnnotationViewCellmlListWidget::expandAll()
{
    // Expand all the CellML elements below the current one

    indexExpandAll(mTreeViewWidget->currentIndex());
}

//==============================================================================

void CellmlAnnotationViewCellmlListWidget::collapseAll()
{
    // Collapse all the CellML elements below the current one

    indexCollapseAll(mTreeViewWidget->currentIndex());
}

//==============================================================================

void CellmlAnnotationViewCellmlListWidget::removeCurrentMetadata()
{
    // Remove all the metadata associated with the current node

    mCellmlFile->rdfTriples().remove(currentCellmlElementItem()->element());

    // Re-update the metadata details view now that the current node doesn't
    // have any metadata associated with it

    updateMetadataDetails(mTreeViewWidget->currentIndex(), QModelIndex());
}

//==============================================================================

void CellmlAnnotationViewCellmlListWidget::removeAllMetadata()
{
    // Remove all the metadata associated with the CellML file

    mCellmlFile->rdfTriples().removeAll();

    // Re-update the metadata details view now that the CellML file doesn't have
    // any metadata associated with it

    updateMetadataDetails(mTreeViewWidget->currentIndex(), QModelIndex());
}

//==============================================================================

void CellmlAnnotationViewCellmlListWidget::indexExpandAll(const QModelIndex &pIndex) const
{
    // Recursively expand all the CellML elements below the current one
    // Note: the test with against pIndex.child(0, 0) is to ensure that we are
    //       not trying to expand an index which item has no children. Indeed,
    //       a call to expand() is quite expensive, so the fewer of those we
    //       make the better...

    if (pIndex.model()->index(0, 0, pIndex).isValid()) {
        mTreeViewWidget->expand(pIndex);

        QStandardItem *item = mTreeViewModel->itemFromIndex(pIndex);

        for (int i = 0, iMax = item->rowCount(); i < iMax; ++i)
            indexExpandAll(item->child(i)->index());
    }
}

//==============================================================================

void CellmlAnnotationViewCellmlListWidget::indexCollapseAll(const QModelIndex &pIndex) const
{
    // Recursively collapse all the CellML elements below the current one
    // Note: see the note in indexExpandAll() above...

    if (pIndex.model()->index(0, 0, pIndex).isValid()) {
        QStandardItem *item = mTreeViewModel->itemFromIndex(pIndex);

        for (int i = 0, iMax = item->rowCount(); i < iMax; ++i)
            indexCollapseAll(item->child(i)->index());

        mTreeViewWidget->collapse(pIndex);
    }
}

//==============================================================================

bool CellmlAnnotationViewCellmlListWidget::indexIsAllExpanded(const QModelIndex &pIndex) const
{
    // Recursively check that the current CellML element and all of its children
    // are expanded
    // Note: see the note in indexExpandAll() above...

    if (pIndex.model()->index(0, 0, pIndex).isValid()) {
        QStandardItem *item = mTreeViewModel->itemFromIndex(pIndex);

        for (int i = 0, iMax = item->rowCount(); i < iMax; ++i)
            if (!indexIsAllExpanded(item->child(i)->index()))
                return false;

        return mTreeViewWidget->isExpanded(pIndex);
    } else {
        return true;
    }
}

//==============================================================================

Core::TreeViewWidget * CellmlAnnotationViewCellmlListWidget::treeViewWidget() const
{
    // Return our tree view widget

    return mTreeViewWidget;
}

//==============================================================================

CellmlAnnotationViewCellmlElementItem * CellmlAnnotationViewCellmlListWidget::currentCellmlElementItem() const
{
    // Return the current CellML element item

    return static_cast<CellmlAnnotationViewCellmlElementItem *>(mTreeViewModel->itemFromIndex(mTreeViewWidget->currentIndex()));
}

//==============================================================================

void CellmlAnnotationViewCellmlListWidget::openImport()
{
    // Ask OpenCOR to open the imported file

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();
    QUrl url = fileManagerInstance->isRemote(mCellmlFile->fileName())?
                   fileManagerInstance->url(mCellmlFile->fileName()):
                   QUrl::fromLocalFile(mCellmlFile->fileName());

    url = url.resolved(currentCellmlElementItem()->text());

    QDesktopServices::openUrl("opencor://openFile/"+url.toString());
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
