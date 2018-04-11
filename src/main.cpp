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
// Main
//==============================================================================

#include "checkforupdatesdialog.h"
#include "cliapplication.h"
#include "cliutils.h"
#include "guiapplication.h"
#include "guiutils.h"
#include "mainwindow.h"
#include "splashscreenwindow.h"

#ifdef Q_OS_MAC
    #include "macos.h"
#endif

//==============================================================================

#include <QDir>
#include <QLocale>
#include <QProcess>
#include <QSettings>
#include <QVariant>

#if defined(Q_OS_WIN) && defined(USE_PREBUILT_QTWEBKIT_PACKAGE)
    #include <QWebSettings>
#endif

//==============================================================================

int main(int pArgC, char *pArgV[])
{
    // Initialise Qt's message pattern

    OpenCOR::initQtMessagePattern();

    // On macOS, make sure that no ApplePersistenceIgnoreState message is shown
    // and that some macOS specific menu items are not shown

#ifdef Q_OS_MAC
    QProcess::execute("defaults",
                      QStringList() << "write"
                                    << "ws.opencor"
                                    << "ApplePersistenceIgnoreState"
                                    << "NO");

    OpenCOR::removeMacosSpecificMenuItems();
#endif

    // Determine whether we should try the CLI version of OpenCOR:
    //  - Windows: we never try the CLI version of OpenCOR. We go straight for
    //             its GUI version.
    //  - Linux: we always try the CLI version of OpenCOR and then go for its
    //           GUI version, if needed.
    //  - macOS: we try the CLI version of OpenCOR unless the user double clicks
    //           on the OpenCOR bundle or opens it from the command line by
    //           entering something like:
    //              open OpenCOR.app
    //          in which case we go for its GUI version.
    // Note #1: on Windows, we have two binaries (.com and .exe that are for the
    //          CLI and GUI versions of OpenCOR, respectively). This means that
    //          when a console window is open, to enter something like:
    //              C:\>OpenCOR
    //          will effectively call OpenCOR.com. From there, should there be
    //          no argument that requires CLI treatment, then the GUI version of
    //          OpenCOR will be run. This is, unfortunately, the only way to
    //          have OpenCOR to behave as both a CLI and a GUI application on
    //          Windows, hence the [OpenCOR]/windows/main.cpp file, which is
    //          used to generate the CLI version of OpenCOR...
    // Note #2: on macOS, if we were to try to open the OpenCOR bundle from the
    //          command line, then we would get an error message similar to:
    //              LSOpenURLsWithRole() failed with error -10810 for the file
    //              [SomePath]/OpenCOR.app.
    //          Fortunately, when double clicking on the OpenCOR bundle or
    //          opening it from the command line, a special argument in the form
    //          of -psn_0_1234567 is passed to OpenCOR, so we can use that to
    //          determine whether we need to force OpenCOR to be run in GUI mode
    //          or whether we first try the CLI version of OpenCOR, and then its
    //          GUI version, if needed...

#if defined(Q_OS_WIN)
    bool tryCliVersion = false;
#elif defined(Q_OS_LINUX)
    bool tryCliVersion = true;
#elif defined(Q_OS_MAC)
    bool tryCliVersion = (pArgC == 1) || memcmp(pArgV[1], "-psn_", 5);
#else
    #error Unsupported platform
#endif

    // Run the CLI version of OpenCOR, if possible/needed

    if (tryCliVersion) {
        // Initialise the plugins path

        OpenCOR::initPluginsPath(pArgV[0]);

        // Create and initialise the CLI version of OpenCOR

        OpenCOR::CliApplication *cliApp = new OpenCOR::CliApplication(pArgC, pArgV);

        OpenCOR::initApplication();

        // Try to run the CLI version of OpenCOR

        int res;
        bool runCliApplication = cliApp->run(&res);

        delete cliApp;

        if (runCliApplication) {
            // OpenCOR was run as a CLI application, so leave

            return res;
        }

        // Note: at this stage, we tried the CLI version of OpenCOR, but in the
        //       end we need to go for its GUI version, so start over but with
        //       the GUI version of OpenCOR this time...
    }

    // Initialise the plugins path

    OpenCOR::initPluginsPath(pArgV[0]);

    // Create the GUI version of OpenCOR

    OpenCOR::GuiApplication *guiApp = new OpenCOR::GuiApplication(QFileInfo(pArgV[0]).baseName(),
                                                                  pArgC, pArgV);

    // Send a message (containing the arguments that were passed to this
    // instance of OpenCOR minus the first one since it corresponds to the full
    // path to our executable, which we are not interested in) to our 'official'
    // instance of OpenCOR, should there be one (if there is none, then just
    // carry on as normal, otherwise exit since we want only one instance of
    // OpenCOR at any given time)

    QStringList appArguments = guiApp->arguments();

    appArguments.removeFirst();

    QString arguments = appArguments.join('|');

    if (guiApp->isRunning()) {
        guiApp->sendMessage(arguments);

        delete guiApp;

        return 0;
    }

    // Initialise the GUI version of OpenCOR

    QString appDate = QString();

    OpenCOR::initApplication(&appDate);

    // Check whether we want to check for new versions at startup and, if so,
    // whether a new version of OpenCOR is available

#ifndef QT_DEBUG
    QSettings settings;

    settings.beginGroup(OpenCOR::SettingsCheckForUpdatesDialog);
        bool checkForUpdatesAtStartup = settings.value(OpenCOR::SettingsCheckForUpdatesAtStartup, true).toBool();
        bool includeSnapshots = settings.value(OpenCOR::SettingsIncludeSnapshots, false).toBool();

        if (checkForUpdatesAtStartup) {
            OpenCOR::CheckForUpdatesEngine *checkForUpdatesEngine = new OpenCOR::CheckForUpdatesEngine(appDate);

            checkForUpdatesEngine->check();

            if (   ( includeSnapshots && checkForUpdatesEngine->hasNewerVersion())
                || (!includeSnapshots && checkForUpdatesEngine->hasNewerOfficialVersion())) {
                // Retrieve the language to be used to show the check for
                // updates window

                QString locale = OpenCOR::locale();

                QLocale::setDefault(QLocale(locale));

                QTranslator qtBaseTranslator;
                QTranslator qtHelpTranslator;
                QTranslator qtXmlPatternsTranslator;
                QTranslator appTranslator;

                qtBaseTranslator.load(QString(":/translations/qtbase_%1.qm").arg(locale));
                guiApp->installTranslator(&qtBaseTranslator);

                qtHelpTranslator.load(QString(":/translations/qt_help_%1.qm").arg(locale));
                guiApp->installTranslator(&qtHelpTranslator);

                qtXmlPatternsTranslator.load(QString(":/translations/qtxmlpatterns_%1.qm").arg(locale));
                guiApp->installTranslator(&qtXmlPatternsTranslator);

                appTranslator.load(":/app_"+locale);
                guiApp->installTranslator(&appTranslator);

                // Show the check for updates window
                // Note: checkForUpdatesEngine gets deleted by
                //       checkForUpdatesDialog...

                OpenCOR::CheckForUpdatesDialog checkForUpdatesDialog(&settings, checkForUpdatesEngine);

                checkForUpdatesDialog.exec();
            } else {
                delete checkForUpdatesEngine;
            }
        }
    settings.endGroup();
#endif

    // Create and show our splash screen, if we are not in debug mode

#ifndef QT_DEBUG
    OpenCOR::SplashScreenWindow *splashScreen = new OpenCOR::SplashScreenWindow();

    splashScreen->show();
#endif

    // Create our main window

    OpenCOR::MainWindow *win = new OpenCOR::MainWindow(appDate);

    // Keep track of our main window (required by QtSingleApplication so that it
    // can do what it's supposed to be doing)

    guiApp->setActivationWindow(win);

    // Handle our arguments

    win->handleArguments(appArguments);

    // Show our main window

    win->show();

    // By default, we can and should execute our application

    bool canExecuteAplication = true;

    // Close and delete our splash screen once our main window is visible, if we
    // are not in debug mode

#ifndef QT_DEBUG
    splashScreen->closeAndDeleteAfter(win);

    // Make sure that our main window is in the foreground, unless the user
    // decided to close our main window while we were showing our splash screen
    // Note: indeed, on Linux, to show our splash screen may result in our main
    //       window being shown in the background...

    if (!OpenCOR::aboutToQuit())
        win->showSelf();
    else
        canExecuteAplication = false;
#endif

    // Execute our application, if possible

    int res;

    if (canExecuteAplication)
        res = guiApp->exec();
    else
        res = 0;

    // Keep track of our application file and directory paths (in case we need
    // to restart OpenCOR)

    QString appFilePath = guiApp->applicationFilePath();
    QString appDirPath = guiApp->applicationDirPath();

    // Delete our main window

    delete win;

    // Clear QtWebKit's memory caches
    // Note #1: indeed, our use of QtWebKit (and QWebPage in particular) may
    //          result in some leak-related messages when leaving OpenCOR on
    //          Windows. This is because an object cache is shared between all
    //          of our QWebPage instances. So to destroy a QWebPage instance
    //          doesn't clear the cache, hence the leak-related messages. Those
    //          messages are 'only' warnings, so we could safely ignore them.
    //          Still, they don't look 'good', hence we clear QtWebKit's memory
    //          caches so as to avoid them...
    // Note #2: the below must absolutely be done after calling guiApp->exec()
    //          and before deleting guiApp...

#if defined(Q_OS_WIN) && defined(USE_PREBUILT_QTWEBKIT_PACKAGE)
    QWebSettings::clearMemoryCaches();
#endif

    // Delete our application

    delete guiApp;

    // We are done with the execution of our application, so now the question is
    // whether we need to restart
    // Note: we do this here rather than 'within' the GUI because once we have
    //       launched a new instance of OpenCOR, we want this instance of
    //       OpenCOR to finish as soon as possible, which will be the case here
    //       since all that remains to be done is to return the result of the
    //       execution of our application...

    if ((res == OpenCOR::CleanRestart) || (res == OpenCOR::NormalRestart)) {
        // We want to restart, so the question is whether we want a normal
        // restart or a clean one

        if (res == OpenCOR::CleanRestart) {
            // We want a clean restart, so clear all the user settings (indeed,
            // this will ensure that the various windows are, for instance,
            // properly reset with regards to their dimensions)

            QSettings().clear();
        }

        // Restart OpenCOR, but without providing any of the arguments that were
        // originally passed to us since we want to reset everything

        QProcess::startDetached(appFilePath, QStringList(), appDirPath);
    }

    // We are done running the GUI version of OpenCOR, so leave

    return res;
}

//==============================================================================
// End of file
//==============================================================================
