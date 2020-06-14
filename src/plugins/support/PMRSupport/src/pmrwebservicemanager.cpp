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
// PMR Web service manager
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "pmrauthentication.h"
#include "pmrsupportplugin.h"
#include "pmrsupportpreferenceswidget.h"
#include "pmrwebservice.h"
#include "pmrwebservicemanager.h"
#include "pmrwebserviceresponse.h"
#include "pmrwebviewerdialog.h"
#include "progressbarwidget.h"
#include "webviewerwidget.h"

//==============================================================================

#include <QLayout>
#include <QMainWindow>
#include <QTimer>

//==============================================================================

#include "oauthbegin.h"
    #include "o2/o1requestor.h"
#include "oauthend.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

PmrWebServiceManager::PmrWebServiceManager(const QString &pPmrUrl,
                                           PmrWebService *pPmrWebService) :
    QNetworkAccessManager(pPmrWebService),
    mPmrWebService(pPmrWebService)
{
    // Make sure that we get told when there are SSL errors (which would happen
    // if the website's certificate is invalid, e.g. it has expired)

    connect(this, &PmrWebServiceManager::sslErrors,
            this, &PmrWebServiceManager::ignoreSslErrors);

    // Create, by updating ourselves, our PMR authentication object

    update(pPmrUrl);
}

//==============================================================================

bool PmrWebServiceManager::isAuthenticated() const
{
    // Return whether we are authenticated to PMR

    return mPmrAuthentication->linked();
}

//==============================================================================

void PmrWebServiceManager::authenticate(bool pAuthenticate)
{
    // Authenticate ourselves to PMR

    mWebViewerDialogUsed = false;

    if (pAuthenticate) {
        mPmrAuthentication->link();
    } else {
        mPmrAuthentication->unlink();
    }
}

//==============================================================================

void PmrWebServiceManager::authenticationSucceeded()
{
    // Let people know whether we are really authenticated, i.e. linked

    emit authenticated(qobject_cast<PmrAuthentication *>(sender())->linked());
}

//==============================================================================

void PmrWebServiceManager::authenticationFailed()
{
    // Let the user know if s/he tried to authenticate him/herself but was just
    // not able to do so

    if (!mWebViewerDialogUsed) {
        Core::warningMessageBox(tr("PMR Authentication"),
                                 tr("PMR Authentication could not be performed.")+"<br/><br/>"
                                +tr("<strong>Note:</strong> you might want to check that your system time is correct."));
    }

    // Let people know that authentication failed

    emit authenticated(false);
}

//==============================================================================

void PmrWebServiceManager::openBrowser(const QUrl &pUrl)
{
    // Open the given URL in our Web browser

    mWebViewerDialogUsed = true;

    if (mWebViewerDialog == nullptr) {
        mWebViewerDialog = new PmrWebViewerDialog(Core::mainWindow());

        connect(mWebViewerDialog, &Core::Dialog::rejected,
                this, &PmrWebServiceManager::authenticationCancelled);
    } else {
        mWebViewerDialog->load({});
        // Note: this is in case we used our Web brower before, in which case it
        //       will contain something, which we don't want to see when
        //       reopening it...

        mWebViewerDialog->retranslateUi();
    }

    mWebViewerDialog->load(pUrl);

    mWebViewerDialog->exec();
}

//==============================================================================

void PmrWebServiceManager::startCloseBrowser()
{
    // Close our temporary Web browser after a slight delay
    // Note: the whole idea is that granting/denying access resuts in a new
    //       (empty) page being loaded, so we want to wait until that page is
    //       loaded before actually closing our temporary Web browser. So, here,
    //       we have a short delay to give time to our temporary Web browser to
    //       start loading that new (empty) page. Then, in closeBrowser(), we
    //       checking whether the loading is finished and, if so, close our
    //       temporary Web browser otherwise check again in a bit...

    QTimer::singleShot(169, this, &PmrWebServiceManager::closeBrowser);
}

//==============================================================================

void PmrWebServiceManager::closeBrowser()
{
    // Close our temporary Web browser, but only if the current page has
    // finished loading otherwise try again in a bit

    if (mWebViewerDialog->isLoadFinished()) {
        mWebViewerDialog->close();
    } else {
        QTimer::singleShot(169, this, &PmrWebServiceManager::closeBrowser);
    }
}

//==============================================================================

void PmrWebServiceManager::ignoreSslErrors(QNetworkReply *pNetworkReply,
                                           const QList<QSslError> &pSslErrors)
{
    // Ignore SSL errors since we trust the website and therefore its
    // certificate (even if it is invalid, e.g. it has expired)

    pNetworkReply->ignoreSslErrors(pSslErrors);
}

//==============================================================================

PmrWebServiceResponse * PmrWebServiceManager::request(const QString &pUrl,
                                                      bool pSecureRequest,
                                                      bool pUsePost,
                                                      const QJsonDocument &pJsonDocument)
{
    // Check that we are connected to the Internet

    if (!Core::hasInternetConnection()) {
        emit error(Core::noInternetConnectionMessage());

        return nullptr;
    }

    // Let the user know that we are (going to be) busy

    emit busy(true);

    // Send our request to PMR, asking for the response to be compressed

    QNetworkRequest networkRequest;

    networkRequest.setRawHeader("Accept", "application/json");
    networkRequest.setRawHeader("Accept-Encoding", "gzip");

    networkRequest.setUrl(QUrl(pUrl));

    // Use the authenticated link if it's available

    QNetworkReply *networkReply;

    if (pSecureRequest && mPmrAuthentication->linked()) {
        auto requestor = new O1Requestor(this, mPmrAuthentication, this);

        if (!pUsePost && pJsonDocument.isEmpty()) {
            networkReply = requestor->get(networkRequest, {});
        } else {
            networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, RequestMimeType);

            networkReply = requestor->post(networkRequest, {},
                                           pJsonDocument.isNull()?
                                               QByteArray():
                                               pJsonDocument.toJson(QJsonDocument::Compact));
        }
    } else {
        if (!pUsePost && pJsonDocument.isEmpty()) {
            networkReply = get(networkRequest);
        } else {
            networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, RequestMimeType);

            networkReply = post(networkRequest,
                                pJsonDocument.isNull()?
                                    QByteArray():
                                    pJsonDocument.toJson(QJsonDocument::Compact));
        }
    }

    auto pmrWebServiceResponse = new PmrWebServiceResponse(networkReply);

    connect(pmrWebServiceResponse, &PmrWebServiceResponse::busy,
            mPmrWebService, &PmrWebService::busy);
    connect(pmrWebServiceResponse, &PmrWebServiceResponse::error,
            mPmrWebService, &PmrWebService::error);
    connect(pmrWebServiceResponse, &PmrWebServiceResponse::forbidden,
            mPmrWebService, &PmrWebService::forbidden);

    return pmrWebServiceResponse;
}

//==============================================================================

void PmrWebServiceManager::update(const QString &pPmrUrl)
{
    // Delete any previous instance of our PMR authentication object

    delete mPmrAuthentication;

    // Create our PMR authentication object

    mPmrAuthentication = new PmrAuthentication(pPmrUrl, this);

    // Connect some signals

    connect(mPmrAuthentication, &PmrAuthentication::linkingSucceeded,
            this, &PmrWebServiceManager::authenticationSucceeded);
    connect(mPmrAuthentication, &PmrAuthentication::linkingFailed,
            this, &PmrWebServiceManager::authenticationFailed);

    connect(mPmrAuthentication, &PmrAuthentication::openBrowser,
            this, &PmrWebServiceManager::openBrowser);
    connect(mPmrAuthentication, &PmrAuthentication::closeBrowser,
            this, &PmrWebServiceManager::startCloseBrowser);
}

//==============================================================================

} // namespace PMRSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
