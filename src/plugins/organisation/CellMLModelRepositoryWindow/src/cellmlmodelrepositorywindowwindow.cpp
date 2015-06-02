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

#include <QClipboard>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMenu>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPoint>
#include <QRegularExpression>

//==============================================================================

#include "git2.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLModelRepositoryWindow {

//==============================================================================

CellmlModelRepositoryWindowWindow::CellmlModelRepositoryWindowWindow(QWidget *pParent) :
    Core::OrganisationWidget(pParent),
    mGui(new Ui::CellmlModelRepositoryWindowWindow),
    mModelListRequested(false)
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

    // Create and populate our context menu

    mContextMenu = new QMenu(this);

    mContextMenu->addAction(mGui->actionCopy);

    // We want our own context menu for our widget (indeed, we don't want the
    // default one, which has the reload menu item)

    mCellmlModelRepositoryWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(mCellmlModelRepositoryWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu(const QPoint &)));

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

    // Connection to update the enabled state of our copy action

    connect(mCellmlModelRepositoryWidget, SIGNAL(copyTextEnabled(const bool &)),
            mGui->actionCopy, SLOT(setEnabled(bool)));
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

    // Retranslate our list of models

    outputModelList(mModelList);
}

//==============================================================================

void CellmlModelRepositoryWindowWindow::outputModelList(const QStringList &pModelList)
{
    // Keep track of our model list and remove any duplicates that it may
    // contain
    // Note: the duplicates are dealt with below when compiling determing the
    //       model names and URLs...

    mModelList = pModelList;

    mModelList.removeDuplicates();

    // Check whether some models should be listed (which may include 2+ models
    // with the same name) or whether an error occurred

    QString contents = QString();

    if (mModelList.count()) {
        int numberOfModels = 0;
        QString modelsListing = QString();
        int modelIndex;

        foreach (const QString &model, mModelList) {
            modelIndex = -1;

            forever {
                modelIndex = mModelNames.indexOf(model, ++modelIndex);

                if (modelIndex == -1) {
                    break;
                } else {
                    ++numberOfModels;

                    modelsListing += "    <li><a href=\""+mModelUrls[modelIndex]+"\">"+model+"</a></li>\n";
                }
            }
        }

        contents += "<p>\n";

        if (numberOfModels == 1)
            contents += "    "+tr("<strong>1</strong> CellML model was found:")+"\n";
        else
            contents += "    "+tr("<strong>%1</strong> CellML models were found:").arg(numberOfModels)+"\n";

        contents += "</p>\n";
        contents += "\n";
        contents += "<ul>\n";
        contents += modelsListing;
        contents += "</ul>";
    } else if (mModelNames.empty()) {
        if (mErrorMessage.count()) {
            contents += "<p>\n";
            contents += "    "+tr("<strong>Error:</strong> ")+Core::formatMessage(mErrorMessage, true, true);
            contents += "</p>\n";
        }
    } else {
        contents += "<p>\n";
        contents += "    "+tr("No CellML model matches your criteria");
        contents += "</p>\n";
    }

    // Show/hide our busy widget and output the list matching the search
    // criteria, or a message telling the user what went wrong, if anything and
    // if needed

    if (mModelListRequested) {
        showBusyWidget(mCellmlModelRepositoryWidget);
    } else {
        hideBusyWidget();

        mCellmlModelRepositoryWidget->output(contents);
    }
}

//==============================================================================

void CellmlModelRepositoryWindowWindow::on_filterValue_textChanged(const QString &text)
{
    // Generate a Web page that contains all the models that match our search
    // criteria

    outputModelList(mModelNames.filter(QRegularExpression(text, QRegularExpression::CaseInsensitiveOption)));
}

//==============================================================================

void CellmlModelRepositoryWindowWindow::on_actionCopy_triggered()
{
    // Copy the current selection to the clipboard

    QApplication::clipboard()->setText(mCellmlModelRepositoryWidget->selectedText());
}

//==============================================================================

void CellmlModelRepositoryWindowWindow::on_refreshButton_clicked()
{
    // Output the message telling the user that the list is being downloaded
    // Note: to clear mModelNames ensures that we get the correct message from
    //       outputModelList...

    mModelListRequested = true;

    mModelNames.clear();

    outputModelList(QStringList());

    // Disable the GUI side, so that the user doesn't get confused and ask to
    // refresh over and over again while he should just be patient

    mGui->dockWidgetContents->setEnabled(false);

    // Get the list of CellML models

    QNetworkRequest networkRequest;

    networkRequest.setRawHeader("Accept", "application/vnd.physiome.pmr2.json.1");
    networkRequest.setUrl(QUrl("https://models.physiomeproject.org/exposure"));

    mNetworkAccessManager->get(networkRequest);
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
    // Clear some properties

    mModelNames.clear();
    mModelUrls.clear();

    mModelListRequested = false;

    // Output the list of models, should we have retrieved it without any
    // problem

    if (pNetworkReply->error() == QNetworkReply::NoError) {
        // Parse the JSON code

        QJsonParseError jsonParseError;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(pNetworkReply->readAll(), &jsonParseError);

        if (jsonParseError.error == QJsonParseError::NoError) {
            // Retrieve the list of CellML models

            QVariantMap resultMap = jsonDocument.object().toVariantMap();
            QVariantMap exposureDetailsVariant;

            foreach (const QVariant &exposureVariant, resultMap["collection"].toMap()["links"].toList()) {
                exposureDetailsVariant = exposureVariant.toMap();

                mModelNames << exposureDetailsVariant["prompt"].toString().trimmed();
                mModelUrls << exposureDetailsVariant["href"].toString().trimmed();
            }

            mErrorMessage = QString();
        } else {
            mErrorMessage = jsonParseError.errorString();
        }
    } else {
        mErrorMessage = pNetworkReply->errorString();
    }

    // Initialise the output using whatever search criteria is present

    on_filterValue_textChanged(mGui->filterValue->text());

    // Re-enable the GUI side

    mGui->dockWidgetContents->setEnabled(true);

    // Give, within the current window, the focus to mGui->filterValue, but
    // only if the current window already has the focus

    Core::setFocusTo(mGui->filterValue);

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

void CellmlModelRepositoryWindowWindow::showCustomContextMenu(const QPoint &pPosition) const
{
    Q_UNUSED(pPosition);

    // Show our context menu for our CellML Models Repository widget

    mContextMenu->exec(QCursor::pos());
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

}   // namespace CellMLModelRepositoryWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
