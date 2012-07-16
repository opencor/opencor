//==============================================================================
// CellML annotation view metadata details widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmlannotationviewmetadatabiomodelsdotnetviewdetailswidget.h"
#include "cellmlannotationviewmetadatadetailswidget.h"
#include "cellmlannotationviewmetadataeditdetailswidget.h"
#include "cellmlannotationviewmetadataviewdetailswidget.h"
#include "cellmlannotationviewwidget.h"
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

    mMetadataEditDetails = new CellmlAnnotationViewMetadataEditDetailsWidget(pParent);
    mMetadataViewDetails = new CellmlAnnotationViewMetadataViewDetailsWidget(pParent, true);
    mWebView             = new QWebView(pParent);

    mBorderedMetadataEditDetails = new Core::BorderedWidget(mMetadataEditDetails,
                                                            false, true, true, false);
    mBorderedMetadataViewDetails = new Core::BorderedWidget(mMetadataViewDetails,
                                                            true, true, true, false);
    mBorderedWebView = new Core::BorderedWidget(mWebView,
                                                true, true, false, false);

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

    // Populate our splitter widget

    mSplitter->addWidget(mBorderedMetadataEditDetails);
    mSplitter->addWidget(mBorderedMetadataViewDetails);
    mSplitter->addWidget(mBorderedWebView);

    // Keep track of our splitter being moved

    connect(mSplitter, SIGNAL(splitterMoved(int,int)),
            this, SLOT(emitSplitterMoved()));

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

    mMetadataEditDetails->retranslateUi();
    mMetadataViewDetails->retranslateUi();

    // Update our unsupported metadata message

    mUnsupportedMetadataMsg->setMessage(tr("Sorry, but the <strong>%1</strong> view does not support this type of metadata...").arg(mParent->pluginViewName()));
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples)
{
    static CellMLSupport::CellmlFileRdfTriples rdfTriples = CellMLSupport::CellmlFileRdfTriples();

    if (pRdfTriples == rdfTriples)
        // We want to show the same RDF triples, so...

        return;

    // Keep track of the RDF triples

    rdfTriples = pRdfTriples;

    // Show/hide our unsupported metadata message depending on whether the type
    // of the RDF triples is known or not

    bool isUnknownMetadata = pRdfTriples.type() == CellMLSupport::CellmlFileRdfTriple::Unknown;

    mBorderedUnsupportedMetadataMsg->setVisible(isUnknownMetadata);

    // Show/hide our metadata edit details and web viewer, depending on whether
    // the type of the metadata is known or not

    mBorderedMetadataEditDetails->setVisible(!isUnknownMetadata);
    mBorderedWebView->setVisible(!isUnknownMetadata);

    mBorderedMetadataViewDetails->setTopBorderVisible(!isUnknownMetadata);
    mBorderedMetadataViewDetails->setBottomBorderVisible(!isUnknownMetadata);

    // Update our metadata view details

    mMetadataViewDetails->updateGui(pRdfTriples);
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::updateSizes(const QList<int> &pSizes)
{
    // The splitter of another CellmlAnnotationViewMetadataDetailsWidget object
    // has been moved, so update our sizes

    mSplitter->setSizes(pSizes);
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::emitSplitterMoved()
{
    // Let people know that our splitter has been moved

    emit splitterMoved(mSplitter->sizes());
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::qualifierLookupRequested(const QString &pQualifier,
                                                                         const bool &pRetranslate)
{
    // Ask our parent to update our web viewer for us

    mParent->updateWebViewerWithQualifierDetails(mWebView, pQualifier, pRetranslate);
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::resourceLookupRequested(const QString &pResource,
                                                                        const bool &pRetranslate)
{
    // Ask our parent to update our web viewer for us

    mParent->updateWebViewerWithResourceDetails(mWebView, pResource, pRetranslate);
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::resourceIdLookupRequested(const QString &pResource,
                                                                          const QString &pId,
                                                                          const bool &pRetranslate)
{
    // Ask our parent to update our web viewer for us

    mParent->updateWebViewerWithResourceIdDetails(mWebView, pResource, pId, pRetranslate);
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::unknownLookupRequested()
{
    // We are 'asked' to look up something unknown, so 'clean up' our web view

    mWebView->setUrl(QUrl());
}

//==============================================================================

QSplitter * CellmlAnnotationViewMetadataDetailsWidget::splitter() const
{
    // Return our splitter widget

    return mSplitter;
}

//==============================================================================

CellmlAnnotationViewMetadataEditDetailsWidget * CellmlAnnotationViewMetadataDetailsWidget::metadataEditDetails() const
{
    // Return our metadata edit details widget

    return mMetadataEditDetails;
}

//==============================================================================

CellmlAnnotationViewMetadataViewDetailsWidget * CellmlAnnotationViewMetadataDetailsWidget::metadataViewDetails() const
{
    // Return our metadata view details widget

    return mMetadataViewDetails;
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
