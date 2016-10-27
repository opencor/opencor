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
// PMR web service response
//==============================================================================

#pragma once

//==============================================================================

#include <QJsonDocument>
#include <QObject>

//==============================================================================

class QNetworkReply;

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

class PmrWebServiceResponse : public QObject
{
    Q_OBJECT
public:
    explicit PmrWebServiceResponse(QNetworkReply *pNetworkReply);

private:
    QNetworkReply *mNetworkReply;

signals:
    void busy(const bool &pBusy);

    void response(const QJsonDocument &pJsonDocument);
    void finished();

    void unauthorised(const QString &pUrl);
    void movedLocation(const QString &locationUrl);
    void error(const QString &pErrorMessage,
               const bool &pInternetConnectionAvailable);

private slots:
    void processResponse();
};

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
