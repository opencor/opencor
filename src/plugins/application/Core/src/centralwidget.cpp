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
#include <QPainter>
#include <QSettings>
#include <QStackedWidget>
#include <QUrl>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

CentralWidget::CentralWidget(QWidget *pParent) :
    QWidget(pParent),
    CommonWidget(pParent),
    mUi(new Ui::CentralWidget),
    mSimulationViewInterface(0)
{
    // Set up the UI

    mUi->setupUi(this);

    // Allow for things to be dropped on us

    setAcceptDrops(true);

    // Create our file manager

    mFileManager = new FileManager();

    // Logo settings

    static const QString logoResourceName = ":logo";

    mLogo.load(logoResourceName);

    mLogoWidth  = mLogo.width();
    mLogoHeight = mLogo.height();

    // Set the background colour to that of the logo's

    QPalette pal = palette();

    pal.setColor(QPalette::Window, QImage(logoResourceName).pixel(0, 0));

    setPalette(pal);

    // Have the background filled automatically

    setAutoFillBackground(true);

    // Create our modes tab bar with no tabs by default

    mModes = newTabBar(QTabBar::RoundedWest);

    mModeEnabled.insert(GuiViewSettings::Editing, false);
    mModeEnabled.insert(GuiViewSettings::Simulation, false);
    mModeEnabled.insert(GuiViewSettings::Analysis, false);

    // Create our files tab bar widget

    mFiles = newTabBar(QTabBar::RoundedNorth, true, true);

    // Create our contents widget

    mContents = new QStackedWidget(this);

    mContents->setFrameStyle(QFrame::StyledPanel);

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

    mContents->addWidget(mNoView);

    // Create and set up our central widget

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *centralWidgetVBoxLayout = new QVBoxLayout(centralWidget);

    centralWidgetVBoxLayout->setContentsMargins(QMargins());
    centralWidgetVBoxLayout->setSpacing(0);

    centralWidget->setLayout(centralWidgetVBoxLayout);

    centralWidgetVBoxLayout->addWidget(mFiles);
    centralWidgetVBoxLayout->addWidget(mContents);

    // Create our different views tab bars

    mEditingViews  = newTabBar(QTabBar::RoundedEast);
    mAnalysisViews = newTabBar(QTabBar::RoundedEast);

    // Add the widgets/layout to our horizontal layout

    mUi->horizontalLayout->addWidget(mModes);

    mUi->horizontalLayout->addWidget(centralWidget);

    mUi->horizontalLayout->addWidget(mEditingViews);
    mUi->horizontalLayout->addWidget(mAnalysisViews);

    // Update the GUI

    updateGui();

    // Some connections to handle our files tab bar

    connect(mFiles, SIGNAL(tabCloseRequested(int)),
            this, SLOT(closeFile(const int &)));
    connect(mFiles, SIGNAL(currentChanged(int)),
            this, SLOT(fileSelected(const int &)));

    // A connection to handle our files tab bar

    connect(mFiles, SIGNAL(currentChanged(int)),
            this, SLOT(updateGui()));

    // A connection to handle our modes tab bar

    connect(mModes, SIGNAL(currentChanged(int)),
            this, SLOT(updateGui()));

    // Some connections to handle our views tab bars

    connect(mEditingViews, SIGNAL(currentChanged(int)),
            this, SLOT(updateGui()));
    connect(mAnalysisViews, SIGNAL(currentChanged(int)),
            this, SLOT(updateGui()));
}

//==============================================================================

CentralWidget::~CentralWidget()
{
    // Close all the files

    closeAllFiles();

    // Delete some internal objects

    delete mFileManager;
    delete mUi;
}

//==============================================================================

void CentralWidget::retranslateUi()
{
    // Retranslate the modes tab bar

    if (mModeEnabled.value(GuiViewSettings::Editing))
        mModes->setTabText(modeTabIndex(GuiViewSettings::Editing),
                           tr("Editing"));

    if (mModeEnabled.value(GuiViewSettings::Simulation))
        mModes->setTabText(modeTabIndex(GuiViewSettings::Simulation),
                           tr("Simulation"));

    if (mModeEnabled.value(GuiViewSettings::Analysis))
        mModes->setTabText(modeTabIndex(GuiViewSettings::Analysis),
                           tr("Analysis"));

    // Retranslate our views tab bar

    for (int i = 0; i < mEditingViews->count(); ++i)
        mEditingViews->setTabText(i,
                                  mEditingViewInterfaces.value(i)->viewName(mEditingViewSettings.value(i)->index()));

    for (int i = 0; i < mAnalysisViews->count(); ++i)
        mAnalysisViews->setTabText(i,
                                   mAnalysisViewInterfaces.value(i)->viewName(mAnalysisViewSettings.value(i)->index()));

    // Retranslate our no view widget message

    updateNoViewMsg();
}

//==============================================================================

static const QString SettingsOpenedFiles = "OpenedFiles";
static const QString SettingsActiveFile  = "ActiveFile";

//==============================================================================

void CentralWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the files that were previously opened

    QStringList openedFiles;

    openedFiles = pSettings->value(SettingsOpenedFiles).toStringList();

    for (int i = 0; i < openedFiles.count(); ++i)
        openFile(openedFiles.at(i));

    // Retrieve the active file

    if (openedFiles.count())
        // There is at least one file, so we can try to activate one of them

        activateFile(openedFiles.at(pSettings->value(SettingsActiveFile).toInt()));
}

//==============================================================================

void CentralWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the files that are opened

    QStringList openedFiles;

    for (int i = 0; i < mFiles->count(); ++i)
        openedFiles << mFiles->tabToolTip(i);

    pSettings->setValue(SettingsOpenedFiles, openedFiles);

    // Keep track of the active file

    pSettings->setValue(SettingsActiveFile, mFiles->currentIndex());
}

//==============================================================================

bool CentralWidget::openFile(const QString &pFileName)
{
    if (!mModes->count() || !QFileInfo(pFileName).exists())
        // No mode is available or the file doesn't exist, so...

        return false;

    // Check whether or not the file is already opened

    if (activateFile(pFileName))
        // The file is already opened and got selected, so...

        return false;

    // Register the file with our file manager

    QString fileName = QDir::toNativeSeparators(pFileName);
    // Note: this ensures that we are always dealing with a file name that makes
    //       sense on the platform on which we are

    mFileManager->manage(fileName);

    // Create a new tab and have the editor as its contents

    QFileInfo fileInfo = fileName;

    mFiles->setCurrentIndex(mFiles->addTab(fileInfo.fileName()));

    // Set the full name of the file as the tool tip for the new tab

    mFiles->setTabToolTip(mFiles->currentIndex(), fileName);

    // Update the GUI

    updateGui();

    // Everything went fine, so let people know that the file has been opened

    emit fileOpened(fileName);

    // Everything went fine, so...

    return true;
}

//==============================================================================

void CentralWidget::openFiles(const QStringList &pFileNames)
{
    // Open the various files

    for (int i = 0; i < pFileNames.count(); ++i)
        openFile(pFileNames.at(i));
}

//==============================================================================

bool CentralWidget::closeFile(const int &pIndex)
{
    // Close the file at the given tab index or the current tab index, if no tab
    // index is provided, and then return the name of the file that was closed,
    // if any

    int realIndex = (pIndex != -1)?pIndex:mFiles->currentIndex();

    if (realIndex != -1) {
        // There is a file currently opened, so first retrieve its file name

        QString fileName = mFiles->tabToolTip(realIndex);

        // Next, we must close the tab

        mFiles->removeTab(realIndex);

        // Unregister the file from our file manager

        mFileManager->unmanage(fileName);

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

    QString realFileName = QDir::toNativeSeparators(pFileName);

    for (int i = 0; i < mFiles->count(); ++i)
        if (!mFiles->tabToolTip(i).compare(realFileName)) {
            // We have found the file, so set the current index to that of its
            // tab

            mFiles->setCurrentIndex(i);

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

    emit fileSelected(mFiles->tabToolTip(pIndex));
}

//==============================================================================

int CentralWidget::nbOfFilesOpened() const
{
    // Return the number of files currently opened

    return mFiles->count();
}

//==============================================================================

QString CentralWidget::activeFileName() const
{
    // Return the name of the file currently active, if any

    if (mFiles->count())
        return mFiles->tabToolTip(mFiles->currentIndex());
    else
        return QString();
}

//==============================================================================

bool CentralWidget::isModeEnabled(const GuiViewSettings::Mode &pMode) const
{
    // Return whether a particular mode is enabled

    return mModeEnabled.contains(pMode);
}

//==============================================================================

int CentralWidget::modeTabIndex(const GuiViewSettings::Mode &pMode) const
{
    // Return the tab index of the requested mode

    switch (pMode) {
    case GuiViewSettings::Simulation:
        // If the simulation mode exists, then it has to be the first or second
        // tab, depending on whether the editing mode exists

        return  mModeEnabled.value(pMode)?
                     mModeEnabled.value(GuiViewSettings::Editing)?1:0
                    :-1;
    case GuiViewSettings::Analysis:
        // If the analysis mode exists, then it has to be the first, second or
        // third tab, depending on whether the editing and/or simulation modes
        // exist

        return  mModeEnabled.value(pMode)?
                      (mModeEnabled.value(GuiViewSettings::Editing)?1:0)
                     +(mModeEnabled.value(GuiViewSettings::Simulation)?1:0)
                    :-1;
    default:   // GuiViewSettings::Editing
        // If the editing mode exists, then it has to be the first tab

        return mModeEnabled.value(pMode)?0:-1;
    }
}

//==============================================================================

void CentralWidget::addMode(const GuiViewSettings::Mode &pMode)
{
    if (!mModeEnabled.value(pMode)) {
        // There is no tab for the mode, so add one

        mModes->addTab(QString());

        mModeEnabled.insert(pMode, true);
    }
}

//==============================================================================

void CentralWidget::addView(Plugin *pPlugin, GuiViewSettings *pSettings)
{
    static const QString CoreSimulationPlugin = "CoreSimulation";

    // Make sure that our list of required modes is up-to-date

    addMode(pSettings->mode());

    // Add the requested view to the mode's views tab bar and associate the
    // plugin to the new tab index
    // Note: the simulation mode doesn't have and need a views tab bar, since it
    //       should have only one view

    GuiInterface *guiInterface = qobject_cast<GuiInterface *>(pPlugin->instance());

    if (pSettings->mode() == GuiViewSettings::Editing) {
        int tabIndex = mEditingViews->addTab(QString());

        mEditingViewInterfaces.insert(tabIndex, guiInterface);
        mEditingViewSettings.insert(tabIndex, pSettings);
    } else if (pSettings->mode() == GuiViewSettings::Simulation) {
        if (   !mSimulationViewInterface
            && !pPlugin->name().compare(CoreSimulationPlugin))
            // The simulation view plugin hasn't already been set and we are
            // dealing with the CoreSimulation plugin, so set our simulation
            // view interface to that of the CoreSimulation plugin
            // Note: there shouldn't be a need to test that we are indeed
            //       dealing with the CoreSimulation plugin, but this is a
            //       safeguard against someone creating a simulation plugin and
            //       (stupidly) adding a view...

            mSimulationViewInterface = guiInterface;
    } else if (pSettings->mode() == GuiViewSettings::Analysis) {
        int tabIndex = mAnalysisViews->addTab(QString());

        mAnalysisViewInterfaces.insert(tabIndex, guiInterface);
        mAnalysisViewSettings.insert(tabIndex, pSettings);
    }
}

//==============================================================================

void CentralWidget::dragEnterEvent(QDragEnterEvent *pEvent)
{
    // Accept the proposed action for the event, but only if at least one mode
    // is available and if we are dropping URIs or items from our file organiser

    if (   mModes->count()
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

    for (int i = 0; i < urlList.count(); ++i)
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

void CentralWidget::paintEvent(QPaintEvent *pEvent)
{
    // Display our logo, in case no file is being managed

    if (!mFileManager->count()) {
        QPainter painter(this);

        // Paint the widget with the logo's background colour

        int widgetWidth  = width();
        int widgetHeight = height();

        // Draw the logo itself

        painter.drawPixmap(QRect(0.5*(widgetWidth-mLogoWidth),
                                 0.5*(widgetHeight-mLogoHeight),
                                 mLogoWidth, mLogoHeight),
                           mLogo);

#ifndef Q_WS_MAC
        // Draw a border around the widget

        QPen pen = painter.pen();

        pen.setColor(qApp->palette().color(QPalette::Midlight));

        painter.setPen(pen);

        QRect border = rect();

        border.adjust(0, 0, -1, -1);

        painter.drawRect(border);
#endif

        // Accept the event

        pEvent->accept();
    } else {
        // There is at least one managed file, so revert to the default paint
        // handler

        QWidget::paintEvent(pEvent);
    }
}

//==============================================================================

void CentralWidget::updateGui()
{
    bool atLeastOneManagedFile = mFileManager->count();

    // Show/hide the modes tab bar depending on whether there is at least one
    // managed file

    mModes->setVisible(atLeastOneManagedFile);

    // Do the same for the files tab bar and contents

    mFiles->setVisible(atLeastOneManagedFile);
    mContents->setVisible(atLeastOneManagedFile);

    // Show/hide the editing and analysis modes' corresponding views tab, as
    // needed

    if (atLeastOneManagedFile) {
        int crtTab = mModes->currentIndex();

        bool editingMode    = crtTab == modeTabIndex(GuiViewSettings::Editing);
        bool simulationMode = crtTab == modeTabIndex(GuiViewSettings::Simulation);
        bool analysisMode   = crtTab == modeTabIndex(GuiViewSettings::Analysis);

        mEditingViews->setVisible(editingMode);
        mAnalysisViews->setVisible(analysisMode);

        // Show/hide the required view for the current mode

        // Retrieve the GUI interface for the view we are after

        GuiInterface *guiInterface;
        int viewIndex;

        if (editingMode) {
            int tabIndex = mEditingViews->currentIndex();

            guiInterface = mEditingViewInterfaces.value(tabIndex);
            viewIndex    = mEditingViewSettings.value(tabIndex)->index();
        } else if (simulationMode) {
            guiInterface = mSimulationViewInterface;
            viewIndex    = 0;   // Since there is only one simulation view
        } else if (analysisMode) {
            int tabIndex = mAnalysisViews->currentIndex();

            guiInterface = mAnalysisViewInterfaces.value(tabIndex);
            viewIndex    = mAnalysisViewSettings.value(tabIndex)->index();
        }

        // Ask the GUI interface for the widget to use for the current file

        QWidget *newView = guiInterface->viewWidget(mFiles->tabToolTip(mFiles->currentIndex()),
                                                    viewIndex);

        if (!newView) {
            // The interface doesn't have a view for the current file, so use
            // our no view widget instead and update its v

            newView = mNoView;

            updateNoViewMsg();
        }

        // Replace the current view with the new one
        // Note: the order in which the adding and removing (as well as the
        //       showing/hiding) of view is done ensures that the replacement
        //       looks as good as possible...

        mContents->removeWidget(mContents->currentWidget());
        mContents->addWidget(newView);
    } else {
        mEditingViews->setVisible(false);
        mAnalysisViews->setVisible(false);
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

void CentralWidget::updateNoViewMsg()
{
    // Customise our no view widget so that it shows a relevant warning message

    QString viewName;
    int crtTab = mModes->currentIndex();

    if (crtTab == -1) {
        // There is no tab, so...

        return;
    } else if (crtTab == modeTabIndex(GuiViewSettings::Editing)) {
        int tabIndex = mEditingViews->currentIndex();

        viewName = mEditingViewInterfaces.value(tabIndex)->viewName(mEditingViewSettings.value(tabIndex)->index());
    } else if (crtTab == modeTabIndex(GuiViewSettings::Simulation)) {
        // In the case of the simulation mode, we have only one view, so...

        viewName = tr("Simulation");
    } else if (crtTab == modeTabIndex(GuiViewSettings::Analysis)) {
        int tabIndex = mAnalysisViews->currentIndex();

        viewName = mAnalysisViewInterfaces.value(tabIndex)->viewName(mAnalysisViewSettings.value(tabIndex)->index());
    }

    mNoViewMsg->setText(tr("Sorry, but the <strong>%1</strong> view does not currently support this type of file...").arg(viewName));
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
