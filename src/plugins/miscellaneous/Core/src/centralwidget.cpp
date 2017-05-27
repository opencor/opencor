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
#include "tabbarwidget.h"
#include "usermessagewidget.h"
#include "viewinterface.h"
#include "viewwidget.h"

//==============================================================================

#include <QApplication>
#include <QDesktopWidget>
#include <QDialogButtonBox>
#include <QDragEnterEvent>
#include <QFile>
#include <QFileInfo>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMimeData>
#include <QPushButton>
#include <QRect>
#include <QSettings>
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

CentralWidgetMode::CentralWidgetMode(CentralWidget *pOwner) :
    mEnabled(false),
    mViewPlugins(Plugins())
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

void CentralWidgetMode::setEnabled(const bool &pEnabled)
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
    Widget(pParent),
    mState(Starting),
    mSettings(0),
    mLoadedFileHandlingPlugins(Plugins()),
    mLoadedFileTypePlugins(Plugins()),
    mLoadedGuiPlugins(Plugins()),
    mLoadedViewPlugins(Plugins()),
    mModeTabIndexModes(QMap<int, ViewInterface::Mode>()),
    mModeModeTabIndexes(QMap<ViewInterface::Mode, int>()),
    mFileModeTabIndexes(QMap<QString, int>()),
    mFileModeViewTabIndexes(QMap<QString, QMap<int, int>>()),
    mFileTypeInterfaces(FileTypeInterfaces()),
    mFileNames(QStringList()),
    mModes(QMap<ViewInterface::Mode, CentralWidgetMode *>()),
    mRemoteLocalFileNames(QMap<QString, QString>()),
    mViews(QMap<QString, QWidget *>())
{
    // Create and set our horizontal layout

    QHBoxLayout *layout = new QHBoxLayout(this);

    layout->setContentsMargins(QMargins());
    layout->setSpacing(0);

    setLayout(layout);

    // Allow for things to be dropped on us

    setAcceptDrops(true);

    // Create our modes tab bar with no tabs by default

    mModeTabs = newTabBarWidget(QTabBar::RoundedWest);

    // Create our modes

#ifdef ENABLE_SAMPLE_PLUGINS
    mModes.insert(ViewInterface::SampleMode, new CentralWidgetMode(this));
#endif
#ifdef ENABLE_TEST_PLUGINS
    mModes.insert(ViewInterface::TestMode, new CentralWidgetMode(this));
#endif
    mModes.insert(ViewInterface::EditingMode, new CentralWidgetMode(this));
    mModes.insert(ViewInterface::SimulationMode, new CentralWidgetMode(this));
    mModes.insert(ViewInterface::AnalysisMode, new CentralWidgetMode(this));
    // Note: these will be deleted in CentralWidget's destructor...

    // Create our files tab bar widget

    mFileTabs = newTabBarWidget(QTabBar::RoundedNorth, true, true);

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

    mNoViewMsg->setVisible(false);

    // Create and set up our central widget

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *centralWidgetVBoxLayout = new QVBoxLayout(centralWidget);

    centralWidgetVBoxLayout->setContentsMargins(QMargins());
    centralWidgetVBoxLayout->setSpacing(0);

    centralWidget->setLayout(centralWidgetVBoxLayout);

    centralWidgetVBoxLayout->addWidget(mFileTabs);
    centralWidgetVBoxLayout->addWidget(mContents);

    // Add the widgets to our layout

    layout->addWidget(mModeTabs);
    layout->addWidget(centralWidget);

    foreach (CentralWidgetMode *mode, mModes)
        layout->addWidget(mode->viewTabs());

    // A connection to handle the case where a file was created or duplicated

    FileManager *fileManagerInstance = FileManager::instance();

    connect(fileManagerInstance, SIGNAL(fileCreated(const QString &, const QString &)),
            this, SLOT(fileCreated(const QString &, const QString &)));
    connect(fileManagerInstance, SIGNAL(fileDuplicated(const QString &)),
            this, SLOT(fileDuplicated(const QString &)));

    // Some connections to handle changes to a file

    connect(fileManagerInstance, SIGNAL(filePermissionsChanged(const QString &)),
            this, SLOT(filePermissionsChanged(const QString &)));
    connect(fileManagerInstance, SIGNAL(fileModified(const QString &)),
            this, SLOT(fileModified(const QString &)));

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
        connect(mode->viewTabs(), SIGNAL(currentChanged(int)),
                this, SLOT(updateGui()));
        connect(mode->viewTabs(), SIGNAL(currentChanged(int)),
                this, SLOT(updateFileTabIcons()));
    }

    // Create our remote file dialog
//---GRY--- THE ORIGINAL PLAN WAS TO HAVE A REGULAR EXPRESSION TO VALIDATE A
//          URL, BUT IT LOOKS LIKE THERE MIGHT BE AN ISSUE WITH
//          QRegularExpressionValidator, SO WE SIMPLY ALLOW FREE TEXT FOR NOW
//          (SEE https://bugreports.qt.io/browse/QTBUG-38034)

    mRemoteFileDialog = new Dialog(this);
    QGridLayout *dialogLayout = new QGridLayout(mRemoteFileDialog);

    mRemoteFileDialog->setLayout(dialogLayout);

    mRemoteFileDialogUrlLabel = new QLabel(mRemoteFileDialog);
    mRemoteFileDialogUrlValue = new QLineEdit(mRemoteFileDialog);

    mRemoteFileDialogUrlValue->setMinimumWidth(qApp->desktop()->availableGeometry().width()/5);

    dialogLayout->addWidget(mRemoteFileDialogUrlLabel, 0, 0);
    dialogLayout->addWidget(mRemoteFileDialogUrlValue, 0, 1);

    mRemoteFileDialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Open|QDialogButtonBox::Cancel, this);

    dialogLayout->addWidget(mRemoteFileDialogButtonBox, 1, 0, 1, 2);

    dialogLayout->setSizeConstraint(QLayout::SetFixedSize);

    openRemoteFileChanged();

    // Some connections to handle our remote file dialog

    connect(mRemoteFileDialogUrlValue, SIGNAL(textChanged(const QString &)),
            this, SLOT(openRemoteFileChanged()));

    connect(mRemoteFileDialogButtonBox, SIGNAL(accepted()),
            this, SLOT(doOpenRemoteFile()));
    connect(mRemoteFileDialogButtonBox, SIGNAL(rejected()),
            this, SLOT(cancelOpenRemoteFile()));
}

//==============================================================================

CentralWidget::~CentralWidget()
{
    // We are shutting down, so we must let updateGui() know about it otherwise
    // we may get a segmentation fault (should there be a need to switch from
    // one view to another)

    mState = Stopping;

    // Close all the files
    // Note: we force the closing of all the files since canClose() will have
    //       been called before to decide what needs to be done with modified
    //       files...

    closeAllFiles(true);

    // Delete our various modes

    foreach (CentralWidgetMode *mode, mModes)
        delete mode;
}

//==============================================================================

static const auto SettingsFileNamesOrUrls      = QStringLiteral("FileNamesOrUrls");
static const auto SettingsCurrentFileNameOrUrl = QStringLiteral("CurrentFileNameOrUrl");
static const auto SettingsFileIsRemote         = QStringLiteral("FileIsRemote%1");
static const auto SettingsFileMode             = QStringLiteral("FileMode%1");
static const auto SettingsFileModeView         = QStringLiteral("FileModeView%1%2");

//==============================================================================

void CentralWidget::loadSettings(QSettings *pSettings)
{
    // Keep track of our settings

    mSettings = pSettings;

    // Some connections to handle an external change in the state of a file
    // Note: we do it here because we want other plugins to get a chance to
    //       handle our file manager's signals before us. Indeed, in the case of
    //       the CellML file manager for example, we will want the CellML file
    //       manager to reload a CellML file before our different views get
    //       asked (by us, the central widget) to reload the file (i.e. update
    //       their GUIs)...

    FileManager *fileManagerInstance = FileManager::instance();

    connect(fileManagerInstance, SIGNAL(fileChanged(const QString &, const bool &, const bool &)),
            this, SLOT(fileChanged(const QString &, const bool &, const bool &)));
    connect(fileManagerInstance, SIGNAL(fileDeleted(const QString &)),
            this, SLOT(fileDeleted(const QString &)));

    // Some connections to handle an internal change in the state of a file
    // Note: we do it here for the same reason as above (see the note for the
    //       connections to handle an external change in the state of a file)...

    connect(fileManagerInstance, SIGNAL(fileModified(const QString &)),
            this, SLOT(updateModifiedSettings()));
    connect(fileManagerInstance, SIGNAL(fileReloaded(const QString &, const bool &)),
            this, SLOT(fileReloaded(const QString &, const bool &)));
    connect(fileManagerInstance, SIGNAL(fileRenamed(const QString &, const QString &)),
            this, SLOT(fileRenamed(const QString &, const QString &)));

    connect(fileManagerInstance, SIGNAL(fileSaved(const QString &)),
            this, SLOT(fileSaved(const QString &)));

    // Let the user know of a few default things about ourselves by emitting a
    // few signals

    emit canSave(false);
    emit canSaveAs(false);
    emit canSaveAll(false);

    emit atLeastOneFile(false);
    emit atLeastTwoFiles(false);

    // Retrieve and open the files that were previously opened

    QStringList fileNamesOrUrls = pSettings->value(SettingsFileNamesOrUrls).toStringList();

    foreach (const QString &fileNameOrUrl, fileNamesOrUrls) {
        if (pSettings->value(SettingsFileIsRemote.arg(fileNameOrUrl)).toBool())
            openRemoteFile(fileNameOrUrl, false);
        else
            openFile(fileNameOrUrl);
    }

    // Retrieve the selected modes and views of our different files

    foreach (const QString &fileName, mFileNames) {
        QString fileNameOrUrl = fileManagerInstance->isRemote(fileName)?
                                    fileManagerInstance->url(fileName):
                                    fileName;
        ViewInterface::Mode fileMode = ViewInterface::modeFromString(pSettings->value(SettingsFileMode.arg(fileNameOrUrl)).toString());

        if (fileMode != ViewInterface::UnknownMode)
            mFileModeTabIndexes.insert(fileName, mModeModeTabIndexes.value(fileMode));

        QMap<int, int> modeViewTabIndexes = QMap<int, int>();

        for (int i = 0, iMax = mModeTabs->count(); i < iMax; ++i) {
            fileMode = mModeTabIndexModes.value(i);

            QString viewPluginName = pSettings->value(SettingsFileModeView.arg(fileNameOrUrl, ViewInterface::modeAsString(fileMode))).toString();
            Plugins viewPlugins = mModes.value(fileMode)->viewPlugins();

            for (int j = 0, jMax = viewPlugins.count(); j < jMax; ++j) {
                if (!viewPluginName.compare(viewPlugins[j]->name())) {
                    modeViewTabIndexes.insert(i, j);

                    break;
                }
            }
        }

        mFileModeViewTabIndexes.insert(fileName, modeViewTabIndexes);
    }

    // Select the previously selected file, if it still exists, by pretending to
    // open it (which, in turn, will select the file)

    QString crtFileNameOrUrl = pSettings->value(SettingsCurrentFileNameOrUrl).toString();
    QString crtFileName = pSettings->value(SettingsFileIsRemote.arg(crtFileNameOrUrl)).toBool()?
                              mRemoteLocalFileNames.value(crtFileNameOrUrl):
                              crtFileNameOrUrl;

    if (mFileNames.contains(crtFileName)) {
        openFile(crtFileName);
    } else {
        // The previously selected file doesn't exist anymore, so select the
        // first file (otherwise the last file will be selected)

        mFileTabs->setCurrentIndex(0);
    }

    // Retrieve the seleted modes and views, in case there are no files

    if (mFileNames.isEmpty()) {
        ViewInterface::Mode fileMode = ViewInterface::modeFromString(pSettings->value(SettingsFileMode.arg(QString())).toString());

        if (fileMode != ViewInterface::UnknownMode)
            mModeTabs->setCurrentIndex(mModeModeTabIndexes.value(fileMode));

        for (int i = 0, iMax = mModeTabs->count(); i < iMax; ++i) {
            fileMode = mModeTabIndexModes.value(i);

            CentralWidgetMode *mode = mModes.value(fileMode);
            QString viewPluginName = pSettings->value(SettingsFileModeView.arg(QString(), ViewInterface::modeAsString(fileMode))).toString();
            Plugins viewPlugins = mode->viewPlugins();

            for (int j = 0, jMax = viewPlugins.count(); j < jMax; ++j) {
                if (!viewPluginName.compare(viewPlugins[j]->name())) {
                    mode->viewTabs()->setCurrentIndex(j);

                    break;
                }
            }
        }
    }
}

//==============================================================================

void CentralWidget::saveSettings(QSettings *pSettings) const
{
    // Remove all the settings related to previously opened files

    static const QString SettingsFileIsRemoteHeader = SettingsFileIsRemote.arg(QString());
    static const QString SettingsFileModeHeader = SettingsFileMode.arg(QString());
    static const QString SettingsFileModeViewHeader = SettingsFileModeView.arg(QString(), QString());

    foreach (const QString &key, pSettings->allKeys()) {
        if (   key.startsWith(SettingsFileIsRemoteHeader)
            || key.startsWith(SettingsFileModeHeader)
            || key.startsWith(SettingsFileModeViewHeader)) {
            pSettings->remove(key);
        }
    }

    // Keep track of the files that are opened, skipping new files

    FileManager *fileManagerInstance = FileManager::instance();
    QStringList fileNames = QStringList();
    QStringList fileNamesOrUrls = QStringList();

    foreach (const QString &fileName, mFileNames) {
        if (!fileManagerInstance->isNew(fileName)) {
            // The file is not new, so keep track of it, as well as of whether
            // it's a remote file

            bool fileIsRemote = fileManagerInstance->isRemote(fileName);

            if (fileIsRemote)
                fileNamesOrUrls << fileManagerInstance->url(fileName);
            else
                fileNamesOrUrls << fileName;

            fileNames << fileName;

            pSettings->setValue(SettingsFileIsRemote.arg(fileNamesOrUrls.last()), fileIsRemote);
        }
    }

    pSettings->setValue(SettingsFileNamesOrUrls, fileNamesOrUrls);

    // Keep track of the selected modes and views of our different files

    foreach (const QString &fileName, fileNames) {
        QString fileNameOrUrl = fileManagerInstance->isRemote(fileName)?
                                    fileManagerInstance->url(fileName):
                                    fileName;

        pSettings->setValue(SettingsFileMode.arg(fileNameOrUrl),
                            ViewInterface::modeAsString(mModeTabIndexModes.value(mFileModeTabIndexes.value(fileName))));

        QMap<int, int> modeViewTabIndexes = mFileModeViewTabIndexes.value(fileName);

        for (int i = 0, iMax = mModeTabs->count(); i < iMax; ++i) {
            ViewInterface::Mode fileMode = mModeTabIndexModes.value(i);

            pSettings->setValue(SettingsFileModeView.arg(fileNameOrUrl, ViewInterface::modeAsString(fileMode)),
                                mModes.value(fileMode)->viewPlugins()[modeViewTabIndexes.value(i)]->name());
        }
    }

    // Keep track of the currently selected file
    // Note: we don't rely on mFileTabs->currentIndex() since it may refer to a
    //       new file, which we will have been skipped above...

    QString crtFileNameOrUrl = QString();

    if (fileNames.count()) {
        QString crtFileName = mFileNames[mFileTabs->currentIndex()];

        if (fileNames.contains(crtFileName)) {
            crtFileNameOrUrl = fileManagerInstance->isRemote(crtFileName)?
                                   fileManagerInstance->url(crtFileName):
                                   crtFileName;
        }
    }

    pSettings->setValue(SettingsCurrentFileNameOrUrl, crtFileNameOrUrl);

    // Keep track of the selected modes and views, should there be no files the
    // next time we use OpenCOR

    pSettings->setValue(SettingsFileMode.arg(QString()),
                        ViewInterface::modeAsString(mModeTabIndexModes.value(mModeTabs->currentIndex())));

    for (int i = 0, iMax = mModeTabs->count(); i < iMax; ++i) {
        ViewInterface::Mode fileMode = mModeTabIndexModes.value(i);
        CentralWidgetMode *mode = mModes.value(fileMode);

        pSettings->setValue(SettingsFileModeView.arg(QString(), ViewInterface::modeAsString(fileMode)),
                            mode->viewPlugins()[mode->viewTabs()->currentIndex()]->name());
    }
}

//==============================================================================

void CentralWidget::settingsLoaded(const Plugins &pLoadedPlugins)
{
    // Determine which loaded plugins support a given interface

    foreach (Plugin *plugin, pLoadedPlugins) {
        if (qobject_cast<FileHandlingInterface *>(plugin->instance()))
            mLoadedFileHandlingPlugins << plugin;

        if (qobject_cast<FileTypeInterface *>(plugin->instance()))
            mLoadedFileTypePlugins << plugin;

        if (qobject_cast<GuiInterface *>(plugin->instance()))
            mLoadedGuiPlugins << plugin;

        if (qobject_cast<ViewInterface *>(plugin->instance()))
            mLoadedViewPlugins << plugin;
    }

    // Update our state now that our plugins  are fully ready

    mState = Idling;

    // Update the GUI

    updateGui();

    // Let our plugins know that our files have been opened
    // Note: the below is normally done as part of openFile(), but
    //       mLoadedFileHandlingPlugins is not yet set when openFile() gets
    //       called as part of OpenCOR's loading its settings, so we do it here
    //       instead...

    foreach (Plugin *plugin, mLoadedFileHandlingPlugins) {
        foreach (const QString &fileName, mFileNames)
            qobject_cast<FileHandlingInterface *>(plugin->instance())->fileOpened(fileName);
    }
}

//==============================================================================

void CentralWidget::retranslateUi()
{
    // Retranslate our modes tab bar

#ifdef ENABLE_SAMPLE_PLUGINS
    mModeTabs->setTabText(mModeModeTabIndexes.value(ViewInterface::SampleMode, -1),
                          tr("Sample"));
#endif
#ifdef ENABLE_TEST_PLUGINS
    mModeTabs->setTabText(mModeModeTabIndexes.value(ViewInterface::TestMode, -1),
                          tr("Test"));
#endif
    mModeTabs->setTabText(mModeModeTabIndexes.value(ViewInterface::EditingMode, -1),
                          tr("Editing"));
    mModeTabs->setTabText(mModeModeTabIndexes.value(ViewInterface::SimulationMode, -1),
                          tr("Simulation"));
    mModeTabs->setTabText(mModeModeTabIndexes.value(ViewInterface::AnalysisMode, -1),
                          tr("Analysis"));

    // Retranslate our mode views tab bar

    foreach (CentralWidgetMode *mode, mModes) {
        TabBarWidget *viewTabs = mode->viewTabs();

        for (int i = 0, iMax = viewTabs->count(); i < iMax; ++i)
            viewTabs->setTabText(i, qobject_cast<ViewInterface *>(mode->viewPlugins()[i]->instance())->viewName());
    }

    // Retranslate our modified settings, if needed

    updateModifiedSettings();

    // Retranslate our no view widget message

    updateNoViewMsg();

    // Retranslate our remote file dialog

    mRemoteFileDialog->setWindowTitle(tr("Open Remote File"));
    mRemoteFileDialogUrlLabel->setText(tr("URL:"));
}

//==============================================================================

void CentralWidget::setFileTypeInterfaces(const FileTypeInterfaces &pFileTypeInterfaces)
{
    // Set the file type interfaces

    mFileTypeInterfaces = pFileTypeInterfaces;
}

//==============================================================================

QString CentralWidget::currentFileName() const
{
    // Return the current file name, if any

    int fileTabIndex = mFileTabs->currentIndex();

    if (fileTabIndex != -1)
        return mFileNames[fileTabIndex];
    else
        return QString();
}

//==============================================================================

void CentralWidget::updateFileTab(const int &pIndex, const bool &pIconOnly)
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

        mFileTabs->setTabText(pIndex, tabText+(fileManagerInstance->isNewOrModified(fileName)?
                                                   "*":
                                                   QString()));
        mFileTabs->setTabToolTip(pIndex, fileIsNew?
                                             tabText:
                                             fileIsRemote?
                                                 url:
                                                 fileName);

        tabIcon = qobject_cast<ViewInterface *>(viewPlugin(pIndex)->instance())->fileTabIcon(mFileNames[pIndex]);
    }

    if (tabIcon.isNull()) {
        mFileTabs->setTabIcon(pIndex, fileIsRemote?
                                          InternetIcon:
                                          QFile::exists(fileName)?
                                              fileManagerInstance->isReadableAndWritable(fileName)?
                                                  NoIcon:
                                                  LockedIcon:
                                              NoIcon);
        // Note: we really want to call isReadableAndWritable() rather than
        //       isLocked() since no icon should be shown only if the file can
        //       be both readable and writable (see
        //       CorePlugin::filePermissionsChanged())...
    } else {
        mFileTabs->setTabIcon(pIndex, tabIcon);
    }
}

//==============================================================================

void CentralWidget::openFile(const QString &pFileName, const File::Type &pType,
                             const QString &pUrl)
{
    // Make sure that modes are available and that the file exists

    if (!mModeTabs->count() || !QFile::exists(pFileName)) {
        // Let the user know about us not being able to open the file, but only
        // if we are not starting OpenCOR, i.e. only if our main window is
        // visible

        if (mainWindow()->isVisible()) {
            warningMessageBox(pUrl.isEmpty()?
                                  tr("Open File"):
                                  tr("Open Remote File"),
                              tr("<strong>%1</strong> could not be opened.").arg(pFileName));
        }

        return;
    }

    // Check whether the file is already opened and, if so, select it and leave

    QString nativeFileName = nativeCanonicalFileName(pFileName);

    for (int i = 0, iMax = mFileNames.count(); i < iMax; ++i) {
        if (!mFileNames[i].compare(nativeFileName)) {
            mFileTabs->setCurrentIndex(i);

            return;
        }
    }

    // Register the file with our file manager

    FileManager::instance()->manage(nativeFileName, pType, pUrl);

    // Keep track of the mapping between the remote file and its local version,
    // if needed

    if (!pUrl.isEmpty())
        mRemoteLocalFileNames.insert(pUrl, nativeFileName);

    // Create a new tab, insert it just after the current tab, set the full name
    // of the file as the tool tip for the new tab, and make the new tab the
    // current one
    // Note #1: mFileNames is, for example, used to retrieve the name of a file,
    //          which we want to close (see CentralWidget::closeFile()), so we
    //          must make sure that the order of its contents matches that of
    //          the tabs...
    // Note #2: rather than using mFileNames, we could have used a tab's tool
    //          tip, but this makes it a bit tricky to handle with regards to
    //          connections and therefore with regards to some events triggering
    //          updateGui() to be called when the tool tip has not yet been
    //          assigned...

    int fileTabIndex = mFileTabs->currentIndex()+1;

    mFileNames.insert(fileTabIndex, nativeFileName);
    mFileTabs->insertTab(fileTabIndex, QString());

    updateFileTab(fileTabIndex);

    mFileTabs->setCurrentIndex(fileTabIndex);

    // Everything went fine, so let our plugins know that our file has been
    // opened
    // Note: this requires using mLoadedFileHandlingPlugins, but it will not be
    //       set when we come here following OpenCOR's loading of settings,
    //       hence we do something similar to what is done in
    //       settingsLoaded()...

    foreach (Plugin *plugin, mLoadedFileHandlingPlugins)
        qobject_cast<FileHandlingInterface *>(plugin->instance())->fileOpened(nativeFileName);
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

    QStringList files = getOpenFileNames(tr("Open File"),
                                         filters(mFileTypeInterfaces));

    // Open the file(s)

    openFiles(files);
}

//==============================================================================

void CentralWidget::openRemoteFile(const QString &pUrl,
                                   const bool &pShowWarning)
{
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

        openFile(fileNameOrUrl);

        return;
    }

    // Check whether the remote file is already opened and if so select it,
    // otherwise retrieve its contents

    QString fileName = mRemoteLocalFileNames.value(fileNameOrUrl);

    if (fileName.isEmpty()) {
        // The remote file isn't already opened, so download its contents

        QByteArray fileContents;
        QString errorMessage;

        if (readFileContentsFromUrlWithBusyWidget(fileNameOrUrl, fileContents, &errorMessage)) {
            // We were able to retrieve the contents of the remote file, so ask
            // our file manager to create a new remote file

            FileManager *fileManagerInstance = FileManager::instance();

#ifdef QT_DEBUG
            FileManager::Status createStatus =
#endif
            fileManagerInstance->create(fileNameOrUrl, fileContents);

#ifdef QT_DEBUG
            // Make sure that the file has indeed been created

            if (createStatus != FileManager::Created)
                qFatal("FATAL ERROR | %s:%d: the remote file was not created.", __FILE__, __LINE__);
#endif
        } else {
            // We were not able to retrieve the contents of the remote file, so
            // let the user know about it

            if (pShowWarning) {
                warningMessageBox(tr("Open Remote File"),
                                  tr("<strong>%1</strong> could not be opened (%2).").arg(fileNameOrUrl, formatMessage(errorMessage)));
            }
        }
    } else {
        openFile(fileName, File::Remote, fileNameOrUrl);
    }
}

//==============================================================================

void CentralWidget::openRemoteFileChanged()
{
    // Enable/disable the open button depending on whether we have some text for
    // the remote file

    mRemoteFileDialogButtonBox->button(QDialogButtonBox::Open)->setEnabled(!mRemoteFileDialogUrlValue->text().isEmpty());
}

//==============================================================================

void CentralWidget::doOpenRemoteFile()
{
    // Open the remote file

    mRemoteFileDialog->accept();

    openRemoteFile(mRemoteFileDialogUrlValue->text());
}

//==============================================================================

void CentralWidget::cancelOpenRemoteFile()
{
    // Cancel the opening of a remote file

    mRemoteFileDialog->reject();
}

//==============================================================================

void CentralWidget::openRemoteFile()
{
    // Ask for the URL of the remote file that is to be opened

    mRemoteFileDialogUrlValue->setText(QString());

    mSettings->beginGroup("RemoteFileDialog");
        mRemoteFileDialog->exec(mSettings);
    mSettings->endGroup();
}

//==============================================================================

void CentralWidget::reloadFile(const int &pIndex, const bool &pForce)
{
    // Ask our file manager to reload the file, but only if it isn't new and if
    // the user wants (in case the file has been modified)

    int realIndex = (pIndex == -1)?mFileTabs->currentIndex():pIndex;

    if (realIndex != -1) {
        FileManager *fileManagerInstance = FileManager::instance();
        QString fileName = mFileNames[realIndex];

        if (!fileManagerInstance->isNew(fileName)) {
            bool doReloadFile = true;

            if (!pForce && fileManagerInstance->isModified(fileName)) {
                // The current file is modified, so ask the user whether s/he
                // still wants to reload it

                doReloadFile = questionMessageBox(tr("File Modified"),
                                                  tr("<strong>%1</strong> has been modified. Do you still want to reload it?").arg(fileName)) == QMessageBox::Yes;
            }

            // Reload the file, if needed, and consider it as non-modified
            // anymore (in case it was before)
            // Note: by making the file non-modified anymore, we clearly assume
            //       that all the view plugins do their job properly and update
            //       their GUI...

            if (doReloadFile) {
                // Actually redownload the file, if it is a remote one, making
                // sure that our busy widget is show during that process (since
                // it may take some time)
                // Note: our busy widget will get hidden in updateGui() (which
                //       will be called from fileReloaded())...

                if (fileManagerInstance->isRemote(fileName)) {
                    QString url = fileManagerInstance->url(fileName);
                    QString fileContents;
                    QString errorMessage;

                    bool res = readFileContentsFromUrlWithBusyWidget(url, fileContents, &errorMessage);

                    if (res) {
                        writeFileContentsToFile(fileName, fileContents);

                        fileManagerInstance->reload(fileName, true);
                    } else {
                        warningMessageBox(tr("Reload Remote File"),
                                          tr("<strong>%1</strong> could not be reloaded (%2).").arg(url, formatMessage(errorMessage)));
                    }
                } else {
                    fileManagerInstance->reload(fileName, true);

                    fileManagerInstance->setModified(fileName, false);
                }
            }
        }
    }
}

//==============================================================================

void CentralWidget::duplicateFile()
{
    // Make sure that the file is neither new nor modified

    QString fileName = currentFileName();
    FileManager *fileManagerInstance = FileManager::instance();

    if (fileManagerInstance->isLocalNewOrModified(fileName))
        return;

    // Ask our file manager to duplicate the current file

#ifdef QT_DEBUG
    FileManager::Status duplicateStatus =
#endif
    fileManagerInstance->duplicate(fileName);

#ifdef QT_DEBUG
    // Make sure that the file has indeed been duplicated

    if (duplicateStatus != FileManager::Duplicated)
        qFatal("FATAL ERROR | %s:%d: '%s' did not get duplicated.", __FILE__, __LINE__, qPrintable(fileName));
#endif
}

//==============================================================================

void CentralWidget::toggleLockedFile()
{
    // Make sure that the file is neither new nor modified

    QString fileName = currentFileName();
    FileManager *fileManagerInstance = FileManager::instance();

    if (fileManagerInstance->isLocalNewOrModified(fileName))
        return;

    // Ask our file manager to toggle the locked state of the current file

    bool fileLocked = fileManagerInstance->isLocked(fileName);

    if (fileManagerInstance->setLocked(fileName, !fileLocked) == FileManager::LockedNotSet) {
        warningMessageBox(fileLocked?tr("Unlock File"):tr("Lock File"),
                          tr("<strong>%1</strong> could not be %2.").arg(fileName, fileLocked?tr("unlocked"):tr("locked")));
    }
}

//==============================================================================

bool CentralWidget::saveFile(const int &pIndex, const bool &pNeedNewFileName)
{
    // Make sure that we have a valid index

    if ((pIndex < 0) || (pIndex >= mFileNames.count()))
        return false;

    // Make sure that the view plugin supports saving

    Plugin *fileViewPlugin = viewPlugin(pIndex);
    FileHandlingInterface *fileHandlingInterface = qobject_cast<FileHandlingInterface *>(fileViewPlugin->instance());
    ViewInterface *viewInterface = qobject_cast<ViewInterface *>(fileViewPlugin->instance());

    if (!fileHandlingInterface) {
        warningMessageBox(tr("Save File"),
                          tr("The <strong>%1</strong> view does not support saving files.").arg(viewInterface->viewName()));

        return false;
    }

    // Make sure that we have a valid view for the file

    QString oldFileName = mFileNames[pIndex];

    if (!viewInterface->viewWidget(oldFileName)) {
        warningMessageBox(tr("Save File"),
                          tr("The <strong>%1</strong> view cannot save <strong>%2</strong>.").arg(viewInterface->viewName(), oldFileName));

        return false;
    }

    // Make sure that we have a file name

    FileManager *fileManagerInstance = FileManager::instance();
    QString newFileName = oldFileName;
    bool fileIsNew = fileManagerInstance->isNew(oldFileName);
    bool hasNewFileName = false;

    if (fileIsNew || pNeedNewFileName) {
        // Either we are dealing with a new file or we want to save the file
        // under a new name, so we ask the user for a file name based on the
        // MIME types supported by our current view

        QStringList supportedFilters = filters(mFileTypeInterfaces, viewInterface->viewMimeTypes(ViewInterface::SaveMimeTypeMode));
        QString firstSupportedFilter = supportedFilters.isEmpty()?QString():supportedFilters.first();

        newFileName = getSaveFileName(pNeedNewFileName?
                                          tr("Save File As"):
                                          tr("Save File"),
                                      fileIsNew?
                                          Core::newFileName(mFileTabs->tabToolTip(pIndex), viewInterface->viewDefaultFileExtension()):
                                          Core::newFileName(newFileName, tr("New"), true),
                                      supportedFilters, &firstSupportedFilter);

        // Make sure that a new file name was retrieved

        if (    newFileName.isEmpty()
            || !newFileName.compare(oldFileName)) {
            return false;
        } else {
            hasNewFileName = true;
        }
    }

    // Try to save the file in case it has been modified or it needs a new file
    // name (either as a result of a save as or because the file was new)

    bool fileIsModified = fileManagerInstance->isModified(oldFileName);

    if (fileIsModified || hasNewFileName) {
        if (fileIsNew || fileIsModified) {
            // The file is or has been modified, so ask the current view to save
            // it

            bool needFeedback = true;

            if (!fileHandlingInterface->saveFile(oldFileName, newFileName, needFeedback)) {
                if (needFeedback) {
                    warningMessageBox(tr("Save File"),
                                      tr("The <strong>%1</strong> view could not save <strong>%2</strong>.").arg(viewInterface->viewName(), newFileName));
                }

                return false;
            }
        } else {
            // The file hasn't been modified, so we just make a physical copy of
            // it
            // Note: there may already be a file, which name is that of the one
            //       we want to use, so remove it (if no such file exists, then
            //       nothing will happen, so we are fine)...

            QFile::remove(newFileName);

            if (!QFile::copy(oldFileName, newFileName)) {
                warningMessageBox(tr("Save File"),
                                  tr("<strong>%1</strong> could not be saved.").arg(newFileName));

                return false;
            }
        }

        // Delete the 'old' file, if it was a new one that got saved
        // Note: we delete the 'old' file before updating the file name in case
        //       someone handles the renaming of a file and checks whether the
        //       old file still exists (see CorePlugin::fileRenamed() and
        //       CorePlugin::fileClosed())...

        if (fileIsNew)
            QFile::remove(oldFileName);

        // Update its file name, if needed

        if (hasNewFileName) {
            // Ask our file manager to rename the file

#ifdef QT_DEBUG
            FileManager::Status renameStatus =
#endif
            fileManagerInstance->rename(oldFileName, newFileName);

#ifdef QT_DEBUG
            // Make sure that the file has indeed been renamed

            if (renameStatus != FileManager::Renamed)
                qFatal("FATAL ERROR | %s:%d: '%s' did not get renamed to '%s'.", __FILE__, __LINE__, qPrintable(oldFileName), qPrintable(newFileName));
#endif
        }

        // The file has been saved, so ask our file manager to 'save' it too

        fileManagerInstance->save(newFileName);

        return true;
    } else {
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

    if (mFileTabs->count()) {
        mFileTabs->setCurrentIndex(mFileTabs->currentIndex()?
                                       mFileTabs->currentIndex()-1:
                                       mFileTabs->count()-1);
    }
}

//==============================================================================

void CentralWidget::nextFile()
{
    // Select the next file

    if (mFileTabs->count()) {
        mFileTabs->setCurrentIndex((mFileTabs->currentIndex() == mFileTabs->count()-1)?
                                       0:
                                       mFileTabs->currentIndex()+1);
    }
}

//==============================================================================

bool CentralWidget::canCloseFile(const int &pIndex)
{
    FileManager *fileManagerInstance = FileManager::instance();
    QString fileName = mFileNames[pIndex];

    if (fileManagerInstance->isModified(fileName)) {
        // The current file is modified, so ask the user whether to save it or
        // ignore it

        switch (questionMessageBox(fileManagerInstance->isNew(fileName)?
                                       tr("New File"):
                                       tr("File Modified"),
                                   fileManagerInstance->isNew(fileName)?
                                       tr("<strong>%1</strong> is new. Do you want to save it before closing it?").arg(mFileTabs->tabToolTip(pIndex)):
                                       tr("<strong>%1</strong> has been modified. Do you want to save it before closing it?").arg(fileName))) {
        case QMessageBox::Yes:
            return saveFile(pIndex);
        case QMessageBox::No:
            return true;
        case QMessageBox::Cancel:
        default:
            return false;
        }
    } else {
        return true;
    }
}

//==============================================================================

bool CentralWidget::closeFile(const int &pIndex, const bool &pForceClosing)
{
    if (mState == UpdatingGui)
        // We are updating the GUI, so we can't close the file for now

        return false;

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

        if (!closingAllFiles && !pForceClosing && !canCloseFile(realIndex))
            return false;

        // Retrieve the file name

        QString fileName = mFileNames[realIndex];

        // Update some of our internals

        mFileNames.removeAt(realIndex);

        mFileModeTabIndexes.remove(fileName);
        mFileModeViewTabIndexes.remove(fileName);

        FileManager *fileManagerInstance = FileManager::instance();

        if (fileManagerInstance->isRemote(fileName))
            mRemoteLocalFileNames.remove(fileManagerInstance->url(fileName));

        // Remove the file tab

        mFileTabs->removeTab(realIndex);

        // Remove track of the views for the file

        for (int i = 0, iMax = mModeTabs->count(); i < iMax; ++i) {
            ViewInterface::Mode fileMode = mModeTabIndexModes.value(i);

            for (int j = 0, jMax = mModes.value(fileMode)->viewPlugins().count(); j < jMax; ++j)
                mViews.remove(viewKey(i, j, fileName));
        }

        // Ask our view plugins to remove the corresponding view for the file

        foreach (Plugin *plugin, mLoadedViewPlugins)
            qobject_cast<ViewInterface *>(plugin->instance())->removeViewWidget(fileName);

        // Let our plugins know about the file having just been closed

        foreach (Plugin *plugin, mLoadedFileHandlingPlugins)
            qobject_cast<FileHandlingInterface *>(plugin->instance())->fileClosed(fileName);

        // Unregister the file from our file manager

        fileManagerInstance->unmanage(fileName);

        // Update our modified settings

        updateModifiedSettings();

        return true;
    } else {
        return false;
    }
}

//==============================================================================

void CentralWidget::closeAllFiles(const bool &pForceClosing)
{
    // Check whether we can close all the files

    if (!pForceClosing && !canClose())
        return;

    // Close all the files

    while (closeFile(-2, pForceClosing))
        ;
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

    for (int i = 0, iMax = mFileTabs->count(); i < iMax; ++i) {
        if (!canCloseFile(i))
            return false;
    }

    return true;
}

//==============================================================================

bool CentralWidget::selectMode(const QString &pModeName)
{
    // Select the given mode

    ViewInterface::Mode mode = ViewInterface::modeFromString(pModeName);

    if (mode != ViewInterface::UnknownMode) {
        mModeTabs->setCurrentIndex(mModeModeTabIndexes.value(mode));

        return true;
    } else {
        return false;
    }
}

//==============================================================================

bool CentralWidget::selectView(const QString &pViewName)
{
    // Select the given view, after making sure that the corresponding mode has
    // been selected

    for (int i = 0, iMax = mLoadedViewPlugins.count(); i < iMax; ++i) {
        if (   !mLoadedViewPlugins[i]->name().compare(pViewName)
            &&  selectMode(ViewInterface::modeAsString(qobject_cast<ViewInterface *>(mLoadedViewPlugins[i]->instance())->viewMode()))) {
            CentralWidgetMode *mode = mModes.value(mModeTabIndexModes.value(mModeTabs->currentIndex()));
            Plugins viewPlugins = mode->viewPlugins();

            for (int j = 0, iMax = viewPlugins.count(); j < iMax; ++j) {
                if (!viewPlugins[j]->name().compare(pViewName)) {
                    mode->viewTabs()->setCurrentIndex(j);

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

void CentralWidget::dragEnterEvent(QDragEnterEvent *pEvent)
{
    // Accept the proposed action for the event, but only if at least one mode
    // is available and if we are dropping URIs or items from our file organiser

    if (   mModeTabs->count()
        && (pEvent->mimeData()->hasFormat(FileSystemMimeType))
        && (!pEvent->mimeData()->urls().isEmpty())) {
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

                if (QFile::exists(fileName))
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

Plugin * CentralWidget::viewPlugin(const int &pIndex) const
{
    // Return the view plugin associated with the file, which index is given

    if (pIndex != -1) {
        int modeTabIndex = mFileModeTabIndexes.value(mFileNames[pIndex]);
        CentralWidgetMode *mode = mModes.value(mModeTabIndexModes.value(modeTabIndex));

        return mode?mode->viewPlugins()[mFileModeViewTabIndexes.value(mFileNames[pIndex]).value(modeTabIndex)]:0;
    } else {
        return 0;
    }
}

//==============================================================================

Plugin * CentralWidget::viewPlugin(const QString &pFileName) const
{
    // Return the view plugin associated with the file, which file name is given

    for (int i = 0, iMax = mFileNames.count(); i < iMax; ++i) {
        if (!pFileName.compare(mFileNames[i]))
            return viewPlugin(i);
    }

    return 0;
}

//==============================================================================

QString CentralWidget::viewKey(const int &pMode, const int &pView,
                               const QString &pFileName)
{
    // Return the view key to be used with mViews for the given mode, view and
    // file name

    return QString("%1|%2|%3").arg(QString::number(pMode), QString::number(pView), pFileName);
}

//==============================================================================

void CentralWidget::updateGui()
{
    if (mState != Idling) {
        // We are doing something, so too risky to update the GUI during that
        // time (e.g. things may not be fully initialised)

        return;
    }

    // Update our state to reflect the fact that we are updating the GUI

    mState = UpdatingGui;

    // Determine whether we are here as a result of changing files, modes or
    // views

    bool changedFiles = sender() == mFileTabs;
    bool changedModes = sender() == mModeTabs;
    bool changedViews = false;

    foreach (CentralWidgetMode *mode, mModes) {
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
            if (changedModes)
                fileModeTabIndex = mModeTabs->currentIndex();
            else
                mModeTabs->setCurrentIndex(fileModeTabIndex);

            CentralWidgetMode *mode = mModes.value(mModeTabIndexModes.value(fileModeTabIndex));
            QMap<int, int> modeViewTabIndexes = mFileModeViewTabIndexes.value(fileName);

            mode->viewTabs()->setCurrentIndex(modeViewTabIndexes.value(fileModeTabIndex));
        } else if (!changedViews) {
            // We are opening a file, so determine the default views that we
            // should try and if there are none, then try the Raw Text view

            QStringList defaultViews = QStringList();

            foreach (Plugin *plugin, mLoadedFileTypePlugins) {
                FileTypeInterface *fileTypeInterface = qobject_cast<FileTypeInterface *>(plugin->instance());

                if (fileTypeInterface->isFile(fileName)) {
                    defaultViews = fileTypeInterface->fileTypeDefaultViews();

                    break;
                }
            }

            if (defaultViews.isEmpty())
                defaultViews << "RawTextView";

            foreach (const QString &defaultView, defaultViews) {
                if (selectView(defaultView))
                    break;
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

    // Show/hide the editing, simulation and analysis modes' corresponding views
    // tab, as needed, and retrieve the corresponding view plugin

    int fileModeTabIndex = mModeTabs->currentIndex();

#ifdef ENABLE_SAMPLE_PLUGINS
    mModes.value(ViewInterface::SampleMode)->viewTabs()->setVisible(fileModeTabIndex == mModeModeTabIndexes.value(ViewInterface::SampleMode));
#endif
#ifdef ENABLE_TEST_PLUGINS
    mModes.value(ViewInterface::TestMode)->viewTabs()->setVisible(fileModeTabIndex == mModeModeTabIndexes.value(ViewInterface::TestMode));
#endif
    mModes.value(ViewInterface::EditingMode)->viewTabs()->setVisible(fileModeTabIndex == mModeModeTabIndexes.value(ViewInterface::EditingMode));
    mModes.value(ViewInterface::SimulationMode)->viewTabs()->setVisible(fileModeTabIndex == mModeModeTabIndexes.value(ViewInterface::SimulationMode));
    mModes.value(ViewInterface::AnalysisMode)->viewTabs()->setVisible(fileModeTabIndex == mModeModeTabIndexes.value(ViewInterface::AnalysisMode));

    // Ask the GUI interface for the widget to use the current file (should
    // there be one)

    CentralWidgetMode *mode = mModes.value(mModeTabIndexModes.value(fileModeTabIndex));
    Plugin *viewPlugin = mode?mode->viewPlugins()[mode->viewTabs()->currentIndex()]:0;
    ViewInterface *viewInterface = viewPlugin?qobject_cast<ViewInterface *>(viewPlugin->instance()):0;
    QWidget *newView;

    if (fileName.isEmpty()) {
        newView = mLogoView;
    } else {
        // There is a current file, so retrieve its view

        QString fileViewKey = viewKey(fileModeTabIndex, mode->viewTabs()->currentIndex(), fileName);

        newView = viewInterface?viewInterface->viewWidget(fileName):0;

        if (newView) {
            // We could get a view for the current file, so keep track of it

            if (!mViews.value(fileViewKey))
                mViews.insert(fileViewKey, newView);
        } else {
            // The interface doesn't have a view for the current file, so use
            // our no-view widget instead and update its message

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

    ViewWidget *newViewWidget = dynamic_cast<ViewWidget *>(newView);

    if (newViewWidget) {
        connect(newViewWidget, SIGNAL(updateFileTabIcon(const QString &, const QString &, const QIcon &)),
                this, SLOT(updateFileTabIcon(const QString &, const QString &, const QIcon &)),
                Qt::UniqueConnection);

        updateStatusBarWidgets(newViewWidget->statusBarWidgets());

        newView = newViewWidget->widget(fileName);
    } else {
        updateStatusBarWidgets(QList<QWidget *>());
    }

    // Let people know that we are about to update the GUI

    emit guiUpdated(viewPlugin, fileName);

    // Replace the current view with the new one, if needed
    // Note: to do this as smoothly as possible, we temporarily hide the status
    //       bar. Indeed, not do this will result in some awful flickering when
    //       switching from one file to another with the mouse over a
    //       button-like widget and the status bar visible (see issues #405 and
    //       #1027)...

    if (mContents->currentWidget() != newView) {
        bool statusBarVisible = mainWindow()->statusBar()->isVisible();

        mainWindow()->statusBar()->setVisible(false);
            mContents->removeWidget(mContents->currentWidget());
            mContents->addWidget(newView);
        mainWindow()->statusBar()->setVisible(statusBarVisible);
    }

    // Give the focus to the new view after first checking that it has a focused
    // widget

    if (newView->focusWidget()) {
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

    // Update our modified settings

    updateModifiedSettings();

    // Let people know whether there is at least one view, as well as whether we
    // can save as and there is/are at least one/two file/s

    emit atLeastOneView(mModeTabs->currentIndex() != -1);

    emit canSaveAs(   mFileTabs->count() && (newView != mNoViewMsg)
                   && !FileManager::instance()->isRemote(fileName)
                   && viewInterface);

    emit atLeastOneFile(mFileTabs->count());
    emit atLeastTwoFiles(mFileTabs->count() > 1);

    mState = Idling;
}

//==============================================================================

TabBarWidget *CentralWidget::newTabBarWidget(const QTabBar::Shape &pShape,
                                             const bool &pMovable,
                                             const bool &pTabsClosable)
{
    // Create and return a tab bar

    TabBarWidget *res = new TabBarWidget(this);

    res->setExpanding(false);
    // Note: if the above property is not enabled and many files are opened,
    //       then the central widget will widen, reducing the width of any
    //       docked window, which is clearly not what we want...
    res->setFocusPolicy(Qt::NoFocus);
    res->setMovable(pMovable);
    res->setShape(pShape);
    res->setTabsClosable(pTabsClosable);
    res->setUsesScrollButtons(true);
    // Note: the above property is style dependent and it happens that it's not
    //       enabled on macOS, so set it in all cases, even though it's already
    //       set on Windows and Linux (but better be safe than sorry)...

    return res;
}

//==============================================================================

void CentralWidget::updateNoViewMsg()
{
    // Customise, if possible, our no view widget so that it shows a relevant
    // warning message

    Plugin *fileViewPlugin = viewPlugin(mFileTabs->currentIndex());

    if (fileViewPlugin)
        mNoViewMsg->setMessage(tr("The <strong>%1</strong> view does not support this type of file...").arg(qobject_cast<ViewInterface *>(fileViewPlugin->instance())->viewName()));
}

//==============================================================================

void CentralWidget::fileChanged(const QString &pFileName,
                                const bool &pFileChanged,
                                const bool &pDependenciesChanged)
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
                                       tr("<strong>%1</strong> and one or several of its dependencies has been modified. Do you want to reload it?").arg(pFileName):
                                       tr("<strong>%1</strong> has been modified. Do you want to reload it?").arg(pFileName):
                                   tr("<strong>%1</strong> has had one or several of its dependencies modified. Do you want to reload it?").arg(pFileName)) == QMessageBox::Yes) {
            // The user wants to reload the file, so find it and reload it

            for (int i = 0, iMax = mFileNames.count(); i < iMax; ++i) {
                if (!mFileNames[i].compare(pFileName)) {
                    reloadFile(i, true);

                    break;
                }
            }
        } else {
            // The user doesn't want to reload the file, so check whether the
            // file and/or one or several of its dependencies has changed and,
            // if so, keep track of that/those fact/s

            if (fileChanged)
                fileManagerInstance->setModified(pFileName, true);

            if (pDependenciesChanged)
                fileManagerInstance->setDependenciesModified(pFileName, true);
        }

        fileManagerInstance->setCheckFilesEnabled(true);
    }
}

//==============================================================================

void CentralWidget::fileDeleted(const QString &pFileName)
{
    // The given file doesn't exist anymore, so ask the user whether to close it

    if (questionMessageBox(tr("File Deleted"),
                           tr("<strong>%1</strong> does not exist anymore. Do you want to close it?").arg(pFileName)) == QMessageBox::Yes) {
        // The user wants to close the file

        for (int i = 0, iMax = mFileNames.count(); i < iMax; ++i) {
            if (!mFileNames[i].compare(pFileName)) {
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

        if (fileManagerInstance->isNewOrModified(mFileNames[i]))
            ++nbOfModifiedFiles;
    }

    // Reset the enabled state and tool tip of our Mode tabs and of all our View
    // tabs

    mModeTabs->setEnabled(true);
    mModeTabs->setToolTip(QString());

    foreach (CentralWidgetMode *mode, mModes) {
        TabBarWidget *viewTabs = mode->viewTabs();

        viewTabs->setEnabled(true);
        viewTabs->setToolTip(QString());
    }

    // Enable/disable the Mode tabs and the current mode's View tabs, in case
    // the current file has been modified

    QString fileName = mFileTabs->count()?
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
    emit canSaveAll(nbOfModifiedFiles);
}

//==============================================================================

void CentralWidget::filePermissionsChanged(const QString &pFileName)
{
    // Update the tab text for the given file

    for (int i = 0, iMax = mFileNames.count(); i < iMax; ++i) {
        if (!pFileName.compare(mFileNames[i])) {
            updateFileTab(i);

            break;
        }
    }

    // Let our plugins know about the file having had its permissions changed

    foreach (Plugin *plugin, mLoadedFileHandlingPlugins)
        qobject_cast<FileHandlingInterface *>(plugin->instance())->filePermissionsChanged(pFileName);
}

//==============================================================================

void CentralWidget::fileModified(const QString &pFileName)
{
    // Let our plugins know about the file having been modified

    foreach (Plugin *plugin, mLoadedFileHandlingPlugins)
        qobject_cast<FileHandlingInterface *>(plugin->instance())->fileModified(pFileName);
}

//==============================================================================

void CentralWidget::fileReloaded(const QString &pFileName,
                                 const bool &pFileChanged,
                                 const bool &pExcludeFileViewPlugin)
{
    // Let all our plugins, but the current one (if requested), know about the
    // file having been reloaded
    // Note: in the case of the current plugin, we don't need and don't want
    //       our current plugin to reload it if it has just saved it (see
    //       fileSaved(); indeed, it may mess up our current view; e.g. the
    //       caret of a QScintilla-based view will get moved back to its
    //       original position)...

    Plugin *fileViewPlugin = viewPlugin(pFileName);

    foreach (Plugin *plugin, mLoadedFileHandlingPlugins) {
        if (   !pExcludeFileViewPlugin
            ||  (pExcludeFileViewPlugin && (plugin != fileViewPlugin))) {
            qobject_cast<FileHandlingInterface *>(plugin->instance())->fileReloaded(pFileName, pFileChanged);
        }
    }

    // Now, because of the way some of our views may reload a file (see, for
    // example, CoreEditingPlugin::fileReloaded()), we need to tell them to
    // update their GUI

    foreach (Plugin *plugin, mLoadedGuiPlugins) {
        if (   !pExcludeFileViewPlugin
            ||  (pExcludeFileViewPlugin && (plugin != fileViewPlugin))) {
            qobject_cast<GuiInterface *>(plugin->instance())->updateGui(fileViewPlugin, pFileName);
        }
    }

    // Similarly, we need to update our GUI

    updateGui();

    // Update our modified settings

    updateModifiedSettings();
}

//==============================================================================

void CentralWidget::fileCreated(const QString &pFileName, const QString &pUrl)
{
    // A file got created, so open it as new if no URL is provided or remote
    // otherwise

    openFile(pFileName, pUrl.isEmpty()?File::New:File::Remote, pUrl);
}

//==============================================================================

void CentralWidget::fileDuplicated(const QString &pFileName)
{
    // A file got duplicated, so open it as new

    openFile(pFileName, File::New);
}

//==============================================================================

void CentralWidget::fileRenamed(const QString &pOldFileName,
                                const QString &pNewFileName)
{
    // A file has been renamed, so update various things

    for (int i = 0, iMax = mFileNames.count(); i < iMax; ++i) {
        if (!mFileNames[i].compare(pOldFileName)) {
            // Update our internal copy of the file name

            mFileNames[i] = pNewFileName;

            // Update the model and view raw names

            mFileModeTabIndexes.insert(pNewFileName, mFileModeTabIndexes.value(pOldFileName));
            mFileModeViewTabIndexes.insert(pNewFileName, mFileModeViewTabIndexes.value(pOldFileName));

            mFileModeTabIndexes.remove(pOldFileName);
            mFileModeViewTabIndexes.remove(pOldFileName);

            // Update the file tab

            mFileTabs->setTabText(i, QFileInfo(pNewFileName).fileName());
            mFileTabs->setTabToolTip(i, pNewFileName);

            // Let our plugins know about a file having been renamed

            foreach (Plugin *plugin, mLoadedFileHandlingPlugins)
                qobject_cast<FileHandlingInterface *>(plugin->instance())->fileRenamed(pOldFileName, pNewFileName);

            break;
        }
    }
}

//==============================================================================

void CentralWidget::fileSaved(const QString &pFileName)
{
    // A file has been saved, so we want all the plugins, but the current one,
    // to reload it

    fileReloaded(pFileName, true, true);
}

//==============================================================================

void CentralWidget::updateFileTabIcon(const QString &pViewName,
                                      const QString &pFileName,
                                      const QIcon &pIcon)
{
    // Update the requested file tab icon, but only if the view plugin (from
    // which the signal was emitted) is the one currently active

    if (!pViewName.compare(qobject_cast<ViewInterface *>(viewPlugin(pFileName)->instance())->viewName())) {
        // The view from which the signal was emitted is the currently active
        // one, so we can try to handle its signal

        int index = mFileNames.indexOf(pFileName);

        if (index != -1) {
            if (pIcon.isNull())
                updateFileTab(index, true);
            else
                mFileTabs->setTabIcon(index, pIcon);
        }
    }
}

//==============================================================================

void CentralWidget::updateFileTabIcons()
{
    // Update all the file tab icons

    for (int i = 0, iMax = mFileTabs->count(); i < iMax; ++i) {
        QIcon tabIcon = qobject_cast<ViewInterface *>(viewPlugin(i)->instance())->fileTabIcon(mFileNames[i]);

        if (tabIcon.isNull())
            updateFileTab(i, true);
        else
            mFileTabs->setTabIcon(i, tabIcon);
    }
}

//==============================================================================

void CentralWidget::updateStatusBarWidgets(QList<QWidget *> pWidgets)
{
    // Remove (hide) our existing status bar widgets

    static QList<QWidget *> statusBarWidgets = QList<QWidget *>();

    foreach (QWidget *statusBarWidget, statusBarWidgets)
        mainWindow()->statusBar()->removeWidget(statusBarWidget);

    // Add and show the given status bar widgets, and keep track of them

    statusBarWidgets.clear();

    foreach (QWidget *widget, pWidgets) {
        mainWindow()->statusBar()->addWidget(widget);

        widget->show();

        statusBarWidgets << widget;
    }
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
