/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Python shell plugin
//==============================================================================

#include "pythonshellplugin.h"
#include "pythonqtsupport.h"

//==============================================================================

#include <iomanip>
#include <iostream>
#include <sstream>

//==============================================================================

#include "pythonbegin.h"
    #include "PythonQt/PythonQtPythonInclude.h"
#include "pythonend.h"

//==============================================================================

namespace OpenCOR {
namespace PythonShell {

//==============================================================================

PLUGININFO_FUNC PythonShellPluginInfo()
{
    static const Descriptions descriptions = {
                                                 { "en", QString::fromUtf8("the <a href=\"https://python.org/\">Python</a> shell plugin.") },
                                                 { "fr", QString::fromUtf8("le plugin shell <a href=\"https://python.org/\">Python</a>.") }
                                             };

    return new PluginInfo(PluginInfo::Category::Miscellaneous, false, true,
                          { "Core", "PythonQtSupport", "SimulationSupport" },
                          descriptions);
}

//==============================================================================
// CLI interface
//==============================================================================

bool PythonShellPlugin::executeCommand(const QString &pCommand,
                                       const QStringList &pArguments,
                                       int &pRes)
{
    // Run the given CLI command

    static const QString Help = "help";
    static const QString Python = "python";

    if (pCommand == Help) {
        // Display the commands that we support

        runHelpCommand();

        return true;
    }

    if (pCommand.isEmpty() || (pCommand == Python)) {
        // Run a Python shell

        return runPython(pArguments, pRes);
    }

    // Not a CLI command that we support

    runHelpCommand();

    return false;
}

//==============================================================================
// Plugin specific
//==============================================================================

void PythonShellPlugin::runHelpCommand()
{
    // Output the commands we support

    std::cout << "Commands supported by the PythonShell plugin:" << std::endl;
    std::cout << " * Display the commands supported by the PythonShell plugin:" << std::endl;
    std::cout << "      help" << std::endl;
    std::cout << " * Run an interactive Python shell in OpenCOR's environment:" << std::endl;
    std::cout << "      [python] [<option> ...] [-c <command> | -m <module> | <file> | -] [<argument> ...]" << std::endl;
    std::cout << "   where" << std::endl;
    std::cout << "      -c <command> executes a program passed in as a string" << std::endl;
    std::cout << "      -m <module> runs a library module as a script" << std::endl;
    std::cout << "      <file> runs a program read from a script file" << std::endl;
    std::cout << "      - runs a program read from the standard input" << std::endl;
}

//==============================================================================

static const char * pyStringAsCString(const wchar_t *pString)
{
    auto *unicode = PyUnicode_FromWideChar(pString, -1);

    if (unicode == nullptr) {
        return "";
    }

    auto *bytes = PyUnicode_AsUTF8String(unicode);

#include "pythonbegin.h"
    Py_DECREF(unicode);
#include "pythonend.h"

    if (bytes == nullptr) {
        return "";
    }

    return PyBytes_AsString(bytes);
}

//==============================================================================

static std::string pyStringListAsString(const PyWideStringList pList)
{
    std::ostringstream res;

    res << "[";

    for (Py_ssize_t i = 0; i < pList.length; ++i) {
        if (i > 0) {
            res << ", ";
        }

        res << std::quoted(pyStringAsCString(pList.items[i]));
    }

    res << "]";

    return res.str();
}

//==============================================================================

static void runCommand(const wchar_t *pCommand)
{
    PythonQtSupport::evaluateScript(pyStringAsCString(pCommand));
}

//==============================================================================

static void runModule(const wchar_t *pModule, const PyWideStringList pArgV)
{
    static const QString module = R"PYTHON(
import runpy
import sys

sys.argv = %1

sys.path.insert(0, '.')

runpy.run_module('%2', init_globals=globals(), run_name='__main__')
)PYTHON";

    PythonQtSupport::evaluateScript(module.arg(pyStringListAsString(pArgV).c_str())
                                          .arg(pyStringAsCString(pModule)));
}

//==============================================================================

static void runFilename(const wchar_t *pFileName, const PyWideStringList pArgV)
{
    static const QString file = R"PYTHON(
import runpy
import sys

sys.argv = %1

sys.path.insert(0, '.')

runpy.run_path('%2', init_globals=globals(), run_name='__main__')
)PYTHON";

    PythonQtSupport::evaluateScript(file.arg(pyStringListAsString(pArgV).c_str())
                                        .arg(pyStringAsCString(pFileName)));
}

//==============================================================================

static void runInteractive()
{
    static const QString interactive = R"PYTHON(
import code
import sys

copyright = 'Type "help", "copyright", "credits" or "license" for more information.'

code.interact(banner=f'Python {sys.version} on {sys.platform}\n{copyright}', exitmsg='')
)PYTHON";

    PythonQtSupport::evaluateScript(interactive);
}

//==============================================================================

bool PythonShellPlugin::runPython(const QStringList &pArguments, int &pRes)
{
    // Get the command line arguments to pass to Python

    const int argC = pArguments.size()+1;
    auto argV = reinterpret_cast<wchar_t **>(PyMem_RawMalloc((argC+1)*sizeof(wchar_t*)));
    auto argVCopy = reinterpret_cast<wchar_t **>(PyMem_RawMalloc((argC+1)*sizeof(wchar_t*)));

    if ((argV == nullptr) || (argVCopy == nullptr)) {
        std::cerr << "Out of memory." << std::endl;

        pRes = 1;

        return false;
    }

    char *locale = _PyMem_RawStrdup(setlocale(LC_ALL, nullptr));

    if (locale == nullptr) {
        std::cerr << "Out of memory." << std::endl;

        pRes = 1;

        return false;
    }

    setlocale(LC_ALL, "");

    argVCopy[0] = argV[0] = Py_DecodeLocale("python", nullptr);

    for (int i = 1; i < argC; ++i) {
        argV[i] = Py_DecodeLocale(pArguments[i-1].toUtf8().constData(), nullptr);

        if (argV[i] == nullptr) {
            PyMem_RawFree(locale);

            std::cerr << "Fatal Python error: unable to decode the command line argument #" << i+1 << "." << std::endl;

            pRes = 1;

            return false;
        }

        argVCopy[i] = argV[i];
    }

    argVCopy[argC] = argV[argC] = nullptr;

    setlocale(LC_ALL, locale);

    PyMem_RawFree(locale);

    // PythonQt has already initialised Python, so we need to update the
    // existing configuration

    PyConfig config;

    PyConfig_InitPythonConfig(&config);

    config.user_site_directory = 0;

    // Set our arguments into the new configuration and parse them

    PyStatus status = PyConfig_SetArgv(&config, argC, argV);

    if (PyStatus_Exception(status)) {
        pRes = 1;

        goto done;
    }

    status = PyConfig_Read(&config);

    if (PyStatus_Exception(status)) {
        pRes = 1;

        goto done;
    }

    if (config.run_command != nullptr) {
        runCommand(config.run_command);
    } else if (config.run_module != nullptr) {
        runModule(config.run_module, config.argv);
    } else if (config.run_filename != nullptr) {
        runFilename(config.run_filename, config.argv);
    } else {
        runInteractive();
    }

done:
    // Cleanup

    PyConfig_Clear(&config);

    for (int i = 0; i < argC; ++i) {
        PyMem_RawFree(argVCopy[i]);
    }

    PyMem_RawFree(argV);
    PyMem_RawFree(argVCopy);

    return pRes == 0;
}

//==============================================================================

} // namespace PythonShell
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
