/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// CellML annotation view metadata edit details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWMETADATAEDITDETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWMETADATAEDITDETAILSWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "widget.h"

//==============================================================================

#include <QMap>
#include <QModelIndex>
#include <QStandardItem>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewMetadataEditDetailsWidget;
}

//==============================================================================

class QComboBox;
class QLabel;
class QLineEdit;
class QMenu;
class QNetworkAccessManager;
class QNetworkReply;
class QPushButton;
class QScrollArea;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class UserMessageWidget;
}   // namespace Core

//==============================================================================

namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewEditingWidget;
class CellmlAnnotationViewMetadataWebViewWidget;

//==============================================================================

class CellmlAnnotationViewMetadataEditDetailsWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewMetadataEditDetailsWidget(CellmlAnnotationViewEditingWidget *pParent);
    ~CellmlAnnotationViewMetadataEditDetailsWidget();

    virtual void retranslateUi();

    void filePermissionsChanged();

private:
    enum InformationType {
        None,
        Qualifier,
        Resource,
        Id
    };

    struct Item
    {
        QString name;
        QString resource;
        QString id;

        bool operator==(const Item &pItem) const;
        bool operator<(const Item &pItem) const;
    };

    typedef QList<Item> Items;

    CellmlAnnotationViewEditingWidget *mParent;

    Ui::CellmlAnnotationViewMetadataEditDetailsWidget *mGui;

    QNetworkAccessManager *mNetworkAccessManager;

    QLabel *mQualifierLabel;
    QComboBox *mQualifierValue;
    QPushButton *mLookUpQualifierButton;

    QLabel *mTermLabel;
    QLineEdit *mTermValue;
    QPushButton *mAddTermButton;

    QString mTerm;
    QStringList mTerms;

    int mItemsCount;

    QString mErrorMessage;
    bool mLookUpTerm;

    Core::Widget *mOutput;

    QScrollArea *mOutputMessageScrollArea;
    Core::UserMessageWidget *mOutputMessage;

    QString mOutputOntologicalTermsTemplate;
    CellmlAnnotationViewMetadataWebViewWidget *mOutputOntologicalTerms;

    InformationType mInformationType;

    bool mLookUpInformation;

    QMap<QString, Item> mItemsMapping;
    QMap<QString, bool> mEnabledItems;

    CellMLSupport::CellmlFile *mCellmlFile;

    ObjRef<iface::cellml_api::CellMLElement> mElement;

    QMap<QString, QString> mUrls;
    QStringList mItemInformationSha1s;
    QString mItemInformationSha1;

    QString mLink;
    QString mTextContent;

    QMenu *mContextMenu;

    QNetworkReply *mNetworkReply;

    void upudateOutputMessage(const bool &pLookUpTerm,
                              const QString &pErrorMessage,
                              bool *pShowBusyWidget = 0);
    void updateOutputHeaders();

    void updateItemsGui(const Items &pItems, const bool &pLookUpTerm,
                        const QString &pErrorMessage);

    static Item item(const QString &pName,
                     const QString &pResource, const QString &pId);

    void genericLookUp(const QString &pItemInformation = QString(),
                       const InformationType &pInformationType = None);

    bool isDirectTerm(const QString &pTerm) const;

Q_SIGNALS:
    void qualifierLookUpRequested(const QString &pQualifier);
    void resourceLookUpRequested(const QString &pResource);
    void idLookUpRequested(const QString &pResource, const QString &pId);
    void noLookUpRequested();

public Q_SLOTS:
    void updateGui(iface::cellml_api::CellMLElement *pElement,
                   const bool &pResetItemsGui = false);

private Q_SLOTS:
    void on_actionCopy_triggered();

    void disableLookUpInformation();

    void qualifierChanged(const QString &pQualifier);

    void lookUpQualifier();

    void linkClicked();

    void lookUpTerm();

    void termChanged(const QString &pTerm);
    void termLookedUp(QNetworkReply *pNetworkReply);

    void addTerm();

    void showCustomContextMenu(const QPoint &pPosition);

    void recenterBusyWidget();
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
