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
#pragma once

#include <QAction>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QLabel>
#include <QSettings>
#include <QTimer>

#include "thirdparty/qsintwidgets/slider2d.h"

#include "scenefileserializerinterface.h"
#include "sceneoptionsdialog.h"

namespace OpenCOR {
namespace BondGraphUI {

class BondGraphUIWidget;
class BGElementEditorScene;
class BGPort;
class BGEditorView;
class BGAnnotationViewMetadataEditDetailsUI;

class BGElementEditorUIController : public QObject {
  Q_OBJECT

public:
  BGElementEditorUIController(BondGraphUIWidget *parent, bool doBackup = true);
  virtual ~BGElementEditorUIController();

  QSettings &getApplicationSettings() const;
  void doReadSettings(QSettings &settings);
  void doWriteSettings(QSettings &settings);

  void setAnnotationEditor(BGAnnotationViewMetadataEditDetailsUI *editor);

  // callbacks
  void onNewDocumentCreated();
  void onDocumentLoaded(const QString &fileName);
  void removeBackUp();
  void reloadBackup();

Q_SIGNALS:
  void serializedToCellML(QString path);

public Q_SLOTS:
  void undo();
  void redo();
  void newBondGraph();
  void loadFile();
  void exportFile();
  void exportSVG();
  void exportToCellML();
  void copy();
  void cut();
  void paste();
  void selectAll();
  void del();
  // protected API
protected:
  BGElementEditorScene *scene() { return m_editorScene; }
  bool serialise(const FileSerializerInterface &exporter);

private Q_SLOTS:
  // tbd: move to export controller

  void doBackup();

  void onNavigatorShown();

  void onSelectionChanged();
  void onSceneChanged();
  void onSceneHint(const QString &text);
  void onSceneStatusChanged(int status);
  void onSceneDoubleClicked(QGraphicsSceneMouseEvent *mouseEvent,
                            QGraphicsItem *clickedItem);

  void sceneEditMode(QAction *);
  void onEditModeChanged(int mode);

  void onZoomChanged(double currentZoom);
  void zoom();
  void unzoom();
  void resetZoom();

  void sceneOptions();

  void showElementIds(bool on);
  void showConnectionIds(bool on);

  void changeItemId();
  // void addElementPort();
  void editNodePort();
  void find();
  void onLayoutFinished();

  void onElementAnnotationUpdated(BGElement *elem);
  void elementUpdated(BGElement *elem);

private:
  void createMenus();
  void createPanels();
  void createNavigator();

  void readDefaultSceneSettings();
  void writeDefaultSceneSettings();

  void updateSceneOptions();

  void updateActions();
  void updateFromActions();

  void editNodePort(BGPort &port);

private:
  BondGraphUIWidget *m_parent = nullptr;
  BGElementEditorScene *m_editorScene = nullptr;
  BGEditorView *m_editorView = nullptr;

  QSint::Slider2d *m_sliderView = nullptr;
  QLabel *m_statusLabel = nullptr;
  QMenu *m_viewMenu = nullptr;
  QActionGroup *m_editModesGroup;
  QAction *findAction = nullptr;
  QAction *modeDefaultAction;
  QAction *modeNodesAction;
  QAction *modeTransformAction;
  QAction *modeFactorAction;
  QAction *zoomAction;
  QAction *unzoomAction;
  QAction *resetZoomAction;
  // QAction *resetZoomAction2;
  QAction *fitZoomAction;
  QAction *fitZoomSelectedAction;
  QAction *fitZoomBackAction;
  QAction *sceneCropAction;
  QAction *schemesAction;
  QAction *gridAction;
  QAction *gridSnapAction;
  QAction *sceneAction;
  QAction *actionShowLabels;
  QAction *m_actionShowNodeIds;
  QAction *m_actionShowEdgeIds;
  QAction *undoAction;
  QAction *redoAction;
  friend class SceneMenuController; // Allow scenemenucontroller to access
                                    // actions
  OptionsData m_optionsData;

  QTimer m_backupTimer;
  QString m_backupFile;
  class BGColorSchemesUIController *m_schemesController = nullptr;
  class BGElementConnectionPropertiesUI *m_propertiesPanel = nullptr;
  class BGBasicElementAnnotationTable *m_elementPropertiesPanel = nullptr;
  class BGConnectionsTable *m_connectionsPanel = nullptr;
  class BGElementAttributesEditorUI *m_defaultsPanel = nullptr;
  class BGEditorSearchDialog *m_searchDialog = nullptr;
  class BGImageExportDialog *m_imageDialog = nullptr;
  BGAnnotationViewMetadataEditDetailsUI *m_annotationEditor = nullptr;
};

} // namespace BondGraphUI
} // namespace OpenCOR