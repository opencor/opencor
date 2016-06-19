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
// PMR repository
//==============================================================================

#include "corecliutils.h"
#include "pmrrepository.h"
#include "pmrrepositorymanager.h"
#include "pmrrepositoryresponse.h"

//==============================================================================

#include <QFuture>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtConcurrent/QtConcurrent>

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

const QString PmrRepository::Url()
{
    return "https://models.physiomeproject.org";
}

//==============================================================================

const QByteArray PmrRepository::CollectionMimeType()
{
    return "application/vnd.physiome.pmr2.json.1";
}

//==============================================================================

const QByteArray PmrRepository::RequestMimeType()
{
    return "application/vnd.physiome.pmr2.json.0";
}

//==============================================================================

PmrRepository::PmrRepository(QObject *parent) : QObject(parent),
    mExposures(QMap<QString, PmrExposure *>())
{
    // Create a network access manager so that we can then retrieve various
    // things from the PMR

    mPmrRepositoryManager = new PmrRepositoryManager(this);

    // Pass network manager signals directly to ourselves

    connect(mPmrRepositoryManager, SIGNAL(authenticated(bool)), this, SIGNAL(authenticated(bool)));
    connect(mPmrRepositoryManager, SIGNAL(busy(bool)), this, SIGNAL(busy(bool)));
    connect(mPmrRepositoryManager, SIGNAL(error(QString, bool)), this, SIGNAL(error(QString, bool)));
}

//==============================================================================

PmrRepository::~PmrRepository()
{
    delete mPmrRepositoryManager;
}

//==============================================================================

void PmrRepository::authenticate(const bool &pLink)
{
    mPmrRepositoryManager->authenticate(pLink);
}

//==============================================================================

void PmrRepository::getAuthenticationStatus(void)
{
    emit authenticated(mPmrRepositoryManager->isAuthenticated());
}

//==============================================================================

QString PmrRepository::informationNoteMessage() const
{
    // Return some information note

    return tr("<strong>Note:</strong> you might want to email <a href=\"mailto: help@physiomeproject.org\">help@physiomeproject.org</a> and ask why this is the case.");
}

//==============================================================================

void PmrRepository::emitInformation(const QString &pMessage)
{
    emit pMessage + "<br/><br/>" + informationNoteMessage();
}

//==============================================================================
//==============================================================================

static const char *DirNameProperty    = "DirName";
static const char *ExposureProperty   = "Exposure";
static const char *NextActionProperty = "NextAction";
static const char *WorkspaceProperty  = "Workspace";

//==============================================================================



//==============================================================================
//==============================================================================

void PmrRepository::requestExposuresList(void)
{
    // Get the list of exposures from the PMR after making sure that our
    // internal data has been reset

    auto repositoryResponse = mPmrRepositoryManager->sendPmrRequest(QString("%1/exposure").arg(Url()),
                                                                    false);
    connect(repositoryResponse, SIGNAL(gotJsonResponse(QJsonDocument)),
            this, SLOT(exposuresListResponse(QJsonDocument)));
}

//==============================================================================

void PmrRepository::exposuresListResponse(const QJsonDocument &pJsonDocument)
{
    PmrExposureList exposureList = PmrExposureList();

    QVariantMap collectionMap = pJsonDocument.object().toVariantMap()["collection"].toMap();
    foreach (const QVariant &linksVariant, collectionMap["links"].toList()) {
        QVariantMap linksMap = linksVariant.toMap();

        if (!linksMap["rel"].toString().compare("bookmark")) {
            QString exposureUrl = linksMap["href"].toString().trimmed();
            QString exposureName = linksMap["prompt"].toString().simplified();

            if (   !exposureUrl.isEmpty() && !exposureName.isEmpty()) {
                auto exposure = new PmrExposure(exposureUrl, exposureName, this);

                mExposures.insert(exposureUrl, exposure);
                exposureList.append(exposure);
            }
        }
    }
    std::sort(exposureList.begin(), exposureList.end(), PmrExposure::compare);

    // Respond with a list of exposures

    emit exposuresList(exposureList);
}

//==============================================================================
//==============================================================================

void PmrRepository::requestExposureFiles(const QString &pUrl)
{
    auto exposure = mExposures.value(pUrl);

    if (exposure->isNull()) {
        emit warning(tr("Unknown exposure: ") + pUrl);
    }
    else if (exposure->fileUrlsLeftCount() < 0) {
        requestExposureInformation(exposure, RequestExposureFiles);
    }
    else if (exposure->fileUrlsLeftCount() == 0) {
        if (exposure->exposureFileList().count())
            emit exposureFilesList(pUrl, exposure->exposureFileList());
        else
            emitInformation(tr("No exposure files could be found for %1.").arg(exposure->toHtml()));
    }
}

//==============================================================================
//==============================================================================

void PmrRepository::requestExposureInformation(PmrExposure *pExposure, const Action &pNextAction)
{
    auto repositoryResponse = mPmrRepositoryManager->sendPmrRequest(pExposure->url(), false);

    repositoryResponse->setProperty(ExposureProperty, QVariant::fromValue((void *)pExposure));
    repositoryResponse->setProperty(NextActionProperty, pNextAction);
    connect(repositoryResponse, SIGNAL(gotJsonResponse(QJsonDocument)),
            this, SLOT(exposureInformationResponse(QJsonDocument)));
}

//==============================================================================

void PmrRepository::exposureInformationResponse(const QJsonDocument &pJsonDocument)
{
    auto exposure = (PmrExposure *)sender()->property(ExposureProperty).value<void *>();

    if (exposure) {
        QVariantMap collectionMap = pJsonDocument.object().toVariantMap()["collection"].toMap();


        // Retrieve the URLs that will help us to retrieve some
        // information about the exposure's workspace and exposure
        // files

        QString workspaceUrl;
        QStringList exposureFileUrls;

        foreach (const QVariant &linksVariant, collectionMap["links"].toList()) {
            QVariantMap linksMap = linksVariant.toMap();
            QString relValue = linksMap["rel"].toString();

            if (!relValue.compare("via")) { // What about prompt == "Workspace URL" ??

                workspaceUrl = linksMap["href"].toString().trimmed();

            } else if (!relValue.compare("bookmark")) {
                QString exposureFileUrl = linksMap["href"].toString().trimmed();
                if (!exposureFileUrl.isEmpty()) {

                    exposureFileUrls << exposureFileUrl;
                }
            }
        }
        exposure->setFileUrlsLeftCount(exposureFileUrls.count());

        // Make sure that we at least have a workspace

        if (workspaceUrl.isEmpty()) {
            emitInformation(tr("No workspace URL could be found for %1.").arg(exposure->toHtml()));
        }
        else {
            // Retrieve the workspace and exposure file information from PMR

            auto dirName = sender()->property(DirNameProperty).toString();
            requestWorkspaceInformation(workspaceUrl, dirName, exposure);
        }

        if (exposureFileUrls.isEmpty()
         && Action(sender()->property(NextActionProperty).toInt()) == RequestExposureFiles)
            emitInformation(tr("No exposure file URLs could be found for %1.").arg(exposure->toHtml()));

        foreach (const QString &exposureFileUrl, exposureFileUrls) {
            requestExposureFileInformation(exposure, exposureFileUrl);
        }
    }
}

//==============================================================================
//==============================================================================

void PmrRepository::requestExposureFileInformation(PmrExposure *pExposure, const QString &pUrl)
{
    auto repositoryResponse = mPmrRepositoryManager->sendPmrRequest(pUrl, false);
    repositoryResponse->setProperty(ExposureProperty, QVariant::fromValue((void *)pExposure));
    connect(repositoryResponse, SIGNAL(gotJsonResponse(QJsonDocument)),
            this, SLOT(exposureFileInformationResponse(QJsonDocument)));

}

//==============================================================================

void PmrRepository::exposureFileInformationResponse(const QJsonDocument &pJsonDocument)
{
    auto exposure = (PmrExposure *)sender()->property(ExposureProperty).value<void *>();

    if (exposure) {

        bool hasExposureFileInformation = false;

        QVariantMap collectionMap = pJsonDocument.object().toVariantMap()["collection"].toMap();
        QVariantList itemsList = collectionMap["items"].toList();

        if (itemsList.count()) {
            QVariantList linksList = itemsList.first().toMap()["links"].toList();

            if (linksList.count()) {
                // Retrieve the exposure file name, from the
                // exposure file information, and keep track of it

                QString exposureFile = linksList.first().toMap()["href"].toString().trimmed();
                if (!exposureFile.isEmpty()) {
                    exposure->addExposureFile(exposureFile); // Decrements count left...

                    // Ask our widget to add our exposure files,
                    // should we have no exposure file URLs left to
                    // handle

                    if (exposure->fileUrlsLeftCount() == 0) {
                        emit exposureFilesList(exposure->url(), exposure->exposureFileList());
                    }
                    hasExposureFileInformation = true;
                }
            }
        }

        if (!hasExposureFileInformation)
            emitInformation(tr("No exposure file information could be found for %1.").arg(exposure->toHtml()));
    }
}

//==============================================================================

void PmrRepository::requestCloneWorkspace(PmrWorkspace *pWorkspace, const QString &pDirName)
{
    emit busy(true);
    connect(pWorkspace, SIGNAL(warning(QString)), this, SIGNAL(warning(QString)));
    connect(pWorkspace, SIGNAL(workspaceCloned(PmrWorkspace *)),
            this, SLOT(cloneWorkspaceResponse(PmrWorkspace *)));

    QFuture<void> future = QtConcurrent::run(pWorkspace, &PmrWorkspace::clone, pDirName);
}

//==============================================================================

void PmrRepository::cloneWorkspaceResponse(PmrWorkspace *pWorkspace)
{
    emit busy(false);
    emit workspaceCloned(pWorkspace);
}

//==============================================================================

void PmrRepository::requestCloneExposureWorkspace(const QString &pUrl, const QString &pDirName)
{
    // Check whether we already know about the workspace for the given exposure

    auto exposure = mExposures.value(pUrl);

    if (exposure->isNull()) {
        emit warning(tr("Unknown exposore: ") + pUrl);
    }
    else if (!exposure->workspace()->isNull()) {
        requestCloneWorkspace(exposure->workspace(), pDirName);
    }
    else {
        // To clone the workspace associated with the given exposure, we first
        // need to retrieve some information about the exposure itself

        auto repositoryResponse = mPmrRepositoryManager->sendPmrRequest(pUrl, false);
        repositoryResponse->setProperty(ExposureProperty, QVariant::fromValue((void *)exposure));
        repositoryResponse->setProperty(NextActionProperty, CloneExposureWorkspace);
        repositoryResponse->setProperty(DirNameProperty, pDirName);
        connect(repositoryResponse, SIGNAL(gotJsonResponse(QJsonDocument)),
                this, SLOT(exposureInformationResponse(QJsonDocument)));
    }
}

//==============================================================================
//==============================================================================

void PmrRepository::requestWorkspaceInformation(const QString &pUrl)
{
    auto repositoryResponse = mPmrRepositoryManager->sendPmrRequest(pUrl, true);
    connect(repositoryResponse, SIGNAL(gotJsonResponse(QJsonDocument)),
            this, SLOT(workspaceInformationResponse(QJsonDocument)));
}

//==============================================================================

void PmrRepository::requestWorkspaceInformation(const QString &pUrl, const QString &pDirName,
                                                PmrExposure *pExposure)
{
    auto repositoryResponse = mPmrRepositoryManager->sendPmrRequest(pUrl, true);
    repositoryResponse->setProperty(ExposureProperty, QVariant::fromValue((void *)pExposure));
    repositoryResponse->setProperty(DirNameProperty, pDirName);
    connect(repositoryResponse, SIGNAL(gotJsonResponse(QJsonDocument)),
            this, SLOT(workspaceInformationResponse(QJsonDocument)));
}

//==============================================================================

void PmrRepository::workspaceInformationResponse(const QJsonDocument &pJsonDocument)
{
    QVariantMap collectionMap = pJsonDocument.object().toVariantMap()["collection"].toMap();

    QVariantList itemsList = collectionMap["items"].toList();

    if (itemsList.count()) {
        // Retrieve details of the workspace we are dealing with

        QString workspaceUrl = itemsList.first().toMap()["href"].toString().trimmed();

        QString storageValue = QString();
        QString workspaceDescription = QString();
        QString workspaceOwner = QString();
        QString workspaceName = QString();

        foreach (const QVariant &dataVariant, itemsList.first().toMap()["data"].toList()) {
            QVariantMap dataMap = dataVariant.toMap();

            if      (!dataMap["name"].toString().compare("storage"))
                storageValue = dataMap["value"].toString();
            else if (!dataMap["name"].toString().compare("description"))
                workspaceDescription = dataMap["value"].toString();
            else if (!dataMap["name"].toString().compare("owner"))
                workspaceOwner = dataMap["value"].toString();
            else if (!dataMap["name"].toString().compare("title"))
                workspaceName = dataMap["value"].toString();
        }

        if (!workspaceUrl.isEmpty())
            emit workspaceInformation(workspaceUrl, workspaceName,
                                      workspaceDescription, workspaceOwner);

        auto exposure = (PmrExposure *)sender()->property(ExposureProperty).value<void *>();

        if (!workspaceUrl.isEmpty()) {

            // Make sure that our workspace is a Git repository

            if (!storageValue.compare("git")) {

                auto workspace = new PmrWorkspace(workspaceUrl, workspaceName, exposure);

                // Cloning a workspace from an exposure

                if (exposure) exposure->setWorkspace(workspace);

                // Cloning after creating a new workspace

                else requestWorkspaceCredentials(workspace);

                // Clone the workspace, if requested

                auto dirName = sender()->property(DirNameProperty).toString();
                if (!dirName.isEmpty()) requestCloneWorkspace(workspace, dirName);

            } else if (exposure) {
                    emitInformation(tr("The workspace for %1 is not a Git repository.").arg(exposure->toHtml()));
            }

        } else if (exposure) {
            emitInformation(tr("No workspace information could be found for %1.").arg(exposure->toHtml()));
        }

    }
}

//==============================================================================
//==============================================================================

void PmrRepository::requestNewWorkspace(const QString &pName, const QString &pDescription,
                                        const QString &pDirName)
{
    Q_UNUSED(pDirName)  // set as property

    auto jsonCreateWorkspace = QJsonDocument::fromJson(QString(
        "{\"template\": {\"data\": ["
            "{\"name\": \"form.widgets.title\", \"value\": \"%1\"},"
            "{\"name\": \"form.widgets.description\", \"value\": \"%2\"},"
            "{\"name\": \"form.widgets.storage\", \"value\": \"git\"},"
            "{\"name\": \"form.buttons.add\", \"value\": \"Add\"}"
        "]}}").arg(pName, pDescription).toUtf8());

    auto repositoryResponse =
        mPmrRepositoryManager->sendPmrRequest(QString("%1/workspace/+/addWorkspace").arg(Url()),
                                              true, true, jsonCreateWorkspace);
    repositoryResponse->setProperty(DirNameProperty, pDirName);

    connect(repositoryResponse, SIGNAL(movedLocation(QString)), this, SLOT(newWorkspaceResponse(QString)));
}

//==============================================================================

void PmrRepository::newWorkspaceResponse(const QString &pUrl)
{
    emit workspaceCreated(pUrl);

    auto dirName = sender()->property(DirNameProperty).toString();
    requestWorkspaceInformation(pUrl, dirName);  // Non-empty dirname will clone workspace
}

//==============================================================================
//==============================================================================

void PmrRepository::requestWorkspacesList(void)
{
    auto repositoryResponse = mPmrRepositoryManager->sendPmrRequest(QString("%1/my-workspaces").arg(Url()),
                                                                    true);
    connect(repositoryResponse, SIGNAL(gotJsonResponse(QJsonDocument)),
            this, SLOT(workspacesListResponse(QJsonDocument)));
}

//==============================================================================

void PmrRepository::workspacesListResponse(const QJsonDocument &pJsonDocument)
{
    // Retrieve the list of workspaces

    QVariantMap collectionMap = pJsonDocument.object().toVariantMap()["collection"].toMap();

    PmrWorkspaceList workspaceList = PmrWorkspaceList();

    foreach (const QVariant &linksVariant, collectionMap["links"].toList()) {
        QVariantMap linksMap = linksVariant.toMap();

        if (!linksMap["rel"].toString().compare("bookmark")) {
            QString workspaceUrl = linksMap["href"].toString().trimmed();
            QString workspaceName = linksMap["prompt"].toString().simplified();

            if (   !workspaceUrl.isEmpty()
                && !workspaceName.isEmpty()) {

                workspaceList.add(workspaceUrl, workspaceName, this);
            }
        }
    }

    std::sort(workspaceList.begin(), workspaceList.end(), PmrWorkspace::compare);

    emit workspacesList(workspaceList);
}

//==============================================================================
//==============================================================================

void PmrRepository::requestWorkspaceCredentials(PmrWorkspace *pWorkspace)
{
    auto repositoryResponse = mPmrRepositoryManager->sendPmrRequest(pWorkspace->url() + "/request_temporary_password",
                                                                    true, true);
    repositoryResponse->setProperty(WorkspaceProperty, QVariant::fromValue((void *)pWorkspace));
    connect(repositoryResponse, SIGNAL(gotJsonResponse(QJsonDocument)),
            this, SLOT(workspaceCredentialsResponse(QJsonDocument)));

    // Don't return until response has been processed

    QEventLoop waitLoop;
    connect(repositoryResponse, SIGNAL(finished()), &waitLoop, SLOT(quit()));
    waitLoop.exec();
}

//==============================================================================

void PmrRepository::workspaceCredentialsResponse(const QJsonDocument &pJsonDocument)
{
    QVariantMap jsonResponse = pJsonDocument.object().toVariantMap();

    auto workspace = (PmrWorkspace *)sender()->property(WorkspaceProperty).value<void *>();

    if (workspace && jsonResponse["target"].toString() == workspace->url())
        workspace->setCredentials(jsonResponse["user"].toString(), jsonResponse["key"].toString());
}

//==============================================================================
//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
