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
// CellML Model Repository window
//==============================================================================

#include "cellmlmodelrepositorywindowwindow.h"
#include "cellmlmodelrepositorywindowwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"

//==============================================================================

#include "ui_cellmlmodelrepositorywindowwindow.h"

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
namespace CellMLModelRepositoryWindow {

//==============================================================================

CellmlModelRepositoryWindowWindow::CellmlModelRepositoryWindowWindow(QWidget *pParent) :
    Core::OrganisationWidget(pParent),
    mGui(new Ui::CellmlModelRepositoryWindowWindow),
    mNumberOfUntreatedSourceFiles(0),
    mWorkspaces(QMap<QString, QString>())
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

    // Create and add the CellML Model Repository widget

    mCellmlModelRepositoryWidget = new CellmlModelRepositoryWindowWidget(this);

    mGui->dockWidgetContents->layout()->addWidget(mCellmlModelRepositoryWidget);

    // Keep track of the window's visibility, so that we can request the list of
    // models, if necessary

    connect(this, SIGNAL(visibilityChanged(bool)),
            this, SLOT(retrieveModelList(const bool &)));

    // Create a network access manager so that we can then retrieve a list of
    // CellML models from the CellML Model Repository

    mNetworkAccessManager = new QNetworkAccessManager(this);

    // Make sure that we get told when the download of our Internet file is
    // complete, as well as if there are SSL errors (which would happen if the
    // website's certificate is invalid, e.g. it has expired)

    connect(mNetworkAccessManager, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(finished(QNetworkReply *)));
    connect(mNetworkAccessManager, SIGNAL(sslErrors(QNetworkReply *, const QList<QSslError> &)),
            this, SLOT(sslErrors(QNetworkReply *, const QList<QSslError> &)));

    // Some connections to know what our CellML Model Repository widget wants

    connect(mCellmlModelRepositoryWidget, SIGNAL(cloneModel(const QString &, const QString &)),
            this, SLOT(cloneModel(const QString &, const QString &)));
}

//==============================================================================

CellmlModelRepositoryWindowWindow::~CellmlModelRepositoryWindowWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlModelRepositoryWindowWindow::retranslateUi()
{
    // Retranslate the whole window

    mGui->retranslateUi(this);

    // Retranslate our CellML Model Repository widget

    mCellmlModelRepositoryWidget->retranslateUi();
}

//==============================================================================

static const char *PmrRequestProperty = "PmrRequest";
static const char *ExtraProperty      = "Extra";

//==============================================================================

void CellmlModelRepositoryWindowWindow::sendPmrRequest(const PmrRequest &pPmrRequest,
                                                       const QString &pUrl,
                                                       const QString &pExtra)
{
    // Let the user that we are downloading the list of models

    showBusyWidget(mCellmlModelRepositoryWidget);

    mGui->dockWidgetContents->setEnabled(false);

    // Send our request to the Physiome Model Repository

    QNetworkRequest networkRequest;

    networkRequest.setRawHeader("Accept", "application/vnd.physiome.pmr2.json.1");

    switch (pPmrRequest) {
    case BookmarkUrls:
    case SourceFile:
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

void CellmlModelRepositoryWindowWindow::cloneWorkspace(const QString &pWorkspace)
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

void CellmlModelRepositoryWindowWindow::on_filterValue_textChanged(const QString &text)
{
    // Ask our CellML Model Repository widget to filter its output

    mCellmlModelRepositoryWidget->filter(text);
}

//==============================================================================

void CellmlModelRepositoryWindowWindow::on_refreshButton_clicked()
{
    // Get the list of CellML models from the Physiome Model Repository

    sendPmrRequest(ModelList);
}

//==============================================================================

void CellmlModelRepositoryWindowWindow::finished(QNetworkReply *pNetworkReply)
{
    // Check whether we were able to retrieve the list of models

    PmrRequest pmrRequest = PmrRequest(pNetworkReply->property(PmrRequestProperty).toInt());

    CellmlModelRepositoryWindowModels models = CellmlModelRepositoryWindowModels();
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
            case BookmarkUrls:
                foreach (const QVariant &linkVariant, collectionMap["links"].toList())
                    bookmarkUrls << linkVariant.toMap()["href"].toString().trimmed();

                mNumberOfUntreatedSourceFiles = bookmarkUrls.count();

                break;
            case SourceFile: {
                sourceFile = collectionMap["items"].toList().first().toMap()["links"].toList().first().toMap()["href"].toString().trimmed();

                --mNumberOfUntreatedSourceFiles;

                break;
            }
            default:   // ModelList
                // Retrieve the list of models

                foreach (const QVariant &linkVariant, collectionMap["links"].toList()) {
                    QVariantMap linkMap = linkVariant.toMap();

                    models << CellmlModelRepositoryWindowModel(linkMap["href"].toString().trimmed(),
                                                               linkMap["prompt"].toString().trimmed());
                }
            }
        } else {
            errorMessage = jsonParseError.errorString();
        }
    } else {
        errorMessage = pNetworkReply->errorString();
    }

    // Ask our CellML Model Repository widget to initilise itself and then
    // filter its list of models, should we have been asked to retrieve a list
    // of models

    if (pmrRequest == ModelList) {
        mCellmlModelRepositoryWidget->initialize(models, errorMessage);

        mCellmlModelRepositoryWidget->filter(mGui->filterValue->text());
    }

    // Some additional processing depending on the request that was sent to the
    // Physiome Model Repository

    if (pmrRequest == BookmarkUrls) {
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
                sendPmrRequest(SourceFile, bookmarkUrl, url);
        }
    } else if (pmrRequest == SourceFile) {
        // Determine the workspace associated with the model, should we have
        // retrieved all of its source files, and clone it
        // Note: this can be done with any source file (for a given model), but
        //       we do it with the last one in case of a problem occuring
        //       between the retrieval of the first and last source files...

        if (!mNumberOfUntreatedSourceFiles) {
            QString workspace = sourceFile.remove(QRegularExpression("/rawfile/.*$"));

            mWorkspaces.insert(pNetworkReply->property(ExtraProperty).toString(), workspace);

            cloneWorkspace(workspace);
        }
    }

    // Re-enable the GUI side and give, within the current window, the focus to
    // mGui->filterValue (only if the current window already has the focus), but
    // only under certain conditions

    if (    (pmrRequest == ModelList)
        || ((pmrRequest == BookmarkUrls) &&  bookmarkUrls.isEmpty())
        || ((pmrRequest == SourceFile)   && !mNumberOfUntreatedSourceFiles)) {
        hideBusyWidget();

        mGui->dockWidgetContents->setEnabled(true);

        Core::setFocusTo(mGui->filterValue);
    }

    // Delete (later) the network reply

    pNetworkReply->deleteLater();
}

//==============================================================================

void CellmlModelRepositoryWindowWindow::sslErrors(QNetworkReply *pNetworkReply,
                                                  const QList<QSslError> &pSslErrors)
{
    // Ignore the SSL errors since we trust the website and therefore its
    // certificate (even if it is invalid, e.g. it has expired)

    pNetworkReply->ignoreSslErrors(pSslErrors);
}

//==============================================================================

void CellmlModelRepositoryWindowWindow::retrieveModelList(const bool &pVisible)
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

void CellmlModelRepositoryWindowWindow::cloneModel(const QString &pUrl,
                                                   const QString &pDescription)
{
    // Check whether we already know about the workspace for the given model

    QString workspace = mWorkspaces.value(pUrl);

    if (!workspace.isEmpty()) {
        cloneWorkspace(workspace);
    } else {
        // To retrieve the workspace associated with the given model, we need to
        // retrieve its bookmark URLs

        sendPmrRequest(BookmarkUrls, pUrl, pDescription);
//---GRY--- THE BELOW IS TO BE REMOVED ONCE WE HAVE IMPLEMENTED ISSUE #592...
Q_UNUSED(pUrl);
Q_UNUSED(pDescription);
//        sendPmrRequest(BookmarkUrls, "https://models.physiomeproject.org/e/71");                                        // CellML and SED-ML files
//        sendPmrRequest(BookmarkUrls, "https://models.physiomeproject.org/e/e1");                                        // HTML file
//        sendPmrRequest(BookmarkUrls, "https://models.physiomeproject.org/e/1e");                                        // CellML 1.0 and a CellML 1.1 files
//        sendPmrRequest(BookmarkUrls, "https://models.physiomeproject.org/exposure/42", "Broken");                       // Broken
//        sendPmrRequest(BookmarkUrls, "https://models.physiomeproject.org/exposure/4f9511d703b55ace4780879b253777d6");   // Correct version of the above?
    }
}

//==============================================================================

}   // namespace CellMLModelRepositoryWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
