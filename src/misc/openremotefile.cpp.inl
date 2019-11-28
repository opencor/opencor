#ifdef GUI_SUPPORT
QString CentralWidget::openRemoteFile(const QString &pUrl, bool pShowWarning)
#else
QString cliOpenRemoteFile(const QString &pUrl)
#endif
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

#ifdef GUI_SUPPORT
        return openFile(fileNameOrUrl, File::Type::Local, QString(), pShowWarning);
#else
        return cliOpenFile(fileNameOrUrl);
#endif
    }

    // Check whether the remote file is already opened and if so select it,
    // otherwise retrieve its contents

    FileManager *fileManagerInstance = FileManager::instance();
    QString fileName = fileManagerInstance->fileName(fileNameOrUrl);

    if (fileName.isEmpty()) {
        // The remote file isn't already opened, so download its contents

        QByteArray fileContents;
        QString errorMessage;

#ifdef GUI_SUPPORT
        showBusyWidget();
        // Note: we don't subsequently hide our busy widget in case we are
        //       loading a SED-ML file / COMBINE archive. Indeed, such files may
        //       require further initialisation (in the case of the Simulation
        //       Experiment view, for example). So, instead, our busy widget
        //       will get hidden in updateGui()...
#endif

        if (readFile(fileNameOrUrl, fileContents, &errorMessage)) {
#ifdef GUI_SUPPORT
            // We were able to retrieve the contents of the remote file, so ask
            // our file manager to create a new remote file

            FileManager::Status status = fileManagerInstance->create(fileNameOrUrl, fileContents);

            // Make sure that the file has indeed been created

            if (status != FileManager::Status::Created) {
#ifdef QT_DEBUG
                qFatal("FATAL ERROR | %s:%d: '%s' could not be created.", __FILE__, __LINE__, qPrintable(fileNameOrUrl)); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay, cppcoreguidelines-pro-type-vararg)
#else
                return QObject::tr("'%1' could not be created.").arg(fileNameOrUrl);
#endif
            }

            return {};
#else
            // We were able to retrieve the contents of the remote file, so save
            // it locally

            fileName = Core::temporaryFileName();

            if (!writeFile(fileName, fileContents)) {
#ifdef QT_DEBUG
                qFatal("FATAL ERROR | %s:%d: '%s' could not be created.", __FILE__, __LINE__, qPrintable(fileNameOrUrl)); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay, cppcoreguidelines-pro-type-vararg)
#else
                return QObject::tr("'%1' could not be created.").arg(fileNameOrUrl);
#endif
            }
        } else {
            // We were not able to retrieve the contents of the remote file, so let
            // the user know about it

            return QObject::tr("'%1' could not be opened (%2).").arg(fileNameOrUrl,
                                                                     formatMessage(errorMessage));
#endif
        }

        // We were not able to retrieve the contents of the remote file, so let
        // the user know about it, after having hidden our busy widget

#ifdef GUI_SUPPORT
        hideBusyWidget();

        if (pShowWarning) {
            warningMessageBox(QObject::tr("Open Remote File"),
                              QObject::tr("<strong>%1</strong> could not be opened (%2).").arg(fileNameOrUrl,
                                                                                               formatMessage(errorMessage)));
        }

        return QObject::tr("'%1' could not be opened (%2).").arg(fileNameOrUrl,
                                                                 formatMessage(errorMessage));
#endif
    }

#ifdef GUI_SUPPORT
    return openFile(fileName, File::Type::Remote, fileNameOrUrl, pShowWarning);
#else
    return cliOpenFile(fileName, File::Type::Remote, fileNameOrUrl);
#endif
}
