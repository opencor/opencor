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
// Run Python script plugin
//==============================================================================

#include "pythonqtsupportplugin.h"
#include "pythonrunscriptplugin.h"

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
namespace PythonRunScript {

//==============================================================================

PLUGININFO_FUNC PythonRunScriptPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("the run Python script  plugin."));
    descriptions.insert("fr", QString::fromUtf8("the run Python script  plugin."));

    return new PluginInfo(PluginInfo::Category::Miscellaneous, true, true,
                          QStringList() << "Core" << "DataStore"
                                        << "SimulationSupport"
                                        << "PythonQtSupport",
                          descriptions);
}

//==============================================================================
// CLI interface
//==============================================================================

bool PythonRunScriptPlugin::executeCommand(const QString &pCommand,
                                          const QStringList &pArguments,
                                          int &pRes)
{
    // Run the given CLI command

    if (!pCommand.compare("help")) {
        // Display the commands that we support

        runHelpCommand();

        return true;
    } else if (!pCommand.compare("script")) {
        // Run the Python script in the specified file

        return runScript(pArguments, pRes);
    } else {
        // Not a CLI command that we support

        runHelpCommand();

        return false;
    }
}

//==============================================================================
// Plugin specific
//==============================================================================

void PythonRunScriptPlugin::runHelpCommand()
{
    // Output the commands we support

    std::cout << "Commands supported by the run Python script plugin:" << std::endl;
    std::cout << " * Display the commands supported by the plugin:" << std::endl;
    std::cout << "      help" << std::endl;
    std::cout << " * Run a Python script with OpenCOR:" << std::endl;
    std::cout << "      script <python_file> [parameters...]" << std::endl;
}

//==============================================================================

bool PythonRunScriptPlugin::runScript(const QStringList &pArguments, int &pRes)
{
    // Make sure that we have sufficient arguments

    if (pArguments.count() < 1) {
        runHelpCommand();

        return -1;
    }

    const QString filename = pArguments[0];

    if (!QFile::exists(filename)) {
        std::cerr << "File '" << filename.toStdString() << "' does not exist" << std::endl;

        return false;
    }

    // Set `sys.argv` to the list of arguments

    PyObject *arguments = PyList_New(0);

    for (const auto &argument : pArguments) {
        PyList_Append(arguments, PyUnicode_FromString(argument.toUtf8().constData()));
    }

    auto sysModule = PythonQtSupport::importModule("sys");

    PyModule_AddObject(sysModule.object(), "argv", arguments);  // Steals the reference

    // Run the file

    PythonQtSupport::evalFile(filename);

    // Pass back Python's `sys.exit()` code

    pRes = PythonQtSupport::PythonQtSupportPlugin::instance()->systemExitCode();

    return true;
}

//==============================================================================

}   // namespace PythonRunScript
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
