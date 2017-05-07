/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// CellML Annotation view metadata details widget
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfile.h"
#include "corecliutils.h"
#include "widget.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class BorderedWidget;
    class SplitterWidget;
    class UserMessageWidget;
}   // namespace Core

//==============================================================================

namespace WebViewerWidget {
    class WebViewerWidget;
}   // namespace WebViewerWidget

//==============================================================================

namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewEditingWidget;
class CellmlAnnotationViewMetadataEditDetailsWidget;
class CellmlAnnotationViewMetadataViewDetailsWidget;
class CellMLAnnotationViewPlugin;
class CellmlAnnotationViewWidget;

//==============================================================================

class CellmlAnnotationViewMetadataDetailsWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewMetadataDetailsWidget(CellMLAnnotationViewPlugin *pPlugin,
                                                       CellmlAnnotationViewWidget *pViewWidget,
                                                       CellmlAnnotationViewEditingWidget *pViewEditingWidget,
                                                       CellMLSupport::CellmlFile *pCellmlFile,
                                                       QWidget *pParent);

    virtual void retranslateUi();

    Core::SplitterWidget * splitter() const;

    CellmlAnnotationViewMetadataEditDetailsWidget * metadataEditDetails() const;
    CellmlAnnotationViewMetadataViewDetailsWidget * metadataViewDetails() const;

    void filePermissionsChanged();

private:
    CellMLAnnotationViewPlugin *mPlugin;

    Core::BorderedWidget *mBorderedCategoryMessage;
    Core::UserMessageWidget *mCategoryMessage;

    Core::BorderedWidget *mBorderedUnsupportedMetadataMessage;
    Core::UserMessageWidget *mUnsupportedMetadataMessage;

    Core::SplitterWidget *mSplitter;

    Core::BorderedWidget *mBorderedMetadataEditDetails;
    Core::BorderedWidget *mBorderedMetadataViewDetails;
    Core::BorderedWidget *mBorderedWebViewer;

    CellmlAnnotationViewMetadataEditDetailsWidget *mMetadataEditDetails;
    CellmlAnnotationViewMetadataViewDetailsWidget *mMetadataViewDetails;
    WebViewerWidget::WebViewerWidget *mWebViewer;

    CellMLSupport::CellmlFile *mCellmlFile;

    ObjRef<iface::cellml_api::CellMLElement> mElement;

    void retranslateUnsupportedMetadataMessage();

signals:
    void splitterMoved(const QIntList &pSizes);


    void qualifierDetailsRequested(WebViewerWidget::WebViewerWidget *pWebViewer,
                                   const QString &pQualifier);
    void resourceDetailsRequested(WebViewerWidget::WebViewerWidget *pWebViewer,
                                  const QString &pResource);
    void idDetailsRequested(WebViewerWidget::WebViewerWidget *pWebViewer,
                            const QString &pResource, const QString &pId);

public slots:
    void updateGui(iface::cellml_api::CellMLElement *pElement);

private slots:
    void emitSplitterMoved();

    void lookUpQualifier(const QString &pQualifier);
    void lookUpResource(const QString &pResource);
    void lookUpId(const QString &pResource, const QString &pId);
    void lookUpNothing();

    void removeAllMetadata();

    void updateMetadataEditDetails();
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
