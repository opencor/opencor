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
// Python plugin
//==============================================================================

#include "pythonplugin.h"
#include "Python.h"

//==============================================================================

#include <Qt>
#include <QSettings>

//==============================================================================

#include <locale.h>
#include <stdio.h>

#ifdef __FreeBSD__
#include <fenv.h>
#endif

//==============================================================================

namespace OpenCOR {
namespace Python {

//==============================================================================

PLUGININFO_FUNC PythonPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to provide the <a href=\"https://www.python.org/\">Python</a> language."));
    descriptions.insert("fr", QString::fromUtf8("une extension ..."));

    return new PluginInfo("Third-party", false, true,
                          QStringList(),
                          descriptions);
}

//==============================================================================
// CLI interface
//==============================================================================

int PythonPlugin::executeCommand(const QString &pCommand,
                                 const QStringList &pArguments)
{
    // Pass the given CLI command to the Python interpreter

    if (!pCommand.compare("help")) {
        // Display the commands that we support

        runHelpCommand();

        return 0;
    } else if (!pCommand.compare("shell")) {
        // Run the Python interpreter

        return runPythonShell(pArguments);
// TODO: pip install....
    } else {
        // Not a CLI command that we support

        runHelpCommand();

        return -1;
    }
}

//==============================================================================
// Plugin interface
//==============================================================================

void PythonPlugin::initializePlugin()
{
    // We don't handle this interface...
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

    // TODO: Retrieve our Python settings
}

//==============================================================================

void PythonPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // TODO: Keep track of our Python settings
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
/**
    stdout << "Commands supported by the Python plugin:" << std::endl;
    std::cout << " * Display the commands supported by the Python plugin:" << std::endl;
    std::cout << "      help" << std::endl;
    std::cout << " * Run a Python interpreter:" << std::endl;
    std::cout << "      shell ..." << std::endl;
    std::cout << " * Install ...:" << std::endl;
    std::cout << "      pip ..." << std::endl;
**/
}

//==============================================================================

// Based on `Programs/python.c` from the Python sources.

/** TODO: Do we need this for Windows ??
#ifdef MS_WINDOWS
int
wmain(int argc, wchar_t **argv)
{
    return Py_Main(argc, argv);
}
**/

int PythonPlugin::runPythonShell(const QStringList &pArguments)
{

// TODO: argv[0] = full_path_to_python_exe...
// or argv[0] = "PythonPlugin" and set PYTHONHOME
// Do this when starting CLI OpenCOR ???

//Py_SetProgramName(), Py_SetPythonHome() and Py_SetPath()

// PYTHONHOME sets <prefix>
//    ${FULL_DEST_EXTERNAL_BINARIES_DIR}/Python
//    const wchar_t *home = L"/Users/dave/build/OpenCOR/src/plugins/thirdParty/Python/packages/osx";
//    Py_SetPythonHome((wchar_t *)home);

//    const wchar_t *stdlib = L"lib/python35.zip:lib/python3.5/lib-dynload";
//    Py_SetPath(stdlib);



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

    res = Py_Main(argc, argv_copy);

    for (i = 0; i < argc; i++) {
        PyMem_RawFree(argv_copy2[i]);
    }
    PyMem_RawFree(argv_copy);
    PyMem_RawFree(argv_copy2);
    return res;
}

//==============================================================================

}   // namespace Python
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
