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

#include <QMap>
#include <QPushButton>
#include <QWidget>


class QTreeWidget;
class QTreeWidgetItem;

namespace OpenCOR {
namespace BondGraphEditorWindow {

class AbstractCategory;

enum CategoryType { Button, Line };

class WidgetBox : public QWidget {
  Q_OBJECT

public:
  WidgetBox(QWidget *parent = nullptr);
  ~WidgetBox();

  CategoryType categoryType() const { return mCategoryType; }
  void setCategoryType(const CategoryType type);
  void load(QString json);
  int count() const;
  QWidget *page(int index) const;
  QWidget *widget(int index) const;
  QString pageTitle() const;
  QString pageTitle(int index) const;
  bool isPageExpanded() const;
  bool isPageExpanded(int index) const;
  int currentIndex() const;
  QString getIconURI(QString domain, QString element);

public Q_SLOTS:
  void setCurrentIndex(int index);

  void addPage(QWidget *widget);
  void insertPage(int index, QWidget *widget);
  void removePage(int index);

  void setPageTitle(QString const &newTitle);
  void setPageExpanded(bool expanded);
  void setPageExpanded(int index, bool expanded);

protected:
  QTreeWidgetItem *addCategory(QString pageName);
  QTreeWidgetItem *insertCategory(int index, QString pageName);

  AbstractCategory *category(int index) const;

  void createContainerWidget(QTreeWidgetItem *page, QWidget *widget);
  void createCategory(QTreeWidgetItem *page, QString pageName);
  void changeQtDesignerProperty(QString propertyName, QVariant value,
                                bool markChangedOnly = false);

protected Q_SLOTS:
  void onItemClicked(QTreeWidgetItem *item, int);
  void setPageExpandedProperty();
  void mousePressEvent(QMouseEvent *event);

Q_SIGNALS:
  void currentIndexChanged(int index);
  void pageTitleChanged(const QString &title);

private:
  bool checkIndex(int index) const;
  void setupWidget(QWidget *widget);
  int getPageIndex(QTreeWidgetItem *item);
  void recreateCategories();

  QTreeWidget *mTreeWidget;
  CategoryType mCategoryType = Button;
  QMap<QString, QMap<QString, QString>> elementMap;
  QMap<QString, QMap<QString, QByteArray>> mimeData;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR