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
// CellML Annotation view editing widget
//==============================================================================

#pragma once

//==============================================================================

#include "corecliutils.h"
#include "splitterwidget.h"

//==============================================================================

#include <QMap>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFile;
}   // namespace CellMLSupport

//==============================================================================

namespace WebViewerWidget {
    class WebViewerWidget;
}   // namespace WebViewerWidget

//==============================================================================

namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewCellmlListWidget;
class CellmlAnnotationViewMetadataDetailsWidget;
class CellMLAnnotationViewPlugin;
class CellmlAnnotationViewWidget;

//==============================================================================

class CellmlAnnotationViewEditingWidget : public Core::SplitterWidget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewEditingWidget(CellMLAnnotationViewPlugin *pPlugin,
                                               const QString &pFileName,
                                               CellmlAnnotationViewWidget *pViewWidget,
                                               QWidget *pParent);

    virtual void retranslateUi();

    CellMLSupport::CellmlFile * cellmlFile() const;

    CellmlAnnotationViewCellmlListWidget * cellmlList() const;
    CellmlAnnotationViewMetadataDetailsWidget * metadataDetails() const;

    void filePermissionsChanged();

private:
    CellMLSupport::CellmlFile *mCellmlFile;

    CellmlAnnotationViewCellmlListWidget *mCellmlList;
    CellmlAnnotationViewMetadataDetailsWidget *mMetadataDetails;

    QString mModelQualifierSvg;
    QString mBiologyQualifierSvg;

    QString mQualifierInformationTemplate;

signals:
    void splitterMoved(const QIntList &pSizes);

private slots:
    void emitSplitterMoved();

    void updateWebViewerWithQualifierDetails(WebViewerWidget::WebViewerWidget *pWebViewer,
                                             const QString &pQualifier);
    void updateWebViewerWithResourceDetails(WebViewerWidget::WebViewerWidget *pWebViewer,
                                            const QString &pResource);
    void updateWebViewerWithIdDetails(WebViewerWidget::WebViewerWidget *pWebViewer,
                                      const QString &pResource,
                                      const QString &pId);
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
