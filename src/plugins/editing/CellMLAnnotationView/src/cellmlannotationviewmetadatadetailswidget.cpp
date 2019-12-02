/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// CellML Annotation view metadata details widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmlannotationvieweditingwidget.h"
#include "cellmlannotationviewmetadatadetailswidget.h"
#include "cellmlannotationviewmetadataeditdetailswidget.h"
#include "cellmlannotationviewmetadatanormalviewdetailswidget.h"
#include "cellmlannotationviewmetadataviewdetailswidget.h"
#include "cellmlannotationviewplugin.h"
#include "filemanager.h"
#include "progressbarwidget.h"
#include "usermessagewidget.h"
#include "webviewerwidget.h"

//==============================================================================

#include <QVBoxLayout>
#include <QWebView>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataDetailsWidget::CellmlAnnotationViewMetadataDetailsWidget(CellMLAnnotationViewPlugin *pPlugin,
                                                                                     CellmlAnnotationViewWidget *pAnnotationWidget,
                                                                                     CellmlAnnotationViewEditingWidget *pParent) :
    Widget(pParent),
    mPlugin(pPlugin),
    mCellmlFile(pParent->cellmlFile())
{
    // Create and set our vertical layout

    auto layout = new QVBoxLayout(this);

    layout->setContentsMargins(QMargins());
    layout->setSpacing(0);

    setLayout(layout);

    // Create and hide our category message widget

    mCategoryMessage = new Core::UserMessageWidget(":/oxygen/actions/help-hint.png", this);
    mBorderedCategoryMessage = new Core::BorderedWidget(mCategoryMessage,
                                                        false, true, false, false);

    mBorderedCategoryMessage->hide();

    // Create and hide our unsupported metadata message widget

    mUnsupportedMetadataMessage = new Core::UserMessageWidget(":/oxygen/actions/help-about.png", this);
    mBorderedUnsupportedMetadataMessage = new Core::BorderedWidget(mUnsupportedMetadataMessage,
                                                                   false, true, true, false);

    mBorderedUnsupportedMetadataMessage->hide();

    // Create our splitter widget

    mSplitter = new Core::SplitterWidget(Qt::Vertical, this);

    // Create our details widgets

    mMetadataEditDetails = new CellmlAnnotationViewMetadataEditDetailsWidget(pAnnotationWidget, pParent, mCellmlFile, mSplitter);
    mMetadataViewDetails = new CellmlAnnotationViewMetadataViewDetailsWidget(mCellmlFile, mSplitter);
    mWebViewer = new WebViewerWidget::WebViewerWidget(mSplitter);

    mWebViewer->showProgressBar();

    mWebViewer->webView()->setContextMenuPolicy(Qt::NoContextMenu);
    mWebViewer->setOverrideCursor(true);
    mWebViewer->setZoomingEnabled(false);

    mBorderedMetadataEditDetails = new Core::BorderedWidget(mMetadataEditDetails,
                                                            false, true, true, false);
    mBorderedMetadataViewDetails = new Core::BorderedWidget(mMetadataViewDetails,
                                                            true, true, true, false);
    mBorderedWebViewer = new Core::BorderedWidget(mWebViewer,
                                                  true, true, false, false);

    // Some connections to handle the looking up of a qualifier from our
    // metadata edit details view, as well as the disabling of information look
    // up for the metadata view details widget

    connect(mMetadataEditDetails, &CellmlAnnotationViewMetadataEditDetailsWidget::qualifierLookUpRequested,
            mMetadataViewDetails->normalView(), &CellmlAnnotationViewMetadataNormalViewDetailsWidget::disableLookUpInformation);
    connect(mMetadataEditDetails, &CellmlAnnotationViewMetadataEditDetailsWidget::qualifierLookUpRequested,
            this, &CellmlAnnotationViewMetadataDetailsWidget::lookUpQualifier);

    connect(mMetadataEditDetails, &CellmlAnnotationViewMetadataEditDetailsWidget::resourceLookUpRequested,
            mMetadataViewDetails->normalView(), &CellmlAnnotationViewMetadataNormalViewDetailsWidget::disableLookUpInformation);
    connect(mMetadataEditDetails, &CellmlAnnotationViewMetadataEditDetailsWidget::resourceLookUpRequested,
            this, &CellmlAnnotationViewMetadataDetailsWidget::lookUpResource);

    connect(mMetadataEditDetails, &CellmlAnnotationViewMetadataEditDetailsWidget::idLookUpRequested,
            mMetadataViewDetails->normalView(), &CellmlAnnotationViewMetadataNormalViewDetailsWidget::disableLookUpInformation);
    connect(mMetadataEditDetails, &CellmlAnnotationViewMetadataEditDetailsWidget::idLookUpRequested,
            this, &CellmlAnnotationViewMetadataDetailsWidget::lookUpId);

    connect(mMetadataEditDetails, &CellmlAnnotationViewMetadataEditDetailsWidget::noLookUpRequested,
            mMetadataViewDetails->normalView(), &CellmlAnnotationViewMetadataNormalViewDetailsWidget::disableLookUpInformation);
    connect(mMetadataEditDetails, &CellmlAnnotationViewMetadataEditDetailsWidget::noLookUpRequested,
            this, &CellmlAnnotationViewMetadataDetailsWidget::lookUpNothing);

    // Some connections to handle the looking up of a qualifier, resource and
    // resource id from our normal view, as well as the disabling of information
    // look up for the metadata edit details widget

    connect(mMetadataViewDetails->normalView(), &CellmlAnnotationViewMetadataNormalViewDetailsWidget::qualifierLookUpRequested,
            mMetadataEditDetails, &CellmlAnnotationViewMetadataEditDetailsWidget::disableLookUpInformation);
    connect(mMetadataViewDetails->normalView(), &CellmlAnnotationViewMetadataNormalViewDetailsWidget::qualifierLookUpRequested,
            this, &CellmlAnnotationViewMetadataDetailsWidget::lookUpQualifier);

    connect(mMetadataViewDetails->normalView(), &CellmlAnnotationViewMetadataNormalViewDetailsWidget::resourceLookUpRequested,
            mMetadataEditDetails, &CellmlAnnotationViewMetadataEditDetailsWidget::disableLookUpInformation);
    connect(mMetadataViewDetails->normalView(), &CellmlAnnotationViewMetadataNormalViewDetailsWidget::resourceLookUpRequested,
            this, &CellmlAnnotationViewMetadataDetailsWidget::lookUpResource);

    connect(mMetadataViewDetails->normalView(), &CellmlAnnotationViewMetadataNormalViewDetailsWidget::idLookUpRequested,
            mMetadataEditDetails, &CellmlAnnotationViewMetadataEditDetailsWidget::disableLookUpInformation);
    connect(mMetadataViewDetails->normalView(), &CellmlAnnotationViewMetadataNormalViewDetailsWidget::idLookUpRequested,
            this, &CellmlAnnotationViewMetadataDetailsWidget::lookUpId);

    connect(mMetadataViewDetails->normalView(), &CellmlAnnotationViewMetadataNormalViewDetailsWidget::noLookUpRequested,
            mMetadataEditDetails, &CellmlAnnotationViewMetadataEditDetailsWidget::disableLookUpInformation);
    connect(mMetadataViewDetails->normalView(), &CellmlAnnotationViewMetadataNormalViewDetailsWidget::noLookUpRequested,
            this, &CellmlAnnotationViewMetadataDetailsWidget::lookUpNothing);

    // A connection to handle the removal of an RDF triple from our normal view
    // and to have our metadata edit details widget update itself with regards
    // to possible terms (i.e. enable/disable add buttons)

    connect(mMetadataViewDetails->normalView(), &CellmlAnnotationViewMetadataNormalViewDetailsWidget::rdfTripleRemoved,
            this, &CellmlAnnotationViewMetadataDetailsWidget::updateMetadataEditDetails);

    // A connection to handle the clicking of the link in the unsupported
    // message

    connect(mUnsupportedMetadataMessage, &Core::UserMessageWidget::linkActivated,
            this, &CellmlAnnotationViewMetadataDetailsWidget::removeAllMetadata);

    // Populate our splitter widget

    mSplitter->addWidget(mBorderedMetadataEditDetails);
    mSplitter->addWidget(mBorderedMetadataViewDetails);
    mSplitter->addWidget(mBorderedWebViewer);

    // Keep track of our splitter being moved

    connect(mSplitter, &Core::SplitterWidget::splitterMoved,
            this, &CellmlAnnotationViewMetadataDetailsWidget::emitSplitterMoved);

    // Add our unsupported metadata message widget and splitter widget to our
    // layout

    layout->addWidget(mBorderedCategoryMessage);
    layout->addWidget(mBorderedUnsupportedMetadataMessage);
    layout->addWidget(mSplitter);

    // Some further initialisations that are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mMetadataEditDetails->retranslateUi();
    mMetadataViewDetails->retranslateUi();

    // Retranslate our category message

    mCategoryMessage->setMessage(tr("Select a CellML element..."));

    // Retranslate our unsupported metadata message

    retranslateUnsupportedMetadataMessage();
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::retranslateUnsupportedMetadataMessage()
{
    // Retranslate our unsupported metadata message

    mUnsupportedMetadataMessage->setMessage(tr("The <strong>%1</strong> view does not support this type of metadata...").arg(mPlugin->viewName()),
                                            Core::FileManager::instance()->isReadableAndWritable(mCellmlFile->fileName())?
                                                tr(R"(Click <a href="here">here</a> if you want to remove the existing metadata.)"):
                                                QString());
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::updateGui(iface::cellml_api::CellMLElement *pElement)
{
    // Keep track of the CellML element

    mElement = pElement;

    // Show/hide our category message

    mBorderedCategoryMessage->setVisible(pElement == nullptr);
    mSplitter->setVisible(pElement != nullptr);

    // Show/hide our unsupported metadata message depending on whether the type
    // of the RDF triples is known

    bool isUnknownMetadata = (pElement != nullptr)?
                                 mCellmlFile->rdfTriples(pElement).type() == CellMLSupport::CellmlFileRdfTriple::Type::Unknown:
                                 true;

    mBorderedUnsupportedMetadataMessage->setVisible((pElement != nullptr) && isUnknownMetadata);

    // Show/hide our metadata edit details and Web viewer, depending on whether
    // the type of the metadata is known

    mBorderedMetadataEditDetails->setVisible((pElement != nullptr) && !isUnknownMetadata);
    mBorderedWebViewer->setVisible((pElement != nullptr) && !isUnknownMetadata);

    mBorderedMetadataViewDetails->setTopBorderVisible((pElement != nullptr) && !isUnknownMetadata);
    mBorderedMetadataViewDetails->setBottomBorderVisible((pElement != nullptr) && !isUnknownMetadata);

    // Update our metadata edit and view details, if needed

    if ((pElement != nullptr) && !isUnknownMetadata) {
        mMetadataEditDetails->updateGui(pElement);
    }

    if (pElement != nullptr) {
        mMetadataViewDetails->updateGui(pElement);
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::emitSplitterMoved()
{
    // Let people know that our splitter has been moved

    emit splitterMoved(mSplitter->sizes());
}

//==============================================================================

Core::SplitterWidget * CellmlAnnotationViewMetadataDetailsWidget::splitter() const
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

void CellmlAnnotationViewMetadataDetailsWidget::lookUpQualifier(const QString &pQualifier)
{
    // Let people know that we want our Web viewer to be updated with some
    // details about the given qualifier

    mWebViewer->progressBarWidget()->setEnabled(true);

    emit qualifierDetailsRequested(mWebViewer, pQualifier);
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::lookUpResource(const QString &pResource)
{
    // Let people know that we want our Web viewer to be updated with some
    // details about the given resource

    mWebViewer->progressBarWidget()->setEnabled(true);

    emit resourceDetailsRequested(mWebViewer, pResource);
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::lookUpId(const QString &pResource,
                                                         const QString &pId)
{
    // Let people know that we want our Web viewer to be updated with some
    // details about the given id

    mWebViewer->progressBarWidget()->setEnabled(true);

    emit idDetailsRequested(mWebViewer, pResource, pId);
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::lookUpNothing()
{
    // We are 'asked' to look nothing up, so 'clean up' our Web viewer by going
    // to our home page (i.e. blank page)

    mWebViewer->progressBarWidget()->setEnabled(false);

    mWebViewer->goToHomePage();
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

void CellmlAnnotationViewMetadataDetailsWidget::filePermissionsChanged()
{
    // Retranslate our unsupported metadata message since part of it depends on
    // whether the file is un/locked

    retranslateUnsupportedMetadataMessage();

    // Let our metadata edit and view details widgets know that the file has
    // been un/locked

    mMetadataEditDetails->filePermissionsChanged();
    mMetadataViewDetails->filePermissionsChanged();
}

//==============================================================================

} // namespace CellMLAnnotationView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
