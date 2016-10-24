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
#include "pmrwebservice.h"

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

PmrOAuthClient::PmrOAuthClient(const QString &pUrl, QObject *pParent) :
    O1(pParent)
{
    setClientId(ConsumerKey());
    setClientSecret(ConsumerSecret());

    setCallbackUrl(PmrOAuthClient::CallbackUrl());
    setLocalPort(PmrOAuthClient::CallbackPort());

    // Set the urls used to get authentication tokens

    setAccessTokenUrl(QUrl(AccessTokenUrl().arg(pUrl)));
    setAuthorizeUrl(QUrl(AuthorizeUrl().arg(pUrl)));
    setRequestTokenUrl(QUrl(RequestTokenUrl().arg(pUrl)));

    // Specify the scope of token requests

    QList<O0RequestParameter> reqParams = QList<O0RequestParameter>();
    reqParams << O0RequestParameter(QByteArray("scope"),
                                    RequestScope().arg(pUrl).toUtf8());
    setRequestParameters(reqParams);

    // Create a store object for writing the received authentication tokens

    O0SettingsStore *store = new O0SettingsStore(EncryptionKey());
    store->setGroupKey(PmrWebService::Url().toUtf8().constData());
    setStore(store);
}

//==============================================================================

const char *PmrOAuthClient::CallbackUrl()
{
    static const char *callbackUrl = "http://localhost:%1/";

    return callbackUrl;
}

//==============================================================================

int PmrOAuthClient::CallbackPort()
{
    return 1234;
}

//==============================================================================

const QString &PmrOAuthClient::AccessTokenUrl()
{
    static const QString accessTokenUrl = QString("%1/OAuthGetAccessToken");

    return accessTokenUrl;
}

//==============================================================================

const QString &PmrOAuthClient::AuthorizeUrl()
{
    static const QString authorizeUrl = QString("%1/OAuthAuthorizeToken");

    return authorizeUrl;
}

//==============================================================================

const QString &PmrOAuthClient::RequestTokenUrl()
{
    static const QString requestTokenUrl = QString("%1/OAuthRequestToken");

    return requestTokenUrl;
}

//==============================================================================

const QString &PmrOAuthClient::RequestScope()
{
    static const QString requestScope = QString("%1/oauth_scope/collection,"
                                                "%1/oauth_scope/search,"
                                                "%1/oauth_scope/workspace_tempauth,"
                                                "%1/oauth_scope/workspace_full");
    return requestScope;
}

//==============================================================================

const char *PmrOAuthClient::ConsumerKey()
{
    static const char *consumerKey = "9uuENvnELA1cdoy7WhFg4Gsq";

    return consumerKey;
}

//==============================================================================

const char *PmrOAuthClient::ConsumerSecret()
{
    static const char *consumerSecret = "IqCnqYJZmYzVbFmMsRE_C66F";

    return consumerSecret;
}

//==============================================================================

const char *PmrOAuthClient::EncryptionKey()
{
    // Used to encrypt saved access tokens

    static const char *encryptionKey = "hgh189;;099!@7878";

    return encryptionKey;
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
