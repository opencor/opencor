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
// PythonQt support plugin
//==============================================================================

#include "pythoninterface.h"
#include "pythonqtsupport.h"
#include "pythonqtsupportplugin.h"
#include "solverinterface.h"

//==============================================================================

#include "pythonbegin.h"
    #include "PythonQt/PythonQt_QtAll.h"
#include "pythonend.h"

//==============================================================================

namespace OpenCOR {
namespace PythonQtSupport {

//==============================================================================

PLUGININFO_FUNC PythonQtSupportPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8(R"(a plugin to support <a href="https://mevislab.github.io/pythonqt/">PythonQt</a>.)"));
    descriptions.insert("fr", QString::fromUtf8(R"(une extension pour supporter <a href="https://mevislab.github.io/pythonqt/">PythonQt</a>.)"));

    return new PluginInfo(PluginInfo::Category::Support, false, false,
                          QStringList() << "PythonQt" << "PythonSupport",
                          descriptions);
}

//==============================================================================
// Plugin interface
//==============================================================================

bool PythonQtSupportPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool PythonQtSupportPlugin::pluginInterfacesOk(const QString &pFileName,
                                               QObject *pInstance)
{
    Q_UNUSED(pFileName)
    Q_UNUSED(pInstance)

    // We don't handle this interface...

    return false;
}

//==============================================================================

void PythonQtSupportPlugin::initializePlugin()
{
    // Create and initialise a new PythonQt instance

    PythonQt::init(PythonQt::RedirectStdOut);

    // Capture and redirect PythonQt's standard output and error

    connect(PythonQt::self(), &PythonQt::pythonStdOut,
            this, &PythonQtSupportPlugin::printStdOut);

    connect(PythonQt::self(), &PythonQt::pythonStdErr,
            this, &PythonQtSupportPlugin::printStdErr);

    // Remap SIGINT
    // Note: indeed, Python maps SIGINT (Ctrl+C) to its own handler, so we need
    //       to remap it to the default handler so that Ctrl+C can work...

#ifdef SIGINT
    signal(SIGINT, SIG_DFL);
#endif

    // Initialise Qt binding for Python

    PythonQt_QtAll::init();

    // Create a Python module to access OpenCOR's objects

    mModule = PythonQt::self()->createModuleFromScript("OpenCOR");

    // Give a file name to our Python module
    // Note: indeed, IPython tracebacks are noisy if modules don't have have a
    //       file name...

    mModule.addVariable("__file__", "OpenCOR");

    // We already have set PYTHONHOME, so get it

    QString pythonHome = qEnvironmentVariable("PYTHONHOME");

    // The script to update Python scripts

    QString setPythonPathScript = pythonHome
#if defined(Q_OS_WIN)
        + "/Scripts"
#else
        + "/bin"
#endif
        + "/set_python_path.py";

    // Create a buffer in which to pass arguments to Python

    mArgV = reinterpret_cast<wchar_t **>(PyMem_RawMalloc(4*sizeof(wchar_t*)));

    // Set arguments for `set_python_path`
    // Note: we need to use an intermediate variable as otherwise the cast
    //       results in an empty string

    auto wSetPythonPathScript = setPythonPathScript.toStdWString();
    mArgV[0] = const_cast<wchar_t *>(wSetPythonPathScript.c_str());

    auto wPythonHome = pythonHome.toStdWString();
    mArgV[1] = const_cast<wchar_t *>(wPythonHome.c_str());

    mArgV[2] = const_cast<wchar_t *>(L"-s");
    mArgV[3] = nullptr;

    PySys_SetArgvEx(3, mArgV, 0);

    // Actually update the path to Python in scripts

    PythonQtSupport::evaluateFile(setPythonPathScript);

    // Clear the argument buffer so that `sys.argv` is empty in the GUI console

    mArgV[0] = const_cast<wchar_t *>(L"");
    mArgV[1] = nullptr;
    PySys_SetArgvEx(1, mArgV, 0);
}

//==============================================================================

void PythonQtSupportPlugin::finalizePlugin()
{
    // Delete some internal objects

    PyMem_RawFree(mArgV);

    // Clean up PythonQt

    PythonQt::cleanup();
}

//==============================================================================

void PythonQtSupportPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    // Register the Solver::Properties class with Qt
    // Note: indeed, so that it gets automatically wrapped to Python...

    qRegisterMetaType<OpenCOR::Solver::Solver::Properties>("Solver::Solver::Properties");

    // Register wrappers for every plugin that has a Python interface

    for (auto plugin : pLoadedPlugins) {
        PythonInterface *pythonInterface = qobject_cast<PythonInterface *>(plugin->instance());

        if (pythonInterface != nullptr) {
            pythonInterface->registerPythonClasses(mModule);
        }
    }
}

//==============================================================================

void PythonQtSupportPlugin::loadSettings(QSettings &pSettings)
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void PythonQtSupportPlugin::saveSettings(QSettings &pSettings) const
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void PythonQtSupportPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl)

    // We don't handle this interface...
}

//==============================================================================
// Plugin specific
//==============================================================================

void PythonQtSupportPlugin::printStdOut(const QString &pString)
{
    // Print the given text to the standard output

    std::cout << qPrintable(pString);
}

//==============================================================================

void PythonQtSupportPlugin::printStdErr(const QString &pString)
{
    // Print the given text to the standard error

    std::cerr << qPrintable(pString);
}

//==============================================================================

} // namespace PythonQtSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
