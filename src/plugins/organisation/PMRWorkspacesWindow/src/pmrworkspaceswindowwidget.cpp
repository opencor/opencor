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
// PMR Workspaces window widget
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "i18ninterface.h"
#include "pmrwebservice.h"
#include "pmrworkspacemanager.h"
#include "pmrworkspaceswindowcommitdialog.h"
#include "pmrworkspaceswindowwidget.h"
#include "treeviewwidget.h"
#include "usermessagewidget.h"

//==============================================================================

#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QDir>
#include <QLayout>
#include <QMainWindow>
#include <QMenu>
#include <QSettings>
#include <QTimer>

//==============================================================================

#include "git2/remote.h"
#include "git2/repository.h"

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspacesWindow {

//==============================================================================

PmrWorkspacesWindowItem::PmrWorkspacesWindowItem(const Type &pType,
                                                 const QString &pText,
                                                 const QString &pUrl) :
    QStandardItem(pText),
    mType(pType),
    mUrl(pUrl)
{
    // Customise ourselves

    static const QIcon WorkspaceIcon     = QIcon(":/oxygen/places/folder.png");
    static const QIcon WorkspaceFileIcon = QIcon(":/oxygen/mimetypes/application-x-zerosize.png");

    QStandardItem::setIcon((pType == Workspace)?WorkspaceIcon:WorkspaceFileIcon);

    setToolTip(pText);
}

//==============================================================================

int PmrWorkspacesWindowItem::type() const
{
    // Return our type

    return mType;
}

//==============================================================================

QString PmrWorkspacesWindowItem::url() const
{
    // Return our URL

    return mUrl;
}

//==============================================================================

static const auto AboutAction           = QStringLiteral("about");
static const auto CloneAction           = QStringLiteral("clone");
static const auto CommitAction          = QStringLiteral("commit");
static const auto RefreshAction         = QStringLiteral("refresh");
static const auto StageAction           = QStringLiteral("stage");
static const auto UnstageAction         = QStringLiteral("unstage");
static const auto SynchronizeAction     = QStringLiteral("synchronize");
static const auto SynchronizePushAction = QStringLiteral("synchronizePush");
static const auto SynchronizePullAction = QStringLiteral("synchronizePull");

//==============================================================================

static const auto AboutIcon           = QStringLiteral(":/oxygen/actions/help-about.png");
static const auto CloneIcon           = QStringLiteral(":/oxygen/places/folder-downloads.png");
static const auto CommitIcon          = QStringLiteral(":/oxygen/actions/dialog-ok-apply.png");
static const auto FolderOwnedIcon     = QStringLiteral(":/PMRWorkspacesWindow/icons/folder-owned.png");
static const auto FolderOwnedOpenIcon = QStringLiteral(":/PMRWorkspacesWindow/icons/folder-owned-open.png");
static const auto FolderIcon          = QStringLiteral(":/oxygen/places/folder.png");
static const auto FolderOpenIcon      = QStringLiteral(":/oxygen/places/folder-open.png");
static const auto RefreshIcon         = QStringLiteral(":/oxygen/actions/view-refresh.png");
static const auto StageIcon           = QStringLiteral(":/oxygen/actions/list-add.png");
static const auto UnstageIcon         = QStringLiteral(":/oxygen/actions/list-remove.png");
static const auto SynchronizeIcon     = QStringLiteral(":/PMRWorkspacesWindow/icons/synchronize.png");
static const auto SynchronizePushIcon = QStringLiteral(":/PMRWorkspacesWindow/icons/synchronize-push.png");
static const auto SynchronizePullIcon = QStringLiteral(":/PMRWorkspacesWindow/icons/synchronize-pull.png");

//==============================================================================
//---GRY--- TODO:
//           - Shouldn't mTimer be used to refresh *all* workspaces and not only
//             the current one?...
//           - We want to be able to expand *all* workspaces, if the user so
//             desires...
//           - Check and optimise (e.g. create the menu actions once and for
//             all) contextMenuEvent()...
//           - Get rid of mouseMoveEvent() and mousePressEvent() and handle the
//             QWebPage::linkClicked() and QWebPage::linkHovered() signals
//             instead...
//           - Double check loadSettings() and saveSettings() (e.g. we want to
//             be able to have any number of workspaces expanded, not just
//             one)...

PmrWorkspacesWindowWidget::PmrWorkspacesWindowWidget(PMRSupport::PmrWebService *pPmrWebService,
                                                     QWidget *pParent) :
    Core::Widget(pParent),
    mPmrWebService(pPmrWebService),
    mWorkspaceManager(PMRSupport::PmrWorkspaceManager::instance()),
    mWorkspaceFolderUrls(QMap<QString, QString>()),
    mWorkspaceUrlFolderMines(QMap<QString, QPair<QString, bool>>()),
    mCurrentWorkspaceUrl(QString()),
    mExpandedItems(QSet<QString>()),
    mInitialized(false),
    mErrorMessage(QString()),
    mAuthenticated(false)
{
    // Create and customise some objects

    mUserMessageWidget = new Core::UserMessageWidget(this);

    mUserMessageWidget->setScale(0.85);

    mTreeViewModel = new QStandardItemModel(this);
    mTreeViewWidget = new Core::TreeViewWidget(this);

    mTreeViewWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    mTreeViewWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mTreeViewWidget->setHeaderHidden(true);
    mTreeViewWidget->setModel(mTreeViewModel);

qDebug("---[Tree view widget signals]---");
    connect(mTreeViewWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu(const QPoint &)));

    connect(mTreeViewWidget, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(itemDoubleClicked(const QModelIndex &)));
    connect(mTreeViewWidget, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SIGNAL(itemDoubleClicked()));

    connect(mTreeViewWidget, SIGNAL(expanded(const QModelIndex &)),
            this, SLOT(expandedExposure(const QModelIndex &)));
    connect(mTreeViewWidget, SIGNAL(collapsed(const QModelIndex &)),
            this, SLOT(collapsedExposure(const QModelIndex &)));
qDebug("--------------------------------");

    // Populate ourselves

    createLayout();

    layout()->addWidget(mUserMessageWidget);
    layout()->addWidget(mTreeViewWidget);

    // Connections to handle responses from PMR

    connect(mPmrWebService, SIGNAL(workspaceCloned(PMRSupport::PmrWorkspace *)),
            this, SLOT(workspaceCloned(PMRSupport::PmrWorkspace *)));
    connect(mPmrWebService, SIGNAL(workspaceSynchronized(PMRSupport::PmrWorkspace *)),
            this, SLOT(workspaceSynchronized(PMRSupport::PmrWorkspace *)));

    // Connection to handle a response from our workspace manager

    connect(mWorkspaceManager, SIGNAL(workspaceCloned(PMRSupport::PmrWorkspace *)),
            this, SLOT(workspaceCloned(PMRSupport::PmrWorkspace *)));

    // Create a timer for refreshing our workspaces

    mTimer = new QTimer(this);

    connect(mTimer, SIGNAL(timeout()),
            this, SLOT(refreshWorkspaces()));

    // Keep track of when OpenCOR gets/loses the focus, so we can enable/disable
    // the timer that we use to refresh our workspaces
    // Note: the focusWindowChanged() signal comes from QGuiApplication, so we
    //       need to check whether we are running the console version of OpenCOR
    //       or its GUI version...

    if (dynamic_cast<QGuiApplication *>(QCoreApplication::instance())) {
        connect(qApp, SIGNAL(focusWindowChanged(QWindow *)),
                this, SLOT(focusWindowChanged()));
    }

    // Create and populate our context menu

    mContextMenu = new QMenu(this);

    mViewInPmrAction = Core::newAction(this);
    mCopyUrlAction = Core::newAction(this);

    connect(mViewInPmrAction, SIGNAL(triggered(bool)),
            this, SLOT(viewInPmr()));
    connect(mCopyUrlAction, SIGNAL(triggered(bool)),
            this, SLOT(copyUrl()));

    mContextMenu->addAction(mViewInPmrAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mCopyUrlAction);

    // Make our tree view widget our focus proxy

    setFocusProxy(mTreeViewWidget);
}

//==============================================================================

PmrWorkspacesWindowWidget::~PmrWorkspacesWindowWidget()
{
    // Delete some internal objects

    delete mTimer;
}

//==============================================================================

void PmrWorkspacesWindowWidget::retranslateUi()
{
    // Retranslate our actions

    I18nInterface::retranslateAction(mViewInPmrAction, tr("View In PMR"),
                                     tr("View in PMR"));
    I18nInterface::retranslateAction(mCopyUrlAction, tr("Copy URL"),
                                     tr("Copy the URL to the clipboard"));

    // Retranslate our message, if we have been initialised

    if (mInitialized)
        updateMessage();
}

//==============================================================================

static const auto SettingsWorkspaces       = QStringLiteral("Workspaces");
static const auto SettingsFolders          = QStringLiteral("Folders");
static const auto SettingsExpandedItems    = QStringLiteral("ExpandedItems");
static const auto SettingsCurrentWorkspace = QStringLiteral("CurrentWorkspace");

//==============================================================================

void PmrWorkspacesWindowWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve our settings

    pSettings->beginGroup(SettingsWorkspaces);
        // Retrieve the current workspace URL, if any

        mCurrentWorkspaceUrl = pSettings->value(SettingsCurrentWorkspace).toString();

        // Retrieve the names of folders containing cloned workspaces

        foreach (const QString &folder, pSettings->value(SettingsFolders).toStringList()) {
            if (!folder.isEmpty()) {
                QString url = addWorkspaceFolder(folder);

                // Ensure only the current workspace is expanded
//---GRY--- Hmm... this seems useless since we initialise mExpandedItems after
//          this foreach() statement?...

                if (!url.compare(mCurrentWorkspaceUrl))
                    mExpandedItems.insert(mCurrentWorkspaceUrl);
                else if (mExpandedItems.contains(url))
                    mExpandedItems.remove(url);
            }
        }

        // Retrieve the names of expanded workspaces and folders

        mExpandedItems = pSettings->value(SettingsExpandedItems).toStringList().toSet();
    pSettings->endGroup();
}

//==============================================================================

void PmrWorkspacesWindowWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of our settings

    pSettings->beginGroup(SettingsWorkspaces);
        // Keep track of the names of folders containing cloned workspaces

        pSettings->setValue(SettingsFolders, QVariant(mWorkspaceFolderUrls.keys()));

        // Keep track of the names of expanded workspaces and folders

        pSettings->setValue(SettingsExpandedItems, QVariant(mExpandedItems.toList()));

        // Keep track of the current workspace URL

        pSettings->setValue(SettingsCurrentWorkspace, mCurrentWorkspaceUrl);
    pSettings->endGroup();
}

//==============================================================================

static const auto ATag  = QStringLiteral("A");
static const auto TrTag = QStringLiteral("TR");

//==============================================================================

static const auto AStatus  = QStringLiteral("A");
static const auto CStatus  = QStringLiteral("C");
static const auto DStatus  = QStringLiteral("D");
static const auto MStatus  = QStringLiteral("M");

//==============================================================================

static const auto StatusClass  = QStringLiteral("status");
static const auto IStatusClass = QStringLiteral("iStatus");
static const auto WStatusClass = QStringLiteral("wStatus");

//==============================================================================

/*---GRY---
void PmrWorkspacesWindowWidget::contextMenuEvent(QContextMenuEvent *pEvent)
{
//---GRY--- To be reviewed...
    QMenu *menu = new QMenu(this);
    QWebElement trElement = page()->mainFrame()->hitTestContent(pEvent->pos()).element();

    while (!trElement.isNull() && trElement.tagName().compare(TrTag))
        trElement = trElement.parent();

    if (!trElement.isNull() && trElement.hasClass("workspace")) {
        QString elementId = trElement.attribute("id");

        if (!trElement.findFirst("img.clone").isNull()) {
            QAction *action = new QAction(QIcon(CloneIcon), tr("Clone Workspace"), this);

            action->setData(QString(CloneAction+"|%1").arg(elementId));

            menu->addAction(action);
        } else {
            if (!trElement.findFirst("img.commit").isNull()) {
                QAction *action = new QAction(QIcon(CommitIcon), tr("Commit Changes"), this);

                action->setData(QString(CommitAction+"|%1").arg(elementId));

                menu->addAction(action);
            }

            if (!trElement.findFirst("img.synchronizePull").isNull()) {
                QAction *action = new QAction(QIcon(SynchronizePullIcon), tr("Synchronise"), this);

                action->setData(QString(SynchronizePullAction+"|%1").arg(elementId));

                menu->addAction(action);
            }

            if (!trElement.findFirst("img.synchronize").isNull()) {
                QAction *action = new QAction(QIcon(SynchronizeIcon), tr("Synchronise"), this);

                action->setData(QString(SynchronizeAction+"|%1").arg(elementId));

                menu->addAction(action);
            } else if (!trElement.findFirst("img.synchronizePush").isNull()) {
                QAction *action = new QAction(QIcon(SynchronizePushIcon), tr("Synchronise And Upload"), this);

                action->setData(QString(SynchronizePushAction+"|%1").arg(elementId));

                menu->addAction(action);
            }
        }

        menu->addSeparator();

        QAction *refreshAction = new QAction(QIcon(RefreshIcon), tr("Refresh Display"), this);

        refreshAction->setData(QString(RefreshAction+"|%1").arg(elementId));

        menu->addAction(refreshAction);

        PMRSupport::PmrWorkspace *workspace = mWorkspaceManager->workspace(elementId);

        if (workspace) {
            QAction *action = new QAction(tr("View In PMR..."), this);

            action->setData(QString("pmr|%1").arg(workspace->url()));

            menu->addAction(action);

            if (workspace->isLocal()) {
                action = new QAction(tr("Show Containing Folder..."), this);

                action->setData(QString("show|%1").arg(workspace->path()));

                menu->addAction(action);
            }
        }

        menu->addSeparator();

        QAction *aboutAction = new QAction(QIcon(AboutIcon), tr("About Workspace"), this);

        aboutAction->setData(QString(AboutAction+"|%1").arg(elementId));

        menu->addAction(aboutAction);
    }

    if (!menu->isEmpty()) {
        QAction *item = menu->exec(pEvent->globalPos());

        if (item) {
            QStringList linkList = item->data().toString().split("|");
            QString action = linkList[0];
            QString linkId = linkList[1];

            if (!action.compare(AboutAction)) {
                aboutWorkspace(linkId);
            } else if (!action.compare(CloneAction)) {
                cloneWorkspace(linkId);
            } else if (!action.compare(RefreshAction)) {
                refreshWorkspace(linkId);
            } else if (   !action.compare(SynchronizePullAction)
                       || !action.compare(SynchronizeAction)) {
                synchronizeWorkspace(linkId, false);
            } else if (!action.compare(CommitAction)) {
                commitWorkspace(linkId);
            } else if (!action.compare(SynchronizePushAction)) {
                synchronizeWorkspace(linkId);
            } else if (!action.compare("pmr")) {
                QDesktopServices::openUrl(linkId);
            } else if (!action.compare("show")) {
                showInGraphicalShell(linkId);
            }
        }
    }
}
*/

//==============================================================================

void PmrWorkspacesWindowWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
Q_UNUSED(pEvent);
/*---GRY---
    // Default handling of the event
    // Note: this will ensure that CSS works as expected...

    WebViewerWidget::WebViewerWidget::mouseMoveEvent(pEvent);

    // Determine the element that is being hovered

    QWebElement element = page()->mainFrame()->hitTestContent(pEvent->pos()).element();

    while (   !element.isNull()
           &&  element.tagName().compare(ATag)
           &&  element.tagName().compare(TrTag)
           && !element.hasClass(StatusClass)
           && !element.hasClass(IStatusClass)
           && !element.hasClass(WStatusClass)) {
        element = element.parent();
    }

    // Determine the tool tip to show for the element that is being hovered, if
    // any

    QString toolTip = QString();

    if (!element.isNull()) {
        if (   !element.tagName().compare(ATag)
            && !element.attribute("href").isEmpty()) {
            QString action = element.attribute("href").split("|")[0];

            if (!action.compare(StageAction)) {
                toolTip = tr("Stage");
            } else if (!action.compare(UnstageAction)) {
                toolTip = tr("Unstage");
            } else if (element.parent().parent().hasClass("file")) {
                toolTip = tr("Open File");
            } else if (!action.compare(CloneAction)) {
                toolTip = tr("Clone Workspace");
            } else if (   !action.compare(SynchronizeAction)
                       || !action.compare(SynchronizePullAction)) {
                toolTip = tr("Synchronise");
            } else if (!action.compare(SynchronizePushAction)) {
                toolTip = tr("Synchronise And Upload");
            } else if (!action.compare(CommitAction)) {
                toolTip = tr("Commit Staged Changes");
            }
        } else if (   element.hasClass(StatusClass)
                   || element.hasClass(IStatusClass)
                   || element.hasClass(WStatusClass)) {
            QString statusChar = element.toPlainText().trimmed();

            if (element.hasClass(StatusClass)) {
                if (!statusChar.compare(CStatus))
                    toolTip = tr("Conflicts");
            } else {
                if (!statusChar.compare(AStatus))
                    toolTip = tr("Added");
                else if (!statusChar.compare(CStatus))
                    toolTip = tr("Conflicts");
                else if (!statusChar.compare(DStatus))
                    toolTip = tr("Deleted");
                else if (!statusChar.compare(MStatus))
                    toolTip = tr("Modified");
            }
        }
    }

    setToolTip(toolTip);
*/
}

//==============================================================================

void PmrWorkspacesWindowWidget::mousePressEvent(QMouseEvent *pEvent)
{
Q_UNUSED(pEvent);
/*---GRY---
    WebViewerWidget::WebViewerWidget::mousePressEvent(pEvent);

    if (pEvent->button() == Qt::LeftButton) {
        // Find the containing row and highlight it

        QWebElement trElement = page()->mainFrame()->hitTestContent(pEvent->pos()).element();

        while (!trElement.isNull() && trElement.tagName().compare(TrTag))
            trElement = trElement.parent();

        // Select the row that's been clicked in before doing anything else

        QString rowLink = trElement.attribute("id");

        QWebElement aElement = page()->mainFrame()->hitTestContent(pEvent->pos()).element();

        while (   !aElement.isNull()
               &&  aElement.tagName().compare(ATag)
               &&  aElement.tagName().compare(TrTag)) {
            aElement = aElement.parent();
        }

        // Check if an anchor element has been clicked

        QString aLink = aElement.attribute("href");

        if (   !aElement.isNull() && !aElement.tagName().compare(ATag)
            && !aLink.isEmpty()) {
            if (aElement.toPlainText().isEmpty() && aLink.contains("|")) {
                QStringList linkList = aLink.split("|");
                QString action = linkList[0];
                QString linkUrl = linkList[1];

                if (!action.compare(CloneAction)) {
                    cloneWorkspace(linkUrl);
                } else if (!action.compare(CommitAction)) {
                    commitWorkspace(linkUrl);
                } else if (   !action.compare(SynchronizeAction)
                           || !action.compare(SynchronizePullAction)) {
                    synchronizeWorkspace(linkUrl, false);
                } else if (!action.compare(SynchronizePushAction)) {
                    synchronizeWorkspace(linkUrl);
                } else if (!action.compare(StageAction) || !action.compare(UnstageAction)) {
                    QWebElement workspaceElement = parentWorkspaceElement(trElement);

                    if (!workspaceElement.isNull()) {
                        // Stage/unstage the file

                        PMRSupport::PmrWorkspace *workspace = mWorkspaceManager->workspace(workspaceElement.attribute("id"));

                        workspace->stageFile(linkUrl, (!action.compare(StageAction)));

                        // Now update the file's status and action

                        QStringList statusActionHtml = fileStatusActionHtml(workspace, linkUrl);
                        QWebElement statusElement = trElement.findFirst("td.status");

                        if (!statusElement.isNull()) {
                            if (statusActionHtml[0].isEmpty())
                                statusElement.addClass("hidden");
                            else
                                statusElement.removeClass("hidden");

                            statusElement.setInnerXml(statusActionHtml[0]);
                        }

                        QWebElement actionElement = trElement.findFirst("td.actions");

                        if (!actionElement.isNull()) {
                            if (statusActionHtml[0].isEmpty())
                                actionElement.addClass("hidden");
                            else
                                actionElement.removeClass("hidden");

                            actionElement.setInnerXml(statusActionHtml[1]);
                        }

                        // And update the workspace's header icons

                        refreshWorkspace(workspace->url());
                    }
               }
            } else if (!aLink.isEmpty()) {
                // Text link clicked, e.g. to open a file

                emit openFileRequested(aLink);
            }
        } else if (trElement.hasClass("workspace")) {
            if (rowLink == mCurrentWorkspaceUrl)
                expandHtmlTree(rowLink);
            else
                setCurrentWorkspaceUrl(rowLink);

            if (mExpandedItems.contains(rowLink))
                refreshWorkspace(rowLink);
        } else if (trElement.hasClass("folder")) {
            expandHtmlTree(rowLink);
        }
    }
*/
}

//==============================================================================

QSize PmrWorkspacesWindowWidget::sizeHint() const
{
    // Suggest a default size for our PMR workspaces widget
    // Note: this is critical if we want a docked widget, with a PMR workspaces
    //       widget on it, to have a decent size when docked to the main
    //       window...

    return defaultSize(0.15);
}

//==============================================================================

void PmrWorkspacesWindowWidget::updateMessage()
{
    // Determine the message to be displayed, if any

    if (!mAuthenticated) {
        mUserMessageWidget->setIconMessage(":/oxygen/actions/help-hint.png",
                                           tr("Authenticate yourself..."),
                                           tr("Click on the top-right button."));
    } else if (mErrorMessage.isEmpty()) {
        if (!mWorkspaceManager->count()) {
            mUserMessageWidget->setIconMessage(":/oxygen/actions/help-about.png",
                                               tr("No workspaces were found..."));
        } else {
            mUserMessageWidget->resetMessage();
        }
    } else {
        mUserMessageWidget->setIconMessage(":/oxygen/emblems/emblem-important.png",
                                           Core::formatMessage(mErrorMessage, false, true));
    }

    // Show/hide our user message widget and our tree view widget

    mUserMessageWidget->setVisible(!mUserMessageWidget->text().isEmpty());
    mTreeViewWidget->setVisible(mUserMessageWidget->text().isEmpty());
}

//==============================================================================

void PmrWorkspacesWindowWidget::resizeTreeViewToContents()
{
    // Resize our tree view widget so that all of its contents is visible

    mTreeViewWidget->resizeColumnToContents(0);
}

//==============================================================================

PmrWorkspacesWindowItem * PmrWorkspacesWindowWidget::currentItem() const
{
    // Return our current item

    return static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(mTreeViewWidget->currentIndex()));
}

//==============================================================================

void PmrWorkspacesWindowWidget::showCustomContextMenu(const QPoint &pPosition) const
{
    // Determine whether to show the context menu based on whether we are over
    // an item

    PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(mTreeViewWidget->indexAt(pPosition)));

    if (item) {
        // We are over an item, so update our context menu and show it

        mContextMenu->exec(QCursor::pos());
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::addWorkspace(PMRSupport::PmrWorkspace *pWorkspace,
                                             const bool &pOwned)
{
    // Add the given workspace, if it hasn't already been added

    QString folder = pWorkspace->path();
    QString url = pWorkspace->url();

    if (!mWorkspaceFolderUrls.contains(folder)) {
        if (mWorkspaceUrlFolderMines.contains(url)) {
            // Let the user know that there is already a workspace with that URL

            duplicateCloneMessage(url, mWorkspaceUrlFolderMines.value(url).first, folder);
        } else {
            // The given workspace hasn't already been added, so keep track of
            // it and ask our workspace manager to do the same

            mWorkspaceFolderUrls.insert(folder, url);
            mWorkspaceUrlFolderMines.insert(url, QPair<QString, bool>(folder, pOwned));

            mWorkspaceManager->addWorkspace(pWorkspace);
        }
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::duplicateCloneMessage(const QString &pUrl,
                                                      const QString &pPath1,
                                                      const QString &pPath2)
{
    // Let people know that the given workspace has been cloned twice, but only
    // if it doesn't occur when initialising ourselves
    // Note: indeed, someone handling the warning() signal might decide to show
    //       a dialog box to warn the user. Such a thing is fine when everything
    //       is initialised, but not when initialising ourselves (since the
    //       dialog box would show up from nowhere)...

    if (mInitialized)
        emit warning(QString("Workspace '%1' is cloned into both '%2' and '%3'").arg(pUrl, pPath1, pPath2));
}

//==============================================================================

QString PmrWorkspacesWindowWidget::addWorkspaceFolder(const QString &pFolder)
{
    // Add the given workspace folder, if it's not already added

    if (!mWorkspaceFolderUrls.contains(pFolder)) {
        // Retrieve the workspace URL (i.e. remote.origin.url) for the given
        // folder

        QString res = QString();
        QByteArray folderByteArray = pFolder.toUtf8();
        git_repository *gitRepository = 0;

        if (!git_repository_open(&gitRepository, folderByteArray.constData())) {
            git_strarray remotes;

            if (!git_remote_list(&remotes, gitRepository)) {
                for (size_t i = 0; i < remotes.count; ++i) {
                    char *name = remotes.strings[i];

                    if (!strcmp(name, "origin")) {
                        git_remote *remote = 0;

                        if (!git_remote_lookup(&remote, gitRepository, name)) {
                            const char *remoteUrl = git_remote_url(remote);

                            if (remoteUrl)
                                res = QString(remoteUrl);
                        }
                    }
                }
            }

            git_repository_free(gitRepository);
        }

        // Keep track of the workspace space URL, if there is one and it's not
        // already tracked

        if (!res.isEmpty()) {
            if (mWorkspaceUrlFolderMines.contains(res)) {
                duplicateCloneMessage(res, mWorkspaceUrlFolderMines.value(res).first, pFolder);
            } else {
                mWorkspaceFolderUrls.insert(pFolder, res);
                mWorkspaceUrlFolderMines.insert(res, QPair<QString, bool>(pFolder, false));
            }
        }

        return res;
    } else {
        return mWorkspaceFolderUrls.value(pFolder);
    }
}

//==============================================================================

const QString PmrWorkspacesWindowWidget::actionHtml(const StringPairs &pActions)
{
//---GRY--- To be reviewed...
    QString actions = QString();

    foreach (const StringPair &action, pActions)
        actions += QString("<a class=\"noHover\" href=\"%1|%2\"><img class=\"button %1\"></a>").arg(action.first, action.second);

    return actions;
}

//==============================================================================

QStringList PmrWorkspacesWindowWidget::fileStatusActionHtml(const QString &pPath,
                                                            const PMRSupport::CharPair &pGitStatus)
{
//---GRY--- To be reviewed...
    static const QString StatusHtml = "<span class=\"iStatus\">%1</span><span class=\"wStatus\">%2</span>";

    StringPairs actionList = StringPairs();

    if (pGitStatus.second != ' ')
        actionList << StringPair(StageAction, pPath);
    else if (pGitStatus.first != ' ')
        actionList << StringPair(UnstageAction, pPath);

    return QStringList() << StatusHtml.arg(pGitStatus.first).arg(pGitStatus.second)
                         << actionHtml(actionList);
}

//==============================================================================

QStringList PmrWorkspacesWindowWidget::fileStatusActionHtml(const PMRSupport::PmrWorkspace *pWorkspace,
                                                            const QString &pPath)
{
//---GRY--- To be reviewed...
    return fileStatusActionHtml(pPath, pWorkspace->gitFileStatus(pPath));
}

//==============================================================================

QStringList PmrWorkspacesWindowWidget::fileStatusActionHtml(const PMRSupport::PmrWorkspaceFileNode *pFileNode)
{
//---GRY--- To be reviewed...
    return fileStatusActionHtml(pFileNode->fullName(), pFileNode->status());
}

//==============================================================================

QString PmrWorkspacesWindowWidget::fileHtml(const PMRSupport::PmrWorkspaceFileNode *pFileNode)
{
//---GRY--- To be reviewed...
    static const QString Html = "<tr class=\"file\" id=\"%1\">\n"
                                "    <td colspan=\"2\" class=\"name\"><a href=\"%1\">%2</a></td>\n"
                                "    <td class=\"status%3\">%4</td>\n"
                                "    <td class=\"action%5\">%6</td>\n"
                                "</tr>\n";
    QString path = pFileNode->fullName();

    ++mRow;

    QStringList statusActionHtml = fileStatusActionHtml(pFileNode);

    // Use an anchor element to allow us to set the scroll position at a row

    return Html.arg(path, pFileNode->shortName(),
                    statusActionHtml[0].isEmpty()?" hidden":QString(),
                    statusActionHtml[0],
                    statusActionHtml[1].isEmpty()?" hidden":QString(),
                    statusActionHtml[1]);
}

//==============================================================================

QString PmrWorkspacesWindowWidget::containerHtml(const QString &pClass,
                                                 const QString &pId,
                                                 const QString &pIcon,
                                                 const QString &pName,
                                                 const QString &pStatus,
                                                 const StringPairs &pActions)
{
    // Generate and return the HTML code for the given container

    static const QString Html = "                <tr class=\"%1\" id=\"%2\">\n"
                                "                    <td class=\"icon\">\n"
                                "                        <a>%3</a>\n"
                                "                    </td>\n"
                                "                    <td class=\"name fullWidth\">\n"
                                "                        %4\n"
                                "                    </td>\n"
                                "%5"
                                "%6"
                                "                </tr>\n";
    static const QString StatusHtml = "                    <td class=\"status\">\n"
                                      "                        %1\n"
                                      "                    </td>\n";
    static const QString NoStatusHtml = "                    <td class=\"status hidden\">\n"
                                        "                    </td>\n";
    static const QString ActionsHtml = "                    <td class=\"actions button\">\n"
                                       "                        %1\n"
                                       "                    </td>\n";
    static const QString NoActionsHtml = "                    <td class=\"actions button hidden\">\n"
                                         "                    </td>\n";

    const QString iconHtml = QString("<img class=\"%1\">").arg(pIcon);

    // Use an anchor element to allow us to set the scroll position at a row

    QString actions = actionHtml(pActions);

    return Html.arg(pClass, pId, iconHtml, pName,
                    pStatus.isEmpty()?NoStatusHtml:StatusHtml.arg(pStatus),
                    actions.isEmpty()?NoActionsHtml:ActionsHtml.arg(actions));
}

//==============================================================================

QString PmrWorkspacesWindowWidget::fileNodeContentsHtml(const PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                                        const bool &pHidden)
{
    // Generate and return the HTML code for the contents of the given file node

    if (pFileNode) {
        static const QString Html = "                <tr class=\"contents%1\">\n"
                                    "                    <td></td>\n"
                                    "                    <td colspan=\"3\">\n"
                                    "                        <table>\n"
                                    "                            <tr class=\"spacing\">\n"
                                    "                                <td class=\"icon\"></td>\n"
                                    "                                <td class=\"name\"></td>\n"
                                    "                                <td class=\"status hidden\"></td>\n"
                                    "                                <td class=\"action hidden\"></td>\n"
                                    "                            </tr>\n"
                                    "%2"
                                    "                        </table>\n"
                                    "                    </td>\n"
                                    "                </tr>\n";
        static const QString ItemHtml = "                            %1\n";

        QStringList itemHtml = QStringList();

        foreach(PMRSupport::PmrWorkspaceFileNode *fileNode, pFileNode->children()) {
            if (fileNode->hasChildren())
                itemHtml << folderHtml(fileNode);
            else
                itemHtml << fileHtml(fileNode);
        }

        return Html.arg(pHidden?" hidden":QString(), itemHtml.isEmpty()?QString():ItemHtml.arg(itemHtml.join("\n")));
    } else {
        return emptyContentsHtml();
    }
}

//==============================================================================

QString PmrWorkspacesWindowWidget::emptyContentsHtml()
{
    // Generate and return the HTML code for an empty contents

    static const QString Html = "<tr></tr>\n";

    return Html;
}

//==============================================================================

QStringList PmrWorkspacesWindowWidget::workspaceHtml(const PMRSupport::PmrWorkspace *pWorkspace)
{
    // Generate and return the HTML code for the given workspace

    QString url = pWorkspace->url();
    QString path = pWorkspace->path();
    QString status = QString();
    StringPairs actions = StringPairs();
    PMRSupport::PmrWorkspace::WorkspaceStatus workspaceStatus = pWorkspace->gitWorkspaceStatus();

    if (path.isEmpty()) {
        actions << StringPair(CloneAction, url);
    } else if (workspaceStatus & PMRSupport::PmrWorkspace::StatusConflict) {
        // Indicate that there are merge conflicts

        status = "C";
    } else {
        // Indicate whether there are unstaged files

        if (workspaceStatus & PMRSupport::PmrWorkspace::StatusUnstaged)
            status = "?";

        // Indicate whether files are staged for commit

        if (workspaceStatus & PMRSupport::PmrWorkspace::StatusCommit)
            actions << StringPair(CommitAction, url);

        // Show synchronisation button, with different styles to indicate
        // exactly what it will do

        if (pWorkspace->isOwned()) {
            if (workspaceStatus & PMRSupport::PmrWorkspace::StatusAhead)
                actions << StringPair(SynchronizePushAction, url);
            else
                actions << StringPair(SynchronizeAction, url);
        } else {
            actions << StringPair(SynchronizePullAction, url);
        }
    }

    mRow = 0;

    QStringList html = QStringList() << containerHtml("workspace", url,
                                                      pWorkspace->isOwned()?
                                                          "folderOwned":
                                                          "folder",
                                                      pWorkspace->name(),
                                                      status, actions);

    if (!path.isEmpty())
        html << fileNodeContentsHtml(pWorkspace->rootFileNode(), !mExpandedItems.contains(url));
    else
        html << emptyContentsHtml();

    return html;
}

//==============================================================================

QStringList PmrWorkspacesWindowWidget::folderHtml(const PMRSupport::PmrWorkspaceFileNode *pFileNode)
{
//---GRY--- To be reviewed...
    QString fullname = pFileNode->fullName();
    QString icon = mExpandedItems.contains(fullname)?"folderOpen":"folder";

    ++mRow;

    QStringList html = QStringList(containerHtml("folder", fullname, icon,
                                                 pFileNode->shortName(),
                                                 QString(), StringPairs()));

    html << fileNodeContentsHtml(pFileNode, !mExpandedItems.contains(fullname));

    return html;
}

//==============================================================================

void PmrWorkspacesWindowWidget::setCurrentWorkspaceUrl(const QString &pUrl)
{
//---GRY--- To be reviewed...
    if (pUrl != mCurrentWorkspaceUrl) {
        // Close the current workspace if we are selecting a different one

        if (!mCurrentWorkspaceUrl.isEmpty()) {
/*---GRY---
            QWebElement workspaceContents = page()->mainFrame()->documentElement().findFirst(QString("tr.workspace[id=\"%1\"] + tr").arg(mCurrentWorkspaceUrl));

            if (!workspaceContents.isNull()) {
                workspaceContents.addClass("hidden");
                mExpandedItems.remove(mCurrentWorkspaceUrl);
            }
*/
        }

        // Set the new current workspace and ensure it is expanded when displayed

        mCurrentWorkspaceUrl = pUrl;

        mExpandedItems.insert(mCurrentWorkspaceUrl);

        // Set the active directory to the workspace

        PMRSupport::PmrWorkspace *workspace = mWorkspaceManager->workspace(pUrl);

        if (workspace->isLocal())
            Core::setActiveDirectory(workspace->path());
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::startStopTimer()
{
    // Start our timer if OpenCOR is active and we have a current workspace, or
    // stop it if either OpenCOR is not active or we no longer have have a
    // current workspace
    // Note: if we are to start our timer, then we refresh the workspace first
    //       since waiting one second may seem long to a user...

    if (   !mTimer->isActive()
        &&  Core::opencorActive() && !mCurrentWorkspaceUrl.isEmpty()) {
        disconnect(qApp, SIGNAL(focusWindowChanged(QWindow *)),
                   this, SLOT(focusWindowChanged()));

        refreshWorkspaces();

        connect(qApp, SIGNAL(focusWindowChanged(QWindow *)),
                this, SLOT(focusWindowChanged()));

        mTimer->start(1000);
    } else if (   mTimer->isActive()
               && (!Core::opencorActive() || mCurrentWorkspaceUrl.isEmpty())) {
        mTimer->stop();
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::focusWindowChanged()
{
    // Start/stop our timer

    startStopTimer();
}

//==============================================================================

void PmrWorkspacesWindowWidget::refreshWorkspaces()
{
    // Refresh our workspaces
//---GRY--- TO BE DONE...

    refreshWorkspace(mCurrentWorkspaceUrl);
}

//==============================================================================

void PmrWorkspacesWindowWidget::expandHtmlTree(const QString &pId)
{
Q_UNUSED(pId);
/*---GRY---
    QWebElement trElement = page()->mainFrame()->documentElement().findFirst(QString("tr[id=\"%1\"] + tr").arg(pId));

    if (!trElement.isNull()) {
        if (mExpandedItems.contains(pId)) {
            trElement.addClass("hidden");

            mExpandedItems.remove(pId);
        } else if (trElement.hasClass("contents")) {
            trElement.removeClass("hidden");

            mExpandedItems.insert(pId);
        }
    }
*/
}

//==============================================================================

void PmrWorkspacesWindowWidget::displayWorkspaces()
{
/*---GRY---
    // Retrieve our managed workspaces

    PMRSupport::PmrWorkspaces workspaces = mWorkspaceManager->workspaces();

    // Generate and set the HTML code, if we have some managed workspaces, or
    // update our message, if we don't

    if (workspaces.isEmpty()) {
        setHtml(mTemplate.arg(QString()));

        updateMessage();
    } else {
        // We have some managed workspaces, so make sure that they are
        // alphabetically sorted before generating and setting the corresponding
        // HTML code

        std::sort(workspaces.begin(), workspaces.end(), PMRSupport::PmrWorkspace::compare);

        QStringList html = QStringList();

        foreach (PMRSupport::PmrWorkspace *workspace, workspaces)
            html << workspaceHtml(workspace);

        setHtml(mTemplate.arg(html.join("")));
    }
*/
}

//==============================================================================

/*---GRY---
QWebElement PmrWorkspacesWindowWidget::parentWorkspaceElement(const QWebElement &pRowElement)
{
//---GRY--- To be reviewed...
    QWebElement workspaceElement = pRowElement;

    // Find parent workspace

    while (   !workspaceElement.isNull()
           && !(   !workspaceElement.tagName().compare(TrTag)
                &&  workspaceElement.hasClass("workspace"))) {
        workspaceElement = workspaceElement.parent();

        if (workspaceElement.hasClass("contents"))
            workspaceElement = workspaceElement.previousSibling();
    }

    return workspaceElement;
}
*/

//==============================================================================

void PmrWorkspacesWindowWidget::showInGraphicalShell(const QString &pPath)
{
//---GRY--- To be reviewed...
    QDesktopServices::openUrl(QUrl::fromLocalFile(pPath));
}

//==============================================================================

void PmrWorkspacesWindowWidget::initialize(const PMRSupport::PmrWorkspaces &pWorkspaces,
                                           const QString &pErrorMessage,
                                           const bool &pAuthenticated)
{
    // Initialise / keep track of some properties

    mWorkspaceManager->clearWorkspaces();

    mErrorMessage = pErrorMessage;
    mAuthenticated = pAuthenticated;

    if (pErrorMessage.isEmpty() && pAuthenticated) {
        // Reconcile URLs of my-workspaces (on PMR) with those from workspace
        // folders (in doing so, folders/URLs that don't correspond to an actual
        // PMR workspace are pruned from the relevant maps)

        // First, clear the owned flag from the list of URLs with workspace
        // folders

        QMutableMapIterator<QString, QPair<QString, bool>> urlsIterator(mWorkspaceUrlFolderMines);

        while (urlsIterator.hasNext()) {
            urlsIterator.next();

            urlsIterator.setValue(QPair<QString, bool>(urlsIterator.value().first, false));
        }

        foreach (PMRSupport::PmrWorkspace *workspace, pWorkspaces) {
            // Remember our workspace, so we can find it by URL

            QString url = workspace->url();

            mWorkspaceManager->addWorkspace(workspace);

            // Check if we know its folder and flag it as ours

            if (mWorkspaceUrlFolderMines.contains(url)) {
                QString path = mWorkspaceUrlFolderMines.value(url).first;

                mWorkspaceUrlFolderMines.insert(url, QPair<QString, bool>(path, true));

                workspace->setPath(path);
                workspace->open();
            }
        }

        // Now check the workspace folders that aren't from my-workspaces (on
        // PMR)

        urlsIterator.toFront();

        while (urlsIterator.hasNext()) {
            urlsIterator.next();

            if (!urlsIterator.value().second) {
                QString url = urlsIterator.key();
                PMRSupport::PmrWorkspace *workspace = mPmrWebService->workspace(url);

                if (workspace) {
                    // The workspace is known, so ask our workspace manager to
                    // track it, and then open it

                    mWorkspaceManager->addWorkspace(workspace);

                    workspace->setPath(urlsIterator.value().first);
                    workspace->open();
                } else {
                    // The workspace is not known, so forget about it

                    mWorkspaceFolderUrls.remove(urlsIterator.value().first);

                    urlsIterator.remove();
                }
            }
        }
    }

    // Display our list of workspaces

/*---GRY---
    displayWorkspaces();
*/

    resizeTreeViewToContents();

    updateMessage();

    mInitialized = true;
}

//==============================================================================

bool PmrWorkspacesWindowWidget::hasWorkspaces() const
{
    // Return whether we have some workspaces

    return !mWorkspaceManager->workspaces().isEmpty();
}

//==============================================================================

void PmrWorkspacesWindowWidget::aboutWorkspace(const QString &pUrl)
{
Q_UNUSED(pUrl);
/*---GRY---
    QWebElement workspaceElement = page()->mainFrame()->documentElement().findFirst(QString("tr.workspace[id=\"%1\"]").arg(pUrl));

    if (!workspaceElement.isNull()) {
        PMRSupport::PmrWorkspace *workspace = mWorkspaceManager->workspace(workspaceElement.attribute("id"));

        if (workspace) {
            QStringList workspaceInformation = QStringList() << workspace->name();

            if (!workspace->description().isEmpty())
                workspaceInformation << workspace->description();

            if (!workspace->owner().isEmpty())
                workspaceInformation << tr("Owner: %1").arg(workspace->owner());

            workspaceInformation << tr("PMR: <a href=\"%1\">%1</a>").arg(workspace->url());

            if (workspace->isLocal())
                workspaceInformation << tr("Path: <a href=\"file://%1/\">%1</a>").arg(workspace->path());

            emit information(workspaceInformation.join("<br></br><br></br>"));
        }
    }
*/
}

//==============================================================================

void PmrWorkspacesWindowWidget::cloneWorkspace(const QString &pUrl)
{
//---GRY--- To be reviewed...
    PMRSupport::PmrWorkspace *workspace = mWorkspaceManager->workspace(pUrl);

    if (workspace && !workspace->isLocal()) {
        QString dirName = PMRSupport::PmrWebService::getEmptyDirectory();

        if (!dirName.isEmpty()) {
            // Create the folder for the new workspace

            QDir workspaceFolder = QDir(dirName);

            if (!workspaceFolder.exists())
                workspaceFolder.mkpath(".");

            mPmrWebService->requestWorkspaceClone(workspace, dirName);
        }
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::commitWorkspace(const QString &pUrl)
{
//---GRY--- To be reviewed...
    PMRSupport::PmrWorkspace *workspace = mWorkspaceManager->workspace(pUrl);

    if (workspace && workspace->isLocal()) {
        if (workspace->isMerging()) {
            workspace->commitMerge();
        } else {
            PmrWorkspacesWindowCommitDialog commitDialog(workspace->stagedFiles(),
                                                         Core::mainWindow());

            if (commitDialog.exec() == QDialog::Accepted)
                workspace->commit(commitDialog.message());
        }

        refreshWorkspace(workspace->url());
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::refreshWorkspace(const QString &pUrl)
{
Q_UNUSED(pUrl);
/*---GRY---
    QWebElement workspaceElement = page()->mainFrame()->documentElement().findFirst(QString("tr.workspace[id=\"%1\"]").arg(pUrl));

    if (!workspaceElement.isNull()) {
        PMRSupport::PmrWorkspace *workspace = mWorkspaceManager->workspace(workspaceElement.attribute("id"));

        if (workspace) {
            // We have a valid workspace so refresh its status

            workspace->refreshStatus();

            // And replace the header and content rows

            QStringList htmlRows = workspaceHtml(workspace);

            // Using workspaceElement.nextSibling() directly doesn't update Xml

            QWebElement contentsElement = workspaceElement.nextSibling();

            workspaceElement.setOuterXml(htmlRows[0]);
            contentsElement.setOuterXml(htmlRows[1]);
        }
    }
*/
}

//==============================================================================

void PmrWorkspacesWindowWidget::refreshWorkspaceFile(const QString &pPath)
{
Q_UNUSED(pPath);
/*---GRY---
    QWebElement fileElement = page()->mainFrame()->documentElement().findFirst(QString("tr.file[id=\"%1\"]").arg(pPath));
    QWebElement workspaceElement = parentWorkspaceElement(fileElement);

    if (!workspaceElement.isNull()) {
        PMRSupport::PmrWorkspace *workspace = mWorkspaceManager->workspace(workspaceElement.attribute("id"));

        if (workspace) {
            // We have a valid workspace so update file's status

            QStringList statusActionHtml = fileStatusActionHtml(workspace, pPath);
            QWebElement statusElement = fileElement.findFirst("td.status");

            if (!statusElement.isNull()) {
                if (statusActionHtml[0].isEmpty())
                    statusElement.addClass("hidden");
                else
                    statusElement.removeClass("hidden");

                statusElement.setInnerXml(statusActionHtml[0]);
            }

            QWebElement actionElement = fileElement.findFirst("td.actions");

            if (!actionElement.isNull()) {
                if (statusActionHtml[1].isEmpty())
                    actionElement.addClass("hidden");
                else
                    actionElement.removeClass("hidden");

                actionElement.setInnerXml(statusActionHtml[1]);
            }
        }
    }
*/
}

//==============================================================================

void PmrWorkspacesWindowWidget::requestWorkspaces()
{
    // Get the list of workspaces from our PMR web service, after making sure
    // that we have cleared existing workspaces
    // Note: initialize() will be called when the list of workspaces has been
    //       received...

    mWorkspaceManager->clearWorkspaces();

    mPmrWebService->requestWorkspaces();
}

//==============================================================================

void PmrWorkspacesWindowWidget::synchronizeWorkspace(const QString &pUrl,
                                                     const bool &pPush)
{
//---GRY--- To be reviewed...
    PMRSupport::PmrWorkspace *workspace = mWorkspaceManager->workspace(pUrl);

    if (workspace && workspace->isLocal())
        mPmrWebService->requestWorkspaceSynchronize(workspace, pPush);
}

//==============================================================================

void PmrWorkspacesWindowWidget::workspaceCloned(PMRSupport::PmrWorkspace *pWorkspace)
{
//---GRY--- To be reviewed...
    if (pWorkspace) {
        QString url = pWorkspace->url();

        // Ensure our widget knows about the workspace

        if (!mWorkspaceUrlFolderMines.contains(url))
            addWorkspace(pWorkspace);

        // Close display of current workspace and set the cloned one current

        setCurrentWorkspaceUrl(url);

        // Redisplay with workspace expanded

        pWorkspace->open();
        displayWorkspaces();
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::workspaceSynchronized(PMRSupport::PmrWorkspace *pWorkspace)
{
//---GRY--- To be reviewed...
    if (pWorkspace)
        refreshWorkspace(pWorkspace->url());
}

//==============================================================================

void PmrWorkspacesWindowWidget::viewInPmr()
{
    // Show the current item in PMR

    QDesktopServices::openUrl(currentItem()->url());
}

//==============================================================================

void PmrWorkspacesWindowWidget::copyUrl()
{
    // Copy the current item's URL to the clipboard

    QApplication::clipboard()->setText(currentItem()->url());
}

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
