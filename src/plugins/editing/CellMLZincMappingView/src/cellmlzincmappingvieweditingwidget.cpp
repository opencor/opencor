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
// CellML-Zinc Mapping view editing widget
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlzincmappingvieweditingwidget.h"
#include "cellmlzincmappingviewwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "filemanager.h"
#include "interfaces.h"
#include "toolbarwidget.h"
#include "zincwidget.h"

//==============================================================================

#include <QApplication>
#include <QDesktopServices>
#include <QDragEnterEvent>
#include <QFileInfo>
#include <QFormLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLineEdit>
#include <QMimeData>
#include <QScreen>

//==============================================================================

#include "zincbegin.h"
    #include "opencmiss/zinc/fieldconstant.hpp"
    #include "opencmiss/zinc/fieldmodule.hpp"
    #include "opencmiss/zinc/fieldvectoroperators.hpp"
    #include "opencmiss/zinc/status.h"
#include "zincend.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLZincMappingView {

//==============================================================================

CellMLZincMappingViewEditingModel::CellMLZincMappingViewEditingModel(QObject *pParent) :
    QStandardItemModel(pParent)
{
}

//==============================================================================

QStringList CellMLZincMappingViewEditingModel::mimeTypes() const
{
    // Return the MIME type supported by our model

    return { CellMLZincMappingViewEditingMimeType };
}

//==============================================================================

QMimeData * CellMLZincMappingViewEditingModel::mimeData(const QModelIndexList &pIndexes) const
{
    auto res = new QMimeData();
    QString names;

    for (const auto &index : pIndexes) {
        names.append(itemFromIndex(index)->text()+"|"+itemFromIndex(index)->accessibleDescription());
    }

    res->setData(CellMLZincMappingViewEditingMimeType, names.toUtf8());

    return res;
}

//==============================================================================

CellMLZincMappingViewEditingWidget::CellMLZincMappingViewEditingWidget(const QString &pFileName,
                                                                       const QStringList &pMeshFileNames,
                                                                       QWidget *pParent,
                                                                       CellMLZincMappingViewWidget *pViewWidget) :
    Core::Widget(pParent),
    mViewWidget(pViewWidget),
    mZincMeshFileNames(pMeshFileNames)
{
    mCellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);

    //create and fill layout

    QLayout *layout = createLayout();

        //create and add toolbar

        mToolBarWidget = new Core::ToolBarWidget();

        //create and add widget and actions

        QRect availableGeometry = qApp->primaryScreen()->availableGeometry();

        mSaveMapping = Core::newAction(QIcon(":/oxygen/actions/document-save.png"),
                                       mToolBarWidget);

        mOpenMeshFile = Core::newAction(QIcon(":/oxygen/actions/document-open.png"),
                                        mToolBarWidget);

        //TODO trash could be hidden when nothing selected
        mClearNode = Core::newAction(QIcon(":/oxygen/actions/edit-clear.png"),
                                                        mToolBarWidget);

        mOpenMappingFile = Core::newAction(QIcon(":/oxygen/actions/document-import.png"),
                                           mToolBarWidget);
        mNodeSizeWidget = new QwtWheel(mToolBarWidget);
            mNodeSizeWidget->setBorderWidth(0);
            mNodeSizeWidget->setFixedSize(int(0.07*availableGeometry.width()),
                                       mNodeSizeWidget->height()/2);
            mNodeSizeWidget->setFocusPolicy(Qt::NoFocus);
            mNodeSizeWidget->setRange(-50.0, 100.0);
            mNodeSizeWidget->setWheelBorderWidth(0);
            mNodeSizeWidget->setValue(CellMLZincMappingViewZincWidget::nodeSizeOrigin);

        mToolBarWidget->addAction(mSaveMapping);
        mToolBarWidget->addAction(mOpenMeshFile);
        mToolBarWidget->addAction(mClearNode);

        mToolBarWidget->addWidget(mNodeSizeWidget);
        mToolBarWidget->addAction(mOpenMappingFile);

    mTopSeparator = Core::newLineWidget(this);

    layout->addWidget(mToolBarWidget);
    layout->addWidget(mTopSeparator);

    //create horizontal splitterwidget

    mHorizontalSplitterWidget = new Core::SplitterWidget(Qt::Horizontal, this);

        connect(mHorizontalSplitterWidget, &Core::SplitterWidget::splitterMoved,
                this, &CellMLZincMappingViewEditingWidget::emitHorizontalSplitterMoved);

        //create vertical splitterwidget

        mVerticalSplitterWidget = new Core::SplitterWidget(Qt::Vertical, this);

        connect(mVerticalSplitterWidget, &Core::SplitterWidget::splitterMoved,
                this, &CellMLZincMappingViewEditingWidget::emitVerticalSplitterMoved);

        // create vertical layout, for the left column

        Core::Widget *verticalWidget = new Core::Widget(this);
        QBoxLayout *verticalLayout = new QVBoxLayout(verticalWidget);

            //create and add the variable tree:

            mVariableTree = new QTreeView(this);
            mVariableTree->setDragEnabled(true);
            mVariableTree->setEditTriggers(QTreeView::NoEditTriggers);

            mVariableTreeModel = new CellMLZincMappingViewEditingModel();
            mVariableTree->setModel(mVariableTreeModel);
            mVariableTree->setHeaderHidden(true);

            verticalLayout->addWidget(mVariableTree);

            //create add and connect filter line edit

            mFilterLineEdit = new QLineEdit(this);
            verticalLayout->addWidget(mFilterLineEdit);

            connect(mFilterLineEdit, &QLineEdit::textChanged,
                    this, &CellMLZincMappingViewEditingWidget::filterChanged);

        mHorizontalSplitterWidget->addWidget(verticalWidget);

        // add a Zinc widget

        mZincWidget = new CellMLZincMappingViewZincWidget(this, mZincMeshFileNames, this);

        connect(mClearNode, &QAction::triggered,
                mZincWidget, &CellMLZincMappingViewZincWidget::eraseNodeSlot);
        connect(mNodeSizeWidget, &QwtWheel::valueChanged,
                mZincWidget, &CellMLZincMappingViewZincWidget::setNodeSizes);

        mHorizontalSplitterWidget->addWidget(mZincWidget);

        //add the other connectionss

        connect(mSaveMapping, &QAction::triggered,
                this, &CellMLZincMappingViewEditingWidget::saveMappingSlot);
        connect(mOpenMeshFile, &QAction::triggered,
                this, &CellMLZincMappingViewEditingWidget::openMeshFile);
        connect(mOpenMappingFile, &QAction::triggered,
                this, &CellMLZincMappingViewEditingWidget::openMappingFile);

        //create and add informative labels

        Core::Widget *labelWidget = new Core::Widget(this);
        QFormLayout *labelLayout = new QFormLayout(labelWidget);

        QLabel *nodeLabel = new QLabel("Node:",this);
        mNodeValue = new QLabel(this);
        labelLayout->addRow(nodeLabel,mNodeValue);

        QLabel *componentLabel = new QLabel("Component:",this);
        mComponentValue = new QLabel(this);
        labelLayout->addRow(componentLabel,mComponentValue);

        QLabel *variableLabel = new QLabel("Variable:",this);
        mVariableValue = new QLabel(this);
        labelLayout->addRow(variableLabel,mVariableValue);

        //fill vertical Splitter

        mBottomSeparator = Core::newLineWidget(this);

        mVerticalSplitterWidget->addWidget(mHorizontalSplitterWidget);
        mVerticalSplitterWidget->addWidget(mBottomSeparator);
        mVerticalSplitterWidget->addWidget(labelWidget);

    layout->addWidget(mVerticalSplitterWidget);

    populateTree();

    // Allow for things to be dropped on us

    setAcceptDrops(true);
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::retranslateUi()
{
    //TODO ?
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::setNodeValue(const int pId, const QString &pComponent, const QString &pVariable)
{
    if (pId!=-1) {
        mMapMatch.insert(pId,{pComponent,pVariable});
        selectNode(pId);
    }
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::eraseNodeValue(const int pId)
{
    if (pId!=-1) {
        mMapMatch.remove(pId);
        selectNode(pId);
    }
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::setWheelPosition(int pValue)
{
    mNodeSizeWidget->setValue(pValue);
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::filePermissionsChanged()
{
}

//==============================================================================

bool CellMLZincMappingViewEditingWidget::setMeshFiles(const QStringList &pFileNames, bool pShowWarning)
{
    //TODO warnings ?
Q_UNUSED(pShowWarning)

    if (pFileNames.isEmpty()) {
        return false;
    }

    for (QString fileName : pFileNames) {
        QFileInfo check_file;
        check_file.setFile(fileName);

        if (!check_file.exists()) {
            return false;
        }
    }

    mZincMeshFileNames = pFileNames;
    mZincWidget->changeSource(pFileNames);
    mMapMatch.clear();
    selectNode(-1);
    mViewWidget->setDefaultMeshFiles(pFileNames);
    return true;
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::setSizes(const QIntList &pSizesHorizontal,
                                                  const QIntList &pSizesVertical)
{
    mHorizontalSplitterWidget->setSizes(pSizesHorizontal);
    mVerticalSplitterWidget->setSizes(pSizesVertical);
}

//==============================================================================

CellMLZincMappingViewZincWidget * CellMLZincMappingViewEditingWidget::getZincWidget()
{
    return mZincWidget;
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::dragEnterEvent(QDragEnterEvent *pEvent)
{
    // Accept the proposed action for the event, but only if it refers to one or
    // several data store files

    bool acceptEvent = false;

    for (const auto &fileName : Core::droppedFileNames(pEvent)) {
        for (auto fileTypeInterface : Core::dataStoreFileTypeInterfaces()) {
            if (fileTypeInterface->isFile(fileName)) {
                mFileTypeInterfaces.insert(fileName, fileTypeInterface);

                acceptEvent = true;

                break;
            }
        }

        //TODO
        if (fileName.contains(".exelem")||fileName.contains(".exnode")||fileName.contains(".exfile")) {
            acceptEvent = true;
        }
        if (fileName.contains(".json")) {
            acceptEvent = true;
        }

    }

    if (acceptEvent) {
        pEvent->acceptProposedAction();
    } else {
        pEvent->ignore();
    }
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::dragMoveEvent(QDragMoveEvent *pEvent)
{
    // Accept the proposed action for the event

    pEvent->acceptProposedAction();
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::dropEvent(QDropEvent *pEvent)
{
    // Import/open the one or several files
    QStringList meshFileList;
    for (const auto &fileName : Core::droppedFileNames(pEvent)) {
        if (fileName.contains(".exelem")||fileName.contains(".exnode")||fileName.contains(".exfile")) {
            meshFileList.append(fileName);
        } else if (fileName.contains(".json")) {
            openMapping(fileName);
        } else if (mFileTypeInterfaces.contains(fileName)) {
            //import(fileName); //?
            //TODO
        } else {
            QDesktopServices::openUrl("opencor://openFile/"+fileName);
        }
    }

    if (!meshFileList.isEmpty()) {
        setMeshFiles(meshFileList);
    }

    // Accept the proposed action for the event

    pEvent->acceptProposedAction();
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::populateTree()
{

    // Make sure that we have a model before actually populating ourselves

    iface::cellml_api::Model *cellmlModel = mCellmlFile->model();

    if (cellmlModel == nullptr) {
        return;
    }

    mVariableTreeModel->clear();
    mVariableTree->setSelectionMode(QAbstractItemView::SingleSelection);

    // Retrieve the model's components

    ObjRef<iface::cellml_api::CellMLComponentSet> components = cellmlModel->localComponents();

    if (components->length() != 0) {

        // Retrieve the model's components themselves

        ObjRef<iface::cellml_api::CellMLComponentIterator> componentsIter = components->iterateComponents();

        for (ObjRef<iface::cellml_api::CellMLComponent> component = componentsIter->nextComponent();
            component != nullptr; component = componentsIter->nextComponent()) {

            // Retrieve the model's component's variables

            ObjRef<iface::cellml_api::CellMLVariableSet> componentVariables = component->variables();

            if (componentVariables->length() != 0) {

                QStandardItem *componentItem = new QStandardItem(QString::fromStdWString(component->name()));
                componentItem->setDragEnabled(false);

                // Retrieve the model's component's variables themselves

                ObjRef<iface::cellml_api::CellMLVariableIterator> componentVariablesIter = componentVariables->iterateVariables();
                mVariableTreeModel->invisibleRootItem()->appendRow(componentItem);

                for (ObjRef<iface::cellml_api::CellMLVariable> componentVariable = componentVariablesIter->nextVariable();
                    componentVariable != nullptr; componentVariable = componentVariablesIter->nextVariable()) {

                    QStandardItem *variableItem = new QStandardItem(QString::fromStdWString(componentVariable->name()));
                    variableItem->setAccessibleDescription(QString::fromStdWString(component->name()));

                    componentItem->appendRow(variableItem);
                }
            }
        }
    }
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::saveMapping(const QString &pFileName)
{
    //find a filename

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();
    QString localCellmlFileName = mCellmlFile->fileName();
    bool remoteCellmlFile = fileManagerInstance->isRemote(localCellmlFileName);
    QString cellmlFileName = remoteCellmlFile?
                                 fileManagerInstance->url(localCellmlFileName):
                                 localCellmlFileName;
    //TODO do something more json related ?
    QString mappingFileName = fileName(pFileName, cellmlFileName,
                                     QStringLiteral("json"),
                                     tr("Save CellML Mapping File"),
                                     QStringList());

    QJsonObject jsonContent;
    jsonContent.insert("meshfiles",QJsonValue::fromVariant(mZincMeshFileNames));

    //Jsonise the map
    QJsonArray jsonMapArray;

    for (int node : mMapMatch.keys()) {
        QJsonObject item;
        item.insert("node",QJsonValue::fromVariant(node));
        _variable var = mMapMatch[node];
        item.insert("component",QJsonValue::fromVariant(var.component));
        item.insert("variable",QJsonValue::fromVariant(var.variable));
        jsonMapArray.push_back(item);
    }

    jsonContent.insert("map",jsonMapArray);

    QJsonDocument doc(jsonContent);

    //saving it

    QFile jsonFile(mappingFileName);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(doc.toJson());
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::openMapping(const QString &pFileName)
{

    //open the file

    QFile file;
    file.setFileName(pFileName);

    if (!file.exists()) {
        return;
    }

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val = file.readAll();
    file.close();

    //take element from json objects

    QJsonDocument jsonDocument = QJsonDocument::fromJson(val.toUtf8());

    QJsonObject jsonObject = jsonDocument.object();
    QStringList meshFiles = jsonObject.value("meshfiles").toVariant().toStringList();

    //TODO check if mesh files are good

    //clear the map and the graph

    for (int node : mMapMatch.keys()) {
        mZincWidget->eraseNode(node);
    }
    mMapMatch.clear(); //should already be clear

    //insert the new mapping points in the current mapping system

    int id;
    QString component, variable;
    QMap<QString,QSet<QString>> variables;

    //first collect the variables from the cellml tree o know what exists

    auto treeRoot = mVariableTreeModel->invisibleRootItem();
    int numberComponents = treeRoot->rowCount();
    int numberVariables;

    //TODO improvable by creating the Set on the side and linking it to the map after
    for (int i = 0; i < numberComponents; ++i) {
        QStandardItem *componentItem = treeRoot->child(i);
        component = componentItem->text();

        variables.insert(component, QSet<QString>());

        numberVariables = componentItem->rowCount();
        for (int j = 0; j < numberVariables; ++j) {
            variables.find(component)->insert(componentItem->child(j)->text());
        }
    }


    for (auto mapPointJson : jsonObject.value("map").toArray()) {
        QJsonObject mapPoint = mapPointJson.toObject();

        id = mapPoint.value("node").toInt();
        component = mapPoint.value("component").toString();
        variable = mapPoint.value("variable").toString();

        // test if the point is valid

        if (mZincWidget->hasNode(id)
                && variables.contains(component)
                && variables.find(component)->contains(variable)) {

            mZincWidget->setNodeMapped(id,component,variable);
            setNodeValue(id,component,variable);
        }
    }
}

//==============================================================================

QString CellMLZincMappingViewEditingWidget::fileName(const QString &pFileName,
                                                           const QString &pBaseFileName,
                                                           const QString &pFileExtension,
                                                           const QString &pCaption,
                                                           const QStringList &pFileFilters)
{
    // Return the given file name, if it is not empty, or ask the user to
    // provide one using the additional information that is given

    if (!pFileName.isEmpty()) {
        return pFileName;
    }

    QString fileName = pBaseFileName;
    QString baseFileCompleteSuffix = QFileInfo(pBaseFileName).completeSuffix();

    if (baseFileCompleteSuffix.isEmpty()) {
        fileName += "."+pFileExtension;
    } else {
        fileName.replace(QRegularExpression(QRegularExpression::escape(baseFileCompleteSuffix)+"$"), pFileExtension);
    }

    QString firstFileFilter = pFileFilters.isEmpty()?QString():pFileFilters.first();

    return Core::getSaveFileName(pCaption, fileName, pFileFilters, &firstFileFilter);
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::filterChanged(const QString &text)
{
    int nbComponents = mVariableTreeModel->invisibleRootItem()->rowCount();
    int nbVariables;
    for (int t = 0; t<nbComponents; ++t) {
        QStandardItem *componentItem = mVariableTreeModel->invisibleRootItem()->child(t);
        bool variableSelected = false;
        bool componentSelected = componentItem->text().contains(text);
        nbVariables = componentItem->rowCount();
        for (int i = 0; i<nbVariables; ++i) {
            QStandardItem *variableItem = componentItem->child(i);
            bool display = componentSelected || variableItem->text().contains(text);
            variableSelected = variableSelected || display;
            mVariableTree->setRowHidden(i,componentItem->index(),!display);
        }
        mVariableTree->setRowHidden(t,mVariableTreeModel->invisibleRootItem()->index(),!(componentSelected||variableSelected));
    }
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::emitHorizontalSplitterMoved()
{
    // Let people know that our splitter has been moved

    emit horizontalSplitterMoved(mHorizontalSplitterWidget->sizes());
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::emitVerticalSplitterMoved()
{
    // Let people know that our splitter has been moved

    emit verticalSplitterMoved(mVerticalSplitterWidget->sizes());
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::selectNode(int pId)
{
    if (pId==-1) {
        mNodeValue->setText("");
        mComponentValue->setText("");
        mVariableValue->setText("");
    } else {
        mNodeValue->setNum(pId);
        if (mMapMatch.contains(pId)) {
            mComponentValue->setText(mMapMatch[pId].component);
            mVariableValue->setText(mMapMatch[pId].variable);
        } else {
            mComponentValue->setText("");
            mVariableValue->setText("");
        }
    }
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::saveMappingSlot()
{
    saveMapping({});
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::openMappingFile()
{
    openMapping(Core::getOpenFileName("Open mapping file"));
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::openMeshFile()
{
    setMeshFiles(Core::getOpenFileNames("Open mesh file"));
}

} // namespace MappingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
