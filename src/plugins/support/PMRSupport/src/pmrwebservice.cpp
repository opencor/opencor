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
// PMR web service
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "pmrwebservice.h"
#include "pmrwebservicemanager.h"
#include "pmrwebserviceresponse.h"
#include "pmrworkspacemanager.h"

//==============================================================================

#include <QFileDialog>
#include <QJsonObject>
#include <QMainWindow>

//==============================================================================

#include <QtConcurrent/QtConcurrent>

//==============================================================================

#include "git2/repository.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

PmrWebService::PmrWebService(const QString &pPmrUrl, QObject *pParent) :
    QObject(pParent)
{
    // Some initialisations

    mPmrUrl = pPmrUrl;
    mUrlExposures = QMap<QString, PmrExposure *>();
    mFileExposuresLeftCount = QMap<PmrExposure *, int>();

    // Create a PMR web service manager so that we can retrieve various things
    // from PMR

    mPmrWebServiceManager = new PmrWebServiceManager(pPmrUrl, this);

    // Forward any signal we receive from our PMR web service manager

    connect(mPmrWebServiceManager, SIGNAL(busy(const bool &)),
            this, SIGNAL(busy(const bool &)));
    connect(mPmrWebServiceManager, SIGNAL(authenticated(const bool &)),
            this, SIGNAL(authenticated(const bool &)));
    connect(mPmrWebServiceManager, SIGNAL(error(const QString &)),
            this, SIGNAL(error(const QString &)));
    connect(mPmrWebServiceManager, SIGNAL(authenticationCancelled()),
            this, SIGNAL(authenticationCancelled()));
}

//==============================================================================

PmrWebService::PmrWebService(QObject *pParent) :
    PmrWebService(QString(), pParent)
{
}

//==============================================================================

bool PmrWebService::isAuthenticated() const
{
    // Return whether we are authenticated

    return mPmrWebServiceManager->isAuthenticated();
}

//==============================================================================

void PmrWebService::authenticate(const bool &pAuthenticate)
{
    // Un/authenticate ourselves

    mPmrWebServiceManager->authenticate(pAuthenticate);
}

//==============================================================================

void PmrWebService::requestExposures() const
{
    // Request the list of exposures from PMR

    PmrWebServiceResponse *pmrResponse = mPmrWebServiceManager->request(mPmrUrl+"/exposure", false);

    if (pmrResponse) {
        connect(pmrResponse, SIGNAL(response(const QJsonDocument &)),
                this, SLOT(exposuresResponse(const QJsonDocument &)));
    }
}

//==============================================================================

void PmrWebService::exposuresResponse(const QJsonDocument &pJsonDocument)
{
    // Retrieve the list of exposures from the given PMR response

    PmrExposures exposures = PmrExposures();
    QVariantMap collectionMap = pJsonDocument.object().toVariantMap()["collection"].toMap();

    foreach (const QVariant &links, collectionMap["links"].toList()) {
        QVariantMap linksMap = links.toMap();

        if (!linksMap["rel"].toString().compare("bookmark")) {
            QString exposureUrl = linksMap["href"].toString().trimmed();
            QString exposureName = linksMap["prompt"].toString().simplified();

            if (!exposureUrl.isEmpty() && !exposureName.isEmpty()) {
                PmrExposure *exposure = new PmrExposure(exposureUrl, exposureName, this);

                mUrlExposures.insert(exposureUrl, exposure);

                exposures << exposure;
            }
        }
    }

    std::sort(exposures.begin(), exposures.end(), PmrExposure::compare);

    // Let people know about the list of exposures

    emit PmrWebService::exposures(exposures);
}

//==============================================================================

static const char *PathProperty       = "Path";
static const char *ExposureProperty   = "Exposure";
static const char *NextActionProperty = "NextAction";
static const char *WorkspaceProperty  = "Workspace";

//==============================================================================

PmrWorkspace * PmrWebService::workspace(const QString &pUrl) const
{
    // Retrieve and return the workspace for the given URL

    PmrWebServiceResponse *pmrResponse = mPmrWebServiceManager->request(pUrl, true);

    if (pmrResponse) {
        PmrWorkspace *workspace = 0;

        pmrResponse->setProperty(WorkspaceProperty, QVariant::fromValue((void *) &workspace));

        connect(pmrResponse, SIGNAL(response(const QJsonDocument &)),
                this, SLOT(workspaceResponse(const QJsonDocument &)));

        // Don't return until the PMR response has been processed

        QEventLoop waitLoop;

        connect(pmrResponse, SIGNAL(finished()),
                &waitLoop, SLOT(quit()));

        waitLoop.exec();

        return workspace;
    } else {
        return 0;
    }
}

//==============================================================================

void PmrWebService::workspaceResponse(const QJsonDocument &pJsonDocument)
{
    // Retrieve the workspace from the given PMR response

    PmrWorkspace **workspacePointer = (PmrWorkspace **) sender()->property(WorkspaceProperty).value<void *>();
    QVariantMap collectionMap = pJsonDocument.object().toVariantMap()["collection"].toMap();
    QVariantList itemsList = collectionMap["items"].toList();

    if (itemsList.count()) {
        // Retrieve the details of the workspace we are dealing with

        QString workspaceUrl = itemsList.first().toMap()["href"].toString().trimmed();
        QString storage = QString();
        QString workspaceDescription = QString();
        QString workspaceOwner = QString();
        QString workspaceName = QString();
        QString workspaceId = QString();

        foreach (const QVariant &data, itemsList.first().toMap()["data"].toList()) {
            QVariantMap dataMap = data.toMap();
            QString name = dataMap["name"].toString();

            if (!name.compare("storage"))
                storage = dataMap["value"].toString();
            else if (!name.compare("description"))
                workspaceDescription = dataMap["value"].toString();
            else if (!name.compare("owner"))
                workspaceOwner = dataMap["value"].toString();
            else if (!name.compare("title"))
                workspaceName = dataMap["value"].toString();
            else if (!name.compare("id"))
                workspaceId = dataMap["value"].toString();
        }

        if (workspaceName.isEmpty())
            workspaceName = workspaceId;

        if (workspaceName.isEmpty())
            workspaceName = tr("** Unknown name **");

        if (!workspaceUrl.isEmpty() && !storage.compare("git")) {
            *workspacePointer = new PmrWorkspace(false, workspaceName,
                                                 workspaceUrl,
                                                 workspaceDescription,
                                                 workspaceOwner, this);
        }
    }
}

//==============================================================================

void PmrWebService::requestNewWorkspace(const QString &pName,
                                        const QString &pDescription,
                                        const QString &pPath) const
{
    // Create a new workspace

    static const QString CreateWorkspaceJson = "{ \"template\": { \"data\": ["
                                               "  { \"name\": \"form.widgets.title\", \"value\": \"%1\" },"
                                               "  { \"name\": \"form.widgets.description\", \"value\": \"%2\" },"
                                               "  { \"name\": \"form.widgets.storage\", \"value\": \"git\" },"
                                               "  { \"name\": \"form.buttons.add\", \"value\": \"Add\" }"
                                               "] } }";

    QJsonDocument createWorkspaceJson = QJsonDocument::fromJson(QString(CreateWorkspaceJson).arg(pName, pDescription).toUtf8());
    PmrWebServiceResponse *pmrResponse = mPmrWebServiceManager->request(mPmrUrl+"/workspace/+/addWorkspace",
                                                                        true, true, createWorkspaceJson);

    if (pmrResponse) {
        pmrResponse->setProperty(PathProperty, pPath);

        connect(pmrResponse, SIGNAL(found(const QString &)),
                this, SLOT(newWorkspaceResponse(const QString &)));
    }
}

//==============================================================================

void PmrWebService::newWorkspaceResponse(const QString &pUrl)
{
    // Let people know that the workspace, which URL is given, has been created

    emit workspaceCreated(pUrl);

    // Now that the workspace has been created, we can request its information
    // Note: a non-empty path will also clone the workspace...

    requestWorkspaceInformation(pUrl, sender()->property(PathProperty).toString());
}

//==============================================================================

void PmrWebService::requestWorkspaces() const
{
    // Retrieve all the workspaces

    PmrWebServiceResponse *pmrResponse = mPmrWebServiceManager->request(mPmrUrl+"/my-workspaces", true);

    if (pmrResponse) {
        connect(pmrResponse, SIGNAL(response(const QJsonDocument &)),
                this, SLOT(workspacesResponse(const QJsonDocument &)));
    }
}

//==============================================================================

void PmrWebService::workspacesResponse(const QJsonDocument &pJsonDocument)
{
    // Retrieve the list of workspaces from the given PMR response

    PmrWorkspaces workspaces = PmrWorkspaces();
    QVariantMap collectionMap = pJsonDocument.object().toVariantMap()["collection"].toMap();

    foreach (const QVariant &links, collectionMap["links"].toList()) {
        QVariantMap linksMap = links.toMap();

        if (!linksMap["rel"].toString().compare("bookmark")) {
            QString workspaceUrl = linksMap["href"].toString().trimmed();
            QString workspaceName = linksMap["prompt"].toString().simplified();

            if (!workspaceUrl.isEmpty() && !workspaceName.isEmpty()) {
                workspaces << new PmrWorkspace(true, workspaceName,
                                               workspaceUrl, this);
            }
        }
    }

    std::sort(workspaces.begin(), workspaces.end(), PmrWorkspace::compare);

    // Let people know about the list of workspaces

    emit PmrWebService::workspaces(workspaces);
}

//==============================================================================

void PmrWebService::requestWorkspaceInformation(const QString &pUrl,
                                                const QString &pPath,
                                                PmrExposure *pExposure)
{
    // Retrieve some information about the workspace, which URL is given, and
    // clone it (if the given path is non-empty or the given exposure is
    // non-null)

    PmrWebServiceResponse *pmrResponse = mPmrWebServiceManager->request(pUrl, true);

    if (pmrResponse) {
        pmrResponse->setProperty(ExposureProperty, QVariant::fromValue((void *) pExposure));
        pmrResponse->setProperty(PathProperty, pPath);

        connect(pmrResponse, SIGNAL(response(const QJsonDocument &)),
                this, SLOT(workspaceInformationResponse(const QJsonDocument &)));
    }
}

//==============================================================================

void PmrWebService::workspaceInformationResponse(const QJsonDocument &pJsonDocument)
{
    // Retrieve the workspace information from the given PMR response

    QVariantMap collectionMap = pJsonDocument.object().toVariantMap()["collection"].toMap();
    QVariantList itemsList = collectionMap["items"].toList();

    if (itemsList.count()) {
        // Retrieve the details of the workspace we are dealing with

        QString workspaceUrl = itemsList.first().toMap()["href"].toString().trimmed();
        QString storage = QString();
        QString workspaceDescription = QString();
        QString workspaceOwner = QString();
        QString workspaceName = QString();

        foreach (const QVariant &data, itemsList.first().toMap()["data"].toList()) {
            QVariantMap dataMap = data.toMap();

            if (!dataMap["name"].toString().compare("storage"))
                storage = dataMap["value"].toString();
            else if (!dataMap["name"].toString().compare("description"))
                workspaceDescription = dataMap["value"].toString();
            else if (!dataMap["name"].toString().compare("owner"))
                workspaceOwner = dataMap["value"].toString();
            else if (!dataMap["name"].toString().compare("title"))
                workspaceName = dataMap["value"].toString();
        }

        // Retrieve the exposure

        PmrExposure *exposure = (PmrExposure *) sender()->property(ExposureProperty).value<void *>();

        if (!workspaceUrl.isEmpty()) {
            // Make sure that our workspace is a Git repository

            if (!storage.compare("git")) {
                PmrWorkspace *workspace = new PmrWorkspace(!exposure,
                                                           workspaceName,
                                                           workspaceUrl,
                                                           workspaceDescription,
                                                           workspaceOwner,
                                                           this);
                QString dirName = QString();

                if (exposure) {
                    // Cloning a workspace from an exposure

                    exposure->setWorkspace(workspace);

                    // Check that we aren't already managing a clone of the
                    // workspace

                    PmrWorkspace *existingWorkspace = PmrWorkspaceManager::instance()->workspace(workspaceUrl);

                    if (!existingWorkspace) {
                        // Retrieve the name of an empty directory

                        dirName = getEmptyDirectory();
                    } else {
                        emit warning(tr("The workspace for %1 is already cloned in %2.").arg(workspaceUrl,
                                                                                             existingWorkspace->path()));
                    }
                } else {
                    // Cloning after creating a new (owned) workspace

                    dirName = sender()->property(PathProperty).toString();
                }

                // Clone the workspace, if we have an empty directory

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

void PmrWebService::requestWorkspaceClone(PmrWorkspace *pWorkspace,
                                          const QString &pPath)
{
    // Let people know that we are (going to be) busy

    emit busy(true);

    // Get credentials for the given workspace

    requestWorkspaceCredentials(pWorkspace);

    // Clone the given workspace to the given path

    connect(pWorkspace, SIGNAL(warning(const QString &)),
            this, SLOT(workspaceErrored()));
    connect(pWorkspace, SIGNAL(workspaceCloned(PMRSupport::PmrWorkspace *)),
            this, SLOT(workspaceCloneFinished(PMRSupport::PmrWorkspace *)));

    QtConcurrent::run(pWorkspace, &PmrWorkspace::clone, pPath);
}

//==============================================================================

void PmrWebService::workspaceErrored()
{
    // Let people know that we are not busy anymore

    emit busy(false);
}

//==============================================================================

void PmrWebService::workspaceCloneFinished(PMRSupport::PmrWorkspace *pWorkspace)
{
    // Let people know that we are not busy anymore and that a workspace has
    // been cloned

    emit busy(false);
    emit workspaceCloned(pWorkspace);
}

//==============================================================================

void PmrWebService::requestWorkspaceSynchronize(PmrWorkspace *pWorkspace,
                                                const bool &pPush)
{
    // Let people know that we are (going to be) busy

    emit busy(true);

    // Get credentials for the given workspace

    requestWorkspaceCredentials(pWorkspace);

    // Synchronise the given workspace

    connect(pWorkspace, SIGNAL(workspaceSynchronized(PMRSupport::PmrWorkspace *)),
            this, SLOT(workspaceSynchronizeFinished(PMRSupport::PmrWorkspace *)));

    QtConcurrent::run(pWorkspace, &PmrWorkspace::synchronize, pPush);
}

//==============================================================================

void PmrWebService::workspaceSynchronizeFinished(PMRSupport::PmrWorkspace *pWorkspace)
{
    // Let people know that we are not busy anymore and that the given workspace
    // has been synchronised

    emit busy(false);
    emit workspaceSynchronized(pWorkspace);
}

//==============================================================================

void PmrWebService::update(const QString &pPmrUrl)
{
    // Keep track of the new PMR URL and then update our PMR web service manager

    mPmrUrl = pPmrUrl;

    mPmrWebServiceManager->update(pPmrUrl);
}

//==============================================================================

QString PmrWebService::siteName() const
{
    // Determine and return the name of our PMR site

    return tr("%1 Site").arg(Core::formatMessage(QUrl(mPmrUrl).host().split('.').first(), false));
}

//==============================================================================

QString PmrWebService::getEmptyDirectory()
{
    // Retrieve and return the name of an empty directory

    return Core::getEmptyDirectory(tr("Select Empty Directory"));
}

//==============================================================================

QString PmrWebService::getNonGitDirectory()
{
    // Retrieve and return the name of a non-Git directory

    QFileDialog dialog(Core::mainWindow(), tr("Select Directory"),
                       Core::activeDirectory());

    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setOption(QFileDialog::ShowDirsOnly);

    forever {
        if (dialog.exec() != QDialog::Accepted)
            break;

        QString res = Core::nativeCanonicalDirName(dialog.selectedFiles().first());

        if (!res.isEmpty()) {
            // We have retrieved a file name, so update our active directory

            Core::setActiveDirectory(res);

            // Check whether the directory is a Git directory

            if (isGitDirectory(res)) {
                Core::warningMessageBox(tr("Select Directory"),
                                        tr("Please choose a non-Git directory."));

                continue;
            }
        }

        return res;
    }

    return QString();
}

//==============================================================================

bool PmrWebService::isGitDirectory(const QString &pDirName)
{
    // Return whether the given directory is a Git directory

    if (pDirName.isEmpty()) {
        return false;
    } else {
        git_repository *gitRepository = 0;

        if (!git_repository_open(&gitRepository, pDirName.toUtf8().constData())) {
            git_repository_free(gitRepository);

            return true;
        } else {
            return false;
        }
    }
}

//==============================================================================

void PmrWebService::requestWorkspaceCredentials(PmrWorkspace *pWorkspace)
{
    // Request credentials for the given workspace

    PmrWebServiceResponse *pmrResponse = mPmrWebServiceManager->request(pWorkspace->url()+"/request_temporary_password",
                                                                        true, true);

    if (pmrResponse) {
        pmrResponse->setProperty(WorkspaceProperty, QVariant::fromValue((void *) pWorkspace));

        connect(pmrResponse, SIGNAL(response(const QJsonDocument &)),
                this, SLOT(workspaceCredentialsResponse(const QJsonDocument &)));

        // Don't return until the PMR response has been processed

        QEventLoop waitLoop;

        connect(pmrResponse, SIGNAL(finished()),
                &waitLoop, SLOT(quit()));

        waitLoop.exec();
    }
}

//==============================================================================

void PmrWebService::workspaceCredentialsResponse(const QJsonDocument &pJsonDocument)
{
    // Retrieve the credentials from the given PMR response

    QVariantMap jsonResponse = pJsonDocument.object().toVariantMap();
    PmrWorkspace *workspace = (PmrWorkspace *) sender()->property(WorkspaceProperty).value<void *>();

    if (workspace && !jsonResponse["target"].toString().compare(workspace->url()))
        workspace->setCredentials(jsonResponse["user"].toString(), jsonResponse["key"].toString());
}

//==============================================================================

void PmrWebService::requestExposureFileInformation(const QString &pUrl,
                                                   PmrExposure *pExposure)
{
    // Retrieve some information about the exposure file, which URL is given

    PmrWebServiceResponse *pmrResponse = mPmrWebServiceManager->request(pUrl, false);

    if (pmrResponse) {
        pmrResponse->setProperty(ExposureProperty, QVariant::fromValue((void *) pExposure));

        connect(pmrResponse, SIGNAL(response(const QJsonDocument &)),
                this, SLOT(exposureFileInformationResponse(const QJsonDocument &)));
    }
}

//==============================================================================

void PmrWebService::exposureFileInformationResponse(const QJsonDocument &pJsonDocument)
{
    // Retrieve some information about an exposure file

    PmrExposure *exposure = (PmrExposure *) sender()->property(ExposureProperty).value<void *>();

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

                    foreach (const QVariant &links, collectionMap["links"].toList()) {
                        QVariantMap linksMap = links.toMap();
                        QString prompt = linksMap["prompt"].toString();
                        QString rel = linksMap["rel"].toString();

                        if (   !prompt.compare("Launch with OpenCOR")
                            && !rel.compare("section")) {
                            // Our exposure file has a link called
                            // "Launch with OpenCOR", so check whether its href
                            // value is already listed in our list of exposure
                            // files and, if not, add it to it

                            exposureFile = linksMap["href"].toString().trimmed().remove("opencor://openFile/");

                            if (!exposureFile.isEmpty())
                                exposure->addExposureFile(exposureFile);
                        }
                    }

                    // Let people know that exposure files are available, should
                    // we have no exposure file URLs left to handle

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

void PmrWebService::emitInformation(const QString &pMessage)
{
    // Let people know about the given information message

    emit information( pMessage+"<br/><br/>"
                     +tr("<strong>Note:</strong> you might want to email <a href=\"mailto: help@physiomeproject.org\">help@physiomeproject.org</a> and ask why this is the case."));
}

//==============================================================================

void PmrWebService::forbidden(const QString &pUrl)
{
    // Let people know that access to the given URL is forbidden

    emitInformation(tr("Access to %1 is forbidden.").arg(pUrl));
}

//==============================================================================

void PmrWebService::requestExposureFiles(const QString &pUrl)
{
    // Request some information about the exposure, which URL is given, and then
    // request the coorresponding exposure files

    PmrExposure *exposure = mUrlExposures.value(pUrl);
    PmrWebServiceResponse *pmrResponse = mPmrWebServiceManager->request(exposure->url(), false);

    if (pmrResponse) {
        pmrResponse->setProperty(ExposureProperty, QVariant::fromValue((void *) exposure));
        pmrResponse->setProperty(NextActionProperty, RequestExposureFiles);

        connect(pmrResponse, SIGNAL(response(const QJsonDocument &)),
                this, SLOT(exposureInformationResponse(const QJsonDocument &)));
    }
}

//==============================================================================

void PmrWebService::exposureInformationResponse(const QJsonDocument &pJsonDocument)
{
    // Retrieve some information about an exposure from the given PMR response,
    // and then clone it (if requested) and retrieve some information for all
    // the corresponding exposure files

    PmrExposure *exposure = (PmrExposure *) sender()->property(ExposureProperty).value<void *>();

    if (exposure) {
        // Retrieve the URLs that will help us to retrieve some information
        // about the exposure's workspace and its exposure files

        QVariantMap collectionMap = pJsonDocument.object().toVariantMap()["collection"].toMap();
        QString workspaceUrl = QString();
        QStringList exposureFileUrls = QStringList();

        foreach (const QVariant &links, collectionMap["links"].toList()) {
            QVariantMap linksMap = links.toMap();
            QString rel = linksMap["rel"].toString();

            if (!rel.compare("via")) {
                workspaceUrl = linksMap["href"].toString().trimmed();
            } else if (!rel.compare("bookmark")) {
                QString exposureFileUrl = linksMap["href"].toString().trimmed();

                if (!exposureFileUrl.isEmpty())
                    exposureFileUrls << exposureFileUrl;
            }
        }

        mFileExposuresLeftCount.insert(exposure, exposureFileUrls.count());

        // Make sure that we at least have a workspace

        if (workspaceUrl.isEmpty()) {
            emitInformation(tr("No workspace could be found for %1.").arg(exposure->toHtml()));
        } else {
            Action action = Action(sender()->property(NextActionProperty).toInt());

            if (action == CloneExposureWorkspace) {
                // Retrieve some information about the workspace and then clone
                // it

                requestWorkspaceInformation(workspaceUrl, QString(), exposure);
            } else if (   exposureFileUrls.isEmpty()
                       && (action == RequestExposureFiles)) {
                emitInformation(tr("No exposure files could be found for %1.").arg(exposure->toHtml()));
            }
        }

        // Retrieve some information about the exposure files, but only if it
        // hasn't already been done

        if (exposure->exposureFiles().isEmpty()) {
            foreach (const QString &exposureFileUrl, exposureFileUrls)
                requestExposureFileInformation(exposureFileUrl, exposure);
        }
    }
}

//==============================================================================

void PmrWebService::requestExposureWorkspaceClone(const QString &pUrl)
{
    // Ask for the workspace associated with the given exposure to be cloned,
    // but only if we don't already know about that workspace

    PmrExposure *exposure = mUrlExposures.value(pUrl);

    if (exposure->workspace()) {
        QString url = exposure->workspace()->url();
        QString dirName = exposure->workspace()->path();

        // Check that we aren't already managing a clone of the workspace

        if (!dirName.isEmpty()) {
            emit warning(tr("The workspace for %1 is already cloned in %2.").arg(url, dirName));
        } else {
            PmrWorkspace *existingWorkspace = PmrWorkspaceManager::instance()->workspace(url);

            if (!existingWorkspace) {
                // Retrieve the name of an empty directory

                dirName = getEmptyDirectory();

                if (!dirName.isEmpty())
                    requestWorkspaceClone(exposure->workspace(), dirName);
            } else {
                emit warning(tr("The workspace for %1 is already cloned in %2.").arg(url, existingWorkspace->path()));
            }
        }
    } else {
        // To clone the workspace associated with the given exposure, we first
        // need to retrieve some information about the exposure itself

        PmrWebServiceResponse *pmrResponse = mPmrWebServiceManager->request(pUrl, false);

        if (pmrResponse) {
            pmrResponse->setProperty(ExposureProperty, QVariant::fromValue((void *) exposure));
            pmrResponse->setProperty(NextActionProperty, CloneExposureWorkspace);

            connect(pmrResponse, SIGNAL(response(const QJsonDocument &)),
                    this, SLOT(exposureInformationResponse(const QJsonDocument &)));
        }
    }
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
