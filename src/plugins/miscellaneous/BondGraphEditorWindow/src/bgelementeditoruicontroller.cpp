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
#include <QDebug>
#include <QDockWidget>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPixmapCache>
#include <QResizeEvent>
#include <QStatusBar>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>
#include <QWidgetAction>

#include "bgannotationviewmetadataeditdetailsui.h"
#include "bgconnectionstable.h"
#include "bgimageexport.h"
#include "bgimageexportdialog.h"
#include "bgporteditordialog.h"
#include "bgprojectdetails.h"
#include "bguserdefinedimportdialog.h"
#include "scenemenucontrollerinterface.h"
#include <bgbasicelementannotationtable.h>
#include <bgcolorschemesuicontroller.h>
#include <bgeditorsearchdialog.h>
#include <bgelementconnectionpropertiesui.h>
#include <bgelementeditoruicontroller.h>
#include <bondgraphuiwidget.h>

#include <bgeditorview.h>
#include <bgelementeditorscene.h>
#include <bgelementsceneactions.h>
#include <bgexportutilities.h>

#include <bgconnection.h>
#include <bgeditorsceneconstants.h>
#include <bgelement.h>
#include <scenemenucontroller.h>

#include <fstream>

namespace OpenCOR {
namespace BondGraphEditorWindow {

BGElementEditorUIController::BGElementEditorUIController(
    BondGraphUIWidget *parent, bool doBackup)
    : QObject(nullptr), m_parent(parent) {
  // create document
  m_editorScene = new BGElementEditorScene(parent);
  m_editorView = new BGEditorView(m_editorScene, parent);
  parent->setCentralWidget(m_editorView);
  m_imageDialog = new BGImageExportDialog(parent);
  // connect scene
  connect(m_editorScene, &BGEditorScene::sceneChanged, this,
          &BGElementEditorUIController::onSceneChanged);
  connect(m_editorScene, &BGEditorScene::selectionChanged, this,
          &BGElementEditorUIController::onSelectionChanged);

  connect(m_editorScene, &BGEditorScene::infoStatusChanged, this,
          &BGElementEditorUIController::onSceneStatusChanged);
  connect(m_editorScene, &BGElementEditorScene::editModeChanged, this,
          &BGElementEditorUIController::onEditModeChanged);

  connect(m_editorScene, &BGEditorScene::sceneDoubleClicked, this,
          &BGElementEditorUIController::onSceneDoubleClicked);

  SceneMenuController *menuController = new SceneMenuController(this);
  m_editorScene->setContextMenuController(menuController);

  // connect view
  connect(m_editorView, SIGNAL(scaleChanged(double)), this,
          SLOT(onZoomChanged(double)));

  // slider2d
  createNavigator();

  // menus & actions
  createMenus();

  // dock panels
  createPanels();

  // status bar
  m_statusLabel = new QLabel();
  parent->statusBar()->addPermanentWidget(m_statusLabel);

  // update actions
  onSceneChanged();
  onSelectionChanged();
  onZoomChanged(1);
  onSceneStatusChanged(m_editorScene->getInfoStatus());

  // search dialog
  m_searchDialog = new BGEditorSearchDialog(parent);

  // workaround for full screen
#ifndef Q_OS_WIN32
  if (parent->isMaximized()) {
    parent->showNormal();
    QTimer::singleShot(0, parent, SLOT(showMaximized()));
  }
#endif

  // default scene settings
  readDefaultSceneSettings();
  // Clear project settings
  QSettings bgSettings;

  bgSettings.beginGroup("BondGraphEditorWindow/Bondgraph/ProjectDetails");
  bgSettings.remove("");
  bgSettings.endGroup();

  // Store the clean state
  m_editorScene->addUndoState();
  if (doBackup) {
    QDir temp_dir = QDir::tempPath();
    QString org_name = QCoreApplication::organizationName();
    QString app_name = QCoreApplication::applicationName();

    if (!org_name.isEmpty() && !app_name.isEmpty())
      temp_dir = temp_dir.filePath(org_name + "." + app_name);
    else if (!org_name.isEmpty())
      temp_dir = temp_dir.filePath(org_name);
    else if (!app_name.isEmpty())
      temp_dir = temp_dir.filePath(app_name);
    if (!temp_dir.exists())
      temp_dir.mkpath(".");

    m_backupFile = temp_dir.filePath("bgui.bkp");

    qDebug() << "Backup file located at " << m_backupFile;

    // backup timer
    connect(&m_backupTimer, &QTimer::timeout, this,
            &BGElementEditorUIController::doBackup);
  }
}

BGElementEditorUIController::~BGElementEditorUIController() {}

// IO

void BGElementEditorUIController::reloadBackup() {
  QFile backup(m_backupFile);
  if (backup.exists()) {
    QFileInfo finfo(backup);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        nullptr, tr("Backup exists!"),
        tr("A backup from a previous session (%1) exists!\n Do you wish to "
           "load?")
            .arg(finfo.lastModified().toString("dd-MM-yy hh:mm")),
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      m_editorScene->restoreFromFile(m_backupFile);
      m_editorView->centerContent();
    }
    backup.remove();
  }
}

void BGElementEditorUIController::setAnnotationEditor(
    BGAnnotationViewMetadataEditDetailsUI *editor) {
  m_annotationEditor = editor;
  connect(m_annotationEditor,
          &BGAnnotationViewMetadataEditDetailsUI::annotationUpdated, this,
          &BGElementEditorUIController::onElementAnnotationUpdated);
}

void BGElementEditorUIController::onElementAnnotationUpdated(BGElement *) {
  m_editorScene->addUndoState();
}

void BGElementEditorUIController::elementUpdated(BGElement *) {
  // Update the connections table so that the name is reflected appropriately
  m_connectionsPanel->updateContents();
}

// tbd: move to m_ioController
void BGElementEditorUIController::exportFile() {
  QSettings bgSettings;
  BGProjectDetails details(true, nullptr);
  details.setWindowTitle(QObject::tr("Update project details"));
  if (details.exec() == QDialog::Accepted) {
    nlohmann::json jres = details.getJson();
    std::string dir = jres["directory"];
    std::string fileName =
        dir + "/" + std::string(jres["projectname"]) + ".json";

    // Make sure updates are captured
    m_elementPropertiesPanel->flush();
    nlohmann::json res = *m_editorScene;

    res["Provenance"] = jres;
    std::ofstream files(fileName);
    files << res;
    files.close();
  }
}

void BGElementEditorUIController::exportToCellML() {
  // const QString DEFAULT_DIR_KEY("default_save_dir");
  QSettings bgSettings;

  bgSettings.beginGroup("BondGraphEditorWindow/Bondgraph/ProjectDetails");
  QString projectdir = bgSettings.value("Project_Directory", "").toString();
  QString projectname = bgSettings.value("Project_Name", "").toString();
  bgSettings.endGroup();
  // Make sure updates are captured
  m_elementPropertiesPanel->flush();
  QString filename = to_cellML(projectname, projectdir, *m_editorScene);
  if(filename!="")
    Q_EMIT serializedToCellML(filename);
}

void BGElementEditorUIController::newBondGraph() {
  if (m_editorScene->getNumberOfElements() > 0) {
    QMessageBox::StandardButton reply = QMessageBox::question(
        nullptr, "Not Empty", "Scene is not empty, clear the scene?",
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
      return;
    }
    // Flush to ensure that m_currentElement is set to null, else will cause
    // nullptr exception
    m_elementPropertiesPanel->flush();
    m_editorScene->clear();
  }
  m_editorScene->removeBackUp(m_backupFile);
  QSettings bgSettings;

  bgSettings.beginGroup("BondGraphEditorWindow/Bondgraph/ProjectDetails");
  bgSettings.remove("");
  bgSettings.endGroup();
  // store newly created state
  m_editorScene->addUndoState();
  SceneItem::setItemCount(1);
}

void BGElementEditorUIController::loadFile() {
  const QString DEFAULT_DIR_KEY("default_load_dir");
  QSettings bgSettings;

  QString fileName = QFileDialog::getOpenFileName(
      nullptr, tr("Open Bondgraph"),
      bgSettings.value(DEFAULT_DIR_KEY).toString(), tr("Bondgraph (*.json)"));
  if (fileName.isEmpty())
    return;
  QApplication::setOverrideCursor(Qt::WaitCursor);
  QDir CurrentDir;
  bgSettings.setValue(DEFAULT_DIR_KEY, CurrentDir.absoluteFilePath(fileName));
  m_editorScene->removeBackUp(m_backupFile);
  // store newly created state
  m_editorScene->setInitialState();
  try {
    std::ifstream jfile(fileName.toStdString());
    nlohmann::json res;
    jfile >> res;
    from_json(res, *m_editorScene);
    // Setup
    bgSettings.beginGroup("BondGraphEditorWindow/Bondgraph/ProjectDetails");
    if (res.contains("Provenance")) {
      auto prov = res["Provenance"];
      if (prov.contains("projectname"))
        bgSettings.setValue("Project_Name",
                            QString::fromStdString(prov["projectname"]));
      if (prov.contains("authors"))
        bgSettings.setValue("Project_Authors",
                            QString::fromStdString(prov["authors"]));
      if (prov.contains("description"))
        bgSettings.setValue("Project_Description",
                            QString::fromStdString(prov["description"]));
    } else {
      bgSettings.setValue("Project_Name", "");
      bgSettings.setValue("Project_Authors", "");
      bgSettings.setValue("Project_Description", "");
    }
    bgSettings.setValue("Project_Directory",
                        QFileInfo(fileName).absolutePath());

    bgSettings.endGroup();

    // center scene contents
    m_editorView->centerContent();
  } catch (...) {
    QApplication::restoreOverrideCursor();
    QMessageBox::critical(
        nullptr, tr("Failed"),
        tr("Failed to load file!\n File contents may be corrupted!"));
  }
  QApplication::restoreOverrideCursor();
}

bool BGElementEditorUIController::serialise(
    const FileSerializerInterface &exporter) {

  BGProjectDetails details(true, nullptr);
  details.setWindowTitle(QObject::tr("Update project details"));
  if (details.exec() == QDialog::Accepted) {
    nlohmann::json jres = details.getJson();
    std::string dir = jres["directory"];
    std::string fname = jres["projectname"];
    QString desc = exporter.description();
    QString path;
    if (desc == "Image Format") {
      path = QString::fromStdString(dir + "/" + fname + ".png");
    } else {
      path = QString::fromStdString(dir + "/" + fname + ".json");
    }
    if (exporter.save(path, *m_editorScene)) {
      m_parent->showStatusMessage(tr("Export successful (%1)").arg(path));
      return true;
    } else {
      m_parent->showStatusMessage(tr("Export failed (%1)").arg(path));
      return false;
    }
  }
  return false;
}

void BGElementEditorUIController::exportSVG() {
  m_imageDialog->setScene(*m_editorScene);

  auto &settings = m_parent->getApplicationSettings();
  m_imageDialog->doReadSettings(settings);

  if (m_imageDialog->exec() == QDialog::Rejected)
    return;

  if (!serialise(BGImageExport(m_imageDialog->cutToContent(),
                               m_imageDialog->resolution())))
    return;

  m_imageDialog->doWriteSettings(settings);
}

// UI

void BGElementEditorUIController::createMenus() {
  // file actions
  QAction *newBGAction = new QAction(QIcon(":oxygen/actions/document-new.png"), tr("&New..."));
  newBGAction->setText(tr("&New Bondgraph..."));
  connect(newBGAction, &QAction::triggered, this,
          &BGElementEditorUIController::newBondGraph);

  QAction *loadAction = new QAction(QIcon(":oxygen/actions/document-open.png"), tr("&Load..."));
  loadAction->setText(tr("Load &File..."));
  connect(loadAction, &QAction::triggered, this,
          &BGElementEditorUIController::loadFile);

  QAction *exportAction =
      new QAction(QIcon(":oxygen/actions/document-save.png"), tr("&Export..."));
  exportAction->setText(tr("&Export to File..."));
  connect(exportAction, &QAction::triggered, this,
          &BGElementEditorUIController::exportFile);

  QAction *cellmlAction =
      new QAction(QIcon(":/Icons/Cellml"), tr("&CellML..."));
  cellmlAction->setText(tr("Export to &CellML..."));
  connect(cellmlAction, &QAction::triggered, this,
          &BGElementEditorUIController::exportToCellML);

  QAction *exportActionSVG =
      new QAction(QIcon(":oxygen/mimetypes/image-svg+xml.png"), tr("Export as &Image..."));
  connect(exportActionSVG, &QAction::triggered, this,
          &BGElementEditorUIController::exportSVG);

  // add edit menu
  undoAction = new QAction(QIcon(":oxygen/actions/edit-undo.png"), tr("&Undo"));
  undoAction->setStatusTip(tr("Undo latest action"));
  undoAction->setShortcut(QKeySequence::Undo);
  connect(undoAction, &QAction::triggered, this,
          &BGElementEditorUIController::undo);
  connect(m_editorScene, &BGEditorScene::undoAvailable, undoAction,
          &QAction::setEnabled);
  undoAction->setEnabled(m_editorScene->availableUndoCount());

  redoAction = new QAction(QIcon(":oxygen/actions/edit-redo.png"), tr("&Redo"));
  redoAction->setStatusTip(tr("Redo latest action"));
  redoAction->setShortcut(QKeySequence::Redo);
  connect(redoAction, &QAction::triggered, this,
          &BGElementEditorUIController::redo);
  connect(m_editorScene, &BGEditorScene::redoAvailable, redoAction,
          &QAction::setEnabled);
  redoAction->setEnabled(m_editorScene->availableRedoCount());

  QAction *selAction =
      new QAction(QIcon(":oxygen/actions/edit-select-all.png"), tr("Select &All"));
  selAction->setStatusTip(tr("Select all items on the scene"));
  selAction->setToolTip(tr("Select all items"));
  selAction->setShortcut(QKeySequence::SelectAll);
  connect(selAction, &QAction::triggered, m_editorScene,
          &BGEditorScene::selectAll);

  findAction = new QAction(QIcon(":oxygen/actions/edit-find.png"), tr("&Find..."));
  findAction->setStatusTip(tr("Search for items and attributes"));
  findAction->setToolTip(tr("Search for items"));
  findAction->setShortcut(QKeySequence::Find);
  connect(findAction, &QAction::triggered, this,
          &BGElementEditorUIController::find);

  // edit modes
  m_editModesGroup = new QActionGroup(this);
  m_editModesGroup->setExclusive(true);
  connect(m_editModesGroup, &QActionGroup::triggered, this,
          &BGElementEditorUIController::sceneEditMode);

  modeDefaultAction =
      new QAction(QIcon(":oxygen/actions/edit-select.png"), tr("Select Items"));
  modeDefaultAction->setToolTip(tr("Items selection mode"));
  modeDefaultAction->setStatusTip(tr("Select/deselect items in the document"));
  modeDefaultAction->setCheckable(true);
  modeDefaultAction->setActionGroup(m_editModesGroup);
  modeDefaultAction->setChecked(m_editorScene->getEditMode() == EM_Default);
  modeDefaultAction->setData(EM_Default);

  modeTransformAction =
      new QAction(QIcon(":oxygen/actions/transform-scale.png"), tr("Transform"));
  modeTransformAction->setToolTip(tr("Items transformation mode"));
  modeTransformAction->setStatusTip(
      tr("Transform selected nodes (scale & move)"));
  modeTransformAction->setCheckable(true);
  modeTransformAction->setActionGroup(m_editModesGroup);
  modeTransformAction->setChecked(m_editorScene->getEditMode() == EM_Transform);
  modeTransformAction->setData(EM_Transform);

  // scene actions

  sceneCropAction = new QAction(QIcon(":oxygen/actions/transform-crop.png"), tr("&Crop Area"));
  sceneCropAction->setStatusTip(tr("Crop document area to contents"));
  connect(sceneCropAction, &QAction::triggered, m_editorScene,
          &BGEditorScene::crop);

  // color schemes

  m_schemesController = new BGColorSchemesUIController(this);
  m_schemesController->setScene(m_editorScene);
  schemesAction = m_schemesController->getSchemesMenu()->menuAction();
  schemesAction->setText(tr("Apply Colors"));
  schemesAction->setStatusTip(
      tr("Apply predefined color scheme to the project"));

  // scene options

  sceneAction = new QAction(QIcon(":oxygen/actions/configure.png"), tr("&Options..."));
  sceneAction->setStatusTip(tr("Change project properties"));
  connect(sceneAction, &QAction::triggered, this,
          &BGElementEditorUIController::sceneOptions);

  // add view menu

  gridAction = new QAction(QIcon(":/Icons/Grid-Show"), tr("Show &Grid"));
  gridAction->setCheckable(true);
  gridAction->setStatusTip(tr("Show/hide background grid"));
  gridAction->setChecked(m_editorScene->gridEnabled());
  connect(gridAction, SIGNAL(toggled(bool)), m_editorScene,
          SLOT(enableGrid(bool)));

  gridSnapAction = new QAction(QIcon(":/Icons/Grid-Snap"), tr("&Snap to Grid"));
  gridSnapAction->setCheckable(true);
  gridSnapAction->setStatusTip(tr("Snap to grid when dragging"));
  gridSnapAction->setChecked(m_editorScene->gridSnapEnabled());
  connect(gridSnapAction, SIGNAL(toggled(bool)), m_editorScene,
          SLOT(enableGridSnap(bool)));

  m_actionShowNodeIds = new QAction(tr("Show Node Ids"));
  m_actionShowNodeIds->setCheckable(true);
  m_actionShowNodeIds->setStatusTip(tr("Show/hide element ids"));
  m_actionShowNodeIds->setChecked(
      m_editorScene->isClassAttributeVisible(class_element, attr_id));
  connect(m_actionShowNodeIds, SIGNAL(toggled(bool)), this,
          SLOT(showElementIds(bool)));

  m_actionShowEdgeIds = new QAction(tr("Show Edge Ids"));
  m_actionShowEdgeIds->setCheckable(true);
  m_actionShowEdgeIds->setStatusTip(tr("Show/hide edge ids"));
  m_actionShowEdgeIds->setChecked(
      m_editorScene->isClassAttributeVisible(class_connection, attr_id));
  connect(m_actionShowEdgeIds, SIGNAL(toggled(bool)), this,
          SLOT(showConnectionIds(bool)));

  zoomAction = new QAction(QIcon(":oxygen/actions/zoom-in.png"), tr("&Zoom"));
  zoomAction->setStatusTip(tr("Zoom view in"));
  zoomAction->setShortcut(QKeySequence::ZoomIn);
  connect(zoomAction, &QAction::triggered, this,
          &BGElementEditorUIController::zoom);

  unzoomAction = new QAction(QIcon(":oxygen/actions/zoom-out.png"), tr("&Unzoom"));
  unzoomAction->setStatusTip(tr("Zoom view out"));
  unzoomAction->setShortcut(QKeySequence::ZoomOut);
  connect(unzoomAction, &QAction::triggered, this,
          &BGElementEditorUIController::unzoom);

  resetZoomAction = new QAction(QIcon(":oxygen/actions/zoom-original.png"), tr("&Reset Zoom"));
  resetZoomAction->setStatusTip(tr("Zoom view to 100%"));
  connect(resetZoomAction, &QAction::triggered, this,
          &BGElementEditorUIController::resetZoom);

  fitZoomAction = new QAction(QIcon(":oxygen/actions/zoom-fit-best.png"), tr("&Fit to View"));
  fitZoomAction->setStatusTip(tr("Zoom to fit all the items to view"));
  connect(fitZoomAction, &QAction::triggered, m_editorView,
          &BGEditorView::fitToView);

  fitZoomSelectedAction =
      new QAction(QIcon(":oxygen/actions/zoom-select.png"), tr("Fit &Selection"));
  fitZoomSelectedAction->setToolTip(tr("Fit selected items to view"));
  fitZoomSelectedAction->setStatusTip(tr("Zoom to fit selected items to view"));
  connect(fitZoomSelectedAction, &QAction::triggered, m_editorView,
          &BGEditorView::fitSelectedToView);

  fitZoomBackAction =
      new QAction(QIcon(":oxygen/actions/zoom-previous.png"), tr("Zoom &Back"));
  fitZoomBackAction->setStatusTip(tr("Zoom to previous state before last fit"));
  connect(fitZoomBackAction, &QAction::triggered, m_editorView,
          &BGEditorView::zoomBack);

  // add zoom toolbar
  // add edit toolbar
  QToolBar *toolBar = m_parent->getToolBar();
  // toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  toolBar->addAction(newBGAction);
  toolBar->addAction(loadAction);
  toolBar->addSeparator();
  toolBar->addAction(exportAction);
  toolBar->addAction(exportActionSVG);
  toolBar->addSeparator();
  toolBar->addAction(cellmlAction);

  toolBar->addSeparator();
  // Handled by mouse action
  toolBar->addAction(findAction);

  // add edit modes toolbar
  toolBar->addSeparator();
  toolBar->addAction(modeDefaultAction);
  toolBar->addAction(modeTransformAction);

  toolBar->addSeparator();
  toolBar->addAction(sceneAction);
}

void BGElementEditorUIController::createPanels() {

  m_propertiesPanel = new BGElementConnectionPropertiesUI(m_parent);
  m_propertiesPanel->setScene(m_editorScene);
  m_parent->addControlPane(tr("Visual Properties"), m_propertiesPanel);

  m_elementPropertiesPanel =
      new BGBasicElementAnnotationTable(m_parent, m_editorScene);
  connect(m_elementPropertiesPanel,
          &BGBasicElementAnnotationTable::elementUpdated, this,
          &BGElementEditorUIController::elementUpdated);
  m_parent->addControlPane(tr("Item Properties"), m_elementPropertiesPanel);

  m_connectionsPanel = new BGConnectionsTable(m_parent);
  m_connectionsPanel->setScene(m_editorScene);
  m_parent->addControlPane(tr("Connections"), m_connectionsPanel);
}

void BGElementEditorUIController::createNavigator() {
  m_sliderView = new QSint::Slider2d(m_parent);
  m_sliderView->connectSource(m_editorView);

  QToolButton *sliderButton = m_sliderView->makeAsButton();
  m_editorView->setCornerWidget(sliderButton);

  sliderButton->setIcon(QIcon(":/Icons/Navigator"));
  sliderButton->setToolTip(tr("Show scene navigator"));
  connect(m_sliderView, SIGNAL(aboutToShow()), this, SLOT(onNavigatorShown()));

  m_sliderView->setFixedSize(200, 200);
  m_sliderView->setSliderOpacity(0.3);
  m_sliderView->setSliderBrush(Qt::blue);
}

void BGElementEditorUIController::onNavigatorShown() {
  double w = m_editorScene->sceneRect().width();
  double h = m_editorScene->sceneRect().height();
  double cw = w > h ? 200.0 : 200.0 * (w / h);
  double ch = h > w ? 200.0 : 200.0 * (h / w);
  m_sliderView->setFixedSize(cw, ch);

  // Qt bug: update menu size
  QResizeEvent re(m_sliderView->size(), m_sliderView->parentWidget()->size());
  qApp->sendEvent(m_sliderView->parentWidget(), &re);

  QPixmap pm(m_sliderView->size());
  QPainter p(&pm);

  BGEditorScene *tempScene = m_editorScene->clone();
  tempScene->enableGrid(false);
  tempScene->enableItemLabels(false);
  tempScene->render(&p);
  delete tempScene;

  m_sliderView->setBackgroundBrush(pm);
}

void BGElementEditorUIController::onSelectionChanged() {
  auto selectedItems = m_editorScene->selectedItems();
  int selectionCount = selectedItems.size();

  fitZoomSelectedAction->setEnabled(selectionCount > 0);

  if (selectionCount == 1 && m_annotationEditor != nullptr) {
    for (auto item : selectedItems) {
      if (BGElement *bg = dynamic_cast<BGElement *>(item)) {
        if (bg->getClassType() != "userdefined") {
          m_annotationEditor->setBGElement(bg);
          if (bg->getClassType() == "annotation") {
            m_editorScene->setEditMode(EM_Transform);
          }
        } else {
          // If data is already set
          auto &uds = bg->getJson();
          if (uds.contains("mapdefinition")) {
            m_annotationEditor->setBGElement(bg);
          } else {
            BGUserdefinedImportDialog *dialog = new BGUserdefinedImportDialog();
            if (dialog->exec(*m_editorScene)) {
              auto udef = dialog->getDefinition();
              // Define element properties based on user selection
              uds["mapdefinition"] = udef;
              auto port = bg->getPort("temp");
              QString name = "Port " + QString::fromStdString(udef["variable"]);
              bg->renamePort("temp", name.toUtf8());
              port->show();
              m_editorScene->addUndoState();
              m_annotationEditor->setBGElement(bg);
              m_elementPropertiesPanel->onSelectionChanged();
            }
            delete dialog;
          }
        }
        return;
      }
    }
  }
  if (m_annotationEditor != nullptr) {
    m_annotationEditor->setNoSelection();
    m_editorScene->setEditMode(EM_Default);
  }
}

void BGElementEditorUIController::onSceneChanged() {
  auto nodes = m_editorScene->getItems<BGElement>();
  auto edges = m_editorScene->getItems<BGConnection>();

  m_statusLabel->setText(
      tr("Elements: %1 | Connections: %2").arg(nodes.size()).arg(edges.size()));

  updateActions();
}

// scene

void BGElementEditorUIController::onSceneHint(const QString &text) {
  m_parent->showStatusMessage(text);
}

void BGElementEditorUIController::onSceneStatusChanged(int status) {
  bool isAddNodesMode = (m_editorScene->getEditMode() == EM_AddElements);

  const QString arrowMoveHint =
      tr(" | Cursor keys - move selected items by one point | Shift + Cursor "
         "keys - move selected items by grid step");

  switch (status) {
  case SIS_Edit_Label:
    onSceneHint(tr("Enter - finish edit | Esc - cancel edit | Shift + Enter - "
                   "insert line break"));
    return;

  case SIS_Hover:
    if (isAddNodesMode)
      onSceneHint(tr("Click & drag - create new connection | Double click - "
                     "edit item's label") +
                  arrowMoveHint);
    else
      onSceneHint(
          tr("Ctrl + Click - (un)select item | Click & drag or Ctrl/Shift + "
             "Cursor keys - move selected items | Ctrl + Click & drag - clone "
             "selected items | Ctrl + Alt + Arrow keys - reorient port "
             "locations "));
    return;

  case SIS_Drag:
    onSceneHint(
        tr("Shift - horizontal or vertical snap | Alt - toggle grid snap"));
    return;

  case SIS_Hover_Port:
    onSceneHint(tr("Click & drag - make a connection at this port | Double "
                   "click - show port properties"));
    return;
  case SIS_Move_Port:
    onSceneHint(tr("Click & drag - move this port within the element"));
    return;

  default:
    if (isAddNodesMode)
      onSceneHint(tr("Click - create new element | Click & drag - create new "
                     "connection") +
                  arrowMoveHint);
    else
      onSceneHint(tr("Click & drag - select an area") + arrowMoveHint);
  }
}

void BGElementEditorUIController::onSceneDoubleClicked(
    QGraphicsSceneMouseEvent * /*mouseEvent*/, QGraphicsItem *clickedItem) {
  BGPort *port = dynamic_cast<BGPort *>(clickedItem);
  if (port)
    editNodePort(*port);
}

void BGElementEditorUIController::sceneEditMode(QAction *act) {
  int mode = act->data().toInt();
  m_editorScene->setEditMode((EditMode)mode);
}

void BGElementEditorUIController::onEditModeChanged(int mode) {
  modeDefaultAction->setChecked(mode == EM_Default);
  modeTransformAction->setChecked(mode == EM_Transform);
}

// documents

void BGElementEditorUIController::removeBackUp() {
  m_editorScene->removeBackUp(m_backupFile);
}

void BGElementEditorUIController::doBackup() {
  if (m_parent->isVisible()) {
    m_parent->showStatusMessage(tr("Running backup..."));
    m_editorScene->backupScene(m_backupFile);
    m_parent->showStatusMessage(tr("Backup completed"));
    qApp->processEvents();
  }
}

void BGElementEditorUIController::onNewDocumentCreated() {
  readDefaultSceneSettings();
  // store newly created state
  m_editorScene->addUndoState();
}

void BGElementEditorUIController::onDocumentLoaded(const QString &fileName) {
  QSettings &settings = m_parent->getApplicationSettings();

  // read custom topology of the current document
  settings.beginGroup("BondGraphEditorWindow/CustomFiles");

  QString filename = QFileInfo(fileName).fileName();
  if (!filename.isEmpty() && settings.childGroups().contains(filename)) {
    settings.beginGroup(filename);

    settings.beginGroup("BondGraphEditorWindow/UI/Topology");
    m_connectionsPanel->doReadSettings(settings);
    settings.endGroup();

    settings.endGroup();
  }

  settings.endGroup();

  // workaround: always make the labels visible
  m_editorScene->setClassAttributeVisible(class_item, attr_label, true);
  m_editorScene->setClassAttributeVisible(class_element, attr_label, true);
  m_editorScene->setClassAttributeVisible(class_connection, attr_label, true);

  // store newly created state
  m_editorScene->setInitialState();

  // center scene contents
  m_editorView->centerContent();
}

// settings

QSettings &BGElementEditorUIController::getApplicationSettings() const {
  return m_parent->getApplicationSettings();
}

void BGElementEditorUIController::doReadSettings(QSettings &settings) {
  // options
  int cacheRam = QPixmapCache::cacheLimit();
  cacheRam = settings.value("BondGraphEditorWindow/cacheRam", cacheRam).toInt();
  QPixmapCache::setCacheLimit(cacheRam);

  bool isAA = m_editorView->renderHints().testFlag(QPainter::Antialiasing);
  isAA = settings.value("BondGraphEditorWindow/antialiasing", isAA).toBool();
  m_editorView->setRenderHint(QPainter::Antialiasing, isAA);
  m_editorScene->setFontAntialiased(isAA);

  m_optionsData.backupPeriod =
      settings.value("BondGraphEditorWindow/backupPeriod", m_optionsData.backupPeriod)
          .toInt();

  settings.beginGroup("BondGraphEditorWindow/GraphViz");
  m_optionsData.graphvizPath =
      settings.value("BondGraphEditorWindow/path", m_optionsData.graphvizPath).toString();
  m_optionsData.graphvizDefaultEngine =
      settings
          .value("BondGraphEditorWindow/defaultEngine",
                 m_optionsData.graphvizDefaultEngine)
          .toString();
  settings.endGroup();

  updateSceneOptions();

  // IO
  settings.beginGroup("BondGraphEditorWindow/IO");
  // m_ioController->doReadSettings(settings);
  settings.endGroup();

  // UI elements
  settings.beginGroup("BondGraphEditorWindow/UI/ItemProperties");
  m_propertiesPanel->doReadSettings(settings);
  settings.endGroup();
}

void BGElementEditorUIController::doWriteSettings(QSettings &settings) {
  // temp
  writeDefaultSceneSettings();

  bool isAA = m_editorView->renderHints().testFlag(QPainter::Antialiasing);
  settings.setValue("antialiasing", isAA);

  int cacheRam = QPixmapCache::cacheLimit();
  settings.setValue("cacheRam", cacheRam);

  settings.setValue("backupPeriod", m_optionsData.backupPeriod);

  // Graphviz
  settings.beginGroup("BondGraphEditorWindow/GraphViz");
  settings.setValue("path", m_optionsData.graphvizPath);
  settings.setValue("defaultEngine", m_optionsData.graphvizDefaultEngine);
  settings.endGroup();

  // IO
  settings.beginGroup("BondGraphEditorWindow/IO");
  // m_ioController->doWriteSettings(settings);
  settings.endGroup();

  // UI elements
  settings.beginGroup("BondGraphEditorWindow/UI/ItemProperties");
  m_propertiesPanel->doWriteSettings(settings);
  settings.endGroup();

  // custom topology of the current document
  settings.beginGroup("BondGraphEditorWindow/CustomFiles");

  QString filename = QFileInfo(m_parent->getCurrentFileName()).fileName();
  if (!filename.isEmpty()) {
    settings.beginGroup(filename);

    settings.beginGroup("BondGraphEditorWindow/UI/Topology");
    m_connectionsPanel->doWriteSettings(settings);
    settings.endGroup();

    settings.endGroup();
  }

  settings.endGroup();
}

void BGElementEditorUIController::readDefaultSceneSettings() {
  QSettings &settings = m_parent->getApplicationSettings();

  settings.beginGroup("BondGraphEditorWindow/Scene/Defaults");

  bool showElementIds =
      settings.value("BondGraphEditorWindow/showElementIds", true).toBool();
  bool showConnectionIds =
      settings.value("BondGraphEditorWindow/showConnectionIds", true).toBool();

  QColor bgColor = settings
                       .value("BondGraphEditorWindow/background",
                              m_editorScene->backgroundBrush().color())
                       .value<QColor>();
  QPen gridPen =
      settings.value("BondGraphEditorWindow/grid.color", m_editorScene->getGridPen())
          .value<QPen>();
  int gridSize =
      settings.value("BondGraphEditorWindow/grid.size", m_editorScene->getGridSize())
          .toInt();
  bool gridEnabled =
      settings.value("BondGraphEditorWindow/grid.enabled", m_editorScene->gridEnabled())
          .toBool();
  bool gridSnap =
      settings.value("BondGraphEditorWindow/grid.snap", m_editorScene->gridSnapEnabled())
          .toBool();

  settings.endGroup();

  // workaround: always make the labels visible
  m_editorScene->setClassAttributeVisible(class_item, attr_label, true);
  m_editorScene->setClassAttributeVisible(class_element, attr_label, true);
  m_editorScene->setClassAttributeVisible(class_connection, attr_label, true);

  m_editorScene->setClassAttributeVisible(class_element, attr_id,
                                          showElementIds);
  m_editorScene->setClassAttributeVisible(class_connection, attr_id,
                                          showConnectionIds);
  m_editorScene->setBackgroundBrush(bgColor);
  m_editorScene->setGridPen(gridPen);
  m_editorScene->setGridSize(gridSize);
  m_editorScene->enableGrid(gridEnabled);
  m_editorScene->enableGridSnap(gridSnap);

  updateFromActions();
}

void BGElementEditorUIController::writeDefaultSceneSettings() {
  QSettings &settings = m_parent->getApplicationSettings();

  settings.beginGroup("BondGraphEditorWindow/Scene/Defaults");

  bool showElementIds =
      m_editorScene->isClassAttributeVisible(class_element, attr_id);
  bool showConnectionIds =
      m_editorScene->isClassAttributeVisible(class_connection, attr_id);

  settings.setValue("showElementIds", showElementIds);
  settings.setValue("showConnectionIds", showConnectionIds);

  settings.setValue("background", m_editorScene->backgroundBrush().color());
  settings.setValue("grid.color", m_editorScene->getGridPen());
  settings.setValue("grid.size", m_editorScene->getGridSize());
  settings.setValue("grid.enabled", m_editorScene->gridEnabled());
  settings.setValue("grid.snap", m_editorScene->gridSnapEnabled());

  settings.endGroup();

  settings.sync();
}

void BGElementEditorUIController::sceneOptions() {

  BGProjectDetails *details = new BGProjectDetails(true, nullptr);
  details->setWindowTitle(QObject::tr("Provide details"));
  details->exec();
  delete details;
}

void BGElementEditorUIController::updateSceneOptions() {
#ifdef USE_GVGRAPH
  if (m_gvController) {
    m_gvController->setPathToGraphviz(m_optionsData.graphvizPath);
    m_gvController->setDefaultEngine(m_optionsData.graphvizDefaultEngine);
  }
#endif
  // qDebug() << " options back up "<<m_optionsData.backupPeriod;
  if (m_optionsData.backupPeriod > 0) {
    m_backupTimer.setInterval(m_optionsData.backupPeriod *
                              6000); // 1 minute - m_optionsData.backupPeriod =
                                     // 10, 1000 milli seconds = 1 minute
    // m_backupTimer.setInterval(m_optionsData.backupPeriod * 2000);
    m_backupTimer.start();
  } else
    m_backupTimer.stop();

  updateActions();
}

void BGElementEditorUIController::updateActions() {
  if (m_editorScene) {
    gridAction->setChecked(m_editorScene->gridEnabled());
    gridSnapAction->setChecked(m_editorScene->gridSnapEnabled());

    m_actionShowNodeIds->setChecked(
        m_editorScene->isClassAttributeVisible(class_element, attr_id));
    m_actionShowEdgeIds->setChecked(
        m_editorScene->isClassAttributeVisible(class_connection, attr_id));
  }
}

void BGElementEditorUIController::updateFromActions() {
  if (m_editorScene) {
    m_editorScene->setClassAttributeVisible(class_element, attr_id,
                                            m_actionShowNodeIds->isChecked());
    m_editorScene->setClassAttributeVisible(class_connection, attr_id,
                                            m_actionShowEdgeIds->isChecked());
  }
}

void BGElementEditorUIController::onLayoutFinished() {
  m_editorScene->crop();

  // make an option??
  // m_editorView->fitToView();
}

// zooming

void BGElementEditorUIController::onZoomChanged(double currentZoom) {
  Q_UNUSED(currentZoom);
  // resetZoomAction2->setText(QString("%1%").arg((int)(currentZoom * 100)));

  fitZoomBackAction->setEnabled(m_editorView->getZoomBeforeFit() > 0);
}

void BGElementEditorUIController::zoom() { m_editorView->zoomBy(1.3); }

void BGElementEditorUIController::unzoom() { m_editorView->zoomBy(1.0 / 1.3); }

void BGElementEditorUIController::resetZoom() { m_editorView->zoomTo(1.0); }

// other actions

void BGElementEditorUIController::editNodePort() {
  BGPort *port = dynamic_cast<BGPort *>(m_editorScene->getContextMenuTrigger());
  if (port)
    editNodePort(*port);
}

void BGElementEditorUIController::editNodePort(BGPort &port) {
  BGPortEditorDialog dialog(m_editorScene);
  if (dialog.exec(port) == QDialog::Accepted)
    m_editorScene->addUndoState();
  else
    m_editorScene->revertUndoState();
}

void BGElementEditorUIController::find() {
  m_searchDialog->exec(*m_editorScene);
}

void BGElementEditorUIController::showElementIds(bool on) {
  m_editorScene->setClassAttributeVisible(class_element, attr_id, on);

  m_editorScene->addUndoState();
}

void BGElementEditorUIController::showConnectionIds(bool on) {
  m_editorScene->setClassAttributeVisible(class_connection, attr_id, on);

  m_editorScene->addUndoState();
}

void BGElementEditorUIController::undo() {
  m_editorScene->undo();
  m_annotationEditor->reload();
  updateFromActions();
}

void BGElementEditorUIController::redo() {
  m_editorScene->redo();
  m_annotationEditor->reload();
  updateFromActions();
}

void BGElementEditorUIController::changeItemId() {
  auto sceneActions =
      dynamic_cast<BGElementSceneActions *>(m_editorScene->getActions());
  int nodesCount = m_editorScene->getSelectedElements().size();
  int edgesCount = m_editorScene->getSelectedConnections().size();

  if (nodesCount == 1 && edgesCount == 0 && sceneActions) {
    sceneActions->editElementId(m_editorScene->getSelectedElements().first());
    return;
  }

  if (nodesCount == 0 && edgesCount == 1 && sceneActions) {
    sceneActions->editConnectionId(
        m_editorScene->getSelectedConnections().first());
    return;
  }
}

void BGElementEditorUIController::copy() { m_editorScene->copy(); }
void BGElementEditorUIController::cut() { m_editorScene->cut(); }
void BGElementEditorUIController::paste() { m_editorScene->paste(); }
void BGElementEditorUIController::del() { m_editorScene->del(); }
void BGElementEditorUIController::selectAll() { m_editorScene->selectAll(); }

} // namespace BondGraphEditorWindow
} // namespace OpenCOR