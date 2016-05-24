/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// PMR authentication
//==============================================================================

#include "corecliutils.h"
#include "pmrauthentication.h"

//==============================================================================

#include "o0globals.h"
#include "o1requestor.h"
#include "o0settingsstore.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QDesktopServices>

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

PmrOAuthClient::PmrOAuthClient(QObject *parent) : O1(parent)
{
    setClientId(PmrOAuthClient::ConsumerKey);
    setClientSecret(PmrOAuthClient::ConsumerSecret);

    setCallbackUrl(PmrOAuthClient::CallbackUrl);
    setLocalPort(PmrOAuthClient::CallbackPort);

    // Set the urls used to get authentication tokens

    setAccessTokenUrl(QUrl(QString(AccessTokenUrl).arg(Url)));
    setAuthorizeUrl(QUrl(QString(AuthorizeUrl).arg(Url)));
    setRequestTokenUrl(QUrl(QString(RequestTokenUrl).arg(Url)));

    // Specify the scope of token requests

    QList<O0RequestParameter> reqParams = QList<O0RequestParameter>();
    reqParams << O0RequestParameter(QByteArray("scope"),
                                    QString(RequestScope).arg(Url).toUtf8());
    setRequestParameters(reqParams);

    // Create a store object for writing the received authentication tokens

    O0SettingsStore *store = new O0SettingsStore(EncryptionKey);
    store->setGroupKey("PhysiomeModelRepository");
    setStore(store);
}

//==============================================================================

PmrAuthentication::PmrAuthentication(QObject *parent) : QObject(parent)
{
}

//==============================================================================

void PmrAuthentication::doOAuth()
{
    // Create an OAuth client for the Physiome Model Repository

    mPmrOAuthClient = new PmrOAuthClient(this);

    // Connect signals
    connect(mPmrOAuthClient, SIGNAL(linkedChanged()), this, SLOT(onLinkedChanged()));
    connect(mPmrOAuthClient, SIGNAL(linkingFailed()), this, SIGNAL(linkingFailed()));
    connect(mPmrOAuthClient, SIGNAL(linkingSucceeded()), this, SLOT(onLinkingSucceeded()));
    connect(mPmrOAuthClient, SIGNAL(openBrowser(QUrl)), this, SLOT(onOpenBrowser(QUrl)));
    connect(mPmrOAuthClient, SIGNAL(closeBrowser()), this, SLOT(onCloseBrowser()));

    qWarning() << "Starting OAuth...";

    mPmrOAuthClient->unlink();  // For the sake of this demo
    mPmrOAuthClient->link();
}

//==============================================================================

void PmrAuthentication::onOpenBrowser(const QUrl &url) {
    qWarning() << "Opening browser with URL" << url.toString();
    QDesktopServices::openUrl(url);
}

//==============================================================================

void PmrAuthentication::onCloseBrowser() {
    qWarning() << "Browser closed...";
}

//==============================================================================

void PmrAuthentication::onLinkedChanged() {
    qWarning() << "Linked changed!";
}

//==============================================================================

void PmrAuthentication::onLinkingSucceeded() {
    qWarning() << "Linking succeeded...";
    PmrOAuthClient *o1t = qobject_cast<PmrOAuthClient *>(sender());
    if (!o1t->linked()) {
        return;
    }
    QVariantMap extraTokens = o1t->extraTokens();
    if (!extraTokens.isEmpty()) {
        emit extraTokensReady(extraTokens);
        qWarning() << "Extra tokens in response:";
        foreach (QString key, extraTokens.keys()) {
            qWarning() << "\t" << key << ":" << (extraTokens.value(key).toString().left(3) + "...");
        }
    }
    emit linkingSucceeded();
}

//==============================================================================

void PmrAuthentication::getWorkspaceList(void) {
    qWarning() << "Workspace list";
    if (!mPmrOAuthClient->linked()) {
        qWarning() << "Application is not linked to PMR!";
        emit gotWorkspaces();
        return;
    }

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    PmrOAuthClient* o1 = mPmrOAuthClient;
    O1Requestor* requestor = new O1Requestor(manager, o1, this);

    QNetworkRequest networkRequest;

    networkRequest.setRawHeader("Accept", "application/vnd.physiome.pmr2.json.1");
//    networkRequest.setRawHeader("Accept-Encoding", "gzip");  // *************

    networkRequest.setUrl(QUrl(QString("%1/my-workspaces").arg(PmrOAuthClient::Url)));

    QNetworkReply *networkReply = requestor->get(networkRequest, QList<O0RequestParameter>());

    connect(networkReply, SIGNAL(finished()), this, SLOT(gotWorkspaceList()));
    qWarning() << "Requesting workspaces. Please wait...";
}

//==============================================================================

void PmrAuthentication::gotWorkspaceList(void) {
    qWarning() << "Reply done...";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qWarning() << "ERROR:" << reply->errorString();
    qWarning() << "Content:" << reply->readAll();
    emit gotWorkspaces();
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
