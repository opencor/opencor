//==============================================================================
// CellML annotation view CellML metadata details widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmlannotationviewcellmlmetadatadetailswidget.h"
#include "cellmlannotationviewmetadatabiomodelsdotnetviewdetailswidget.h"
#include "cellmlannotationviewmetadatarawviewdetailswidget.h"
#include "cellmlannotationviewwidget.h"
#include "cellmlannotationviewmetadataviewdetailswidget.h"

//==============================================================================

#include "ui_cellmlannotationviewcellmlmetadatadetailswidget.h"

//==============================================================================

#include <QWebView>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewCellmlMetadataDetailsWidget::CellmlAnnotationViewCellmlMetadataDetailsWidget(CellmlAnnotationViewWidget *pParent) :
    QSplitter(pParent),
    CommonWidget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewCellmlMetadataDetailsWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

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

    // Add our details widgets to our splitter

    addWidget(mBorderedMetadataViewDetails);
    addWidget(mBorderedWebView);

    // Keep track of our splitter being moved

    connect(this, SIGNAL(splitterMoved(int,int)),
            this, SLOT(emitSplitterMoved()));
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
}

//==============================================================================

void CellmlAnnotationViewCellmlMetadataDetailsWidget::updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples)
{
    static CellMLSupport::CellmlFileRdfTriples rdfTriples = CellMLSupport::CellmlFileRdfTriples(mParent->cellmlFile());

    if (pRdfTriples == rdfTriples)
        // We want to show the same RDF triples, so...

        return;

    // Keep track of the RDF triples

    rdfTriples = pRdfTriples;

    // Show/hide our web viewer, depending on whether the type of the metadata
    // is known or not

    bool isUnknownMetadata = pRdfTriples.type() == CellMLSupport::CellmlFileRdfTriple::Unknown;

    mBorderedWebView->setVisible(!isUnknownMetadata);

    mBorderedMetadataViewDetails->setBottomBorderVisible(!isUnknownMetadata);

    // Update our metadata view details

    mMetadataViewDetails->updateGui(pRdfTriples);
}

//==============================================================================

CellmlAnnotationViewMetadataViewDetailsWidget * CellmlAnnotationViewCellmlMetadataDetailsWidget::metadataViewDetails() const
{
    // Return our metadata view details widget

    return mMetadataViewDetails;
}

//==============================================================================

void CellmlAnnotationViewCellmlMetadataDetailsWidget::updateSizes(const QList<int> &pSizes)
{
    // The splitter of another CellmlAnnotationViewCellmlDetailsWidget object
    // has been moved, so update our sizes

    setSizes(pSizes);
}

//==============================================================================

void CellmlAnnotationViewCellmlMetadataDetailsWidget::emitSplitterMoved()
{
    // Let people know that our splitter has been moved

    emit splitterMoved(sizes());
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

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
