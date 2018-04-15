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
// Python plugin
//==============================================================================

#include "Python.h"
// Note: this must be our first include file...

//==============================================================================

#include "coreguiutils.h"
#include "pythonplugin.h"

//==============================================================================

#include <QCoreApplication>
#include <QSettings>

#ifdef Q_OS_WIN
    #include <QDir>
#endif

//==============================================================================

#include <clocale>
#include <iostream>

#ifdef Q_OS_WIN
    #include <io.h>
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

#ifdef Q_OS_LINUX
    #include <fenv.h>
#endif

//==============================================================================

namespace OpenCOR {
namespace Python {

//==============================================================================

PLUGININFO_FUNC PythonPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to access the <a href=\"https://www.python.org/\">Python</a> language."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder au langage <a href=\"https://www.python.org/\">Python</a>."));

    return new PluginInfo(PluginInfo::ThirdParty, false, true,
                          QStringList() << "OpenSSL" << "zlib",
                          descriptions);
}

//==============================================================================
// CLI interface
//==============================================================================

int PythonPlugin::executeCommand(const QString &pCommand,
                                 const QStringList &pArguments)
{
    // Run the given CLI command or, by default, the Python interpreter

    if (!pCommand.compare("help")) {
        // Display the commands that we support

        runHelpCommand();

        return 0;
    } else if (pCommand.isEmpty() || !pCommand.compare("shell")) {
        // Run the Python interpreter

        return runShellCommand(pArguments);
    } else {
        // Not a CLI command that we support

        runHelpCommand();

        return -1;
    }
}

//==============================================================================
// Plugin interface
//==============================================================================

bool PythonPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool PythonPlugin::pluginInterfacesOk(const QString &pFileName,
                                      QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void PythonPlugin::initializePlugin()
{
    // We must set the environment variable PYTHONHOME to the location of our
    // copy of Python and this before calling any Python library code
    // Note: to call Py_SetPythonHome() doesn't work since Py_Initialize() first
    //       checks the environment and, if PYTHONHOME isn't set, uses the
    //       installation path compiled in at Python build time...

    QStringList applicationDirectories = QCoreApplication::applicationDirPath().split("/");

    applicationDirectories.removeLast();

    QString pythonHome = QString();

#if defined(Q_OS_WIN)
    pythonHome = (applicationDirectories << "Python").join("/");
#elif defined(Q_OS_LINUX)
    pythonHome = applicationDirectories.join("/");
#elif defined(Q_OS_MAC)
    pythonHome = (applicationDirectories << "Frameworks" << "Python").join("/");
#else
    #error Unsupported platform
#endif

    qputenv("PYTHONHOME", pythonHome.toUtf8());

    // Put our Python script directories at the head of the system PATH

#ifdef Q_OS_WIN
    static const char PathSeparator = ';';
#else
    static const char PathSeparator = ':';
#endif
    QByteArrayList systemPath = qgetenv("PATH").split(PathSeparator);

    systemPath.prepend((pythonHome+"/bin").toUtf8());
#ifdef Q_OS_WIN
    systemPath.prepend((pythonHome+"/Scripts").toUtf8());
#endif

    qputenv("PATH", systemPath.join(PathSeparator));

    // Ensure the user's Python site directory (in ~/.local, etc.) isn't used

    Py_NoUserSiteDirectory = 1;

#ifdef Q_OS_WIN
    // On Windows, we need to specify the location of the DLLs we have bundled
    // with OpenCOR

    SetDllDirectoryW((LPCWSTR) QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).utf16());
#endif
}

//==============================================================================

void PythonPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void PythonPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void PythonPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void PythonPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void PythonPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Plugin specific
//==============================================================================

void PythonPlugin::runHelpCommand()
{
    // Output the commands we support

    std::cout << "Commands supported by the Python plugin:" << std::endl;
    std::cout << " * Display the commands supported by the Python plugin:" << std::endl;
    std::cout << "      help" << std::endl;
    std::cout << " * Run a Python module:" << std::endl;
    std::cout << "      <module_name> ..." << std::endl;
    std::cout << " * Run a Python interpreter." << std::endl;
}

//==============================================================================

int PythonPlugin::runShellCommand(const QStringList &pArguments)
{
    // The following is adapted from Programs/python.c in the Python sources

    // Allocate some memory to keep track of our arguments
    // Note: a second copy since Python might modify our first one...

    const int argC = pArguments.size()+1;
    wchar_t **argV = (wchar_t **) PyMem_RawMalloc(sizeof(wchar_t *)*(argC+1));
    wchar_t **argV2 = (wchar_t **) PyMem_RawMalloc(sizeof(wchar_t *)*(argC+1));

    if (!argV || !argV2) {
        std::cerr << "Out of memory." << std::endl;

        return 1;
    }

    // Disable floating point exceptions on Linux
    // Note: 754 requires that FP exceptions run in "no stop" mode by default,
    //       and until C vendors implement C99's ways to control FP exceptions,
    //       Python requires non-stop mode. Alas, some platforms enable FP
    //       exceptions by default. Here, we disable them...

#ifdef Q_OS_LINUX
    fedisableexcept(FE_OVERFLOW);
#endif

    char *oldLocale = _PyMem_RawStrdup(setlocale(LC_ALL, 0));

    if (!oldLocale) {
        std::cerr << "Out of memory." << std::endl;

        return 1;
    }

    setlocale(LC_ALL, "");

    argV2[0] = argV[0] = Py_DecodeLocale("python", 0);

    for (int i = 1; i < argC; ++i) {
        argV[i] = Py_DecodeLocale(pArguments[i-1].toUtf8().constData(), 0);

        if (!argV[i]) {
            PyMem_RawFree(oldLocale);

            std::cerr << "Fatal Python error: unable to decode the command line argument #" << i+1 << "." << std::endl;

            return 1;
        }

        argV2[i] = argV[i];
    }

    argV2[argC] = argV[argC] = 0;

    setlocale(LC_ALL, oldLocale);

    PyMem_RawFree(oldLocale);

    int res = Py_Main(argC, argV);
    // Note: Py_Main() calls Py_Initialize() and Py_Finalize()...

    for (int i = 0; i < argC; ++i)
        PyMem_RawFree(argV2[i]);

    PyMem_RawFree(argV);
    PyMem_RawFree(argV2);

    return res;
}

//==============================================================================

}   // namespace Python
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
