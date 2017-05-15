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
// PMR authentication
//==============================================================================

#include "corecliutils.h"
#include "pmrauthentication.h"
#include "pmrsupportpreferenceswidget.h"
#include "pmrwebservice.h"

//==============================================================================

#include "o2/o0globals.h"
#include "o2/o0settingsstore.h"
#include "o2/o1requestor.h"

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

PmrAuthentication::PmrAuthentication(const QString &pPmrUrl, QObject *pParent) :
    O1(pParent)
{
    // Note: our various constants must match the values assigned to PMR...

    // Set the client ID and secret

    setClientId("9uuENvnELA1cdoy7WhFg4Gsq");
    setClientSecret("IqCnqYJZmYzVbFmMsRE_C66F");

    // Set the callback URL and local port

    setCallbackUrl("http://localhost:%1/");
    setLocalPort(1234);

    // Set the URLs used to get authentication tokens

    setAccessTokenUrl(pPmrUrl+"/OAuthGetAccessToken");
    setAuthorizeUrl(pPmrUrl+"/OAuthAuthorizeToken");
    setRequestTokenUrl(pPmrUrl+"/OAuthRequestToken");

    // Specify the scope of token requests

    QList<O0RequestParameter> requestParameters = QList<O0RequestParameter>();

    requestParameters << O0RequestParameter("scope",
                                            QString("%1/oauth_scope/collection,"
                                                    "%1/oauth_scope/search,"
                                                    "%1/oauth_scope/workspace_tempauth,"
                                                    "%1/oauth_scope/workspace_full").arg(pPmrUrl).toUtf8());

    setRequestParameters(requestParameters);

    // Create a store object for writing the received authentication tokens
    // Note: for the group key, we use the given URL's host since the URL itself
    //       contains a "://" and this messes things up with QSettings...

    O0SettingsStore *store = new O0SettingsStore("hgh189;;099!@7878");

    store->setGroupKey(SettingsPlugins+"/"+PluginName+"/Credentials/"+QUrl(pPmrUrl).host());

    setStore(store);
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
