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

#include "pmrwebservice.h"
#include "pmrwebserviceresponse.h"

//==============================================================================

#include <QNetworkReply>

//==============================================================================

#include "zlib.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

PmrWebServiceResponse::PmrWebServiceResponse(QNetworkReply *pNetworkReply) :
    mNetworkReply(pNetworkReply)
{
    // Process the response once we have got it

    connect(mNetworkReply, SIGNAL(finished()),
            this, SLOT(processResponse()));
}

//==============================================================================

void PmrWebServiceResponse::processResponse()
{
    // Retrieve the contents of the response and uncompress it if needed

    QByteArray jsonResponse = QByteArray();

    if (mNetworkReply->rawHeader("Content-Encoding") != "gzip") {
        // The response is not compressed, so just retrieve its raw contents

        jsonResponse = mNetworkReply->readAll();
    } else {
        // Retrieve the raw contents of the response and uncompress it

        QByteArray compressedJsonResponse = mNetworkReply->readAll();
        z_stream stream;

        memset(&stream, 0, sizeof(z_stream));

        if (inflateInit2(&stream, MAX_WBITS+16) == Z_OK) {
            enum {
                BufferSize = 32768
            };

            Bytef buffer[BufferSize];

            stream.next_in = (Bytef *) compressedJsonResponse.data();
            stream.avail_in = compressedJsonResponse.size();

            do {
                stream.next_out = buffer;
                stream.avail_out = BufferSize;

                inflate(&stream, Z_NO_FLUSH);

                if (!stream.msg)
                    jsonResponse += QByteArray::fromRawData((char *) buffer, BufferSize-stream.avail_out);
                else
                    jsonResponse = QByteArray();
            } while (!stream.avail_out);

            inflateEnd(&stream);
        }
    }

    // Check for network errors

    static const QStringList ResponseMimeTypes = QStringList() << "application/json"
                                                               << RequestMimeType
                                                               << CollectionMimeType;

    int httpStatusCode = mNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (mNetworkReply->error() != QNetworkReply::NoError) {
        if (httpStatusCode == 403)
            emit forbidden(mNetworkReply->url().toString());
        else
            emit error(mNetworkReply->errorString());
    } else if (httpStatusCode == 302) {
        emit found(mNetworkReply->header(QNetworkRequest::LocationHeader).toString());
    } else if (!ResponseMimeTypes.contains(mNetworkReply->header(QNetworkRequest::ContentTypeHeader).toString())) {
        emit error(tr("PMR response has unexpected content type"));
    } else {
        // Parse our response

        QJsonParseError jsonParseError;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonResponse, &jsonParseError);

        // Check for parse errors

        if (jsonParseError.error != QJsonParseError::NoError)
            emit error(jsonParseError.errorString());
        else
            emit response(jsonDocument);
    }

    emit busy(false);
    emit finished();

    mNetworkReply->deleteLater();
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
