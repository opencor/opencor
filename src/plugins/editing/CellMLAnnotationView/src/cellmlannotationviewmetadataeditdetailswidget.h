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
#include "commonwidget.h"

//==============================================================================

#include <QMap>
#include <QScrollArea>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewMetadataEditDetailsWidget;
}

//==============================================================================

class QComboBox;
class QFormLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QMenu;
class QNetworkAccessManager;
class QNetworkReply;
class QPushButton;
class QStackedWidget;
class QVBoxLayout;

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewEditingWidget;

//==============================================================================

class CellmlAnnotationViewMetadataEditDetailsWidget : public QScrollArea,
                                                      public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewMetadataEditDetailsWidget(CellmlAnnotationViewEditingWidget *pParent);
    ~CellmlAnnotationViewMetadataEditDetailsWidget();

    virtual void retranslateUi();

    void fileReloaded();

private:
    enum Type {
        No,
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

    QStackedWidget *mWidget;

    QWidget *mMainWidget;
    QVBoxLayout *mMainLayout;

    QWidget *mFormWidget;
    QFormLayout *mFormLayout;

    QScrollArea *mItemsScrollArea;

    QGridLayout *mGridLayout;

    QNetworkAccessManager *mNetworkAccessManager;

    QComboBox *mQualifierValue;
    QPushButton *mLookupQualifierButton;

    int mQualifierIndex;
    bool mLookupQualifierButtonIsChecked;

    QLineEdit *mTermValue;
    QPushButton *mAddTermButton;

    QString mTerm;
    QStringList mTerms;
    bool mTermIsDirect;

    Items mItems;
    QString mErrorMessage;
    bool mLookupTerm;

    QString mInformation;
    Type mType;

    bool mLookupInformation;

    QMap<QObject *, Item> mItemsMapping;

    int mItemsVerticalScrollBarPosition;

    CellMLSupport::CellmlFile *mCellmlFile;

    ObjRef<iface::cellml_api::CellMLElement> mElement;

    QLabel *mCurrentResourceOrIdLabel;

    QMenu *mContextMenu;

    QNetworkReply *mNetworkReply;

    void updateGui(const Items &pItems, const QString &pErrorMessage,
                   const bool &pLookupTerm,
                   const int &pItemsVerticalScrollBarPosition,
                   const bool &pRetranslate);
    void updateItemsGui(const Items &pItems, const QString &pErrorMessage,
                        const bool &pLookupTerm);

    static Item item(const QString &pName,
                     const QString &pResource, const QString &pId);

    void genericLookup(const QString &pItemInformation = QString(),
                       const Type &pType = No,
                       const bool &pRetranslate = false);

Q_SIGNALS:
    void qualifierLookupRequested(const QString &pQualifier,
                                  const bool &pRetranslate);
    void resourceLookupRequested(const QString &pResource,
                                 const bool &pRetranslate);
    void idLookupRequested(const QString &pResource, const QString &pId,
                           const bool &pRetranslate);
    void noLookupRequested();

    void rdfTripleAdded(CellMLSupport::CellmlFileRdfTriple *pRdfTriple);

public Q_SLOTS:
    void updateGui(iface::cellml_api::CellMLElement *pElement,
                   const bool &pResetItemsGui = false);

private Q_SLOTS:
    void on_actionCopy_triggered();

    void disableLookupInformation();

    void qualifierChanged(const int &pQualifierIndex);
    void qualifierChanged(const QString &pQualifier);

    void lookupQualifier();
    void lookupResource(const QString &pItemInformation);
    void lookupId(const QString &pItemInformation);

    void lookupTerm();

    void termChanged(const QString &pTerm);
    void termLookedUp(QNetworkReply *pNetworkReply);

    void addTerm();
    void addRetrievedTerm();

    void trackItemsVerticalScrollBarPosition(const int &pPosition);

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
