//==============================================================================
// CellML annotation view CellML metadata details widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmlannotationviewcellmlmetadatadetailswidget.h"
#include "cellmlannotationviewmetadatabiomodelsdotnetviewdetailswidget.h"
#include "cellmlannotationviewwidget.h"
#include "cellmlannotationviewmetadataviewdetailswidget.h"
#include "usermessagewidget.h"

//==============================================================================

#include "ui_cellmlannotationviewcellmlmetadatadetailswidget.h"

//==============================================================================

#include <QWebView>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewCellmlMetadataDetailsWidget::CellmlAnnotationViewCellmlMetadataDetailsWidget(CellmlAnnotationViewWidget *pParent) :
    Widget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewCellmlMetadataDetailsWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our unsupported metadata message widget

    mUnsupportedMetadataMsg = new Core::UserMessageWidget(pParent,
                                                          ":/oxygen/actions/help-about.png");
    mBorderedUnsupportedMetadataMsg = new Core::BorderedWidget(mUnsupportedMetadataMsg,
                                                               true, true, false, false);

    mBorderedUnsupportedMetadataMsg->setVisible(false);
    // Note: we don't initially want to see it, so...

    // Create our splitter widget

    mSplitter = new QSplitter(Qt::Vertical, pParent);

    // Create our details widgets

    mMetadataViewDetails  = new CellmlAnnotationViewMetadataViewDetailsWidget(pParent, false);
    mWebView              = new QWebView(pParent);

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

CellmlAnnotationViewCellmlMetadataDetailsWidget::~CellmlAnnotationViewCellmlMetadataDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewCellmlMetadataDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    mMetadataViewDetails->retranslateUi();

    // Update our unsupported metadata message

    mUnsupportedMetadataMsg->setMessage(tr("Sorry, but the <strong>%1</strong> view does not support the type of metadata associated with the current CellML element...").arg(mParent->pluginViewName()));
}

//==============================================================================

void CellmlAnnotationViewCellmlMetadataDetailsWidget::updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples)
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

    // Show/hide our web viewer, depending on whether the type of the metadata
    // is known or not

    mBorderedWebView->setVisible(!isUnknownMetadata);

    mBorderedMetadataViewDetails->setBottomBorderVisible(!isUnknownMetadata);

    // Update our metadata view details

    mMetadataViewDetails->updateGui(pRdfTriples);
}

//==============================================================================

void CellmlAnnotationViewCellmlMetadataDetailsWidget::updateSizes(const QList<int> &pSizes)
{
    // The splitter of another CellmlAnnotationViewMetadataDetailsWidget object
    // has been moved, so update our sizes

    mSplitter->setSizes(pSizes);
}

//==============================================================================

void CellmlAnnotationViewCellmlMetadataDetailsWidget::emitSplitterMoved()
{
    // Let people know that our splitter has been moved

    emit splitterMoved(mSplitter->sizes());
}

//==============================================================================

CellmlAnnotationViewMetadataViewDetailsWidget * CellmlAnnotationViewCellmlMetadataDetailsWidget::metadataViewDetails() const
{
    // Return our metadata view details widget

    return mMetadataViewDetails;
}

//==============================================================================

void CellmlAnnotationViewCellmlMetadataDetailsWidget::qualifierLookupRequested(const QString &pQualifier,
                                                                               const bool &pRetranslate)
{
    // Ask our parent to update our web viewer for us

    mParent->updateWebViewerWithQualifierDetails(mWebView, pQualifier, pRetranslate);
}

//==============================================================================

void CellmlAnnotationViewCellmlMetadataDetailsWidget::resourceLookupRequested(const QString &pResource,
                                                                              const bool &pRetranslate)
{
    // Ask our parent to update our web viewer for us

    mParent->updateWebViewerWithResourceDetails(mWebView, pResource, pRetranslate);
}

//==============================================================================

void CellmlAnnotationViewCellmlMetadataDetailsWidget::resourceIdLookupRequested(const QString &pResource,
                                                                                const QString &pId,
                                                                                const bool &pRetranslate)
{
    // Ask our parent to update our web viewer for us

    mParent->updateWebViewerWithResourceIdDetails(mWebView, pResource, pId, pRetranslate);
}

//==============================================================================

void CellmlAnnotationViewCellmlMetadataDetailsWidget::unknownLookupRequested()
{
    // We are 'asked' to lookup something unknown, so 'clean up' our web view

    mWebView->setUrl(QUrl());
}

//==============================================================================

QSplitter * CellmlAnnotationViewCellmlMetadataDetailsWidget::splitter() const
{
    // Return our splitter widget

    return mSplitter;
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
