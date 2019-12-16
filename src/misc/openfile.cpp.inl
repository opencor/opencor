#ifdef GUI_SUPPORT
QString CentralWidget::openFile(const QString &pFileName, File::Type pType,
                                const QString &pUrl, bool pShowWarning)
#else
QString cliOpenFile(const QString &pFileName, File::Type pType,
                    const QString &pUrl)
#endif
{
    // Make sure that modes are available and that the file exists

#ifdef GUI_SUPPORT
    if (    (mModeTabs->count() == 0)
        || !QFileInfo(pFileName).isFile()
        || !QFile::exists(pFileName)) {
        // Let the user know about us not being able to open the file, but only
        // if we are not starting OpenCOR, i.e. only if our main window is
        // visible

        if (pShowWarning && mainWindow()->isVisible()) {
            warningMessageBox(pUrl.isEmpty()?
                                  QObject::tr("Open File"):
                                  QObject::tr("Open Remote File"),
                              QObject::tr("<strong>%1</strong> could not be opened.").arg(pUrl.isEmpty()?
                                                                                              QDir::toNativeSeparators(pFileName):
                                                                                              pFileName));
        }

        return QObject::tr("'%1' could not be opened.").arg(pUrl.isEmpty()?
                                                                QDir::toNativeSeparators(pFileName):
                                                                pFileName);
    }
#else
    if (!QFileInfo(pFileName).isFile() || !QFile::exists(pFileName)) {
        return QObject::tr("'%1' could not be opened.").arg(pUrl.isEmpty()?
                                                                QDir::toNativeSeparators(pFileName):
                                                                pFileName);
    }
#endif

    // Check whether the file is already opened and, if so, select it and leave

    QString fileName = canonicalFileName(pFileName);

#ifdef GUI_SUPPORT
    for (int i = 0, iMax = mFileNames.count(); i < iMax; ++i) {
        if (mFileNames[i] == fileName) {
            setTabBarCurrentIndex(mFileTabs, i);

            return {};
        }
    }
#endif

    // Register the file with our file manager and get its status

    FileManager::Status fileStatus = FileManager::instance()->manage(fileName, pType, pUrl);

    if (fileStatus == FileManager::Status::DoesNotExist) {
        return QObject::tr("'%1' could not be opened.").arg(pUrl.isEmpty()?
                                                                QDir::toNativeSeparators(pFileName):
                                                                pFileName);
        // Note: we should never reach this point (since we test for the file
        //       existence above), but better be safe than sorry...
    }

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

#ifdef GUI_SUPPORT
    int fileTabIndex = mFileTabs->currentIndex()+1;

    mFileNames.insert(fileTabIndex, fileName);
    mFileTabs->insertTab(fileTabIndex, QString());

    updateFileTab(fileTabIndex);

    setTabBarCurrentIndex(mFileTabs, fileTabIndex);
#endif

    // Everything went fine, so let our plugins know that our file has been
    // opened
    // Note: this requires using mLoadedFileHandlingPlugins, but it will not be
    //       set when we come here following OpenCOR's loading of settings,
    //       hence we do something similar to what is done in
    //       settingsLoaded()...

#ifdef GUI_SUPPORT
    for (auto plugin : mLoadedFileHandlingPlugins) {
        qobject_cast<FileHandlingInterface *>(plugin->instance())->fileOpened(fileName);
    }
#endif

    return {};
}
