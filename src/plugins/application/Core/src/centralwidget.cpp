#include "centralwidget.h"
#include "filemanager.h"
#include "tabwidget.h"

#include "ui_centralwidget.h"

#include <QDir>
#include <QDragEnterEvent>
#include <QFileInfo>
#include <QSettings>
#include <QTextStream>
#include <QUrl>

namespace OpenCOR {
namespace Core {

CentralViewWidget::CentralViewWidget(Plugin *pPlugin,
                                     const GuiViewSettings &pSettings) :
    mPlugin(pPlugin),
    mSettings(pSettings)
{
}

Plugin * CentralViewWidget::plugin() const
{
    // Return the view's plugin

    return mPlugin;
}

GuiViewSettings CentralViewWidget::settings() const
{
    // Return the view's settings

    return mSettings;
}

CentralWidget::CentralWidget(QWidget *pParent) :
    QWidget(pParent),
    CommonWidget(pParent),
    mUi(new Ui::CentralWidget)
{
    // Set up the UI

    mUi->setupUi(this);

    // Allow for things to be dropped on us

    setAcceptDrops(true);

    // Create our file manager

    mFileManager = new FileManager();

    // Create our modes tab bar

    mModes = new QTabBar(this);

    mModes->setShape(QTabBar::RoundedWest);

    // Create our tab widget

    mFiles = new TabWidget(":logo", this);

    mFiles->setUsesScrollButtons(true);
    // Note #1: the above property is style dependent and it happens that it's
    //          not enabled on Mac OS X, so... set it in all cases, even though
    //          it's already set on Windows and Linux, but one can never know
    //          what the future holds, so...
    // Note #2: if the above property is not enabled and many files are opened,
    //          then the central widget will widen reducing the width of any
    //          dock window which is clearly not what we want, so...

    // Create our views tab bar

    newViews(GuiViewSettings::Editing);
    newViews(GuiViewSettings::Simulation);
    newViews(GuiViewSettings::Analysis);

    // Add the widgets to our horizontal layout

    mUi->horizontalLayout->addWidget(mModes);
    mUi->horizontalLayout->addWidget(mFiles);

    mUi->horizontalLayout->addWidget(mViews.value(GuiViewSettings::Editing));
    mUi->horizontalLayout->addWidget(mViews.value(GuiViewSettings::Simulation));
    mUi->horizontalLayout->addWidget(mViews.value(GuiViewSettings::Analysis));

    // Update the GUI

    updateGui();

    // Some connections to handle our tab widget

    connect(mFiles, SIGNAL(tabCloseRequested(int)),
            this, SLOT(closeFile(const int &)));
    connect(mFiles, SIGNAL(currentChanged(int)),
            this, SLOT(fileSelected(const int &)));

    // A connection to handle our mode tab bars

    connect(mModes, SIGNAL(currentChanged(int)),
            this, SLOT(modeSelected(const int &)));
}

CentralWidget::~CentralWidget()
{
    // Close all the files

    closeFiles();

    // Delete some internal objects

    delete mFileManager;
    delete mUi;
}

void CentralWidget::retranslateUi()
{
    // Retranslate the modes tab bar by first removing all of them and then
    // adding the ones which are required

    // Remove all the mode tabs and reset our track of which tab refers to
    // which mode

    while (mModes->count())
        mModes->removeTab(0);

    mTabsMode.clear();

    // Add a tab for the required modes and keep track of which tab corresponds
    // to which mode

    if (mRequiredModes.contains(GuiViewSettings::Editing))
        mTabsMode.insert(mModes->addTab(tr("Editing")),
                         GuiViewSettings::Editing);

    if (mRequiredModes.contains(GuiViewSettings::Simulation))
        mTabsMode.insert(mModes->addTab(tr("Simulation")),
                         GuiViewSettings::Simulation);

    if (mRequiredModes.contains(GuiViewSettings::Analysis))
        mTabsMode.insert(mModes->addTab(tr("Analysis")),
                         GuiViewSettings::Analysis);
}

static const QString SettingsOpenedFiles = "OpenedFiles";
static const QString SettingsActiveFile  = "ActiveFile";

void CentralWidget::loadSettings(QSettings *pSettings)
{
    pSettings->beginGroup(objectName());
        // Retrieve the files that were previously opened

        QStringList openedFiles;

        openedFiles = pSettings->value(SettingsOpenedFiles).toStringList();

        for (int i = 0; i < openedFiles.count(); ++i)
            openFile(openedFiles.at(i));

        // Retrieve the active file

        if (openedFiles.count())
            // There is at least one file, so we can try to activate one of them

            activateFile(openedFiles.at(pSettings->value(SettingsActiveFile).toInt()));
    pSettings->endGroup();
}

void CentralWidget::saveSettings(QSettings *pSettings) const
{
    pSettings->beginGroup(objectName());
        // Keep track of the files that are opened

        QStringList openedFiles;

        for (int i = 0; i < mFiles->count(); ++i)
            openedFiles << mFiles->tabToolTip(i);

        pSettings->setValue(SettingsOpenedFiles, openedFiles);

        // Keep track of the active file

        pSettings->setValue(SettingsActiveFile, mFiles->currentIndex());
    pSettings->endGroup();
}

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

    QWidget *dummyWidget = new QWidget(this);

    mFiles->setCurrentIndex(mFiles->addTab(dummyWidget, fileInfo.fileName()));

    // Set the full name of the file as the tool tip for the new tab

    mFiles->setTabToolTip(mFiles->currentIndex(), fileName);

    // Give the focus to the newly created editor

    mFiles->currentWidget()->setFocus();

    // Update the GUI

    updateGui();

    // Everything went fine, so let people know that the file has been opened

    emit fileOpened(fileName);

    // Everything went fine, so...

    return true;
}

void CentralWidget::openFiles(const QStringList &pFileNames)
{
    // Open the various files

    for (int i = 0; i < pFileNames.count(); ++i)
        openFile(pFileNames.at(i));
}

bool CentralWidget::closeFile(const int &pIndex)
{
    // Close the file at the given tab index or the current tab index, if no tab
    // index is provided, and then return the name of the file that was closed,
    // if any

    int realIndex = (pIndex != -1)?pIndex:mFiles->currentIndex();

    QWidget *dummyWidget = qobject_cast<QWidget *>(mFiles->widget(realIndex));

    if (dummyWidget) {
        // There is a file currently opened, so first retrieve its file name

        QString fileName = mFiles->tabToolTip(realIndex);

        // Next, we must close the tab

        mFiles->removeTab(realIndex);

        // Then, we must release the allocated memory for the dummy widget that
        // the tab used to contain

        delete dummyWidget;

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

void CentralWidget::closeFiles()
{
    // Close all the files

    while (closeFile()) {}
}

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

            // Then, give the focus to the editor
            // Note: this will automatically trigger the currentChanged signal
            //       and therefore fileSelected slot

            mFiles->currentWidget()->setFocus();

            // Everything went fine, so...

            return true;
        }

    // We couldn't find the file, so...

    return false;
}

void CentralWidget::fileSelected(const int &pIndex)
{
    // Let people know that a file has been selected

    emit fileSelected(mFiles->tabToolTip(pIndex));
}

int CentralWidget::nbOfFilesOpened() const
{
    // Return the number of files currently opened

    return mFiles->count();
}

QString CentralWidget::activeFileName() const
{
    // Return the name of the file currently active, if any

    if (mFiles->count())
        return mFiles->tabToolTip(mFiles->currentIndex());
    else
        return QString();
}

bool CentralWidget::isModeEnabled(const GuiViewSettings::Mode &pMode) const
{
    // Return whether a particular mode is enabled

    return mRequiredModes.contains(pMode);
}

void CentralWidget::addView(Plugin *pPlugin,
                            const GuiViewSettings &pSettings)
{
    // Add a new view to our list of supported views
    // Note: the reason we are prepending rather than just appending is that
    //       GuiInterface::addView does prepend to ensure that, when creating
    //       menu items for a series of views from the same plugin, the order
    //       required by the user remains intact. E.g. say that we add views A,
    //       B and C. They will be stored as C, B and A (as opposed A, B and C
    //       if we were just to append them to the list) which means that we can
    //       then use a simple foreach statement to add a menu item for the view
    //       (since we can only insert a menu item before an existing one, as
    //       opposed to after an existing one). So, this means that by
    //       prepending to our local list, we end up with A, B and C which is
    //       what we want to then get the tab bars for the views in the right
    //       order, so...

    mSupportedViews.prepend(CentralViewWidget(pPlugin, pSettings));

    // Make sure that our list of required modes is up-to-date

    if (!isModeEnabled(pSettings.mode()))
        mRequiredModes << pSettings.mode();

    // Add the requested view to the mode's views tab bar

    mViews.value(pSettings.mode())->addTab(pSettings.name());
}

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

void CentralWidget::dragMoveEvent(QDragMoveEvent *pEvent)
{
    // Accept the proposed action for the event
    // Note: this is required when dragging external objects...

    pEvent->acceptProposedAction();
}

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

void CentralWidget::updateGui() const
{
    // Show/hide the modes tab bar depending on whether there is at least one
    // file opened

    bool atLeastOneFileOpened = mFiles->count();

    mModes->setVisible(atLeastOneFileOpened);

    // Show/hide the mode's corresponding views, if required
    // Note: we first hide all of the views tab bars, since otherwise we will
    //       see a flicker on Mac OS X

    mViews.value(GuiViewSettings::Editing)->setVisible(false);
    mViews.value(GuiViewSettings::Simulation)->setVisible(false);
    mViews.value(GuiViewSettings::Analysis)->setVisible(false);

    int crtTab = atLeastOneFileOpened?mModes->currentIndex():-1;

    if (crtTab != -1) {
        // The modes tab bar is visible and a tab is therefore selected, so show
        // only the views corresponding to that mode

        GuiViewSettings::Mode crtTabMode = mTabsMode.value(crtTab);

        mViews.value(GuiViewSettings::Editing)->setVisible(crtTabMode == GuiViewSettings::Editing);
        mViews.value(GuiViewSettings::Simulation)->setVisible(crtTabMode == GuiViewSettings::Simulation);
        mViews.value(GuiViewSettings::Analysis)->setVisible(crtTabMode == GuiViewSettings::Analysis);
    }
}

void CentralWidget::newViews(const GuiViewSettings::Mode &pMode)
{
    // Create a tab bar for the views of the requested mode

    QTabBar *tabBar = new QTabBar(this);

    mViews.insert(pMode, tabBar);

    tabBar->setShape(QTabBar::RoundedEast);
}

void CentralWidget::modeSelected(const int &)
{
   // A new mode has been selected, so make sure that its corresponding views
   // are shown

    updateGui();
}

} }
