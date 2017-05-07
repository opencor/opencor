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
// CellML Annotation view metadata normal view details widget
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfile.h"
#include "widget.h"

//==============================================================================

#include <QMap>

//==============================================================================

class QLabel;
class QMenu;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class UserMessageWidget;
}   // namespace Core

//==============================================================================

namespace WebViewerWidget {
    class WebViewerWidget;
}   // namespace WebViewerWidget

//==============================================================================

namespace CellMLAnnotationView {

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
    explicit CellmlAnnotationViewMetadataNormalViewDetailsWidget(CellMLSupport::CellmlFile *pCellmlFile,
                                                                 QWidget *pParent);

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

    int mItemsCount;

    Core::Widget *mOutput;
    Core::UserMessageWidget *mOutputMessage;

    QString mOutputOntologicalTermsTemplate;
    WebViewerWidget::WebViewerWidget *mOutputOntologicalTerms;

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

    QAction *mCopyAction;

    void additionalGuiUpdates(const QString &pRdfTripleInformation,
                              const InformationType &pInformationType,
                              const Information &pLookUpRdfTripleInformation);

    void updateOutputHeaders();

    void genericLookUp(const QString &pRdfTripleInformation = QString(),
                       const InformationType &pInformationType = None);

signals:
    void qualifierLookUpRequested(const QString &pQualifier);
    void resourceLookUpRequested(const QString &pResource);
    void idLookUpRequested(const QString &pResource, const QString &pId);
    void noLookUpRequested();

    void rdfTripleRemoved(CellMLSupport::CellmlFileRdfTriple *pRdfTriple);

private slots:
    void copy();

    void disableLookUpInformation();

    void showLastRdfTriple();

    void linkClicked();
    void linkHovered();

    void showCustomContextMenu();
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
