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
#include "bgannotationviewmetadataeditdetailsui.h"

#include <QAbstractItemView>
#include <QApplication>
#include <QClipboard>
#include <QComboBox>
#include <QCursor>
#include <QFont>
#include <QFormLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QLocale>
#include <QMenu>
#include <QNetworkAccessManager>
#include <QNetworkInterface>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QRegularExpression>
#include <QSet>
#include <QStringList>
#include <QTimer>
#include <QVariant>

//#include <QWebElement>
//#include <QWebFrame>
//#include <QWebPage>
//#include <QWebView>
#include <QDebug>

#include <QCryptographicHash>

#include "bgannotationmiriamtermsui.h"

#include "bgelement.h"

#include "ui_BGAnnotationViewMetadataEditDetailsUI.h"

#include "bgjsonmodels.h"

#include <set>

QString sha1(const QString &str) {
  // Return the SHA-1 value of the given byte array

  return QCryptographicHash::hash(str.toUtf8(), QCryptographicHash::Sha1)
      .toHex();
}

QStringList qualifiersAsStringList() {
  return {"bio:encodes",
          "bio:hasPart",
          "bio:hasProperty",
          "bio:hasVersion",
          "bio:is",
          "bio:isDescribedBy",
          "bio:isEncodedBy",
          "bio:isHomologTo",
          "bio:isPartOf",
          "bio:isPropertyOf",
          "bio:isVersionOf",
          "bio:occursIn",
          "bio:hasTaxon",
          "model:is",
          "model:isDerivedFrom",
          "model:isDescribedBy",
          "model:isInstanceOf",
          "model:hasInstance"};
}

bool hasInternetConnection1() {
  // Check whether an Internet connection is available, this by going through
  // all of our network interfaces and checking whether one of them is both
  // active and is not a loopback, and has at least one IPv4 IP address

  const QList<QNetworkInterface> networkInterfaces =
      QNetworkInterface::allInterfaces();

  for (const auto &networkInterface : networkInterfaces) {
    QNetworkInterface::InterfaceFlags interfaceFlags = networkInterface.flags();

    if (interfaceFlags.testFlag(QNetworkInterface::IsUp) &&
        !interfaceFlags.testFlag(QNetworkInterface::IsLoopBack)) {
      const QList<QNetworkAddressEntry> addressEntries =
          networkInterface.addressEntries();

      for (const auto &addressEntry : addressEntries) {
        QAbstractSocket::NetworkLayerProtocol protocol =
            addressEntry.ip().protocol();

        if ((protocol == QAbstractSocket::IPv4Protocol) ||
            (protocol == QAbstractSocket::AnyIPProtocol)) {
          return true;
        }
      }
    }
  }

  return false;
}

QString stringFromPercentEncoding1(const QString &pString) {
  // Remove the percent encoding from the given string

  return QUrl::fromPercentEncoding(pString.toUtf8());
}

bool decodeTerm1(const QString &pTerm, QString &pResource, QString &pId) {
  // Decode the term, based on whether it matches that of a MIRIAN URN or an
  // identifiers.org URI
  static const auto ResourceRegEx =
      QStringLiteral("[0-9a-z]+((-|\\.)[0-9a-z]+)?");
  static const auto IdRegEx =
      QStringLiteral("[0-9A-Za-z\\.%-_:]+(:[0-9A-Za-z\\.%-_:]+)?");
  static const QRegularExpression MiriamUrnRegEx =
      QRegularExpression("^urn:miriam:" + ResourceRegEx + ":" + IdRegEx);
  static const QRegularExpression IdentifierDotOrgRegEx = QRegularExpression(
      "^http://identifiers.org/" + ResourceRegEx + "/#?" + IdRegEx);

  if (MiriamUrnRegEx.match(pTerm).hasMatch()) {
    // The term is a MIRIAM URN, so retrieve its corresponding resource and
    // id

    QStringList miriamUrnList = pTerm.split(':');

    pResource = stringFromPercentEncoding1(miriamUrnList[2]);
    pId = stringFromPercentEncoding1(miriamUrnList[3]);

    return true;
  }

  if (IdentifierDotOrgRegEx.match(pTerm).hasMatch()) {
    // The term is an identifiers.org URI, so retrieve its corresponding
    // resource and id

    QString identifiersDotOrgUri = pTerm;
    // Note: the above is because pTerm is a const, so we can't directly use
    //       QString::remove() on it...
    QStringList identifiersDotOrgUriList =
        identifiersDotOrgUri.remove("http://identifiers.org/").split('/');

    pResource = stringFromPercentEncoding1(identifiersDotOrgUriList[0]);
    pId = stringFromPercentEncoding1(identifiersDotOrgUriList[1]);

    return true;
  }

  // Not a term we can recognise

  pResource = "???";
  pId = "???";

  return false;
}

namespace OpenCOR {
namespace BondGraphUI {

const QStringList
    BGAnnotationViewMetadataEditDetailsUI::m_supportedAnnotations_static = {
        "bio:encodes",
        "bio:hasPart",
        "bio:hasProperty",
        "bio:hasVersion",
        "bio:is",
        "bio:isDescribedBy",
        "bio:isEncodedBy",
        "bio:isHomologTo",
        "bio:isPartOf",
        "bio:isPropertyOf",
        "bio:isVersionOf",
        "bio:occursIn",
        "bio:hasTaxon",
        "model:is",
        "model:isDerivedFrom",
        "model:isDescribedBy",
        "model:isInstanceOf",
        "model:hasInstance"};

BGAnnotationMetadataEditDetailsUiItem::BGAnnotationMetadataEditDetailsUiItem(
    const QString &pName, const QString &pResource, const QString &pId)
    : mName(pName), mResource(pResource), mId(pId) {}

//==============================================================================

bool BGAnnotationMetadataEditDetailsUiItem::compare(
    const BGAnnotationMetadataEditDetailsUiItem &pItem1,
    const BGAnnotationMetadataEditDetailsUiItem &pItem2) {
  // Determine which of the two items should be first

  int nameComparison = pItem1.name().compare(pItem2.name());
  int resourceComparison = pItem1.resource().compare(pItem2.resource());

  return (nameComparison == 0) ? (resourceComparison == 0)
                                     ? pItem1.id().compare(pItem2.id()) < 0
                                     : resourceComparison < 0
                               : nameComparison < 0;
}

//==============================================================================

QString BGAnnotationMetadataEditDetailsUiItem::name() const {
  // Return our name

  return mName;
}

//==============================================================================

QString BGAnnotationMetadataEditDetailsUiItem::resource() const {
  // Return our resource

  return mResource;
}

//==============================================================================

QString BGAnnotationMetadataEditDetailsUiItem::id() const {
  // Return our id

  return mId;
}

//==============================================================================

BGAnnotationViewMetadataEditDetailsUI::BGAnnotationViewMetadataEditDetailsUI(
    QWidget *pParent)
    : QWidget(pParent), ui(new Ui::BGAnnotationViewMetadataEditDetailsUI) {

  ui->setupUi(this);
  m_tui = new BGAnnotationMiriamtermsUI(
      ui->fmaSearchTerm, ui->fmaSearchResultsTV, ui->relationShips, this);

  connect(m_tui, SIGNAL(relationshipsAssigned(nlohmann::json)), this,
          SLOT(relationshipsAssigned(nlohmann::json)));

  connect(ui->relationShipView,
          SIGNAL(customContextMenuRequested(const QPoint &)), this,
          SLOT(onRelationShipViewContextMenu(const QPoint &)));

  connect(ui->notes, SIGNAL(textChanged()), this, SLOT(itemNotesChanged()),
          Qt::QueuedConnection);
  // Remove groupbox borders
  // ui->elementNameGB->setStyleSheet("border:0;");

  ui->relationShipView->setContextMenuPolicy(Qt::CustomContextMenu);
  ui->relationShipView->installEventFilter(this);
  ui->fmaSearchTerm->installEventFilter(this);
  m_model = new QJsonTreeModel();
  ui->relationShipView->setModel(m_model);
}

//==============================================================================

BGAnnotationViewMetadataEditDetailsUI::
    ~BGAnnotationViewMetadataEditDetailsUI() {
  delete m_tui;
  delete ui;
}

void BGAnnotationViewMetadataEditDetailsUI::itemNotesChanged() {
  if (m_currentElement != nullptr && !updateLock) {
    updateLock = true;
    nlohmann::json &json = m_currentElement->getJson();
    json["Notes"] = ui->notes->toPlainText().toStdString();
    m_elementDataUpdated = true;
    updateLock = false;
  }
}

void BGAnnotationViewMetadataEditDetailsUI::onRelationShipViewContextMenu(
    const QPoint &point) {
  QModelIndex index = ui->relationShipView->indexAt(point);
  QString relationship = index.data().toString();

  if (!(relationship.startsWith("bio:") || relationship.startsWith("model:"))) {
    QMenu contextMenu;
    QAction *act =
        contextMenu.addAction(tr("Delete"), this, SLOT(deleteRelationship()));
    // Get the parent key and child value too
    act->setData(index.data(Qt::UserRole));
    contextMenu.exec(ui->relationShipView->viewport()->mapToGlobal(point));
  }
}

void BGAnnotationViewMetadataEditDetailsUI::deleteRelationship() {
  QAction *action = qobject_cast<QAction *>(sender());
  if (!action) {
    return;
  }
  if (!action->data().isNull()) {
    QPair<QString, QString> kDdata =
        action->data().value<QPair<QString, QString>>();
    QList<int> expandedItems;
    // Store expanded states
    for (int row = 0; row < ui->relationShipView->model()->rowCount(); ++row) {
      QModelIndex index = ui->relationShipView->model()->index(row, 0);
      if (index.isValid() && ui->relationShipView->isExpanded(index)) {
        expandedItems << row;
      }
    }
    auto mjson = m_model->json();
    auto rel = mjson[kDdata.first.toStdString()];
    nlohmann::json::iterator it = rel.end();
    std::string sc = kDdata.second.toStdString();
    for (auto itc = rel.begin(); itc != it; ++itc) {
      std::string vv = *itc;
      if (vv.compare(sc) == 0) {
        rel.erase(itc);
        break;
      }
    }
    mjson[kDdata.first.toStdString()] = rel;
    m_model->loadJson(mjson);
    m_elementDataUpdated = true;
    for (auto row : expandedItems) {
      QModelIndex index = ui->relationShipView->model()->index(row, 0);
      if (index.isValid()) {
        ui->relationShipView->setExpanded(index, true);
      }
    }
  }
}

void BGAnnotationViewMetadataEditDetailsUI::setBGElement(BGElement *elem) {
  if (elem->getId() == "ConnectHelp")
    return;
  if (m_currentElement != nullptr && m_elementDataUpdated) {
    auto mjson = m_model->json();
    nlohmann::json arr = nlohmann::json::array();
    for (int row = 0; row < ui->relationShipView->model()->rowCount(); ++row) {
      QModelIndex index = ui->relationShipView->model()->index(row, 0);
      if (index.isValid() && ui->relationShipView->isExpanded(index)) {
        arr.push_back(row);
      }
    }
    mjson["openItemIndex"] = arr;
    nlohmann::json &json = m_currentElement->getJson();
    json["Annotation"] = mjson;
    json["Notes"] = ui->notes->toPlainText().toStdString();
    m_currentElement->setJson(json);
    Q_EMIT annotationUpdated(m_currentElement);
  }
  nlohmann::json &json = elem->getJson();
  m_elementDataUpdated = false;
  if (json.contains("Notes")) {
    ui->notes->document()->setPlainText(QString::fromStdString(json["Notes"]));
  }
  ui->stackedWidget->setCurrentIndex(1);

  m_currentElement = elem;
  if (m_currentElement->getClassType() != "annotation") {
    ui->elementNameGB->setVisible(true);
    ui->annotationsGB->setVisible(true);

    auto list = qualifiersAsStringList();
    ui->elementName->setPixmap(elem->getRenderedLatex());

    nlohmann::json &anot = json["Annotation"];
    m_model->loadJson(anot);

    if (anot.contains("openItemIndex")) {
      nlohmann::json &arr = anot["openItemIndex"];
      for (int row : arr) {
        QModelIndex index = ui->relationShipView->model()->index(row, 0);
        if (index.isValid()) {
          ui->relationShipView->setExpanded(index, true);
        }
      }
    }
    ui->fmaSearchTerm->setText("");
  } else {
    ui->elementNameGB->setVisible(false);
    ui->annotationsGB->setVisible(false);
  }
}

void BGAnnotationViewMetadataEditDetailsUI::reload() {
  if (m_currentElement != nullptr) {
    nlohmann::json &json = m_currentElement->getJson();
    ui->stackedWidget->setCurrentIndex(1);
    auto list = qualifiersAsStringList();
    ui->elementName->setPixmap(m_currentElement->getRenderedLatex());
    nlohmann::json &anot = json["Annotation"];
    m_model->loadJson(anot);
    m_elementDataUpdated = false;
    if (json.contains("Notes")) {
      ui->notes->document()->setPlainText(
          QString::fromStdString(json["Notes"]));
    }
    if (anot.contains("openItemIndex")) {
      nlohmann::json &arr = anot["openItemIndex"];
      for (int row : arr) {
        QModelIndex index = ui->relationShipView->model()->index(row, 0);
        if (index.isValid()) {
          ui->relationShipView->setExpanded(index, true);
        }
      }
    }
    ui->fmaSearchTerm->setText("");
  }
}

void BGAnnotationViewMetadataEditDetailsUI::setNoSelection() {
  if (m_currentElement != nullptr && m_elementDataUpdated) {
    try {
      auto mjson = m_model->json();
      nlohmann::json arr = nlohmann::json::array();
      for (int row = 0; row < ui->relationShipView->model()->rowCount();
           ++row) {
        QModelIndex index = ui->relationShipView->model()->index(row, 0);
        if (index.isValid() && ui->relationShipView->isExpanded(index)) {
          arr.push_back(row);
        }
      }
      if (arr.size() > 0)
        mjson["openItemIndex"] = arr;
      nlohmann::json &json = m_currentElement->getJson();
      json["Annotation"] = mjson;
      json["Notes"] = ui->notes->toPlainText().toStdString();
      m_currentElement->setJson(json);
      Q_EMIT annotationUpdated(m_currentElement);
    } catch (...) {
    }
  }
  m_currentElement = nullptr;
  m_elementDataUpdated = false;
  ui->stackedWidget->setCurrentIndex(0);
}

bool BGAnnotationViewMetadataEditDetailsUI::eventFilter(QObject *watched,
                                                        QEvent *event) {
  Q_UNUSED(watched);
  if (event->type() != QEvent::KeyPress)
    return false;

  QKeyEvent *key_event = dynamic_cast<QKeyEvent *>(event);
  QKeySequence key_sequence{static_cast<int>(key_event->modifiers()) +
                            key_event->key()};

  if (key_sequence == QKeySequence::Undo) {
    // undo_stack_.undo(); // nothing in the stack
    qDebug() << "undo";
    return true;
  } else if (key_sequence == QKeySequence::Redo) {
    // undo_stack_.redo(); // nothing in the stack
    qDebug() << "redo";
    return true;
  }

  return false;
}

void BGAnnotationViewMetadataEditDetailsUI::undo() {
  qDebug() << Q_FUNC_INFO << " called ";
}

void BGAnnotationViewMetadataEditDetailsUI::redo() {
  qDebug() << Q_FUNC_INFO << " called ";
}

void BGAnnotationViewMetadataEditDetailsUI::relationshipsAssigned(
    nlohmann::json nrel) {
  auto mjson = m_model->json();
  QList<int> expandedItems;
  // Store expanded states
  for (int row = 0; row < ui->relationShipView->model()->rowCount(); ++row) {
    QModelIndex index = ui->relationShipView->model()->index(row, 0);
    if (index.isValid() && ui->relationShipView->isExpanded(index)) {
      expandedItems << row;
    }
  }
  for (auto &k : nrel.items()) {
    if (mjson.contains(k.key())) {
      auto &rel = mjson[k.key()];
      if (rel.size() == 0) {
        mjson[k.key()] = nrel[k.key()];
      } else {
        std::set<std::string> items;
        for (auto &val : rel.items()) {
          for (std::string vv : val.value()) {
            items.insert(vv);
          }
        }
        auto nr = nrel[k.key()];
        for (auto &val : nr.items()) {
          std::string vv = val.value();
          items.insert(vv);
        }
        auto arr = nlohmann::json::array();
        for (auto v : items) {
          arr.push_back(v);
        }
        mjson[k.key()] = arr;
      }
    }
  }
  m_model->loadJson(mjson);
  m_elementDataUpdated = true;
  for (auto row : expandedItems) {
    QModelIndex index = ui->relationShipView->model()->index(row, 0);
    if (index.isValid()) {
      ui->relationShipView->setExpanded(index, true);
    }
  }
}

//==============================================================================

QString resourceUrl1(const QString &pResource) {
  // Return the URL for the given resource

  return "https://registry.identifiers.org/registry/" + pResource;
}

QString idUrl1(const QString &pId) {
  // Return the URL for the given resource

  return "https://identifiers.org/" + pId;
}

} // namespace BondGraphUI
} // namespace OpenCOR
