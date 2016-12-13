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
// PMR window widget
//==============================================================================

#include "coreguiutils.h"
#include "i18ninterface.h"
#include "pmrwindowwidget.h"

//==============================================================================

#include <QApplication>
#include <QClipboard>
#include <QContextMenuEvent>
#include <QDesktopServices>
#include <QIODevice>
#include <QMenu>
#include <QRegularExpression>
#include <QWebElement>
#include <QWebFrame>

//==============================================================================

namespace OpenCOR {
namespace PMRWindow {

//==============================================================================

PmrWindowWidget::PmrWindowWidget(QWidget *pParent) :
    WebViewerWidget::WebViewerWidget(pParent),
    mExposureNames(QStringList()),
    mExposureDisplayed(QBoolList()),
    mExposureUrlId(QMap<QString, int>()),
    mInitialized(false),
    mErrorMessage(QString()),
    mInternetConnectionAvailable(true),
    mNumberOfFilteredExposures(0),
    mExposureUrl(QString())
{
    // Create and populate our context menu

    mContextMenu = new QMenu(this);

    mCopyAction = Core::newAction(QIcon(":/oxygen/actions/edit-copy.png"),
                                  this);

    connect(mCopyAction, SIGNAL(triggered(bool)),
            this, SLOT(copy()));

    mContextMenu->addAction(mCopyAction);

    // Prevent zooming in/out

    setZoomingEnabled(false);

    // Prevent objects from being dropped on us

    setAcceptDrops(false);

    // Some connections to handle the clicking and hovering of a link

    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    connect(page(), SIGNAL(linkClicked(const QUrl &)),
            this, SLOT(linkClicked()));
    connect(page(), SIGNAL(linkHovered(const QString &, const QString &, const QString &)),
            this, SLOT(linkHovered()));

    // Retrieve the HTML template

    QString fileContents;

    Core::readFileContentsFromFile(":/PMRWindow/output.html", fileContents);

    mTemplate = fileContents.arg(Core::iconDataUri(":/oxygen/places/folder-downloads.png", 16, 16),
                                 Core::iconDataUri(":/oxygen/actions/document-open-remote.png", 16, 16),
                                 "%1", "%2");
}

//==============================================================================

void PmrWindowWidget::retranslateUi()
{
    // Retranslate our action

    I18nInterface::retranslateAction(mCopyAction, tr("Copy"),
                                     tr("Copy the URL to the clipboard"));

    // Retranslate our message, if we have been initialised

    if (mInitialized)
        page()->mainFrame()->documentElement().findFirst("p[id=message]").setInnerXml(message());
}

//==============================================================================

void PmrWindowWidget::contextMenuEvent(QContextMenuEvent *pEvent)
{
    // Retrieve some information about the link, if any

    QString textContent;

    retrieveLinkInformation(mExposureUrl, textContent);

    // Show our context menu to allow the copying of the URL of the exposure,
    // but only if we are over a link, i.e. if both mLink and textContent are
    // not empty

    if (!mExposureUrl.isEmpty() && !textContent.isEmpty())
        mContextMenu->exec(pEvent->globalPos());
}

//==============================================================================

QSize PmrWindowWidget::sizeHint() const
{
    // Suggest a default size for our PMR widget
    // Note: this is critical if we want a docked widget, with a PMR widget on
    //       it, to have a decent size when docked to the main window...

    return defaultSize(0.15);
}

 //==============================================================================

QString PmrWindowWidget::message() const
{
    // Determine the message to be displayed, if any

    QString res = QString();

    if (mInternetConnectionAvailable && mErrorMessage.isEmpty()) {
        if (!mNumberOfFilteredExposures) {
            if (!mExposureNames.isEmpty())
                res = tr("No exposure matches your criteria.");
        } else if (mNumberOfFilteredExposures == 1) {
            res = tr("<strong>1</strong> exposure was found:");
        } else {
            res = tr("<strong>%1</strong> exposures were found:").arg(QLocale().toString(mNumberOfFilteredExposures));
        }
    } else {
        res = tr("<strong>Error:</strong> ")+Core::formatMessage(mInternetConnectionAvailable?
                                                                     mErrorMessage:
                                                                     Core::noInternetConnectionAvailableMessage(),
                                                                 true, true);
    }

    return res;
}

//==============================================================================

void PmrWindowWidget::initialize(const PMRSupport::PmrExposures &pExposures,
                                 const QString &pErrorMessage,
                                 const QString &pFilter,
                                 const bool &pInternetConnectionAvailable)
{
    // Initialise / keep track of some properties

    mExposureNames.clear();
    mExposureDisplayed.clear();
    mExposureUrlId.clear();

    mErrorMessage = pErrorMessage;

    mInternetConnectionAvailable = pInternetConnectionAvailable;

    // Initialise our list of exposures

    QString exposures = QString();
    QRegularExpression filterRegEx = QRegularExpression(pFilter, QRegularExpression::CaseInsensitiveOption);

    mNumberOfFilteredExposures = 0;

    for (int i = 0, iMax = pExposures.count(); i < iMax; ++i) {
        QString exposureUrl = pExposures[i]->url();
        QString exposureName = pExposures[i]->name();
        bool exposureDisplayed = exposureName.contains(filterRegEx);

        exposures += "<tr id=\"exposure_"+QString::number(i)+"\" style=\"display: "+(exposureDisplayed?"table-row":"none")+";\">\n"
                     "    <td class=\"exposure\">\n"
                     "        <table class=\"fullWidth\">\n"
                     "            <tbody>\n"
                     "                <tr>\n"
                     "                    <td class=\"fullWidth\">\n"
                     "                        <ul>\n"
                     "                            <li class=\"exposure\">\n"
                     "                                <a href=\""+exposureUrl+"\">"+exposureName+"</a>\n"
                     "                            </li>\n"
                     "                        </ul>\n"
                     "                    </td>\n"
                     "                    <td class=\"button\">\n"
                     "                        <a class=\"noHover\" href=\"cloneWorkspace|"+exposureUrl+"\"><img class=\"button clone\"></a>\n"
                     "                    </td>\n"
                     "                    <td class=\"button\">\n"
                     "                        <a class=\"noHover\" href=\"showExposureFiles|"+exposureUrl+"\"><img id=\"exposureFilesButton_"+QString::number(i)+"\" class=\"button open\"></a>\n"
                     "                    </td>\n"
                     "                </tr>\n"
                     "            </tbody>\n"
                     "        </table>\n"
                     "        <ul id=\"exposureFiles_"+QString::number(i)+"\" style=\"display: none;\">\n"
                     "        </ul>\n"
                     "    </td>\n"
                     "</tr>\n";

        mExposureNames << exposureName;
        mExposureDisplayed << exposureDisplayed;
        mExposureUrlId.insert(exposureUrl, i);

        mNumberOfFilteredExposures += exposureDisplayed;
    }

    setHtml(mTemplate.arg(message(), exposures));

    mInitialized = true;
}

//==============================================================================

void PmrWindowWidget::filter(const QString &pFilter)
{
    // Filter our list of exposures and remove any duplicates (they will be
    // 'reintroduced' in the next step)

    QStringList filteredExposureNames = mExposureNames.filter(QRegularExpression(pFilter, QRegularExpression::CaseInsensitiveOption));

    mNumberOfFilteredExposures = filteredExposureNames.count();

    filteredExposureNames.removeDuplicates();

    // Update our message and show/hide the relevant exposures

    page()->mainFrame()->documentElement().findFirst("p[id=message]").setInnerXml(message());

    QWebElement trElement = page()->mainFrame()->documentElement().findFirst(QString("tbody[id=exposures]")).firstChild();
    QWebElement ulElement;

    for (int i = 0, iMax = mExposureNames.count(); i < iMax; ++i) {
        if (mExposureDisplayed[i] != filteredExposureNames.contains(mExposureNames[i])) {
            QString displayValue = mExposureDisplayed[i]?"none":"table-row";

            trElement.setStyleProperty("display", displayValue);

            ulElement = trElement.firstChild().firstChild().nextSibling();

            if (ulElement.hasClass("visible"))
                ulElement.setStyleProperty("display", displayValue);

            mExposureDisplayed[i] = !mExposureDisplayed[i];
        }

        trElement = trElement.nextSibling();
    }
}

//==============================================================================

void PmrWindowWidget::addExposureFiles(const QString &pUrl,
                                       const QStringList &pExposureFiles)
{
    // Add the given exposure files to the exposure

    static const QRegularExpression FilePathRegEx = QRegularExpression("^.*/");

    QWebElement ulElement = page()->mainFrame()->documentElement().findFirst(QString("ul[id=exposureFiles_%1]").arg(mExposureUrlId.value(pUrl)));

    foreach (const QString &exposureFile, pExposureFiles) {
        ulElement.appendInside(QString("<li class=\"exposureFile\">"
                                       "    <a href=\"%1\">%2</a>"
                                       "</li>").arg(exposureFile, QString(exposureFile).remove(FilePathRegEx)));
    }
}

//==============================================================================

void PmrWindowWidget::showExposureFiles(const QString &pUrl, const bool &pShow)
{
    // Show the exposure files for the given exposure

    int id = mExposureUrlId.value(pUrl);
    QWebElement documentElement = page()->mainFrame()->documentElement();
    QWebElement imgElement = documentElement.findFirst(QString("img[id=exposureFilesButton_%1]").arg(id));
    QWebElement ulElement = documentElement.findFirst(QString("ul[id=exposureFiles_%1]").arg(id));

    if (pShow) {
        imgElement.removeClass("button");
        imgElement.addClass("downButton");

        ulElement.addClass("visible");
        ulElement.setStyleProperty("display", "table-row");
    } else {
        imgElement.addClass("button");
        imgElement.removeClass("downButton");

        ulElement.removeClass("visible");
        ulElement.setStyleProperty("display", "none");
    }
}

//==============================================================================

void PmrWindowWidget::copy()
{
    // Copy the URL of the exposure to the clipboard

    QApplication::clipboard()->setText(mExposureUrl);
}

//==============================================================================

void PmrWindowWidget::linkClicked()
{
    // Retrieve some information about the link

    QString link;
    QString textContent;

    QWebElement element = retrieveLinkInformation(link, textContent);

    // Check whether we have clicked a text or button link

    if (textContent.isEmpty()) {
        // We have clicked on a button link, so let people know whether we want
        // to clone a workspace or whether we want to show/hide exposure files

        QStringList linkList = link.split("|");

        if (!linkList[0].compare("cloneWorkspace")) {
            emit cloneWorkspaceRequested(linkList[1]);
        } else {
            // Show/hide exposure files, if we have them, or let people know
            // that we want to show them

            int id = mExposureUrlId.value(linkList[1]);

            QWebElement documentElement = page()->mainFrame()->documentElement();
            QWebElement ulElement = documentElement.findFirst(QString("ul[id=exposureFiles_%1]").arg(id));

            if (ulElement.firstChild().isNull()) {
                emit showExposureFilesRequested(linkList[1]);
            } else {
                showExposureFiles(linkList[1],
                                  documentElement.findFirst(QString("img[id=exposureFilesButton_%1]").arg(id)).hasClass("button"));
            }
        }
    } else {
        // Open an exposure link in the user's browser or ask for an exposure
        // file to be opened in OpenCOR

        if (element.parent().hasClass("exposureFile"))
            emit openExposureFileRequested(link);
        else
            QDesktopServices::openUrl(link);
    }
}

//==============================================================================

void PmrWindowWidget::linkHovered()
{
    // Retrieve some information about the link

    QString link;
    QString textContent;

    QWebElement element = retrieveLinkInformation(link, textContent);

    // Update our tool tip based on whether we are hovering a text or button
    // link
    // Note: this follows the approach used in linkClicked()...

    QString linkToolTip = QString();

    if (textContent.isEmpty()) {
        QStringList linkList = link.split("|");

        if (!linkList[0].compare("cloneWorkspace")) {
            linkToolTip = tr("Clone Workspace");
        } else if (linkList.count() == 2) {
            if (page()->mainFrame()->documentElement().findFirst(QString("img[id=exposureFilesButton_%1]").arg(mExposureUrlId.value(linkList[1]))).hasClass("button"))
                linkToolTip = tr("Show Exposure Files");
            else
                linkToolTip = tr("Hide Exposure Files");
        }
    } else {
        if (element.parent().hasClass("exposureFile"))
            linkToolTip = tr("Open Exposure File");
        else
            linkToolTip = tr("Browse Exposure");
    }

    setLinkToolTip(linkToolTip);
}

//==============================================================================

}   // namespace PMRWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
