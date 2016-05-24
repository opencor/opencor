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

#pragma once

//==============================================================================

#include "pmrsupportglobal.h"

//==============================================================================

#include "o1.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QUrl>
#include <QVariantMap>

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================


// PMR OAuth 1.0 client

class PmrOAuthClient: public O1
{
    Q_OBJECT

public:
    explicit PmrOAuthClient(QObject *parent = 0);

    static constexpr auto Url = "https://models.physiomeproject.org";

private:
    static constexpr auto ConsumerKey = "9uuENvnELA1cdoy7WhFg4Gsq";
    static constexpr auto ConsumerSecret = "IqCnqYJZmYzVbFmMsRE_C66F";
    static constexpr auto CallbackUrl = "http://localhost:%1/";
    static constexpr auto CallbackPort = 1234;

    // Used to encrypt saved access tokens

    static constexpr auto EncryptionKey = "hgh189;;099!@7878";

    // Authorisation Url templates

    static constexpr auto AccessTokenUrl = "%1/OAuthGetAccessToken";
    static constexpr auto AuthorizeUrl = "%1/OAuthAuthorizeToken";
    static constexpr auto RequestTokenUrl = "%1/OAuthRequestToken";

    // Scope template for request

    static constexpr auto RequestScope = "%1/oauth_scope/collection,"
                                         "%1/oauth_scope/search,"
                                         "%1/oauth_scope/workspace_tempauth,"
                                         "%1/oauth_scope/workspace_full";
};

//==============================================================================


class PMRSUPPORT_EXPORT PmrAuthentication : public QObject
{
    Q_OBJECT

public:
    explicit PmrAuthentication(QObject *parent = 0);

signals:
    void extraTokensReady(const QVariantMap &extraTokens);
    void linkingFailed();
    void linkingSucceeded();
    void gotWorkspaces();

public slots:
    void doOAuth();
    void getWorkspaceList(void);

private slots:
    void onLinkedChanged();
    void onLinkingSucceeded();
    void onOpenBrowser(const QUrl &url);
    void onCloseBrowser();

    void gotWorkspaceList(void);

private:
    PmrOAuthClient *mPmrOAuthClient;
};

//==============================================================================

}   // namespace PMRWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
