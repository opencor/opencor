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

#include "pythonqtsupport.h"
#include "pythonqtsupportplugin.h"
#include "pythonshellplugin.h"

//==============================================================================

namespace OpenCOR {
namespace PythonShell {

//==============================================================================

PLUGININFO_FUNC PythonShellPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("the <a href=\"https://python.org/\">Python</a> shell plugin."));
    descriptions.insert("fr", QString::fromUtf8("le plugin shell <a href=\"https://python.org/\">Python</a>."));

    return new PluginInfo(PluginInfo::Category::Miscellaneous, false, true,
                          QStringList() << "Core" << "PythonQtSupport" << "SimulationSupport",
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

bool PythonShellPlugin::runPython(const QStringList &pArguments, int &pRes)
{
    // The following has been adapted from `Programs/python.c` in the Python
    // source code

    const int argC = pArguments.size()+1;
    auto argV = reinterpret_cast<wchar_t **>(PyMem_RawMalloc((argC+1)*sizeof(wchar_t*)));
    auto argVCopy = reinterpret_cast<wchar_t **>(PyMem_RawMalloc((argC+1)*sizeof(wchar_t*)));

    if ((argV == nullptr) || (argVCopy == nullptr)) {
        std::cerr << "Out of memory." << std::endl;

        pRes = 1;

        return false;
    }

#ifdef __FreeBSD__
    fedisableexcept(FE_OVERFLOW);
#endif

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

    pRes = Py_Main(argC, argV);

    for (int i = 0; i < argC; ++i) {
        PyMem_RawFree(argVCopy[i]);
    }

    PyMem_RawFree(argV);
    PyMem_RawFree(argVCopy);

    return true;
}

//==============================================================================

} // namespace PythonShell
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
