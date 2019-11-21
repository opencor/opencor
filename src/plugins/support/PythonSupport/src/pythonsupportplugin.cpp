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
// Python support plugin
//==============================================================================

#include "pythonsupportplugin.h"

//==============================================================================

#include <QCoreApplication>
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    #include <QDir>
#endif

//==============================================================================

#ifdef Q_OS_WIN
    #include <Windows.h>
#endif

//==============================================================================

#include "pythonbegin.h"
    #include "PythonQt/PythonQtPythonInclude.h"
#include "pythonend.h"

//==============================================================================

namespace OpenCOR {
namespace PythonSupport {

//==============================================================================

PLUGININFO_FUNC PythonSupportPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8(R"(a plugin to support <a href="https://www.python.org/">Python</a>.)"));
    descriptions.insert("fr", QString::fromUtf8(R"(une extension pour supporter <a href="https://www.python.org/">Python</a>.)"));

    return new PluginInfo(PluginInfo::Category::Support, false, false,
                          QStringList() << "Python" << "PythonPackages",
                          descriptions);
}

//==============================================================================
// Plugin interface
//==============================================================================

bool PythonSupportPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool PythonSupportPlugin::pluginInterfacesOk(const QString &pFileName,
                                               QObject *pInstance)
{
    Q_UNUSED(pFileName)
    Q_UNUSED(pInstance)

    // We don't handle this interface...

    return false;
}

//==============================================================================

void PythonSupportPlugin::initializePlugin()
{
    // Set the environment variable PYTHONHOME to the location of our copy of
    // Python
    // Note #1: we are doing something similar to what initPluginsPath() does
    //          when it comes to the "../" bit...
    // Note #2: to call Py_SetPythonHome() doesn't work since Py_Initialize()
    //          first checks the environment and, if PYTHONHOME isn't set, uses
    //          the installation path compiled in at Python build time...

    QString pythonHome = QCoreApplication::applicationDirPath()+"/../";

#if defined(Q_OS_WIN)
    pythonHome += "Python";
#elif defined(Q_OS_LINUX)
    pythonHome += "python";
#else
    pythonHome = pythonHome+"Frameworks/Python";
#endif

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    if (!QDir(pythonHome).exists()) {
        pythonHome = QCoreApplication::applicationDirPath()+"/";

#ifdef Q_OS_WIN
        pythonHome += "Python";
#else
        pythonHome += "python";
#endif
    }
#endif

    qputenv("PYTHONHOME", pythonHome.toUtf8());

    // Update our system PATH by prepending our Python script directories to it

#ifdef Q_OS_WIN
    static const char PathSeparator = ';';
#else
    static const char PathSeparator = ':';
#endif

    QByteArray path = (pythonHome+"/bin").toUtf8()+PathSeparator+qgetenv("PATH");

#ifdef Q_OS_WIN
    path = (pythonHome+"/Scripts").toUtf8()+PathSeparator+path;
#endif

    qputenv("PATH", path);

    // Ensure that the user's Python site directory (in ~/.local, etc.) isn't
    // used

    Py_NoUserSiteDirectory = 1;
}

//==============================================================================

void PythonSupportPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void PythonSupportPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins)

    // We don't handle this interface...
}

//==============================================================================

void PythonSupportPlugin::loadSettings(QSettings &pSettings)
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void PythonSupportPlugin::saveSettings(QSettings &pSettings) const
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void PythonSupportPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl)

    // We don't handle this interface...
}

//==============================================================================

} // namespace PythonSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
