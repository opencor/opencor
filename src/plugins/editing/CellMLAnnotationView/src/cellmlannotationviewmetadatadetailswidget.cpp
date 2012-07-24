//==============================================================================
// CellML annotation view metadata details widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmlannotationviewmetadatanormalviewdetailswidget.h"
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

    mWebView->setAcceptDrops(false);

    mBorderedMetadataEditDetails = new Core::BorderedWidget(mMetadataEditDetails,
                                                            false, true, true, false);
    mBorderedMetadataViewDetails = new Core::BorderedWidget(mMetadataViewDetails,
                                                            true, true, true, false);
    mBorderedWebView = new Core::BorderedWidget(mWebView,
                                                true, true, false, false);

    // Some connections to handle the looking up of a qualifier from our
    // metadata edit details view, as well as the disabling of information
    // lookup for the metadata view details widget

    connect(mMetadataEditDetails, SIGNAL(qualifierLookupRequested(const QString &, const bool &)),
            mMetadataViewDetails->normalView(), SLOT(disableLookupInformation()));
    connect(mMetadataEditDetails, SIGNAL(qualifierLookupRequested(const QString &, const bool &)),
            this, SLOT(lookupQualifier(const QString &, const bool &)));

    connect(mMetadataEditDetails, SIGNAL(resourceLookupRequested(const QString &, const bool &)),
            mMetadataViewDetails->normalView(), SLOT(disableLookupInformation()));
    connect(mMetadataEditDetails, SIGNAL(resourceLookupRequested(const QString &, const bool &)),
            this, SLOT(lookupResource(const QString &, const bool &)));

    connect(mMetadataEditDetails, SIGNAL(idLookupRequested(const QString &, const QString &, const bool &)),
            mMetadataViewDetails->normalView(), SLOT(disableLookupInformation()));
    connect(mMetadataEditDetails, SIGNAL(idLookupRequested(const QString &, const QString &, const bool &)),
            this, SLOT(lookupId(const QString &, const QString &, const bool &)));

    connect(mMetadataEditDetails, SIGNAL(noLookupRequested()),
            mMetadataViewDetails->normalView(), SLOT(disableLookupInformation()));
    connect(mMetadataEditDetails, SIGNAL(noLookupRequested()),
            this, SLOT(lookupNothing()));

    // Some connections to handle the looking up of a qualifier, resource and
    // resource id from our normal view, as well as the disabling of information
    // lookup for the metadata edit details widget

    connect(mMetadataViewDetails->normalView(), SIGNAL(qualifierLookupRequested(const QString &, const bool &)),
            mMetadataEditDetails, SLOT(disableLookupInformation()));
    connect(mMetadataViewDetails->normalView(), SIGNAL(qualifierLookupRequested(const QString &, const bool &)),
            this, SLOT(lookupQualifier(const QString &, const bool &)));

    connect(mMetadataViewDetails->normalView(), SIGNAL(resourceLookupRequested(const QString &, const bool &)),
            mMetadataEditDetails, SLOT(disableLookupInformation()));
    connect(mMetadataViewDetails->normalView(), SIGNAL(resourceLookupRequested(const QString &, const bool &)),
            this, SLOT(lookupResource(const QString &, const bool &)));

    connect(mMetadataViewDetails->normalView(), SIGNAL(idLookupRequested(const QString &, const QString &, const bool &)),
            mMetadataEditDetails, SLOT(disableLookupInformation()));
    connect(mMetadataViewDetails->normalView(), SIGNAL(idLookupRequested(const QString &, const QString &, const bool &)),
            this, SLOT(lookupId(const QString &, const QString &, const bool &)));

    connect(mMetadataViewDetails->normalView(), SIGNAL(noLookupRequested()),
            mMetadataEditDetails, SLOT(disableLookupInformation()));
    connect(mMetadataViewDetails->normalView(), SIGNAL(noLookupRequested()),
            this, SLOT(lookupNothing()));

    // A connection to handle the addition of some metadata to our CellML file,
    // therefore needing to update our normal view

    connect(mMetadataEditDetails, SIGNAL(metadataAdded(CellMLSupport::CellmlFileRdfTriple *)),
            mMetadataViewDetails->normalView(), SLOT(addRdfTriple(CellMLSupport::CellmlFileRdfTriple *)));

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

void CellmlAnnotationViewMetadataDetailsWidget::updateGui(CellMLSupport::CellmlFileElement *pCellmlElement)
{
    if (!pCellmlElement)
        return;

    // Show/hide our unsupported metadata message depending on whether the type
    // of the RDF triples is known or not

    bool isUnknownMetadata = pCellmlElement->rdfTriples().type() == CellMLSupport::CellmlFileRdfTriple::Unknown;

    mBorderedUnsupportedMetadataMsg->setVisible(isUnknownMetadata);

    // Show/hide our metadata edit details and web viewer, depending on whether
    // the type of the metadata is known or not

    mBorderedMetadataEditDetails->setVisible(!isUnknownMetadata);
    mBorderedWebView->setVisible(!isUnknownMetadata);

    mBorderedMetadataViewDetails->setTopBorderVisible(!isUnknownMetadata);
    mBorderedMetadataViewDetails->setBottomBorderVisible(!isUnknownMetadata);

    // Update our metadata view details

    mMetadataViewDetails->updateGui(pCellmlElement);
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

void CellmlAnnotationViewMetadataDetailsWidget::lookupQualifier(const QString &pQualifier,
                                                                const bool &pRetranslate)
{
    // Ask our parent to update our web viewer for us

    mParent->updateWebViewerWithQualifierDetails(mWebView, pQualifier, pRetranslate);
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::lookupResource(const QString &pResource,
                                                               const bool &pRetranslate)
{
    // Ask our parent to update our web viewer for us

    mParent->updateWebViewerWithResourceDetails(mWebView, pResource, pRetranslate);
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::lookupId(const QString &pResource,
                                                         const QString &pId,
                                                         const bool &pRetranslate)
{
    // Ask our parent to update our web viewer for us

    mParent->updateWebViewerWithIdDetails(mWebView, pResource, pId, pRetranslate);
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::lookupNothing()
{
    // We are 'asked' to look nothing up, so 'clean up' our web view

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
