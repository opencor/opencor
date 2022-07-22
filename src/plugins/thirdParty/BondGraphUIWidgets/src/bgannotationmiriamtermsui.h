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

#include "thirdparty/json.hpp"
#include <QAbstractTableModel>
#include <QList>
#include <QNetworkAccessManager>
#include <QSslError>
#include <QStringList>
#include <QWidget>

class QStandardItemModel;
class QSortFilterProxyModel;
class QTableView;
class QLineEdit;
class QNetworkReply;
class QUrl;
class QModelIndex;
class QComboBox;

namespace OpenCOR {
namespace BondGraphUI {

class DownloadManager : public QObject {
  Q_OBJECT

public:
  DownloadManager();
  void doDownload(const QUrl &url);
  static bool isHttpRedirect(QNetworkReply *reply);

public Q_SLOTS:
  void downloadFinished(QNetworkReply *reply);
  void sslErrors(const QList<QSslError> &error);

Q_SIGNALS:
  void downloadCompleted(QUrl &url, QString result);
  void downloadFailed(QUrl &url, QString result);

protected:
  QNetworkAccessManager mManager;
  QNetworkReply *mCurrentDownloads;
  QList<QSslError> msslErrors;
};

class BGAnnotationMiriamtermsModel : public QAbstractTableModel {
  Q_OBJECT
public:
  BGAnnotationMiriamtermsModel(QStandardItemModel *model,
                               QObject *parent = nullptr);
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role) const override;

protected:
  QStandardItemModel *model;
};

class BGAnnotationMiriamtermsUI : public QObject {
  Q_OBJECT

public:
  explicit BGAnnotationMiriamtermsUI(QLineEdit *searchTerm,
                                     QTableView *itemsTable, QComboBox *rel,
                                     QWidget *pParent);

  void reset();
  void selectTerms(const QList<QString> &terms);

Q_SIGNALS:
  void termSelected(QString term);
  void dataLoaded();
  void relationshipsAssigned(nlohmann::json json);

public Q_SLOTS:
  void termDeSelected(QString term);
  void filterText(QString term);
  void cellClicked(const QModelIndex &index);
  void assignRelationship(int index);

protected:
  void loadMiriamTermsFromResources();
  QString qError = "";
  BGAnnotationMiriamtermsModel *m_miriamTerms;
  QLineEdit *m_searchTerm;
  QTableView *m_itemsTable;
  QComboBox *m_relationships;
  
  QSortFilterProxyModel *m_Proxy;
  QSortFilterProxyModel *m_Empty;
};

} // namespace BondGraphUI
} // namespace OpenCOR