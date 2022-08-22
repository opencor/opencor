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
#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wold-style-cast"
#endif

#include <QBoxLayout>
#include <QCoreApplication>

#include <QDir>
#include <QDrag>
#include <QEvent>
#include <QHeaderView>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMimeData>
#include <QMouseEvent>
#include <QPixmap>
#include <QTreeWidget>

#include "categorywidgets.h"
#include "libraryitem.h"
#include "pageeventfilter.h"
#include "widgetbox.h"

// Q_INIT_RESOURCE should be called from global namespace
void initializeWidgeBoxResources() { Q_INIT_RESOURCE(widgetboxicons); }

namespace OpenCOR {
namespace BondGraphEditorWindow {

WidgetBox::WidgetBox(QWidget *parent) : QWidget(parent) {

  initializeWidgeBoxResources();

  // Set default tree widget settings
  mTreeWidget = new QTreeWidget(this);
  mTreeWidget->setFrameStyle(QFrame::NoFrame);
  mTreeWidget->setAnimated(true);
  mTreeWidget->setVerticalScrollMode(QTreeWidget::ScrollPerPixel);
  mTreeWidget->header()->hide();
  mTreeWidget->setRootIsDecorated(false);
  mTreeWidget->setIndentation(0);
  mTreeWidget->setUniformRowHeights(false);
  mTreeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
  mTreeWidget->setExpandsOnDoubleClick(false);
  mTreeWidget->setDragEnabled(true);

  // Make tree widget same color as normal widget
  // QPalette pal = mTreeWidget->palette();
  // pal.setColor(QPalette::Base, palette().background().color());
  // mTreeWidget->setPalette(pal);

  // Create WidgetBox layout
  QBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(mTreeWidget);

  connect(mTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
          SLOT(onItemClicked(QTreeWidgetItem *, int)));
  connect(mTreeWidget, SIGNAL(itemActivated(QTreeWidgetItem *, int)),
          SLOT(onItemClicked(QTreeWidgetItem *, int)));
}

WidgetBox::~WidgetBox() {}

QString WidgetBox::getIconURI(QString domain, QString element) {
  if (elementMap.contains(domain)) {
    QMap<QString, QString> &iconMap = elementMap[domain];
    if (iconMap.contains(element)) {
      return iconMap[element];
    }
  } else {
    QMap<QString, QString> &iconMap = elementMap["Generic"];
    if (iconMap.contains(element)) {
      return iconMap[element];
    }
  }
  throw "No icon assigned for " + element + " of domain " + domain;
}

void WidgetBox::load(QString ijson) {
  // std::map<QString, std::map<QString, QString> > elementMap;
  // std::map<QString, std::map<QString, QByteArray> > mimeData;
  elementMap.clear();
  mimeData.clear();
  QMap<int, QString> catOrder = {{1, "Generic"},     {2, "Electrical"},
                                 {3, "Biochemical"}, {4, "Fluid Mechanics"},
                                 {5, "Composition"}, {6, "Annotation"}};
  int maxOrder = (int)catOrder.size();
  QString installPath =
      QCoreApplication::applicationDirPath(); // Ideally installation path
  // Check applications settings to find if any other location is
  QJsonParseError jsonError;
  QJsonDocument json = QJsonDocument::fromJson(ijson.toUtf8(), &jsonError);
  if (jsonError.error == QJsonParseError::NoError) {
    QJsonObject root = json.object();
    QJsonObject icons = root["Icons"].toObject();
    // Load the icons
    for (auto key : icons.keys()) {
      QString domain = key;
      QJsonObject domainIcons = icons[key].toObject();
      QMap<QString, QString> iconMap;
      for (auto ik : domainIcons.keys()) {
        QString filename = domainIcons[ik].toString();
        if (filename.at(0) == ':') { // Resource icon
          iconMap[ik] = filename;
        } else if (filename.at(0) == '.') { // With respect to installation
          QString iconFile =
              QDir(installPath).filePath(filename.right(filename.length() - 1));
          iconMap[ik] = iconFile;
        } else {
          iconMap[ik] = filename;
        }
      }
      elementMap[domain] = iconMap;
    }
    QJsonObject elementDef = root["ElementDefinitions"].toObject();
    // Load the mimeData
    for (auto key : elementDef.keys()) {
      QString domain = key;
      QJsonObject domainDef = elementDef[key].toObject();
      QMap<QString, QByteArray> ddata;
      bool catOrderSet = false;

      for (auto dd : domainDef.keys()) {
        if (dd.compare("location", Qt::CaseInsensitive) == 0) {
          int order = domainDef[dd].toInt();
          if (catOrder.find(order) != catOrder.end() &&
              catOrder[order] != domain) {
            catOrder[maxOrder + 1] = domain;
            maxOrder += 1;
          } else {
            catOrder[order] = domain;
          }
          catOrderSet = true;
          continue;
        }
        // Skip Bond dimensions
        if (dd.compare("BondDimensions", Qt::CaseInsensitive) != 0) {
          auto dob = domainDef[dd].toObject();
          // Add the type information, as it is used for type specific settings
          dob["type"] = dd;
          QByteArray value = QJsonDocument(dob).toJson(QJsonDocument::Compact);
          ddata[dd] = value;
        }
      }
      // Set the order in which the domain will be listed in the
      if (!catOrderSet) {
        catOrder[maxOrder + 1] = domain;
        maxOrder += 1;
      }
      mimeData[domain] = ddata;
    }
    // Remove any existing definitions
    mTreeWidget->clear();

    // Create a page (qwidget) for each domain
    QVector<QWidget *> pages;
    QMap<QString, bool>
        existingDomains; // catOrder is initialised with default positions for
                         // selected domains. If these domains do not have a
                         // order in the json, they will be assigned another cat
                         // order at 129, this will ensure that only a widget is
                         // added per domain name
    // Get the domain names in the sorted categorical order
    for (const auto &co : catOrder.keys()) {
      QString domain = catOrder[co];
      if (mimeData.find(domain) == mimeData.end() ||
          existingDomains.find(domain) != existingDomains.end())
        continue;

      QMap<QString, QByteArray> definitions = mimeData[domain];
      QMap<QString, QString> domainIcons = elementMap[domain];
      QWidget *domainElementsPage = new QWidget;
      QVBoxLayout *vl = new QVBoxLayout(domainElementsPage);
      vl->setSpacing(0);
      vl->setMargin(0);

      for (auto elementName : definitions.keys()) {
        QString eicon = domainIcons[elementName];
        if (eicon == "") {
          // Use the short name
          QJsonDocument edef =
              QJsonDocument::fromJson(definitions[elementName], &jsonError);
          eicon = edef["shortname"].toString();
        }
        auto dndn1 = new LibraryItem(elementName, eicon, domainElementsPage);
        // Add elementType to mimeData
        dndn1->setMimeData(definitions[elementName]);
        vl->addWidget(dndn1);
      }
      domainElementsPage->setWindowTitle(
          domain); // Set the domain name for the widget box to show, else Page
                   // %# will be shown
      pages.push_back(domainElementsPage);
      existingDomains[domain] = true;
    }
    // Add pages in the specified order
    for (auto page : pages) {
      addPage(page);
    }
  } 
}

int WidgetBox::count() const { return mTreeWidget->topLevelItemCount(); }

void WidgetBox::createCategory(QTreeWidgetItem *page, QString page_name) {
  AbstractCategory *category;
  switch (mCategoryType) {
  case Button:
    category = new ButtonCategory(page_name, mTreeWidget, page);
    break;
  case Line:
    category = new LineCategory(page_name, mTreeWidget, page);
    break;
  default:
    category = new ButtonCategory(page_name, mTreeWidget, page);
    break;
  }
  // Set new item widget: Qt removes old widget automatically, no need to delete
  // old one
  mTreeWidget->setItemWidget(page, 0, category);
  // Change property in Designer on page expand/unexpand
  connect(category, SIGNAL(pageExpanded(bool)),
          SLOT(setPageExpandedProperty()));
}

void WidgetBox::recreateCategories() {
  for (int i = 0; i < count(); i++) {
    QString pageName = pageTitle(i);
    bool expanded = isPageExpanded(i);
    // Qt removes old widget automatically, no need to delete old one
    createCategory(mTreeWidget->topLevelItem(i), pageName);
    category(i)->setExpanded(expanded);
  }
}

void WidgetBox::setCategoryType(const CategoryType type) {
  mCategoryType = type;
  recreateCategories();
}

QTreeWidgetItem *WidgetBox::addCategory(QString page_name) {
  return insertCategory(count(), page_name);
}

void WidgetBox::setupWidget(QWidget *widget) {
  // The given widget's autoFillBackground property must be set to true,
  // otherwise the widget's background will be transparent,
  // showing both the model data and the tree widget item.
  widget->setAutoFillBackground(true);
  widget->setPalette(palette());
  widget->setBackgroundRole(backgroundRole());
  widget->setStyle(style()); // Set parent widget style
}

void WidgetBox::createContainerWidget(QTreeWidgetItem *page, QWidget *widget) {
  QTreeWidgetItem *container = new QTreeWidgetItem();
  container->setDisabled(true);
  page->addChild(container);
  mTreeWidget->setItemWidget(container, 0, widget);
  setupWidget(widget);

  // Send mouse events from page widget to tree widget
  PageEventFilter *eventFilter = new PageEventFilter(this, container);
  widget->installEventFilter(eventFilter);
  widget->setObjectName(QString("CWPage%1").arg(count() + 1));
}

void WidgetBox::addPage(QWidget *widget) { insertPage(count(), widget); }

QTreeWidgetItem *WidgetBox::insertCategory(int index, QString page_name) {
  QTreeWidgetItem *page = new QTreeWidgetItem();
  mTreeWidget->insertTopLevelItem(index, page);
  createCategory(page, page_name);

  return page;
}

void WidgetBox::insertPage(int index, QWidget *widget) {
  if (index < 0 || index > count())
    return;

  // Set default page name
  QString pageName = widget->windowTitle();
  if (pageName.isEmpty()) {
    pageName = tr("Page %1").arg(count() + 1);
    widget->setWindowTitle(pageName);
  }

  QTreeWidgetItem *page = insertCategory(index, pageName);
  createContainerWidget(page, widget);

  connect(widget, SIGNAL(windowTitleChanged(QString)), category(index),
          SLOT(setTitle(QString)));
}

void WidgetBox::removePage(int index) {
  QTreeWidgetItem *topLevelItem = mTreeWidget->topLevelItem(index);
  if (!topLevelItem)
    return;

  if (topLevelItem->childCount() > 0)
    mTreeWidget->removeItemWidget(topLevelItem->child(0), 0);
  mTreeWidget->removeItemWidget(topLevelItem, 0);

  delete topLevelItem;
}

int WidgetBox::currentIndex() const {
  if (count() > 0) {
    return mTreeWidget->currentIndex().row();
  } else {
    return -1;
  }
}

void WidgetBox::setCurrentIndex(int index) {
  if (index != currentIndex() && checkIndex(index)) {
    mTreeWidget->setCurrentItem(mTreeWidget->topLevelItem(index));
    Q_EMIT currentIndexChanged(index);
  }
}

bool WidgetBox::checkIndex(int index) const {
  return index >= 0 && index < count();
}

QWidget *WidgetBox::page(int index) const {
  if (checkIndex(index)) {
    QTreeWidgetItem *page = mTreeWidget->topLevelItem(index);
    return mTreeWidget->itemWidget(page->child(0), 0);
  } else {
    return 0;
  }
}

QWidget *WidgetBox::widget(int index) const { return page(index); }

void WidgetBox::setPageTitle(QString const &new_title) {
  if (checkIndex(currentIndex())) {
    category(currentIndex())->setTitle(new_title);
    // Qt doc: use QWidget::windowTitle property to store the page title.
    // Note that currently there is no way of adding a custom property
    // (e.g., a page title) to the pages without using a predefined property as
    // placeholder.
    page(currentIndex())->setWindowTitle(new_title);
    Q_EMIT pageTitleChanged(new_title);
  }
}

QString WidgetBox::pageTitle() const { return pageTitle(currentIndex()); }

QString WidgetBox::pageTitle(int index) const {
  if (checkIndex(index)) {
    return category(index)->title();
  } else {
    return QString();
  }
}

AbstractCategory *WidgetBox::category(int index) const {
  return (AbstractCategory *)mTreeWidget->itemWidget(
      mTreeWidget->topLevelItem(index), 0);
}

bool WidgetBox::isPageExpanded() const {
  return isPageExpanded(currentIndex());
}

bool WidgetBox::isPageExpanded(int index) const {
  if (checkIndex(index)) {
    return category(index)->isExpanded();
  } else {
    return false;
  }
}

void WidgetBox::setPageExpandedProperty() {
  changeQtDesignerProperty("isPageExpanded", isPageExpanded(currentIndex()),
                           true);
}

void WidgetBox::setPageExpanded(bool expanded) {
  if (checkIndex(currentIndex())) {
    setPageExpanded(currentIndex(), expanded);
    setPageExpandedProperty();
  }
}

void WidgetBox::setPageExpanded(int index, bool expanded) {
  if (checkIndex(index))
    category(index)->setExpanded(expanded);
}

int WidgetBox::getPageIndex(QTreeWidgetItem *item) {
  if (!item)
    return -1;

  QTreeWidgetItem *parent = item->parent();
  if (parent) // Parent is top level item
  {
    return mTreeWidget->indexOfTopLevelItem(parent);
  } else // Current item is top level
  {
    return item->treeWidget()->indexOfTopLevelItem(item);
  }
}

void WidgetBox::onItemClicked(QTreeWidgetItem *item, int) {
  int index = getPageIndex(item);
  setCurrentIndex(index);

  changeQtDesignerProperty("currentIndex", index);
}

void WidgetBox::mousePressEvent(QMouseEvent *event) {
  QLabel *child = static_cast<QLabel *>(childAt(event->pos()));

  if (!child)
    return;

  QPixmap pixmap = *child->pixmap();

  QByteArray itemData;
  QDataStream dataStream(&itemData, QIODevice::WriteOnly);
  dataStream << pixmap << QPoint(event->pos() - child->pos());
  QMimeData *mimeDataptr = new QMimeData;
  mimeDataptr->setData("application/x-dnditemdata", itemData);
  QDrag *drag = new QDrag(this);
  drag->setMimeData(mimeDataptr);
  drag->setPixmap(pixmap);
  drag->setHotSpot(event->pos() - child->pos());
}

void WidgetBox::changeQtDesignerProperty(QString property_name, QVariant value,
                                         bool mark_changed_only) {
  Q_UNUSED(property_name);
  Q_UNUSED(value);
  Q_UNUSED(mark_changed_only);
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR


