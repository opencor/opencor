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
#include "bondgraphuiwidget.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QScreen>
#include <QSettings>
#include <QSplitter>
#include <QStatusBar>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

#include "bgannotationviewmetadataeditdetailsui.h"
#include "bgelementeditoruicontroller.h"

#include "thirdparty/widgetbox/libraryitem.h"
#include "thirdparty/qtaccordian/qaccordion.h"
#include "thirdparty/widgetbox/widgetbox.h"

#include "thirdparty/json.hpp"
#include <bondgraph.hpp>

namespace OpenCOR {
namespace BondGraphEditorWindow {

BondGraphUIWidget::BondGraphUIWidget(QWidget *parent, bool doBackup)
    : QWidget(parent), m_doBackup(doBackup) {
  setupUi();
}

BondGraphUIWidget::~BondGraphUIWidget() {}

QSettings &BondGraphUIWidget::getApplicationSettings() const {
  // static QSettings settings(QCoreApplication::organizationName(),
  // QCoreApplication::applicationName()); return settings;

  static QSettings *settings =
      new QSettings(QCoreApplication::organizationName(),
                    QCoreApplication::applicationName());
  return *settings;
}

QToolBar *BondGraphUIWidget::getToolBar() const { return m_toolBar; }

void BondGraphUIWidget::saveSettings(QSettings &settings) const {

  settings.setValue("geometry", saveGeometry());
  settings.setValue("maximized", isMaximized());

  settings.setValue("lastPath", m_lastPath);
}

void BondGraphUIWidget::readSettings(QSettings &settings) {
  settings.sync();
  showNormal();

  // window geometry
  const QByteArray geometry =
      settings.value("BondGraphEditorWindow/geometry", QByteArray()).toByteArray();
  if (geometry.isEmpty()) {
    const QRect availableGeometry =
        QApplication::desktop()->availableGeometry(this);
    resize(availableGeometry.width() - 200, availableGeometry.height() - 100);
    move(100, 50);
  } else {
    restoreGeometry(geometry);
  }

  QApplication::processEvents();

  // window state
  if (settings.value("BondGraphEditorWindow/maximized", true).toBool()) {
    //#ifdef Q_OS_WIN32
    showMaximized();
    //#else
    //		showNormal();
    //        QTimer::singleShot(0, this, SLOT(showMaximized()));
    //#endif
  } else
    showNormal();

  // path
  m_lastPath = settings.value("BondGraphEditorWindow/lastPath").toString();
}

QFrame *createSampleFrame() {
  auto *frame = new QFrame();
  frame->setLayout(new QVBoxLayout());
  auto *ipsumLabel = new QLabel();
  frame->layout()->addWidget(ipsumLabel);
  dynamic_cast<QVBoxLayout *>(frame->layout())->addStretch();
  ipsumLabel->setTextFormat(Qt::TextFormat::RichText);
  ipsumLabel->setWordWrap(true);
  ipsumLabel->setText("This is a sample frame with populated contents");
  return frame;
}

void BondGraphUIWidget::showStatusMessage(const QString &msg, int timeout) {
  m_statusBar->showMessage(msg, timeout);
}

void BondGraphUIWidget::setCentralWidget(QWidget *mainWidget) {
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->addWidget(mainWidget);
  m_canvasParent->setLayout(layout);
}

void BondGraphUIWidget::setupUi() {
  QVBoxLayout *mainLayout = new QVBoxLayout;
  // Create tool bar
  m_toolBar = new QToolBar(this);
  mainLayout->addWidget(m_toolBar, 0);

  QSplitter *wSplitter = new QSplitter(this);
  m_widgetBox = new WidgetBox(this);
  // Get the elements supported by the bondgraph library
  nlohmann::json support = BG::getSupportedPhysicalDomainsAndFactoryMethods();
  // Set up icons
  nlohmann::json iconMaps;
  nlohmann::json iconfiles;
  iconfiles["Electrical"] = nlohmann::json::parse(
      R"(
        {
          "Resistance": ":/Icons/resistor",
          "Capacitance": ":/Icons/capacitor",
          "Inductance": ":/Icons/inductor",
          "PotentialSource": ":/Icons/effortsource",
          "FlowSource": ":/Icons/flowsource"
        }
        )");
  iconfiles["Generic"] = nlohmann::json::parse(
      R"(
      {
        "Resistance": ":/Icons/resistor",
        "Capacitance": ":/Icons/capacitor",
        "Inductance": ":/Icons/inductor",
        "PotentialSource": ":/Icons/effortsource",
        "FlowSource": ":/Icons/flowsource",
        "Gyrator": ":/Icons/gyrator",
        "Transformer": ":/Icons/transformer",
        "ZeroJunction": ":/Icons/zerojunction",
        "OneJunction": ":/Icons/onejunction"        
      }
      )");

  iconMaps["Icons"] = iconfiles;
  iconMaps["ElementDefinitions"] = nlohmann::json::parse(
      R"(
        {
      "Composition":{
        "location":3,
        "UserDefined":{
          "name":"userdefined",
          "description": "User Defined blackbox or BG",
          "shortname": "ud",
          "variableprefix": "UD",
          "class": "userdefined",
          "domain": "Composition",        
          "ports": {
            "limit": 1
          }
        }
      },
      "Annotation":{
        "location":4,
        "Region":{
          "name":"region",
          "description": "Spatial Annotation",
          "shortname": "Text",
          "variableprefix": "",
          "class": "annotation",
          "domain": "Annotation",        
          "ports": {
            "limit": 0
          }
        }
      }      
    }
    )");

  nlohmann::json &edf = support["ElementDefinitions"];
  if (iconMaps.contains("ElementDefinitions")) {
    nlohmann::json &jdf = iconMaps["ElementDefinitions"];
    for (auto &doj : jdf.items()) {
      // Update
      if (edf.contains(doj.key())) {
        doj.value().update(edf[doj.key()]);
      }
    }
    // Addin elements in support that are not in the iconMap
    for (auto &doj : edf.items()) {
      // Update
      if (!jdf.contains(doj.key())) {
        jdf[doj.key()] = doj.value();
        // Check if icon is available
      }
    }
  } else {
    // Update
    iconMaps["ElementDefinitions"] = edf;
  }
  // Add domain information to each element definition
  nlohmann::json &df = iconMaps["ElementDefinitions"];
  for (auto &ed : df.items()) {
    nlohmann::json &jdf = ed.value();
    std::string key = ed.key();
    for (auto &elem : jdf.items()) {
      nlohmann::json &j_ = elem.value();
      std::string k_ = elem.key();
      if (j_.is_object() && k_ != "BondDimensions") {
        j_["domain"] = key;
        j_["type"] = k_;
      }
    }
  }
  QString js = QString::fromStdString(iconMaps.dump());
  // qDebug()<<"\n\n"<<js<<"\n\n";
  m_widgetBox->load(js);

  wSplitter->addWidget(m_widgetBox);
  m_canvasParent = new QWidget(this);
  m_canvasParent->setAcceptDrops(true);
  QSplitter *cSplitter = new QSplitter(this);

  cSplitter->addWidget(m_canvasParent);
  m_controls = new QAccordion(this);
  m_controls->setMultiActive(
      true); // Enable multiple panes to be open simultaneously

  auto annotation = new BGAnnotationViewMetadataEditDetailsUI(this);
  m_annotation = annotation;
  auto controlsTab = new QTabWidget(this);
  controlsTab->addTab(m_controls, "Graphics");
  controlsTab->addTab(m_annotation, "Annotation");

  // Create status bar
  m_statusBar = new QStatusBar(this);

  // Setup the controller
  uiController = new BGElementEditorUIController(this, m_doBackup);
  uiController->setAnnotationEditor(annotation);
  connect(uiController, &BGElementEditorUIController::serializedToCellML,
            [this](QString path) {
              Q_EMIT cellMLFileGenerated(path);
            });

  // cSplitter->addWidget(m_controls);
  cSplitter->addWidget(controlsTab);
  wSplitter->addWidget(cSplitter);
  wSplitter->setHandleWidth(5);
  // Setup splitter collapsing experience
  {
    auto handle = wSplitter->handle(1);
    auto layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    auto m_widgetBoxButton = new QToolButton(handle);
    m_widgetBoxButton->setArrowType(Qt::RightArrow);
    m_widgetBoxButton->setVisible(false);
    m_widgetBoxButton->setIconSize(QSize(15, 15));
    connect(m_widgetBoxButton, &QToolButton::clicked,
            [wSplitter, m_widgetBoxButton]() {
              int largeWidth = QGuiApplication::primaryScreen()->size().width();
              wSplitter->setSizes(QList<int>(
                  {std::min(600, largeWidth / 4),
                   std::max(largeWidth - 600, (3 * largeWidth) / 4)}));
              m_widgetBoxButton->setVisible(false);
              wSplitter->setHandleWidth(5);
            });
    connect(wSplitter, &QSplitter::splitterMoved,
            [m_widgetBoxButton, wSplitter](int pos, int index) {
              Q_UNUSED(index);
              if (pos < 5) { // When widgetBox is closed
                m_widgetBoxButton->setVisible(true);
                wSplitter->setHandleWidth(25);
              } else {
                m_widgetBoxButton->setVisible(false);
              }
            });
  }
  {
    auto handle = cSplitter->handle(1);
    auto layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    auto m_controlBoxButton = new QToolButton(handle);
    m_controlBoxButton->setArrowType(Qt::LeftArrow);
    m_controlBoxButton->setVisible(false);
    m_controlBoxButton->setIconSize(QSize(15, 15));
    connect(m_controlBoxButton, &QToolButton::clicked,
            [cSplitter, m_controlBoxButton]() {
              int largeWidth = QGuiApplication::primaryScreen()->size().width();
              cSplitter->setSizes(
                  QList<int>({(largeWidth * 3) / 4, largeWidth / 4}));
              m_controlBoxButton->setVisible(false);
              cSplitter->setHandleWidth(5);
            });
    connect(cSplitter, &QSplitter::splitterMoved,
            [m_controlBoxButton, cSplitter](int pos, int index) {
              Q_UNUSED(pos);
              Q_UNUSED(index);
              if (cSplitter->sizes().at(1) < 5) { // When controlBox is closed
                m_controlBoxButton->setVisible(true);
                cSplitter->setHandleWidth(25);
              } else {
                m_controlBoxButton->setVisible(false);
              }
            });
  }
  uiController->doReadSettings(getApplicationSettings());
  int largeWidth = QGuiApplication::primaryScreen()->size().width();
  wSplitter->setSizes(
      QList<int>({std::min(400, largeWidth / 4),
                  std::max(largeWidth - 400, (3 * largeWidth) / 4)}));

  mainLayout->addWidget(wSplitter, 10);

  mainLayout->addWidget(m_statusBar, 0);
  setLayout(mainLayout);

  resize(1600, 1000);
}

QString BondGraphUIWidget::iconURI(QString domain, QString element) {
  return m_widgetBox->getIconURI(domain, element);
}

void BondGraphUIWidget::addControlPane(QString name, QWidget *widget) {
  auto *frame = new QFrame();
  frame->setLayout(new QVBoxLayout());
  frame->layout()->addWidget(widget);
  dynamic_cast<QVBoxLayout *>(frame->layout())->addStretch();
  m_controls->addContentPane(name, frame);
}

void BondGraphUIWidget::showEvent(QShowEvent *event) {
  QWidget::showEvent(event);
  if (m_initialShowEvent) { // check for backups from previous crash, if there
                            // is one offer to load
    m_initialShowEvent = false;
    uiController->reloadBackup();
  }
}

void BondGraphUIWidget::retranslateUi() {}

void BondGraphUIWidget::closeEvent(QCloseEvent *event) {
  // Remove backup file
  uiController->removeBackUp();
  // then call parent's procedure
  QWidget::closeEvent(event);
}

void BondGraphUIWidget::saveFile(const QString &pOldFileName,
                                 const QString &pNewFileName,
                                 bool &pNeedFeedback) {
  Q_UNUSED(pOldFileName);
  Q_UNUSED(pNewFileName);
  Q_UNUSED(pNeedFeedback);
  // Ask user to us
  uiController->exportFile();
}
void BondGraphUIWidget::cut() { uiController->cut(); }
void BondGraphUIWidget::copy() { uiController->copy(); }
void BondGraphUIWidget::paste() { uiController->paste(); }
void BondGraphUIWidget::del() { uiController->del(); }
void BondGraphUIWidget::undo() { uiController->undo(); }
void BondGraphUIWidget::redo() { uiController->redo(); }

} // namespace BondGraphEditorWindow
} // namespace OpenCOR