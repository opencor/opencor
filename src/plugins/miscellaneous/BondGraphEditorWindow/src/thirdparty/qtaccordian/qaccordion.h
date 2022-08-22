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


#include <QGridLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QSpacerItem>
#include <QString>
#include <QStyleOption>
#include <QVBoxLayout>
#include <QWidget>

#include <algorithm>
#include <iterator>
#include <memory>
#include <vector>

namespace OpenCOR {
namespace BondGraphEditorWindow {

class ContentPane;

class QAccordion : public QWidget {
  Q_OBJECT
public:
  QAccordion(QWidget *parent = nullptr);

  int numberOfContentPanes() const;

  int addContentPane(QString header);
  int addContentPane(QString header, QFrame *contentFrame);
  int addContentPane(ContentPane *cpane);
  bool insertContentPane(uint index, QString header);
  bool insertContentPane(uint index, QString header, QFrame *contentFrame);
  bool insertContentPane(uint index, ContentPane *cpane);
  bool swapContentPane(uint index, ContentPane *cpane);
  bool removeContentPane(bool deleteObject, uint index);
  bool removeContentPane(bool deleteObject, QString header);
  bool removeContentPane(bool deleteObject, QFrame *contentframe);
  bool removeContentPane(bool deleteObject, ContentPane *contentPane);
  bool moveContentPane(uint currentIndex, uint newIndex);
  ContentPane *getContentPane(uint index);
  int getContentPaneIndex(QString header) const;
  int getContentPaneIndex(QFrame *contentFrame) const;
  int getContentPaneIndex(ContentPane *contentPane) const;
  void getActiveContentPaneIndex(std::vector<int> &indexVector) const;
  int getNumberOfContentPanes() const;
  void setMultiActive(bool status);
  bool getMultiActive() const;
  void setCollapsible(bool status);
  bool getCollapsible() const;
  QString getError();

Q_SIGNALS:
  void numberOfContentPanesChanged(int number);

public Q_SLOTS:

private:
  std::vector<ContentPane *> contentPanes;

  QSpacerItem *spacer;

  QString errorString;

  bool multiActive;
  bool collapsible;

  int internalAddContentPane(QString header, QFrame *cframe = nullptr,
                             ContentPane *cpane = nullptr);
  bool internalInsertContentPane(uint index, QString header,
                                 QFrame *contentFrame = nullptr,
                                 ContentPane *cpane = nullptr);
  bool internalRemoveContentPane(bool deleteOject, int index = -1,
                                 QString name = "",
                                 QFrame *contentFrame = nullptr,
                                 ContentPane *cpane = nullptr);
  int findContentPaneIndex(QString name = "", QFrame *cframe = nullptr,
                           ContentPane *cpane = nullptr) const;

  bool checkIndexError(uint index, bool sizeIndexAllowed,
                       const QString &errMessage);
  void handleClickedSignal(ContentPane *cpane);

private Q_SLOTS:
  void numberOfPanesChanged(int number);

protected:
  void paintEvent(QPaintEvent *event) override;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR




