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
// Python Support plugin
//==============================================================================

#include "pythonsupportplugin.h"

//==============================================================================

#include <QCoreApplication>
#ifdef Q_OS_WIN
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

    descriptions.insert("en", QString::fromUtf8("the Python support plugin."));
    descriptions.insert("fr", QString::fromUtf8("the Python support plugin."));

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
    // We must set the environment variable PYTHONHOME to the location of our
    // copy of Python and this before calling any Python code
    // Note: to call Py_SetPythonHome() doesn't work since Py_Initialize() first
    //       checks the environment and, if PYTHONHOME isn't set, uses the
    //       installation path compiled in at Python build time...

    QStringList applicationDirectories = QCoreApplication::applicationDirPath().split("/");

    applicationDirectories.removeLast();

    QString pythonHome = QString();

#if defined(Q_OS_WIN)
    pythonHome = (applicationDirectories << "Python").join("/");
#elif defined(Q_OS_LINUX)
    pythonHome = (applicationDirectories << "python").join("/");
#elif defined(Q_OS_MAC)
    pythonHome = (applicationDirectories << "Frameworks" << "Python").join("/");
#endif

    qputenv("PYTHONHOME", pythonHome.toUtf8());

    // Get our current system PATH

#ifdef Q_OS_WIN
    static const char PathSeparator = ';';
#else
    static const char PathSeparator = ':';
#endif
    QByteArrayList systemPath = qgetenv("PATH").split(PathSeparator);

    // Put our Python script directories at the head of the system PATH

    systemPath.prepend((pythonHome+"/bin").toUtf8());
#ifdef Q_OS_WIN
    systemPath.prepend((pythonHome+"/Scripts").toUtf8());
#endif

    // Update the system PATH

    qputenv("PATH", systemPath.join(PathSeparator));

    // Ensure the user's Python site directory (in ~/.local, etc.) isn't used

    Py_NoUserSiteDirectory = 1;
}

//==============================================================================

void PythonSupportPlugin::finalizePlugin()
{
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
