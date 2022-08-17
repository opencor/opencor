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

#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

#include "categorybutton.h"

class QTreeWidgetItem;
class QTreeWidget;

namespace OpenCOR {
namespace BondGraphEditorWindow {

class PageEventFilter;

class AbstractCategory : public QWidget {
  Q_OBJECT
public:
  AbstractCategory(const QString &text, QTreeWidget *parent,
                   QTreeWidgetItem *item);
  bool isExpanded() const;
  virtual QString title() const = 0;

public Q_SLOTS:
  virtual void setTitle(QString const &title) = 0;
  virtual void setExpanded(bool expanded) { onPageExpand(expanded); }

protected:
  QTreeWidgetItem *item() const { return mItem; }
  PageEventFilter *eventFilter() const { return mEventFilter; }
  int itemIndex() const;

Q_SIGNALS:
  void pageExpanded(bool expanded);

protected Q_SLOTS:
  virtual void onButtonPress();
  void onPageExpand(bool expanded = false);

private:
  QTreeWidgetItem *mItem;
  PageEventFilter *mEventFilter;
};

class ButtonCategory : public AbstractCategory {
  Q_OBJECT
public:
  ButtonCategory(const QString &text, QTreeWidget *parent,
                 QTreeWidgetItem *item);
  QString title() const override { return mButton->text(); }

public Q_SLOTS:
  void setTitle(QString const &title) override;

private Q_SLOTS:
  void onButtonPress() override;

private:
  CategoryButton *mButton;
};

class LineCategory : public AbstractCategory {
  Q_OBJECT
public:
  LineCategory(const QString &text, QTreeWidget *parent, QTreeWidgetItem *item);
  QString title() const override { return mLabel->text(); }

public Q_SLOTS:
  void setTitle(QString const &title) override;
  void setExpanded(bool expanded) override;

private:
  QLabel *mLabel;
  QCheckBox *mCheckBox;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
