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
    connect(mMetadataViewDetails->bioModelsDotNetView(), SIGNAL(unknownLookupRequested()),
            this, SLOT(unknownLookupRequested()));

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

    // 'Clean up' our web view

    mWebView->setUrl(QString());

    // Update our Metadata view details GUI

    mMetadataViewDetails->updateGui(pRdfTriples);
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
    // We are 'asked' to lookup something unknown, so 'clean up' our web view

    mWebView->setUrl(QString());
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::metadataUpdated()
{
    // Some metadata has been updated, so we need to update the metadata
    // information we show to the user

    updateGui(mParent->cellmlFile()->rdfTriples(mParent->listsWidget()->metadataList()->currentId()));
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
