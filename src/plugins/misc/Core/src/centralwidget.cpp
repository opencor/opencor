//==============================================================================
// Central widget
//==============================================================================

#include "centralwidget.h"
#include "filemanager.h"
#include "plugin.h"

//==============================================================================

#include "ui_centralwidget.h"

//==============================================================================

#include <QDir>
#include <QDragEnterEvent>
#include <QFileInfo>
#include <QLabel>
#include <QSettings>
#include <QStackedWidget>
#include <QUrl>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

CentralWidgetMode::CentralWidgetMode(CentralWidget *pOwner) :
    mIsEnabled(false)
{
    // Initialise a few internal objects

    mViews = pOwner->newTabBar(QTabBar::RoundedEast);

    mViewNames = new CentralWidgetViewNames;

    mViewInterfaces = new CentralWidgetViewInterfaces;
    mViewSettings   = new CentralWidgetViewSettings;
}

//==============================================================================

CentralWidgetMode::~CentralWidgetMode()
{
    // Delete some internal objects

    delete mViewSettings;
    delete mViewInterfaces;

    delete mViewNames;

    delete mViews;
}

//==============================================================================

bool CentralWidgetMode::isEnabled() const
{
    // Return whether the mode is enabled

    return mIsEnabled;
}

//==============================================================================

void CentralWidgetMode::setIsEnabled(const bool &pIsEnabled)
{
    // Set whether a mode is enabled

    mIsEnabled = pIsEnabled;
}

//==============================================================================

QTabBar * CentralWidgetMode::views() const
{
    // Return the mode's views

    return mViews;
}

//==============================================================================

CentralWidgetViewNames * CentralWidgetMode::viewNames() const
{
    // Return the mode's view names

    return mViewNames;
}

//==============================================================================

CentralWidgetViewInterfaces * CentralWidgetMode::viewInterfaces() const
{
    // Return the mode's view interfaces

    return mViewInterfaces;
}

//==============================================================================

CentralWidgetViewSettings * CentralWidgetMode::viewSettings() const
{
    // Return the mode's view settings

    return mViewSettings;
}

//==============================================================================

CentralWidget::CentralWidget(QWidget *pParent) :
    QWidget(pParent),
    CommonWidget(pParent),
    mUi(new Ui::CentralWidget),
    mStatus(Starting)
{
    // Set up the UI

    mUi->setupUi(this);

    // Allow for things to be dropped on us

    setAcceptDrops(true);

    // Create our modes tab bar with no tabs by default

    mModeTabs = newTabBar(QTabBar::RoundedWest);

    // Create our modes

    mModes.insert(GuiViewSettings::Editing, new CentralWidgetMode(this));
    mModes.insert(GuiViewSettings::Simulation, new CentralWidgetMode(this));
    mModes.insert(GuiViewSettings::Analysis, new CentralWidgetMode(this));

    // Create our files tab bar widget

    mFileTabs = newTabBar(QTabBar::RoundedNorth, true, true);

    // Create our contents widget

    mContents = new QStackedWidget(this);

    mContents->setFrameStyle(QFrame::StyledPanel);

    // Create our logo view which simply display OpenCOR's logo

    mLogoView = new QWidget(this);

    mLogoView->setStyleSheet("background-color: white;"
                             "background-image: url(':logo');"
                             "background-position: center;"
                             "background-repeat: no-repeat;");

    mContents->addWidget(mLogoView);

    // Create our no view widget which contains a label that will display a
    // customised warning message to the user to let him know that there is no
    // view for the current file using the current view

    mNoView = new QWidget(this);

    mNoView->setBackgroundRole(QPalette::Base);
    mNoView->setAutoFillBackground(true);

    mNoViewMsg = new QLabel(mNoView);

    QFont noViewMsgFont = mNoViewMsg->font();

    noViewMsgFont.setPointSize(1.5*noViewMsgFont.pointSize());

    mNoViewMsg->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    mNoViewMsg->setFont(noViewMsgFont);
    mNoViewMsg->setWordWrap(true);

    mNoView->setLayout(new QVBoxLayout(mNoView));
    mNoView->layout()->addWidget(mNoViewMsg);

    mNoView->setVisible(false);
    // Note: we don't initially want to see our no-view widget...

    // Create and set up our central widget

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *centralWidgetVBoxLayout = new QVBoxLayout(centralWidget);

    centralWidgetVBoxLayout->setContentsMargins(QMargins());
    centralWidgetVBoxLayout->setSpacing(0);

    centralWidget->setLayout(centralWidgetVBoxLayout);

    centralWidgetVBoxLayout->addWidget(mFileTabs);
    centralWidgetVBoxLayout->addWidget(mContents);

    // Add the widgets to our horizontal layout

    mUi->horizontalLayout->addWidget(mModeTabs);
    mUi->horizontalLayout->addWidget(centralWidget);

    foreach (CentralWidgetMode *mode, mModes)
        mUi->horizontalLayout->addWidget(mode->views());

    // Some connections to handle our files tab bar

    connect(mFileTabs, SIGNAL(tabCloseRequested(int)),
            this, SLOT(closeFile(const int &)));
    connect(mFileTabs, SIGNAL(currentChanged(int)),
            this, SLOT(fileSelected(const int &)));
    connect(mFileTabs, SIGNAL(currentChanged(int)),
            this, SLOT(updateGui()));

    // A connection to handle our modes tab bar

    connect(mModeTabs, SIGNAL(currentChanged(int)),
            this, SLOT(updateGui()));

    // Some connections to handle our mode views tab bar

    foreach (CentralWidgetMode *mode, mModes)
        connect(mode->views(), SIGNAL(currentChanged(int)),
                this, SLOT(updateGui()));
}

//==============================================================================

CentralWidget::~CentralWidget()
{
    // We are shutting down, so we must let updateGui() know about it otherwise
    // we may get a segmentation fault (should there be a need to switch from
    // one view to another)

    mStatus = Stopping;

    // Close all the files

    closeAllFiles();

    // Delete some internal objects

    delete mUi;
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
            modeViews->setTabText(i, mode->viewInterfaces()->value(i)->viewName(mode->viewSettings()->value(i)->index()));
    }

    // Retranslate our no view widget message

    updateNoViewMsg();
}

//==============================================================================

static const QString SettingsOpenedFiles        = "OpenedFiles";
static const QString SettingsCurrentFile        = "CurrentFile";
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
    CentralWidgetViewNames *modeViewNames = mode->viewNames();

    for (int i = 0, iMax = modeViews->count(); i < iMax; ++i)
        if (!modeViewNames->value(i).compare(modeViewName)) {
            // A valid current mode view was retrieved, so select it

            modeViews->setCurrentIndex(i);

            break;
        }
}

//==============================================================================

void CentralWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the files that were previously opened

    QStringList openedFiles;

    openedFiles = pSettings->value(SettingsOpenedFiles).toStringList();

    for (int i = 0, iMax = openedFiles.count(); i < iMax; ++i)
        openFile(openedFiles.at(i));

    // Retrieve the current file

    if (openedFiles.count())
        // There is at least one file, so we can try to activate one of them

        activateFile(openedFiles.at(pSettings->value(SettingsCurrentFile).toInt()));

    // Retrieve the currently active mode and views
    // Note: if no current mode or view can be retrieved, then we use whatever
    //       mode or view we are in...

    GuiViewSettings::Mode currentMode = (GuiViewSettings::Mode) pSettings->value(SettingsCurrentMode).toInt();

    loadModeSettings(pSettings, currentMode, GuiViewSettings::Editing);
    loadModeSettings(pSettings, currentMode, GuiViewSettings::Simulation);
    loadModeSettings(pSettings, currentMode, GuiViewSettings::Analysis);

    // Update our status now that we are fully ready

    mStatus = Running;

    // Update the GUI

    updateGui();
}

//==============================================================================

void CentralWidget::saveModeSettings(QSettings *pSettings,
                                     const GuiViewSettings::Mode &pMode) const
{
    // Keep track of the mode, should it be the currently active one

    int modeTabsCrtIndex = mModeTabs->currentIndex();

    if (modeTabsCrtIndex == modeTabIndex(pMode))
        pSettings->setValue(SettingsCurrentMode, pMode);

    // Keep track of the currently active view for the given mode

    CentralWidgetMode *mode = mModes.value(pMode);

    pSettings->setValue(SettingsCurrentViewForMode+QString::number(pMode),
                        mode->viewNames()->value(mode->views()->currentIndex()));
}

//==============================================================================

void CentralWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the files that are opened

    QStringList openedFiles = QStringList();

    for (int i = 0, iMax = mFileTabs->count(); i < iMax; ++i)
        openedFiles << mFileTabs->tabToolTip(i);

    pSettings->setValue(SettingsOpenedFiles, openedFiles);

    // Keep track of the current file

    pSettings->setValue(SettingsCurrentFile, mFileTabs->currentIndex());

    // Keep track of the currently active mode and views

    saveModeSettings(pSettings, GuiViewSettings::Editing);
    saveModeSettings(pSettings, GuiViewSettings::Simulation);
    saveModeSettings(pSettings, GuiViewSettings::Analysis);
}

//==============================================================================

void CentralWidget::setFocus()
{
    // Set the focus to the contents' top widget

    mContents->currentWidget()->setFocus();
}

//==============================================================================

bool CentralWidget::openFile(const QString &pFileName)
{
    if (!mModeTabs->count() || !QFileInfo(pFileName).exists())
        // No mode is available or the file doesn't exist, so...

        return false;

    // Check whether or not the file is already opened

    if (activateFile(pFileName))
        // The file is already opened and got selected, so...

        return false;

    // Register the file with our file manager

    Core::FileManager::instance()->manage(pFileName);

    // Create a new tab, insert it just after the current tab and make it the
    // new current one

    QString nativeFileName = QDir::toNativeSeparators(pFileName);
    QFileInfo fileInfo = QFileInfo(nativeFileName);

    mFileTabs->setCurrentIndex(mFileTabs->insertTab(mFileTabs->currentIndex()+1,
                                                    fileInfo.fileName()));

    // Set the full name of the file as the tool tip for the new tab
    // Note: this, for example, allows us to retrieve the name of a file which
    //       we want to close (see CentralWidget::closeFile())

    mFileTabs->setTabToolTip(mFileTabs->currentIndex(), nativeFileName);

    // Update the GUI

    updateGui();

    // Everything went fine, so let people know that the file has been opened

    emit fileOpened(nativeFileName);

    // Everything went fine, so...

    return true;
}

//==============================================================================

void CentralWidget::openFiles(const QStringList &pFileNames)
{
    // Open the various files

    for (int i = 0, iMax = pFileNames.count(); i < iMax; ++i)
        openFile(pFileNames.at(i));
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

bool CentralWidget::closeFile(const int &pIndex)
{
    // Close the file at the given tab index or the current tab index, if no tab
    // index is provided, and then return the name of the file that was closed,
    // if any

    int realIndex = (pIndex != -1)?pIndex:mFileTabs->currentIndex();

    if (realIndex != -1) {
        // There is a file currently opened, so first retrieve its file name

        QString fileName = mFileTabs->tabToolTip(realIndex);

        // Next, we must close the tab

        mFileTabs->removeTab(realIndex);

        // Unregister the file from our file manager

        Core::FileManager::instance()->unmanage(fileName);

        // Update the GUI

        updateGui();

        // Finally, we let people know about the file having just been closed

        emit fileClosed(fileName);

        // Everything went fine, so...

        return true;
    } else {
        // The file is not currently opened, so...

        return false;
    }
}

//==============================================================================

void CentralWidget::closeAllFiles()
{
    // Close all the files

    while (closeFile()) {}
}

//==============================================================================

bool CentralWidget::activateFile(const QString &pFileName)
{
    // Go through the different tabs and check whether one of them corresponds
    // to the requested file

    QString nativeFileName = QDir::toNativeSeparators(pFileName);

    for (int i = 0, iMax = mFileTabs->count(); i < iMax; ++i)
        if (!mFileTabs->tabToolTip(i).compare(nativeFileName)) {
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

void CentralWidget::fileSelected(const int &pIndex)
{
    // Let people know that a file has been selected

    emit fileSelected(mFileTabs->tabToolTip(pIndex));
}

//==============================================================================

int CentralWidget::openedFilesCount() const
{
    // Return the number of files currently opened

    return mFileTabs->count();
}

//==============================================================================

QString CentralWidget::activeFileName() const
{
    // Return the name of the file currently active, if any

    if (mFileTabs->count())
        return mFileTabs->tabToolTip(mFileTabs->currentIndex());
    else
        return QString();
}

//==============================================================================

bool CentralWidget::isModeEnabled(const GuiViewSettings::Mode &pMode) const
{
    // Return whether a particular mode is enabled

    return mModes.value(pMode)->isEnabled();
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

        mModes.value(pMode)->setIsEnabled(true);
    }
}

//==============================================================================

void CentralWidget::addModeView(Plugin *pPlugin, GuiViewSettings *pSettings,
                                const GuiViewSettings::Mode &pMode)
{
    // Add a mode view

    CentralWidgetMode *mode = mModes.value(pMode);
    int modeViewTabIndex = mode->views()->addTab(QString());

    mode->viewNames()->insert(modeViewTabIndex, pPlugin->name());
    mode->viewInterfaces()->insert(modeViewTabIndex,
                                   qobject_cast<GuiInterface *>(pPlugin->instance()));
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
        QString fileName = urlList.at(i).toLocalFile();
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

void CentralWidget::updateModeGui(const GuiViewSettings::Mode &pMode,
                                  GuiInterface * &pGuiInterface,
                                  int &pModeViewIndex)
{
    // Show/hide the mode's corresponding views tab, as needed

    bool modeActive = mModeTabs->currentIndex() == modeTabIndex(pMode);
    CentralWidgetMode *mode = mModes.value(pMode);

    mode->views()->setVisible(modeActive);

    // Retrieve the GUI interface for the view we are after, should the mode be
    // active

    if (modeActive) {
        int modeViewsCrtIndex = mode->views()->currentIndex();

        pGuiInterface  = mode->viewInterfaces()->value(modeViewsCrtIndex);
        pModeViewIndex = mode->viewSettings()->value(modeViewsCrtIndex)->index();
    }
}

//==============================================================================

void CentralWidget::updateGui()
{
    if (mStatus != Running)
        // We are either starting or stopping, so too risky to update the GUI
        // during that time (e.g. things may not be fully initialised)

        return;

    // Show/hide the editing, simulation and analysis modes' corresponding views
    // tab, as needed, and retrieve the GUI interface for the view we are after

    GuiInterface *guiInterface;
    int modeViewIndex;

    updateModeGui(GuiViewSettings::Editing, guiInterface, modeViewIndex);
    updateModeGui(GuiViewSettings::Simulation, guiInterface, modeViewIndex);
    updateModeGui(GuiViewSettings::Analysis, guiInterface, modeViewIndex);

    // Ask the GUI interface for the widget to use for the current file (should
    // there be one)

    int fileTabsCrtIndex = mFileTabs->currentIndex();
    QString fileName = (fileTabsCrtIndex == -1)?QString():mFileTabs->tabToolTip(mFileTabs->currentIndex());

    if (fileName.isEmpty()) {
        // There is no current file, so show our logo instead

        mContents->removeWidget(mContents->currentWidget());
        mContents->addWidget(mLogoView);
    } else {
        // There is a current file, so retrieve its view

        QWidget *newView = guiInterface->viewWidget(fileName, modeViewIndex);

        if (!newView) {
            // The interface doesn't have a view for the current file, so use
            // our no-view widget instead and update its message

            newView = mNoView;

            updateNoViewMsg();
        }

        // Replace the current view with the new one
        // Note: the order in which the adding and removing (as well as the
        //       showing/hiding) of view is done ensures that the replacement
        //       looks as good as possible...

        mContents->removeWidget(mContents->currentWidget());
        mContents->addWidget(newView);
    }
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
    //          not enabled on Mac OS X, so... set it in all cases, even though
    //          it's already set on Windows and Linux, but one can never know
    //          what the future holds, so...
    // Note #2: if the above property is not enabled and many files are opened,
    //          then the central widget will widen reducing the width of any
    //          dock window which is clearly not what we want, so...

    return res;
}

//==============================================================================

QString CentralWidget::modeViewName(const GuiViewSettings::Mode &pMode)
{
    // Return the name of the mode's current view

    CentralWidgetMode *mode = mModes.value(pMode);
    int modeViewsCrtIndex = mode->views()->currentIndex();

    return mode->viewInterfaces()->value(modeViewsCrtIndex)->viewName(mode->viewSettings()->value(modeViewsCrtIndex)->index());
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

    mNoViewMsg->setText(tr("Sorry, but the <strong>%1</strong> view does not support this type of file...").arg(viewName));
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
