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
// Python shell plugin
//==============================================================================

#include "pythonqtsupport.h"
#include "pythonqtsupportplugin.h"
#include "pythonshellplugin.h"

//==============================================================================

#include <QCoreApplication>
#include <QFile>
#include <QSettings>
#include <QTimer>

//==============================================================================

namespace OpenCOR {
namespace PythonShell {

//==============================================================================

PLUGININFO_FUNC PythonShellPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("the Python shell plugin."));
    descriptions.insert("fr", QString::fromUtf8("the Python shell plugin."));

    return new PluginInfo(PluginInfo::Category::Miscellaneous, true, true,
                          QStringList() << "Core" << "DataStore"
                                        << "SimulationSupport"
                                        << "PythonQtSupport",
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

    std::cout << "Commands supported by the Python shell plugin:" << std::endl;
    std::cout << " * Display the commands supported by the plugin:" << std::endl;
    std::cout << "      help" << std::endl;
    std::cout << " * Run an interactive Python shell in OpenCOR's environment:" << std::endl;
    std::cout << "      python [option] ... [-c cmd | -m mod | file | -] [arg] ..." << std::endl;
}

//==============================================================================

bool PythonShellPlugin::runPython(const QStringList &pArguments, int &pRes)
{
    // The following has been adapted from `Programs/python.c` in the Python
    // source code

    const int argC = pArguments.size() + 1;
    auto argV = reinterpret_cast<wchar_t **>(PyMem_RawMalloc((argC+1)*sizeof(wchar_t*)));

    auto argVCopy = reinterpret_cast<wchar_t **>(PyMem_RawMalloc((argC+1)*sizeof(wchar_t*)));

    if ((argV == nullptr) || (argVCopy == nullptr)) {
        fprintf(stderr, "out of memory\n");

        pRes = 1;

        return false;
    }

#ifdef __FreeBSD__
    fedisableexcept(FE_OVERFLOW);
#endif

    char *oldloc = _PyMem_RawStrdup(setlocale(LC_ALL, nullptr));

    if (oldloc == nullptr) {
        fprintf(stderr, "out of memory\n");

        pRes = 1;

        return false;
    }

    setlocale(LC_ALL, "");

    argVCopy[0] = argV[0] = Py_DecodeLocale("python", nullptr);

    for (int i = 1; i < argC; ++i) {
        argV[i] = Py_DecodeLocale(pArguments[i-1].toUtf8().constData(), nullptr);

        if (argV[i] == nullptr) {
            PyMem_RawFree(oldloc);

            fprintf(stderr, "Fatal Python error: unable to decode the command line argument #%i\n", i+1);

            pRes = 1;

            return false;
        }

        argVCopy[i] = argV[i];
    }

    argVCopy[argC] = argV[argC] = nullptr;

    setlocale(LC_ALL, oldloc);

    PyMem_RawFree(oldloc);

    pRes = Py_Main(argC, argV);

    for (int i = 0; i < argC; ++i) {
        PyMem_RawFree(argVCopy[i]);
    }

    PyMem_RawFree(argV);
    PyMem_RawFree(argVCopy);

    return true;
}

//==============================================================================

}   // namespace PythonShell
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
