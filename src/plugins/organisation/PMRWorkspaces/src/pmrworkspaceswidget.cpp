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
// PMR Workspaces widget
//==============================================================================

#include "coreguiutils.h"
#include "pmrrepository.h"
#include "pmrworkspace.h"
#include "pmrworkspaceswidget.h"

//==============================================================================

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHeaderView>
#include <QHelpEvent>
#include <QSettings>
#include <QStandardPaths>
#include <QUrl>
#include <QWebElement>
#include <QWebFrame>

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspaces {

//==============================================================================

PmrWorkspacesWidget::PmrWorkspacesWidget(PMRSupport::PmrRepository *pPmrRepository, QWidget *pParent) :
    WebViewerWidget::WebViewerWidget(pParent),
    Core::CommonWidget(),
    mPmrRepository(pPmrRepository),
    mExpandedItems(QMap<QString, bool>()),
    mSelectedUrl(QString())
{
    // Prevent objects from being dropped on us

    setAcceptDrops(false);

    // Some connections to handle the clicking and hovering of a link

    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    connect(page(), SIGNAL(linkClicked(const QUrl &)),
            this, SLOT(linkClicked()));
    connect(page(), SIGNAL(linkHovered(const QString &, const QString &, const QString &)),
            this, SLOT(linkHovered()));
   // Connections to handle responses from PMR

    // Retrieve the output template
    connect(mPmrRepository, SIGNAL(workspaceCreated(QString)),
            this, SLOT(workspaceCreated(QString)));
    connect(mPmrRepository, SIGNAL(workspaceCloned(PMRSupport::PmrWorkspace *)),
            this, SLOT(workspaceCloned(PMRSupport::PmrWorkspace *)));

    QByteArray fileContents;

    Core::readFileContentsFromFile(":/PMRWorkspaces/output.html", fileContents);

    mTemplate = QString(fileContents).arg( // about, clone, close, open
                                          Core::iconDataUri(":/oxygen/actions/help-about.png", 16, 16),
                                          Core::iconDataUri(":/oxygen/actions/document-open-remote.png", 16, 16),
                                          Core::iconDataUri(":/PMRWorkspaces/icons/workspace-open.png", 16, 16),
                                          Core::iconDataUri(":/PMRWorkspaces/icons/workspace-closed.png", 16, 16),
                                          "%1");
}

//==============================================================================

PmrWorkspacesWidget::~PmrWorkspacesWidget()
{
}

//==============================================================================

QSize PmrWorkspacesWidget::sizeHint() const
{
    // Suggest a default size for our PMR workspaces widget
    // Note: this is critical if we want a docked widget, with a PMR workspaces
    //        widget on it, to have a decent size when docked to the main window.

    return defaultSize(0.15);
}

//==============================================================================

static const auto SettingsWorkspaces    = QStringLiteral("Workspaces");
static const auto SettingsFolders       = QStringLiteral("Folders");
static const auto SettingsExpandedItems = QStringLiteral("ExpandedItems");
static const auto SettingsSelectedItem  = QStringLiteral("SelectedItem");

//==============================================================================

void PmrWorkspacesWidget::loadSettings(QSettings *pSettings)
{
    pSettings->beginGroup(SettingsWorkspaces);

        // Retrieve the names of folders containing cloned workspaces

        QStringList folders = pSettings->value(SettingsFolders).toStringList();
        foreach (QString folder, folders) {
            if (!folder.isEmpty()) addWorkspaceFolder(folder);
        }

        // Retrieve the names of expanded workspaces and folders

        mExpandedItems = pSettings->value(SettingsExpandedItems).toStringList().toSet();

        // Retrieve the currently selected item, if any

        mSelectedItem = pSettings->value(SettingsSelectedItem).toString();

    pSettings->endGroup();
}

//==============================================================================

void PmrWorkspacesWidget::saveSettings(QSettings *pSettings) const
{
    pSettings->remove(SettingsWorkspaces);
    pSettings->beginGroup(SettingsWorkspaces);

        // Keep track of the names of folders containing cloned workspaces

        pSettings->setValue(SettingsFolders, QVariant(mWorkspaceFolders.keys()));

        // Keep track of the names of expanded workspaces and folders

        pSettings->setValue(SettingsExpandedItems, QVariant(mExpandedItems.toList()));

        // Keep track of the currently selected item

        pSettings->setValue(SettingsSelectedItem, mSelectedItem);

    pSettings->endGroup();
}

//==============================================================================
//==============================================================================

void PmrWorkspacesWidget::setSelected(const QString &pSelectedUrl)
{
    mSelectedUrl = pSelectedUrl;
}

//==============================================================================

// This needs to go into pmrworkspaces.cpp ...

QString PmrWorkspacesWidget::workspacePath(const QString &pUrl, const QString &pPath)
{
    Q_UNUSED(pUrl);

    QString workspacePath = PMRSupport::PmrWorkspace::WorkspacesDirectory + QDir::separator() + pPath;
qDebug() << "Checking: " << pUrl << "  Local: " << workspacePath;
    QDir workspaceDirectory = QDir(workspacePath);
    if (!workspacePath.isEmpty() && workspaceDirectory.exists()) {
        return workspaceDirectory.absolutePath();

    }
    return "";
}

//==============================================================================

QString PmrWorkspacesWidget::actionHtml(const QList<QPair<QString, QString> > &pActions)
{
    QPair<QString, QString> action;
    QStringList actions;
    foreach (action, pActions)
        actions << QString("<a href=\"%1|%2\"><img class=\"%1\" /></a>").arg(action.first, action.second);

    return actions.join("");
}

//==============================================================================

QString PmrWorkspacesWidget::containerHtml(const QString &pClass, const QString &pIcon,
                                           const QString &pId, const QString &pName,
                                           const QString &pStatus,
                                           const QList<QPair<QString, QString> > &pActions)
{
    static const QString html = "<tr class=\"%1\" id=\"%2\">\n"
                                "  <td class=\"icon\"><img class=\"%3\" /></td>\n" /* tr.CLASS td.icon img.clone/open/close { } */
                                "  <td class=\"name\">%4</td>\n"
                                "  <td class=\"status\">%5</td>\n"
                                "  <td class=\"action\">%6</td>\n"
                                "</tr>\n";
qDebug() << pClass << "container for " << pName << " is " << pIcon;
    return html.arg(pClass, pId, pIcon, pName, pStatus, actionHtml(pActions));
}

//==============================================================================

QString PmrWorkspacesWidget::fileHtml(const QString &pName, const QString &pStatus,
                                      const QList<QPair<QString, QString> > &pActions)
{
    static const QString html = "<tr>\n"
                                "  <td colspan=\"2\">%1</td>\n"
                                "  <td class=\"status\">%2</td>\n"
                                "  <td class=\"action\">%3</td>\n"
                                "</tr>\n";
qDebug() << "File " << pName;
static bool nomod = false;
nomod = !nomod;
    return html.arg(pName, nomod ? pStatus : "M", actionHtml(pActions));
}

//==============================================================================

QString PmrWorkspacesWidget::emptyContentsHtml(void)
{
    static const QString html = "<tr></tr>\n";

    return html;
}

//==============================================================================

QString PmrWorkspacesWidget::contentsHtml(const QString &pPath) const
{
    static const QString html = "<tr>\n"
                                "  <td></td>\n"
                                "  <td colspan=\"3\">\n"
                                "    <table>\n"
                                "      <tr class=\"packer\">\n"
                                "        <td class=\"icon\"><img /></td>\n"
                                "        <td class=\"name\"></td>\n"
                                "        <td class=\"status\"><img /></td>\n"
                                "        <td class=\"action\"><img /></td>\n"
                                "      </tr>\n"
                                "      %1\n"
                                "    </table>\n"
                                "  </td>\n"
                                "</tr>\n";

    QStringList itemHtml;

    QDir directory = QDir(pPath);
    if (directory.exists()) {
        QFileInfo info;
        foreach(info, directory.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name)) {
            if (info.isDir()) itemHtml << folderHtml(info.absoluteFilePath());
            else              itemHtml << fileHtml(info.fileName(), "", QList<QPair<QString, QString> >()); // status, actions
        }
    }

    return html.arg(itemHtml.join("\n"));
}

//==============================================================================


QStringList PmrWorkspacesWidget::workspaceHtml(const QString &pUrl, const QString &pName, const QString &pPath) const
{
    QString icon = pPath.isEmpty()               ? "clone"
                 : mExpandedItems.contains(pUrl) ? "close"
                 :                                 "open";

    auto actionList = QList<QPair<QString, QString> >()
                    << QPair<QString, QString>("about", pUrl);
    QStringList html = QStringList(containerHtml("workspace", icon, pUrl, pName, "", actionList));

    if (!pPath.isEmpty() /*&& mExpandedItems.contains(pUrl)*/) html << contentsHtml(pPath);
    else                                                   html << emptyContentsHtml();

    return html;
}

//==============================================================================

QStringList PmrWorkspacesWidget::folderHtml(const QString &pPath) const
{
    QFileInfo info = QFileInfo(pPath);
    QString fullname = info.absoluteFilePath();

    QString icon = mExpandedItems.contains(fullname) ? "close"
                 :                                     "open";

    QStringList html = QStringList(containerHtml("folder", icon, fullname, info.fileName(), "", QList<QPair<QString, QString> >())); // status, actions

    if (true /* mExpandedItems.contains(fullname) */) html << contentsHtml(pPath);
    else                                   html << emptyContentsHtml();
qDebug() << "Folder " << pPath << "  Name: " << fullname;
    return html;
}

//==============================================================================

void PmrWorkspacesWidget::clearWorkspaces(void)
{
//    mWorkspaceNames = QStringList();
//    mWorkspaceUrlId = QMap<QString, int>();

    setHtml(mTemplate.arg(QString()));
}

//==============================================================================

void PmrWorkspacesWidget::initialiseWorkspaces(const PMRSupport::PmrWorkspaceList &pWorkspaceList)
{
/*

    A workspace view is either closed or open (= contents visible).
    Contents are an alphabetical list of non-hidden files and folders.
    A folder may be closed or open. If open then show its contents.

        * workspace identified by its URL
        * file/folder (within a workspace) identified by its full path

    Operations:

        1. Load settings (open/closed/selection)
        2. When list of workspaces received (from PMR), update settings as details are displayed.
            a. If workspace is known then get its state, otherwise add workspace with state closed.
            b. If open then get (top-level) files/folders from file system.
                i. For each folder check if known and get its state or add folder with state closed.
                ii. If folder open then recurse.
            c. Highlight selected item if it exists.
            d. Remove all workspaces/folders that no longer exist.
        3. Save settings.

    Need list of expanded items. Is there a need to prune this list and remove unknown items? Maybe only
    when saving?? In which case we need QMap<QString, bool>.

    Save just the identifiers; there's no need to group them -- it's just a list of expanded items.

*/
//    mWorkspaceNames = QStringList();
//    mWorkspaceUrlId = QMap<QString, int>();

    QStringList html;

    foreach (PMRSupport::PmrWorkspace *workspace, pWorkspaceList) {
        QString url = workspace->url();
        QString name = workspace->name();

        // Want to have a LocalWorkspace...
        QString path = workspacePath(url, name);

        html.append(workspaceHtml(url, name, path));
        }
//qDebug() << mTemplate.arg(html.join("\n"));
    setHtml(mTemplate.arg(html.join("\n")));
    // Can we position view so that the selected line is shown??
}

//==============================================================================

{



void PmrWorkspacesWidget::linkClicked()
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

        if (!linkList[0].compare("about")) {
            mPmrRepository->requestWorkspaceInformation(linkList[1]);
            // And somewhere connect workspaceInformation SIGNAL to display result
        }
    }
}

//==============================================================================

void PmrWorkspacesWidget::linkHovered()
void PmrWorkspacesWidget::refreshWorkspaces(const bool &pScanFolders)
{
    // Retrieve some information about the link
    if (pScanFolders) scanDefaultWorkspaceDirectory();

    mPmrRepository->requestWorkspacesList();
}

    QString link;
    QString textContent;
//==============================================================================

    QWebElement element = retrieveLinkInformation(link, textContent);

    // Update our tool tip based on whether we are hovering a text or button
    // link
    // Note: this follows the approach used in linkClicked()...

    QString linkToolTip = QString();

    if (textContent.isEmpty()) {
        QStringList linkList = link.split("|");

        if (!linkList[0].compare("about")) {
            linkToolTip = tr("About the Workspace");
        }
    }

    setLinkToolTip(linkToolTip);
{

}







}

//==============================================================================

{
    }
}

//==============================================================================

{


        }

        }
    }

}

//==============================================================================

{

        }
    }
}

//==============================================================================

{






    }

        }
    }
}

//==============================================================================
//==============================================================================

{




    }







    }




    }


}

//==============================================================================

{

}

//==============================================================================

{
}

//==============================================================================

{






    }
}

//==============================================================================

void PmrWorkspacesWidget::workspaceCreated(const QString &pUrl)
{
    Q_UNUSED(pUrl)

    // Ignore as `workspaceCloned` will be called next
}

//==============================================================================

void PmrWorkspacesWidget::workspaceCloned(PMRSupport::PmrWorkspace *pWorkspace)
{
    if (pWorkspace) {
        QString url = pWorkspace->url();

        if (!mWorkspacesMap.contains(url))
            addWorkspace(pWorkspace);

        // Redisplay with workspace expanded and selected

        mExpandedItems.insert(url);
        displayWorkspaces();
        setSelected(url);
    }
}

//==============================================================================

}   // namespace PMRWorkspaces
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
