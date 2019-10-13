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

#include "pythonqtsupportplugin.h"
#include "pythonshellplugin.h"

//==============================================================================

#include <QCoreApplication>
#include <QFile>
#include <QSettings>
#include <QTimer>

//==============================================================================

#include "pythonbegin.h"
    #include "pythonqtsupport.h"
#include "pythonend.h"

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

    if (!pCommand.compare("help")) {
        // Display the commands that we support

        runHelpCommand();

        return true;
    } else if (pCommand.isEmpty() || !pCommand.compare("python")) {
        // Run a Python shell

        return runPython(pArguments, pRes);
    } else {
        // Not a CLI command that we support

        runHelpCommand();

        return false;
    }
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
    // The following has been adapted from `Programs/python.c` in the Python sources.

    const int argc = pArguments.size() + 1;
    wchar_t **argv = reinterpret_cast<wchar_t **>(PyMem_RawMalloc(sizeof(wchar_t*) * (argc+1)));

    /* We need a second copy, as Python might modify the first one. */
    wchar_t **argv_copy = reinterpret_cast<wchar_t **>(PyMem_RawMalloc(sizeof(wchar_t*) * (argc+1)));

    if (!argv || !argv_copy) {
        fprintf(stderr, "out of memory\n");
        pRes = 1;
        return false;
    }

    /* PEP 754 requires that FP exceptions run in "no stop" mode by default,
     * and until C vendors implement C99's ways to control FP exceptions,
     * Python requires non-stop mode.  Alas, some platforms enable FP
     * exceptions by default.  Here we disable them.
     */
#ifdef __FreeBSD__
    fedisableexcept(FE_OVERFLOW);
#endif

    char *oldloc = _PyMem_RawStrdup(setlocale(LC_ALL, nullptr));
    if (!oldloc) {
        fprintf(stderr, "out of memory\n");
        pRes = 1;
        return false;
    }

    setlocale(LC_ALL, "");
    argv_copy[0] = argv[0] = Py_DecodeLocale("python", nullptr);
    for (int i = 1; i < argc; i++) {
        argv[i] = Py_DecodeLocale(pArguments[i-1].toUtf8().constData(), nullptr);
        if (!argv[i]) {
            PyMem_RawFree(oldloc);
            fprintf(stderr, "Fatal Python error: "
                            "unable to decode the command line argument #%i\n",
                            i + 1);
            pRes = 1;
            return false;
        }
        argv_copy[i] = argv[i];
    }
    argv_copy[argc] = argv[argc] = nullptr;

    setlocale(LC_ALL, oldloc);
    PyMem_RawFree(oldloc);

    // N.B. PyMain() calls Py_Initialize() and Py_Finalize()

    pRes = Py_Main(argc, argv);

    for (int i = 0; i < argc; i++) {
        PyMem_RawFree(argv_copy[i]);
    }
    PyMem_RawFree(argv);
    PyMem_RawFree(argv_copy);

    return true;
}

//==============================================================================

}   // namespace PythonShell
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
