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

    void error(const QString &pErrorMessage);
    void forbidden(const QString &pUrl);
    void found(const QString &locationUrl);

private slots:
    void processResponse();
};

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
