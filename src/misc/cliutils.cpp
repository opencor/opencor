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
#include <QFileInfo>
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

void initQtMessagePattern()
{
    // We don't want to see debug/warning messages when not in debug mode

#ifndef QT_DEBUG
    qSetMessagePattern("%{if-debug}%{endif}"
                       "%{if-warning}%{endif}"
                       "%{if-critical}%{message}%{endif}"
                       "%{if-fatal}%{message}%{endif}");
#endif
}

//==============================================================================

void initPluginsPath(const QString &pAppFileName)
{
    // Initialise the plugins path

    QFileInfo appFileInfo = pAppFileName;
    QString appDir;

#ifdef Q_OS_WIN
    if (appFileInfo.completeSuffix().isEmpty()) {
        // If pAppFileName has no suffix, then it means that we tried to run
        // OpenCOR using something like "[OpenCOR]/OpenCOR", in which case
        // QFileInfo() will be lost when trying to retrieve the canonical path
        // for OpenCOR. Now, when we use something like "[OpenCOR]/OpenCOR",
        // it's as if we were to use something like "[OpenCOR]/OpenCOR.com", so
        // update appFileInfo accordingly

        appFileInfo = pAppFileName+".com";
    }
#endif

    appDir = nativeCanonicalDirName(appFileInfo.canonicalPath());

    QString pluginsDir = QString();

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    pluginsDir = appDir+QDir::separator()+QString("..")+QDir::separator()+"plugins";

    if (!QDir(pluginsDir).exists()) {
        // The plugins directory doesn't exist, which should only happen if we
        // are trying to run OpenCOR from within Qt Creator, in which case
        // OpenCOR's file name will be [OpenCOR]/build/OpenCOR.exe rather than
        // [OpenCOR]/build/bin/OpenCOR.exe as it should normally be if we were
        // to mimic the case where OpenCOR has been deployed. Then, because the
        // plugins are in [OpenCOR]/build/plugins/OpenCOR, we must skip the
        // "../" bit. So, yes, it's not neat, but is there another solution?...

        pluginsDir = appDir+QDir::separator()+"plugins";
    }
#elif defined(Q_OS_MAC)
    pluginsDir = appDir+QDir::separator()+QString("..")+QDir::separator()+"PlugIns";
#else
    #error Unsupported platform
#endif

    QCoreApplication::setLibraryPaths(QStringList() << nativeCanonicalDirName(pluginsDir));
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

    readFileContentsFromFile(":/app_versiondate", versionData);

    QStringList versionDataList = versionData.split(eolString());

    qApp->setApplicationVersion(versionDataList.first());

    if (pAppDate)
        *pAppDate = versionDataList.last();
}

//==============================================================================

QString applicationDescription(const bool &pGuiMode)
{
    QString res = QObject::tr("%1 is a cross-platform modelling environment, which can be used to organise, edit, simulate and analyse <a href=\"http://www.cellml.org/\">CellML</a> files.").arg("<a href=\""+QString(HomePageUrl)+"\">"+qAppName()+"</a>");

    return pGuiMode?res:plainString(res);
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
