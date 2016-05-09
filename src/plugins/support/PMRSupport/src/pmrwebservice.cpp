/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// PMR web service
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "pmrwebservice.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

//==============================================================================

#include "git2.h"
#include "zlib.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

PmrWebService::PmrWebService() :
    mNumberOfExposureFileUrlsLeft(0),
    mWorkspaces(QMap<QString, QString>()),
    mExposureUrls(QMap<QString, QString>()),
    mExposureNames(QMap<QString, QString>()),
    mExposureFileNames(QMap<QString, QString>())
{
    // Create a network access manager so that we can then retrieve various
    // things from the PMR

    mNetworkAccessManager = new QNetworkAccessManager(this);

    // Make sure that we get told when our PMR requests are finished, as well as
    // if there are SSL errors (which would happen if the website's certificate
    // is invalid, e.g. it has expired)

    connect(mNetworkAccessManager, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(finished(QNetworkReply *)));
    connect(mNetworkAccessManager, SIGNAL(sslErrors(QNetworkReply *, const QList<QSslError> &)),
            this, SLOT(sslErrors(QNetworkReply *, const QList<QSslError> &)));
}

//==============================================================================

PmrWebService::~PmrWebService()
{
}

//==============================================================================

static const char *PmrRequestProperty = "PmrRequest";
static const char *ActionProperty     = "Action";
static const char *NameProperty       = "Name";

//==============================================================================

void PmrWebService::sendPmrRequest(const PmrRequest &pPmrRequest,
                                   const QString &pUrl,
                                   const Action pAction,
                                   const QString &pName)
{
    // Let the user know that we are busy

    emit busy(true);

    // Send our request to the PMR, asking for the response to be compressed,
    // but only if we are connected to the Internet

    if (Core::internetConnectionAvailable()) {
        QNetworkRequest networkRequest;

        networkRequest.setRawHeader("Accept", "application/vnd.physiome.pmr2.json.1");
        networkRequest.setRawHeader("Accept-Encoding", "gzip");

        switch (pPmrRequest) {
        case ExposuresList:
            networkRequest.setUrl(QUrl("https://models.physiomeproject.org/exposure"));

            break;
        case ExposureInformation:
        case WorkspaceInformation:
        case ExposureFileInformation:
            networkRequest.setUrl(QUrl(pUrl));

            break;
        }

        QNetworkReply *networkReply = mNetworkAccessManager->get(networkRequest);

        // Keep track of the type of our PMR request and of the action to carry
        // out (once we have retrieved everything we need from PMR)

        networkReply->setProperty(PmrRequestProperty, pPmrRequest);
        networkReply->setProperty(ActionProperty, pAction);
        networkReply->setProperty(NameProperty, pName);
    } else {
        finished();
    }
}

//==============================================================================

QString PmrWebService::informationNoteMessage() const
{
    // Return some information note

    return tr("<strong>Note:</strong> you might want to email <a href=\"mailto: help@physiomeproject.org\">help@physiomeproject.org</a> and ask why this is the case.");
}

//==============================================================================

void PmrWebService::doCloneWorkspace(const QString &pWorkspace,
                                     const QString &pDirName)
{
   // Clone the workspace

   git_libgit2_init();

   git_repository *gitRepository = 0;
   QByteArray workspaceByteArray = pWorkspace.toUtf8();
   QByteArray dirNameByteArray = pDirName.toUtf8();

   int res = git_clone(&gitRepository, workspaceByteArray.constData(),
                       dirNameByteArray.constData(), 0);

   if (res) {
       const git_error *gitError = giterr_last();

       emit warning(gitError?
                        tr("Error %1: %2.").arg(QString::number(gitError->klass),
                                                Core::formatMessage(gitError->message)):
                        tr("An error occurred while trying to clone the workspace."));
   } else if (gitRepository) {
       git_repository_free(gitRepository);
   }

   git_libgit2_shutdown();
}

//==============================================================================

void PmrWebService::doShowExposureFiles(const QString &pExposureUrl)
{
    // Show the exposure files, but only if there are some

    if (!mExposureFileNames.values(pExposureUrl).isEmpty()) {
        emit showExposureFiles(pExposureUrl);
    } else {
        emit information( tr("No exposure file URL could be found for <a href=\"%1\">%2</a>.").arg(pExposureUrl, mExposureNames.value(pExposureUrl))
                         +"<br/><br/>"+informationNoteMessage());
    }
}

//==============================================================================

bool sortExposureFiles(const QString &pExposureFile1,
                       const QString &pExposureFile2)
{
    // Determine which of the two exposure files should be first (without
    // worrying about casing)

    return pExposureFile1.compare(pExposureFile2, Qt::CaseInsensitive) < 0;
}

//==============================================================================

void PmrWebService::finished(QNetworkReply *pNetworkReply)
{
    // Check whether our PMR request was successful

    PmrRequest pmrRequest = pNetworkReply?PmrRequest(pNetworkReply->property(PmrRequestProperty).toInt()):ExposuresList;
    bool internetConnectionAvailable = true;
    QString errorMessage = QString();
    QString informationMessage = QString();
    PmrExposures exposures = PmrExposures();
    QString workspaceUrl = QString();
    QStringList exposureFileUrls = QStringList();
    QString exposureUrl = QString();

    if (pNetworkReply) {
        if (pNetworkReply->error() == QNetworkReply::NoError) {
            // Retrieve and uncompress our JSON data

            QByteArray compressedData = pNetworkReply->readAll();
            QByteArray uncompressedData = QByteArray();
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
                        uncompressedData += QByteArray::fromRawData((char *) buffer, BufferSize-stream.avail_out);
                    else
                        uncompressedData = QByteArray();
                } while (!stream.avail_out);

                inflateEnd(&stream);
            }

            // Parse our uncompressed JSON data

            QJsonParseError jsonParseError;
            QJsonDocument jsonDocument = QJsonDocument::fromJson(uncompressedData, &jsonParseError);
            QString url = pNetworkReply->url().toString();

            if (jsonParseError.error == QJsonParseError::NoError) {
                // Check our PMR request to determine what we should do with the
                // data

                QVariantMap collectionMap = jsonDocument.object().toVariantMap()["collection"].toMap();

                switch (pmrRequest) {
                case ExposuresList:
                    // Retrieve the list of exposures

                    foreach (const QVariant &linksVariant, collectionMap["links"].toList()) {
                        QVariantMap linksMap = linksVariant.toMap();

                        if (!linksMap["rel"].toString().compare("bookmark")) {
                            QString exposureUrl = linksMap["href"].toString().trimmed();
                            QString exposureName = linksMap["prompt"].toString().simplified();

                            if (   !exposureUrl.isEmpty()
                                && !exposureName.isEmpty()) {
                                mExposureNames.insert(exposureUrl, exposureName);

                                exposures.add(exposureUrl, exposureName);
                            }
                        }
                    }

                    std::sort(exposures.begin(), exposures.end(),
                              PmrExposure::compare);

                    break;
                case ExposureInformation:
                    // Retrieve the URLs that will help us to retrieve some
                    // information about the exposure's workspace and exposure
                    // files

                    exposureUrl = url;

                    foreach (const QVariant &linksVariant, collectionMap["links"].toList()) {
                        QVariantMap linksMap = linksVariant.toMap();
                        QString relValue = linksMap["rel"].toString();

                        if (!relValue.compare("via")) {
                            workspaceUrl = linksMap["href"].toString().trimmed();
                        } else if (!relValue.compare("bookmark")) {
                            QString exposureFileUrl = linksMap["href"].toString().trimmed();

                            if (!exposureFileUrl.isEmpty()) {
                                exposureFileUrls << exposureFileUrl;

                                ++mNumberOfExposureFileUrlsLeft;
                            }
                        }
                    }

                    // Make sure that we at least have a workspace

                    if (workspaceUrl.isEmpty()) {
                        informationMessage = tr("No workspace URL could be found for <a href=\"%1\">%2</a>.");

                        break;
                    }

                    // (Pretend) to show our exposure files in case this is what
                    // we ultimately want to do and in case we have no exposure
                    // files to show
                    // Note: indeed, this will result in an information message
                    //       being shown to tell us that there are no exposure
                    //       files. If we were not do the below, then we would
                    //       never get that message the first time we try to
                    //       show our exposure files (since that first time, we
                    //       would normally show our exposure files after having
                    //       retrieved the last exposure file information)...

                    if (   exposureFileUrls.isEmpty()
                        && (Action(pNetworkReply->property(ActionProperty).toInt()) == ShowExposureFiles)) {
                        doShowExposureFiles(exposureUrl);
                    }

                    // Retrieve the workspace and exposure file information from
                    // PMR

                    mExposureUrls.insert(workspaceUrl, url);

                    sendPmrRequest(WorkspaceInformation, workspaceUrl,
                                   Action(pNetworkReply->property(ActionProperty).toInt()));

                    foreach (const QString &exposureFileUrl, exposureFileUrls) {
                        mExposureUrls.insert(exposureFileUrl, url);

                        sendPmrRequest(ExposureFileInformation, exposureFileUrl,
                                       Action(pNetworkReply->property(ActionProperty).toInt()));
                    }

                    break;
                case WorkspaceInformation: {
                    QVariantList itemsList = collectionMap["items"].toList();

                    exposureUrl = mExposureUrls.value(url);

                    if (itemsList.count()) {
                        // Retrieve the ype of workspace we are dealing with

                        QString storageValue = QString();

                        foreach (const QVariant &dataVariant, itemsList.first().toMap()["data"].toList()) {
                            QVariantMap dataMap = dataVariant.toMap();

                            if (!dataMap["name"].toString().compare("storage")) {
                                storageValue = dataMap["value"].toString();

                                break;
                            }
                        }

                        // Make sure that our workspace is a Git repository

                        if (!storageValue.compare("git")) {
                            // Retrieve the workspace URL from the workspace
                            // information, and keep track of it

                            QString workspace = itemsList.first().toMap()["href"].toString().trimmed();

                            if (!workspace.isEmpty())
                                mWorkspaces.insert(exposureUrl, workspace);
                        } else {
                            informationMessage = tr("The workspace for <a href=\"%1\">%2</a> is not a Git repository.");
                        }
                    } else {
                        informationMessage = tr("No workspace information could be found for <a href=\"%1\">%2</a>.");
                    }

                    break;
                }
                case ExposureFileInformation: {
                    bool hasExposureFileInformation = true;
                    QVariantList itemsList = collectionMap["items"].toList();

                    exposureUrl = mExposureUrls.value(url);

                    if (itemsList.count()) {
                        QVariantList linksList = itemsList.first().toMap()["links"].toList();

                        if (linksList.count()) {
                            // Retrieve the exposure file name, from the
                            // exposure file information, and keep track of it

                            QString exposureFile = linksList.first().toMap()["href"].toString().trimmed();

                            if (!exposureFile.isEmpty()) {
                                exposureUrl = mExposureUrls.value(url);

                                mExposureFileNames.insertMulti(exposureUrl, exposureFile);

                                --mNumberOfExposureFileUrlsLeft;

                                // Ask our widget to add our exposure files,
                                // should we have no exposure file URL left to
                                // handle

                                if (!mNumberOfExposureFileUrlsLeft) {
                                    QStringList exposureFileNames = mExposureFileNames.values(exposureUrl);

                                    std::sort(exposureFileNames.begin(), exposureFileNames.end(),
                                              sortExposureFiles);

                                    emit addExposureFiles(exposureUrl, exposureFileNames);
                                }
                            }
                        } else {
                            hasExposureFileInformation = false;
                        }
                    } else {
                        hasExposureFileInformation = false;
                    }

                    if (!hasExposureFileInformation)
                        informationMessage = tr("No exposure file information could be found for <a href=\"%1\">%2</a>.");

                    break;
                }
                }

                // Check whether something wrong happened during the processing
                // of our request

                if (!informationMessage.isEmpty()) {
                    emit information( informationMessage.arg(exposureUrl, mExposureNames.value(exposureUrl))
                                     +"<br/><br/>"+informationNoteMessage());
                }
            } else {
                errorMessage = jsonParseError.errorString();
            }
        } else {
            errorMessage = pNetworkReply->errorString();
        }
    } else {
        internetConnectionAvailable = false;
    }

    // Some additional processing based on our PMR request

    switch (pmrRequest) {
    case ExposuresList:
        // Respond with a list of exposures

        emit exposuresList(exposures, errorMessage, internetConnectionAvailable);

        break;
    case ExposureInformation:
        // No additional processing needed

        break;
    case WorkspaceInformation:
        // Clone the workspace, if possible and requested

        if (   mWorkspaces.contains(exposureUrl)
            && (Action(pNetworkReply->property(ActionProperty).toInt()) == CloneWorkspace)) {
            doCloneWorkspace(mWorkspaces.value(exposureUrl),
                             pNetworkReply->property(NameProperty).toString());
        }

        break;
    case ExposureFileInformation:
        // Show the exposure files, if possible and requested

        if (   !mNumberOfExposureFileUrlsLeft
            &&  (Action(pNetworkReply->property(ActionProperty).toInt()) == ShowExposureFiles)) {
            doShowExposureFiles(exposureUrl);
        }

        break;
    }

    // Show ourselves as not busy anymore

    emit busy(false);

    // Delete (later) the network reply

    if (pNetworkReply)
        pNetworkReply->deleteLater();
}

//==============================================================================

void PmrWebService::sslErrors(QNetworkReply *pNetworkReply,
                                const QList<QSslError> &pSslErrors)
{
    // Ignore the SSL errors since we trust the website and therefore its
    // certificate (even if it is invalid, e.g. it has expired)

    pNetworkReply->ignoreSslErrors(pSslErrors);
}

//==============================================================================

void PmrWebService::requestExposuresList(void)
{
    // Get the list of exposures from the PMR after making sure that our
    // internal data has been reset

    mWorkspaces.clear();
    mExposureUrls.clear();
    mExposureNames.clear();
    mExposureFileNames.clear();

    sendPmrRequest(ExposuresList);
}

//==============================================================================

void PmrWebService::cloneWorkspace(const QString &pUrl, const QString &pDirName)
{
    // Check whether we already know about the workspace for the given exposure

    QString workspace = mWorkspaces.value(pUrl);

    if (!workspace.isEmpty()) {
        emit busy(true);

        doCloneWorkspace(workspace, pDirName);

        emit busy(false);
    } else {
        // To clone the workspace associated with the given exposure, we first
        // need to retrieve some information about the exposure itself

        mNumberOfExposureFileUrlsLeft = 0;

        sendPmrRequest(ExposureInformation, pUrl, CloneWorkspace, pDirName);
    }
}

//==============================================================================

void PmrWebService::requestExposureFiles(const QString &pUrl)
{
    // Check whether we already know about the exposure URL for the given
    // exposure

    QString exposureUrl = mExposureUrls.value(mWorkspaces.value(pUrl));

    if (!exposureUrl.isEmpty()) {
        doShowExposureFiles(exposureUrl);
    } else {
        // To show the exposure files associated with the given exposure, we
        // first need to retrieve some information about the exposure itself

        mNumberOfExposureFileUrlsLeft = 0;

        sendPmrRequest(ExposureInformation, pUrl, ShowExposureFiles);
    }
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
