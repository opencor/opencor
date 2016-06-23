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
#include "pmrrepository.h"

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

PmrOAuthClient::PmrOAuthClient(const QString &pUrl, QObject *parent) : O1(parent)
{
    setClientId(ConsumerKey());
    setClientSecret(ConsumerSecret());

    setCallbackUrl(PmrOAuthClient::CallbackUrl);
    setLocalPort(PmrOAuthClient::CallbackPort);

    // Set the urls used to get authentication tokens

    setAccessTokenUrl(QUrl(QString(AccessTokenUrl).arg(pUrl)));
    setAuthorizeUrl(QUrl(QString(AuthorizeUrl).arg(pUrl)));
    setRequestTokenUrl(QUrl(QString(RequestTokenUrl).arg(pUrl)));

    // Specify the scope of token requests

    QList<O0RequestParameter> reqParams = QList<O0RequestParameter>();
    reqParams << O0RequestParameter(QByteArray("scope"),
                                    QString(RequestScope).arg(pUrl).toUtf8());
    setRequestParameters(reqParams);

    // Create a store object for writing the received authentication tokens

    O0SettingsStore *store = new O0SettingsStore(EncryptionKey());
    store->setGroupKey(PmrRepository::Url().toUtf8().constData());
    setStore(store);
}

//==============================================================================

const char *PmrOAuthClient::ConsumerKey(void)
{
    return "9uuENvnELA1cdoy7WhFg4Gsq";
}

//==============================================================================

const char *PmrOAuthClient::ConsumerSecret(void)
{
    return "IqCnqYJZmYzVbFmMsRE_C66F";
}

//==============================================================================

const char *PmrOAuthClient::EncryptionKey(void)
{
    // Used to encrypt saved access tokens

    return "hgh189;;099!@7878";
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
