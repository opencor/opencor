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
// PMR response
//==============================================================================

#pragma once

//==============================================================================

#include <QJsonDocument>
#include <QObject>
#include <QString>

//==============================================================================

class QNetworkReply;

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

class PmrRepositoryResponse : public QObject
{
    Q_OBJECT
public:
    explicit PmrRepositoryResponse(QNetworkReply *pNetworkReply);

    static const QStringList ResponseMimeTypes;

private:
    QNetworkReply *mNetworkReply;

Q_SIGNALS:
    void busy(bool);
    void error(const QString &pErrorMessage, const bool &pInternetConnectionAvailable);
    void gotJsonResponse(const QJsonDocument &pJsonDocument);
    void movedLocation(const QString &locationUrl);

private Q_SLOTS:
    void finished(void);
};

//==============================================================================

}   // namespace PMRWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
