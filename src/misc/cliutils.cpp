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
// CLI utilities
//==============================================================================

#include "cliutils.h"

//==============================================================================

#include <QCoreApplication>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkInterface>
#include <QNetworkProxyFactory>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcess>
#include <QResource>
#include <QSettings>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QXmlStreamReader>

//==============================================================================

namespace OpenCOR {

//==============================================================================

#include "corecliutils.cpp.inl"

//==============================================================================

#ifdef QT_DEBUG
static QtMessageHandler gDefaultMessageHandler;
#endif

//==============================================================================

#ifdef QT_DEBUG
void messageHandler(QtMsgType pType, const QMessageLogContext &pContext,
                    const QString &pMessage)
{
    // Handle all messages, except a specific warning from libpng
    // Note: https://wiki.archlinux.org/index.php/Libpng_errors explains the
    //       warning we want to filter out. It also provides a fix and although
    //       it worked for us in the past, it doesn't seem to work anymore,
    //       hence our filtering it out...

    static const QString LibpngMessage = "libpng warning: iCCP: known incorrect sRGB profile";

    if (pMessage != LibpngMessage) {
        gDefaultMessageHandler(pType, pContext, pMessage);
    }
}
#endif

//==============================================================================

void initQtMessagePattern()
{
    // We don't want to see debug/warning messages when not in debug mode while
    // we want to see everything when in debug mode, except some messages that
    // we want to filter out

#ifdef QT_DEBUG
    gDefaultMessageHandler = qInstallMessageHandler(messageHandler);
#else
    qSetMessagePattern("%{if-debug}%{endif}"
                       "%{if-warning}%{endif}"
                       "%{if-critical}%{message}%{endif}"
                       "%{if-fatal}%{message}%{endif}");
#endif
}

//==============================================================================

void initPluginsPath(int pArgC, char *pArgV[]) // NOLINT(hicpp-avoid-c-arrays, modernize-avoid-c-arrays)
{
    // Initialise the plugins path
    // Note: a user might have set the OpenCOR path in his/her PATH environment
    //       variable, so that s/he could then start OpenCOR from the command
    //       line by simply typing OpenCOR (see issue #1688). However, in that
    //       case, pArgV[0] won't contain the full path to OpenCOR, just its
    //       basename. So, we need to use a temporary QCoreApplication object to
    //       determine the full path to OpenCOR and, as a result, to its plugins
    //       directory, thus making it possible to run OpenCOR (and load its
    //       various plugins)...

    QFileInfo appFileInfo = QCoreApplication(pArgC, pArgV).applicationFilePath(); // NOLINT(readability-static-accessed-through-instance)
    QString appDir;

#ifdef Q_OS_WIN
    if (appFileInfo.completeSuffix().isEmpty()) {
        // If appFileInfo has no suffix, then it means that we tried to run
        // OpenCOR using something like "[OpenCOR]/OpenCOR", in which case
        // QFileInfo() will be lost when trying to retrieve the canonical path
        // for OpenCOR. Now, when we use something like "[OpenCOR]/OpenCOR",
        // it's as if we were to use something like "[OpenCOR]/OpenCOR.com", so
        // update appFileInfo accordingly

        appFileInfo = appFileInfo.absoluteFilePath()+".com";
    }
#endif

    appDir = canonicalDirName(appFileInfo.canonicalPath());

    QString pluginsDir = QString();

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    pluginsDir = appDir+"/../plugins";

    if (!QDir(pluginsDir).exists()) {
        // The plugins directory doesn't exist, which should only happen if we
        // are trying to run OpenCOR from within Qt Creator, in which case
        // OpenCOR's file name will be [OpenCOR]/build/OpenCOR.exe rather than
        // [OpenCOR]/build/bin/OpenCOR.exe as it should normally be if we were
        // to mimic the case where OpenCOR has been deployed. Then, because the
        // plugins are in [OpenCOR]/build/plugins/OpenCOR, we must skip the
        // "../" bit. So, yes, it's not neat, but is there another solution?...

        pluginsDir = appDir+"/plugins";
    }
#else
    pluginsDir = appDir+"/../PlugIns";
#endif

    QCoreApplication::setLibraryPaths(QStringList() << canonicalDirName(pluginsDir));
}

//==============================================================================

void initApplication(QString *pAppDate)
{
    // Use the system's proxy settings

    QNetworkProxyFactory::setUseSystemConfiguration(true);

    // Ignore SSL-related warnings
    // Note #1: this is to address an issue with QSslSocket not being able to
    //          resolve some methods...
    // Note #2: see issue #516 for more information...

    qputenv("QT_LOGGING_RULES", "qt.network.ssl.warning=false");

    // Set the organisation and application names of our application

    qApp->setOrganizationName("Physiome");
    qApp->setApplicationName("OpenCOR");

    // Retrieve and set the version of the application

    QString versionData;

    readFile(":/app_versiondate", versionData);

    QStringList versionDataList = versionData.split(eolString());

    qApp->setApplicationVersion(versionDataList.first());

    if (pAppDate != nullptr) {
        *pAppDate = versionDataList.last();
    }
}

//==============================================================================

QString applicationDescription(bool pGuiMode)
{
    // Return the application description

    QString res = QObject::tr(R"(%1 is a cross-platform modelling environment, which can be used to organise, edit, simulate and analyse <a href="http://www.cellml.org/">CellML</a> files.)").arg(R"(<a href=")"+QString(HomePageUrl)+R"(">)"+qAppName()+"</a>");

    return pGuiMode?res:plainString(res);
}

//==============================================================================

QString prettyProductName()
{
    // Return a pretty version of the product name

    return QSysInfo::prettyProductName();
}

//==============================================================================

} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
