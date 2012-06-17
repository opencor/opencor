//==============================================================================
// CellML annotation view CellML details widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmlannotationviewcellmldetailswidget.h"
#include "cellmlannotationviewmetadatabiomodelsdotnetviewdetailswidget.h"
#include "cellmlannotationviewwidget.h"
#include "cellmlannotationviewmetadataviewdetailswidget.h"
#include "coreutils.h"

//==============================================================================

#include "ui_cellmlannotationviewcellmldetailswidget.h"

//==============================================================================

#include <QComboBox>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QWebView>

//==============================================================================

#include <QJsonParser>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewCellmlDetailsWidget::CellmlAnnotationViewCellmlDetailsWidget(CellmlAnnotationViewWidget *pParent) :
    QSplitter(pParent),
    CommonWidget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewCellmlDetailsWidget),
    mQualifier(QString())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our details widgets

    mCellmlElementDetails = new CellmlAnnotationViewCellmlElementDetailsWidget(pParent);
    mMetadataViewDetails  = new CellmlAnnotationViewMetadataViewDetailsWidget(pParent);
    mWebView              = new QWebView(pParent);

    // A connection to handle the looking up of a resource and a resource id

    connect(mMetadataViewDetails->bioModelsDotNetView(), SIGNAL(qualifierLookupRequested(const QString &)),
            this, SLOT(qualifierLookupRequested(const QString &)));
    connect(mMetadataViewDetails->bioModelsDotNetView(), SIGNAL(resourceLookupRequested(const QString &)),
            this, SLOT(resourceLookupRequested(const QString &)));
    connect(mMetadataViewDetails->bioModelsDotNetView(), SIGNAL(resourceIdLookupRequested(const QString &, const QString &)),
            this, SLOT(resourceIdLookupRequested(const QString &, const QString &)));

    // Add our details widgets to our splitter

    addWidget(new Core::BorderedWidget(mCellmlElementDetails,
                                       false, true, true, false));
    addWidget(new Core::BorderedWidget(mMetadataViewDetails,
                                       true, true, true, false));
    addWidget(new Core::BorderedWidget(mWebView,
                                       true, true, false, false));

    // Keep track of our splitter being moved

    connect(this, SIGNAL(splitterMoved(int,int)),
            this, SLOT(emitSplitterMoved()));

    // Retrieve the output template

    QFile qualifierInformationFile(":qualifierInformation");

    qualifierInformationFile.open(QIODevice::ReadOnly);

    mQualifierInformationTemplate = QString(qualifierInformationFile.readAll());

    qualifierInformationFile.close();

    // Retrieve the SVG diagrams

    QFile modelQualifierFile(":modelQualifier");
    QFile biologyQualifierFile(":biologyQualifier");

    modelQualifierFile.open(QIODevice::ReadOnly);
    biologyQualifierFile.open(QIODevice::ReadOnly);

    mModelQualifierSvg   = QString(modelQualifierFile.readAll());
    mBiologyQualifierSvg = QString(biologyQualifierFile.readAll());

    modelQualifierFile.close();
    biologyQualifierFile.close();
}

//==============================================================================

CellmlAnnotationViewCellmlDetailsWidget::~CellmlAnnotationViewCellmlDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    mCellmlElementDetails->retranslateUi();
    mMetadataViewDetails->retranslateUi();

    if (!mQualifier.isEmpty())
        qualifierLookupRequested(mQualifier);
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::updateGui(const CellmlAnnotationViewCellmlElementDetailsWidget::Items &pItems)
{
    // Stop tracking any change in the cmeta:id value of our CellML element

    if (mCellmlElementDetails->cmetaIdValue()) {
        disconnect(mCellmlElementDetails->cmetaIdValue(), SIGNAL(currentIndexChanged(const QString &)),
                   this, SLOT(newCmetaIdValue(const QString &)));
        disconnect(mCellmlElementDetails->cmetaIdValue(), SIGNAL(editTextChanged(const QString &)),
                   this, SLOT(newCmetaIdValue(const QString &)));
    }

    // 'Clean up' our web view

    mWebView->setUrl(QString());

    // Update our CellML element details GUI

    mCellmlElementDetails->updateGui(pItems);

    // Track any change in the cmeta:id value of our CellML element

    connect(mCellmlElementDetails->cmetaIdValue(), SIGNAL(currentIndexChanged(const QString &)),
            this, SLOT(newCmetaIdValue(const QString &)));
    connect(mCellmlElementDetails->cmetaIdValue(), SIGNAL(editTextChanged(const QString &)),
            this, SLOT(newCmetaIdValue(const QString &)));

    // Update our metadata details GUI

    newCmetaIdValue(mCellmlElementDetails->cmetaIdValue()->currentText());
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::updateSizes(const QList<int> &pSizes)
{
    // The splitter of another CellmlAnnotationViewCellmlDetailsWidget object
    // has been moved, so update our sizes

    setSizes(pSizes);
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::emitSplitterMoved()
{
    // Let whoever know that our splitter has been moved

    emit splitterMoved(sizes());
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::newCmetaIdValue(const QString &pCmetaIdValue)
{
    // The cmeta:id value of our CellML element has changed, so update its
    // metadata details, but first 'clean up' our web view

    mWebView->setUrl(QString());

    if (pCmetaIdValue.isEmpty())
        // The CellML element doesn't have a cmeta:id value, so we don't want
        // any metadata to be shown for it (not even the model-wide metadata)

        mMetadataViewDetails->updateGui();
    else
        mMetadataViewDetails->updateGui(mParent->rdfTriples(pCmetaIdValue));
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::qualifierLookupRequested(const QString &pQualifier)
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

    // Keep track of the qualifier (useful when wanting to retranslate OpenCOR)

    mQualifier = pQualifier;

    // Generate the web page containing some information about the qualifier

    QString qualifierSvg;
    QString shortDescription;
    QString longDescription;

    if (!pQualifier.compare("model:is")) {
        qualifierSvg = mModelQualifierSvg;

        shortDescription = tr("Identity");
        longDescription  = tr("The modelling object represented by the model element is identical with the subject of the referenced resource (\"Modelling Object B\"). For instance, this qualifier might be used to link an encoded model to a database of models.");
    } else if (!pQualifier.compare("model:isDerivedFrom")) {
        qualifierSvg = mModelQualifierSvg;

        shortDescription = tr("Origin");
        longDescription  = tr("The modelling object represented by the model element is derived from the modelling object represented by the referenced resource (\"Modelling Object B\"). This relation may be used, for instance, to express a refinement or adaptation in usage for a previously described modelling component.");
    } else if (!pQualifier.compare("model:isDescribedBy")) {
        qualifierSvg = mModelQualifierSvg;

        shortDescription = tr("Description");
        longDescription  = tr("The modelling object represented by the model element is described by the subject of the referenced resource (\"Modelling Object B\"). This relation might be used to link a model or a kinetic law to the literature that describes it.");
    } else if (!pQualifier.compare("bio:encodes")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Encodement");
        longDescription  = tr("The biological entity represented by the model element encodes, directly or transitively, the subject of the referenced resource (\"Biological Entity B\"). This relation may be used to express, for example, that a specific DNA sequence encodes a particular protein.");
    } else if (!pQualifier.compare("bio:hasPart")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Part");
        longDescription  = tr("The biological entity represented by the model element includes the subject of the referenced resource (\"Biological Entity B\"), either physically or logically. This relation might be used to link a complex to the description of its components.");
    } else if (!pQualifier.compare("bio:hasProperty")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Property");
        longDescription  = tr("The subject of the referenced resource (\"Biological Entity B\") is a property of the biological entity represented by the model element. This relation might be used when a biological entity exhibits a certain enzymatic activity or exerts a specific function.");
    } else if (!pQualifier.compare("bio:hasVersion")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Version");
        longDescription  = tr("The subject of the referenced resource (\"Biological Entity B\") is a version or an instance of the biological entity represented by the model element. This relation may be used to represent an isoform of modified form of a biological entity.");
    } else if (!pQualifier.compare("bio:is")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Indentity");
        longDescription  = tr("The biological entity represented by the model element has identity with the subject of the referenced resource (\"Biological Entity B\"). This relation might be used to link a reaction to its exact counterpart in a database, for instance.");
    } else if (!pQualifier.compare("bio:isDescribedBy")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Description");
        longDescription  = tr("The biological entity represented by the model element is described by the subject of the referenced resource (\"Biological Entity B\"). This relation should be used, for instance, to link a species or a parameter to the literature that describes the concentration of that species or the value of that parameter.");
    } else if (!pQualifier.compare("bio:isEncodedBy")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Encoder");
        longDescription  = tr("The biological entity represented by the model element is encoded, directly or transitively, by the subject of the referenced resource (\"Biological Entity B\"). This relation may be used to express, for example, that a protein is encoded by a specific DNA sequence.");
    } else if (!pQualifier.compare("bio:isHomologTo")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Homolog");
        longDescription  = tr("The biological entity represented by the model element is homologous to the subject of the referenced resource (\"Biological Entity B\"). This relation can be used to represent biological entities that share a common ancestor.");
    } else if (!pQualifier.compare("bio:isPartOf")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Parthood");
        longDescription  = tr("The biological entity represented by the model element is a physical or logical part of the subject of the referenced resource (\"Biological Entity B\"). This relation may be used to link a model component to a description of the complex in which it is a part.");
    } else if (!pQualifier.compare("bio:isPropertyOf")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Property bearer");
        longDescription  = tr("The biological entity represented by the model element is a property of the referenced resource (\"Biological Entity B\").");
    } else if (!pQualifier.compare("bio:isVersionOf")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Hypernym");
        longDescription  = tr("The biological entity represented by the model element is a version or an instance of the subject of the referenced resource (\"Biological Entity B\"). This relation may be used to represent, for example, the 'superclass' or 'parent' form of a particular biological entity.");
    } else if (!pQualifier.compare("bio:occursIn")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Container");
        longDescription  = tr("The biological entity represented by the model element is physically limited to a location, which is the subject of the referenced resource (\"Biological Entity B\"). This relation may be used to ascribe a compartmental location, within which a reaction takes place.");
    } else if (!pQualifier.compare("bio:hasTaxon")) {
        qualifierSvg = mBiologyQualifierSvg;

        shortDescription = tr("Taxon");
        longDescription  = tr("The biological entity represented by the model element is taxonomically restricted, where the restriction is the subject of the referenced resource (\"Biological Entity B\"). This relation may be used to ascribe a species restriction to a biochemical reaction.");
    } else {
        qualifierSvg = "";

        shortDescription = tr("Unknown");
        longDescription  = tr("Unknown");
    }

    // Show the information

    mWebView->setHtml(mQualifierInformationTemplate.arg(pQualifier,
                                                        qualifierSvg,
                                                        shortDescription,
                                                        longDescription,
                                                        Core::copyright()));
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::resourceLookupRequested(const QString &pResource)
{
    // The user requested a resource to be looked up, so retrieve it using
    // identifiers.org

    mQualifier = QString();

    mWebView->setUrl("http://identifiers.org/"+pResource+"/?redirect=true");
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::resourceIdLookupRequested(const QString &pResource,
                                                                        const QString &pId)
{
    // The user requested a resource id to be looked up, so retrieve it using
    // identifiers.org

    mQualifier = QString();

    mWebView->setUrl("http://identifiers.org/"+pResource+"/"+pId+"?profile=most_reliable&redirect=true");
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
