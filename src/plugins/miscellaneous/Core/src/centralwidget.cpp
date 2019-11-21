/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Central widget
//==============================================================================

#include "centralwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "filehandlinginterface.h"
#include "filemanager.h"
#include "guiinterface.h"
#include "interfaces.h"
#include "remotefiledialog.h"
#include "tabbarwidget.h"
#include "usermessagewidget.h"
#include "viewinterface.h"
#include "viewwidget.h"

//==============================================================================

#include <QAction>
#include <QApplication>
#include <QDialogButtonBox>
#include <QDragEnterEvent>
#include <QFile>
#include <QFileInfo>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMimeData>
#include <QPushButton>
#include <QRect>
#include <QScreen>
#include <QSizePolicy>
#include <QStackedWidget>
#include <QStatusBar>
#include <QUrl>
#include <QVariant>
#include <QVBoxLayout>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

CentralWidgetMode::CentralWidgetMode(CentralWidget *pOwner)
{
    // Initialise a few internal objects

    mViewTabs = pOwner->newTabBarWidget(QTabBar::RoundedEast);
}

//==============================================================================

CentralWidgetMode::~CentralWidgetMode()
{
    // Delete some internal objects

    delete mViewTabs;
}

//==============================================================================

bool CentralWidgetMode::isEnabled() const
{
    // Return whether the mode is enabled

    return mEnabled;
}

//==============================================================================

void CentralWidgetMode::setEnabled(bool pEnabled)
{
    // Set whether a mode is enabled

    mEnabled = pEnabled;
}

//==============================================================================

TabBarWidget * CentralWidgetMode::viewTabs() const
{
    // Return our view tabs

    return mViewTabs;
}

//==============================================================================

Plugins CentralWidgetMode::viewPlugins() const
{
    // Return our view plugins

    return mViewPlugins;
}

//==============================================================================

void CentralWidgetMode::addViewPlugin(Plugin *pViewPlugin)
{
    // Add the given view plugin to our list

    mViewPlugins << pViewPlugin;
}

//==============================================================================

CentralWidget::CentralWidget(QWidget *pParent) :
    Widget(pParent)
{
    // Create and set our horizontal layout

    auto layout = new QHBoxLayout(this);

    layout->setContentsMargins(QMargins());
    layout->setSpacing(0);

    setLayout(layout);

    // Allow for things to be dropped on us

    setAcceptDrops(true);

    // Create our modes tab bar with no tabs by default

    mModeTabs = newTabBarWidget(QTabBar::RoundedWest);

    // Create our modes

    mModes.insert(ViewInterface::Mode::Editing, new CentralWidgetMode(this));
    mModes.insert(ViewInterface::Mode::Simulation, new CentralWidgetMode(this));
#ifdef ENABLE_SAMPLE_PLUGINS
    mModes.insert(ViewInterface::Mode::Sample, new CentralWidgetMode(this));
#endif
#ifdef ENABLE_TEST_PLUGINS
    mModes.insert(ViewInterface::Mode::Test, new CentralWidgetMode(this));
#endif
    // Note: these will be deleted in CentralWidget's destructor...

    // Create our files tab bar widget

    mFileTabs = newTabBarWidget(QTabBar::RoundedNorth, true);

    // Create our contents widget

    mContents = new QStackedWidget(this);

    mContents->setFrameShape(QFrame::StyledPanel);

    // Create our logo view which simply displays OpenCOR's logo

    mLogoView = new QWidget(this);

    mLogoView->setStyleSheet("QWidget {"
                             "    background-color: white;"
                             "    background-image: url(:/Core/logo.png);"
                             "    background-position: center;"
                             "    background-repeat: no-repeat;"
                             "}");

    mContents->addWidget(mLogoView);

    // Create our no view message widget which will display a customised message
    // to let the user know that there view doesn't support the type of the
    // current file

    mNoViewMsg = new UserMessageWidget(":/oxygen/actions/help-about.png", this);

    mNoViewMsg->hide();

    // Create and set up our central widget

    auto centralWidget = new QWidget(this);
    auto centralWidgetVBoxLayout = new QVBoxLayout(centralWidget);

    centralWidgetVBoxLayout->setContentsMargins(QMargins());
    centralWidgetVBoxLayout->setSpacing(0);

    centralWidget->setLayout(centralWidgetVBoxLayout);

    centralWidgetVBoxLayout->addWidget(mFileTabs);
    centralWidgetVBoxLayout->addWidget(mContents);

    // Add the widgets to our layout

    layout->addWidget(mModeTabs);
    layout->addWidget(centralWidget);

    for (auto mode : mModes) {
        layout->addWidget(mode->viewTabs());
    }

    // A connection to handle the case where a file was created or duplicated

    FileManager *fileManagerInstance = FileManager::instance();

    connect(fileManagerInstance, &FileManager::fileCreated,
            this, &CentralWidget::fileCreated);
    connect(fileManagerInstance, &FileManager::fileDuplicated,
            this, &CentralWidget::fileDuplicated);

    // Some connections to handle changes to a file

    connect(fileManagerInstance, &FileManager::filePermissionsChanged,
            this, &CentralWidget::filePermissionsChanged);
    connect(fileManagerInstance, &FileManager::fileModified,
            this, &CentralWidget::fileModified);

    // Some connections to handle our files tab bar

    connect(mFileTabs, &TabBarWidget::currentChanged,
            this, &CentralWidget::updateGui);
    connect(mFileTabs, &TabBarWidget::tabMoved,
            this, &CentralWidget::moveFile);
    connect(mFileTabs, &TabBarWidget::tabCloseRequested,
            this, QOverload<int>::of(&CentralWidget::closeFile));

    // A connection to handle our modes tab bar

    connect(mModeTabs, &TabBarWidget::currentChanged,
            this, &CentralWidget::updateGui);
    connect(mModeTabs, &TabBarWidget::currentChanged,
            this, &CentralWidget::updateFileTabIcons);

    // Some connections to handle our mode views tab bar

    for (auto mode : mModes) {
        connect(mode->viewTabs(), &TabBarWidget::currentChanged,
                this, &CentralWidget::updateGui);
        connect(mode->viewTabs(), &TabBarWidget::currentChanged,
                this, &CentralWidget::updateFileTabIcons);
    }
}

//==============================================================================

CentralWidget::~CentralWidget()
{
    // We are shutting down, so we must let updateGui() know about it otherwise
    // we may get a segmentation fault (should there be a need to switch from
    // one view to another)

    mState = State::Stopping;

    // Close all the files
    // Note: we force the closing of all the files since canClose() will have
    //       been called before to decide what needs to be done with modified
    //       files...

    closeAllFiles(true);

    // Delete our various modes

    for (auto mode : mModes) {
        delete mode;
    }
}

//==============================================================================

static const char *SettingsFileNamesOrUrls      = "FileNamesOrUrls";
static const char *SettingsCurrentFileNameOrUrl = "CurrentFileNameOrUrl";
static const char *SettingsFileMode             = "FileMode%1";
static const char *SettingsFileModeView         = "FileModeView%1%2";

//==============================================================================

void CentralWidget::loadSettings(QSettings &pSettings)
{
    // Some connections to handle an external change in the state of a file
    // Note: we do it here because we want other plugins to get a chance to
    //       handle our file manager's signals before us. Indeed, in the case of
    //       the CellML file manager for example, we will want the CellML file
    //       manager to reload a CellML file before our different views get
    //       asked (by us, the central widget) to reload the file (i.e. update
    //       their GUIs)...

    FileManager *fileManagerInstance = FileManager::instance();

    connect(fileManagerInstance, &FileManager::fileChanged,
            this, &CentralWidget::fileChanged);
    connect(fileManagerInstance, &FileManager::fileDeleted,
            this, &CentralWidget::fileDeleted);

    // Some connections to handle an internal change in the state of a file
    // Note: we do it here for the same reason as above (see the note for the
    //       connections to handle an external change in the state of a file)...

    connect(fileManagerInstance, &FileManager::fileModified,
            this, &CentralWidget::updateModifiedSettings);
    connect(fileManagerInstance, &FileManager::fileReloaded,
            this, &CentralWidget::fileReloaded);
    connect(fileManagerInstance, &FileManager::fileRenamed,
            this, &CentralWidget::fileRenamed);

    connect(fileManagerInstance, &FileManager::fileSaved,
            this, &CentralWidget::fileSaved);

    // Let the user know of a few default things about ourselves by emitting a
    // few signals

    emit canSave(false);
    emit canSaveAs(false);
    emit canSaveAll(false);

    emit atLeastOneFile(false);
    emit atLeastTwoFiles(false);

    // Retrieve and open the files that were previously opened and as if they
    // were remote files
    // Note: if a file is not a remote file then openRemoteFile() will open it
    //       as a normal file...

    QStringList fileNamesOrUrls = pSettings.value(SettingsFileNamesOrUrls).toStringList();

    for (const auto &fileNameOrUrl : fileNamesOrUrls) {
        openRemoteFile(fileNameOrUrl, false);
    }

    // Retrieve the selected modes and views of our different files

    for (const auto &fileName : mFileNames) {
        QString fileNameOrUrl = fileManagerInstance->isRemote(fileName)?
                                    fileManagerInstance->url(fileName):
                                    fileName;
        ViewInterface::Mode fileMode = ViewInterface::modeFromString(pSettings.value(QString(SettingsFileMode).arg(fileNameOrUrl)).toString());

        if (fileMode != ViewInterface::Mode::Unknown) {
            mFileModeTabIndexes.insert(fileName, mModeModeTabIndexes.value(fileMode));
        }

        QMap<int, int> modeViewTabIndexes = QMap<int, int>();

        for (int i = 0, iMax = mModeTabs->count(); i < iMax; ++i) {
            fileMode = mModeTabIndexModes.value(i);

            QString viewPluginName = pSettings.value(QString(SettingsFileModeView).arg(fileNameOrUrl,
                                                                                       ViewInterface::modeAsString(fileMode))).toString();
            Plugins viewPlugins = mModes.value(fileMode)->viewPlugins();

            for (int j = 0, jMax = viewPlugins.count(); j < jMax; ++j) {
                if (viewPluginName == viewPlugins[j]->name()) {
                    modeViewTabIndexes.insert(i, j);

                    break;
                }
            }
        }

        mFileModeViewTabIndexes.insert(fileName, modeViewTabIndexes);
    }

    // Select the previously selected file, if it still exists, by pretending to
    // open it (which, in turn, will select it)

    QString crtFileNameOrUrl = pSettings.value(SettingsCurrentFileNameOrUrl).toString();
    bool isLocalFile;

    checkFileNameOrUrl(crtFileNameOrUrl, isLocalFile, crtFileNameOrUrl);

    QString crtFileName = isLocalFile?
                              crtFileNameOrUrl:
                              fileManagerInstance->fileName(crtFileNameOrUrl);

    if (mFileNames.contains(crtFileName)) {
        openFile(crtFileName);
    } else {
        // The previously selected file doesn't exist anymore, so select the
        // first file (otherwise the last file will be selected)

        setTabBarCurrentIndex(mFileTabs, 0);
    }

    // Retrieve the seleted modes and views, in case there are no files

    if (mFileNames.isEmpty()) {
        ViewInterface::Mode fileMode = ViewInterface::modeFromString(pSettings.value(QString(SettingsFileMode).arg(QString())).toString());

        if (fileMode != ViewInterface::Mode::Unknown) {
            setTabBarCurrentIndex(mModeTabs, mModeModeTabIndexes.value(fileMode));
        }

        for (int i = 0, iMax = mModeTabs->count(); i < iMax; ++i) {
            fileMode = mModeTabIndexModes.value(i);

            CentralWidgetMode *mode = mModes.value(fileMode);
            QString viewPluginName = pSettings.value(QString(SettingsFileModeView).arg(QString(),
                                                                                       ViewInterface::modeAsString(fileMode))).toString();
            Plugins viewPlugins = mode->viewPlugins();

            for (int j = 0, jMax = viewPlugins.count(); j < jMax; ++j) {
                if (viewPluginName == viewPlugins[j]->name()) {
                    setTabBarCurrentIndex(mode->viewTabs(), j);

                    break;
                }
            }
        }
    }
}

//==============================================================================

void CentralWidget::saveSettings(QSettings &pSettings) const
{
    // Remove all the settings related to previously opened files

    static const QString SettingsFileModeHeader = QString(SettingsFileMode).arg(QString());
    static const QString SettingsFileModeViewHeader = QString(SettingsFileModeView).arg(QString(),
                                                                                        QString());

    for (const auto &key : pSettings.allKeys()) {
        if (   key.startsWith(SettingsFileModeHeader)
            || key.startsWith(SettingsFileModeViewHeader)) {
            pSettings.remove(key);
        }
    }

    // Keep track of the files that are opened, skipping new files

    FileManager *fileManagerInstance = FileManager::instance();
    QStringList fileNames = QStringList();
    QStringList fileNamesOrUrls = QStringList();

    for (const auto &fileName : mFileNames) {
        if (!fileManagerInstance->isNew(fileName)) {
            // The file is not new, so keep track of it

            bool fileIsRemote = fileManagerInstance->isRemote(fileName);

            if (fileIsRemote) {
                fileNamesOrUrls << fileManagerInstance->url(fileName);
            } else {
                fileNamesOrUrls << fileName;
            }

            fileNames << fileName;
        }
    }

    pSettings.setValue(SettingsFileNamesOrUrls, fileNamesOrUrls);

    // Keep track of the selected modes and views of our different files

    for (const auto &fileName : fileNames) {
        QString fileNameOrUrl = fileManagerInstance->isRemote(fileName)?
                                    fileManagerInstance->url(fileName):
                                    fileName;

        pSettings.setValue(QString(SettingsFileMode).arg(fileNameOrUrl),
                           ViewInterface::modeAsString(mModeTabIndexModes.value(mFileModeTabIndexes.value(fileName))));

        QMap<int, int> modeViewTabIndexes = mFileModeViewTabIndexes.value(fileName);

        for (int i = 0, iMax = mModeTabs->count(); i < iMax; ++i) {
            ViewInterface::Mode fileMode = mModeTabIndexModes.value(i);

            pSettings.setValue(QString(SettingsFileModeView).arg(fileNameOrUrl,
                                                                 ViewInterface::modeAsString(fileMode)),
                               mModes.value(fileMode)->viewPlugins()[modeViewTabIndexes.value(i)]->name());
        }
    }

    // Keep track of the currently selected file
    // Note: we don't rely on mFileTabs->currentIndex() since it may refer to a
    //       new file, which we will have been skipped above...

    QString crtFileNameOrUrl = QString();

    if (!fileNames.isEmpty()) {
        QString fileName = mFileNames[mFileTabs->currentIndex()];

        if (fileNames.contains(fileName)) {
            crtFileNameOrUrl = fileManagerInstance->isRemote(fileName)?
                                   fileManagerInstance->url(fileName):
                                   fileName;
        }
    }

    pSettings.setValue(SettingsCurrentFileNameOrUrl, crtFileNameOrUrl);

    // Keep track of the selected modes and views, should there be no files the
    // next time we use OpenCOR

    pSettings.setValue(QString(SettingsFileMode).arg(QString()),
                       ViewInterface::modeAsString(mModeTabIndexModes.value(mModeTabs->currentIndex())));

    for (int i = 0, iMax = mModeTabs->count(); i < iMax; ++i) {
        ViewInterface::Mode fileMode = mModeTabIndexModes.value(i);
        CentralWidgetMode *mode = mModes.value(fileMode);

        pSettings.setValue(QString(SettingsFileModeView).arg(QString(),
                                                             ViewInterface::modeAsString(fileMode)),
                           mode->viewPlugins()[mode->viewTabs()->currentIndex()]->name());
    }
}

//==============================================================================

void CentralWidget::settingsLoaded(const Plugins &pLoadedPlugins)
{
    // Determine which loaded plugins support a given interface

    for (auto plugin : pLoadedPlugins) {
        if (qobject_cast<FileHandlingInterface *>(plugin->instance()) != nullptr) {
            mLoadedFileHandlingPlugins << plugin;
        }

        if (qobject_cast<FileTypeInterface *>(plugin->instance()) != nullptr) {
            mLoadedFileTypePlugins << plugin;
        }

        if (qobject_cast<GuiInterface *>(plugin->instance()) != nullptr) {
            mLoadedGuiPlugins << plugin;
        }

        if (qobject_cast<ViewInterface *>(plugin->instance()) != nullptr) {
            mLoadedViewPlugins << plugin;
        }
    }

    // Update our state now that our plugins  are fully ready

    mState = State::Idling;

    // Update the GUI

    updateGui();

    // Let our plugins know that our files have been opened
    // Note: the below is normally done as part of openFile(), but
    //       mLoadedFileHandlingPlugins is not yet set when openFile() gets
    //       called as part of OpenCOR's loading its settings, so we do it here
    //       instead...

    for (auto plugin : mLoadedFileHandlingPlugins) {
        for (const auto &fileName : mFileNames) {
            qobject_cast<FileHandlingInterface *>(plugin->instance())->fileOpened(fileName);
        }
    }
}

//==============================================================================

void CentralWidget::retranslateUi()
{
    // Retranslate our modes tab bar

    mModeTabs->setTabText(mModeModeTabIndexes.value(ViewInterface::Mode::Editing, -1),
                          tr("Editing"));
    mModeTabs->setTabText(mModeModeTabIndexes.value(ViewInterface::Mode::Simulation, -1),
                          tr("Simulation"));
#ifdef ENABLE_SAMPLE_PLUGINS
    mModeTabs->setTabText(mModeModeTabIndexes.value(ViewInterface::Mode::Sample, -1),
                          tr("Sample"));
#endif
#ifdef ENABLE_TEST_PLUGINS
    mModeTabs->setTabText(mModeModeTabIndexes.value(ViewInterface::Mode::Test, -1),
                          tr("Test"));
#endif

    // Retranslate our mode views tab bar

    for (auto mode : mModes) {
        TabBarWidget *viewTabs = mode->viewTabs();

        for (int i = 0, iMax = viewTabs->count(); i < iMax; ++i) {
            viewTabs->setTabText(i, qobject_cast<ViewInterface *>(mode->viewPlugins()[i]->instance())->viewName());
        }
    }

    // Retranslate our modified settings, if needed

    updateModifiedSettings();

    // Retranslate our no view widget message

    updateNoViewMsg();
}

//==============================================================================

QString CentralWidget::currentFileName() const
{
    // Return the current file name, if any

    int fileTabIndex = mFileTabs->currentIndex();

    if (fileTabIndex != -1) {
        return mFileNames[fileTabIndex];
    }

    return {};
}

//==============================================================================

void CentralWidget::updateFileTab(int pIndex, bool pIconOnly)
{
    // Update the text, tool tip and icon to be used for the given file tab

    static const QIcon NoIcon       = QIcon();
    static const QIcon InternetIcon = QIcon(":/oxygen/categories/applications-internet.png");
    static const QIcon LockedIcon   = QIcon(":/oxygen/status/object-locked.png");

    FileManager *fileManagerInstance = FileManager::instance();
    QString fileName = mFileNames[pIndex];
    bool fileIsRemote = fileManagerInstance->isRemote(fileName);
    QIcon tabIcon = QIcon();

    if (!pIconOnly) {
        bool fileIsNew = fileManagerInstance->isNew(fileName);
        QString url = fileManagerInstance->url(fileName);
        QString tabText = fileIsNew?
                              tr("File")+" #"+QString::number(fileManagerInstance->newIndex(fileName)):
                              fileIsRemote?
                                  QUrl(url).fileName():
                                  QFileInfo(fileName).fileName();
        QString tabToolTip = fileIsNew?
                                 tabText:
                                 fileIsRemote?
                                     url:
                                     QDir::toNativeSeparators(fileName);

        if (fileManagerInstance->isNewOrModified(fileName)) {
            tabText += "*";
        }

        if (tabText != mFileTabs->tabText(pIndex)) {
            mFileTabs->setTabText(pIndex, tabText);
        }

        if (tabToolTip != mFileTabs->tabToolTip(pIndex)) {
            mFileTabs->setTabToolTip(pIndex, tabToolTip);
        }

        tabIcon = qobject_cast<ViewInterface *>(viewPlugin(pIndex)->instance())->fileTabIcon(mFileNames[pIndex]);
    }

    if (tabIcon.isNull()) {
        tabIcon = fileIsRemote?
                      InternetIcon:
                      QFile::exists(fileName)?
                          fileManagerInstance->isReadableAndWritable(fileName)?
                              NoIcon:
                              LockedIcon:
                          NoIcon;
        // Note: we really want to call isReadableAndWritable() rather than
        //       isLocked() since no icon should be shown only if the file can
        //       be both readable and writable (see
        //       CorePlugin::filePermissionsChanged())...
    }

    if (QVariant(tabIcon) != QVariant(mFileTabs->tabIcon(pIndex))) {
        mFileTabs->setTabIcon(pIndex, tabIcon);
    }
}

//==============================================================================

void CentralWidget::importRemoteFile(const QString &pFileNameOrUrl)
{
    // Check whether pFileNameOrUrl refers to a remote or a local file and if it
    // is the latter then import it directly

    bool isLocalFile;
    QString fileNameOrUrl;

    checkFileNameOrUrl(pFileNameOrUrl, isLocalFile, fileNameOrUrl);

    if (isLocalFile) {
        // Try to get our current view to import the given file and if it cannot
        // then just open it as a normal file

        Plugin *fileViewPlugin = viewPlugin(mFileTabs->currentIndex());
        FileHandlingInterface *fileHandlingInterface = qobject_cast<FileHandlingInterface *>((fileViewPlugin != nullptr)?fileViewPlugin->instance():nullptr);

        if (    (fileHandlingInterface == nullptr)
            || !fileHandlingInterface->importFile(fileNameOrUrl)) {
           openFile(fileNameOrUrl);
        }

        return;
    }

    // We are dealing with a remote file, so download its contents

    QByteArray fileContents;
    QString errorMessage;

    showBusyWidget();
        bool remoteFileDownloaded = readFile(fileNameOrUrl, fileContents, &errorMessage);
    hideBusyWidget();

    if (remoteFileDownloaded) {
        // We were able to retrieve the contents of our remote file, so save it
        // to a temporary file and then import it or, if it cannot, then just
        // open it as a normal remote file

        QString temporaryFileName = Core::temporaryFileName();

        if (writeFile(temporaryFileName, fileContents)) {
            Plugin *fileViewPlugin = viewPlugin(mFileTabs->currentIndex());
            FileHandlingInterface *fileHandlingInterface = qobject_cast<FileHandlingInterface *>((fileViewPlugin != nullptr)?fileViewPlugin->instance():nullptr);

            if (    (fileHandlingInterface == nullptr)
                || !fileHandlingInterface->importFile(temporaryFileName)) {
                openRemoteFile(fileNameOrUrl);
            }
        } else {
            warningMessageBox(tr("Import Remote File"),
                              tr("<strong>%1</strong> could not be saved locally (%2).").arg(fileNameOrUrl,
                                                                                             formatMessage(errorMessage)));
        }

        QFile::remove(temporaryFileName);
    } else {
        // We were not able to retrieve the contents of the remote file, so let
        // the user know about it

        warningMessageBox(tr("Import Remote File"),
                          tr("<strong>%1</strong> could not be imported (%2).").arg(fileNameOrUrl,
                                                                                    formatMessage(errorMessage)));
    }
}

//==============================================================================

QString CentralWidget::openFile(const QString &pFileName, File::Type pType,
                                const QString &pUrl, bool pShowWarning)
{
    // Make sure that modes are available and that the file exists

    if ((mModeTabs->count() == 0) || !QFile::exists(pFileName)) {
        // Let the user know about us not being able to open the file, but only
        // if we are not starting OpenCOR, i.e. only if our main window is
        // visible

        if (pShowWarning && mainWindow()->isVisible()) {
            warningMessageBox(pUrl.isEmpty()?
                                  tr("Open File"):
                                  tr("Open Remote File"),
                              tr("<strong>%1</strong> could not be opened.").arg(pUrl.isEmpty()?
                                                                                     QDir::toNativeSeparators(pFileName):
                                                                                     pFileName));
        }

        return tr("'%1' could not be opened.").arg(pUrl.isEmpty()?
                                                       QDir::toNativeSeparators(pFileName):
                                                       pFileName);
    }

    // Check whether the file is already opened and, if so, select it and leave

    QString fileName = canonicalFileName(pFileName);

    for (int i = 0, iMax = mFileNames.count(); i < iMax; ++i) {
        if (mFileNames[i] == fileName) {
            setTabBarCurrentIndex(mFileTabs, i);

            return {};
        }
    }

    // Register the file with our file manager

    FileManager::instance()->manage(fileName, pType, pUrl);

    // Create a new tab, insert it just after the current tab, set the full name
    // of the file as the tool tip for the new tab, and make the new tab the
    // current one
    // Note #1: mFileNames is, for example, used to retrieve the name of a file,
    //          which we want to close (see closeFile()), so we must make sure
    //          that the order of its contents matches that of the tabs...
    // Note #2: rather than using mFileNames, we could have used a tab's tool
    //          tip, but this makes it a bit tricky to handle with regards to
    //          connections and therefore with regards to some events triggering
    //          updateGui() to be called when the tool tip has not yet been
    //          assigned...

    int fileTabIndex = mFileTabs->currentIndex()+1;

    mFileNames.insert(fileTabIndex, fileName);
    mFileTabs->insertTab(fileTabIndex, QString());

    updateFileTab(fileTabIndex);

    setTabBarCurrentIndex(mFileTabs, fileTabIndex);

    // Everything went fine, so let our plugins know that our file has been
    // opened
    // Note: this requires using mLoadedFileHandlingPlugins, but it will not be
    //       set when we come here following OpenCOR's loading of settings,
    //       hence we do something similar to what is done in
    //       settingsLoaded()...

    for (auto plugin : mLoadedFileHandlingPlugins) {
        qobject_cast<FileHandlingInterface *>(plugin->instance())->fileOpened(fileName);
    }

    return {};
}

//==============================================================================

void CentralWidget::openFiles(const QStringList &pFileNames)
{
    // Open the various files

    for (int i = 0, iMax = pFileNames.count(); i < iMax; ++i) {
        openFile(pFileNames[i]);
    }
}

//==============================================================================

void CentralWidget::openFile()
{
    // Ask for the file(s) to be opened

    QStringList fileNames = getOpenFileNames(tr("Open"),
                                             filters(fileTypeInterfaces()));

    // Open the file(s)

    openFiles(fileNames);
}

//==============================================================================

QString CentralWidget::openRemoteFile(const QString &pUrl, bool pShowWarning)
{
    // Note: this method is used by the GUI and should be kept in sync with that
    //       of openRemoteFile() in
    //       src/plugins/miscellaneous/Core/src/corecliutils.cpp...

    // Make sure that pUrl really refers to a remote file

    bool isLocalFile;
    QString fileNameOrUrl;

    checkFileNameOrUrl(pUrl, isLocalFile, fileNameOrUrl);

    if (isLocalFile) {
        // It looks like the user tried to open a local file using a URL, e.g.
        //     file:///home/me/mymodel.cellml
        // rather than a local file name, e.g.
        //     /home/me/mymodel.cellml
        // so open the file as a local file and leave

        return openFile(fileNameOrUrl, File::Type::Local, QString(), pShowWarning);
    }

    // Check whether the remote file is already opened and if so select it,
    // otherwise retrieve its contents

    FileManager *fileManagerInstance = FileManager::instance();
    QString fileName = fileManagerInstance->fileName(fileNameOrUrl);

    if (fileName.isEmpty()) {
        // The remote file isn't already opened, so download its contents

        QByteArray fileContents;
        QString errorMessage;

        showBusyWidget();
        // Note: we don't subsequently hide our busy widget in case we are
        //       loading a SED-ML file / COMBINE archive. Indeed, such files may
        //       require further initialisation (in the case of the Simulation
        //       Experiment view, for example). So, instead, our busy widget
        //       will get hidden in updateGui()...

        if (readFile(fileNameOrUrl, fileContents, &errorMessage)) {
            // We were able to retrieve the contents of the remote file, so ask
            // our file manager to create a new remote file

            FileManager::Status status = fileManagerInstance->create(fileNameOrUrl, fileContents);

            // Make sure that the file has indeed been created

            if (status != FileManager::Status::Created) {
#ifdef QT_DEBUG
                qFatal("FATAL ERROR | %s:%d: '%s' could not be created.", __FILE__, __LINE__, qPrintable(fileNameOrUrl)); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay, cppcoreguidelines-pro-type-vararg)
#else
                return tr("'%1' could not be created.").arg(fileNameOrUrl);
#endif
            }

            return {};
        }

        // We were not able to retrieve the contents of the remote file, so let
        // the user know about it, after having hidden our busy widget

        hideBusyWidget();

        if (pShowWarning) {
            warningMessageBox(tr("Open Remote File"),
                              tr("<strong>%1</strong> could not be opened (%2).").arg(fileNameOrUrl,
                                                                                      formatMessage(errorMessage)));
        }

        return tr("'%1' could not be opened (%2).").arg(fileNameOrUrl,
                                                        formatMessage(errorMessage));
    }

    return openFile(fileName, File::Type::Remote, fileNameOrUrl, pShowWarning);
}

//==============================================================================

void CentralWidget::openRemoteFile()
{
    // Ask for the URL of the remote file that is to be opened

    RemoteFileDialog remoteFileDialog(tr("Open Remote File"), this);

    remoteFileDialog.exec();

    if (remoteFileDialog.result() == QMessageBox::Accepted) {
        openRemoteFile(remoteFileDialog.url());
    }
}

//==============================================================================

void CentralWidget::reloadFile(int pIndex, bool pForce)
{
    // Ask our file manager to reload the file, but only if it isn't new and if
    // the user wants (in case the file has been modified)

    int realIndex = (pIndex == -1)?mFileTabs->currentIndex():pIndex;

    if (realIndex != -1) {
        FileManager *fileManagerInstance = FileManager::instance();
        QString fileName = mFileNames[realIndex];

        if (!fileManagerInstance->isNew(fileName)) {
            bool reloadFile = true;

            if (!pForce && fileManagerInstance->isModified(fileName)) {
                // The current file is modified, so ask the user whether s/he
                // still wants to reload it

                reloadFile = questionMessageBox(tr("File Modified"),
                                                tr("<strong>%1</strong> has been modified. Do you still want to reload it?").arg(QDir::toNativeSeparators(fileName))) == QMessageBox::Yes;
            }

            // Reload the file, if needed, and consider it as non-modified
            // anymore (in case it was before)
            // Note: by making the file non-modified anymore, we clearly assume
            //       that all the view plugins do their job properly and update
            //       their GUI...

            if (reloadFile) {
                // Actually redownload the file, if it is a remote one

                if (fileManagerInstance->isRemote(fileName)) {
                    QString url = fileManagerInstance->url(fileName);
                    QByteArray fileContents;
                    QString errorMessage;

                    showBusyWidget();
                    // Note: we don't subsequently hide our busy widget in case
                    //       we are loading a SED-ML file / COMBINE archive.
                    //       Indeed, such files may require further
                    //       initialisation (in the case of the Simulation
                    //       Experiment view, for example). So, instead, our
                    //       busy widget will get hidden in updateGui()...

                    if (readFile(url, fileContents, &errorMessage)) {
                        writeFile(fileName, fileContents);

                        fileManagerInstance->reload(fileName);
                    } else {
                        warningMessageBox(tr("Reload Remote File"),
                                          tr("<strong>%1</strong> could not be reloaded (%2).").arg(url,
                                                                                                    formatMessage(errorMessage)));
                    }
                } else {
                    fileManagerInstance->reload(fileName);
                }
            }
        }
    }
}

//==============================================================================

void CentralWidget::reloadFile()
{
    // Reload the current file

    reloadFile(-1, false);
}

//==============================================================================

void CentralWidget::duplicateFile()
{
    // Make sure that the file is neither new nor modified

    QString fileName = currentFileName();
    FileManager *fileManagerInstance = FileManager::instance();

    if (fileManagerInstance->isLocalNewOrModified(fileName)) {
        return;
    }

    // Ask our file manager to duplicate the current file

#ifdef QT_DEBUG
    FileManager::Status status =
#endif
    fileManagerInstance->duplicate(fileName);

#ifdef QT_DEBUG
    if (status != FileManager::Status::Duplicated) {
        qFatal("FATAL ERROR | %s:%d: '%s' could not be duplicated.", __FILE__, __LINE__, qPrintable(fileName)); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay, cppcoreguidelines-pro-type-vararg)
    }
#endif
}

//==============================================================================

void CentralWidget::toggleLockedFile()
{
    // Make sure that the file is neither new nor modified

    QString fileName = currentFileName();
    FileManager *fileManagerInstance = FileManager::instance();

    if (fileManagerInstance->isLocalNewOrModified(fileName)) {
        return;
    }

    // Ask our file manager to toggle the locked state of the current file

    bool fileLocked = fileManagerInstance->isLocked(fileName);

    if (fileManagerInstance->setLocked(fileName, !fileLocked) == FileManager::Status::LockedNotSet) {
        warningMessageBox(fileLocked?
                              tr("Unlock File"):
                              tr("Lock File"),
                          tr("<strong>%1</strong> could not be %2.").arg(QDir::toNativeSeparators(fileName),
                                                                         fileLocked?
                                                                             tr("unlocked"):
                                                                             tr("locked")));
    }
}

//==============================================================================

bool CentralWidget::saveFile(int pIndex, bool pNeedNewFileName)
{
    // Make sure that we have a valid index

    if ((pIndex < 0) || (pIndex >= mFileNames.count())) {
        return false;
    }

    // Make sure that the view plugin supports saving

    Plugin *fileViewPlugin = viewPlugin(pIndex);
    FileHandlingInterface *fileHandlingInterface = qobject_cast<FileHandlingInterface *>(fileViewPlugin->instance());
    ViewInterface *viewInterface = qobject_cast<ViewInterface *>(fileViewPlugin->instance());

    if (fileHandlingInterface == nullptr) {
        warningMessageBox(tr("Save File"),
                          tr("The <strong>%1</strong> view does not support saving files.").arg(viewInterface->viewName()));

        return false;
    }

    // Make sure that we have a valid view for the file

    QString oldFileName = mFileNames[pIndex];

    if (viewInterface->viewWidget(oldFileName) == nullptr) {
        warningMessageBox(tr("Save File"),
                          tr("The <strong>%1</strong> view cannot save <strong>%2</strong>.").arg(viewInterface->viewName(),
                                                                                                  oldFileName));

        return false;
    }

    // Make sure that we have a file name

    FileManager *fileManagerInstance = FileManager::instance();
    QString newFileName = oldFileName;
    bool fileIsNew = fileManagerInstance->isNew(oldFileName);
    bool hasNewFileName = false;

    if (fileIsNew || pNeedNewFileName) {
        // Either we are dealing with a new file or we want to save the file
        // under a new name, so we ask the user for a file name based on its
        // MIME type

        QStringList supportedFilters = filters(fileTypeInterfaces(), viewInterface->viewMimeType(oldFileName));
        QString firstSupportedFilter = supportedFilters.isEmpty()?QString():supportedFilters.first();

        newFileName = getSaveFileName(pNeedNewFileName?
                                          tr("Save As"):
                                          tr("Save"),
                                      fileIsNew?
                                          Core::newFileName(mFileTabs->tabToolTip(pIndex), viewInterface->viewDefaultFileExtension()):
                                          Core::newFileName(newFileName, tr("New"), true),
                                      supportedFilters, &firstSupportedFilter);

        // Make sure that a new file name was retrieved

        if (newFileName.isEmpty() || (newFileName == oldFileName)) {
            return false;
        }

        hasNewFileName = true;
    }

    // Try to save the file

    if (!hasNewFileName && !fileManagerInstance->isModified(oldFileName)) {
        return false;
    }

    bool needFeedback = true;

    if (!fileHandlingInterface->saveFile(oldFileName, newFileName, needFeedback)) {
        if (needFeedback) {
            warningMessageBox(tr("Save File"),
                              tr("The <strong>%1</strong> view could not save <strong>%2</strong>.").arg(viewInterface->viewName(),
                                                                                                         QDir::toNativeSeparators(newFileName)));
        }

        return false;
    }

    // Delete the 'old' file, if it was a new one that got saved
    // Note: we delete the 'old' file before updating the file name in case
    //       someone handles the renaming of a file and checks whether the old
    //       file still exists (see CorePlugin::fileRenamed() and
    //       CorePlugin::fileClosed())...

    if (fileIsNew) {
        QFile::remove(oldFileName);
    }

    // Update its file name, if needed

    if (hasNewFileName) {
        // Ask our file manager to rename the file

#ifdef QT_DEBUG
        FileManager::Status status =
#endif
        fileManagerInstance->rename(oldFileName, newFileName);

#ifdef QT_DEBUG
        if (status != FileManager::Status::Renamed) {
            qFatal("FATAL ERROR | %s:%d: '%s' could not be renamed to '%s'.", __FILE__, __LINE__, qPrintable(oldFileName), qPrintable(newFileName)); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay, cppcoreguidelines-pro-type-vararg)
        }
#endif
    }

    // The file has been saved, so ask our file manager to 'save' it too

    fileManagerInstance->save(newFileName);

    // Ask our file manager to unmanage and then (re)manage the file, if it was
    // new
    // Note: indeed, when creating a new file, our different standard file
    //       managers automatically manage it (since it's empty and therefore
    //       considered to be of any standard). So, now that the file has been
    //       successfully saved (and is, therefore, not considered to be new
    //       anymore), it is of a specific, hence we must unmanage it and
    //       (re)manage it, so that only one of our standard file manager
    //       manages it in the end...

    if (hasNewFileName) {
#ifdef QT_DEBUG
        FileManager::Status status =
#endif
        fileManagerInstance->unmanage(newFileName);

#ifdef QT_DEBUG
        if (status != FileManager::Status::Removed) {
            qFatal("FATAL ERROR | %s:%d: '%s' could not be unmanaged.", __FILE__, __LINE__, qPrintable(newFileName)); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay, cppcoreguidelines-pro-type-vararg)
        }

        status =
#endif
        fileManagerInstance->manage(newFileName);

#ifdef QT_DEBUG
        if (status != FileManager::Status::Added) {
            qFatal("FATAL ERROR | %s:%d: '%s' could not be managed.", __FILE__, __LINE__, qPrintable(newFileName)); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay, cppcoreguidelines-pro-type-vararg)
        }
#endif
    }

    return true;
}

//==============================================================================

void CentralWidget::saveFile()
{
    // Save the current file

    saveFile(mFileTabs->currentIndex());
}

//==============================================================================

void CentralWidget::saveFileAs()
{
    // Save the current file under a new name

    saveFile(mFileTabs->currentIndex(), true);
}

//==============================================================================

void CentralWidget::saveAllFiles()
{
    // Go through the different files and ask the current view to save them

    for (int i = 0, iMax = mFileNames.count(); i < iMax; ++i) {
        saveFile(i);
    }
}

//==============================================================================

void CentralWidget::previousFile()
{
    // Select the previous file

    if (mFileTabs->count() != 0) {
        setTabBarCurrentIndex(mFileTabs, (mFileTabs->currentIndex() != 0)?
                                             mFileTabs->currentIndex()-1:
                                             mFileTabs->count()-1);
    }
}

//==============================================================================

void CentralWidget::nextFile()
{
    // Select the next file

    if (mFileTabs->count() != 0) {
        setTabBarCurrentIndex(mFileTabs, (mFileTabs->currentIndex() == mFileTabs->count()-1)?
                                             0:
                                             mFileTabs->currentIndex()+1);
    }
}

//==============================================================================

bool CentralWidget::canCloseFile(int pIndex)
{
    FileManager *fileManagerInstance = FileManager::instance();
    QString fileName = mFileNames[pIndex];

    if (fileManagerInstance->isModified(fileName)) {
        // The current file is modified, so ask the user whether to save it or
        // ignore it

        QMessageBox::StandardButton button = questionMessageBox(fileManagerInstance->isNew(fileName)?
                                                                    tr("New File"):
                                                                    tr("File Modified"),
                                                                fileManagerInstance->isNew(fileName)?
                                                                    tr("<strong>%1</strong> is new. Do you want to save it before closing it?").arg(QDir::toNativeSeparators(mFileTabs->tabToolTip(pIndex))):
                                                                    tr("<strong>%1</strong> has been modified. Do you want to save it before closing it?").arg(QDir::toNativeSeparators(fileName)));

        if (button == QMessageBox::Yes) {
            return saveFile(pIndex);
        }

        return button == QMessageBox::No;
    }

    return true;
}

//==============================================================================

bool CentralWidget::closeFile(int pIndex, bool pForceClosing)
{
    // Make sure that we are not updating the GUI

    if (mState == State::UpdatingGui) {
        return false;
    }

    // Close the file at the given tab index or the current tab index, if no tab
    // index is provided, and then return the name of the file that was closed,
    // if any
    // Note: pIndex can take one of the following values:
    //        - 0+: the index of the file to close;
    //        - -1: the current file is to be closed; and
    //        - -2: same as -1, except that we come here with the intention of
    //              closing all the files (see closeAllFiles()).

    bool closingAllFiles = pIndex == -2;
    int realIndex = (pIndex < 0)?mFileTabs->currentIndex():pIndex;

    if (realIndex != -1) {
        // There is a file currently opened, so check whether the file can be
        // closed
        // Note: this may involve saving the file (possibly under a new name),
        //       hence we must retrieve the file name after making sure that the
        //       file can be closed...

        if (!closingAllFiles && !pForceClosing && !canCloseFile(realIndex)) {
            return false;
        }

        // Retrieve the file name

        QString fileName = mFileNames[realIndex];

        // Update some of our internals

        mFileNames.removeAt(realIndex);

        mFileModeTabIndexes.remove(fileName);
        mFileModeViewTabIndexes.remove(fileName);

        // Remove the file tab

        mFileTabs->removeTab(realIndex);

        // Ask our view plugins to remove the corresponding view for the file

        for (auto plugin : mLoadedViewPlugins) {
            qobject_cast<ViewInterface *>(plugin->instance())->removeViewWidget(fileName);
        }

        // Let our plugins know about the file having just been closed

        for (auto plugin : mLoadedFileHandlingPlugins) {
            qobject_cast<FileHandlingInterface *>(plugin->instance())->fileClosed(fileName);
        }

        // Unregister the file from our file manager

        FileManager::instance()->unmanage(fileName);

        // Update our modified settings

        updateModifiedSettings();

        return true;
    }

    return false;
}

//==============================================================================

bool CentralWidget::closeFile(int pIndex)
{
    // Close the file which index is given

    return closeFile(pIndex, false);
}

//==============================================================================

bool CentralWidget::closeFile(const QString &pFileName)
{
    // Close the given file

    int index = mFileNames.indexOf(pFileName);

    if (index != -1) {
        return closeFile(index, true);
    }

    return false;
}

//==============================================================================

bool CentralWidget::closeFile()
{
    // Close the current file

    return closeFile(-1, false);
}

//==============================================================================

void CentralWidget::closeAllFiles(bool pForceClosing)
{
    // Check whether we can close all the files

    if (!pForceClosing && !canClose()) {
        return;
    }

    // Close all the files

    while (closeFile(-2, pForceClosing)) {
    }
}

//==============================================================================

void CentralWidget::closeAllFiles()
{
    // Close all the files

    closeAllFiles(false);
}

//==============================================================================

void CentralWidget::moveFile(int pFromIndex, int pToIndex)
{
    // Update our list of file names to reflect the fact that a tab has been
    // moved

    mFileNames.move(pFromIndex, pToIndex);
}

//==============================================================================

bool CentralWidget::canClose()
{
    // We can close only if none of the opened files are modified

    for (int i = 0, iMax = mFileTabs->count(); i < iMax; ++i) {
        if (!canCloseFile(i)) {
            return false;
        }
    }

    return true;
}

//==============================================================================

bool CentralWidget::selectMode(const QString &pModeName)
{
    // Select the given mode

    ViewInterface::Mode mode = ViewInterface::modeFromString(pModeName);

    if (mode != ViewInterface::Mode::Unknown) {
        setTabBarCurrentIndex(mModeTabs, mModeModeTabIndexes.value(mode));

        return true;
    }

    return false;
}

//==============================================================================

bool CentralWidget::selectView(const QString &pViewName)
{
    // Select the given view, after making sure that the corresponding mode has
    // been selected

    for (int i = 0, iMax = mLoadedViewPlugins.count(); i < iMax; ++i) {
        if (   (mLoadedViewPlugins[i]->name() == pViewName)
            && selectMode(ViewInterface::modeAsString(qobject_cast<ViewInterface *>(mLoadedViewPlugins[i]->instance())->viewMode()))) {
            CentralWidgetMode *mode = mModes.value(mModeTabIndexModes.value(mModeTabs->currentIndex()));
            Plugins viewPlugins = mode->viewPlugins();

            for (int j = 0, jMax = viewPlugins.count(); j < jMax; ++j) {
                if (viewPlugins[j]->name() == pViewName) {
                    setTabBarCurrentIndex(mode->viewTabs(), j);

                    return true;
                }
            }
        }
    }

    return false;
}

//==============================================================================

void CentralWidget::addView(Plugin *pPlugin)
{
    // Make sure that our list of required modes is up to date

    ViewInterface::Mode viewMode = qobject_cast<ViewInterface *>(pPlugin->instance())->viewMode();

    if (!mModes.value(viewMode)->isEnabled()) {
        // There is no tab for the mode, so add one and enable it

        int tabIndex = mModeTabs->addTab(QString());

        mModes.value(viewMode)->setEnabled(true);

        // Keep track of the correspondance mode type/index

        mModeTabIndexModes.insert(tabIndex, viewMode);
        mModeModeTabIndexes.insert(viewMode, tabIndex);
    }

    // Add the requested view to the mode's views tab bar

    CentralWidgetMode *mode = mModes.value(viewMode);

    mode->addViewPlugin(pPlugin);
    mode->viewTabs()->addTab(QString());
}

//==============================================================================

Plugin * CentralWidget::currentViewPlugin() const
{
    // Return the current view plugin

    return viewPlugin(mFileTabs->currentIndex());
}

//==============================================================================

void CentralWidget::dragEnterEvent(QDragEnterEvent *pEvent)
{
    // Accept the proposed action for the event, but only if at least one mode
    // is available and if we are dropping URIs or items from our file organiser

    if (    (mModeTabs->count() != 0)
        &&  pEvent->mimeData()->hasFormat(FileSystemMimeType)
        && !pEvent->mimeData()->urls().isEmpty()) {
        // Note: we test the list of URLs in case we are trying to drop one or
        //       several folders (and no file) from the file organiser, in which
        //       case the list of URLs will be empty...

        pEvent->acceptProposedAction();
    } else {
        pEvent->ignore();
    }
}

//==============================================================================

void CentralWidget::dragMoveEvent(QDragMoveEvent *pEvent)
{
    // Accept the proposed action for the event
    // Note: this is required when dragging external objects...

    pEvent->acceptProposedAction();
}

//==============================================================================

void CentralWidget::dropEvent(QDropEvent *pEvent)
{
    // Open the various files

    openFiles(droppedFileNames(pEvent));

    // Accept the proposed action for the event

    pEvent->acceptProposedAction();
}

//==============================================================================

Plugin * CentralWidget::viewPlugin(int pIndex) const
{
    // Return the view plugin associated with the file, which index is given

    if (pIndex != -1) {
        int modeTabIndex = mFileModeTabIndexes.value(mFileNames[pIndex]);
        CentralWidgetMode *mode = mModes.value(mModeTabIndexModes.value(modeTabIndex));

        return (mode != nullptr)?mode->viewPlugins()[mFileModeViewTabIndexes.value(mFileNames[pIndex]).value(modeTabIndex)]:nullptr;
    }

    return nullptr;
}

//==============================================================================

Plugin * CentralWidget::viewPlugin(const QString &pFileName) const
{
    // Return the view plugin associated with the file, which file name is given

    for (int i = 0, iMax = mFileNames.count(); i < iMax; ++i) {
        if (pFileName == mFileNames[i]) {
            return viewPlugin(i);
        }
    }

    return nullptr;
}

//==============================================================================

void CentralWidget::fileReloadedOrSaved(const QString &pFileName,
                                        bool pFileReloaded)
{
    // Let all our plugins know about the file having been reloaded/saved

    for (auto plugin : mLoadedFileHandlingPlugins) {
        if (pFileReloaded) {
            qobject_cast<FileHandlingInterface *>(plugin->instance())->fileReloaded(pFileName);
        } else {
            qobject_cast<FileHandlingInterface *>(plugin->instance())->fileSaved(pFileName);
        }
    }

    // Now, because of the way some of our views may reload/save a file (see
    // EditingViewPlugin::fileReloaded() for example), we need to tell them to
    // update their GUI

    for (auto plugin : mLoadedGuiPlugins) {
        qobject_cast<GuiInterface *>(plugin->instance())->updateGui(viewPlugin(pFileName), pFileName);
    }

    // Similarly, we need to update our GUI

    updateGui();

    // Update our modified settings

    updateModifiedSettings();
}

//==============================================================================

void CentralWidget::setTabBarCurrentIndex(TabBarWidget *pTabBar, int pIndex)
{
    // Update the current index of the given tab bar widget, after having
    // temporarily disabled its handling of the currentChanged() signal, if
    // needed

    if (mState == State::UpdatingGui) {
        disconnect(pTabBar, &TabBarWidget::currentChanged,
                   this, &CentralWidget::updateGui);
    }

    pTabBar->setCurrentIndex(pIndex);

    if (mState == State::UpdatingGui) {
        connect(pTabBar, &TabBarWidget::currentChanged,
                this, &CentralWidget::updateGui);
    }
}

//==============================================================================

void CentralWidget::showEnableActions(const QList<QAction *> &pActions)
{
    // Show/enable or hide/disable the given actions, depending on whether they
    // correspond to a menu with visible/enabled or hidden/disabled actions,
    // respectively

    for (auto action : pActions) {
        QMenu *actionMenu = action->menu();

        if (actionMenu != nullptr) {
            QList<QAction *> actionMenuActions = actionMenu->actions();

            showEnableActions(actionMenuActions);

            bool showEnable = false;

            for (auto actionMenuAction : actionMenuActions) {
                if (   !actionMenuAction->isSeparator()
                    &&  actionMenuAction->isVisible()) {
                    showEnable = true;

                    break;
                }
            }

            showEnableAction(action, showEnable);
        }
    }
}

//==============================================================================

void CentralWidget::updateGui()
{
    // Check whether we are doing something, in which case we revert to our old
    // tab index, if possible
    // Note: indeed, it's too risky to update our GUI if we are already doing
    //       something since things may, for example, not be fully
    //       initialised...

    auto tabBar = qobject_cast<TabBarWidget *>(sender());

    if (mState != State::Idling) {
        if (tabBar != nullptr) {
            setTabBarCurrentIndex(tabBar, tabBar->oldIndex());
        }

        return;
    }

    // Update our state to reflect the fact that we are updating the GUI

    mState = State::UpdatingGui;

    // Keep track of our future old tab index, if possible

    if (tabBar != nullptr) {
        tabBar->setOldIndex(tabBar->currentIndex());
    }

    // Determine whether we are here as a result of changing files, modes or
    // views

    bool changedFiles = sender() == mFileTabs;
    bool changedModes = sender() == mModeTabs;
    bool changedViews = false;

    for (auto mode : mModes) {
        if (sender() == mode->viewTabs()) {
            changedViews = true;

            break;
        }
    }

    bool directCall = !changedFiles && !changedModes && !changedViews;

    // Set or keep track of the mode and view for the current file

    QString fileName = currentFileName();

    if (!fileName.isEmpty()) {
        int fileModeTabIndex = mFileModeTabIndexes.value(fileName, -1);

        // Set the mode and view for the current file, depending on the case in
        // which we are (i.e. direct call, switching files/modes/views or
        // opening a file)

        if (   changedModes
            || ((fileModeTabIndex != -1) && (changedFiles || directCall))) {
            if (changedModes) {
                fileModeTabIndex = mModeTabs->currentIndex();
            } else {
                setTabBarCurrentIndex(mModeTabs, fileModeTabIndex);
            }

            setTabBarCurrentIndex(mModes.value(mModeTabIndexModes.value(fileModeTabIndex))->viewTabs(),
                                  mFileModeViewTabIndexes.value(fileName).value(fileModeTabIndex));
        } else if (!changedViews) {
            // We are opening a file, so determine the default views that we
            // should try and if there are none, then try the Raw Text view

            QStringList defaultViews = QStringList();

            for (auto plugin : mLoadedFileTypePlugins) {
                FileTypeInterface *fileTypeInterface = qobject_cast<FileTypeInterface *>(plugin->instance());

                if (fileTypeInterface->isFile(fileName)) {
                    defaultViews = fileTypeInterface->fileTypeDefaultViews();

                    break;
                }
            }

            if (defaultViews.isEmpty()) {
                defaultViews << "RawTextView";
            }

            for (const auto &defaultView : defaultViews) {
                if (selectView(defaultView)) {
                    break;
                }
            }
        }

        // Keep track of the mode and view of the current file, should we not
        // have any track of them or should we be changing modes or views

        if ((fileModeTabIndex == -1) || changedModes || changedViews) {
            fileModeTabIndex = mModeTabs->currentIndex();

            mFileModeTabIndexes.insert(fileName, fileModeTabIndex);

            QMap<int, int> modeViewTabIndexes = mFileModeViewTabIndexes.value(fileName);
            ViewInterface::Mode fileMode = mModeTabIndexModes.value(fileModeTabIndex);

            modeViewTabIndexes.insert(fileModeTabIndex, mModes.value(fileMode)->viewTabs()->currentIndex());
            mFileModeViewTabIndexes.insert(fileName, modeViewTabIndexes);
        }
    }

    // Show/hide the different modes' corresponding views tab, as needed, and
    // retrieve the corresponding view plugin
    // Note: to prevent some GUI glitches on Windows and Linux, we must first
    //       make sure that all the views tabs are hidden before we can actually
    //       show the one we are after...

    int fileModeTabIndex = mModeTabs->currentIndex();

    mModes.value(ViewInterface::Mode::Editing)->viewTabs()->hide();
    mModes.value(ViewInterface::Mode::Simulation)->viewTabs()->hide();
#ifdef ENABLE_SAMPLE_PLUGINS
    mModes.value(ViewInterface::Mode::Sample)->viewTabs()->hide();
#endif
#ifdef ENABLE_TEST_PLUGINS
    mModes.value(ViewInterface::Mode::Test)->viewTabs()->hide();
#endif

    if (fileModeTabIndex == mModeModeTabIndexes.value(ViewInterface::Mode::Editing)) {
        mModes.value(ViewInterface::Mode::Editing)->viewTabs()->show();
    } else if (fileModeTabIndex == mModeModeTabIndexes.value(ViewInterface::Mode::Simulation)) {
        mModes.value(ViewInterface::Mode::Simulation)->viewTabs()->show();
#ifdef ENABLE_SAMPLE_PLUGINS
    } else if (fileModeTabIndex == mModeModeTabIndexes.value(ViewInterface::Mode::Sample)) {
        mModes.value(ViewInterface::Mode::Sample)->viewTabs()->show();
#endif
#ifdef ENABLE_TEST_PLUGINS
    } else if (fileModeTabIndex == mModeModeTabIndexes.value(ViewInterface::Mode::Test)) {
        mModes.value(ViewInterface::Mode::Test)->viewTabs()->show();
#endif
    }

    // Ask the GUI interface for the widget to use the current file (should
    // there be one)

    CentralWidgetMode *mode = mModes.value(mModeTabIndexModes.value(fileModeTabIndex));
    Plugin *viewPlugin = (mode != nullptr)?mode->viewPlugins()[mode->viewTabs()->currentIndex()]:nullptr;
    ViewInterface *viewInterface = (viewPlugin != nullptr)?qobject_cast<ViewInterface *>(viewPlugin->instance()):nullptr;
    QWidget *newView;

    if (fileName.isEmpty()) {
        newView = mLogoView;
    } else {
        // There is a current file, so retrieve its view and if none exists then
        // use our no-view widget instead and update its message

        newView = (viewInterface != nullptr)?viewInterface->viewWidget(fileName):nullptr;

        if (newView == nullptr) {
            newView = mNoViewMsg;

            updateNoViewMsg();
        }
    }

    // Create a connection to update the tab icon for the current file and
    // update our status bar widgets, but all of this only if we have a proper
    // view for our current file
    // Note: we pass Qt::UniqueConnection in our call to connect() so that we
    //       don't end up with several identical connections (something that
    //       might happen if we were to switch views and back)...

    auto newViewWidget = qobject_cast<ViewWidget *>(newView);

    if (newViewWidget != nullptr) {
        connect(newViewWidget, &ViewWidget::updateFileTabIcon,
                this, &CentralWidget::updateFileTabIcon,
                Qt::UniqueConnection);

        updateStatusBarWidgets(newViewWidget->statusBarWidgets());

        newView = newViewWidget->widget(fileName);
    } else {
        updateStatusBarWidgets(QList<QWidget *>());
    }

    // Replace the current view with the new one, if needed
    // Note: we have to do various things depending on the platform on which we
    //       are as well as over which widget we are when needing to replace the
    //       current view with the new one. This, so that we don't get the GUI
    //       glitches described in issues #405, #1027 and #1696...

    if (mContents->currentWidget() != newView) {
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
        mContents->setUpdatesEnabled(false);
#else
        bool hideShowStatusBar =    mainWindow()->statusBar()->isVisible()
                                 && (qobject_cast<QTabBar *>(childAt(mapFromGlobal(QCursor::pos()))) == nullptr);

        if (hideShowStatusBar) {
            mainWindow()->statusBar()->hide();
        }
#endif

        mContents->removeWidget(mContents->currentWidget());
        mContents->addWidget(newView);

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
        mContents->setUpdatesEnabled(true);
#else
        if (hideShowStatusBar) {
            mainWindow()->statusBar()->show();
        }
#endif
    }

    // Update our modified settings

    updateModifiedSettings();

    // Force the hiding of our busy widget (useful in some cases, e.g. when we
    // open/reload a remote file)

    hideBusyWidget();

    // Let our different plugins know that the GUI has been updated
    // Note: this can be useful when a plugin (e.g. CellMLTools) offers some
    //       tools that may need to be enabled/disabled and shown/hidden,
    //       depending on which view plugin and/or file are currently active...

    for (auto plugin : mLoadedGuiPlugins) {
        qobject_cast<GuiInterface *>(plugin->instance())->updateGui(viewPlugin, fileName);
    }

    // Go through our different menus and show/hide them, depending on whether
    // they have visible items

    showEnableActions(mainWindow()->menuBar()->actions());

    // Give the focus to the new view after first checking that it has a focused
    // widget

    if (newView->focusWidget() != nullptr) {
        // The new view has a focused widget, so just focus it (indeed, say that
        // we are using the CellML Annotation view and that we are editing some
        // metadata, then we don't want the CellML element list to get the focus
        // back)

        newView->focusWidget()->setFocus();
    } else {
        // The new view doesn't have a focused widget, so simply give the focus
        // to our new view

        newView->setFocus();
    }

    // Let people know whether there is at least one view, as well as whether we
    // can save as and there is/are at least one/two file/s

    emit atLeastOneView(mModeTabs->currentIndex() != -1);

    emit canSaveAs(    (mFileTabs->count() != 0)
                   &&  (newView != mNoViewMsg)
                   && !FileManager::instance()->isRemote(fileName)
                   &&  (viewInterface != nullptr));

    emit atLeastOneFile(mFileTabs->count() != 0);
    emit atLeastTwoFiles(mFileTabs->count() > 1);

    mState = State::Idling;
}

//==============================================================================

TabBarWidget *CentralWidget::newTabBarWidget(QTabBar::Shape pShape,
                                             bool pFileTabs)
{
    // Create and return a tab bar

    auto res = new TabBarWidget(this);

    res->setFocusPolicy(Qt::NoFocus);
    res->setMovable(pFileTabs);
    res->setShape(pShape);
    res->setTabsClosable(pFileTabs);

    return res;
}

//==============================================================================

void CentralWidget::updateNoViewMsg()
{
    // Customise, if possible, our no view widget so that it shows a relevant
    // warning message

    int fileTabIndex = mFileTabs->currentIndex();
    Plugin *fileViewPlugin = viewPlugin(fileTabIndex);

    if (fileViewPlugin != nullptr) {
        if (Core::FileManager::instance()->isNew(mFileNames[fileTabIndex])) {
            mNoViewMsg->setMessage(tr("The <strong>%1</strong> view does not support new files...").arg(qobject_cast<ViewInterface *>(fileViewPlugin->instance())->viewName()));
        } else {
            mNoViewMsg->setMessage(tr("The <strong>%1</strong> view does not support this type of file...").arg(qobject_cast<ViewInterface *>(fileViewPlugin->instance())->viewName()));
        }
    } else {
        mNoViewMsg->resetMessage();
    }
}

//==============================================================================

void CentralWidget::fileChanged(const QString &pFileName, bool pFileChanged,
                                bool pDependenciesChanged)
{
    // Make sure that the fact that the file has been changed is still relevant,
    // i.e. the given file has either been modified or it's different from its
    // physical version

    FileManager *fileManagerInstance = FileManager::instance();
    bool fileChanged =     pFileChanged
                       && !fileManagerInstance->isModified(pFileName)
                       &&  fileManagerInstance->isDifferent(pFileName);

    if (fileChanged || pDependenciesChanged) {
        // The given file and/or one or several of its dependencies has changed,
        // so ask the user whether to reload the given file
        // Note: we temporarily disable the fact that our file manager can check
        //       its fiels. Indeed, we are going to show a message box and this
        //       would normally result in our file manager stopping its timer
        //       and then restarting it straightaway once our message box
        //       disappears (see FileManager::focusWindowChanged()). So, if we
        //       were not to do this, the fileChanged() signal would be handled
        //       a second time before we get a chance to reload the changed
        //       file/dependency...

        fileManagerInstance->setCheckFilesEnabled(false);

        if (questionMessageBox(tr("File Modified"),
                               fileChanged?
                                   pDependenciesChanged?
                                       tr("<strong>%1</strong> and one or several of its dependencies has been modified. Do you want to reload it?").arg(QDir::toNativeSeparators(pFileName)):
                                       tr("<strong>%1</strong> has been modified. Do you want to reload it?").arg(QDir::toNativeSeparators(pFileName)):
                                   tr("<strong>%1</strong> has had one or several of its dependencies modified. Do you want to reload it?").arg(QDir::toNativeSeparators(pFileName))) == QMessageBox::Yes) {
            // The user wants to reload the file, so find it and reload it

            for (int i = 0, iMax = mFileNames.count(); i < iMax; ++i) {
                if (mFileNames[i] == pFileName) {
                    reloadFile(i, true);

                    break;
                }
            }
        } else {
            // The user doesn't want to reload the file, so check whether the
            // file and/or one or several of its dependencies has changed and,
            // if so, keep track of that/those fact/s

            if (fileChanged) {
                fileManagerInstance->setModified(pFileName, true);
            }

            if (pDependenciesChanged) {
                fileManagerInstance->setDependenciesModified(pFileName, true);
            }
        }

        fileManagerInstance->setCheckFilesEnabled(true);
    }
}

//==============================================================================

void CentralWidget::fileDeleted(const QString &pFileName)
{
    // The given file doesn't exist anymore, so ask the user whether to close it

    if (questionMessageBox(tr("File Deleted"),
                           tr("<strong>%1</strong> does not exist anymore. Do you want to close it?").arg(QDir::toNativeSeparators(pFileName))) == QMessageBox::Yes) {
        // The user wants to close the file

        for (int i = 0, iMax = mFileNames.count(); i < iMax; ++i) {
            if (mFileNames[i] == pFileName) {
                // We have found the file to close

                closeFile(i, true);

                break;
            }
        }
    } else {
        // The user wants to keep the file, so consider it as new now

        FileManager::instance()->makeNew(pFileName);

        // Update our modified settings

        updateModifiedSettings();
    }
}

//==============================================================================

void CentralWidget::updateModifiedSettings()
{
    // Update all our file tabs and determine the number of modified files

    FileManager *fileManagerInstance = FileManager::instance();
    int nbOfModifiedFiles = 0;

    for (int i = 0, iMax = mFileTabs->count(); i < iMax; ++i) {
        updateFileTab(i);

        if (fileManagerInstance->isNewOrModified(mFileNames[i])) {
            ++nbOfModifiedFiles;
        }
    }

    // Reset the enabled state and tool tip of our Mode tabs and of all our View
    // tabs

    mModeTabs->setEnabled(true);
    mModeTabs->setToolTip(QString());

    for (auto mode : mModes) {
        TabBarWidget *viewTabs = mode->viewTabs();

        viewTabs->setEnabled(true);
        viewTabs->setToolTip(QString());
    }

    // Enable/disable the Mode tabs and the current mode's View tabs, in case
    // the current file has been modified

    QString fileName = (mFileTabs->count() != 0)?
                           mFileNames[mFileTabs->currentIndex()]:
                           QString();

    if (fileManagerInstance->isModified(fileName)) {
        mModeTabs->setEnabled(false);
        mModeTabs->setToolTip(tr("The file is being edited, so switching modes is not possible for now"));

        TabBarWidget *viewTabs = mModes.value(mModeTabIndexModes.value(mModeTabs->currentIndex()))->viewTabs();

        viewTabs->setEnabled(false);
        viewTabs->setToolTip(tr("The file is being edited, so switching views is not possible for now"));
    }

    // Let people know whether we can save the current file and/or all files

    emit canSave(fileManagerInstance->isNewOrModified(fileName));
    emit canSaveAll(nbOfModifiedFiles != 0);
}

//==============================================================================

void CentralWidget::filePermissionsChanged(const QString &pFileName)
{
    // Update the tab text for the given file

    for (int i = 0, iMax = mFileNames.count(); i < iMax; ++i) {
        if (pFileName == mFileNames[i]) {
            updateFileTab(i);

            break;
        }
    }

    // Let our plugins know about the file having had its permissions changed

    for (auto plugin : mLoadedFileHandlingPlugins) {
        qobject_cast<FileHandlingInterface *>(plugin->instance())->filePermissionsChanged(pFileName);
    }
}

//==============================================================================

void CentralWidget::fileModified(const QString &pFileName)
{
    // Let our plugins know about the file having been modified

    for (auto plugin : mLoadedFileHandlingPlugins) {
        qobject_cast<FileHandlingInterface *>(plugin->instance())->fileModified(pFileName);
    }
}

//==============================================================================

void CentralWidget::fileReloaded(const QString &pFileName)
{
    // Let people know that our file has been reloaded

    fileReloadedOrSaved(pFileName, true);
}

//==============================================================================

void CentralWidget::fileCreated(const QString &pFileName, const QString &pUrl)
{
    // A file got created, so open it as new if no URL is provided or remote
    // otherwise

    openFile(pFileName, pUrl.isEmpty()?File::Type::New:File::Type::Remote, pUrl);
}

//==============================================================================

void CentralWidget::fileDuplicated(const QString &pFileName)
{
    // A file got duplicated, so open it as new

    openFile(pFileName, File::Type::New);
}

//==============================================================================

void CentralWidget::fileRenamed(const QString &pOldFileName,
                                const QString &pNewFileName)
{
    // A file has been renamed, so update various things

    for (int i = 0, iMax = mFileNames.count(); i < iMax; ++i) {
        if (mFileNames[i] == pOldFileName) {
            // Update our internal copy of the file name

            mFileNames[i] = pNewFileName;

            // Update the model and view raw names

            mFileModeTabIndexes.insert(pNewFileName, mFileModeTabIndexes.value(pOldFileName));
            mFileModeViewTabIndexes.insert(pNewFileName, mFileModeViewTabIndexes.value(pOldFileName));

            mFileModeTabIndexes.remove(pOldFileName);
            mFileModeViewTabIndexes.remove(pOldFileName);

            // Update the file tab

            mFileTabs->setTabText(i, QFileInfo(pNewFileName).fileName());
            mFileTabs->setTabToolTip(i, QDir::toNativeSeparators(pNewFileName));

            // Let our plugins know about a file having been renamed

            for (auto plugin : mLoadedFileHandlingPlugins) {
                qobject_cast<FileHandlingInterface *>(plugin->instance())->fileRenamed(pOldFileName, pNewFileName);
            }

            break;
        }
    }
}

//==============================================================================

void CentralWidget::fileSaved(const QString &pFileName)
{
    // Let people know that our file has been saved

    fileReloadedOrSaved(pFileName, false);
}

//==============================================================================

void CentralWidget::updateFileTabIcon(const QString &pViewName,
                                      const QString &pFileName,
                                      const QIcon &pIcon)
{
    // Update the requested file tab icon, but only if the view plugin (from
    // which the signal was emitted) is the one currently active
    // Note: we are a slot, so to be on the safe side, we need to make sure that
    //       the view plugin still exists...

    Plugin *fileViewPlugin = viewPlugin(pFileName);

    if (fileViewPlugin != nullptr) {
        if (pViewName == qobject_cast<ViewInterface *>(fileViewPlugin->instance())->viewName()) {
            // The view from which the signal was emitted is the one currently
            // active, so we can try to handle its signal

            int index = mFileNames.indexOf(pFileName);

            if (index != -1) {
                if (pIcon.isNull()) {
                    updateFileTab(index, true);
                } else {
                    mFileTabs->setTabIcon(index, pIcon);
                }
            }
        }
    }
}

//==============================================================================

void CentralWidget::updateFileTabIcons()
{
    // Update all the file tab icons

    for (int i = 0, iMax = mFileTabs->count(); i < iMax; ++i) {
        QIcon tabIcon = qobject_cast<ViewInterface *>(viewPlugin(i)->instance())->fileTabIcon(mFileNames[i]);

        if (tabIcon.isNull()) {
            updateFileTab(i, true);
        } else {
            mFileTabs->setTabIcon(i, tabIcon);
        }
    }
}

//==============================================================================

void CentralWidget::updateStatusBarWidgets(const QList<QWidget *> &pWidgets)
{
    // Remove (hide) our existing status bar widgets

    static QList<QWidget *> statusBarWidgets = QList<QWidget *>();

    for (auto statusBarWidget : statusBarWidgets) {
        mainWindow()->statusBar()->removeWidget(statusBarWidget);
    }

    // Add and show the given status bar widgets, and keep track of them

    statusBarWidgets.clear();

    for (auto widget : pWidgets) {
        mainWindow()->statusBar()->addWidget(widget);

        widget->show();

        statusBarWidgets << widget;
    }
}

//==============================================================================

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
