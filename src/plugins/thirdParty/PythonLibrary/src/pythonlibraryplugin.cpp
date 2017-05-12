/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Python library plugin
//==============================================================================

// This must be the first include file

#include "Python.h"

//==============================================================================

#include "coreguiutils.h"
#include "pythonlibraryplugin.h"

//==============================================================================

#include <QCoreApplication>
#include <QSettings>

#ifdef _WIN32
    #include <QDir>
#endif

//==============================================================================

#include <clocale>
#include <iostream>

#ifdef _WIN32
    #include <io.h>
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

#ifdef __FreeBSD__
    #include <fenv.h>
#endif

//==============================================================================

namespace OpenCOR {
namespace PythonLibrary {

//==============================================================================

PLUGININFO_FUNC PythonLibraryPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to provide the <a href=\"https://www.python.org/\">Python</a> language."));
    descriptions.insert("fr", QString::fromUtf8("une extension ..."));

    return new PluginInfo(PluginInfo::ThirdParty, false, true,
                          QStringList() << "zlib" << "OpenSSL",
                          descriptions);
}

//==============================================================================
// CLI interface
//==============================================================================

int PythonLibraryPlugin::executeCommand(const QString &pCommand,
                                        const QStringList &pArguments)
{
    // Pass the given CLI command to the Python interpreter

    if (!pCommand.compare("help")) {
        // Display the commands that we support

        runHelpCommand();

        return 0;
    } else if (pCommand.isEmpty() || !pCommand.compare("shell")) {
        // Run the Python interpreter

        return runPythonShell(pArguments);
    } else {
        // Not a CLI command that we support so take
        // it as the name of a module to run

        QStringList arguments = QStringList() << "-m" << pCommand << pArguments;

        return runPythonShell(arguments);
    }
}

//==============================================================================
// Plugin interface
//==============================================================================

bool PythonLibraryPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool PythonLibraryPlugin::pluginInterfacesOk(const QString &pFileName,
                                      QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void PythonLibraryPlugin::initializePlugin()
{
    // We must set PYTHONHOME to where Python is 'installed' **before** calling
    // any Python library code. Calling `Py_SetPythonHome()` doesn't work as
    // the `Py_Initialize()` code first checks the environment and, if PYTHONHOME
    // isn't set, uses the installation path compiled in at Python build time...

    auto applicationDirectories = QCoreApplication::applicationDirPath().split("/");
    applicationDirectories.removeLast();

    auto pythonHome = QString();
#if __APPLE__
    pythonHome = (applicationDirectories << "Frameworks" << "Python").join("/");
#elif _WIN32
    pythonHome = (applicationDirectories << "Python").join("/");
#else
    pythonHome = applicationDirectories.join("/");
#endif
    qputenv("PYTHONHOME", pythonHome.toUtf8());

    // Ensure the user's Python site directory (in ~/.local etc) isn't used

    Py_NoUserSiteDirectory = 1;

#if _WIN32
    // For Windows we need to specify the location of the DLLs we have bundled with OpenCOR

    SetDllDirectoryW((LPCWSTR)QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).utf16());
#endif
}

//==============================================================================

void PythonLibraryPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void PythonLibraryPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void PythonLibraryPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // TODO: Retrieve our Python settings
}

//==============================================================================

void PythonLibraryPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // TODO: Keep track of our Python settings
}

//==============================================================================

void PythonLibraryPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Plugin specific
//==============================================================================

void PythonLibraryPlugin::runHelpCommand()
{
    // Output the commands we support

    std::cout << "Commands supported by the Python plugin:" << std::endl;
    std::cout << " * Display the commands supported by the Python plugin:" << std::endl;
    std::cout << "      help" << std::endl;
    std::cout << " * Run a Python module:" << std::endl;
    std::cout << "      MODULE_NAME ..." << std::endl;
    std::cout << " * Run a Python interpreter:" << std::endl;
    std::cout << "      shell ..." << std::endl;
}

//==============================================================================

int PythonLibraryPlugin::runPythonShell(const QStringList &pArguments)
{
    // The following has been adapted from `Programs/python.c` in the Python sources.

    const int argc = pArguments.size() + 1;
    wchar_t **argv_copy;
    /* We need a second copy, as Python might modify the first one. */
    wchar_t **argv_copy2;
    int i, res;
    char *oldloc;

    argv_copy = (wchar_t **)PyMem_RawMalloc(sizeof(wchar_t*) * (argc+1));
    argv_copy2 = (wchar_t **)PyMem_RawMalloc(sizeof(wchar_t*) * (argc+1));
    if (!argv_copy || !argv_copy2) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }

    /* 754 requires that FP exceptions run in "no stop" mode by default,
     * and until C vendors implement C99's ways to control FP exceptions,
     * Python requires non-stop mode.  Alas, some platforms enable FP
     * exceptions by default.  Here we disable them.
     */
#ifdef __FreeBSD__
    fedisableexcept(FE_OVERFLOW);
#endif

    oldloc = _PyMem_RawStrdup(setlocale(LC_ALL, NULL));
    if (!oldloc) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }

    setlocale(LC_ALL, "");
    argv_copy2[0] = argv_copy[0] = Py_DecodeLocale("python", NULL);
    for (i = 1; i < argc; i++) {
        argv_copy[i] = Py_DecodeLocale(pArguments[i-1].toUtf8().constData(), NULL);
        if (!argv_copy[i]) {
            PyMem_RawFree(oldloc);
            fprintf(stderr, "Fatal Python error: "
                            "unable to decode the command line argument #%i\n",
                            i + 1);
            return 1;
        }
        argv_copy2[i] = argv_copy[i];
    }
    argv_copy2[argc] = argv_copy[argc] = NULL;

    setlocale(LC_ALL, oldloc);
    PyMem_RawFree(oldloc);

    // N.B. PyMain() calls Py_Initialize() and Py_Finalize()

    res = Py_Main(argc, argv_copy);

    for (i = 0; i < argc; i++) {
        PyMem_RawFree(argv_copy2[i]);
    }
    PyMem_RawFree(argv_copy);
    PyMem_RawFree(argv_copy2);
    return res;
}

//==============================================================================

}   // namespace PythonLibrary
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
