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

CellmlModelRepositoryWindowWidget::CellmlModelRepositoryWindowWidget(QWidget *pParent) :
    Core::WebViewWidget(pParent),
    Core::CommonWidget(pParent),
    mGui(new Ui::CellmlModelRepositoryWindowWidget),
    mModelNames(QStringList()),
    mErrorMessage(QString()),
    mNumberOfModels(0),
    mNumberOfFilteredModels(0),
    mLink(QString())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create and populate our context menu

    mContextMenu = new QMenu(this);

    mContextMenu->addAction(mGui->actionCopy);

    // We want out own context menu

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu(const QPoint &)));

    // Prevent objects from being dropped on us

    setAcceptDrops(false);

    // Have links opened in the user's browser rather than in our list

    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    // Some connections

    connect(this, SIGNAL(linkClicked(const QUrl &)),
            this, SLOT(openLink(const QUrl &)));

    // Retrieve the output template

    Core::readTextFromFile(":/output.html", mOutputTemplate);

    setHtml(mOutputTemplate.arg(Core::iconDataUri(":/oxygen/places/folder-downloads.png", 16, 16),
                                Core::iconDataUri(":/oxygen/places/folder-downloads.png", 16, 16, QIcon::Disabled)));
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
            if (!mNumberOfModels)
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

void CellmlModelRepositoryWindowWidget::initialize(const QStringList &pModelNames,
                                                   const QStringList &pModelUrls,
                                                   const QString &pErrorMessage)
{
    // Keep track of some properties

    mModelNames = pModelNames;
    mErrorMessage = pErrorMessage;

    // Initialise our list of models, unless an error occurred

    if (pErrorMessage.isEmpty()) {
        mNumberOfModels = pModelNames.count();

        if (mNumberOfModels) {
            QString models = QString();

            for (int i = 0; i < mNumberOfModels; ++i) {
                models =  models
                         +"<tr id=\"model_"+QString::number(i)+"\">\n"
                         +"    <td>\n"
                         +"        <ul>\n"
                         +"            <li>\n"
                         +"                <a href=\""+pModelUrls[i]+"\">"+pModelNames[i]+"</a>\n"
                         +"            </li>\n"
                         +"        </ul>\n"
                         +"    </td>\n"
                         +"    <td class=\"button\">\n"
                         +"        <a class=\"noHover\" href=\"\"><img class=\"button clone\"/></a>\n"
                         +"    </td>\n"
                         +"</tr>\n";
            }

            QWebElement modelsElement = page()->mainFrame()->documentElement().findFirst("tbody");

            modelsElement.removeAllChildren();
            modelsElement.appendInside(models);
        }
    }
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

    // Determine which models should be shown/hidden

    QIntList modelIndexes = QIntList();
    int modelIndex;

    foreach (const QString &filteredModelName, filteredModelNames) {
        modelIndex = -1;

        forever {
            modelIndex = mModelNames.indexOf(filteredModelName, ++modelIndex);

            if (modelIndex == -1)
                break;
            else
                modelIndexes << modelIndex;
        }
    }

    // Show/hide the relevant models

    QWebElement documentElement = page()->mainFrame()->documentElement();

    for (int i = 0, iMax = mModelNames.count(); i < iMax; ++i)
        documentElement.findFirst(QString("tr[id=model_%1]").arg(i)).setStyleProperty("display", modelIndexes.contains(i)?"table-row":"none");
}

//==============================================================================

void CellmlModelRepositoryWindowWidget::on_actionCopy_triggered()
{
    // Copy the URL of the model to the clipboard

    QApplication::clipboard()->setText(mLink);
}

//==============================================================================

void CellmlModelRepositoryWindowWidget::openLink(const QUrl &pUrl)
{
    // Open the link in the user's browser

    QDesktopServices::openUrl(pUrl);
}

//==============================================================================

void CellmlModelRepositoryWindowWidget::showCustomContextMenu(const QPoint &pPosition)
{
    Q_UNUSED(pPosition);

    // Retrieve some information about the link, if any

    QString textContent;

    retrieveLinkInformation(mLink, textContent);

    // Show our context menu to allow the copying of the URL of the model, but
    // only if we are over a link, i.e. if both mLink and textContent are not
    // empty

    if (!mLink.isEmpty() && !textContent.isEmpty())
        mContextMenu->exec(QCursor::pos());
}

//==============================================================================

}   // namespace CellMLModelRepositoryWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
