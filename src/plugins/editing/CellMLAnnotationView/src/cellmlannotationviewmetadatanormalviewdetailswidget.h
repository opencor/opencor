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
// CellML annotation view metadata normal view details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWMETADATANORMALVIEWDETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWMETADATANORMALVIEWDETAILSWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "widget.h"

//==============================================================================

#include <QMap>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewMetadataNormalViewDetailsWidget;
}

//==============================================================================

class QLabel;
class QMenu;
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

class CellmlAnnotationViewMetadataNormalViewDetailsWidget : public Core::Widget
{
    Q_OBJECT

private:
    enum Information {
        No,
        First,
        Any,
        Last
    };

    enum InformationType {
        None,
        Qualifier,
        Resource,
        Id
    };

public:
    explicit CellmlAnnotationViewMetadataNormalViewDetailsWidget(CellmlAnnotationViewEditingWidget *pParent);
    ~CellmlAnnotationViewMetadataNormalViewDetailsWidget();

    virtual void retranslateUi();

    void updateGui(iface::cellml_api::CellMLElement *pElement,
                   const QString &pRdfTripleInformation = QString(),
                   const InformationType &pInformationType = None,
                   const Information &pLookUpRdfTripleInformation = First);

    void addRdfTriple(CellMLSupport::CellmlFileRdfTriple *pRdfTriple,
                      const bool &pNeedAdditionalGuiUpdates = true);

    void filePermissionsChanged();

private:
    CellMLSupport::CellmlFile *mCellmlFile;

    Ui::CellmlAnnotationViewMetadataNormalViewDetailsWidget *mGui;

    int mItemsCount;

    Core::Widget *mOutput;

    QScrollArea *mOutputMessageScrollArea;
    Core::UserMessageWidget *mOutputMessage;

    QString mOutputOntologicalTermsTemplate;
    CellmlAnnotationViewMetadataWebViewWidget *mOutputOntologicalTerms;

    ObjRef<iface::cellml_api::CellMLElement> mElement;

    QString mRdfTripleInformation;
    InformationType mInformationType;

    Information mLookUpRdfTripleInformation;

    QMap<QString, CellMLSupport::CellmlFileRdfTriple *> mRdfTriplesMapping;

    QMap<QString, QString> mUrls;
    QStringList mRdfTripleInformationSha1s;
    QString mRdfTripleInformationSha1;

    QString mFirstRdfTripleInformation;
    QString mLastRdfTripleInformation;

    QString mLink;
    QString mTextContent;

    QMenu *mContextMenu;

    void additionalGuiUpdates(const QString &pRdfTripleInformation,
                              const InformationType &pInformationType,
                              const Information &pLookUpRdfTripleInformation);

    void updateOutputHeaders();

    void genericLookUp(const QString &pRdfTripleInformation = QString(),
                       const InformationType &pInformationType = None);

Q_SIGNALS:
    void qualifierLookUpRequested(const QString &pQualifier);
    void resourceLookUpRequested(const QString &pResource);
    void idLookUpRequested(const QString &pResource, const QString &pId);
    void noLookUpRequested();

    void rdfTripleRemoved(CellMLSupport::CellmlFileRdfTriple *pRdfTriple);

private Q_SLOTS:
    void on_actionCopy_triggered();

    void disableLookUpInformation();

    void showLastRdfTriple();

    void linkClicked();

    void showCustomContextMenu(const QPoint &pPosition);
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
