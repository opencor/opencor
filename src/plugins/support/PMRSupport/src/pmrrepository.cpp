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

#include <QJsonDocument>
#include <QJsonObject>

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

QString PmrRepository::url() const
{
    return "https://models.physiomeproject.org";
}

//==============================================================================

{


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

{

}

//==============================================================================

{
}

//==============================================================================
//==============================================================================

static const char *ExposureProperty   = "Exposure";
static const char *NextActionProperty = "NextAction";
static const char *DirNameProperty    = "DirName";

//==============================================================================











        }
    }

//==============================================================================

QString PmrRepository::informationNoteMessage() const
{
    // Return some information note

    return tr("<strong>Note:</strong> you might want to email <a href=\"mailto: help@physiomeproject.org\">help@physiomeproject.org</a> and ask why this is the case.");
}

//==============================================================================

void PmrRepository::doShowExposureFiles(const QString &pExposureUrl)
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

{









            }


















                    }































            }

        }

    }
}

//==============================================================================

{

}

//==============================================================================

void PmrRepository::requestExposuresList(void)
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

void PmrRepository::cloneWorkspace(const QString &pUrl, const QString &pDirName)
{
    // Check whether we already know about the workspace for the given exposure

    PmrWorkspace *workspace = mWorkspaces.value(pUrl);

    if (workspace && !workspace->url().isEmpty()) {
        emit busy(true);

        workspace->clone(pDirName);

        emit busy(false);
    } else {
        // To clone the workspace associated with the given exposure, we first
        // need to retrieve some information about the exposure itself

        mNumberOfExposureFileUrlsLeft = 0;

        sendPmrRequest(ExposureInformation, pUrl, CloneWorkspace, pDirName);
    }
}

//==============================================================================

void PmrRepository::requestExposureFiles(const QString &pUrl)
{
    // Check whether we already know about the exposure URL for the given
    // exposure

    PmrWorkspace *workspace = mWorkspaces.value(pUrl);
    QString exposureUrl = workspace ? mExposureUrls.value(workspace->url()) : "";

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

void PmrRepository::requestWorkspacesList(void)
{
    // Get the list of the user's workspaces from the PMR
    // after making sure that our
    // internal data has been reset

    //mWorkspaces.clear();

//    if (!mPmrOAuthClient->linked()) {
//        emit information( tr("No exposure file URL could be found for <a href=\"%1\">%2</a>.").arg(pExposureUrl, mExposureNames.value(pExposureUrl))
//                         +"<br/><br/>"+informationNoteMessage());
//        emit << "Application is not linked to PMR!";
//        emit gotWorkspaces();
//        return;
//    }
    sendPmrRequest(WorkspacesList);

}

//==============================================================================

void PmrRepository::requestWorkspaceDetails(const QString &pUrl)
{

    sendPmrRequest(WorkspaceDetails, pUrl);

}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
