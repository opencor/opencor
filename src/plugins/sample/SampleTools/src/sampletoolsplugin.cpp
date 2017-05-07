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
// Sample tools plugin
//==============================================================================

#include "coreguiutils.h"
#include "sampletoolsplugin.h"
#include "sampleutilities.h"

//==============================================================================

#include <QAction>
#include <QInputDialog>
#include <QMainWindow>

//==============================================================================

#include <iostream>

//==============================================================================

namespace OpenCOR {
namespace SampleTools {

//==============================================================================

PLUGININFO_FUNC SampleToolsPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin that provides an addition tool."));
    descriptions.insert("fr", QString::fromUtf8("une extension qui fournit un outil d'addition."));

    return new PluginInfo(PluginInfo::Sample, true, true,
                          QStringList() << "Core" << "Sample",
                          descriptions);
}

//==============================================================================
// CLI interface
//==============================================================================

int SampleToolsPlugin::executeCommand(const QString &pCommand,
                                      const QStringList &pArguments)
{
    // Run the given CLI command

    if (!pCommand.compare("help")) {
        // Display the commands that we support

        runHelpCommand();

        return 0;
    } else if (!pCommand.compare("add")) {
        // Add some numbers

        return runAddCommand(pArguments);
    } else {
        // Not a CLI command that we support, so show our help and leave

        runHelpCommand();

        return -1;
    }
}

//==============================================================================
// GUI interface
//==============================================================================

void SampleToolsPlugin::updateGui(Plugin *pViewPlugin, const QString &pFileName)
{
    Q_UNUSED(pViewPlugin);
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

Gui::Menus SampleToolsPlugin::guiMenus() const
{
    // We don't handle this interface...

    return Gui::Menus();
}

//==============================================================================

Gui::MenuActions SampleToolsPlugin::guiMenuActions() const
{
    // Return our menu actions

    return Gui::MenuActions() << Gui::MenuAction(Gui::MenuAction::Tools, mAddTwoNumbersAction)
                              << Gui::MenuAction(Gui::MenuAction::Tools);
}

//==============================================================================
// I18n interface
//==============================================================================

void SampleToolsPlugin::retranslateUi()
{
    // Retranslate our different Tools actions

    retranslateAction(mAddTwoNumbersAction, tr("Add Two Numbers..."), tr("Add two numbers together"));
}

//==============================================================================
// Plugin interface
//==============================================================================

bool SampleToolsPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool SampleToolsPlugin::pluginInterfacesOk(const QString &pFileName,
                                           QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void SampleToolsPlugin::initializePlugin()
{
    // Create our Add Two Numbers action

    mAddTwoNumbersAction = new QAction(Core::mainWindow());

    // A connection to handle our Add Two Numbers action

    connect(mAddTwoNumbersAction, SIGNAL(triggered()),
            this, SLOT(addTwoNumbers()));
}

//==============================================================================

void SampleToolsPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void SampleToolsPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void SampleToolsPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void SampleToolsPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void SampleToolsPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Plugin specific
//==============================================================================

void SampleToolsPlugin::runHelpCommand()
{
    // Output the commands we support

    std::cout << "Commands supported by the SampleTools plugin:" << std::endl;
    std::cout << " * Display the commands supported by the SampleTools plugin:" << std::endl;
    std::cout << "      help" << std::endl;
    std::cout << " * Add two numbers:" << std::endl;
    std::cout << "      add <nb1> <nb2>" << std::endl;
}

//==============================================================================

int SampleToolsPlugin::runAddCommand(const QStringList &pArguments)
{
    // Make sure that we have the correct number of arguments

    if (pArguments.count() != 2) {
        runHelpCommand();

        return -1;
    }

    // Make sure that the two arguments are valid numbers

    bool ok;

    double nb1 = pArguments.first().toDouble(&ok);

    if (!ok) {
        std::cout << "Sorry, but " << qPrintable(pArguments.first()) << " is not a valid number." << std::endl;

        return -1;
    }

    double nb2 = pArguments.last().toDouble(&ok);

    if (!ok) {
        std::cout << "Sorry, but " << qPrintable(pArguments.last()) << " is not a valid number." << std::endl;

        return -1;
    }

    // Add the two numbers and output the result

    std::cout << qPrintable(pArguments.first()) << " + " << qPrintable(pArguments.last()) << " = " << Sample::add(nb1, nb2) << std::endl;

    return 0;
}

//==============================================================================

void SampleToolsPlugin::addTwoNumbers()
{
    bool ok;
    double nb1 = QInputDialog::getDouble(Core::mainWindow(), tr("Add Two Numbers"), tr("First number:"),
                                         0, -2147483647, 2147483647, 1, &ok);
    double nb2;

    if (ok) {
        nb2 = QInputDialog::getDouble(Core::mainWindow(), tr("Add Two Numbers"), tr("Second number:"),
                                      0, -2147483647, 2147483647, 1, &ok);

        if (ok) {
            Core::informationMessageBox(tr("Add Two Numbers"),
                                        QString::number(nb1)+" + "+QString::number(nb2)+" = "+QString::number(Sample::add(nb1, nb2)));
        }
    }
}

//==============================================================================

}   // namespace SampleTools
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
