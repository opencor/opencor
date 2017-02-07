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
#include "pmrwebservicemanager.h"

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

PmrAuthentication::PmrAuthentication(const QString &pUrl,
                                     PmrWebServiceManager *pParent) :
    O1(pParent)
{
    // Note: our various constants must match the values assigned to PMR...

    // Make sure that we get told when our parent's PMR URL has changed

    connect(pParent, SIGNAL(pmrUrlChanged(const QString &)),
            this, SLOT(pmrUrlChanged(const QString &)));

    // Set the client ID and secret

    setClientId("9uuENvnELA1cdoy7WhFg4Gsq");
    setClientSecret("IqCnqYJZmYzVbFmMsRE_C66F");

    // Set the callback URL and local port

    setCallbackUrl("http://localhost:%1/");
    setLocalPort(1234);

    // Set the URLs used to get authentication tokens

    setAccessTokenUrl(pUrl+"/OAuthGetAccessToken");
    setAuthorizeUrl(pUrl+"/OAuthAuthorizeToken");
    setRequestTokenUrl(pUrl+"/OAuthRequestToken");

    // Specify the scope of token requests

    QList<O0RequestParameter> requestParameters = QList<O0RequestParameter>();

    requestParameters << O0RequestParameter("scope",
                                            QString("%1/oauth_scope/collection,"
                                                    "%1/oauth_scope/search,"
                                                    "%1/oauth_scope/workspace_tempauth,"
                                                    "%1/oauth_scope/workspace_full").arg(pUrl).toUtf8());

    setRequestParameters(requestParameters);

    // Create a store object for writing the received authentication tokens
    // Note: for the group key, we use the given URL's host since the URL itself
    //       contains a "://" and this messes things up with QSettings...

    O0SettingsStore *store = new O0SettingsStore("hgh189;;099!@7878");

    store->setGroupKey("Plugins/PMRSupport/Credentials/"+QUrl(pUrl).host());

    setStore(store);
}

//==============================================================================

void PmrAuthentication::pmrUrlChanged(const QString &pPmrUrl)
{
//---ISSUE1069--- TO BE DONE...
qDebug("NEW PMR URL: %s", qPrintable(pPmrUrl));
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
