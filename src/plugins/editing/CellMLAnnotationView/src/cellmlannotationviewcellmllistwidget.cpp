/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

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
    Core::StyledItemDelegate(pParent)
{
}

//==============================================================================

void CellmlAnnotationViewCellmlElementItemDelegate::paint(QPainter *pPainter,
                                                          const QStyleOptionViewItem &pOption,
                                                          const QModelIndex &pIndex) const
{
    // Paint the item as normal, except for error/warning/category items

    auto cellmlElementItem = static_cast<CellmlAnnotationViewCellmlElementItem *>(qobject_cast<const QStandardItemModel *>(pIndex.model())->itemFromIndex(pIndex));

    QStyleOptionViewItem option(pOption);

    initStyleOption(&option, pIndex);

    if (   (cellmlElementItem->type() == int(CellmlAnnotationViewCellmlElementItem::Type::Error))
        || (cellmlElementItem->type() == int(CellmlAnnotationViewCellmlElementItem::Type::Warning))) {
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

CellmlAnnotationViewCellmlElementItem::CellmlAnnotationViewCellmlElementItem(bool pCategory,
                                                                             Type pType,
                                                                             const QString &pText,
                                                                             iface::cellml_api::CellMLElement *pElement,
                                                                             int pNumber) :
    QStandardItem(pText),
    mCategory(pCategory),
    mType(pType),
    mElement(pElement),
    mNumber(pNumber)
{
}

//==============================================================================

CellmlAnnotationViewCellmlElementItem::CellmlAnnotationViewCellmlElementItem(bool pError,
                                                                             const QString &pText) :
    CellmlAnnotationViewCellmlElementItem(false, pError?Type::Error:Type::Warning, pText, nullptr, -1)
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

CellmlAnnotationViewCellmlElementItem::CellmlAnnotationViewCellmlElementItem(Type pType,
                                                                             const QString &pText) :
    CellmlAnnotationViewCellmlElementItem(true, pType, pText, nullptr, -1)
{
    // Use its text as a tooltip (in case it's too long and doesn't fit within
    // the allocated space we have)

    setToolTip(pText);

    // Set the icon for the item

    setIcon(pType);
}

//==============================================================================

CellmlAnnotationViewCellmlElementItem::CellmlAnnotationViewCellmlElementItem(Type pType,
                                                                             iface::cellml_api::CellMLElement *pElement,
                                                                             int pNumber) :
    CellmlAnnotationViewCellmlElementItem(false, pType, QString(), pElement, pNumber)
{
    // Set the text for some types

    enum {
        RightArrow = 0x2192
    };

    if (pType == Type::Import) {
        ObjRef<iface::cellml_api::URI> xlinkHref = dynamic_cast<iface::cellml_api::CellMLImport *>(pElement)->xlinkHref();

        setText(QString::fromStdWString(xlinkHref->asText()));
    } else if (pType == Type::UnitElement) {
        setText(QString::fromStdWString(dynamic_cast<iface::cellml_api::Unit *>(pElement)->units()));
    } else if (pType == Type::Group) {
        setText(tr("Group #%1").arg(pNumber));
    } else if (pType == Type::RelationshipReference) {
        QString name = QString::fromStdWString(dynamic_cast<iface::cellml_api::RelationshipRef *>(pElement)->name());

        setText( QString::fromStdWString(dynamic_cast<iface::cellml_api::RelationshipRef *>(pElement)->relationship())
                +(name.isEmpty()?QString():" "+name));
    } else if (pType == Type::ComponentReference) {
        setText(QString::fromStdWString(dynamic_cast<iface::cellml_api::ComponentRef *>(pElement)->componentName()));
    } else if (pType == Type::Connection) {
        setText(tr("Connection #%1").arg(pNumber));
    } else if (pType == Type::ComponentMapping) {
        ObjRef<iface::cellml_api::MapComponents> mapComponents = dynamic_cast<iface::cellml_api::MapComponents *>(pElement);

        setText(QString("%1 %2 %3").arg(QString::fromStdWString(mapComponents->firstComponentName()),
                                        QChar(RightArrow),
                                        QString::fromStdWString(mapComponents->secondComponentName())));
    } else if (pType == Type::VariableMapping) {
        ObjRef<iface::cellml_api::MapVariables> mapVariables = dynamic_cast<iface::cellml_api::MapVariables *>(pElement);

        setText(QString("%1 %2 %3").arg(QString::fromStdWString(mapVariables->firstVariableName()),
                                        QChar(RightArrow),
                                        QString::fromStdWString(mapVariables->secondVariableName())));
    } else {
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

void CellmlAnnotationViewCellmlElementItem::setIcon(Type pType)
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
    case Type::Error:
        QStandardItem::setIcon(ErrorIcon);

        break;
    case Type::Warning:
        QStandardItem::setIcon(WarningIcon);

        break;
    case Type::Model:
        QStandardItem::setIcon(ModelIcon);

        break;
    case Type::Import:
        QStandardItem::setIcon(ImportIcon);

        break;
    case Type::ImportUnit:
    case Type::Unit:
        QStandardItem::setIcon(UnitIcon);

        break;
    case Type::UnitElement:
        QStandardItem::setIcon(UnitElementIcon);

        break;
    case Type::ImportComponent:
    case Type::Component:
    case Type::ComponentMapping:
        QStandardItem::setIcon(ComponentIcon);

        break;
    case Type::Variable:
    case Type::VariableMapping:
        QStandardItem::setIcon(VariableIcon);

        break;
    case Type::Group:
        QStandardItem::setIcon(GroupIcon);

        break;
    case Type::RelationshipReference:
        QStandardItem::setIcon(RelationshipIcon);

        break;
    case Type::ComponentReference:
        QStandardItem::setIcon(ComponentIcon);

        break;
    case Type::Connection:
        QStandardItem::setIcon(ConnectionIcon);

        break;
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

    return int(mType);
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

    connect(mTreeViewWidget, &Core::TreeViewWidget::customContextMenuRequested,
            this, &CellmlAnnotationViewCellmlListWidget::showCustomContextMenu);

    // Create our actions

    mExpandAllAction = Core::newAction(QIcon(":/oxygen/actions/list-add.png"),
                                       this);
    mCollapseAllAction = Core::newAction(QIcon(":/oxygen/actions/list-remove.png"),
                                         this);
    mRemoveCurrentMetadataAction = Core::newAction(this);
    mRemoveAllMetadataAction = Core::newAction(this);
    mOpenImportAction = Core::newAction(this);

    connect(mExpandAllAction, &QAction::triggered,
            this, &CellmlAnnotationViewCellmlListWidget::expandAll);
    connect(mCollapseAllAction, &QAction::triggered,
            this, &CellmlAnnotationViewCellmlListWidget::collapseAll);
    connect(mRemoveCurrentMetadataAction, &QAction::triggered,
            this, &CellmlAnnotationViewCellmlListWidget::removeCurrentMetadata);
    connect(mRemoveAllMetadataAction, &QAction::triggered,
            this, &CellmlAnnotationViewCellmlListWidget::removeAllMetadata);
    connect(mOpenImportAction, &QAction::triggered,
            this, &CellmlAnnotationViewCellmlListWidget::openImport);

    // Some connections to handle the expansion/collapse of a CellML element

    connect(mTreeViewWidget, &Core::TreeViewWidget::expanded,
            this, &CellmlAnnotationViewCellmlListWidget::resizeTreeViewToContents);
    connect(mTreeViewWidget, &Core::TreeViewWidget::collapsed,
            this, &CellmlAnnotationViewCellmlListWidget::resizeTreeViewToContents);

    // Some connections to handle the change of CellML element

    connect(mTreeViewWidget->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &CellmlAnnotationViewCellmlListWidget::updateMetadataDetails);

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
    // Retranslate our CellML element's children

    for (int i = 0, iMax = pCellmlElementItem->rowCount(); i < iMax; ++i) {
        retranslateDataItem(static_cast<CellmlAnnotationViewCellmlElementItem *>(pCellmlElementItem->child(i)));
    }

    // Retranslate our CellML element item, if it's not our model's invisible
    // root item

    if (pCellmlElementItem == mTreeViewModel->invisibleRootItem()) {
        return;
    }

    if (pCellmlElementItem->isCategory()) {
        // We are dealing with a category, so retranslate its type

        auto categoryType = CellmlAnnotationViewCellmlElementItem::Type(pCellmlElementItem->type());

        if (categoryType == CellmlAnnotationViewCellmlElementItem::Type::Import) {
            pCellmlElementItem->setText(tr("Imports"));
        } else if (categoryType == CellmlAnnotationViewCellmlElementItem::Type::Unit) {
            pCellmlElementItem->setText(tr("Units"));
        } else if (categoryType == CellmlAnnotationViewCellmlElementItem::Type::Component) {
            pCellmlElementItem->setText(tr("Components"));
        } else if (categoryType == CellmlAnnotationViewCellmlElementItem::Type::Variable) {
            pCellmlElementItem->setText(tr("Variables"));
        } else if (categoryType == CellmlAnnotationViewCellmlElementItem::Type::Group) {
            pCellmlElementItem->setText(tr("Groups"));
        } else if (categoryType == CellmlAnnotationViewCellmlElementItem::Type::RelationshipReference) {
            pCellmlElementItem->setText(tr("Relationship References"));
        } else if (categoryType == CellmlAnnotationViewCellmlElementItem::Type::ComponentReference) {
            pCellmlElementItem->setText(tr("Component References"));
        } else if (categoryType == CellmlAnnotationViewCellmlElementItem::Type::Connection) {
            pCellmlElementItem->setText(tr("Connections"));
        }
    } else {
        // We are not dealing with a category, so check the type and see whether
        // a CellML element needs retranslating

        auto type = CellmlAnnotationViewCellmlElementItem::Type(pCellmlElementItem->type());

        if (type == CellmlAnnotationViewCellmlElementItem::Type::Group) {
            pCellmlElementItem->setText(tr("Group #%1").arg(pCellmlElementItem->number()));
        } else if (type == CellmlAnnotationViewCellmlElementItem::Type::Connection) {
            pCellmlElementItem->setText(tr("Connection #%1").arg(pCellmlElementItem->number()));
        }
    }
}

//==============================================================================

void CellmlAnnotationViewCellmlListWidget::initializeTreeViewWidget(bool pSelectFirstItem)
{
    // Populate our tree view widget

    populateModel();

    // Expand our tree view widget enough so that we can see the meaningful
    // parts of the CellML file

    mTreeViewWidget->expandToDepth(1);

    // Resize our tree view widget, just to be on the safe side

    resizeTreeViewToContents();

    // Select the first item of our tree view widget

    if (pSelectFirstItem) {
        mTreeViewWidget->selectFirstItem();
    }
}

//==============================================================================

void CellmlAnnotationViewCellmlListWidget::populateModel()
{
    // Make sure that we have a model before actually populating ourselves

    iface::cellml_api::Model *cellmlModel = mCellmlFile->model();

    if (cellmlModel == nullptr) {
        return;
    }

    // Retrieve the model's root

    auto modelItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::Model,
                                                               cellmlModel);

    mTreeViewModel->invisibleRootItem()->appendRow(modelItem);

    // Retrieve the model's imports

    ObjRef<iface::cellml_api::CellMLImportSet> imports = cellmlModel->imports();

    if (imports->length() != 0) {
        // Imports category

        auto importsItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::Import,
                                                                     tr("Imports"));

        modelItem->appendRow(importsItem);

        // Retrieve the model's imports themselves

        ObjRef<iface::cellml_api::CellMLImportIterator> importsIter = imports->iterateImports();

        for (ObjRef<iface::cellml_api::CellMLImport> import = importsIter->nextImport();
             import != nullptr; import = importsIter->nextImport()) {
            auto importItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::Import,
                                                                        import);

            importsItem->appendRow(importItem);

            // Retrieve the model's import's units

            ObjRef<iface::cellml_api::ImportUnitsSet> importUnitsSet = import->units();

            if (importUnitsSet->length() != 0) {
                // Units category

                auto unitsItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::ImportUnit,
                                                                           tr("Units"));

                importItem->appendRow(unitsItem);

                // Retrieve the model's import's units themselves

                ObjRef<iface::cellml_api::ImportUnitsIterator> importUnitsIter = importUnitsSet->iterateImportUnits();

                for (ObjRef<iface::cellml_api::ImportUnits> importUnit = importUnitsIter->nextImportUnits();
                     importUnit != nullptr; importUnit = importUnitsIter->nextImportUnits()) {
                    unitsItem->appendRow(new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::ImportUnit,
                                                                                   importUnit));
                }
            }

            // Retrieve the model's import's components

            ObjRef<iface::cellml_api::ImportComponentSet> importComponents = import->components();

            if (importComponents->length() != 0) {
                // Components category

                auto componentsItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::ImportComponent,
                                                                                tr("Components"));

                importItem->appendRow(componentsItem);

                // Retrieve the model's import's components themselves

                ObjRef<iface::cellml_api::ImportComponentIterator> importComponentsIter = importComponents->iterateImportComponents();

                for (ObjRef<iface::cellml_api::ImportComponent> importComponent = importComponentsIter->nextImportComponent();
                     importComponent != nullptr; importComponent = importComponentsIter->nextImportComponent()) {
                    componentsItem->appendRow(new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::ImportComponent,
                                                                                        importComponent));
                }
            }
        }
    }

    // Retrieve the model's units

    ObjRef<iface::cellml_api::UnitsSet> unitsSet = cellmlModel->localUnits();

    populateUnitsModel(modelItem, unitsSet);

    // Retrieve the model's components

    ObjRef<iface::cellml_api::CellMLComponentSet> components = cellmlModel->localComponents();

    if (components->length() != 0) {
        // Components category

        auto componentsItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::Component,
                                                                        tr("Components"));

        modelItem->appendRow(componentsItem);

        // Retrieve the model's components themselves

        ObjRef<iface::cellml_api::CellMLComponentIterator> componentsIter = components->iterateComponents();

        for (ObjRef<iface::cellml_api::CellMLComponent> component = componentsIter->nextComponent();
             component != nullptr; component = componentsIter->nextComponent()) {
            auto componentItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::Component,
                                                                           component);

            componentsItem->appendRow(componentItem);

            // Retrieve the model's component's units

            ObjRef<iface::cellml_api::UnitsSet> componentUnitsSet = component->units();

            populateUnitsModel(componentItem, componentUnitsSet);

            // Retrieve the model's component's variables

            ObjRef<iface::cellml_api::CellMLVariableSet> componentVariables = component->variables();

            if (componentVariables->length() != 0) {
                // Variables category

                auto variablesItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::Variable,
                                                                               tr("Variables"));

                componentItem->appendRow(variablesItem);

                // Retrieve the model's component's variables themselves

                ObjRef<iface::cellml_api::CellMLVariableIterator> componentVariablesIter = componentVariables->iterateVariables();

                for (ObjRef<iface::cellml_api::CellMLVariable> componentVariable = componentVariablesIter->nextVariable();
                     componentVariable != nullptr; componentVariable = componentVariablesIter->nextVariable()) {
                    variablesItem->appendRow(new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::Variable,
                                                                                       componentVariable));
                }
            }
        }
    }

    // Retrieve the model's groups

    ObjRef<iface::cellml_api::GroupSet> groups = cellmlModel->groups();

    if (groups->length() != 0) {
        // Groups category

        auto groupsItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::Group,
                                                                    tr("Groups"));

        modelItem->appendRow(groupsItem);

        // Retrieve the model's groups themselves

        int counter = 0;
        ObjRef<iface::cellml_api::GroupIterator> groupsIter = groups->iterateGroups();

        for (ObjRef<iface::cellml_api::Group> group = groupsIter->nextGroup();
             group != nullptr; group = groupsIter->nextGroup()) {
            auto groupItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::Group,
                                                                       group, ++counter);

            groupsItem->appendRow(groupItem);

            // Retrieve the model's group's relationship references

            ObjRef<iface::cellml_api::RelationshipRefSet> groupRelationshipReferences = group->relationshipRefs();

            if (groupRelationshipReferences->length() != 0) {
                // Relationship references category

                auto groupRelationshipReferencesItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::RelationshipReference,
                                                                                                 tr("Relationship References"));

                groupItem->appendRow(groupRelationshipReferencesItem);

                // Retrieve the model's group's relationship references
                // themselves

                ObjRef<iface::cellml_api::RelationshipRefIterator> groupRelationshipReferencesIter = groupRelationshipReferences->iterateRelationshipRefs();

                for (ObjRef<iface::cellml_api::RelationshipRef> groupRelationshipReference = groupRelationshipReferencesIter->nextRelationshipRef();
                     groupRelationshipReference != nullptr; groupRelationshipReference = groupRelationshipReferencesIter->nextRelationshipRef()) {
                    groupRelationshipReferencesItem->appendRow(new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::RelationshipReference,
                                                                                                         groupRelationshipReference));
                }
            }

            // Retrieve the model's group's component references

            ObjRef<iface::cellml_api::ComponentRefSet> groupComponentReferences = group->componentRefs();

            if (groupComponentReferences->length() != 0) {
                // Component references category

                auto groupComponentReferencesItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::ComponentReference,
                                                                                              tr("Component References"));

                groupItem->appendRow(groupComponentReferencesItem);

                // Retrieve the model's group's relationship references
                // themselves

                ObjRef<iface::cellml_api::ComponentRefIterator> groupComponentReferencesIter = groupComponentReferences->iterateComponentRefs();

                for (ObjRef<iface::cellml_api::ComponentRef> groupComponentReference = groupComponentReferencesIter->nextComponentRef();
                     groupComponentReference != nullptr; groupComponentReference = groupComponentReferencesIter->nextComponentRef()) {
                    populateGroupComponentReferenceModel(groupComponentReferencesItem, groupComponentReference);
                }
            }
        }
    }

    // Retrieve the model's connections

    ObjRef<iface::cellml_api::ConnectionSet> connections = cellmlModel->connections();

    if (connections->length() != 0) {
        // Connections category

        auto connectionsItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::Connection,
                                                                         tr("Connections"));

        modelItem->appendRow(connectionsItem);

        // Retrieve the model's connections themselves

        int counter = 0;
        ObjRef<iface::cellml_api::ConnectionIterator> connectionsIter = connections->iterateConnections();

        for (ObjRef<iface::cellml_api::Connection> connection = connectionsIter->nextConnection();
             connection != nullptr; connection = connectionsIter->nextConnection()) {
            auto connectionItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::Connection,
                                                                            connection, ++counter);

            connectionsItem->appendRow(connectionItem);

            // Component mapping

            ObjRef<iface::cellml_api::MapComponents> connectionComponentMapping = connection->componentMapping();
            QString connectionComponentMappingFirstComponentName = QString::fromStdWString(connectionComponentMapping->firstComponentName());
            QString connectionComponentMappingSecondComponentName = QString::fromStdWString(connectionComponentMapping->secondComponentName());

            if (   !connectionComponentMappingFirstComponentName.isEmpty()
                && !connectionComponentMappingSecondComponentName.isEmpty()) {
                connectionItem->appendRow(new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::ComponentMapping,
                                                                                    connectionComponentMapping));
            }

            // Variable mappings

            ObjRef<iface::cellml_api::MapVariablesSet> connectionVariableMappings = connection->variableMappings();
            ObjRef<iface::cellml_api::MapVariablesIterator> connectionVariableMappingsIter = connectionVariableMappings->iterateMapVariables();

            for (ObjRef<iface::cellml_api::MapVariables> connectionVariableMapping = connectionVariableMappingsIter->nextMapVariables();
                 connectionVariableMapping != nullptr; connectionVariableMapping = connectionVariableMappingsIter->nextMapVariables()) {
                connectionItem->appendRow(new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::VariableMapping,
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

    if (pUnitsSet->length() != 0) {
        // Units category

        auto unitsItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::Unit,
                                                                   tr("Units"));

        pCellmlElementItem->appendRow(unitsItem);

        // Retrieve the units themselves

        ObjRef<iface::cellml_api::UnitsIterator> unitsIter = pUnitsSet->iterateUnits();

        for (ObjRef<iface::cellml_api::Units> units = unitsIter->nextUnits();
             units != nullptr; units = unitsIter->nextUnits()) {
            auto unitItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::Unit,
                                                                      units);

            unitsItem->appendRow(unitItem);

            // Retrieve the units' unit references

            ObjRef<iface::cellml_api::UnitSet> unitSet = units->unitCollection();
            ObjRef<iface::cellml_api::UnitIterator> unitIter = unitSet->iterateUnits();

            for (ObjRef<iface::cellml_api::Unit> unit = unitIter->nextUnit();
                 unit != nullptr; unit = unitIter->nextUnit()) {
                unitItem->appendRow(new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::UnitElement,
                                                                              unit));
            }
        }
    }
}

//==============================================================================

void CellmlAnnotationViewCellmlListWidget::populateGroupComponentReferenceModel(CellmlAnnotationViewCellmlElementItem *pCellmlElementItem,
                                                                                iface::cellml_api::ComponentRef *pGroupComponentReference)
{
    auto groupComponentReferencesItem = new CellmlAnnotationViewCellmlElementItem(CellmlAnnotationViewCellmlElementItem::Type::ComponentReference,
                                                                                  pGroupComponentReference);

    pCellmlElementItem->appendRow(groupComponentReferencesItem);

    // Retrieve the component reference's children

    ObjRef<iface::cellml_api::ComponentRefSet> groupComponentReferences = pGroupComponentReference->componentRefs();
    ObjRef<iface::cellml_api::ComponentRefIterator> groupComponentReferencesIter = groupComponentReferences->iterateComponentRefs();

    for (ObjRef<iface::cellml_api::ComponentRef> groupComponentReference = groupComponentReferencesIter->nextComponentRef();
         groupComponentReference != nullptr; groupComponentReference = groupComponentReferencesIter->nextComponentRef()) {
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
    Q_UNUSED(pOldIndex)

    // Make sure that we have a valid new index

    if (!pNewIndex.isValid()) {
        return;
    }

    // Let people know that we want to see the metadata associated with the
    // CellML element

    emit metadataDetailsRequested(static_cast<CellmlAnnotationViewCellmlElementItem *>(mTreeViewModel->itemFromIndex(pNewIndex))->element());
}

//==============================================================================

void CellmlAnnotationViewCellmlListWidget::showCustomContextMenu(const QPoint &pPosition) const
{
    // Make sure that we are dealing with a CellML files that doesn't have any
    // issues

    if (!mCellmlFile->issues().isEmpty()) {
        return;
    }

    // Determine whether to show the context menu based on whether we are over
    // an item

    CellmlAnnotationViewCellmlElementItem *item = static_cast<CellmlAnnotationViewCellmlElementItem *>(mTreeViewModel->itemFromIndex(mTreeViewWidget->indexAt(pPosition)));

    if (item != nullptr) {
        // We are over an item, so create a custom context menu for our current
        // item

        // Update the enabled status of our actions

        bool fileReadableAndWritable = Core::FileManager::instance()->isReadableAndWritable(mCellmlFile->fileName());

        mExpandAllAction->setEnabled(item->hasChildren() && !indexIsAllExpanded(mTreeViewWidget->currentIndex()));
        mCollapseAllAction->setEnabled(item->hasChildren() && mTreeViewWidget->isExpanded(mTreeViewWidget->currentIndex()));

        mRemoveCurrentMetadataAction->setEnabled(fileReadableAndWritable && !item->isCategory() && !mCellmlFile->rdfTriples(item->element()).isEmpty());
        mRemoveAllMetadataAction->setEnabled(fileReadableAndWritable && !item->isCategory() && !mCellmlFile->rdfTriples().isEmpty());

        mOpenImportAction->setEnabled(item->type() == int(CellmlAnnotationViewCellmlElementItem::Type::Import));

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

        if (item->type() == int(CellmlAnnotationViewCellmlElementItem::Type::Import)) {
            menu.addSeparator();
            menu.addAction(mOpenImportAction);
        }

        if (!menu.isEmpty()) {
            menu.exec(QCursor::pos());
        }
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

        for (int i = 0, iMax = item->rowCount(); i < iMax; ++i) {
            indexExpandAll(item->child(i)->index());
        }
    }
}

//==============================================================================

void CellmlAnnotationViewCellmlListWidget::indexCollapseAll(const QModelIndex &pIndex) const
{
    // Recursively collapse all the CellML elements below the current one
    // Note: see the note in indexExpandAll() above...

    if (pIndex.model()->index(0, 0, pIndex).isValid()) {
        QStandardItem *item = mTreeViewModel->itemFromIndex(pIndex);

        for (int i = 0, iMax = item->rowCount(); i < iMax; ++i) {
            indexCollapseAll(item->child(i)->index());
        }

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

        for (int i = 0, iMax = item->rowCount(); i < iMax; ++i) {
            if (!indexIsAllExpanded(item->child(i)->index())) {
                return false;
            }
        }

        return mTreeViewWidget->isExpanded(pIndex);
    }

    return true;
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

} // namespace CellMLAnnotationView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
