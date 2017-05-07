/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// CellML Annotation view metadata details widget
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
#include "progressbarwidget.h"
#include "webviewerwidget.h"

//==============================================================================

#include <QVBoxLayout>
#include <QWebView>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataDetailsWidget::CellmlAnnotationViewMetadataDetailsWidget(CellMLAnnotationViewPlugin *pPlugin,
                                                                                     CellmlAnnotationViewWidget *pViewWidget,
                                                                                     CellmlAnnotationViewEditingWidget *pViewEditingWidget,
                                                                                     CellMLSupport::CellmlFile *pCellmlFile,
                                                                                     QWidget *pParent) :
    Widget(pParent),
    mPlugin(pPlugin),
    mCellmlFile(pCellmlFile),
    mElement(0)
{
    // Create and set our vertical layout

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->setContentsMargins(QMargins());
    layout->setSpacing(0);

    setLayout(layout);

    // Create and hide our category message widget

    mCategoryMessage = new Core::UserMessageWidget(":/oxygen/actions/help-hint.png", this);
    mBorderedCategoryMessage = new Core::BorderedWidget(mCategoryMessage,
                                                        false, true, false, false);

    mBorderedCategoryMessage->setVisible(false);

    // Create and hide our unsupported metadata message widget

    mUnsupportedMetadataMessage = new Core::UserMessageWidget(":/oxygen/actions/help-about.png", this);
    mBorderedUnsupportedMetadataMessage = new Core::BorderedWidget(mUnsupportedMetadataMessage,
                                                                   false, true, true, false);

    mBorderedUnsupportedMetadataMessage->setVisible(false);

    // Create our splitter widget

    mSplitter = new Core::SplitterWidget(Qt::Vertical, this);

    // Create our details widgets

    mMetadataEditDetails = new CellmlAnnotationViewMetadataEditDetailsWidget(pViewWidget, pViewEditingWidget, pCellmlFile, mSplitter);
    mMetadataViewDetails = new CellmlAnnotationViewMetadataViewDetailsWidget(pCellmlFile, mSplitter);
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

    connect(mMetadataEditDetails, SIGNAL(qualifierLookUpRequested(const QString &)),
            mMetadataViewDetails->normalView(), SLOT(disableLookUpInformation()));
    connect(mMetadataEditDetails, SIGNAL(qualifierLookUpRequested(const QString &)),
            this, SLOT(lookUpQualifier(const QString &)));

    connect(mMetadataEditDetails, SIGNAL(resourceLookUpRequested(const QString &)),
            mMetadataViewDetails->normalView(), SLOT(disableLookUpInformation()));
    connect(mMetadataEditDetails, SIGNAL(resourceLookUpRequested(const QString &)),
            this, SLOT(lookUpResource(const QString &)));

    connect(mMetadataEditDetails, SIGNAL(idLookUpRequested(const QString &, const QString &)),
            mMetadataViewDetails->normalView(), SLOT(disableLookUpInformation()));
    connect(mMetadataEditDetails, SIGNAL(idLookUpRequested(const QString &, const QString &)),
            this, SLOT(lookUpId(const QString &, const QString &)));

    connect(mMetadataEditDetails, SIGNAL(noLookUpRequested()),
            mMetadataViewDetails->normalView(), SLOT(disableLookUpInformation()));
    connect(mMetadataEditDetails, SIGNAL(noLookUpRequested()),
            this, SLOT(lookUpNothing()));

    // Some connections to handle the looking up of a qualifier, resource and
    // resource id from our normal view, as well as the disabling of information
    // look up for the metadata edit details widget

    connect(mMetadataViewDetails->normalView(), SIGNAL(qualifierLookUpRequested(const QString &)),
            mMetadataEditDetails, SLOT(disableLookUpInformation()));
    connect(mMetadataViewDetails->normalView(), SIGNAL(qualifierLookUpRequested(const QString &)),
            this, SLOT(lookUpQualifier(const QString &)));

    connect(mMetadataViewDetails->normalView(), SIGNAL(resourceLookUpRequested(const QString &)),
            mMetadataEditDetails, SLOT(disableLookUpInformation()));
    connect(mMetadataViewDetails->normalView(), SIGNAL(resourceLookUpRequested(const QString &)),
            this, SLOT(lookUpResource(const QString &)));

    connect(mMetadataViewDetails->normalView(), SIGNAL(idLookUpRequested(const QString &, const QString &)),
            mMetadataEditDetails, SLOT(disableLookUpInformation()));
    connect(mMetadataViewDetails->normalView(), SIGNAL(idLookUpRequested(const QString &, const QString &)),
            this, SLOT(lookUpId(const QString &, const QString &)));

    connect(mMetadataViewDetails->normalView(), SIGNAL(noLookUpRequested()),
            mMetadataEditDetails, SLOT(disableLookUpInformation()));
    connect(mMetadataViewDetails->normalView(), SIGNAL(noLookUpRequested()),
            this, SLOT(lookUpNothing()));

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
    mSplitter->addWidget(mBorderedWebViewer);

    // Keep track of our splitter being moved

    connect(mSplitter, SIGNAL(splitterMoved(int, int)),
            this, SLOT(emitSplitterMoved()));

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
                                                tr("Click <a href=\"here\">here</a> if you want to remove the existing metadata."):
                                                QString());
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
    mBorderedWebViewer->setVisible(pElement && !isUnknownMetadata);

    mBorderedMetadataViewDetails->setTopBorderVisible(pElement && !isUnknownMetadata);
    mBorderedMetadataViewDetails->setBottomBorderVisible(pElement && !isUnknownMetadata);

    // Update our metadata edit and view details, if needed

    if (pElement && !isUnknownMetadata)
        mMetadataEditDetails->updateGui(pElement);

    if (pElement)
        mMetadataViewDetails->updateGui(pElement);
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
    // Let people know that we want our web viewer to be updated with some
    // details about the given qualifier

    mWebViewer->progressBarWidget()->setEnabled(true);

    emit qualifierDetailsRequested(mWebViewer, pQualifier);
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::lookUpResource(const QString &pResource)
{
    // Let people know that we want our web viewer to be updated with some
    // details about the given resource

    mWebViewer->progressBarWidget()->setEnabled(true);

    emit resourceDetailsRequested(mWebViewer, pResource);
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::lookUpId(const QString &pResource,
                                                         const QString &pId)
{
    // Let people know that we want our web viewer to be updated with some
    // details about the given id

    mWebViewer->progressBarWidget()->setEnabled(true);

    emit idDetailsRequested(mWebViewer, pResource, pId);
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::lookUpNothing()
{
    // We are 'asked' to look nothing up, so 'clean up' our web viewer by going
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

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
