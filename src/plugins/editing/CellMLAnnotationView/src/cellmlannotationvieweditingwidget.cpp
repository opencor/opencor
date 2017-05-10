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

#include "borderedwidget.h"
#include "cellmlannotationviewcellmllistwidget.h"
#include "cellmlannotationvieweditingwidget.h"
#include "cellmlannotationviewmetadatanormalviewdetailswidget.h"
#include "cellmlannotationviewmetadatadetailswidget.h"
#include "cellmlannotationviewmetadataeditdetailswidget.h"
#include "cellmlannotationviewplugin.h"
#include "cellmlannotationviewwidget.h"
#include "cellmlfilemanager.h"
#include "corecliutils.h"
#include "treeviewwidget.h"
#include "webviewerwidget.h"

//==============================================================================

#include <QWebView>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewEditingWidget::CellmlAnnotationViewEditingWidget(CellMLAnnotationViewPlugin *pPlugin,
                                                                     const QString &pFileName,
                                                                     CellmlAnnotationViewWidget *pViewWidget,
                                                                     QWidget *pParent) :
    Core::SplitterWidget(pParent)
{
    // Retrieve some SVG diagrams

    Core::readFileContentsFromFile(":/CellMLAnnotationView/modelQualifier.svg", mModelQualifierSvg);
    Core::readFileContentsFromFile(":/CellMLAnnotationView/biologyQualifier.svg", mBiologyQualifierSvg);

    // Retrieve our output template

    Core::readFileContentsFromFile(":/CellMLAnnotationView/qualifierInformation.html", mQualifierInformationTemplate);

    // Retrieve and load, in case it's necessary, the requested CellML file

    mCellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);

    mCellmlFile->load();

    // Customise our GUI which consists of two main parts:
    //
    //  1) A couple of lists (for CellML elements and metadata, resp.); and
    //  2) Some details (for a CellML element or metadata).
    //
    // These two main parts are widgets of ourselves and moving the splitter
    // will result in the splitter of other CellML files' view to be moved too

    // Create our two main parts

    mCellmlList = new CellmlAnnotationViewCellmlListWidget(this);
    mMetadataDetails = new CellmlAnnotationViewMetadataDetailsWidget(pPlugin, pViewWidget, this, mCellmlFile, this);

    // Populate ourselves

    addWidget(new Core::BorderedWidget(mCellmlList,
                                       false, false, false, true));
    addWidget(mMetadataDetails);

    // Keep track of our splitter being moved

    connect(this, SIGNAL(splitterMoved(int, int)),
            this, SLOT(emitSplitterMoved()));

    // A connection to let our details widget know that we want to see the
    // metadata details of some CellML element

    connect(mCellmlList, SIGNAL(metadataDetailsRequested(iface::cellml_api::CellMLElement *)),
            mMetadataDetails, SLOT(updateGui(iface::cellml_api::CellMLElement *)));

    // Some connections to keep track of what our details widget wants

    connect(mMetadataDetails, SIGNAL(qualifierDetailsRequested(WebViewerWidget::WebViewerWidget *, const QString &)),
            this, SLOT(updateWebViewerWithQualifierDetails(WebViewerWidget::WebViewerWidget *, const QString &)));
    connect(mMetadataDetails, SIGNAL(resourceDetailsRequested(WebViewerWidget::WebViewerWidget *, const QString &)),
            this, SLOT(updateWebViewerWithResourceDetails(WebViewerWidget::WebViewerWidget *, const QString &)));
    connect(mMetadataDetails, SIGNAL(idDetailsRequested(WebViewerWidget::WebViewerWidget *, const QString &, const QString &)),
            this, SLOT(updateWebViewerWithIdDetails(WebViewerWidget::WebViewerWidget *, const QString &, const QString &)));

    // Make our CellML list widget our focus proxy

    setFocusProxy(mCellmlList);

    // Select the first item from our CellML list widget
    // Note: we need to do this after having set up the connections above since
    //       we want our metadata details widget to get updated when the first
    //       item from our CellML list widget gets selected...

    mCellmlList->treeViewWidget()->selectFirstItem();
}

//==============================================================================

void CellmlAnnotationViewEditingWidget::retranslateUi()
{
    // Retranslate our lists and details widgets

    mCellmlList->retranslateUi();
    mMetadataDetails->retranslateUi();
}

//==============================================================================

CellMLSupport::CellmlFile * CellmlAnnotationViewEditingWidget::cellmlFile() const
{
    // Return our CellML file

    return mCellmlFile;
}

//==============================================================================

void CellmlAnnotationViewEditingWidget::emitSplitterMoved()
{
    // Let people know that our splitter has been moved

    emit splitterMoved(sizes());
}

//==============================================================================

CellmlAnnotationViewCellmlListWidget * CellmlAnnotationViewEditingWidget::cellmlList() const
{
    // Return our CellML list widget

    return mCellmlList;
}

//==============================================================================

CellmlAnnotationViewMetadataDetailsWidget * CellmlAnnotationViewEditingWidget::metadataDetails() const
{
    // Return our metadata details widget

    return mMetadataDetails;
}

//==============================================================================

void CellmlAnnotationViewEditingWidget::updateWebViewerWithQualifierDetails(WebViewerWidget::WebViewerWidget *pWebViewer,
                                                                            const QString &pQualifier)
{
    // The user requested a qualifier to be looked up, so generate a web page
    // containing some information about the qualifier
    // Note: ideally, there would be a way to refer to a particular qualifier
    //       using http://biomodels.net/qualifiers/, but that would require
    //       anchors and they don't have any, so instead we use the information
    //       which can be found on that site and present it to the user in the
    //       form of a web page...

    if (pQualifier.isEmpty())
        return;

    // Generate the web page containing some information about the qualifier

    QString qualifierSvg;
    QString shortDescription;
    QString longDescription;

    if (!pQualifier.compare("model:is")) {
        qualifierSvg = mModelQualifierSvg;

        shortDescription = tr("Identity");
        longDescription = tr("The modelling object represented by the model element is identical with the subject of the referenced resource (\"Modelling Object B\"). For instance, this qualifier might be used to link an encoded model to a database of models.");
    } else if (!pQualifier.compare("model:isDerivedFrom")) {
        qualifierSvg = mModelQualifierSvg;

        shortDescription = tr("Origin");
        longDescription = tr("The modelling object represented by the model element is derived from the modelling object represented by the referenced resource (\"Modelling Object B\"). This relation may be used, for instance, to express a refinement or adaptation in usage for a previously described modelling component.");
    } else if (!pQualifier.compare("model:isDescribedBy")) {
        qualifierSvg = mModelQualifierSvg;

        shortDescription = tr("Description");
        longDescription = tr("The modelling object represented by the model element is described by the subject of the referenced resource (\"Modelling Object B\"). This relation might be used to link a model or a kinetic law to the literature that describes it.");
    } else if (!pQualifier.compare("model:isInstanceOf")) {
        qualifierSvg = mModelQualifierSvg;

        shortDescription = tr("Class");
        longDescription = tr("The modelling object represented by the model element is an instance of the subject of the referenced resource (\"Modelling Object B\"). For instance, this qualifier might be used to link a specific model with its generic form.");
    } else if (!pQualifier.compare("model:hasInstance")) {
        qualifierSvg = mModelQualifierSvg;

        shortDescription = tr("Instance");
        longDescription = tr("The modelling object represented by the model element has for instance (is a class of) the subject of the referenced resource (\"Modelling Object B\"). For instance, this qualifier might be used to link a generic model with its specific forms.");
    } else if (!pQualifier.compare("bio:encodes")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Encodement");
        longDescription = tr("The biological entity represented by the model element encodes, directly or transitively, the subject of the referenced resource (\"Biological Entity B\"). This relation may be used to express, for example, that a specific DNA sequence encodes a particular protein.");
    } else if (!pQualifier.compare("bio:hasPart")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Part");
        longDescription = tr("The biological entity represented by the model element includes the subject of the referenced resource (\"Biological Entity B\"), either physically or logically. This relation might be used to link a complex to the description of its components.");
    } else if (!pQualifier.compare("bio:hasProperty")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Property");
        longDescription = tr("The subject of the referenced resource (\"Biological Entity B\") is a property of the biological entity represented by the model element. This relation might be used when a biological entity exhibits a certain enzymatic activity or exerts a specific function.");
    } else if (!pQualifier.compare("bio:hasVersion")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Version");
        longDescription = tr("The subject of the referenced resource (\"Biological Entity B\") is a version or an instance of the biological entity represented by the model element. This relation may be used to represent an isoform or modified form of a biological entity.");
    } else if (!pQualifier.compare("bio:is")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Indentity");
        longDescription = tr("The biological entity represented by the model element has identity with the subject of the referenced resource (\"Biological Entity B\"). This relation might be used to link a reaction to its exact counterpart in a database, for instance.");
    } else if (!pQualifier.compare("bio:isDescribedBy")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Description");
        longDescription = tr("The biological entity represented by the model element is described by the subject of the referenced resource (\"Biological Entity B\"). This relation should be used, for instance, to link a species or a parameter to the literature that describes the concentration of that species or the value of that parameter.");
    } else if (!pQualifier.compare("bio:isEncodedBy")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Encoder");
        longDescription = tr("The biological entity represented by the model element is encoded, directly or transitively, by the subject of the referenced resource (\"Biological Entity B\"). This relation may be used to express, for example, that a protein is encoded by a specific DNA sequence.");
    } else if (!pQualifier.compare("bio:isHomologTo")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Homolog");
        longDescription = tr("The biological entity represented by the model element is homologous to the subject of the referenced resource (\"Biological Entity B\"). This relation can be used to represent biological entities that share a common ancestor.");
    } else if (!pQualifier.compare("bio:isPartOf")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Parthood");
        longDescription = tr("The biological entity represented by the model element is a physical or logical part of the subject of the referenced resource (\"Biological Entity B\"). This relation may be used to link a model component to a description of the complex in which it is a part.");
    } else if (!pQualifier.compare("bio:isPropertyOf")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Property bearer");
        longDescription = tr("The biological entity represented by the model element is a property of the referenced resource (\"Biological Entity B\").");
    } else if (!pQualifier.compare("bio:isVersionOf")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Hypernym");
        longDescription = tr("The biological entity represented by the model element is a version or an instance of the subject of the referenced resource (\"Biological Entity B\"). This relation may be used to represent, for example, the 'superclass' or 'parent' form of a particular biological entity.");
    } else if (!pQualifier.compare("bio:occursIn")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Container");
        longDescription = tr("The biological entity represented by the model element is physically limited to a location, which is the subject of the referenced resource (\"Biological Entity B\"). This relation may be used to ascribe a compartmental location, within which a reaction takes place.");
    } else if (!pQualifier.compare("bio:hasTaxon")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Taxon");
        longDescription = tr("The biological entity represented by the model element is taxonomically restricted, where the restriction is the subject of the referenced resource (\"Biological Entity B\"). This relation may be used to ascribe a species restriction to a biochemical reaction.");
    } else {
        qualifierSvg = QString();

        shortDescription = tr("Unknown");
        longDescription = tr("Unknown");
    }

    // Show the information

    pWebViewer->webView()->setHtml(mQualifierInformationTemplate.arg(pQualifier, qualifierSvg, shortDescription, longDescription));
}

//==============================================================================

void CellmlAnnotationViewEditingWidget::updateWebViewerWithResourceDetails(WebViewerWidget::WebViewerWidget *pWebViewer,
                                                                           const QString &pResource)
{
    // The user requested a resource to be looked up, so retrieve it using
    // identifiers.org

    pWebViewer->webView()->load(CellmlAnnotationViewWidget::resourceUrl(pResource));
}

//==============================================================================

void CellmlAnnotationViewEditingWidget::updateWebViewerWithIdDetails(WebViewerWidget::WebViewerWidget *pWebViewer,
                                                                     const QString &pResource,
                                                                     const QString &pId)
{
    // The user requested a resource id to be looked up, so retrieve it using
    // identifiers.org

    pWebViewer->webView()->load(CellmlAnnotationViewWidget::idUrl(pResource, pId));
}

//==============================================================================

void CellmlAnnotationViewEditingWidget::filePermissionsChanged()
{
    // Let our metadata details widget know that the file has been un/locked
    // Note: we don't need to let our CellML list widget know about it since
    //       anything that is related to file permissions is done on the fly (to
    //       show a context menu)...

    mMetadataDetails->filePermissionsChanged();
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
