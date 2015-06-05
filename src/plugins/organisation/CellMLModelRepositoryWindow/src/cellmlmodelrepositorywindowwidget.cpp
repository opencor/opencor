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
// CellML Model Repository widget
//==============================================================================

#include "cellmlmodelrepositorywindowwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"

//==============================================================================

#include "ui_cellmlmodelrepositorywindowwidget.h"

//==============================================================================

#include <QClipboard>
#include <QDesktopServices>
#include <QIODevice>
#include <QMenu>
#include <QPaintEvent>
#include <QRegularExpression>
#include <QWebElement>
#include <QWebFrame>

//==============================================================================

namespace OpenCOR {
namespace CellMLModelRepositoryWindow {

//==============================================================================

CellmlModelRepositoryWindowModel::CellmlModelRepositoryWindowModel(const QString &pUrl,
                                                                   const QString &pName) :
    mUrl(pUrl),
    mName(pName)
{
}

//==============================================================================

QString CellmlModelRepositoryWindowModel::url() const
{
    // Return our URL

    return mUrl;
}

//==============================================================================

QString CellmlModelRepositoryWindowModel::name() const
{
    // Return our name

    return mName;
}

//==============================================================================

CellmlModelRepositoryWindowWidget::CellmlModelRepositoryWindowWidget(QWidget *pParent) :
    Core::WebViewWidget(pParent),
    Core::CommonWidget(pParent),
    mGui(new Ui::CellmlModelRepositoryWindowWidget),
    mModelNames(QStringList()),
    mModelUrlsIds(QMap<QString, int>()),
    mErrorMessage(QString()),
    mNumberOfFilteredModels(0),
    mUrl(QString())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Add a small margin ourselves, so that no visual trace of the border drawn
    // by drawBorder() in paintEvent() is left when scrolling (on Windows and
    // Linux, but it doesn't harm doing it for all our supported platforms)
    // Note: not sure why, but no matter how many pixels are specified for the
    //       margin, no margin actually exists, but it addresses the issue with
    //       the border drawn by drawBorder()...

    setStyleSheet("QWebView {"
                  "    margin: 1px;"
                  "}");

    // Create and populate our context menu

    mContextMenu = new QMenu(this);

    mContextMenu->addAction(mGui->actionCopy);

    // We want out own context menu

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu()));

    // Prevent objects from being dropped on us

    setAcceptDrops(false);

    // Have links opened in the user's browser rather than in our list

    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    // Some connections

    connect(page(), SIGNAL(linkClicked(const QUrl &)),
            this, SLOT(linkClicked()));

    // Retrieve the output template

    Core::readTextFromFile(":/output.html", mOutputTemplate);

    setHtml(mOutputTemplate.arg(Core::iconDataUri(":/oxygen/places/folder-downloads.png", 16, 16),
                                Core::iconDataUri(":/oxygen/actions/document-open-remote.png", 16, 16)));
}

//==============================================================================

CellmlModelRepositoryWindowWidget::~CellmlModelRepositoryWindowWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlModelRepositoryWindowWidget::retranslateUi()
{
    // Retranslate our message

    QWebElement messageElement = page()->mainFrame()->documentElement().findFirst("p[id=message]");

    if (mErrorMessage.isEmpty()) {
        if (!mNumberOfFilteredModels) {
            if (mModelNames.isEmpty())
                messageElement.removeAllChildren();
            else
                messageElement.setInnerXml(tr("No CellML model matches your criteria."));
        } else if (mNumberOfFilteredModels == 1) {
            messageElement.setInnerXml(tr("<strong>1</strong> CellML model was found:"));
        } else {
            messageElement.setInnerXml(tr("<strong>%1</strong> CellML models were found:").arg(mNumberOfFilteredModels));
        }
    } else {
        messageElement.setInnerXml(tr("<strong>Error:</strong> ")+Core::formatMessage(mErrorMessage, true, true));
    }
}

//==============================================================================

QSize CellmlModelRepositoryWindowWidget::sizeHint() const
{
    // Suggest a default size for the CellML Model Repository widget
    // Note: this is critical if we want a docked widget, with a CellML Model
    //       Repository widget on it, to have a decent size when docked to the
    //       main window...

    return defaultSize(0.15);
}

//==============================================================================

void CellmlModelRepositoryWindowWidget::paintEvent(QPaintEvent *pEvent)
{
    // Default handling of the event

    QWebView::paintEvent(pEvent);

    // Draw a border

    drawBorder(
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
               true, true, true, true,
#elif defined(Q_OS_MAC)
               true, false, false, false,
#else
    #error Unsupported platform
#endif
               true, false, false, false
              );
}

//==============================================================================

void CellmlModelRepositoryWindowWidget::initialize(const CellmlModelRepositoryWindowModels &pModels,
                                                   const QString &pErrorMessage)
{
    // Initialise / keep track of some properties

    mModelNames = QStringList();
    mModelUrlsIds = QMap<QString, int>();

    mErrorMessage = pErrorMessage;

    // Initialise our list of models

    QString models = QString();

    for (int i = 0, iMax = pModels.count(); i < iMax; ++i) {
        CellmlModelRepositoryWindowModel model = pModels[i];

        models =  models
                 +"<tr id=\"model_"+QString::number(i)+"\">\n"
                 +"    <td>\n"
                 +"        <table class=\"fullWidth\">\n"
                 +"            <tbody>\n"
                 +"                <tr>\n"
                 +"                    <td class=\"fullWidth\">\n"
                 +"                        <ul>\n"
                 +"                            <li class=\"model\">\n"
                 +"                                <a href=\""+model.url()+"\">"+model.name()+"</a>\n"
                 +"                            </li>\n"
                 +"                        </ul>\n"
                 +"                    </td>\n"
                 +"                    <td class=\"button\">\n"
                 +"                        <a class=\"noHover\" href=\"clone|"+model.url()+"|"+model.name()+"\"><img class=\"button clone\"/></a>\n"
                 +"                    </td>\n"
                 +"                    <td class=\"button\">\n"
                 +"                        <a class=\"noHover\" href=\"files|"+model.url()+"|"+model.name()+"\"><img id=\"model_"+QString::number(i)+"\" class=\"button open\"/></a>\n"
                 +"                    </td>\n"
                 +"                </tr>\n"
                 +"            </tbody>\n"
                 +"        </table>\n"
                 +"    </td>\n"
                 +"</tr>\n"
                 +"<tr id=\"modelFiles_"+QString::number(i)+"\" style=\"display: none;\">\n"
                 +"    <td>\n"
                 +"        <ul id=\"modelFiles_"+QString::number(i)+"\">\n"
                 +"        </ul>\n"
                 +"    </td>\n"
                 +"</tr>\n"
                 +"<tr>\n"
                 +"    <td class=\"verticalSpace\">\n"
                 +"    </td>\n"
                 +"</tr>\n";

        mModelNames << model.name();
        mModelUrlsIds.insert(model.url(), i);
    }

    QWebElement modelsElement = page()->mainFrame()->documentElement().findFirst("tbody");

    modelsElement.removeAllChildren();
    modelsElement.appendInside(models);
}

//==============================================================================

void CellmlModelRepositoryWindowWidget::filter(const QString &pFilter)
{
    // Make sure that we have something to filter (i.e. no error message)

    if (!mErrorMessage.isEmpty())
        return;

    // Filter our list of models, remove duplicates (they will be reintroduced
    // in the next step) and update our message (by retranslate ourselves)

    QStringList filteredModelNames = mModelNames.filter(QRegularExpression(pFilter, QRegularExpression::CaseInsensitiveOption));

    mNumberOfFilteredModels = filteredModelNames.count();

    filteredModelNames.removeDuplicates();

    retranslateUi();

    // Show/hide the relevant models

    QWebElement documentElement = page()->mainFrame()->documentElement();

    for (int i = 0, iMax = mModelNames.count(); i < iMax; ++i) {
        QString displayValue = filteredModelNames.contains(mModelNames[i])?"table-row":"none";
        QWebElement trElement = documentElement.findFirst(QString("tr[id=model_%1]").arg(i));

        trElement.setStyleProperty("display", displayValue);

        if (trElement.nextSibling().hasClass("visible"))
            trElement.nextSibling().setStyleProperty("display", displayValue);
    }
}

//==============================================================================

void CellmlModelRepositoryWindowWidget::addModelFiles(const QString &pUrl,
                                                      const QStringList &pSourceFiles)
{
    // Add the given files to the model

    QWebElement ulElement = page()->mainFrame()->documentElement().findFirst(QString("ul[id=modelFiles_%1]").arg(mModelUrlsIds.value(pUrl)));

    foreach (const QString &sourceFile, pSourceFiles) {
        ulElement.appendInside(QString("<li class=\"modelFile\">"
                                       "    <a href=\"%1\">%2</a>"
                                       "</li>").arg(sourceFile, QString(sourceFile).remove(QRegularExpression(".*/"))));
    }
}

//==============================================================================

void CellmlModelRepositoryWindowWidget::showModelFiles(const QString &pUrl,
                                                       const bool &pShow)
{
    // Show the files for the given model

    int id = mModelUrlsIds.value(pUrl);
    QWebElement documentElement = page()->mainFrame()->documentElement();
    QWebElement buttonElement = documentElement.findFirst(QString("img[id=model_%1]").arg(id));
    QWebElement trElement = documentElement.findFirst(QString("tr[id=modelFiles_%1]").arg(id));

    if (pShow) {
        buttonElement.removeClass("button");
        buttonElement.addClass("downButton");

        trElement.addClass("visible");
        trElement.setStyleProperty("display", "table-row");
    } else {
        buttonElement.addClass("button");
        buttonElement.removeClass("downButton");

        trElement.removeClass("visible");
        trElement.setStyleProperty("display", "none");
    }
}

//==============================================================================

void CellmlModelRepositoryWindowWidget::on_actionCopy_triggered()
{
    // Copy the URL of the model to the clipboard

    QApplication::clipboard()->setText(mUrl);
}

//==============================================================================

void CellmlModelRepositoryWindowWidget::linkClicked()
{
    // Retrieve some information about the link

    QString link;
    QString textContent;

    QWebElement element = retrieveLinkInformation(link, textContent);

    // Check whether we have clicked a model link or a button link, i.e. that we
    // want to clone the model

    if (textContent.isEmpty()) {
        // We have clicked on a button link, so let people know whether we want
        // to clone a model or whether we want to show/hide its files

        QStringList linkList = link.split("|");

        if (!linkList[0].compare("clone")) {
            emit cloneModel(linkList[1], linkList[2]);
        } else {
            // Show/hide the model's files, if we have them, or let people know
            // that we want them

            int id = mModelUrlsIds.value(linkList[1]);

            QWebElement ulElement = page()->mainFrame()->documentElement().findFirst(QString("ul[id=modelFiles_%1]").arg(id));

            if (ulElement.firstChild().isNull()) {
                emit showModelFiles(linkList[1], linkList[2]);
            } else {
                showModelFiles(linkList[1],
                               page()->mainFrame()->documentElement().findFirst(QString("img[id=model_%1]").arg(id)).hasClass("button"));
            }
        }
    } else {
        // Open the model link in the user's browser or ask for it to be opened
        // in OpenCOR

        if (element.parent().hasClass("modelFile"))
            emit modelFileOpenRequested(link);
        else
            QDesktopServices::openUrl(link);
    }
}

//==============================================================================

void CellmlModelRepositoryWindowWidget::showCustomContextMenu()
{
    // Retrieve some information about the link, if any

    QString textContent;

    retrieveLinkInformation(mUrl, textContent);

    // Show our context menu to allow the copying of the URL of the model, but
    // only if we are over a link, i.e. if both mLink and textContent are not
    // empty

    if (!mUrl.isEmpty() && !textContent.isEmpty())
        mContextMenu->exec(QCursor::pos());
}

//==============================================================================

}   // namespace CellMLModelRepositoryWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
