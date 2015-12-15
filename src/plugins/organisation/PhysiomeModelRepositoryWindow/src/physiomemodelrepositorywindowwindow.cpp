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
    mNumberOfExposureFilesLeft(0),
    mWorkspaces(QMap<QString, QString>()),
    mExposureFiles(QMap<QString, QString>())
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

    mGui->dockWidgetContents->layout()->addWidget(mPhysiomeModelRepositoryWidget);

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

    connect(mPhysiomeModelRepositoryWidget, SIGNAL(cloneWorkspace(const QString &, const QString &)),
            this, SLOT(cloneWorkspace(const QString &, const QString &)));
    connect(mPhysiomeModelRepositoryWidget, SIGNAL(showExposureFiles(const QString &, const QString &)),
            this, SLOT(showExposureFiles(const QString &, const QString &)));

    connect(mPhysiomeModelRepositoryWidget, SIGNAL(exposureFileOpenRequested(const QString &)),
            this, SLOT(openFile(const QString &)));
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
    // Retranslate the whole window

    mGui->retranslateUi(this);

    // Retranslate our PMR widget

    mPhysiomeModelRepositoryWidget->retranslateUi();
}

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::busy(const bool &pBusy)
{
    // Show ourselves as busy or not busy anymore

    if (pBusy) {
        showBusyWidget(mPhysiomeModelRepositoryWidget);

        mGui->dockWidgetContents->setEnabled(false);
    } else {
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
static const char *ExtraProperty      = "Extra";

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::sendPmrRequest(const PmrRequest &pPmrRequest,
                                                         const QString &pUrl,
                                                         const QString &pExtra)
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
        case BookmarkUrlsForCloning:
        case BookmarkUrlsForExposureFiles:
        case ExposureFileForCloning:
        case ExposureFileForExposureFiles:
            networkRequest.setUrl(QUrl(pUrl));

            break;
        default:   // ExposuresList
            networkRequest.setUrl(QUrl("https://models.physiomeproject.org/exposure"));
        }

        QNetworkReply *networkReply = mNetworkAccessManager->get(networkRequest);

        // Keep track of the type of our PMR request and of the given extra
        // information

        networkReply->setProperty(PmrRequestProperty, pPmrRequest);
        networkReply->setProperty(ExtraProperty, pExtra);
    } else {
        finished();
    }
}

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::cloneWorkspace(const QString &pWorkspace)
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

void PhysiomeModelRepositoryWindowWindow::on_filterValue_textChanged(const QString &pText)
{
    // Ask our PMR widget to filter its output using the given regular
    // expression

    mPhysiomeModelRepositoryWidget->filter(pText);
}

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::on_refreshButton_clicked()
{
    // Get the list of exposures from the PMR

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

    PhysiomeModelRepositoryWindowExposures exposures = PhysiomeModelRepositoryWindowExposures();
    QString exposureFile = QString();
    QString errorMessage = QString();
    QStringList bookmarkUrls = QStringList();
    bool internetConnectionAvailable = true;

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

            if (jsonParseError.error == QJsonParseError::NoError) {
                // Check our PMR request to determine what we should do with the
                // data

                QVariantMap collectionMap = jsonDocument.object().toVariantMap()["collection"].toMap();

                switch (pmrRequest) {
                case BookmarkUrlsForCloning:
                case BookmarkUrlsForExposureFiles:
                    foreach (const QVariant &linkVariant, collectionMap["links"].toList())
                        bookmarkUrls << linkVariant.toMap()["href"].toString().trimmed();

                    mNumberOfExposureFilesLeft = bookmarkUrls.count();

                    break;
                case ExposureFileForCloning:
                case ExposureFileForExposureFiles: {
                    exposureFile = collectionMap["items"].toList().first().toMap()["links"].toList().first().toMap()["href"].toString().trimmed();

                    --mNumberOfExposureFilesLeft;

                    break;
                }
                default:   // ExposuresList
                    // Retrieve the list of exposures

                    foreach (const QVariant &linkVariant, collectionMap["links"].toList()) {
                        QVariantMap linkMap = linkVariant.toMap();

                        exposures << PhysiomeModelRepositoryWindowExposure(linkMap["href"].toString().trimmed(),
                                                                           linkMap["prompt"].toString().trimmed().replace("\n", " ").replace("  ", " "));
                        // Note: the prompt may contain some '\n', so we want to
                        //       remove them, which in turn may mean that it will
                        //       contain two consecutive spaces (should we have had
                        //       something like "xxx \nyyy"), which we want to
                        //       replace with only one of them...
                    }
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
    case BookmarkUrlsForCloning:
    case BookmarkUrlsForExposureFiles:
        // Make sure that we have got at least one bookmark URL and retrieve the
        // corresponding exposure file(s) from the PMR

        if (bookmarkUrls.isEmpty()) {
            QString url = pNetworkReply->url().toString();

            QMessageBox::information( Core::mainWindow(), tr("Bookmark URLs"),
                                      tr("No bookmark URL could be found for <a href=\"%1\">%2</a>.").arg(url, pNetworkReply->property(ExtraProperty).toString())
                                     +"<br/><br/>"+tr("<strong>Note:</strong> you might want to email <a href=\"mailto: help@physiomeproject.org\">help@physiomeproject.org</a> and ask why this is the case."),
                                      QMessageBox::Ok);
        } else {
            QString url = pNetworkReply->url().toString();

            foreach (const QString &bookmarkUrl, bookmarkUrls) {
                sendPmrRequest((pmrRequest == BookmarkUrlsForCloning)?
                                   ExposureFileForCloning:
                                   ExposureFileForExposureFiles,
                               bookmarkUrl, url);
            }
        }

        break;
    case ExposureFileForCloning:
    case ExposureFileForExposureFiles: {
        // Keep track of the exposure file

        QString url = pNetworkReply->property(ExtraProperty).toString();

        mExposureFiles.insertMulti(url, exposureFile);

        // Determine the URL of the workspace for the exposure, should we have
        // retrieved all of its exposure files, and ask our PMR widget to add
        // some exposure files and maybe also show them
        // Note: this can be done with any exposure file, but we do it with the
        //       last one in case of a problem occuring between the retrieval of
        //       the first and last exposure file...

        if (!mNumberOfExposureFilesLeft) {
            static const QRegularExpression RawFileInfoRegEx = QRegularExpression("/rawfile/.*$");

            QStringList exposureFiles = mExposureFiles.values(url);

            std::sort(exposureFiles.begin(), exposureFiles.end(), sortExposureFiles);

            mWorkspaces.insert(url, exposureFile.remove(RawFileInfoRegEx));
            mPhysiomeModelRepositoryWidget->addExposureFiles(url, exposureFiles);

            if (pmrRequest == ExposureFileForExposureFiles)
                mPhysiomeModelRepositoryWidget->showExposureFiles(url);

            // Remove the exposure source files since we don't need them anymore

            mExposureFiles.remove(url);
        }

        // Clone the workspace, if possible and requested

        if (!mNumberOfExposureFilesLeft && (pmrRequest == ExposureFileForCloning))
            cloneWorkspace(mWorkspaces.value(url));

        break;
    }
    default:   // ExposuresList
        // Make sure that our exposures are sorted alphabetically

        std::sort(exposures.begin(), exposures.end());

        // Ask our PMR widget to initialise itself and filter its output

        mPhysiomeModelRepositoryWidget->initialize(exposures, errorMessage,
                                                   internetConnectionAvailable);
        mPhysiomeModelRepositoryWidget->filter(mGui->filterValue->text());
    }

    // Show ourselves as not busy anymore, but only under certain conditions

    if (        (pmrRequest == ExposuresList)
        || ((   (pmrRequest == BookmarkUrlsForCloning)
             || (pmrRequest == BookmarkUrlsForExposureFiles)) &&  bookmarkUrls.isEmpty())
        || ((   (pmrRequest == ExposureFileForCloning)
             || (pmrRequest == ExposureFileForExposureFiles)) && !mNumberOfExposureFilesLeft)) {
        busy(false);
    }

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

void PhysiomeModelRepositoryWindowWindow::cloneWorkspace(const QString &pUrl,
                                                         const QString &pDescription)
{
    // Check whether we already know about the workspace for the given exposure

    QString workspace = mWorkspaces.value(pUrl);

    if (!workspace.isEmpty()) {
        busy(true);

        cloneWorkspace(workspace);

        busy(false);
    } else {
        // To retrieve the workspace associated with the given exposure, we
        // first need to retrieve its bookmark URLs

        sendPmrRequest(BookmarkUrlsForCloning, pUrl, pDescription);
    }
}

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::showExposureFiles(const QString &pUrl,
                                                            const QString &pDescription)
{
    // To show exposure files, we first need to retrieve the bookmark URLs for
    // the given exposure

    sendPmrRequest(BookmarkUrlsForExposureFiles, pUrl, pDescription);
}

//==============================================================================

}   // namespace PhysiomeModelRepositoryWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
