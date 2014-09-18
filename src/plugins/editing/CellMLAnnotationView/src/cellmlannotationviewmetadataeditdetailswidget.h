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
#include <QScrollArea>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewMetadataEditDetailsWidget;
}

//==============================================================================

class QComboBox;
class QLabel;
class QLayout;
class QLineEdit;
class QMenu;
class QNetworkAccessManager;
class QNetworkReply;
class QPushButton;
class QVBoxLayout;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class Widget;
}   // namespace Core

//==============================================================================

namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewEditingWidget;

//==============================================================================

class CellmlAnnotationViewMetadataEditDetailsWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewMetadataEditDetailsWidget(CellmlAnnotationViewEditingWidget *pParent);
    ~CellmlAnnotationViewMetadataEditDetailsWidget();

    virtual void retranslateUi();

    void fileReloaded();

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

    Core::Widget *mItemsWidget;

    QNetworkAccessManager *mNetworkAccessManager;

    QLabel *mQualifierLabel;
    QComboBox *mQualifierValue;
    QPushButton *mLookUpQualifierButton;

    QLabel *mTermLabel;
    QLineEdit *mTermValue;
    QPushButton *mAddTermButton;

    QString mTerm;
    QStringList mTerms;
    bool mTermIsDirect;

    Items mItems;
    QString mErrorMessage;
    bool mLookUpTerm;

    InformationType mInformationType;

    bool mLookUpInformation;

    QMap<QObject *, Item> mItemsMapping;

    int mItemsVerticalScrollBarPosition;

    CellMLSupport::CellmlFile *mCellmlFile;

    ObjRef<iface::cellml_api::CellMLElement> mElement;

    QLabel *mCurrentResourceOrIdLabel;

    QMenu *mContextMenu;

    QNetworkReply *mNetworkReply;

    void removeLayoutWidgets(QLayout *pLayout);

    void updateItemsGui(const Items &pItems, const QString &pErrorMessage,
                        const bool &pLookUpTerm);

    static Item item(const QString &pName,
                     const QString &pResource, const QString &pId);

    void genericLookUp(const QString &pItemInformation = QString(),
                       const InformationType &pInformationType = None,
                       const bool &pRetranslate = false);

Q_SIGNALS:
    void qualifierLookUpRequested(const QString &pQualifier,
                                  const bool &pRetranslate);
    void resourceLookUpRequested(const QString &pResource,
                                 const bool &pRetranslate);
    void idLookUpRequested(const QString &pResource, const QString &pId,
                           const bool &pRetranslate);
    void noLookUpRequested();

    void rdfTripleAdded(CellMLSupport::CellmlFileRdfTriple *pRdfTriple);

public Q_SLOTS:
    void updateGui(iface::cellml_api::CellMLElement *pElement,
                   const bool &pResetItemsGui = false);

private Q_SLOTS:
    void on_actionCopy_triggered();

    void disableLookUpInformation();

    void qualifierChanged(const QString &pQualifier);

    void lookUpQualifier();
    void lookUpResource(const QString &pItemInformation);
    void lookUpId(const QString &pItemInformation);

    void lookUpTerm();

    void termChanged(const QString &pTerm);
    void termLookedUp(QNetworkReply *pNetworkReply);

    void addTerm();
    void addRetrievedTerm();

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
