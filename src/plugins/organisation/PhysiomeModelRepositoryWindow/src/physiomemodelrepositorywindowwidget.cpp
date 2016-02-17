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
// Physiome Model Repository widget
//==============================================================================

#include "coreguiutils.h"
#include "physiomemodelrepositorywindowwidget.h"

//==============================================================================

#include "ui_physiomemodelrepositorywindowwidget.h"

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
namespace PhysiomeModelRepositoryWindow {

//==============================================================================

PhysiomeModelRepositoryWindowExposure::PhysiomeModelRepositoryWindowExposure(const QString &pUrl,
                                                                             const QString &pName) :
    mUrl(pUrl),
    mName(pName)
{
}

//==============================================================================

bool PhysiomeModelRepositoryWindowExposure::operator<(const PhysiomeModelRepositoryWindowExposure &pExposure) const
{
    // Return whether the current exposure is lower than the given one (without
    // worrying about casing)

    return mName.compare(pExposure.name(), Qt::CaseInsensitive) < 0;
}

//==============================================================================

QString PhysiomeModelRepositoryWindowExposure::url() const
{
    // Return our URL

    return mUrl;
}

//==============================================================================

QString PhysiomeModelRepositoryWindowExposure::name() const
{
    // Return our name

    return mName;
}

//==============================================================================

PhysiomeModelRepositoryWindowWidget::PhysiomeModelRepositoryWindowWidget(QWidget *pParent) :
    Core::WebViewWidget(pParent),
    Core::CommonWidget(pParent),
    mGui(new Ui::PhysiomeModelRepositoryWindowWidget),
    mExposureNames(QStringList()),
    mExposureDisplayed(QBoolList()),
    mExposureUrlId(QMap<QString, int>()),
    mErrorMessage(QString()),
    mInternetConnectionAvailable(true),
    mNumberOfFilteredExposures(0),
    mExposureUrl(QString())
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

    // We want our own context menu

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu()));

    // Prevent objects from being dropped on us

    setAcceptDrops(false);

    // Some connections to handle the clicking and hovering of a link

    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    connect(page(), SIGNAL(linkClicked(const QUrl &)),
            this, SLOT(linkClicked()));
    connect(page(), SIGNAL(linkHovered(const QString &, const QString &, const QString &)),
            this, SLOT(linkHovered()));

    // Retrieve the output template

    QByteArray fileContents;

    Core::readFileContentsFromFile(":/output.html", fileContents);

    setHtml(QString(fileContents).arg(Core::iconDataUri(":/oxygen/places/folder-downloads.png", 16, 16),
                                      Core::iconDataUri(":/oxygen/actions/document-open-remote.png", 16, 16)));
}

//==============================================================================

PhysiomeModelRepositoryWindowWidget::~PhysiomeModelRepositoryWindowWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void PhysiomeModelRepositoryWindowWidget::retranslateUi()
{
    // Retranslate our message

    QWebElement messageElement = page()->mainFrame()->documentElement().findFirst("p[id=message]");

    if (mInternetConnectionAvailable && mErrorMessage.isEmpty()) {
        if (!mNumberOfFilteredExposures) {
            if (mExposureNames.isEmpty())
                messageElement.removeAllChildren();
            else
                messageElement.setInnerXml(tr("No exposure matches your criteria."));
        } else if (mNumberOfFilteredExposures == 1) {
            messageElement.setInnerXml(tr("<strong>1</strong> exposure was found:"));
        } else {
            messageElement.setInnerXml(tr("<strong>%1</strong> exposures were found:").arg(mNumberOfFilteredExposures));
        }
    } else {
        messageElement.setInnerXml(tr("<strong>Error:</strong> ")+Core::formatMessage(mInternetConnectionAvailable?
                                                                                          mErrorMessage:
                                                                                          Core::noInternetConnectionAvailableMessage(),
                                                                                      true, true));
    }
}

//==============================================================================

QSize PhysiomeModelRepositoryWindowWidget::sizeHint() const
{
    // Suggest a default size for our PMR widget
    // Note: this is critical if we want a docked widget, with a PMR widget on
    //       it, to have a decent size when docked to the main window...

    return defaultSize(0.15);
}

//==============================================================================

void PhysiomeModelRepositoryWindowWidget::paintEvent(QPaintEvent *pEvent)
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

void PhysiomeModelRepositoryWindowWidget::initialize(const PhysiomeModelRepositoryWindowExposures &pExposures,
                                                     const QString &pErrorMessage,
                                                     const bool &pInternetConnectionAvailable)
{
    // Initialise / keep track of some properties

    mExposureNames = QStringList();
    mExposureDisplayed = QBoolList();
    mExposureUrlId = QMap<QString, int>();

    mErrorMessage = pErrorMessage;

    mInternetConnectionAvailable = pInternetConnectionAvailable;

    // Initialise our list of exposures

    QWebElement tbodyElement = page()->mainFrame()->documentElement().findFirst("tbody[id=exposures]");

    tbodyElement.removeAllChildren();

    for (int i = 0, iMax = pExposures.count(); i < iMax; ++i) {
        QString exposureUrl = pExposures[i].url();
        QString exposureName = pExposures[i].name();

        tbodyElement.appendInside("<tr id=\"exposure_"+QString::number(i)+"\">\n"
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
                                  "                        <a class=\"noHover\" href=\"cloneWorkspace|"+exposureUrl+"|"+exposureName+"\"><img class=\"button clone\"/></a>\n"
                                  "                    </td>\n"
                                  "                    <td class=\"button\">\n"
                                  "                        <a class=\"noHover\" href=\"showExposureFiles|"+exposureUrl+"|"+exposureName+"\"><img id=\"exposure_"+QString::number(i)+"\" class=\"button open\"/></a>\n"
                                  "                    </td>\n"
                                  "                </tr>\n"
                                  "            </tbody>\n"
                                  "        </table>\n"
                                  "        <ul id=\"exposureFiles_"+QString::number(i)+"\" style=\"display: none;\">\n"
                                  "        </ul>\n"
                                  "    </td>\n"
                                  "</tr>\n");

        mExposureNames << exposureName;
        mExposureDisplayed << true;
        mExposureUrlId.insert(exposureUrl, i);
    }
}

//==============================================================================

void PhysiomeModelRepositoryWindowWidget::filter(const QString &pFilter)
{
    // Filter our list of exposures, remove any duplicates (they will be
    // reintroduced in the next step) and update our message (by retranslating
    // ourselves)

    QStringList filteredExposureNames = mExposureNames.filter(QRegularExpression(pFilter, QRegularExpression::CaseInsensitiveOption));

    mNumberOfFilteredExposures = filteredExposureNames.count();

    filteredExposureNames.removeDuplicates();

    retranslateUi();

    // Show/hide the relevant exposures
    // Note: to call QWebElement::setStyleProperty() many times is time
    //       consuming, hence we rely on mExposureDisplayed to determine when we
    //       should change the display property of our elements...

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

void PhysiomeModelRepositoryWindowWidget::addExposureFiles(const QString &pUrl,
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

void PhysiomeModelRepositoryWindowWidget::showExposureFiles(const QString &pUrl,
                                                            const bool &pShow)
{
    // Show the exposure files for the given exposure

    int id = mExposureUrlId.value(pUrl);
    QWebElement documentElement = page()->mainFrame()->documentElement();
    QWebElement imgElement = documentElement.findFirst(QString("img[id=exposure_%1]").arg(id));
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

void PhysiomeModelRepositoryWindowWidget::on_actionCopy_triggered()
{
    // Copy the URL of the exposure to the clipboard

    QApplication::clipboard()->setText(mExposureUrl);
}

//==============================================================================

void PhysiomeModelRepositoryWindowWidget::linkClicked()
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
            emit cloneWorkspace(linkList[1], linkList[2]);
        } else {
            // Show/hide exposure files, if we have them, or let people know
            // that we want to show them

            int id = mExposureUrlId.value(linkList[1]);

            QWebElement documentElement = page()->mainFrame()->documentElement();
            QWebElement ulElement = documentElement.findFirst(QString("ul[id=exposureFiles_%1]").arg(id));

            if (ulElement.firstChild().isNull()) {
                emit showExposureFiles(linkList[1], linkList[2]);
            } else {
                showExposureFiles(linkList[1],
                                  documentElement.findFirst(QString("img[id=exposure_%1]").arg(id)).hasClass("button"));
            }
        }
    } else {
        // Open an exposure link in the user's browser or ask for an exposure
        // file to be opened in OpenCOR

        if (element.parent().hasClass("exposureFile"))
            emit exposureFileOpenRequested(link);
        else
            QDesktopServices::openUrl(link);
    }
}

//==============================================================================

void PhysiomeModelRepositoryWindowWidget::linkHovered()
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
        } else if (linkList.count() == 3) {
            if (page()->mainFrame()->documentElement().findFirst(QString("ul[id=exposureFiles_%1]").arg(mExposureUrlId.value(linkList[1]))).firstChild().isNull())
                linkToolTip = tr("Show Exposure Files");
            else
                linkToolTip = tr("Hide Exposure Files");
        }
    } else {
        if (element.parent().hasClass("exposureFile"))
            linkToolTip = tr("Open Exposure File");
        else
            linkToolTip = tr("Browser Exposure");
    }

    setLinkToolTip(linkToolTip);
}

//==============================================================================

void PhysiomeModelRepositoryWindowWidget::showCustomContextMenu()
{
    // Retrieve some information about the link, if any

    QString textContent;

    retrieveLinkInformation(mExposureUrl, textContent);

    // Show our context menu to allow the copying of the URL of the exposure,
    // but only if we are over a link, i.e. if both mLink and textContent are
    // not empty

    if (!mExposureUrl.isEmpty() && !textContent.isEmpty())
        mContextMenu->exec(QCursor::pos());
}

//==============================================================================

}   // namespace PhysiomeModelRepositoryWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
