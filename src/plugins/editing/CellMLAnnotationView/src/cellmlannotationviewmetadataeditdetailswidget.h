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

//==============================================================================
// CellML Annotation view metadata edit details widget
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfile.h"
#include "widget.h"

//==============================================================================

#include <QSslError>

//==============================================================================

class QComboBox;
class QLabel;
class QLineEdit;
class QMenu;
class QNetworkAccessManager;
class QNetworkReply;
class QPushButton;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class UserMessageWidget;
} // namespace Core

//==============================================================================

namespace WebViewerWidget {
    class WebViewerWidget;
} // namespace WebViewerWidget

//==============================================================================

namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewEditingWidget;
class CellmlAnnotationViewWidget;

//==============================================================================

class CellmlAnnotationViewMetadataEditDetailsItem
{
public:
    explicit CellmlAnnotationViewMetadataEditDetailsItem(const QString &pName = {},
                                                         const QString &pResource = {},
                                                         const QString &pId = {});

    static bool compare(const CellmlAnnotationViewMetadataEditDetailsItem &pItem1,
                        const CellmlAnnotationViewMetadataEditDetailsItem &pItem2);

    QString name() const;
    QString resource() const;
    QString id() const;

private:
    QString mName;
    QString mResource;
    QString mId;
};

//==============================================================================

using CellmlAnnotationViewMetadataEditDetailsItems = QList<CellmlAnnotationViewMetadataEditDetailsItem>;

//==============================================================================

class CellmlAnnotationViewMetadataEditDetailsWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewMetadataEditDetailsWidget(CellmlAnnotationViewWidget *pAnnotationWidget,
                                                           CellmlAnnotationViewEditingWidget *pEditingWidget,
                                                           CellMLSupport::CellmlFile *pCellmlFile,
                                                           QWidget *pParent);

    void retranslateUi() override;

    void filePermissionsChanged();

private:
    enum class InformationType {
        None,
        Qualifier,
        Resource,
        Id
    };

    CellmlAnnotationViewWidget *mAnnotationWidget;
    CellmlAnnotationViewEditingWidget *mEditingWidget;

    QNetworkAccessManager *mNetworkAccessManager;

    QLabel *mQualifierLabel;
    QComboBox *mQualifierValue;
    QPushButton *mLookUpQualifierButton;

    QLabel *mTermLabel;
    QLineEdit *mTermValue = nullptr;
    QPushButton *mAddTermButton = nullptr;

    QString mTerm;
    QStringList mTerms;

    CellmlAnnotationViewMetadataEditDetailsItems mItems;

    bool mLookUpTerm = false;
    QString mErrorMessage;
    bool mHasInternetConnection = true;

    Core::Widget *mOutput;
    Core::UserMessageWidget *mOutputMessage;

    QString mOutputOntologicalTermsTemplate;
    WebViewerWidget::WebViewerWidget *mOutputOntologicalTerms;

    InformationType mInformationType = InformationType::None;

    bool mLookUpInformation = false;

    QMap<QString, CellmlAnnotationViewMetadataEditDetailsItem> mItemsMapping;
    QMap<QString, bool> mEnabledItems;

    CellMLSupport::CellmlFile *mCellmlFile;

    ObjRef<iface::cellml_api::CellMLElement> mElement = nullptr;

    QMap<QString, QString> mUrls;
    QStringList mItemInformationSha1s;
    QString mItemInformationSha1;

    QString mLink;
    QString mTextContent;

    QMenu *mContextMenu;

    QAction *mCopyAction;

    QNetworkReply *mNetworkReply = nullptr;

    void upudateOutputMessage(bool pLookUpTerm,
                              const QString &pErrorMessage,
                              bool pHasInternetConnection,
                              bool *pShowBusyWidget = nullptr);
    void updateOutputHeaders();

    void updateItemsGui(const CellmlAnnotationViewMetadataEditDetailsItems &pItems,
                        bool pLookUpTerm, const QString &pErrorMessage = {},
                        bool pHasInternetConnection = true);

    void genericLookUp(const QString &pItemInformation = {},
                       InformationType pInformationType = InformationType::None);

    bool isDirectTerm(const QString &pTerm) const;

signals:
    void qualifierLookUpRequested(const QString &pQualifier);
    void resourceLookUpRequested(const QString &pResource);
    void idLookUpRequested(const QString &pId);
    void noLookUpRequested();

public slots:
    void updateGui(iface::cellml_api::CellMLElement *pElement,
                   bool pResetItemsGui = false,
                   bool pFilePermissionsChanged = false);

    void disableLookUpInformation();

private slots:
    void copy();

    void qualifierChanged(const QString &pQualifier);

    void lookUpQualifier();

    void linkClicked();
    void linkHovered();

    void lookUpTerm();

    void termChanged(const QString &pTerm);
    void termLookedUp(QNetworkReply *pNetworkReply = nullptr);
    void sslErrors(QNetworkReply *pNetworkReply,
                   const QList<QSslError> &pSslErrors);

    void addTerm();

    void showCustomContextMenu();
};

//==============================================================================

} // namespace CellMLAnnotationView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
