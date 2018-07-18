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
#include "tabbarwidget.h"
#include "usermessagewidget.h"
#include "viewinterface.h"
#include "viewwidget.h"

//==============================================================================

#include <QAction>
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
#include <QMenu>
#include <QMenuBar>
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
    mFileNames(QStringList()),
    mModes(QMap<ViewInterface::Mode, CentralWidgetMode *>()),
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

    mModes.insert(ViewInterface::EditingMode, new CentralWidgetMode(this));
    mModes.insert(ViewInterface::SimulationMode, new CentralWidgetMode(this));
    mModes.insert(ViewInterface::AnalysisMode, new CentralWidgetMode(this));
#ifdef ENABLE_SAMPLE_PLUGINS
    mModes.insert(ViewInterface::SampleMode, new CentralWidgetMode(this));
#endif
#ifdef ENABLE_TEST_PLUGINS
    mModes.insert(ViewInterface::TestMode, new CentralWidgetMode(this));
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

    foreach (CentralWidgetMode *mode, mModes) {
        connect(mode->viewTabs(), &TabBarWidget::currentChanged,
                this, &CentralWidget::updateGui);
        connect(mode->viewTabs(), &TabBarWidget::currentChanged,
                this, &CentralWidget::updateFileTabIcons);
    }

    // Create our remote file dialog
//---OPENCOR--- THE ORIGINAL PLAN WAS TO HAVE A REGULAR EXPRESSION TO VALIDATE A
//              URL, BUT IT LOOKS LIKE THERE MIGHT BE AN ISSUE WITH
//              QRegularExpressionValidator, SO WE SIMPLY ALLOW FREE TEXT FOR
//              NOW (SEE https://bugreports.qt.io/browse/QTBUG-38034)...

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

    connect(mRemoteFileDialogUrlValue, &QLineEdit::textChanged,
            this, &CentralWidget::openRemoteFileChanged);

    connect(mRemoteFileDialogButtonBox, &QDialogButtonBox::accepted,
            this, &CentralWidget::doOpenRemoteFile);
    connect(mRemoteFileDialogButtonBox, &QDialogButtonBox::rejected,
            this, &CentralWidget::cancelOpenRemoteFile);
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

            QString viewPluginName = pSettings->value(SettingsFileModeView.arg(fileNameOrUrl)
                                                                          .arg(ViewInterface::modeAsString(fileMode))).toString();
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
                              fileManagerInstance->fileName(crtFileNameOrUrl):
                              crtFileNameOrUrl;

    if (mFileNames.contains(crtFileName)) {
        openFile(crtFileName);
    } else {
        // The previously selected file doesn't exist anymore, so select the
        // first file (otherwise the last file will be selected)

        setTabBarCurrentIndex(mFileTabs, 0);
    }

    // Retrieve the seleted modes and views, in case there are no files

    if (mFileNames.isEmpty()) {
        ViewInterface::Mode fileMode = ViewInterface::modeFromString(pSettings->value(SettingsFileMode.arg(QString())).toString());

        if (fileMode != ViewInterface::UnknownMode)
            setTabBarCurrentIndex(mModeTabs, mModeModeTabIndexes.value(fileMode));

        for (int i = 0, iMax = mModeTabs->count(); i < iMax; ++i) {
            fileMode = mModeTabIndexModes.value(i);

            CentralWidgetMode *mode = mModes.value(fileMode);
            QString viewPluginName = pSettings->value(SettingsFileModeView.arg(QString())
                                                                          .arg(ViewInterface::modeAsString(fileMode))).toString();
            Plugins viewPlugins = mode->viewPlugins();

            for (int j = 0, jMax = viewPlugins.count(); j < jMax; ++j) {
                if (!viewPluginName.compare(viewPlugins[j]->name())) {
                    setTabBarCurrentIndex(mode->viewTabs(), j);

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
    static const QString SettingsFileModeViewHeader = SettingsFileModeView.arg(QString())
                                                                          .arg(QString());

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

            pSettings->setValue(SettingsFileModeView.arg(fileNameOrUrl)
                                                    .arg(ViewInterface::modeAsString(fileMode)),
                                mModes.value(fileMode)->viewPlugins()[modeViewTabIndexes.value(i)]->name());
        }
    }

    // Keep track of the currently selected file
    // Note: we don't rely on mFileTabs->currentIndex() since it may refer to a
    //       new file, which we will have been skipped above...

    QString crtFileNameOrUrl = QString();

    if (fileNames.count()) {
        QString fileName = mFileNames[mFileTabs->currentIndex()];

        if (fileNames.contains(fileName)) {
            crtFileNameOrUrl = fileManagerInstance->isRemote(fileName)?
                                   fileManagerInstance->url(fileName):
                                   fileName;
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

        pSettings->setValue(SettingsFileModeView.arg(QString())
                                                .arg(ViewInterface::modeAsString(fileMode)),
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

    mModeTabs->setTabText(mModeModeTabIndexes.value(ViewInterface::EditingMode, -1),
                          tr("Editing"));
    mModeTabs->setTabText(mModeModeTabIndexes.value(ViewInterface::SimulationMode, -1),
                          tr("Simulation"));
    mModeTabs->setTabText(mModeModeTabIndexes.value(ViewInterface::AnalysisMode, -1),
                          tr("Analysis"));
#ifdef ENABLE_SAMPLE_PLUGINS
    mModeTabs->setTabText(mModeModeTabIndexes.value(ViewInterface::SampleMode, -1),
                          tr("Sample"));
#endif
#ifdef ENABLE_TEST_PLUGINS
    mModeTabs->setTabText(mModeModeTabIndexes.value(ViewInterface::TestMode, -1),
                          tr("Test"));
#endif

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

        if (fileManagerInstance->isNewOrModified(fileName))
            tabText += "*";

        if (tabText.compare(mFileTabs->tabText(pIndex)))
            mFileTabs->setTabText(pIndex, tabText);

        if (tabToolTip.compare(mFileTabs->tabToolTip(pIndex)))
            mFileTabs->setTabToolTip(pIndex, tabToolTip);

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

    if (QVariant(tabIcon) != QVariant(mFileTabs->tabIcon(pIndex)))
        mFileTabs->setTabIcon(pIndex, tabIcon);
}

//==============================================================================

void CentralWidget::openFile(const QString &pFileName, File::Type pType,
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
                              tr("<strong>%1</strong> could not be opened.").arg(pUrl.isEmpty()?
                                                                                     QDir::toNativeSeparators(pFileName):
                                                                                     pFileName));
        }

        return;
    }

    // Check whether the file is already opened and, if so, select it and leave

    QString fileName = canonicalFileName(pFileName);

    for (int i = 0, iMax = mFileNames.count(); i < iMax; ++i) {
        if (!mFileNames[i].compare(fileName)) {
            setTabBarCurrentIndex(mFileTabs, i);

            return;
        }
    }

    // Register the file with our file manager

    FileManager::instance()->manage(fileName, pType, pUrl);

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

    foreach (Plugin *plugin, mLoadedFileHandlingPlugins)
        qobject_cast<FileHandlingInterface *>(plugin->instance())->fileOpened(fileName);
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

    QStringList fileNames = getOpenFileNames(tr("Open File"),
                                             filters(fileTypeInterfaces()));

    // Open the file(s)

    openFiles(fileNames);
}

//==============================================================================

void CentralWidget::openRemoteFile(const QString &pUrl, bool pShowWarning)
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

#ifdef QT_DEBUG
            FileManager::Status createStatus =
#endif
            fileManagerInstance->create(fileNameOrUrl, fileContents);

#ifdef QT_DEBUG
            // Make sure that the file has indeed been created

            if (createStatus != FileManager::Created)
                qFatal("FATAL ERROR | %s:%d: '%s' did not get created.", __FILE__, __LINE__, qPrintable(fileNameOrUrl));
#endif
        } else {
            // We were not able to retrieve the contents of the remote file, so
            // let the user know about it, after having hidden our busy widget

            if (pShowWarning) {
                hideBusyWidget();

                warningMessageBox(tr("Open Remote File"),
                                  tr("<strong>%1</strong> could not be opened (%2).").arg(fileNameOrUrl)
                                                                                     .arg(formatMessage(errorMessage)));
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
                                          tr("<strong>%1</strong> could not be reloaded (%2).").arg(url)
                                                                                               .arg(formatMessage(errorMessage)));
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
        warningMessageBox(fileLocked?
                              tr("Unlock File"):
                              tr("Lock File"),
                          tr("<strong>%1</strong> could not be %2.").arg(QDir::toNativeSeparators(fileName))
                                                                    .arg(fileLocked?
                                                                             tr("unlocked"):
                                                                             tr("locked")));
    }
}

//==============================================================================

bool CentralWidget::saveFile(int pIndex, bool pNeedNewFileName)
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
                          tr("The <strong>%1</strong> view cannot save <strong>%2</strong>.").arg(viewInterface->viewName())
                                                                                             .arg(oldFileName));

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

    // Try to save the file

    bool fileIsModified = fileManagerInstance->isModified(oldFileName);

    if (fileIsModified || hasNewFileName) {
        bool needFeedback = true;

        if (!fileHandlingInterface->saveFile(oldFileName, newFileName, needFeedback)) {
            if (needFeedback) {
                warningMessageBox(tr("Save File"),
                                  tr("The <strong>%1</strong> view could not save <strong>%2</strong>.").arg(viewInterface->viewName())
                                                                                                        .arg(QDir::toNativeSeparators(newFileName)));
            }

            return false;
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
        setTabBarCurrentIndex(mFileTabs, mFileTabs->currentIndex()?
                                             mFileTabs->currentIndex()-1:
                                             mFileTabs->count()-1);
    }
}

//==============================================================================

void CentralWidget::nextFile()
{
    // Select the next file

    if (mFileTabs->count()) {
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

        switch (questionMessageBox(fileManagerInstance->isNew(fileName)?
                                       tr("New File"):
                                       tr("File Modified"),
                                   fileManagerInstance->isNew(fileName)?
                                       tr("<strong>%1</strong> is new. Do you want to save it before closing it?").arg(QDir::toNativeSeparators(mFileTabs->tabToolTip(pIndex))):
                                       tr("<strong>%1</strong> has been modified. Do you want to save it before closing it?").arg(QDir::toNativeSeparators(fileName)))) {
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

bool CentralWidget::closeFile(int pIndex, bool pForceClosing)
{
    // Make sure that we are not updating the GUI

    if (mState == UpdatingGui)
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

        FileManager::instance()->unmanage(fileName);

        // Update our modified settings

        updateModifiedSettings();

        return true;
    } else {
        return false;
    }
}

//==============================================================================

bool CentralWidget::closeFile(int pIndex)
{
    // Close the file which index is given

    return closeFile(pIndex, false);
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

    if (!pForceClosing && !canClose())
        return;

    // Close all the files

    while (closeFile(-2, pForceClosing))
        ;
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
        setTabBarCurrentIndex(mModeTabs, mModeModeTabIndexes.value(mode));

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

Plugin * CentralWidget::viewPlugin(int pIndex) const
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

QString CentralWidget::viewKey(int pMode, int pView, const QString &pFileName)
{
    // Return the view key to be used with mViews for the given mode, view and
    // file name

    return QString("%1|%2|%3").arg(pMode)
                              .arg(pView)
                              .arg(pFileName);
}

//==============================================================================

void CentralWidget::fileReloadedOrSaved(const QString &pFileName,
                                        bool pFileReloaded)
{
    // Let all our plugins know about the file having been reloaded/saved

    foreach (Plugin *plugin, mLoadedFileHandlingPlugins) {
        if (pFileReloaded)
            qobject_cast<FileHandlingInterface *>(plugin->instance())->fileReloaded(pFileName);
        else
            qobject_cast<FileHandlingInterface *>(plugin->instance())->fileSaved(pFileName);
    }

    // Now, because of the way some of our views may reload/save a file (see
    // EditingViewPlugin::fileReloaded() for example), we need to tell them to
    // update their GUI

    foreach (Plugin *plugin, mLoadedGuiPlugins)
        qobject_cast<GuiInterface *>(plugin->instance())->updateGui(viewPlugin(pFileName), pFileName);

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

    if (mState == UpdatingGui) {
        disconnect(pTabBar, &TabBarWidget::currentChanged,
                   this, &CentralWidget::updateGui);
    }

    pTabBar->setCurrentIndex(pIndex);

    if (mState == UpdatingGui) {
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

    foreach (QAction *action, pActions) {
        QMenu *actionMenu = action->menu();

        if (actionMenu) {
            QList<QAction *> actionMenuActions = actionMenu->actions();

            showEnableActions(actionMenuActions);

            bool showEnable = false;

            foreach (QAction *actionMenuAction, actionMenuActions) {
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
    TabBarWidget *tabBar = qobject_cast<TabBarWidget *>(sender());

    if (mState != Idling) {
        // We are doing something, so it's too risky to update the GUI for now
        // (e.g. things may not be fully initialised), so revert to our old tab
        // index, if possible

        if (tabBar)
            setTabBarCurrentIndex(tabBar, tabBar->oldIndex());

        return;
    }

    // Update our state to reflect the fact that we are updating the GUI

    mState = UpdatingGui;

    // Keep track of our future old tab index, if possible

    if (tabBar)
        tabBar->setOldIndex(tabBar->currentIndex());

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
                setTabBarCurrentIndex(mModeTabs, fileModeTabIndex);

            setTabBarCurrentIndex(mModes.value(mModeTabIndexModes.value(fileModeTabIndex))->viewTabs(),
                                  mFileModeViewTabIndexes.value(fileName).value(fileModeTabIndex));
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

    // Show/hide the different modes' corresponding views tab, as needed, and
    // retrieve the corresponding view plugin
    // Note: to prevent some GUI glitches on Windows and Linux, we must first
    //       make sure that all the views tabs are hidden before we can actually
    //       show the one we are after...

    int fileModeTabIndex = mModeTabs->currentIndex();

    mModes.value(ViewInterface::EditingMode)->viewTabs()->hide();
    mModes.value(ViewInterface::SimulationMode)->viewTabs()->hide();
    mModes.value(ViewInterface::AnalysisMode)->viewTabs()->hide();
#ifdef ENABLE_SAMPLE_PLUGINS
    mModes.value(ViewInterface::SampleMode)->viewTabs()->hide();
#endif
#ifdef ENABLE_TEST_PLUGINS
    mModes.value(ViewInterface::TestMode)->viewTabs()->hide();
#endif

    if (fileModeTabIndex == mModeModeTabIndexes.value(ViewInterface::EditingMode))
        mModes.value(ViewInterface::EditingMode)->viewTabs()->show();
    else if (fileModeTabIndex == mModeModeTabIndexes.value(ViewInterface::SimulationMode))
        mModes.value(ViewInterface::SimulationMode)->viewTabs()->show();
    else if (fileModeTabIndex == mModeModeTabIndexes.value(ViewInterface::AnalysisMode))
        mModes.value(ViewInterface::AnalysisMode)->viewTabs()->show();
#ifdef ENABLE_SAMPLE_PLUGINS
    else if (fileModeTabIndex == mModeModeTabIndexes.value(ViewInterface::SampleMode))
        mModes.value(ViewInterface::SampleMode)->viewTabs()->show();
#endif
#ifdef ENABLE_TEST_PLUGINS
    else if (fileModeTabIndex == mModeModeTabIndexes.value(ViewInterface::TestMode))
        mModes.value(ViewInterface::TestMode)->viewTabs()->show();
#endif

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
#elif defined(Q_OS_MAC)
        bool hideShowStatusBar =     mainWindow()->statusBar()->isVisible()
                                 && !qobject_cast<QTabBar *>(childAt(mapFromGlobal(QCursor::pos())));

        if (hideShowStatusBar)
            mainWindow()->statusBar()->hide();
#else
    #error Unsupported platform
#endif

        mContents->removeWidget(mContents->currentWidget());
        mContents->addWidget(newView);

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
        mContents->setUpdatesEnabled(true);
#elif defined(Q_OS_MAC)
        if (hideShowStatusBar)
            mainWindow()->statusBar()->show();
#else
    #error Unsupported platform
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

    foreach (Plugin *plugin, mLoadedGuiPlugins)
        qobject_cast<GuiInterface *>(plugin->instance())->updateGui(viewPlugin, fileName);

    // Go through our different menus and show/hide them, depending on whether
    // they have visible items

    showEnableActions(mainWindow()->menuBar()->actions());

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

TabBarWidget *CentralWidget::newTabBarWidget(QTabBar::Shape pShape,
                                             bool pFileTabs)
{
    // Create and return a tab bar

    TabBarWidget *res = new TabBarWidget(this);

    res->setExpanding(false);
    // Note: if the above property is not enabled and many files are opened,
    //       then the central widget will widen, reducing the width of any
    //       docked window, which is clearly not what we want...
    res->setFocusPolicy(Qt::NoFocus);
    res->setMovable(pFileTabs);
    res->setShape(pShape);
    res->setTabsClosable(pFileTabs);
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

    int fileTabIndex = mFileTabs->currentIndex();
    Plugin *fileViewPlugin = viewPlugin(fileTabIndex);

    if (fileViewPlugin) {
        if (Core::FileManager::instance()->isNew(mFileNames[fileTabIndex]))
            mNoViewMsg->setMessage(tr("The <strong>%1</strong> view does not support new files...").arg(qobject_cast<ViewInterface *>(fileViewPlugin->instance())->viewName()));
        else
            mNoViewMsg->setMessage(tr("The <strong>%1</strong> view does not support this type of file...").arg(qobject_cast<ViewInterface *>(fileViewPlugin->instance())->viewName()));
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
                           tr("<strong>%1</strong> does not exist anymore. Do you want to close it?").arg(QDir::toNativeSeparators(pFileName))) == QMessageBox::Yes) {
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
            mFileTabs->setTabToolTip(i, QDir::toNativeSeparators(pNewFileName));

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

    Plugin *currentViewPlugin = viewPlugin(pFileName);

    if (currentViewPlugin) {
        if (!pViewName.compare(qobject_cast<ViewInterface *>(currentViewPlugin->instance())->viewName())) {
            // The view from which the signal was emitted is the one currently
            // active, so we can try to handle its signal

            int index = mFileNames.indexOf(pFileName);

            if (index != -1) {
                if (pIcon.isNull())
                    updateFileTab(index, true);
                else
                    mFileTabs->setTabIcon(index, pIcon);
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
