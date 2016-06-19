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
// PMR network response
//==============================================================================

#include "pmrrepository.h"
#include "pmrrepositoryresponse.h"

//==============================================================================

#include <QNetworkReply>
#include <QNetworkRequest>

//==============================================================================

#include "zlib.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

const QStringList PmrRepositoryResponse::ResponseMimeTypes = QStringList()
                                                           << QString("application/json")
                                                           << QString(PmrRepository::RequestMimeType())
                                                           << QString(PmrRepository::CollectionMimeType());

//==============================================================================

PmrRepositoryResponse::PmrRepositoryResponse(QNetworkReply *pNetworkReply) : mNetworkReply(pNetworkReply)
{
    connect(mNetworkReply, SIGNAL(finished()), this, SLOT(processResponse()));
}

//==============================================================================

void PmrRepositoryResponse::processResponse(void)
{
    // Uncompress the response body and check that it is valid JSON

    QByteArray contentData = QByteArray();

    if (mNetworkReply->rawHeader("Content-Encoding") != "gzip") {
        // Get the raw content

        contentData = mNetworkReply->readAll();
    }
    else {
        // Retrieve and uncompress the content

        QByteArray compressedData = mNetworkReply->readAll();
        z_stream stream;

        memset(&stream, 0, sizeof(z_stream));

        if (inflateInit2(&stream, MAX_WBITS+16) == Z_OK) {
            static const int BufferSize = 32768;

            Bytef buffer[BufferSize];

            stream.next_in = (Bytef *) compressedData.data();
            stream.avail_in = compressedData.size();

            do {
                stream.next_out = buffer;
                stream.avail_out = BufferSize;

                inflate(&stream, Z_NO_FLUSH);

                if (!stream.msg)
                    contentData += QByteArray::fromRawData((char *) buffer, BufferSize-stream.avail_out);
                else
                    contentData = QByteArray();
            } while (!stream.avail_out);

            inflateEnd(&stream);
        }
    }

    // Get the HTTP response code

    int httpStatusCode = mNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    // Check for network errors

    if (mNetworkReply->error() != QNetworkReply::NoError) {

        QString errorMessage = mNetworkReply->errorString();

        qCritical() << "Http status " << httpStatusCode << ": " << errorMessage;
        qDebug() << contentData;

        emit error(errorMessage, true);
    }

    // Check for a moved location response

    else if (httpStatusCode == 302) {

        emit movedLocation(mNetworkReply->header(QNetworkRequest::LocationHeader).toString());
    }

    // Check we actually have JSON from PMR

    else if (!ResponseMimeTypes.contains(mNetworkReply->header(QNetworkRequest::ContentTypeHeader).toString())) {
        qCritical() << "Response has wrong content type: "
                    << mNetworkReply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << contentData;

        emit error(tr("Response has unexpected content type"), true);
    }

    else {
        // Parse our uncompressed JSON data

        QJsonParseError jsonParseError;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(contentData, &jsonParseError);

        // Check for parse errors

        if (jsonParseError.error != QJsonParseError::NoError) {

            qCritical() << "JSON Error: " << jsonParseError.errorString();
            qDebug() << contentData;

            emit error(jsonParseError.errorString(), true);
        }

        // All good...

        else {

            emit gotJsonResponse(jsonDocument);
        }
    }

    emit busy(false);

    mNetworkReply->deleteLater();

    // Let amyone waiting on us know that we are done

    emit finished();
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

