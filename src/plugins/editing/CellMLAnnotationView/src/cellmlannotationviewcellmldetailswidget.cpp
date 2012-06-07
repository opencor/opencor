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
    mWebViewStatus(Empty),
    mMiriamUrn(QString())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our details widgets

    mCellmlElementDetails = new CellmlAnnotationViewCellmlElementDetailsWidget(pParent);
    mMetadataViewDetails  = new CellmlAnnotationViewMetadataViewDetailsWidget(pParent);
    mWebView              = new QWebView(pParent);

    // A connection to handle the status bar

    connect(mMetadataViewDetails->bioModelsDotNetView(), SIGNAL(miriamUrnLookupRequested(const QString &)),
            this, SLOT(miriamUrnLookupRequested(const QString &)));

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

    // Retrieve the list of models in the CellML Model Repository as JSON code
    // from http://models.cellml.org/workspace/rest/contents.json

    mNetworkAccessManager = new QNetworkAccessManager(this);

    // Make sure that we get told when the download of our Internet file is
    // complete

    connect(mNetworkAccessManager, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(miriamUrnDownloadFinished(QNetworkReply *)) );

    // Retrieve the output template

    QFile cellmlAnnotationViewWidgetErrorFile(":cellmlAnnotationViewWidgetError");

    cellmlAnnotationViewWidgetErrorFile.open(QIODevice::ReadOnly);

    mErrorMsgTemplate = QString(cellmlAnnotationViewWidgetErrorFile.readAll());

    cellmlAnnotationViewWidgetErrorFile.close();
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

    updateWebView();
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

    mWebViewStatus = Empty;

    updateWebView();

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
    // metadata details

    if (pCmetaIdValue.isEmpty())
        // The CellML element doesn't have a cmeta:id value, so we don't want
        // any metadata to be shown for it (not even the model-wide metadata)

        mMetadataViewDetails->updateGui();
    else
        mMetadataViewDetails->updateGui(mParent->rdfTriples(pCmetaIdValue));
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::miriamUrnLookupRequested(const QString &pMiriamUrn)
{
    // The user requested a MIRIAM URN to be looked up, so...

    mMiriamUrn = pMiriamUrn;

    QNetworkRequest networkRequest = QNetworkRequest(QUrl("http://www.ebi.ac.uk/miriamws/main/rest/resolve/"+pMiriamUrn));

    networkRequest.setRawHeader("Accept", "application/json");

    mNetworkAccessManager->get(networkRequest);
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::miriamUrnDownloadFinished(QNetworkReply *pNetworkReply)
{
    // Output the list of models, should we have retrieved it without any
    // problem

    if (pNetworkReply->error() == QNetworkReply::NoError) {
        // We should be getting some JSON code which we can then be used to
        // retrieve the first URL associated with the MIRIAM URN

        QJson::Parser jsonParser;
        bool parsingOk;

        QVariantMap res = jsonParser.parse(pNetworkReply->readAll(), &parsingOk).toMap();

        if (parsingOk) {
            // The parsing went fine, so try to retrieve the first corresponding
            // URL

            QUrl miriamUrl = QUrl();

            QVariant uri = res["uri"];
            QVariantList uriList;

            if (uri.type() == QVariant::Map)
                // Case where there is only one URI item

                uriList << QVariantList() << uri;
            else
                // Case where there is more than one URI item

                uriList = uri.toList();

            // Go through our different URI items and see whether one of them
            // contains a non-deprecated URL and, if so, then use it

            foreach (const QVariant &uriItem, uriList) {
                QVariantMap uriItemProperties = uriItem.toMap();

                // There are up to three possible properties, the first of which
                // is optional and tell us whether the URI item is deprecated or
                // not

                QVariant deprecated = uriItemProperties["@deprecated"];

                if (   (deprecated.type() == QVariant::String)
                    && !deprecated.toString().compare("true"))
                    // The URI item is deprecated, so...

                    continue;

                // Next, we should have the type of the URI item which should be
                // a URL

                QVariant type = uriItemProperties["@type"];

                if (   (type.type() != QVariant::String)
                    || type.toString().compare("URL"))
                    // The URI item is not a URL, so...

                    continue;

                // Finally, we should have the URL itself

                QVariant url = uriItemProperties["$"];

                if (url.type() == QVariant::String) {
                    // This is our first non-deprecated URL, so...

                    miriamUrl.setUrl(url.toString());

                    break;
                }
            }

            // Make sure that we have found a corresponding URL

            if (miriamUrl.isEmpty())
                // We haven't found any corresponding URL, so...

                mWebViewStatus = NoCorrespondingUrl;
            else
                // We found a corresponding URL, so look it up...

                mWebViewStatus = WebPage;

                mWebView->setUrl(miriamUrl);
        } else {
            mWebViewStatus = FailedResolution;
        }
    } else {
        // Something went wrong, so...

        mWebViewStatus = ProblemOccurred;
    }

    // Update our Web view

    updateWebView();
}

//==============================================================================

void CellmlAnnotationViewCellmlDetailsWidget::updateWebView() const
{
    QString errorMsg = QString();

    switch (mWebViewStatus) {
    case WebPage:
        // Nothing to do...

        break;
    case NoCorrespondingUrl:
        errorMsg = tr("No corresponding URL could be found for the MIRIAM URN (%1).").arg(mMiriamUrn);

        break;
    case FailedResolution:
        errorMsg = tr("The resolution of the MIRIAM URN failed (%1).").arg(mMiriamUrn);

        break;
    case ProblemOccurred:
        errorMsg = tr("A problem occurred during the resolution of the MIRIAM URN (%1).").arg(mMiriamUrn);

        break;
    default:
        // Empty

        mWebView->setUrl(QUrl());
    }

    // Check whether we have an error message to show

    if (!errorMsg.isEmpty())
        mWebView->setHtml(mErrorMsgTemplate.arg(tr("Error"),
                                                errorMsg,
                                                tr("Please contact the <a href=\"http://www.ebi.ac.uk/miriam/main/mdb?section=contact\">MIRIAM people</a> about this error."),
                                                tr("Copyright")+" ©2011-2012"));
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
