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
// CellML annotation view metadata details widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWMETADATADETAILSWIDGET_H
#define CELLMLANNOTATIONVIEWMETADATADETAILSWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "widget.h"

//==============================================================================

class QSplitter;
class QWebView;

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewMetadataDetailsWidget;
}

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class BorderedWidget;
    class UserMessageWidget;
}   // namespace Core

//==============================================================================

namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewMetadataEditDetailsWidget;
class CellmlAnnotationViewMetadataViewDetailsWidget;
class CellmlAnnotationViewWidget;

//==============================================================================

class CellmlAnnotationViewMetadataDetailsWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewMetadataDetailsWidget(CellmlAnnotationViewWidget *pParent);
    ~CellmlAnnotationViewMetadataDetailsWidget();

    virtual void retranslateUi();

    void addRdfTriple(CellMLSupport::CellmlFileRdfTriple *pRdfTriple);

    QSplitter * splitter() const;

    CellmlAnnotationViewMetadataEditDetailsWidget * metadataEditDetails() const;
    CellmlAnnotationViewMetadataViewDetailsWidget * metadataViewDetails() const;

    void reload();

private:
    CellmlAnnotationViewWidget *mParent;

    Ui::CellmlAnnotationViewMetadataDetailsWidget *mGui;

    Core::BorderedWidget *mBorderedCategoryMessage;
    Core::UserMessageWidget *mCategoryMessage;

    Core::BorderedWidget *mBorderedUnsupportedMetadataMessage;
    Core::UserMessageWidget *mUnsupportedMetadataMessage;

    QSplitter *mSplitter;

    Core::BorderedWidget *mBorderedMetadataEditDetails;
    Core::BorderedWidget *mBorderedMetadataViewDetails;
    Core::BorderedWidget *mBorderedWebView;

    CellmlAnnotationViewMetadataEditDetailsWidget *mMetadataEditDetails;
    CellmlAnnotationViewMetadataViewDetailsWidget *mMetadataViewDetails;
    QWebView *mWebView;

    CellMLSupport::CellmlFile *mCellmlFile;

    ObjRef<iface::cellml_api::CellMLElement> mElement;

Q_SIGNALS:
    void splitterMoved(const QList<int> &pSizes);

public Q_SLOTS:
    void updateGui(iface::cellml_api::CellMLElement *pElement);

private Q_SLOTS:
    void updateSizes(const QList<int> &pSizes);
    void emitSplitterMoved();

    void lookupQualifier(const QString &pQualifier, const bool &pRetranslate);
    void lookupResource(const QString &pResource, const bool &pRetranslate);
    void lookupId(const QString &pResource, const QString &pId,
                  const bool &pRetranslate);
    void lookupNothing();

    void removeAllMetadata();

    void updateMetadataEditDetails();
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
