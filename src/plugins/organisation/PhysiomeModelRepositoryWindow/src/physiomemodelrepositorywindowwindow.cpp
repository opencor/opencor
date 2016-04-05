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
// Physiome Model Repository window
//==============================================================================

#include "borderedwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "physiomemodelrepositorywindowwidget.h"
#include "physiomemodelrepositorywindowwindow.h"

//==============================================================================

#include "ui_physiomemodelrepositorywindowwindow.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>

//==============================================================================

#include "git2.h"
#include "zlib.h"

//==============================================================================

namespace OpenCOR {
namespace PhysiomeModelRepositoryWindow {

//==============================================================================

PhysiomeModelRepositoryWindowWindow::PhysiomeModelRepositoryWindowWindow(QWidget *pParent) :
    Core::OrganisationWidget(pParent),
    mGui(new Ui::PhysiomeModelRepositoryWindowWindow),
    mNumberOfExposureFileUrlsLeft(0),
    mWorkspaces(QMap<QString, QString>()),
    mExposureUrls(QMap<QString, QString>()),
    mExposureNames(QMap<QString, QString>()),
    mExposureFileNames(QMap<QString, QString>())
{
    // Set up the GUI

    mGui->setupUi(this);

#ifdef Q_OS_MAC
    mGui->filterValue->setAttribute(Qt::WA_MacShowFocusRect, false);
    // Note: the above removes the focus border since it messes up the look of
    //       our filter value widget...
#endif

    // Make the name value our focus proxy

    setFocusProxy(mGui->filterValue);

    // Create and add the PMR widget

    mPhysiomeModelRepositoryWidget = new PhysiomeModelRepositoryWindowWidget(this);

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mGui->dockWidgetContents->layout()->addWidget(new Core::BorderedWidget(mPhysiomeModelRepositoryWidget,
                                                                           true, true, true, true));
#elif defined(Q_OS_MAC)
    mGui->dockWidgetContents->layout()->addWidget(new Core::BorderedWidget(mPhysiomeModelRepositoryWidget,
                                                                           true, false, false, false));
#else
    #error Unsupported platform
#endif

    // Keep track of the window's visibility, so that we can request the list of
    // exposures, if necessary

    connect(this, SIGNAL(visibilityChanged(bool)),
            this, SLOT(retrieveExposuresList(const bool &)));

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

    // Some connections to know what our PMR widget wants from us

    connect(mPhysiomeModelRepositoryWidget, SIGNAL(cloneWorkspaceRequested(const QString &)),
            this, SLOT(cloneWorkspace(const QString &)));
    connect(mPhysiomeModelRepositoryWidget, SIGNAL(showExposureFilesRequested(const QString &)),
            this, SLOT(showExposureFiles(const QString &)));

    connect(mPhysiomeModelRepositoryWidget, SIGNAL(openExposureFileRequested(const QString &)),
            this, SLOT(openFile(const QString &)));

    // Some further initialisations that are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();
}

//==============================================================================

PhysiomeModelRepositoryWindowWindow::~PhysiomeModelRepositoryWindowWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::retranslateUi()
{
    // Retranslate the whole window and our information note message

    mGui->retranslateUi(this);

    mInformationNoteMessage = tr("<strong>Note:</strong> you might want to email <a href=\"mailto: help@physiomeproject.org\">help@physiomeproject.org</a> and ask why this is the case.");

    // Retranslate our PMR widget

    mPhysiomeModelRepositoryWidget->retranslateUi();
}

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::busy(const bool &pBusy)
{
    // Show ourselves as busy or not busy anymore

    static int counter = 0;

    counter += pBusy?1:-1;

    if (pBusy && (counter == 1)) {
        showSemiTransparentBusyWidget(mPhysiomeModelRepositoryWidget);
        // Note: the semi-transparency is to 'show' that our widget cannot be
        //       used since disabling it (in a Qt way) doesn't grey it out...

        mGui->dockWidgetContents->setEnabled(false);
    } else if (!pBusy && !counter) {
        // Re-enable the GUI side and give, within the current window, the focus
        // to mGui->filterValue, but only if the current window already has the
        // focus

        hideBusyWidget();

        mGui->dockWidgetContents->setEnabled(true);

        Core::setFocusTo(mGui->filterValue);
    }
}

//==============================================================================

static const char *PmrRequestProperty = "PmrRequest";
static const char *ActionProperty     = "Action";

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::sendPmrRequest(const PmrRequest &pPmrRequest,
                                                         const QString &pUrl,
                                                         const Action pAction)
{
    // Let the user know that we are busy

    busy(true);

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
    } else {
        finished();
    }
}

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::doCloneWorkspace(const QString &pWorkspace)
{
    // Retrieve the name of an empty directory

    QString dirName = Core::getExistingDirectory(tr("Select Empty Directory"),
                                                 QString(), true);

    if (!dirName.isEmpty()) {
        // We have got a directory name, so before cloning the workspace, make
        // sure that the dialog box has gone (indeed, otherwise, on OS X at
        // least, we will see a blank area where the dialog box used to be)
        // Note: this is a temporary solution until we implement issue #848...

        QCoreApplication::processEvents();

        // We have got a directory name where we can clone the workspace, so
        // clone it

        git_libgit2_init();

        git_repository *gitRepository = 0;
        QByteArray workspaceByteArray = pWorkspace.toUtf8();
        QByteArray dirNameByteArray = dirName.toUtf8();

        int res = git_clone(&gitRepository, workspaceByteArray.constData(),
                            dirNameByteArray.constData(), 0);

        if (res) {
            const git_error *gitError = giterr_last();

            QMessageBox::warning(Core::mainWindow(), tr("Clone Workspace"),
                                 gitError?
                                     tr("Error %1: %2.").arg(QString::number(gitError->klass),
                                                             Core::formatMessage(gitError->message)):
                                     tr("An error occurred while trying to clone the workspace."),
                                 QMessageBox::Ok);
        } else if (gitRepository) {
            git_repository_free(gitRepository);
        }

        git_libgit2_shutdown();
    }
}

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::doShowExposureFiles(const QString &pExposureUrl)
{
    // Show the exposure files, but only if there are some

    if (!mExposureFileNames.values(pExposureUrl).isEmpty()) {
        mPhysiomeModelRepositoryWidget->showExposureFiles(pExposureUrl);
    } else {
        QMessageBox::information( Core::mainWindow(), windowTitle(),
                                  tr("No exposure file URL could be found for <a href=\"%1\">%2</a>.").arg(pExposureUrl, mExposureNames.value(pExposureUrl))
                                 +"<br/><br/>"+mInformationNoteMessage,
                                  QMessageBox::Ok);
    }
}

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::on_filterValue_textChanged(const QString &pText)
{
    // Ask our PMR widget to filter its output using the given regular
    // expression

    mPhysiomeModelRepositoryWidget->filter(pText);
}

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::on_refreshButton_clicked()
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

bool sortExposureFiles(const QString &pExposureFile1,
                       const QString &pExposureFile2)
{
    // Determine which of the two exposure files should be first (without
    // worrying about casing)

    return pExposureFile1.compare(pExposureFile2, Qt::CaseInsensitive) < 0;
}

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::finished(QNetworkReply *pNetworkReply)
{
    // Check whether our PMR request was successful

    PmrRequest pmrRequest = pNetworkReply?PmrRequest(pNetworkReply->property(PmrRequestProperty).toInt()):ExposuresList;
    bool internetConnectionAvailable = true;
    QString errorMessage = QString();
    QString informationMessage = QString();
    PhysiomeModelRepositoryWindowExposures exposures = PhysiomeModelRepositoryWindowExposures();
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

                                exposures << PhysiomeModelRepositoryWindowExposure(exposureUrl, exposureName);
                            }
                        }
                    }

                    std::sort(exposures.begin(), exposures.end());

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

                                    std::sort(exposureFileNames.begin(), exposureFileNames.end(), sortExposureFiles);

                                    mPhysiomeModelRepositoryWidget->addExposureFiles(exposureUrl, exposureFileNames);
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
                    QMessageBox::information( Core::mainWindow(), windowTitle(),
                                              informationMessage.arg(exposureUrl, mExposureNames.value(exposureUrl))
                                             +"<br/><br/>"+mInformationNoteMessage,
                                              QMessageBox::Ok);
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
        // Ask our PMR widget to initialise itself

        mPhysiomeModelRepositoryWidget->initialize(exposures, errorMessage,
                                                   mGui->filterValue->text(),
                                                   internetConnectionAvailable);

        break;
    case ExposureInformation:
        // No additional processing needed

        break;
    case WorkspaceInformation:
        // Clone the workspace, if possible and requested

        if (   mWorkspaces.contains(exposureUrl)
            && (Action(pNetworkReply->property(ActionProperty).toInt()) == CloneWorkspace)) {
            doCloneWorkspace(mWorkspaces.value(exposureUrl));
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

    busy(false);

    // Delete (later) the network reply

    if (pNetworkReply)
        pNetworkReply->deleteLater();
}

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::sslErrors(QNetworkReply *pNetworkReply,
                                                    const QList<QSslError> &pSslErrors)
{
    // Ignore the SSL errors since we trust the website and therefore its
    // certificate (even if it is invalid, e.g. it has expired)

    pNetworkReply->ignoreSslErrors(pSslErrors);
}

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::retrieveExposuresList(const bool &pVisible)
{
    // Retrieve the list of exposures, if we are becoming visible and the list
    // of exposures has never been requested before (through a single shot, this
    // to allow other events, such as the one asking OpenCOR's main window to
    // resize itself, to be handled properly)

    static bool firstTime = true;

    if (pVisible && firstTime) {
        firstTime = false;

        QTimer::singleShot(0, this, SLOT(on_refreshButton_clicked()));
    }
}

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::cloneWorkspace(const QString &pUrl)
{
    // Check whether we already know about the workspace for the given exposure

    QString workspace = mWorkspaces.value(pUrl);

    if (!workspace.isEmpty()) {
        busy(true);

        doCloneWorkspace(workspace);

        busy(false);
    } else {
        // To clone the workspace associated with the given exposure, we first
        // need to retrieve some information about the exposure itself

        mNumberOfExposureFileUrlsLeft = 0;

        sendPmrRequest(ExposureInformation, pUrl, CloneWorkspace);
    }
}

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::showExposureFiles(const QString &pUrl)
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

}   // namespace PhysiomeModelRepositoryWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
