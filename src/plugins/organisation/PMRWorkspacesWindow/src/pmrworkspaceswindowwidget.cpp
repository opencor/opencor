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
#include <QFileIconProvider>
#include <QKeyEvent>
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

void PmrWorkspacesWindowItem::constructor(const Type &pType, const QIcon &pIcon,
                                          const QString &pText,
                                          PMRSupport::PmrWorkspace *pWorkspace,
                                          const QString &pFileName)
{
    // Some initialisations

    mType = pType;
    mWorkspace = pWorkspace;
    mFileName = pFileName;

    // Customise ourselves

    QStandardItem::setIcon(pIcon);

    setToolTip(pText);
}

//==============================================================================

PmrWorkspacesWindowItem::PmrWorkspacesWindowItem(const Type &pType,
                                                 const QIcon &pIcon,
                                                 const QString &pText,
                                                 PMRSupport::PmrWorkspace *pWorkspace) :
    QStandardItem(pText)
{
    // Construct our object

    constructor(pType, pIcon, pText, pWorkspace, QString());
}

//==============================================================================

PmrWorkspacesWindowItem::PmrWorkspacesWindowItem(const Type &pType,
                                                 const QIcon &pIcon,
                                                 const QString &pText,
                                                 const QString &pFileName) :
    QStandardItem(pText)
{
    // Construct our object

    constructor(pType, pIcon, pText, 0, pFileName);
}

//==============================================================================

int PmrWorkspacesWindowItem::type() const
{
    // Return our type

    return mType;
}

//==============================================================================

PMRSupport::PmrWorkspace * PmrWorkspacesWindowItem::workspace() const
{
    // Return our workspace

    return mWorkspace;
}

//==============================================================================

QString PmrWorkspacesWindowItem::url() const
{
    // Return our URL

    return mWorkspace?mWorkspace->url():QString();
}

//==============================================================================

QString PmrWorkspacesWindowItem::fileName() const
{
    // Return our file name

    return mFileName;
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

PmrWorkspacesWindowWidget::PmrWorkspacesWindowWidget(PMRSupport::PmrWebService *pPmrWebService,
                                                     QWidget *pParent) :
    Core::Widget(pParent),
    mPmrWebService(pPmrWebService),
    mWorkspaceManager(PMRSupport::PmrWorkspaceManager::instance()),
    mWorkspaceFolderUrls(QMap<QString, QString>()),
    mOwnedWorkspaceUrlFolders(QMap<QString, QPair<QString, bool>>()),
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
    mTreeViewWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mTreeViewWidget->setVisible(false);

    connect(mTreeViewWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu(const QPoint &)));

    connect(mTreeViewWidget, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(itemDoubleClicked(const QModelIndex &)));

    connect(mTreeViewWidget, SIGNAL(expanded(const QModelIndex &)),
            this, SLOT(resizeTreeViewToContents()));
    connect(mTreeViewWidget, SIGNAL(expanded(const QModelIndex &)),
            this, SLOT(itemExpanded(const QModelIndex &)));

    connect(mTreeViewWidget, SIGNAL(collapsed(const QModelIndex &)),
            this, SLOT(resizeTreeViewToContents()));
    connect(mTreeViewWidget, SIGNAL(collapsed(const QModelIndex &)),
            this, SLOT(itemCollapsed(const QModelIndex &)));

    // Create our owned workspace icons

    static const QIcon FavoriteIcon = QIcon(":/oxygen/places/favorites.png");

    QIcon collapsedFolderIcon = QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon);
    QIcon expandedFolderIcon = QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon);
    int folderIconSize = collapsedFolderIcon.availableSizes().first().width();
    int favoriteIconSize = 0.57*folderIconSize;

    mCollapsedOwnedWorkspaceIcon = Core::overlayedIcon(collapsedFolderIcon,
                                                      FavoriteIcon,
                                                      folderIconSize, folderIconSize,
                                                      folderIconSize-favoriteIconSize, folderIconSize-favoriteIconSize,
                                                      favoriteIconSize, favoriteIconSize);
    mExpandedOwnedWorkspaceIcon = Core::overlayedIcon(expandedFolderIcon,
                                                     FavoriteIcon,
                                                     folderIconSize, folderIconSize,
                                                     folderIconSize-favoriteIconSize, folderIconSize-favoriteIconSize,
                                                     favoriteIconSize, favoriteIconSize);

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
                this, SLOT(startStopTimer()));
    }

    // Create and populate our context menu

    mContextMenu = new QMenu(this);

    mViewInPmrAction = Core::newAction(this);
    mCopyUrlAction = Core::newAction(this);
    mCloneWorkspaceAction = Core::newAction(this);

    connect(mViewInPmrAction, SIGNAL(triggered(bool)),
            this, SLOT(viewInPmr()));
    connect(mCopyUrlAction, SIGNAL(triggered(bool)),
            this, SLOT(copyUrl()));
    connect(mCloneWorkspaceAction, SIGNAL(triggered(bool)),
            this, SLOT(cloneWorkspace()));

    mContextMenu->addAction(mViewInPmrAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mCopyUrlAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mCloneWorkspaceAction);

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
    I18nInterface::retranslateAction(mCloneWorkspaceAction, tr("Clone Workspace..."),
                                     tr("Clone the current workspace"));

    // Retranslate the rest of our GUI by updating it, if we have been
    // initialised

    if (mInitialized)
        updateGui();
}

//==============================================================================

static const auto SettingsClonedWorkspaceFolders = QStringLiteral("ClonedWorkspaceFolders");

//==============================================================================

void PmrWorkspacesWindowWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the names of folders containing a cloned workspace, and add them

    foreach (const QString &cloneWorkspaceFolder,
             pSettings->value(SettingsClonedWorkspaceFolders).toStringList()) {
        addWorkspaceFolder(cloneWorkspaceFolder);
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the names of folders containing cloned workspaces

    pSettings->setValue(SettingsClonedWorkspaceFolders, QVariant(mWorkspaceFolderUrls.keys()));
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

void PmrWorkspacesWindowWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Default handling of the event

    Core::Widget::keyPressEvent(pEvent);

    // Retrieve all the files that are currently selected
    // Note: if there is folder among the selected items, then ignore
    //       everything...

    QStringList fileNames = QStringList();
    QModelIndexList selectedIndexes = mTreeViewWidget->selectedIndexes();

    for (int i = 0, iMax = selectedIndexes.count(); i < iMax; ++i) {
        PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(selectedIndexes[i]));

        if (item->type() != PmrWorkspacesWindowItem::File) {
            fileNames = QStringList();

            break;
        } else {
            fileNames << item->fileName();
        }
    }

    // Let people know about a key having been pressed with the view of opening
    // one or several files

    if (   fileNames.count()
        && ((pEvent->key() == Qt::Key_Enter) || (pEvent->key() == Qt::Key_Return))) {
        // There are some files that are selected and we want to open them, so
        // let people know about it

        emit openFilesRequested(fileNames);
    }
}

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

void PmrWorkspacesWindowWidget::updateGui()
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

    // Show/hide our user message widget and our tree view widget, and then
    // check whether we should start/stop our timer

    mUserMessageWidget->setVisible(!mUserMessageWidget->text().isEmpty());
    mTreeViewWidget->setVisible(mUserMessageWidget->text().isEmpty());

    startStopTimer();
}

//==============================================================================

void PmrWorkspacesWindowWidget::initialize(const PMRSupport::PmrWorkspaces &pWorkspaces,
                                           const QString &pErrorMessage,
                                           const bool &pAuthenticated)
{
    // Initialise / keep track of some properties

    mWorkspaceManager->clearWorkspaces();

    mTreeViewModel->clear();

    mErrorMessage = pErrorMessage;
    mAuthenticated = pAuthenticated;

    if (pErrorMessage.isEmpty() && pAuthenticated) {
        // Reconcile the URLs of my-workspaces (on PMR) with those from our
        // workspace folders (in doing so, folders/URLs that don't correspond to
        // an actual PMR workspace are pruned from the relevant maps)

        // First, clear the owned flag from the list of URLs with workspace
        // folders

        QMutableMapIterator<QString, QPair<QString, bool>> urlsIterator(mOwnedWorkspaceUrlFolders);

        while (urlsIterator.hasNext()) {
            urlsIterator.next();

            urlsIterator.setValue(QPair<QString, bool>(urlsIterator.value().first, false));
        }

        foreach (PMRSupport::PmrWorkspace *workspace, pWorkspaces) {
            // Remember our workspace, so we can find it by URL

            QString url = workspace->url();

            mWorkspaceManager->addWorkspace(workspace);

            // Check if we know its folder and flag it as ours

            if (mOwnedWorkspaceUrlFolders.contains(url)) {
                QString path = mOwnedWorkspaceUrlFolders.value(url).first;

                mOwnedWorkspaceUrlFolders.insert(url, QPair<QString, bool>(path, true));

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

    // Populate our tree view widget with our different workspaces

    foreach (PMRSupport::PmrWorkspace *workspace, mWorkspaceManager->workspaces())
        addWorkspace(workspace);

    updateGui();

    mInitialized = true;
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

void PmrWorkspacesWindowWidget::addWorkspace(PMRSupport::PmrWorkspace *pWorkspace)
{
    // Determine where in our tree view widget the given workspace should be
    // inserted

    QString workspaceName = pWorkspace->name();
    int row = -1;

    for (int i = 0, iMax = mTreeViewModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
        if (workspaceName.compare(mTreeViewModel->invisibleRootItem()->child(i)->text()) < 0) {
            row = i;

            break;
        }
    }

    // Add the given workspace to our tree view widget

    PmrWorkspacesWindowItem *workspaceItem = new PmrWorkspacesWindowItem(pWorkspace->isOwned()?
                                                                             PmrWorkspacesWindowItem::OwnedWorkspace:
                                                                             PmrWorkspacesWindowItem::Workspace,
                                                                         pWorkspace->isOwned()?
                                                                             mCollapsedOwnedWorkspaceIcon:
                                                                             QFileIconProvider().icon(QFileIconProvider::Folder),
                                                                         workspaceName,
                                                                         pWorkspace);

    if (row == -1)
        mTreeViewModel->invisibleRootItem()->appendRow(workspaceItem);
    else
        mTreeViewModel->invisibleRootItem()->insertRow(row, workspaceItem);

    if (pWorkspace->rootFileNode())
        populateWorkspace(workspaceItem, pWorkspace->rootFileNode());

    resizeTreeViewToContents();
}

//==============================================================================

void PmrWorkspacesWindowWidget::populateWorkspace(PMRSupport::PmrWorkspace *pWorkspace)
{
    // Retrieve the folder item for the given workspace and populate it

    for (int i = 0, iMax = mTreeViewModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
        PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->invisibleRootItem()->child(i));

        if (item->workspace() == pWorkspace) {
            populateWorkspace(item, pWorkspace->rootFileNode());

            break;
        }
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::populateWorkspace(PmrWorkspacesWindowItem *pFolderItem,
                                                  PMRSupport::PmrWorkspaceFileNode *pFileNode)
{
    // Populate the given folder item with its children, which are referenced in
    // the given file node

    foreach(PMRSupport::PmrWorkspaceFileNode *fileNode, pFileNode->children()) {
        if (fileNode->hasChildren()) {
            PmrWorkspacesWindowItem *folderItem = new PmrWorkspacesWindowItem(PmrWorkspacesWindowItem::Folder,
                                                                              QFileIconProvider().icon(QFileIconProvider::Folder),
                                                                              fileNode->name());

            pFolderItem->appendRow(folderItem);

            if (fileNode->hasChildren())
                populateWorkspace(folderItem, fileNode);
        } else {
            pFolderItem->appendRow(new PmrWorkspacesWindowItem(PmrWorkspacesWindowItem::File,
                                                               QFileIconProvider().icon(QFileIconProvider::File),
                                                               fileNode->name(),
                                                               fileNode->fullName()));
        }
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::showCustomContextMenu(const QPoint &pPosition) const
{
    // Determine whether to show the context menu based on whether we are over
    // an item

    PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(mTreeViewWidget->indexAt(pPosition)));

    if (item) {
        // We are over an item, so update our context menu and show it

        bool workspaceItem =    (item->type() == PmrWorkspacesWindowItem::OwnedWorkspace)
                             || (item->type() == PmrWorkspacesWindowItem::Workspace);

        mViewInPmrAction->setVisible(workspaceItem);
        mCopyUrlAction->setVisible(workspaceItem);
        mCloneWorkspaceAction->setVisible(item->type() == PmrWorkspacesWindowItem::OwnedWorkspace);

        mCopyUrlAction->setEnabled(mTreeViewWidget->selectedIndexes().count() == 1);
        mCloneWorkspaceAction->setEnabled(mOwnedWorkspaceUrlFolders.value(item->url()).first.isEmpty());

        mContextMenu->exec(QCursor::pos());
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::itemDoubleClicked(const QModelIndex &pIndex)
{
    // Ask for a file to be opened

    PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(pIndex));

    if (item->type() == PmrWorkspacesWindowItem::File)
        emit openFileRequested(item->fileName());
}

//==============================================================================

void PmrWorkspacesWindowWidget::itemExpanded(const QModelIndex &pIndex)
{
    // Update the icon of the item, if its type is that an owned workspace

    PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(pIndex));

    if (item->type() == PmrWorkspacesWindowItem::OwnedWorkspace)
        item->setIcon(mExpandedOwnedWorkspaceIcon);
}

//==============================================================================

void PmrWorkspacesWindowWidget::itemCollapsed(const QModelIndex &pIndex)
{
    // Update the icon of the item, if its type is that an owned workspace

    PmrWorkspacesWindowItem *item = static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(pIndex));

    if (item->type() == PmrWorkspacesWindowItem::OwnedWorkspace)
        item->setIcon(mCollapsedOwnedWorkspaceIcon);
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

QString PmrWorkspacesWindowWidget::addWorkspaceFolder(const QString &pWorkspaceFolder)
{
    // Add the given workspace folder, if it has not already been added

    if (!mWorkspaceFolderUrls.contains(pWorkspaceFolder)) {
        // Retrieve the workspace URL (i.e. remote.origin.url) for the given
        // folder

        QString res = QString();
        QByteArray folderByteArray = pWorkspaceFolder.toUtf8();
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
            if (mOwnedWorkspaceUrlFolders.contains(res)) {
                duplicateCloneMessage(res, mOwnedWorkspaceUrlFolders.value(res).first, pWorkspaceFolder);
            } else {
                mWorkspaceFolderUrls.insert(pWorkspaceFolder, res);
                mOwnedWorkspaceUrlFolders.insert(res, QPair<QString, bool>(pWorkspaceFolder, false));
            }
        }

        return res;
    } else {
        return mWorkspaceFolderUrls.value(pWorkspaceFolder);
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::startStopTimer()
{
    // Start our timer if OpenCOR is active and we have at least one workspace,
    // or stop it if either OpenCOR is not active or we no longer have at least
    // one workspace
    // Note: if we are to start our timer, then we refresh the workspace first
    //       since waiting one second may seem long to a user...

    if (   !mTimer->isActive()
        &&  Core::opencorActive() && mTreeViewModel->rowCount()) {
        disconnect(qApp, SIGNAL(focusWindowChanged(QWindow *)),
                   this, SLOT(startStopTimer()));

        refreshWorkspaces();

        connect(qApp, SIGNAL(focusWindowChanged(QWindow *)),
                this, SLOT(startStopTimer()));

        mTimer->start(1000);
    } else if (   mTimer->isActive()
               && (!Core::opencorActive() || !mTreeViewModel->rowCount())) {
        mTimer->stop();
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::refreshWorkspaces()
{
    // Refresh our workspaces

    foreach (PMRSupport::PmrWorkspace *workspace, mWorkspaceManager->workspaces())
        refreshWorkspace(workspace);
}

//==============================================================================

void PmrWorkspacesWindowWidget::showInGraphicalShell(const QString &pPath)
{
//---GRY--- To be reviewed...
    QDesktopServices::openUrl(QUrl::fromLocalFile(pPath));
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

        refreshWorkspace(workspace);
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::refreshWorkspace(PMRSupport::PmrWorkspace *pWorkspace)
{
    // Refresh the status of the given workspace

    pWorkspace->refreshStatus();

//---GRY--- Somehow need to update our tree view, if needed...
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
    // Add the given cloned workspace to ourselves, if possible

    if (pWorkspace) {
        QString url = pWorkspace->url();

        // Check whether we already know about the workspace

        if (!mOwnedWorkspaceUrlFolders.contains(url)) {
            // This is a cloned workspace that we don't own, so keep track of
            // it, open it and then add it to our GUI

            QString folder = pWorkspace->path();

            mWorkspaceFolderUrls.insert(folder, url);
            mOwnedWorkspaceUrlFolders.insert(url, QPair<QString, bool>(folder, false));

            mWorkspaceManager->addWorkspace(pWorkspace);

            pWorkspace->open();

            addWorkspace(pWorkspace);
        } else {
            // We already about that workspace, so check whether it's because we
            // have just cloned a workspace that we own

            QPair<QString, bool> folderOwned = mOwnedWorkspaceUrlFolders.value(url);

            if (folderOwned.second) {
                // We have just cloned a workspace that we own, so simply
                // populate it

                populateWorkspace(pWorkspace);
            } else {
                // This is not a workspace that we own, meaning that we have
                // already cloned a workspace with the same URL, so let the user
                // know about it

                duplicateCloneMessage(url, folderOwned.first, pWorkspace->path());
            }
        }
    }
}

//==============================================================================

void PmrWorkspacesWindowWidget::workspaceSynchronized(PMRSupport::PmrWorkspace *pWorkspace)
{
//---GRY--- To be reviewed...
    if (pWorkspace)
        refreshWorkspace(pWorkspace);
}

//==============================================================================

void PmrWorkspacesWindowWidget::viewInPmr()
{
    // Show the selected items in PMR

    QModelIndexList selectedIndexes = mTreeViewWidget->selectedIndexes();

    for (int i = 0, iMax = selectedIndexes.count(); i < iMax; ++i)
        QDesktopServices::openUrl(static_cast<PmrWorkspacesWindowItem *>(mTreeViewModel->itemFromIndex(selectedIndexes[i]))->url());
}

//==============================================================================

void PmrWorkspacesWindowWidget::copyUrl()
{
    // Copy the current item's URL to the clipboard

    QApplication::clipboard()->setText(currentItem()->url());
}

//==============================================================================

void PmrWorkspacesWindowWidget::cloneWorkspace()
{
    // Let people know that we want to clone the current (owned) workspace

    emit cloneOwnedWorkspaceRequested(mWorkspaceManager->workspace(currentItem()->url()));
}

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
