/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// CellML annotation view metadata details widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmlannotationvieweditingwidget.h"
#include "cellmlannotationviewmetadatanormalviewdetailswidget.h"
#include "cellmlannotationviewmetadatadetailswidget.h"
#include "cellmlannotationviewmetadataeditdetailswidget.h"
#include "cellmlannotationviewmetadataviewdetailswidget.h"
#include "cellmlannotationviewplugin.h"
#include "filemanager.h"
#include "usermessagewidget.h"

//==============================================================================

#include "ui_cellmlannotationviewmetadatadetailswidget.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QLabel>
#include <QWebView>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataDetailsWidget::CellmlAnnotationViewMetadataDetailsWidget(CellmlAnnotationViewEditingWidget *pParent) :
    Widget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewMetadataDetailsWidget),
    mCellmlFile(pParent->cellmlFile()),
    mElement(0)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our unsupported metadata message widget

    mCategoryMessage = new Core::UserMessageWidget(":/oxygen/actions/help-about.png", this);
    mBorderedCategoryMessage = new Core::BorderedWidget(mCategoryMessage,
                                                        false, true, true, false);

    mBorderedCategoryMessage->setVisible(false);
    // Note: we don't initially want to see it, so...

    // Create our unsupported metadata message widget

    mUnsupportedMetadataMessage = new Core::UserMessageWidget(":/oxygen/actions/help-about.png", this);
    mBorderedUnsupportedMetadataMessage = new Core::BorderedWidget(mUnsupportedMetadataMessage,
                                                                   false, true, true, false);

    mBorderedUnsupportedMetadataMessage->setVisible(false);
    // Note: we don't initially want to see it, so...

    // Create our splitter widget

    mSplitter = new QSplitter(Qt::Vertical, this);

    // Create our details widgets

    mMetadataEditDetails = new CellmlAnnotationViewMetadataEditDetailsWidget(pParent);
    mMetadataViewDetails = new CellmlAnnotationViewMetadataViewDetailsWidget(pParent);
    mWebView             = new QWebView(this);

    mWebView->setAcceptDrops(false);
    mWebView->setContextMenuPolicy(Qt::NoContextMenu);

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

    // A connection to handle the removal of an RDF triple from our normal view
    // and to have our metadata edit details widget update itself with regards
    // to possible terms (i.e. enable/disable add buttons)

    connect(mMetadataViewDetails->normalView(), SIGNAL(rdfTripleRemoved(CellMLSupport::CellmlFileRdfTriple *)),
            this, SLOT(updateMetadataEditDetails()));

    // A connection to handle the clicking of the link in the unsupported
    // message

    connect(mUnsupportedMetadataMessage, SIGNAL(linkActivated(const QString &)),
            this, SLOT(removeAllMetadata()));

    // Populate our splitter widget

    mSplitter->addWidget(mBorderedMetadataEditDetails);
    mSplitter->addWidget(mBorderedMetadataViewDetails);
    mSplitter->addWidget(mBorderedWebView);

    // Keep track of our splitter being moved

    connect(mSplitter, SIGNAL(splitterMoved(int, int)),
            this, SLOT(emitSplitterMoved()));

    // Add our unsupported metadata message widget and splitter widget to our
    // layout

    mGui->layout->addWidget(mBorderedCategoryMessage);
    mGui->layout->addWidget(mBorderedUnsupportedMetadataMessage);
    mGui->layout->addWidget(mSplitter);

    // Some further initialisations that are done as part of retranslating the
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

    // Retranslate our category message

    mCategoryMessage->setMessage( "<div align=center>"
                                  "    <p>"
                                  "        "+tr("Please select a CellML element...")
                                 +"    </p>"
                                  "</div>");

    // Retranslate our unsupported metadata message

    QString extraMessage = Core::FileManager::instance()->isReadableAndWritable(mCellmlFile->fileName())?
                               "    <p>"
                               "        <small><em>("+tr("Please click <a href=\"here\">here</a> if you want to remove the existing metadata.")+")</em></small>"
                               "    </p>":
                               QString();

    mUnsupportedMetadataMessage->setMessage( "<div align=center>"
                                             "    <p>"
                                             "        "+tr("Sorry, but the <strong>%1</strong> view does not support this type of metadata...").arg(mParent->pluginParent()->viewName())
                                            +"    </p>"
                                            +extraMessage
                                            +"</div>");
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::updateGui(iface::cellml_api::CellMLElement *pElement)
{
    // Keep track of the CellML element

    mElement = pElement;

    // Show/hide our category message

    mBorderedCategoryMessage->setVisible(!pElement);
    mSplitter->setVisible(pElement);

    // Show/hide our unsupported metadata message depending on whether the type
    // of the RDF triples is known

    bool isUnknownMetadata = pElement?
                                 mCellmlFile->rdfTriples(pElement).type() == CellMLSupport::CellmlFileRdfTriple::Unknown:
                                 true;

    mBorderedUnsupportedMetadataMessage->setVisible(pElement && isUnknownMetadata);

    // Show/hide our metadata edit details and web viewer, depending on whether
    // the type of the metadata is known

    mBorderedMetadataEditDetails->setVisible(pElement && !isUnknownMetadata);
    mBorderedWebView->setVisible(pElement && !isUnknownMetadata);

    mBorderedMetadataViewDetails->setTopBorderVisible(pElement && !isUnknownMetadata);
    mBorderedMetadataViewDetails->setBottomBorderVisible(pElement && !isUnknownMetadata);

    // Update our metadata edit and view details, if needed

    if (pElement && !isUnknownMetadata)
        mMetadataEditDetails->updateGui(pElement);

    if (pElement)
        mMetadataViewDetails->updateGui(pElement);
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::addRdfTriple(CellMLSupport::CellmlFileRdfTriple *pRdfTriple)
{
    if (!pRdfTriple)
        return;

    // Add the given RDF triple to our metadata view details

    mMetadataViewDetails->addRdfTriple(pRdfTriple);
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::emitSplitterMoved()
{
    // Let people know that our splitter has been moved

    emit splitterMoved(mSplitter->sizes());
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

void CellmlAnnotationViewMetadataDetailsWidget::removeAllMetadata()
{
    // Remove all the metadata from the current CellML element and ask our
    // details widget to update itself

    mCellmlFile->rdfTriples().remove(mElement);

    updateGui(mElement);
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::updateMetadataEditDetails()
{
    // Update our metadata edit details widget

    mMetadataEditDetails->updateGui(mElement);
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::fileReloaded()
{
    // Let our metadata edit details widget know that the file has been reloaded
    // Note: we don't need to let our metadata view details widget know about it
    //       since it only presents information to the user...

    mMetadataEditDetails->fileReloaded();
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
