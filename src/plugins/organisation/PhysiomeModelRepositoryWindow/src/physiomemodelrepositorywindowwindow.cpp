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

#include "physiomemodelrepositorywindowwindow.h"
#include "physiomemodelrepositorywindowwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"

//==============================================================================

#include "ui_physiomemodelrepositorywindowwindow.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

//==============================================================================

#include "git2.h"

//==============================================================================

namespace OpenCOR {
namespace PhysiomeModelRepositoryWindow {

//==============================================================================

PhysiomeModelRepositoryWindowWindow::PhysiomeModelRepositoryWindowWindow(QWidget *pParent) :
    Core::OrganisationWidget(pParent),
    mGui(new Ui::PhysiomeModelRepositoryWindowWindow),
    mNumberOfUntreatedSourceFiles(0),
    mWorkspaces(QMap<QString, QString>()),
    mSourceFiles(QMap<QString, QString>())
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

    // Create and add the Physiome Model Repository widget

    mPhysiomeModelRepositoryWidget = new PhysiomeModelRepositoryWindowWidget(this);

    mGui->dockWidgetContents->layout()->addWidget(mPhysiomeModelRepositoryWidget);

    // Keep track of the window's visibility, so that we can request the list of
    // models, if necessary

    connect(this, SIGNAL(visibilityChanged(bool)),
            this, SLOT(retrieveModelList(const bool &)));

    // Create a network access manager so that we can then retrieve a list of
    // Physiome models from the Physiome Model Repository

    mNetworkAccessManager = new QNetworkAccessManager(this);

    // Make sure that we get told when the download of our Internet file is
    // complete, as well as if there are SSL errors (which would happen if the
    // website's certificate is invalid, e.g. it has expired)

    connect(mNetworkAccessManager, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(finished(QNetworkReply *)));
    connect(mNetworkAccessManager, SIGNAL(sslErrors(QNetworkReply *, const QList<QSslError> &)),
            this, SLOT(sslErrors(QNetworkReply *, const QList<QSslError> &)));

    // Some connections to know what our Physiome Model Repository widget wants
    // from us

    connect(mPhysiomeModelRepositoryWidget, SIGNAL(cloneModel(const QString &, const QString &)),
            this, SLOT(cloneModel(const QString &, const QString &)));
    connect(mPhysiomeModelRepositoryWidget, SIGNAL(showModelFiles(const QString &, const QString &)),
            this, SLOT(showModelFiles(const QString &, const QString &)));

    connect(mPhysiomeModelRepositoryWidget, SIGNAL(modelFileOpenRequested(const QString &)),
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

    // Retranslate our Physiome Model Repository widget

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

    // Send our request to the Physiome Model Repository

    QNetworkRequest networkRequest;

    networkRequest.setRawHeader("Accept", "application/vnd.physiome.pmr2.json.1");

    switch (pPmrRequest) {
    case BookmarkUrlsForCloning:
    case BookmarkUrlsForShowingFiles:
    case SourceFileForCloning:
    case SourceFileForShowingFiles:
        networkRequest.setUrl(QUrl(pUrl));

        break;
    default:   // ModelList
        networkRequest.setUrl(QUrl("https://models.physiomeproject.org/exposure"));
    }

    QNetworkReply *networkReply = mNetworkAccessManager->get(networkRequest);

    // Keep track of the type of request type and the description

    networkReply->setProperty(PmrRequestProperty, pPmrRequest);
    networkReply->setProperty(ExtraProperty, pExtra);
}

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::cloneWorkspace(const QString &pWorkspace)
{
    // Retrieve the name of an empty directory

    QString dirName = Core::getExistingDirectory(tr("Select Empty Directory"),
                                                 QString(), true);

    if (!dirName.isEmpty()) {
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

            QMessageBox::warning(qApp->activeWindow(),
                                 tr("Clone Workspace"),
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

void PhysiomeModelRepositoryWindowWindow::on_filterValue_textChanged(const QString &text)
{
    // Ask our Physiome Model Repository widget to filter its output

    mPhysiomeModelRepositoryWidget->filter(text);
}

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::on_refreshButton_clicked()
{
    // Get the list of Physiome models from the Physiome Model Repository

    sendPmrRequest(ModelList);
}

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::finished(QNetworkReply *pNetworkReply)
{
    // Check whether we were able to retrieve the list of models

    PmrRequest pmrRequest = PmrRequest(pNetworkReply->property(PmrRequestProperty).toInt());

    PhysiomeModelRepositoryWindowModels models = PhysiomeModelRepositoryWindowModels();
    QString errorMessage = QString();

    QStringList bookmarkUrls = QStringList();

    QString sourceFile = QString();

    if (pNetworkReply->error() == QNetworkReply::NoError) {
        // Parse the JSON code

        QJsonParseError jsonParseError;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(pNetworkReply->readAll(), &jsonParseError);

        if (jsonParseError.error == QJsonParseError::NoError) {
            // Check the request type to determine what we should do with the
            // data

            QVariantMap collectionMap = jsonDocument.object().toVariantMap()["collection"].toMap();

            switch (pmrRequest) {
            case BookmarkUrlsForCloning:
            case BookmarkUrlsForShowingFiles:
                foreach (const QVariant &linkVariant, collectionMap["links"].toList())
                    bookmarkUrls << linkVariant.toMap()["href"].toString().trimmed();

                mNumberOfUntreatedSourceFiles = bookmarkUrls.count();

                break;
            case SourceFileForCloning:
            case SourceFileForShowingFiles: {
                sourceFile = collectionMap["items"].toList().first().toMap()["links"].toList().first().toMap()["href"].toString().trimmed();

                --mNumberOfUntreatedSourceFiles;

                break;
            }
            default:   // ModelList
                // Retrieve the list of models

                foreach (const QVariant &linkVariant, collectionMap["links"].toList()) {
                    QVariantMap linkMap = linkVariant.toMap();

                    models << PhysiomeModelRepositoryWindowModel(linkMap["href"].toString().trimmed(),
                                                                 linkMap["prompt"].toString().trimmed());
                }
            }
        } else {
            errorMessage = jsonParseError.errorString();
        }
    } else {
        errorMessage = pNetworkReply->errorString();
    }

    // Some additional processing depending on the request that was sent to the
    // Physiome Model Repository

    switch (pmrRequest) {
    case BookmarkUrlsForCloning:
    case BookmarkUrlsForShowingFiles:
        // Make sure that we got at least one bookmark URL and retrieve their
        // corresponding source file from the Physiome Model Repository

        if (bookmarkUrls.isEmpty()) {
            QString url = pNetworkReply->url().toString();

            QMessageBox::information(qApp->activeWindow(),
                                     tr("Bookmark URLs"),
                                      tr("No bookmark URL could be found for <a href=\"%1\">%2</a>.").arg(url, pNetworkReply->property(ExtraProperty).toString())
                                     +"<br/><br/>"+tr("<strong>Note:</strong> you might want to check with <a href=\"mailto: Tommy Yu <tommy.yu@auckland.ac.nz>\">Tommy Yu</a> (the person behind the <a href=\"https://models.physiomeproject.org/\">Physiome Model Repository</a>) why this is the case."),
                                     QMessageBox::Ok);
        } else {
            QString url = pNetworkReply->url().toString();

            foreach (const QString &bookmarkUrl, bookmarkUrls)
                sendPmrRequest((pmrRequest == BookmarkUrlsForCloning)?
                                   SourceFileForCloning:
                                   SourceFileForShowingFiles,
                               bookmarkUrl, url);
        }

        break;
    case SourceFileForCloning:
    case SourceFileForShowingFiles: {
        // Keep track of the model's source file

        QString url = pNetworkReply->property(ExtraProperty).toString();

        mSourceFiles.insertMulti(url, sourceFile);

        // Determine the workspace associated with the model, should we have
        // retrieved all of its source files, and ask our Physiome Model
        // Repository widget to add some source files and maybe also show them
        // Note: this can be done with any model's source file, but we do it
        //       with the last one in case of a problem occuring between the
        //       retrieval of the first and last model's source files...

        if (!mNumberOfUntreatedSourceFiles) {
            mWorkspaces.insert(url, sourceFile.remove(QRegularExpression("/rawfile/.*$")));

            mPhysiomeModelRepositoryWidget->addModelFiles(url, mSourceFiles.values(url));

            if (pmrRequest == SourceFileForShowingFiles)
                mPhysiomeModelRepositoryWidget->showModelFiles(url);

            // Remove the model's source files since we don't need them anymore
            // (and there is no point in wasting memory for no reason)

            mSourceFiles.remove(url);
        }

        // Clone the workspace, if needed

        if (    !mNumberOfUntreatedSourceFiles
            && (pmrRequest == SourceFileForCloning)) {
                cloneWorkspace(mWorkspaces.value(url));
        }

        break;
    }
    default:   // ModelList
        // Ask our Physiome Model Repository widget to initialise itself and
        // then filter its list of models, should we have been asked to
        // retrieve a list of models

        mPhysiomeModelRepositoryWidget->initialize(models, errorMessage);

        mPhysiomeModelRepositoryWidget->filter(mGui->filterValue->text());
    }

    // Show ourselves as not busy anymore, but only under certain conditions

    if (    (pmrRequest == ModelList)
        || ((   (pmrRequest == BookmarkUrlsForCloning)
             || (pmrRequest == BookmarkUrlsForShowingFiles)) &&  bookmarkUrls.isEmpty())
        || ((   (pmrRequest == SourceFileForCloning)
             || (pmrRequest == SourceFileForShowingFiles))   && !mNumberOfUntreatedSourceFiles)) {
        busy(false);
    }

    // Delete (later) the network reply

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

void PhysiomeModelRepositoryWindowWindow::retrieveModelList(const bool &pVisible)
{
    // Retrieve the list of models, if we are becoming visible and the list of
    // models has never been requested before

    static bool firstTime = true;

    if (pVisible && firstTime) {
        firstTime = false;

        on_refreshButton_clicked();
    }
}

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::cloneModel(const QString &pUrl,
                                                     const QString &pDescription)
{
    // Check whether we already know about the workspace for the given model

    QString workspace = mWorkspaces.value(pUrl);

    if (!workspace.isEmpty()) {
        busy(true);

        cloneWorkspace(workspace);

        busy(false);
    } else {
        // To retrieve the workspace associated with the given model, we first
        // need to retrieve its bookmark URLs

        sendPmrRequest(BookmarkUrlsForCloning, pUrl, pDescription);
    }
}

//==============================================================================

void PhysiomeModelRepositoryWindowWindow::showModelFiles(const QString &pUrl,
                                                         const QString &pDescription)
{
    // To retrieve the model's files, we first need to retrieve its bookmark
    // URLs

    sendPmrRequest(BookmarkUrlsForShowingFiles, pUrl, pDescription);
}

//==============================================================================

}   // namespace PhysiomeModelRepositoryWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
