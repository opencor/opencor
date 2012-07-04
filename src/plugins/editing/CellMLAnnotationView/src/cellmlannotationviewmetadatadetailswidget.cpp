//==============================================================================
// CellML annotation view metadata details widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmlannotationviewlistswidget.h"
#include "cellmlannotationviewmetadatabiomodelsdotnetviewdetailswidget.h"
#include "cellmlannotationviewmetadatadetailswidget.h"
#include "cellmlannotationviewmetadatalistwidget.h"
#include "cellmlannotationviewmetadataviewdetailswidget.h"
#include "cellmlannotationviewplugin.h"
#include "cellmlannotationviewwidget.h"
#include "treeview.h"
#include "usermessagewidget.h"

//==============================================================================

#include "ui_cellmlannotationviewmetadatadetailswidget.h"

//==============================================================================

#include <QLabel>
#include <QWebView>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataDetailsWidget::CellmlAnnotationViewMetadataDetailsWidget(CellmlAnnotationViewWidget *pParent) :
    Widget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewMetadataDetailsWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our unsupported metadata message widget

    mUnsupportedMetadataMsg = new Core::UserMessageWidget(pParent,
                                                          ":/oxygen/actions/help-about.png");
    mBorderedUnsupportedMetadataMsg = new Core::BorderedWidget(mUnsupportedMetadataMsg,
                                                               false, true, true, false);

    mBorderedUnsupportedMetadataMsg->setVisible(false);
    // Note: we don't initially want to see it, so...

    // Create our splitter widget

    mSplitter = new QSplitter(Qt::Vertical, pParent);

    // Create our details widgets

    mMetadataViewDetails = new CellmlAnnotationViewMetadataViewDetailsWidget(pParent, true);
    mWebView             = new QWebView(pParent);

    // Some connections to handle the looking up of a qualifier, resource and
    // resource id

    connect(mMetadataViewDetails->bioModelsDotNetView(), SIGNAL(qualifierLookupRequested(const QString &, const bool &)),
            this, SLOT(qualifierLookupRequested(const QString &, const bool &)));
    connect(mMetadataViewDetails->bioModelsDotNetView(), SIGNAL(resourceLookupRequested(const QString &, const bool &)),
            this, SLOT(resourceLookupRequested(const QString &, const bool &)));
    connect(mMetadataViewDetails->bioModelsDotNetView(), SIGNAL(resourceIdLookupRequested(const QString &, const QString &, const bool &)),
            this, SLOT(resourceIdLookupRequested(const QString &, const QString &, const bool &)));

    // Populate our splitter widget

    mSplitter->addWidget(new Core::BorderedWidget(mMetadataViewDetails,
                                                  false, true, true, false));
    mSplitter->addWidget(new Core::BorderedWidget(mWebView,
                                                  true, true, false, false));

    // Add our unsupported metadata message widget and splitter widget to our
    // layout

    mGui->layout->addWidget(mBorderedUnsupportedMetadataMsg);
    mGui->layout->addWidget(mSplitter);

    // Some further initialisations which are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();
}

//==============================================================================

CellmlAnnotationViewMetadataDetailsWidget::~CellmlAnnotationViewMetadataDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    mMetadataViewDetails->retranslateUi();

    // Update our unsupported metadata message

    mUnsupportedMetadataMsg->setMessage(tr("Sorry, but the <strong>%1</strong> view does not support this type of metadata...").arg(mParent->pluginParent()->viewName()));
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples)
{
    static CellMLSupport::CellmlFileRdfTriples rdfTriples = CellMLSupport::CellmlFileRdfTriples(mParent->cellmlFile());

    if (pRdfTriples == rdfTriples)
        // We want to show the same RDF triples, so...

        return;

    // Keep track of the RDF triples

    rdfTriples = pRdfTriples;

    // Show/hide our unsupported metadata message depending on whether the type
    // of the RDF triples is known or not

    mBorderedUnsupportedMetadataMsg->setVisible(pRdfTriples.type() == CellMLSupport::CellmlFileRdfTriple::Unknown);

    // Update our Metadata view details GUI

    mMetadataViewDetails->updateGui(pRdfTriples);
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::qualifierLookupRequested(const QString &pQualifier,
                                                                         const bool &pRetranslate)
{
//    Q_UNUSED(pRetranslate);

//    // The user requested a qualifier to be looked up, so generate a web page
//    // containing some information about the qualifier
//    // Note: ideally, there would be a way to refer to a particular qualifier
//    //       using http://biomodels.net/qualifiers/, but that would require
//    //       anchors and they don't have any, so instead we use the information
//    //       which can be found on that site and present it to the user in the
//    //       form of a web page...

//    if (pQualifier.isEmpty())
//        return;

//    // Generate the web page containing some information about the qualifier

//    QString qualifierSvg;
//    QString shortDescription;
//    QString longDescription;

//    if (!pQualifier.compare("model:is")) {
//        qualifierSvg = mModelQualifierSvg;

//        shortDescription = tr("Identity");
//        longDescription  = tr("The modelling object represented by the model element is identical with the subject of the referenced resource (\"Modelling Object B\"). For instance, this qualifier might be used to link an encoded model to a database of models.");
//    } else if (!pQualifier.compare("model:isDerivedFrom")) {
//        qualifierSvg = mModelQualifierSvg;

//        shortDescription = tr("Origin");
//        longDescription  = tr("The modelling object represented by the model element is derived from the modelling object represented by the referenced resource (\"Modelling Object B\"). This relation may be used, for instance, to express a refinement or adaptation in usage for a previously described modelling component.");
//    } else if (!pQualifier.compare("model:isDescribedBy")) {
//        qualifierSvg = mModelQualifierSvg;

//        shortDescription = tr("Description");
//        longDescription  = tr("The modelling object represented by the model element is described by the subject of the referenced resource (\"Modelling Object B\"). This relation might be used to link a model or a kinetic law to the literature that describes it.");
//    } else if (!pQualifier.compare("bio:encodes")) {
//        qualifierSvg = mBiologyQualifierSvg;

//        shortDescription = tr("Encodement");
//        longDescription  = tr("The biological entity represented by the model element encodes, directly or transitively, the subject of the referenced resource (\"Biological Entity B\"). This relation may be used to express, for example, that a specific DNA sequence encodes a particular protein.");
//    } else if (!pQualifier.compare("bio:hasPart")) {
//        qualifierSvg = mBiologyQualifierSvg;

//        shortDescription = tr("Part");
//        longDescription  = tr("The biological entity represented by the model element includes the subject of the referenced resource (\"Biological Entity B\"), either physically or logically. This relation might be used to link a complex to the description of its components.");
//    } else if (!pQualifier.compare("bio:hasProperty")) {
//        qualifierSvg = mBiologyQualifierSvg;

//        shortDescription = tr("Property");
//        longDescription  = tr("The subject of the referenced resource (\"Biological Entity B\") is a property of the biological entity represented by the model element. This relation might be used when a biological entity exhibits a certain enzymatic activity or exerts a specific function.");
//    } else if (!pQualifier.compare("bio:hasVersion")) {
//        qualifierSvg = mBiologyQualifierSvg;

//        shortDescription = tr("Version");
//        longDescription  = tr("The subject of the referenced resource (\"Biological Entity B\") is a version or an instance of the biological entity represented by the model element. This relation may be used to represent an isoform or modified form of a biological entity.");
//    } else if (!pQualifier.compare("bio:is")) {
//        qualifierSvg = mBiologyQualifierSvg;

//        shortDescription = tr("Indentity");
//        longDescription  = tr("The biological entity represented by the model element has identity with the subject of the referenced resource (\"Biological Entity B\"). This relation might be used to link a reaction to its exact counterpart in a database, for instance.");
//    } else if (!pQualifier.compare("bio:isDescribedBy")) {
//        qualifierSvg = mBiologyQualifierSvg;

//        shortDescription = tr("Description");
//        longDescription  = tr("The biological entity represented by the model element is described by the subject of the referenced resource (\"Biological Entity B\"). This relation should be used, for instance, to link a species or a parameter to the literature that describes the concentration of that species or the value of that parameter.");
//    } else if (!pQualifier.compare("bio:isEncodedBy")) {
//        qualifierSvg = mBiologyQualifierSvg;

//        shortDescription = tr("Encoder");
//        longDescription  = tr("The biological entity represented by the model element is encoded, directly or transitively, by the subject of the referenced resource (\"Biological Entity B\"). This relation may be used to express, for example, that a protein is encoded by a specific DNA sequence.");
//    } else if (!pQualifier.compare("bio:isHomologTo")) {
//        qualifierSvg = mBiologyQualifierSvg;

//        shortDescription = tr("Homolog");
//        longDescription  = tr("The biological entity represented by the model element is homologous to the subject of the referenced resource (\"Biological Entity B\"). This relation can be used to represent biological entities that share a common ancestor.");
//    } else if (!pQualifier.compare("bio:isPartOf")) {
//        qualifierSvg = mBiologyQualifierSvg;

//        shortDescription = tr("Parthood");
//        longDescription  = tr("The biological entity represented by the model element is a physical or logical part of the subject of the referenced resource (\"Biological Entity B\"). This relation may be used to link a model component to a description of the complex in which it is a part.");
//    } else if (!pQualifier.compare("bio:isPropertyOf")) {
//        qualifierSvg = mBiologyQualifierSvg;

//        shortDescription = tr("Property bearer");
//        longDescription  = tr("The biological entity represented by the model element is a property of the referenced resource (\"Biological Entity B\").");
//    } else if (!pQualifier.compare("bio:isVersionOf")) {
//        qualifierSvg = mBiologyQualifierSvg;

//        shortDescription = tr("Hypernym");
//        longDescription  = tr("The biological entity represented by the model element is a version or an instance of the subject of the referenced resource (\"Biological Entity B\"). This relation may be used to represent, for example, the 'superclass' or 'parent' form of a particular biological entity.");
//    } else if (!pQualifier.compare("bio:occursIn")) {
//        qualifierSvg = mBiologyQualifierSvg;

//        shortDescription = tr("Container");
//        longDescription  = tr("The biological entity represented by the model element is physically limited to a location, which is the subject of the referenced resource (\"Biological Entity B\"). This relation may be used to ascribe a compartmental location, within which a reaction takes place.");
//    } else if (!pQualifier.compare("bio:hasTaxon")) {
//        qualifierSvg = mBiologyQualifierSvg;

//        shortDescription = tr("Taxon");
//        longDescription  = tr("The biological entity represented by the model element is taxonomically restricted, where the restriction is the subject of the referenced resource (\"Biological Entity B\"). This relation may be used to ascribe a species restriction to a biochemical reaction.");
//    } else {
//        qualifierSvg = "";

//        shortDescription = tr("Unknown");
//        longDescription  = tr("Unknown");
//    }

//    // Show the information

//    mWebView->setHtml(mQualifierInformationTemplate.arg(pQualifier,
//                                                        qualifierSvg,
//                                                        shortDescription,
//                                                        longDescription,
//                                                        Core::copyright()));
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::resourceLookupRequested(const QString &pResource,
                                                                        const bool &pRetranslate)
{
    // The user requested a resource to be looked up, so retrieve it using
    // identifiers.org, but only if we are not retranslating since the looking
    // up would already be correct

    if (!pRetranslate)
        mWebView->setUrl("http://identifiers.org/"+pResource+"/?redirect=true");
        //---GRY--- NOTE THAT redirect=true DOESN'T WORK AT THE MOMENT, SO WE DO
        //          END UP WITH A FRAME, BUT THE identifiers.org GUYS ARE GOING
        //          TO 'FIX' THAT, SO WE SHOULD BE READY FOR WHEN IT'S DONE...
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::resourceIdLookupRequested(const QString &pResource,
                                                                          const QString &pId,
                                                                          const bool &pRetranslate)
{
    // The user requested a resource id to be looked up, so retrieve it using
    // identifiers.org, but only if we are not retranslating since the looking
    // up would already be correct

    if (!pRetranslate)
        mWebView->setUrl("http://identifiers.org/"+pResource+"/"+pId+"?profile=most_reliable&redirect=true");
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::metadataUpdated()
{
    // Some metadata has been updated, so we need to update the metadata
    // information we show to the user
qDebug(">>> HELLO! :)");

    updateGui(mParent->cellmlFile()->rdfTriples(mParent->listsWidget()->metadataList()->currentId()));
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
