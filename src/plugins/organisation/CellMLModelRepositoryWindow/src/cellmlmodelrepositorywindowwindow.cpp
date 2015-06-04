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

#include <QDir>
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
    mNumberOfUntreatedSourceFiles(0)
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

static const char *PmrRequestProperty  = "PmrRequest";
static const char *DescriptionProperty = "Description";

//==============================================================================

void CellmlModelRepositoryWindowWindow::sendPmrRequest(const PmrRequest &pPmrRequest,
                                                       const QString &pUrl,
                                                       const QString &pDescription)
{
    // Let the user that we are downloading the list of models

    showBusyWidget(mCellmlModelRepositoryWidget);

    // Disable the GUI side, so that the user doesn't get confused and ask to
    // refresh over and over again while he should just be patient

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
    networkReply->setProperty(DescriptionProperty, pDescription);
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

void CellmlModelRepositoryWindowWindow::on_gitButton_clicked()
{
    // Simple git clone test...

    QString gitCloneTestDir = QDir::homePath()+QDir::separator()+"Desktop/GitCloneTest";

    QDir(gitCloneTestDir).removeRecursively();

    git_libgit2_init();

    git_repository *gitRepository = 0;

    int res = git_clone(&gitRepository,
                        "https://models.physiomeproject.org/workspace/noble_1962",
                        qPrintable(gitCloneTestDir), 0);

    if (res) {
        const git_error *gitError = giterr_last();

        if (gitError)
            qDebug("Error %d: %s.", gitError->klass, gitError->message);
        else
            qDebug("Error %d: no detailed information.", res);
    } else {
        if (gitRepository)
            git_repository_free(gitRepository);
    }

    git_libgit2_shutdown();
}

//==============================================================================

void CellmlModelRepositoryWindowWindow::finished(QNetworkReply *pNetworkReply)
{
    // Check whether we were able to retrieve the list of models

    PmrRequest pmrRequest = PmrRequest(pNetworkReply->property(PmrRequestProperty).toInt());
//qDebug(">>> Request type: %d", pmrRequest);

    CellmlModelRepositoryWindowModels models = CellmlModelRepositoryWindowModels();
    QString errorMessage = QString();

    QStringList bookmarkUrls = QStringList();

    if (pNetworkReply->error() == QNetworkReply::NoError) {
        // Parse the JSON code
QByteArray byteArray = pNetworkReply->readAll();
//qDebug("---[START]---");
//qDebug("%s", qPrintable(QString(byteArray)));
//qDebug("---[END]---");

        QJsonParseError jsonParseError;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(byteArray, &jsonParseError);

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
            case SourceFile:
                foreach (const QVariant &itemVariant, collectionMap["items"].toList().first().toMap()["links"].toList())
qDebug(">>> Source file: %s", qPrintable(itemVariant.toMap()["href"].toString().trimmed()));

                --mNumberOfUntreatedSourceFiles;

                break;
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

    // Re-enable the GUI side and give, within the current window, the focus to
    // mGui->filterValue (only if the current window already has the focus), but
    // only under certain conditions

    if (    (pmrRequest == ModelList)
        || ((pmrRequest == BookmarkUrls) &&  bookmarkUrls.isEmpty())
        || ((pmrRequest == SourceFile)   && !mNumberOfUntreatedSourceFiles)) {
        hideBusyWidget();

        mGui->dockWidgetContents->setEnabled(true);

        Core::setFocusTo(mGui->filterValue);
    } else if ((pmrRequest == BookmarkUrls) && !bookmarkUrls.isEmpty()) {
        // We got some bookmark URLs, so we now need to get their corresponding
        // source file from the Physiome Model Repository
qDebug("---------");

        foreach (const QString &bookmarkUrl, bookmarkUrls)
            sendPmrRequest(SourceFile, bookmarkUrl);
    }

    // Make sure that we got at least one bookmark URL, if we were supposed to
    // get some

    if ((pmrRequest == BookmarkUrls) &&  bookmarkUrls.isEmpty()) {
        QString url = pNetworkReply->url().toString();

        QMessageBox::information(qApp->activeWindow(),
                                 tr("Bookmark URLs"),
                                  tr("No bookmark URL could be found for <a href=\"%1\">%2</a>.").arg(url, pNetworkReply->property(DescriptionProperty).toString())
                                 +"<br/><br/>"+tr("<strong>Note:</strong> you might want to check with <a href=\"mailto: Tommy Yu <tommy.yu@auckland.ac.nz>\">Tommy Yu</a> (the person behind the <a href=\"https://models.physiomeproject.org/\">Physiome Model Repository</a>) why this is the case."),
                                 QMessageBox::Ok);
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
    // To clone a model, we need to know about its workspace, which requires
    // retrieving the model's bookmark URLs
Q_UNUSED(pUrl);
Q_UNUSED(pDescription);

    sendPmrRequest(BookmarkUrls, pUrl, pDescription);
//    sendPmrRequest(BookmarkUrls, "https://models.physiomeproject.org/e/71");
}

//==============================================================================

}   // namespace CellMLModelRepositoryWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
