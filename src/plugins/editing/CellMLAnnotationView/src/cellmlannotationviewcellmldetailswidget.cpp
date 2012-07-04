//==============================================================================
// CellML annotation view CellML details widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmlannotationviewcellmldetailswidget.h"
#include "cellmlannotationviewmetadatabiomodelsdotnetviewdetailswidget.h"
#include "cellmlannotationviewwidget.h"
#include "cellmlannotationviewmetadataviewdetailswidget.h"

//==============================================================================

#include "ui_cellmlannotationviewcellmldetailswidget.h"

//==============================================================================

#include <QComboBox>
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
    mGui(new Ui::CellmlAnnotationViewCellmlDetailsWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our details widgets

    mCellmlElementDetails = new CellmlAnnotationViewCellmlElementDetailsWidget(pParent);
    mMetadataViewDetails  = new CellmlAnnotationViewMetadataViewDetailsWidget(pParent, false);
    mWebView              = new QWebView(pParent);

    // Some connections to handle the looking up of a qualifier, resource and
    // resource id

    connect(mMetadataViewDetails->bioModelsDotNetView(), SIGNAL(qualifierLookupRequested(const QString &, const bool &)),
            this, SLOT(qualifierLookupRequested(const QString &, const bool &)));
    connect(mMetadataViewDetails->bioModelsDotNetView(), SIGNAL(resourceLookupRequested(const QString &, const bool &)),
            this, SLOT(resourceLookupRequested(const QString &, const bool &)));
    connect(mMetadataViewDetails->bioModelsDotNetView(), SIGNAL(resourceIdLookupRequested(const QString &, const QString &, const bool &)),
            this, SLOT(resourceIdLookupRequested(const QString &, const QString &, const bool &)));
    connect(mMetadataViewDetails->bioModelsDotNetView(), SIGNAL(unknownLookupRequested()),
            this, SLOT(unknownLookupRequested()));

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
    // Note: we must also update the connection for our cmeta:id widget since it
    //       gets recreated as a result of the retranslation...

    if (mCellmlElementDetails->cmetaIdValue())
        disconnect(mCellmlElementDetails->cmetaIdValue(), SIGNAL(editTextChanged(const QString &)),
                   this, SLOT(newCmetaId(const QString &)));

    mGui->retranslateUi(this);

    mCellmlElementDetails->retranslateUi();
    mMetadataViewDetails->retranslateUi();

    if (mCellmlElementDetails->cmetaIdValue())
        connect(mCellmlElementDetails->cmetaIdValue(), SIGNAL(editTextChanged(const QString &)),
                this, SLOT(newCmetaId(const QString &)));
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::updateGui(const CellmlAnnotationViewCellmlElementDetailsWidget::Items &pItems)
{
    static CellmlAnnotationViewCellmlElementDetailsWidget::Items items = CellmlAnnotationViewCellmlElementDetailsWidget::Items();

    if (pItems == items)
        // We want to show the same items, so...

        return;

    // Keep track of the items

    items = pItems;

    // Stop tracking changes to the cmeta:id value of our CellML element

    if (mCellmlElementDetails->cmetaIdValue())
        disconnect(mCellmlElementDetails->cmetaIdValue(), SIGNAL(editTextChanged(const QString &)),
                   this, SLOT(newCmetaId(const QString &)));

    // 'Clean up' our web view

    mWebView->setUrl(QString());

    // Update our CellML element details GUI

    mCellmlElementDetails->updateGui(pItems);

    // Re-track changes to the cmeta:id value of our CellML element and update
    // our metadata details GUI

    if (mCellmlElementDetails->cmetaIdValue()) {
        connect(mCellmlElementDetails->cmetaIdValue(), SIGNAL(editTextChanged(const QString &)),
                this, SLOT(newCmetaId(const QString &)));

        newCmetaId(mCellmlElementDetails->cmetaIdValue()->currentText());
    }
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

void CellmlAnnotationViewCellmlDetailsWidget::newCmetaId(const QString &pCmetaId)
{
    // Retrieve the RDF triples for the cmeta:id

    CellMLSupport::CellmlFileRdfTriples rdfTriples = mParent->cellmlFile()->rdfTriples(pCmetaId);

    // Check that we are not dealing with the same RDF triples
    // Note: this may happen when manually typing the name of a cmeta:id and the
    //       QComboBox suggesting something for you, e.g. you start typing "C_"
    //       and the QComboBox suggests "C_C" (which will get us here) and then
    //       you finish typing "C_C" (which will also get us here)

    static CellMLSupport::CellmlFileRdfTriples oldRdfTriples = CellMLSupport::CellmlFileRdfTriples(mParent->cellmlFile());

    if (rdfTriples == oldRdfTriples)
        return;

    oldRdfTriples = rdfTriples;

    // 'Clean up' our web view

    mWebView->setUrl(QString());

    // Update its metadata details

    mMetadataViewDetails->updateGui(rdfTriples);
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::qualifierLookupRequested(const QString &pQualifier,
                                                                       const bool &pRetranslate)
{
    // Ask our parent to update our web viewer for us

    mParent->updateWebViewerWithQualifierDetails(mWebView, pQualifier, pRetranslate);
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::resourceLookupRequested(const QString &pResource,
                                                                      const bool &pRetranslate)
{
    // Ask our parent to update our web viewer for us

    mParent->updateWebViewerWithResourceDetails(mWebView, pResource, pRetranslate);
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::resourceIdLookupRequested(const QString &pResource,
                                                                        const QString &pId,
                                                                        const bool &pRetranslate)
{
    // Ask our parent to update our web viewer for us

    mParent->updateWebViewerWithResourceIdDetails(mWebView, pResource, pId, pRetranslate);
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::unknownLookupRequested()
{
    // We are 'asked' to lookup something unknown, so 'clean up' our web view

    mWebView->setUrl(QString());
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::metadataUpdated()
{
    // Some metadata has been updated, so we need to update the metadata
    // information we show to the user

    if (mCellmlElementDetails->cmetaIdValue())
        newCmetaId(mCellmlElementDetails->cmetaIdValue()->currentText());
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
