//==============================================================================
// Central widget
//==============================================================================

#include "centralwidget.h"
#include "coreutils.h"
#include "filemanager.h"
#include "plugin.h"
#include "usermessagewidget.h"
#include "viewwidget.h"

//==============================================================================

#include "ui_centralwidget.h"

//==============================================================================

#include <QDragEnterEvent>
#include <QFileInfo>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QMimeData>
#include <QSettings>
#include <QStackedWidget>
#include <QUrl>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

CentralWidgetMode::CentralWidgetMode(CentralWidget *pOwner) :
    mEnabled(false)
{
    // Initialise a few internal objects

    mViews = pOwner->newTabBar(QTabBar::RoundedEast);

    mViewPlugins  = new CentralWidgetViewPlugins();
    mViewSettings = new CentralWidgetViewSettings();
}

//==============================================================================

CentralWidgetMode::~CentralWidgetMode()
{
    // Delete some internal objects

    delete mViewSettings;
    delete mViewPlugins;

    delete mViews;
}

//==============================================================================

bool CentralWidgetMode::isEnabled() const
{
    // Return whether the mode is enabled

    return mEnabled;
}

//==============================================================================

void CentralWidgetMode::setEnabled(const bool &pEnabled)
{
    // Set whether a mode is enabled

    mEnabled = pEnabled;
}

//==============================================================================

QTabBar * CentralWidgetMode::views() const
{
    // Return the mode's views

    return mViews;
}

//==============================================================================

CentralWidgetViewPlugins * CentralWidgetMode::viewPlugins() const
{
    // Return the mode's view plugins

    return mViewPlugins;
}

//==============================================================================

CentralWidgetViewSettings * CentralWidgetMode::viewSettings() const
{
    // Return the mode's view settings

    return mViewSettings;
}

//==============================================================================

CentralWidget::CentralWidget(QMainWindow *pMainWindow) :
    Widget(pMainWindow),
    mMainWindow(pMainWindow),
    mGui(new Ui::CentralWidget),
    mStatus(Starting),
    mLoadedPlugins(Plugins()),
    mSupportedFileTypes(FileTypes()),
    mFileNames(QStringList()),
    mPlugin(0)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Allow for things to be dropped on us

    setAcceptDrops(true);

    // Create our modes tab bar with no tabs by default

    mModeTabs = newTabBar(QTabBar::RoundedWest);

    // Create our modes

    mModes = QMap<GuiViewSettings::Mode, CentralWidgetMode *>();

    mModes.insert(GuiViewSettings::Editing, new CentralWidgetMode(this));
    mModes.insert(GuiViewSettings::Simulation, new CentralWidgetMode(this));
    mModes.insert(GuiViewSettings::Analysis, new CentralWidgetMode(this));
    // Note: these will be deleted in CentralWidget's destructor...

    // Create our files tab bar widget

    mFileTabs = newTabBar(QTabBar::RoundedNorth, true, true);

    // Create our contents widget

    mContents = new QStackedWidget(this);

    mContents->setFrameShape(QFrame::StyledPanel);

    // Create our logo view which simply display OpenCOR's logo

    mLogoView = new QWidget(this);

    mLogoView->setStyleSheet("QWidget {"
                             "    background-color: white;"
                             "    background-image: url(:logo);"
                             "    background-position: center;"
                             "    background-repeat: no-repeat;"
                             "}");

    mContents->addWidget(mLogoView);

    // Create our no view message widget which will display a customised message
    // to let the user know that there view doesn't support the type of the
    // current file

    mNoViewMsg = new UserMessageWidget(":/oxygen/actions/help-about.png", this);

    mNoViewMsg->setVisible(false);
    // Note: we don't initially want to see our no view message widget, so...

    // Create and set up our central widget

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *centralWidgetVBoxLayout = new QVBoxLayout(centralWidget);

    centralWidgetVBoxLayout->setMargin(0);
    centralWidgetVBoxLayout->setSpacing(0);

    centralWidget->setLayout(centralWidgetVBoxLayout);

    centralWidgetVBoxLayout->addWidget(mFileTabs);
    centralWidgetVBoxLayout->addWidget(mContents);

    // Add the widgets to our layout

    mGui->layout->addWidget(mModeTabs);
    mGui->layout->addWidget(centralWidget);

    foreach (CentralWidgetMode *mode, mModes)
        mGui->layout->addWidget(mode->views());

    // Some connections to handle our files tab bar

    connect(mFileTabs, SIGNAL(currentChanged(int)),
            this, SLOT(updateGui()));
    connect(mFileTabs, SIGNAL(tabMoved(int, int)),
            this, SLOT(moveFile(const int &, const int &)));
    connect(mFileTabs, SIGNAL(tabCloseRequested(int)),
            this, SLOT(closeFile(const int &)));

    // A connection to handle our modes tab bar

    connect(mModeTabs, SIGNAL(currentChanged(int)),
            this, SLOT(updateGui()));
    connect(mModeTabs, SIGNAL(currentChanged(int)),
            this, SLOT(updateFileTabIcons()));

    // Some connections to handle our mode views tab bar

    foreach (CentralWidgetMode *mode, mModes) {
        connect(mode->views(), SIGNAL(currentChanged(int)),
                this, SLOT(updateGui()));
        connect(mode->views(), SIGNAL(currentChanged(int)),
                this, SLOT(updateFileTabIcons()));
    }

    // A connection to handle a change in the modified status of a file

    connect(FileManager::instance(), SIGNAL(fileModified(const QString &, const bool &)),
            this, SLOT(updateModifiedSettings()));
}

//==============================================================================

CentralWidget::~CentralWidget()
{
    // We are shutting down, so we must let updateGui() know about it otherwise
    // we may get a segmentation fault (should there be a need to switch from
    // one view to another)

    mStatus = Stopping;

    // Close all the files
    // Note: we force the closing of all the files since canClose() will have
    //       been called before to decide what needs to be done with modified
    //       files, so...

    closeAllFiles(true);

    // Delete our various modes

    foreach (CentralWidgetMode *mMode, mModes)
        delete mMode;

    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CentralWidget::retranslateUi()
{
    // Retranslate the modes tab bar

    mModeTabs->setTabText(modeTabIndex(GuiViewSettings::Editing),
                          tr("Editing"));
    mModeTabs->setTabText(modeTabIndex(GuiViewSettings::Simulation),
                          tr("Simulation"));
    mModeTabs->setTabText(modeTabIndex(GuiViewSettings::Analysis),
                          tr("Analysis"));

    // Retranslate our mode views tab bar

    foreach (CentralWidgetMode *mode, mModes) {
        QTabBar *modeViews = mode->views();

        for (int i = 0, iMax = modeViews->count(); i < iMax; ++i)
            modeViews->setTabText(i, qobject_cast<GuiInterface *>(mode->viewPlugins()->value(i)->instance())->viewName());
    }

    // Retranslate our modified settings, if needed

    updateModifiedSettings();

    // Retranslate our no view widget message

    updateNoViewMsg();
}

//==============================================================================

static const QString SettingsFileNames          = "FileNames";
static const QString SettingsCurrentFileName    = "CurrentFileName";
static const QString SettingsCurrentMode        = "CurrentMode";
static const QString SettingsCurrentViewForMode = "CurrentViewForMode";

//==============================================================================

void CentralWidget::loadModeSettings(QSettings *pSettings,
                                     const GuiViewSettings::Mode &pCurrentMode,
                                     const GuiViewSettings::Mode &pMode)
{
    // Select the given mode, if it is the currently active one

    if (pCurrentMode == pMode) {
        int currentModeTabIndex = modeTabIndex(pCurrentMode);

        if (currentModeTabIndex != -1) {
            // A valid current mode was retrieved, so select it

            mModeTabs->setCurrentIndex(currentModeTabIndex);

            // Activate the window so that we can then give the focus to the
            // current view

            activateWindow();
        }
    }

    // Retrieve the currently active view for the given mode

    QString modeViewName = pSettings->value(SettingsCurrentViewForMode+QString::number(pMode)).toString();
    CentralWidgetMode *mode = mModes.value(pMode);
    QTabBar *modeViews = mode->views();

    for (int i = 0, iMax = modeViews->count(); i < iMax; ++i)
        if (!mode->viewPlugins()->value(i)->name().compare(modeViewName)) {
            // A valid current mode view was retrieved, so select it

            modeViews->setCurrentIndex(i);

            break;
        }
}

//==============================================================================

void CentralWidget::loadSettings(QSettings *pSettings)
{
    // Let the user know of a few default things about ourselves by emitting a
    // few signals

    emit canSave(false);
    emit canSaveAs(false);
    emit canSaveAll(false);

    emit atLeastOneFile(false);
    emit atLeastTwoFiles(false);

    // Retrieve and open the files that were previously opened

    QStringList files;

    files = pSettings->value(SettingsFileNames).toStringList();

    openFiles(files);

    // Retrieve the current file

    if (files.count())
        // There is at least one file, so we can try to activate one of them

        activateFile(files[pSettings->value(SettingsCurrentFileName).toInt()]);

    // Retrieve the currently active mode and views
    // Note: if no current mode or view can be retrieved, then we use whatever
    //       mode or view we are in...

    GuiViewSettings::Mode currentMode = (GuiViewSettings::Mode) pSettings->value(SettingsCurrentMode).toInt();

    loadModeSettings(pSettings, currentMode, GuiViewSettings::Editing);
    loadModeSettings(pSettings, currentMode, GuiViewSettings::Simulation);
    loadModeSettings(pSettings, currentMode, GuiViewSettings::Analysis);

    // Retrieve the active directory

    setActiveDirectory(pSettings->value(SettingsActiveDirectory,
                                        QDir::homePath()).toString());
}

//==============================================================================

void CentralWidget::saveModeSettings(QSettings *pSettings,
                                     const GuiViewSettings::Mode &pMode) const
{
    // Keep track of the mode, should it be the currently active one

    if (mModeTabs->currentIndex() == modeTabIndex(pMode))
        pSettings->setValue(SettingsCurrentMode, pMode);

    // Keep track of the currently active view for the given mode

    CentralWidgetMode *mode = mModes.value(pMode);
    Plugin *plugin = mode->viewPlugins()->value(mode->views()->currentIndex());

    if (plugin)
        pSettings->setValue(SettingsCurrentViewForMode+QString::number(pMode),
                            plugin->name());
}

//==============================================================================

void CentralWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the files that are opened

    pSettings->setValue(SettingsFileNames, mFileNames);

    // Keep track of the current file

    pSettings->setValue(SettingsCurrentFileName, mFileTabs->currentIndex());

    // Keep track of the currently active mode and views

    saveModeSettings(pSettings, GuiViewSettings::Editing);
    saveModeSettings(pSettings, GuiViewSettings::Simulation);
    saveModeSettings(pSettings, GuiViewSettings::Analysis);

    // Keep track of the active directory

    pSettings->setValue(SettingsActiveDirectory, activeDirectory());
}

//==============================================================================

void CentralWidget::loadingOfSettingsDone(const Plugins &pLoadedPlugins)
{
    // Keep track of the loaded plugins

    mLoadedPlugins = pLoadedPlugins;

    // Update our status now that all the plugins  are fully ready

    mStatus = Idling;

    // Update the GUI

    updateGui();
}

//==============================================================================

void CentralWidget::setSupportedFileTypes(const FileTypes &pSupportedFileTypes)
{
    // Set the supported file types

    mSupportedFileTypes = pSupportedFileTypes;
}

//==============================================================================

void CentralWidget::openFile(const QString &pFileName)
{
    if (!mModeTabs->count() || !QFileInfo(pFileName).exists())
        // No mode is available or the file doesn't exist, so...

        return;

    // Check whether the file is already opened

    if (activateFile(pFileName))
        // The file is already opened and, if anything, got selected, so...

        return;

    // Register the file with our file manager

    FileManager::instance()->manage(pFileName);

    // Create a new tab, insert it just after the current tab, set the full name
    // of the file as the tool tip for the new tab, and make the new tab the
    // current one
    // Note #1: mFileNames is, for example, used to retrieve the name of a file
    //          which we want to close (see CentralWidget::closeFile()), so we
    //          must make sure that the order of its contents matches that of
    //          the tabs...
    // Note #2: rather than using mFileNames, we could have used a tab's tool
    //          tip, but this makes it a bit tricky to handle with regards to
    //          connections and therefore some events triggering updateGui() to
    //          be called when the tool tip has not yet been assigned, so...

    QString nativeFileName = nativeCanonicalFileName(pFileName);
    QFileInfo fileInfo = nativeFileName;
    int fileTabIndex = mFileTabs->currentIndex()+1;

    mFileNames.insert(fileTabIndex, nativeFileName);
    mFileTabs->insertTab(fileTabIndex, fileInfo.fileName());

    mFileTabs->setTabToolTip(fileTabIndex, nativeFileName);

    mFileTabs->setCurrentIndex(fileTabIndex);

    // Everything went fine, so let people know that the file has been opened

    emit fileOpened(nativeFileName);
}

//==============================================================================

void CentralWidget::openFiles(const QStringList &pFileNames)
{
    // Open the various files

    for (int i = 0, iMax = pFileNames.count(); i < iMax; ++i)
        openFile(pFileNames[i]);
}

//==============================================================================

void CentralWidget::openFile()
{
    // Ask for the file(s) to be opened

    QString supportedFileTypes;

    foreach (const FileType &supportedFileType, mSupportedFileTypes)
        supportedFileTypes +=  ";;"
                              +supportedFileType.description()
                              +" (*."+supportedFileType.fileExtension()+")";

    QStringList files = getOpenFileNames(tr("Open File"),
                                         tr("All Files")
                                         +" (*"
#ifdef Q_OS_WIN
                                         +".*"
#endif
                                         +")"+supportedFileTypes);

    // Open the file(s)

    openFiles(files);
}

//==============================================================================

bool CentralWidget::saveFile(const int &pIndex, const bool &pNeedNewFileName)
{
    // Save the file, under a new name if needed

    // Make sure that we have a file name

    QString oldFileName = mFileNames[pIndex];
    QString newFileName = oldFileName;
    bool hasNewFileName = false;
    GuiInterface *guiInterface = qobject_cast<GuiInterface *>(mPlugin->instance());

    if (pNeedNewFileName || newFileName.isEmpty()) {
        // Either we want to save the file under a new name or we are dealing
        // with a new file, so we ask the user for a file name based on the MIME
        // types supported by our current view

        QStringList mimeTypes = guiInterface->guiSettings()->view()->mimeTypes();

        QString supportedFileTypes = QString();

        foreach (const FileType &supportedFileType, mSupportedFileTypes)
            if (mimeTypes.contains(supportedFileType.mimeType())) {
                if (!supportedFileTypes.isEmpty())
                    supportedFileTypes += ";;";

                supportedFileTypes +=  supportedFileType.description()
                                      +" (*."+supportedFileType.fileExtension()+")";
            }

        newFileName = getSaveFileName(tr("Save File"), newFileName, supportedFileTypes);

        // Determine whether a new file name was retrieved and leave if not

        hasNewFileName = !newFileName.isEmpty();

        if (!hasNewFileName)
            return false;
    }

    // Try to save the file in case it has been modified or it needs a new file
    // name (either as a result of a save as or because the file was new)

    if (   (FileManager::instance()->isModified(oldFileName) && !pNeedNewFileName)
        || hasNewFileName) {
        if (guiInterface->saveFile(oldFileName, newFileName)) {
            // Let people know about the file having been saved

            emit fileSaved(newFileName);

            // The file was saved, so update its file name, if needed

            if (hasNewFileName) {
                // Ask our file manager to rename the file

                FileManager::Status renameStatus = FileManager::instance()->rename(oldFileName, newFileName);

                // Make sure that the file has indeed been renamed

#ifdef QT_DEBUG
                Q_ASSERT(renameStatus == FileManager::Renamed);
#else
                Q_UNUSED(renameStatus);
#endif

                // Update our file names and tabs

                mFileNames[pIndex] = newFileName;

                mFileTabs->setTabText(pIndex, QFileInfo(newFileName).fileName());
                mFileTabs->setTabToolTip(pIndex, newFileName);

                // Let people know about the file having been renamed

                emit fileRenamed(oldFileName, newFileName);
            }

            // Update our modified settings

            updateModifiedSettings();

            // Everything went fine, so...

            return true;
        } else {
            // The file couldn't be saved, so...

            QMessageBox::warning(mMainWindow, tr("Save File"),
                                 tr("Sorry, but <strong>%1</strong> could not be saved.").arg(newFileName));

            return false;
        }
    } else {
        // Nothing to save, so...

        return false;
    }
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

    for (int i = 0, iMax = mFileNames.count(); i < iMax; ++i)
        saveFile(i);
}

//==============================================================================

void CentralWidget::previousFile()
{
    // Select the previous file

    mFileTabs->setCurrentIndex(mFileTabs->currentIndex()?
                                   mFileTabs->currentIndex()-1:
                                   mFileTabs->count()-1);
}

//==============================================================================

void CentralWidget::nextFile()
{
    // Select the next file

    mFileTabs->setCurrentIndex((mFileTabs->currentIndex() == mFileTabs->count()-1)?
                                   0:
                                   mFileTabs->currentIndex()+1);
}

//==============================================================================

bool CentralWidget::canCloseFile(const int &pIndex)
{
    QString fileName = mFileNames[pIndex];

    if (FileManager::instance()->isModified(fileName))
        // The current file is modified, so ask the user whether to save it or
        // ignore it

        switch (QMessageBox::question(mMainWindow, qApp->applicationName(),
                                      tr("<strong>%1</strong> has been modified. Do you want to save it before closing it?").arg(fileName),
                                      QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel,
                                      QMessageBox::Yes)) {
        case QMessageBox::Yes:
            // The user wants to save the file, so...

            return saveFile(pIndex);
        case QMessageBox::No:
            // The user doesn't want to save the file, so...

            return true;
        default:   // QMessageBox::Cancel
            // The user wants to cancel, so...

            return false;
        }
    else
        // The current file is not modified, so...

        return true;
}

//==============================================================================

bool CentralWidget::closeFile(const int &pIndex)
{
    if (mStatus == UpdatingGui)
        // We are updating the GUI, so we can't close the file for now

        return false;

    // Close the file at the given tab index or the current tab index, if no tab
    // index is provided, and then return the name of the file that was closed,
    // if any

    bool closingAllFiles = pIndex == -1;
    int realIndex = closingAllFiles?mFileTabs->currentIndex():pIndex;

    if (realIndex != -1) {
        // There is a file currently opened, so first retrieve its file name

        QString fileName = mFileNames[realIndex];

        // Check whether the file can be closed

        if (!closingAllFiles && !canCloseFile(realIndex))
            // The file cannot be closed, so...

            return false;

        // Next, we must close the tab

        mFileNames.removeAt(realIndex);
        mFileTabs->removeTab(realIndex);

        // Ask all the view plugins to remove the corresponding view for the
        // file

        foreach (Plugin *plugin, mLoadedPlugins) {
            GuiInterface *guiInterface = qobject_cast<GuiInterface *>(plugin->instance());

            if (guiInterface)
                mContents->removeWidget(guiInterface->removeViewWidget(fileName));
        }

        // Unregister the file from our file manager

        FileManager::instance()->unmanage(fileName);

        // Let people know about the file having just been closed

        emit fileClosed(fileName);

        // Update our modified settings

        updateModifiedSettings();

        // Everything went fine, so...

        return true;
    } else {
        // The file is not currently opened, so...

        return false;
    }
}

//==============================================================================

void CentralWidget::closeAllFiles(const bool &pForceClosing)
{
    // Check whether we can close all the files

    if (!pForceClosing && !canClose())
        // We can't close all the files, so...

        return;

    // Close all the files

    while (closeFile()) {}
}

//==============================================================================

bool CentralWidget::activateFile(const QString &pFileName)
{
    // Go through the different tabs and check whether one of them corresponds
    // to the requested file

    QString nativeFileName = nativeCanonicalFileName(pFileName);

    for (int i = 0, iMax = mFileTabs->count(); i < iMax; ++i)
        if (!mFileNames[i].compare(nativeFileName)) {
            // We have found the file, so set the current index to that of its
            // tab

            mFileTabs->setCurrentIndex(i);

            // Everything went fine, so...

            return true;
        }

    // We couldn't find the file, so...

    return false;
}

//==============================================================================

void CentralWidget::moveFile(const int &pFromIndex, const int &pToIndex)
{
    // Update our list of file names to reflect the fact that a tab has been
    // moved

    mFileNames.move(pFromIndex, pToIndex);
}

//==============================================================================

bool CentralWidget::canClose()
{
    // We can close only if none of the opened files is modified

    for (int i = 0, iMax = mFileTabs->count(); i < iMax; ++i)
        if (!canCloseFile(i))
            return false;

    // We checked all the opened files, so...

    return true;
}

//==============================================================================

int CentralWidget::modeTabIndex(const GuiViewSettings::Mode &pMode) const
{
    // Return the tab index of the requested mode

    switch (pMode) {
    case GuiViewSettings::Simulation:
        // If the simulation mode exists, then it has to be the first or second
        // tab, depending on whether the editing mode exists

        return  mModes.value(pMode)->isEnabled()?
                     mModes.value(GuiViewSettings::Editing)->isEnabled()?1:0
                    :-1;
    case GuiViewSettings::Analysis:
        // If the analysis mode exists, then it has to be the first, second or
        // third tab, depending on whether the editing and/or simulation modes
        // exist

        return  mModes.value(pMode)->isEnabled()?
                      (mModes.value(GuiViewSettings::Editing)->isEnabled()?1:0)
                     +(mModes.value(GuiViewSettings::Simulation)->isEnabled()?1:0)
                    :-1;
    default:   // GuiViewSettings::Editing
        // If the editing mode exists, then it has to be the first tab

        return mModes.value(pMode)->isEnabled()?0:-1;
    }
}

//==============================================================================

void CentralWidget::addMode(const GuiViewSettings::Mode &pMode)
{
    if (!mModes.value(pMode)->isEnabled()) {
        // There is no tab for the mode, so add one

        mModeTabs->addTab(QString());

        mModes.value(pMode)->setEnabled(true);
    }
}

//==============================================================================

void CentralWidget::addModeView(Plugin *pPlugin, GuiViewSettings *pSettings,
                                const GuiViewSettings::Mode &pMode)
{
    // Add a mode view

    CentralWidgetMode *mode = mModes.value(pMode);
    int modeViewTabIndex = mode->views()->addTab(QString());

    mode->viewPlugins()->insert(modeViewTabIndex, pPlugin);
    mode->viewSettings()->insert(modeViewTabIndex, pSettings);
}

//==============================================================================

void CentralWidget::addView(Plugin *pPlugin, GuiViewSettings *pSettings)
{
    // Make sure that our list of required modes is up-to-date

    GuiViewSettings::Mode mode = pSettings->mode();

    addMode(mode);

    // Add the requested view to the mode's views tab bar and associate the
    // plugin with the new tab index
    // Note: the simulation mode doesn't have and need a views tab bar, since it
    //       should have only one view

    if (mode == GuiViewSettings::Editing)
        addModeView(pPlugin, pSettings, GuiViewSettings::Editing);
    else if (mode == GuiViewSettings::Simulation)
        addModeView(pPlugin, pSettings, GuiViewSettings::Simulation);
    else
        addModeView(pPlugin, pSettings, GuiViewSettings::Analysis);
}

//==============================================================================

void CentralWidget::dragEnterEvent(QDragEnterEvent *pEvent)
{
    // Accept the proposed action for the event, but only if at least one mode
    // is available and if we are dropping URIs or items from our file organiser

    if (   mModeTabs->count()
        && (pEvent->mimeData()->hasFormat(FileSystemMimeType))
        && (!pEvent->mimeData()->urls().isEmpty()))
        // Note: we test the list of URLs in case we are trying to drop one or
        //       several folders (and no file) from the file organiser, in which
        //       case the list of URLs will be empty...

        pEvent->acceptProposedAction();
    else
        pEvent->ignore();
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
    // Retrieve the name of the various files that have been dropped

    QList<QUrl> urlList = pEvent->mimeData()->urls();
    QStringList fileNames;

    for (int i = 0, iMax = urlList.count(); i < iMax; ++i)
    {
        QString fileName = urlList[i].toLocalFile();
        QFileInfo fileInfo = fileName;

        if (fileInfo.isFile()) {
            if (fileInfo.isSymLink()) {
                // We are dropping a symbolic link, so retrieve its target and
                // check that it exists, and if it does then add it

                fileName = fileInfo.symLinkTarget();

                if (QFileInfo(fileName).exists())
                    fileNames << fileName;
            } else {
                // We are dropping a file, so we can just add it

                fileNames << fileName;
            }
        }
    }

    // fileNames may contain duplicates (in case we dropped some symbolic
    // links), so remove them

    fileNames.removeDuplicates();

    // Open the various files

    openFiles(fileNames);

    // Accept the proposed action for the event

    pEvent->acceptProposedAction();
}

//==============================================================================

void CentralWidget::updateModeGui(const GuiViewSettings::Mode &pMode)
{
    // Show/hide the mode's corresponding views tab, as needed

    bool modeActive = mModeTabs->currentIndex() == modeTabIndex(pMode);
    CentralWidgetMode *mode = mModes.value(pMode);

    mode->views()->setVisible(modeActive);

    // Retrieve the view plugin, should the mode be active

    if (modeActive)
        mPlugin = mode->viewPlugins()->value(mode->views()->currentIndex());
}

//==============================================================================

void CentralWidget::updateGui()
{
    if (mStatus != Idling)
        // We are doing something, so too risky to update the GUI during that
        // time (e.g. things may not be fully initialised)

        return;

    // Update our status to reflect the fact that we are updating the GUI

    mStatus = UpdatingGui;

    // Show/hide the editing, simulation and analysis modes' corresponding views
    // tab, as needed, and retrieve the corresponding view plugin

    updateModeGui(GuiViewSettings::Editing);
    updateModeGui(GuiViewSettings::Simulation);
    updateModeGui(GuiViewSettings::Analysis);

    // Ask the GUI interface for the widget to use for the current file (should
    // there be one)

    int fileTabsCrtIndex = mFileTabs->currentIndex();
    GuiInterface *guiInterface = mPlugin?qobject_cast<GuiInterface *>(mPlugin->instance()):0;

    if (fileTabsCrtIndex == -1) {
        // Let people know that we are about to update the GUI

        emit guiUpdated(mPlugin);

        // There is no current file, so show our logo instead

        mContents->removeWidget(mContents->currentWidget());
        mContents->addWidget(mLogoView);
    } else {
        // There is a current file, so retrieve its view

        QString fileName = mFileNames[fileTabsCrtIndex];
        QWidget *newView = guiInterface->viewWidget(fileName);

        if (newView) {
            // We have a view for the current file, so create a connection
            // (should it be be of the right type) to keep track of any request
            // for a change in its corresponding file tab icon
            // Note: we pass Qt::UniqueConnection in our call to connect(),
            //       thus ensuring that we don't have several identical
            //       connections (something which could happen if we were to
            //       switch views and back)...

            ViewWidget *newViewWidget = dynamic_cast<ViewWidget *>(newView);

            if (newViewWidget)
                connect(newViewWidget, SIGNAL(updateFileTabIcon(const QString &, const QIcon &)),
                        this, SLOT(updateFileTabIcon(const QString &, const QIcon &)),
                        Qt::UniqueConnection);
        } else {
            // The interface doesn't have a view for the current file, so use
            // our no-view widget instead and update its message

            newView = mNoViewMsg;

            updateNoViewMsg();
        }

        // Let people know that we are about to update the GUI

        emit guiUpdated(mPlugin);

        // Replace the current view with the new one
        // Note: the order in which the adding and removing (as well as the
        //       showing/hiding) of view is done ensures that the replacement
        //       looks as good as possible...

        mContents->removeWidget(mContents->currentWidget());
        mContents->addWidget(newView);
    }

    // Give the focus to the new view after first checking whether it has a
    // focused widget

    if (mContents->currentWidget()->focusWidget())
        // The new view has a focused widget, so just focus it (indeed, say that
        // we are using the CellML Annotation view and that we are editing some
        // metadata, then we don't want the CellML element list to get the focus
        // back, so...)

        mContents->currentWidget()->focusWidget()->setFocus();
    else
        // The new view doesn't have a focused widget, so simply give the focus
        // to our new view

        mContents->currentWidget()->setFocus();

    // Update our modified settings

    updateModifiedSettings();

    // Let people know whether we can save as, as well as whether there is/are
    // at least one/two file/s

    emit canSaveAs(   mFileTabs->count()
                   && (mContents->currentWidget() != mNoViewMsg)
                   && guiInterface && guiInterface->guiSettings()->view()->mimeTypes().count());

    emit atLeastOneFile(mFileTabs->count());
    emit atLeastTwoFiles(mFileTabs->count() > 1);

    // We are done with updating the GUI, so...

    mStatus = Idling;
}

//==============================================================================

QTabBar * CentralWidget::newTabBar(const QTabBar::Shape &pShape,
                                   const bool &pMovable,
                                   const bool &pTabsClosable)
{
    // Create and return a tab bar

    QTabBar *res = new QTabBar(this);

    res->setExpanding(false);
    res->setMovable(pMovable);
    res->setShape(pShape);
    res->setTabsClosable(pTabsClosable);
    res->setUsesScrollButtons(true);
    // Note #1: the above property is style dependent and it happens that it's
    //          not enabled on OS X, so... set it in all cases, even though it's
    //          already set on Windows and Linux, but one can never know what
    //          the future holds, so...
    // Note #2: if the above property is not enabled and many files are opened,
    //          then the central widget will widen reducing the width of any
    //          docked window which is clearly not what we want, so...

    return res;
}

//==============================================================================

QString CentralWidget::modeViewName(const GuiViewSettings::Mode &pMode)
{
    // Return the name of the mode's current view

    CentralWidgetMode *mode = mModes.value(pMode);
    int modeViewsCrtIndex = mode->views()->currentIndex();

    return qobject_cast<GuiInterface *>(mode->viewPlugins()->value(modeViewsCrtIndex)->instance())->viewName();
}

//==============================================================================

void CentralWidget::updateNoViewMsg()
{
    // Customise our no view widget so that it shows a relevant warning message

    QString viewName;
    int modeTabsCrtIndex = mModeTabs->currentIndex();

    if (modeTabsCrtIndex == -1)
        // There is no tab, so...

        return;
    else if (modeTabsCrtIndex == modeTabIndex(GuiViewSettings::Editing))
        viewName = modeViewName(GuiViewSettings::Editing);
    else if (modeTabsCrtIndex == modeTabIndex(GuiViewSettings::Simulation))
        viewName = modeViewName(GuiViewSettings::Simulation);
    else
        viewName = modeViewName(GuiViewSettings::Analysis);

    mNoViewMsg->setMessage(tr("Sorry, but the <strong>%1</strong> view does not support this type of file...").arg(viewName));
}

//==============================================================================

void CentralWidget::updateModifiedSettings()
{
    // Enable or disable the Mode and Views tabs, depending on whether one or
    // several files have been modified, and update the tab text if necessary

    int nbOfModifiedFiles = 0;

    for (int i = 0, iMax = mFileTabs->count(); i < iMax; ++i) {
        QString tabText = mFileTabs->tabText(i);

        if (FileManager::instance()->isModified(mFileNames[i])) {
            // The current file has been modified, so the Mode and Views tabs
            // should be disabled

            ++nbOfModifiedFiles;

            // Update the tab text, if needed

            if (!tabText.endsWith("*"))
                mFileTabs->setTabText(i, tabText+"*");
        } else {
            // The current file isn't modified, so update its tab text, if
            // needed

            if (tabText.endsWith("*"))
                mFileTabs->setTabText(i, tabText.remove("*"));
        }
    }

    // Enable/disable the Editing mode's View tabs, in case some files have been
    // modified

    QTabBar *editingViews = mModes.value(GuiViewSettings::Editing)->views();

    editingViews->setEnabled(!nbOfModifiedFiles);
    editingViews->setToolTip(nbOfModifiedFiles?
                                 (nbOfModifiedFiles == 1)?
                                     tr("A file is being edited, so switching editing views is not possible for now"):
                                     tr("Several files are being edited, so switching editing views is not possible for now"):
                                 QString());

    // Let people know that we can save at least one file

    emit canSave(mFileTabs->count()?
                     FileManager::instance()->isModified(mFileNames[mFileTabs->currentIndex()]):
                     false);
    emit canSaveAll(nbOfModifiedFiles);
}

//==============================================================================

void CentralWidget::updateFileTabIcons()
{
    // Update all the file tab icons

    if (mPlugin) {
        GuiInterface *guiInterface = qobject_cast<GuiInterface *>(mPlugin->instance());

        for (int i = 0, iMax = mFileTabs->count(); i < iMax; ++i)
            mFileTabs->setTabIcon(i, guiInterface->fileTabIcon(mFileNames[i]));
    }
}

//==============================================================================

void CentralWidget::updateFileTabIcon(const QString &pFileName,
                                      const QIcon &pIcon)
{
    // Update the requested file tab icon, but only if the view (from which the
    // signal was emitted) is the currently active one

    QWidget *viewWidget = qobject_cast<QWidget *>(sender());

    if (viewWidget->isVisible())
        // The view from which the signal was emitted is visible, so we can
        // handle its signal

        for (int i = 0, iMax = mFileTabs->count(); i < iMax; ++i)
            if (!pFileName.compare(mFileNames[i])) {
                // This is the file tab for which we want to update the icon, so
                // do it and leave

                mFileTabs->setTabIcon(i, pIcon);

                return;
            }
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
