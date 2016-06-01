/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// CellML annotation view editing widget
//==============================================================================

#pragma once

//==============================================================================

#include "commonwidget.h"
#include "corecliutils.h"

//==============================================================================

#include <QMap>
#include <QSplitter>

//==============================================================================

class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFile;
}   // namespace CellMLSupport

//==============================================================================

namespace WebViewer {
    class WebViewerWidget;
}   // namespace WebViewer

//==============================================================================

namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewCellmlListWidget;
class CellmlAnnotationViewMetadataDetailsWidget;
class CellMLAnnotationViewPlugin;
class CellmlAnnotationViewWidget;

//==============================================================================

class CellmlAnnotationViewEditingWidget : public QSplitter,
                                          public Core::CommonWidget
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

Q_SIGNALS:
    void splitterMoved(const QIntList &pSizes);

private Q_SLOTS:
    void emitSplitterMoved();

    void updateWebViewerWithQualifierDetails(WebViewer::WebViewerWidget *pWebViewer,
                                             const QString &pQualifier);
    void updateWebViewerWithResourceDetails(WebViewer::WebViewerWidget *pWebViewer,
                                            const QString &pResource);
    void updateWebViewerWithIdDetails(WebViewer::WebViewerWidget *pWebViewer,
                                      const QString &pResource,
                                      const QString &pId);
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
