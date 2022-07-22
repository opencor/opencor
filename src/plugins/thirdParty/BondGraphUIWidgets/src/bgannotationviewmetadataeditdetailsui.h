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
#include <QSslError>
#include <QWidget>

class QComboBox;
class QLabel;
class QLineEdit;
class QMenu;
class QNetworkAccessManager;
class QNetworkReply;
class QPushButton;

namespace Ui {
class BGAnnotationViewMetadataEditDetailsUI;
}

namespace OpenCOR {
namespace BondGraphUI {

class BGElement;
class BGAnnotationMiriamtermsUI;
class QJsonTreeModel;

class BGAnnotationMetadataEditDetailsUiItem {
public:
  explicit BGAnnotationMetadataEditDetailsUiItem(const QString &pName = {},
                                                 const QString &pResource = {},
                                                 const QString &pId = {});

  static bool compare(const BGAnnotationMetadataEditDetailsUiItem &pItem1,
                      const BGAnnotationMetadataEditDetailsUiItem &pItem2);

  QString name() const;
  QString resource() const;
  QString id() const;

private:
  QString mName;
  QString mResource;
  QString mId;
};

using BGAnnotationMetadataEditDetailsUiItems =
    QList<BGAnnotationMetadataEditDetailsUiItem>;

class BGAnnotationViewMetadataEditDetailsUI : public QWidget {
  Q_OBJECT

public:
  explicit BGAnnotationViewMetadataEditDetailsUI(QWidget *pParent = 0);
  ~BGAnnotationViewMetadataEditDetailsUI();
  bool eventFilter(QObject *watched, QEvent *event) override;

Q_SIGNALS:
  void annotationUpdated(BGElement *elem);

public Q_SLOTS:
  void setBGElement(BGElement *elem);
  void setNoSelection();
  void reload();
  void undo();
  void redo();

protected Q_SLOTS:
  void relationshipsAssigned(nlohmann::json nrel);
  void onRelationShipViewContextMenu(const QPoint &point);
  void deleteRelationship();
  void itemNotesChanged();

private:
  Ui::BGAnnotationViewMetadataEditDetailsUI *ui;

  enum class InformationType { None, Qualifier, Resource, Id };

  QString mTerm;
  QStringList mTerms;

  BGAnnotationMetadataEditDetailsUiItems mItems;

  bool mLookUpTerm = false;
  QString mErrorMessage;
  bool mHasInternetConnection = true;

  QString mOutputOntologicalTermsTemplate;
  // WebViewerWidget::WebViewerWidget *mOutputOntologicalTerms;

  InformationType mInformationType = InformationType::None;

  bool mLookUpInformation = false;
  bool updateLock = false;
  QMap<QString, BGAnnotationMetadataEditDetailsUiItem> mItemsMapping;
  QMap<QString, bool> mEnabledItems;

  BGElement *mElement = nullptr;

  QMap<QString, QString> m_Urls;
  QStringList m_ItemInformationSha1s;
  QString m_ItemInformationSha1;

  QString m_Link;
  QString m_TextContent;

  QMenu *m_ContextMenu;

  QAction *m_CopyAction;
  QAction *m_relationshipDeleteAction;
  QNetworkReply *m_NetworkReply = nullptr;
  BGAnnotationMiriamtermsUI *m_tui = nullptr;
  QJsonTreeModel *m_model = nullptr;
  BGElement *m_currentElement = nullptr;
  bool m_elementDataUpdated = false;

public:
  static const QStringList m_supportedAnnotations_static;
};

} // namespace BondGraphUI
} // namespace OpenCOR
