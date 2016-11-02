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
// PMR web service
//==============================================================================

#include "coreguiutils.h"
#include "pmrwebservice.h"
#include "pmrwebservicemanager.h"
#include "pmrwebserviceresponse.h"
#include "pmrworkspacemanager.h"

//==============================================================================

#include <QJsonObject>

//==============================================================================

#include <QtConcurrent/QtConcurrent>

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

PmrWebService::PmrWebService(QObject *pParent) :
    QObject(pParent),
    mUrlExposures(QMap<QString, PmrExposure *>()),
    mFileExposuresLeftCount(QMap<PmrExposure *, int>())
{
    // Create a network access manager so that we can then retrieve various
    // things from PMR

    mPmrWebServiceManager = new PmrWebServiceManager(this);

    // Pass network manager signals directly to ourselves

    connect(mPmrWebServiceManager, SIGNAL(authenticated(const bool &)),
            this, SIGNAL(authenticated(const bool &)));
    connect(mPmrWebServiceManager, SIGNAL(busy(const bool &)),
            this, SIGNAL(busy(const bool &)));
    connect(mPmrWebServiceManager, SIGNAL(error(const QString &, const bool &)),
            this, SIGNAL(error(const QString &, const bool &)));
}

//==============================================================================

void PmrWebService::authenticate(const bool &pAuthenticate)
{
    mPmrWebServiceManager->authenticate(pAuthenticate);
}

//==============================================================================

void PmrWebService::getAuthenticationStatus()
{
    emit authenticated(mPmrWebServiceManager->isAuthenticated());
}

//==============================================================================

QString PmrWebService::informationNoteMessage() const
{
    // Return some information note

    return tr("<strong>Note:</strong> you might want to email <a href=\"mailto: help@physiomeproject.org\">help@physiomeproject.org</a> and ask why this is the case.");
}

//==============================================================================

void PmrWebService::emitInformation(const QString &pMessage)
{
    emit information(pMessage+"<br/><br/>"+informationNoteMessage());
}

//==============================================================================

void PmrWebService::unauthorised(const QString &pUrl)
{
    emitInformation(tr("Not authorised to access %1").arg(pUrl));
}

//==============================================================================

static const char *DirNameProperty    = "DirName";
static const char *ExposureProperty   = "Exposure";
static const char *NextActionProperty = "NextAction";
static const char *WorkspaceProperty  = "Workspace";

//==============================================================================

void PmrWebService::requestExposures()
{
    // Request the list of exposures from PMR

    PmrWebServiceResponse *pmrResponse = mPmrWebServiceManager->request(PmrUrl+"/exposure", false);

    connect(pmrResponse, SIGNAL(response(const QJsonDocument &)),
            this, SLOT(exposuresResponse(const QJsonDocument &)));
}

//==============================================================================

void PmrWebService::exposuresResponse(const QJsonDocument &pJsonDocument)
{
    PmrExposures exposures = PmrExposures();

    QVariantMap collectionMap = pJsonDocument.object().toVariantMap()["collection"].toMap();
    foreach (const QVariant &linksVariant, collectionMap["links"].toList()) {
        QVariantMap linksMap = linksVariant.toMap();

        if (!linksMap["rel"].toString().compare("bookmark")) {
            QString exposureUrl = linksMap["href"].toString().trimmed();
            QString exposureName = linksMap["prompt"].toString().simplified();

            if (   !exposureUrl.isEmpty() && !exposureName.isEmpty()) {
                PmrExposure *exposure = new PmrExposure(exposureUrl, exposureName, this);

                mUrlExposures.insert(exposureUrl, exposure);

                exposures << exposure;
            }
        }
    }

    std::sort(exposures.begin(), exposures.end(), PmrExposure::compare);

    // Respond with a list of exposures

    emit PmrWebService::exposures(exposures);
}

//==============================================================================

void PmrWebService::requestExposureFiles(const QString &pUrl)
{
    // Request some exposure information (and then exposure files) for the
    // exposure which URL is given

    requestExposureInformation(mUrlExposures.value(pUrl), RequestExposureFiles);
}

//==============================================================================

void PmrWebService::requestExposureInformation(PmrExposure *pExposure,
                                               const Action &pNextAction)
{
    PmrWebServiceResponse *pmrResponse = mPmrWebServiceManager->request(pExposure->url(), false);

    pmrResponse->setProperty(ExposureProperty, QVariant::fromValue((void *)pExposure));
    pmrResponse->setProperty(NextActionProperty, pNextAction);

    connect(pmrResponse, SIGNAL(response(const QJsonDocument &)),
            this, SLOT(exposureInformationResponse(const QJsonDocument &)));
}

//==============================================================================

void PmrWebService::exposureInformationResponse(const QJsonDocument &pJsonDocument)
{
    PmrExposure *exposure = (PmrExposure *)sender()->property(ExposureProperty).value<void *>();

    if (exposure) {
        QVariantMap collectionMap = pJsonDocument.object().toVariantMap()["collection"].toMap();

        // Retrieve the URLs that will help us to retrieve some
        // information about the exposure's workspace and exposure
        // files

        QString workspaceUrl = QString();
        QStringList exposureFileUrls = QStringList();

        foreach (const QVariant &linksVariant, collectionMap["links"].toList()) {
            QVariantMap linksMap = linksVariant.toMap();
            QString relValue = linksMap["rel"].toString();

            if (!relValue.compare("via")) {
                workspaceUrl = linksMap["href"].toString().trimmed();
            } else if (!relValue.compare("bookmark")) {
                QString exposureFileUrl = linksMap["href"].toString().trimmed();

                if (!exposureFileUrl.isEmpty())
                    exposureFileUrls << exposureFileUrl;
            }
        }

        mFileExposuresLeftCount.insert(exposure, exposureFileUrls.count());

        // Make sure that we at least have a workspace

        if (workspaceUrl.isEmpty()) {
            emitInformation(tr("No workspace could be found for %1.").arg(exposure->toHtml()));
        } else if (Action(sender()->property(NextActionProperty).toInt()) == CloneExposureWorkspace) {
            // Retrieve workspace file information and clone the workspace

            requestWorkspaceInformation(workspaceUrl, QString(), exposure);
        }

        if (   exposureFileUrls.isEmpty()
            && (Action(sender()->property(NextActionProperty).toInt()) == RequestExposureFiles)) {
            emitInformation(tr("No exposure files could be found for %1.").arg(exposure->toHtml()));
        }

        foreach (const QString &exposureFileUrl, exposureFileUrls)
            requestExposureFileInformation(exposure, exposureFileUrl);
    }
}

//==============================================================================

void PmrWebService::requestExposureFileInformation(PmrExposure *pExposure, const QString &pUrl)
{
    PmrWebServiceResponse *pmrResponse = mPmrWebServiceManager->request(pUrl, false);

    pmrResponse->setProperty(ExposureProperty, QVariant::fromValue((void *)pExposure));

    connect(pmrResponse, SIGNAL(response(const QJsonDocument &)),
            this, SLOT(exposureFileInformationResponse(const QJsonDocument &)));
}

//==============================================================================

void PmrWebService::exposureFileInformationResponse(const QJsonDocument &pJsonDocument)
{
    PmrExposure *exposure = (PmrExposure *)sender()->property(ExposureProperty).value<void *>();

    if (exposure) {

        bool hasExposureFileInformation = false;

        QVariantMap collectionMap = pJsonDocument.object().toVariantMap()["collection"].toMap();
        QVariantList itemsList = collectionMap["items"].toList();

        if (itemsList.count()) {
            QVariantList linksList = itemsList.first().toMap()["links"].toList();

            if (linksList.count()) {
                // Retrieve the exposure file name, from the exposure file
                // information, and keep track of it

                QString exposureFile = linksList.first().toMap()["href"].toString().trimmed();

                if (!exposureFile.isEmpty()) {
                    hasExposureFileInformation = true;

                    exposure->addExposureFile(exposureFile);

                    mFileExposuresLeftCount.insert(exposure, mFileExposuresLeftCount.value(exposure)-1);

                    // Check whether the exposure file has a link called
                    // "Launch with OpenCOR" (e.g. for SED-ML files)

                    foreach (const QVariant &linksVariant, collectionMap["links"].toList()) {
                        QVariantMap linksMap = linksVariant.toMap();
                        QString promptValue = linksMap["prompt"].toString();
                        QString relValue = linksMap["rel"].toString();

                        if (   !promptValue.compare("Launch with OpenCOR")
                            && !relValue.compare("section")) {
                            // Our exposure file has a link called
                            // "Launch with OpenCOR", so check whether its href
                            // value is already listed in our list of exposure
                            // files and, if not, then add it to it

                            exposureFile = linksMap["href"].toString().trimmed().remove("opencor://openFile/");

                            if (!exposureFile.isEmpty())
                                exposure->addExposureFile(exposureFile);
                        }
                    }

                    // Ask our widget to add our exposure files, should we have
                    // no exposure file URLs left to handle

                    if (!mFileExposuresLeftCount.value(exposure))
                        emit exposureFiles(exposure->url(), exposure->exposureFiles());
                }
            }
        }

        if (!hasExposureFileInformation)
            emitInformation(tr("No exposure file information could be found for %1.").arg(exposure->toHtml()));
    }
}

//==============================================================================

void PmrWebService::requestWorkspaceClone(PmrWorkspace *pWorkspace, const QString &pDirName)
{
    emit busy(true);
    getWorkspaceCredentials(pWorkspace);
    connect(pWorkspace, SIGNAL(workspaceCloned(PMRSupport::PmrWorkspace *)),
            this, SLOT(workspaceCloneFinished()));

    QtConcurrent::run(pWorkspace, &PmrWorkspace::clone, pDirName);
}

//==============================================================================

void PmrWebService::workspaceCloneFinished()
{
    emit busy(false);
}

//==============================================================================

void PmrWebService::requestExposureWorkspaceClone(const QString &pExposureUrl)
{
    // Check whether we already know about the workspace for the given exposure

    PmrExposure *exposure = mUrlExposures.value(pExposureUrl);

    if (exposure->workspace()) {
        QString url = exposure->workspace()->url();
        QString dirName = exposure->workspace()->path();

        // Check that we aren't already managing a clone of the workspace
        if (!dirName.isEmpty()) {
            emit warning(tr("Workspace %1 is already cloned in %2.").arg(url, dirName));
            // TODO Prompt user to create a fork on PMR??
        } else {
            PmrWorkspace *existing = PmrWorkspaceManager::instance()->workspace(url);

            if (!existing) {
                // Retrieve the name of an empty directory

                dirName = getEmptyDirectory();

                if (!dirName.isEmpty())
                    requestWorkspaceClone(exposure->workspace(), dirName);
            } else {
                emit warning(tr("Workspace %1 is already cloned in %2.").arg(url, existing->path()));
                // TODO Prompt user to create a fork on PMR??
            }
        }
    } else {
        // To clone the workspace associated with the given exposure, we first
        // need to retrieve some information about the exposure itself

        PmrWebServiceResponse *pmrResponse = mPmrWebServiceManager->request(pExposureUrl, false);

        pmrResponse->setProperty(ExposureProperty, QVariant::fromValue((void *)exposure));
        pmrResponse->setProperty(NextActionProperty, CloneExposureWorkspace);

        connect(pmrResponse, SIGNAL(response(const QJsonDocument &)),
                this, SLOT(exposureInformationResponse(const QJsonDocument &)));
    }
}

//==============================================================================

void PmrWebService::requestWorkspaceSynchronize(PmrWorkspace *pWorkspace, const bool pOnlyPull)
{
    emit busy(true);
    getWorkspaceCredentials(pWorkspace);
    connect(pWorkspace, SIGNAL(workspaceSynchronized(PMRSupport::PmrWorkspace *)),
            this, SLOT(workspaceSynchroniseFinished(PMRSupport::PmrWorkspace *)));

    QtConcurrent::run(pWorkspace, &PmrWorkspace::synchronise, pOnlyPull);
}

//==============================================================================

void PmrWebService::workspaceSynchroniseFinished(PMRSupport::PmrWorkspace *pWorkspace)
{
    emit busy(false);
    emit workspaceSynchronized(pWorkspace);
}

//==============================================================================

void PmrWebService::requestWorkspaceInformation(const QString &pUrl)
{
    PmrWebServiceResponse *pmrResponse = mPmrWebServiceManager->request(pUrl, true);

    connect(pmrResponse, SIGNAL(response(const QJsonDocument &)),
            this, SLOT(workspaceInformationResponse(const QJsonDocument &)));
}

//==============================================================================

void PmrWebService::requestWorkspaceInformation(const QString &pUrl, const QString &pDirName,
                                                PmrExposure *pExposure)
{
    PmrWebServiceResponse *pmrResponse = mPmrWebServiceManager->request(pUrl, true);

    pmrResponse->setProperty(ExposureProperty, QVariant::fromValue((void *)pExposure));
    pmrResponse->setProperty(DirNameProperty, pDirName);

    connect(pmrResponse, SIGNAL(response(const QJsonDocument &)),
            this, SLOT(workspaceInformationResponse(const QJsonDocument &)));
}

//==============================================================================

void PmrWebService::workspaceInformationResponse(const QJsonDocument &pJsonDocument)
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

            if (!dataMap["name"].toString().compare("storage"))
                storageValue = dataMap["value"].toString();
            else if (!dataMap["name"].toString().compare("description"))
                workspaceDescription = dataMap["value"].toString();
            else if (!dataMap["name"].toString().compare("owner"))
                workspaceOwner = dataMap["value"].toString();
            else if (!dataMap["name"].toString().compare("title"))
                workspaceName = dataMap["value"].toString();
        }

        PmrExposure *exposure = (PmrExposure *)sender()->property(ExposureProperty).value<void *>();

        if (!workspaceUrl.isEmpty()) {

            // Make sure that our workspace is a Git repository

            if (!storageValue.compare("git")) {
                PmrWorkspace *workspace = new PmrWorkspace(workspaceUrl,
                                                           workspaceName,
                                                           workspaceDescription,
                                                           workspaceOwner,
                                                           this);
                QString dirName = QString();

                if (exposure) {
                    // Cloning a workspace from an exposure

                    exposure->setWorkspace(workspace);

                    // Check that we aren't already managing a clone of the
                    // workspace

                    PmrWorkspace *existing = PmrWorkspaceManager::instance()->workspace(workspaceUrl);

                    if (!existing) {
                        // Retrieve the name of an empty directory

                        dirName = getEmptyDirectory();
                    } else {
                        emit warning(tr("Workspace %1 is already cloned in %2.").arg(workspaceUrl, existing->path()));
                        // TODO Prompt user to create a fork on PMR??
                    }
                } else {
                    // Cloning after creating a new workspace

                    dirName = sender()->property(DirNameProperty).toString();
                }

                // Clone the workspace, if we have a directory

                if (!dirName.isEmpty())
                    requestWorkspaceClone(workspace, dirName);
            } else if (exposure) {
                emitInformation(tr("The workspace for %1 is not a Git repository.").arg(exposure->toHtml()));
            }
        } else if (exposure) {
            emitInformation(tr("No workspace information could be found for %1.").arg(exposure->toHtml()));
        }
    }
}

//==============================================================================

void PmrWebService::requestNewWorkspace(const QString &pName, const QString &pDescription,
                                        const QString &pDirName)
{
    QJsonDocument jsonCreateWorkspace = QJsonDocument::fromJson(QString(
        "{\"template\": {\"data\": ["
            "{\"name\": \"form.widgets.title\", \"value\": \"%1\"},"
            "{\"name\": \"form.widgets.description\", \"value\": \"%2\"},"
            "{\"name\": \"form.widgets.storage\", \"value\": \"git\"},"
            "{\"name\": \"form.buttons.add\", \"value\": \"Add\"}"
        "]}}").arg(pName, pDescription).toUtf8());

    PmrWebServiceResponse *pmrResponse = mPmrWebServiceManager->request(PmrUrl+"/workspace/+/addWorkspace",
                                                                               true, true, jsonCreateWorkspace);

    pmrResponse->setProperty(DirNameProperty, pDirName);

    connect(pmrResponse, SIGNAL(movedLocation(const QString &)),
            this, SLOT(workspaceCreatedResponse(const QString &)));
}

//==============================================================================

QString PmrWebService::getEmptyDirectory()
{
    // Retrieve and return the name of an empty directory

    return Core::getEmptyDirectory(tr("Select Empty Directory"));
}

//==============================================================================

void PmrWebService::workspaceCreatedResponse(const QString &pUrl)
{
    emit workspaceCreated(pUrl);

    requestWorkspaceInformation(pUrl, sender()->property(DirNameProperty).toString());
    // Note: a non-empty dirname will clone the workspace...
}

//==============================================================================

void PmrWebService::requestWorkspaces()
{
    PmrWebServiceResponse *pmrResponse = mPmrWebServiceManager->request(PmrUrl+"/my-workspaces", true);

    connect(pmrResponse, SIGNAL(response(const QJsonDocument &)),
            this, SLOT(workspacesResponse(const QJsonDocument &)));
}

//==============================================================================

void PmrWebService::workspacesResponse(const QJsonDocument &pJsonDocument)
{
    // Retrieve the list of workspaces

    QVariantMap collectionMap = pJsonDocument.object().toVariantMap()["collection"].toMap();

    PmrWorkspaces workspaces = PmrWorkspaces();

    foreach (const QVariant &linksVariant, collectionMap["links"].toList()) {
        QVariantMap linksMap = linksVariant.toMap();

        if (!linksMap["rel"].toString().compare("bookmark")) {
            QString workspaceUrl = linksMap["href"].toString().trimmed();
            QString workspaceName = linksMap["prompt"].toString().simplified();

            if (   !workspaceUrl.isEmpty()
                && !workspaceName.isEmpty()) {

                workspaces << new PmrWorkspace(workspaceUrl, workspaceName, this);

                workspaces.last()->setOwned(true);
            }
        }
    }

    std::sort(workspaces.begin(), workspaces.end(), PmrWorkspace::compare);

    emit PmrWebService::workspaces(workspaces);
}

//==============================================================================

void PmrWebService::getWorkspaceCredentials(PmrWorkspace *pWorkspace)
{
    PmrWebServiceResponse *pmrResponse = mPmrWebServiceManager->request(pWorkspace->url() + "/request_temporary_password",
                                                                               true, true);

    pmrResponse->setProperty(WorkspaceProperty, QVariant::fromValue((void *)pWorkspace));

    connect(pmrResponse, SIGNAL(response(const QJsonDocument &)),
            this, SLOT(workspaceCredentialsResponse(const QJsonDocument &)));

    // Don't return until the response has been processed

    QEventLoop waitLoop;

    connect(pmrResponse, SIGNAL(finished()),
            &waitLoop, SLOT(quit()));

    waitLoop.exec();
}

//==============================================================================

void PmrWebService::workspaceCredentialsResponse(const QJsonDocument &pJsonDocument)
{
    QVariantMap jsonResponse = pJsonDocument.object().toVariantMap();

    PmrWorkspace *workspace = (PmrWorkspace *)sender()->property(WorkspaceProperty).value<void *>();

    if (workspace && jsonResponse["target"].toString() == workspace->url())
        workspace->setCredentials(jsonResponse["user"].toString(), jsonResponse["key"].toString());
}

//==============================================================================

PmrWorkspace * PmrWebService::getWorkspace(const QString &pUrl)
{
    PmrWorkspace *workspace = 0;
    PmrWebServiceResponse *pmrResponse = mPmrWebServiceManager->request(pUrl, true);

    pmrResponse->setProperty(WorkspaceProperty, QVariant::fromValue((void *) &workspace));

    connect(pmrResponse, SIGNAL(response(const QJsonDocument &)),
            this, SLOT(getWorkspaceResponse(const QJsonDocument &)));

    // We want to catch any 403 (unauthorised) response

    connect(pmrResponse, SIGNAL(unauthorised(const QString &)),
            this, SLOT(workspaceUnauthorised(const QString &)));

    // Don't return until the response has been processed

    QEventLoop waitLoop;

    connect(pmrResponse, SIGNAL(finished()),
            &waitLoop, SLOT(quit()));

    waitLoop.exec();

    return workspace;
}

//==============================================================================

void PmrWebService::getWorkspaceResponse(const QJsonDocument &pJsonDocument)
{
    PmrWorkspace **workspacePointer = (PmrWorkspace **)sender()->property(WorkspaceProperty).value<void *>();

    QVariantMap collectionMap = pJsonDocument.object().toVariantMap()["collection"].toMap();

    QVariantList itemsList = collectionMap["items"].toList();

    if (itemsList.count()) {
        // Retrieve details of the workspace we are dealing with

        QString workspaceUrl = itemsList.first().toMap()["href"].toString().trimmed();

        QString storageValue = QString();
        QString workspaceDescription = QString();
        QString workspaceOwner = QString();
        QString workspaceName = QString();
        QString workspaceId = QString();

        foreach (const QVariant &dataVariant, itemsList.first().toMap()["data"].toList()) {
            QVariantMap dataMap = dataVariant.toMap();
            QString fieldName = dataMap["name"].toString();

            if (!fieldName.compare("storage"))
                storageValue = dataMap["value"].toString();
            else if (!fieldName.compare("description"))
                workspaceDescription = dataMap["value"].toString();
            else if (!fieldName.compare("owner"))
                workspaceOwner = dataMap["value"].toString();
            else if (!fieldName.compare("title"))
                workspaceName = dataMap["value"].toString();
            else if (!fieldName.compare("id"))
                workspaceId = dataMap["value"].toString();
        }

        if (workspaceName.isEmpty())
            workspaceName = workspaceId;

        if (workspaceName.isEmpty())
            workspaceName = tr("** Unknown name **");

        if (!workspaceUrl.isEmpty() && !storageValue.compare("git")) {

            PmrWorkspace *workspace = new PmrWorkspace(workspaceUrl,
                                                       workspaceName,
                                                       workspaceDescription,
                                                       workspaceOwner, this);

            // Return result to requestor

            *workspacePointer = workspace;
        }

    }
}

//==============================================================================

void PmrWebService::workspaceUnauthorised(const QString &pUrl)
{
    Q_UNUSED(pUrl);

    // Do nothing, getWorkspace() will return 0
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
