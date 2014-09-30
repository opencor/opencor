/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// CLI application
//==============================================================================

#include "cliapplication.h"
#include "cliinterface.h"
#include "cliutils.h"
#include "common.h"
#include "pluginmanager.h"

//==============================================================================

#include <iostream>

//==============================================================================

#include <Qt>

//==============================================================================

#include <QCoreApplication>
#include <QXmlStreamReader>

//==============================================================================

namespace OpenCOR {

//==============================================================================

CliApplication::CliApplication(QCoreApplication *pApp) :
    mApp(pApp),
    mPluginManager(0),
    mLoadedCliPlugins(Plugins())
{
}

//==============================================================================

CliApplication::~CliApplication()
{
    // Delete some internal objects

    delete mPluginManager;
}

//==============================================================================

void CliApplication::loadPlugins()
{
    // Load all the plugins by creating our plugin manager

    mPluginManager = new PluginManager(mApp, false);

    // Keep track of our loaded CLI plugins

    foreach (Plugin *plugin, mPluginManager->loadedPlugins())
        if (qobject_cast<CliInterface *>(plugin->instance()))
            mLoadedCliPlugins << plugin;
}

//==============================================================================

QString CliApplication::pluginDescription(Plugin *pPlugin) const
{
    // Retrieve and return the plugin's default description, stripped out of all
    // its HTML (should it have some)
    // Note: we enclose the CLI plugin's default description within an html
    //       tag so that the stripping out can proceed without any
    //       problem...

    QXmlStreamReader description("<html>"+pPlugin->info()->description()+"</html>");
    QString res = QString();

    while (!description.atEnd())
        if (description.readNext() == QXmlStreamReader::Characters)
            res += description.text();

    return res;
}

//==============================================================================

void CliApplication::about() const
{
    // Output some information about OpenCOR

    version();

    std::cout << Core::osName().toStdString() << std::endl;
    std::cout << Core::copyright().toStdString() << std::endl;
    std::cout << std::endl;
    std::cout << mApp->applicationName().toStdString()
              << " is a cross-platform CellML-based modelling environment,"
              << " which can be used to organise, edit,"
              << " simulate and analyse CellML files."
              << std::endl;
}

//==============================================================================

bool CliApplication::command(const QStringList &pArguments, int *pRes) const
{
    // Make sure that we have at least one argument

    if (!pArguments.count())
        return false;

    // Determine whether the command is to be executed by all the CLI plugins or
    // only a given CLI plugin

    static const QString CommandSeparator = "::";

    QString commandName = pArguments.first();
    QString commandPlugin = commandName;
    int commandSeparatorPosition = commandName.indexOf(CommandSeparator);

    if (commandSeparatorPosition != -1) {
        commandPlugin = commandPlugin.remove(commandSeparatorPosition, commandName.length()-commandSeparatorPosition);
        commandName = commandName.remove(0, commandPlugin.length()+CommandSeparator.length());

        // Make sure that the plugin to which the command is to be sent exists

        if (!commandPlugin.isEmpty()) {
            bool pluginFound = false;
            bool pluginHasCliSupport = false;

            foreach (Plugin *plugin, mPluginManager->loadedPlugins())
                if (!commandPlugin.compare(plugin->name())) {
                    pluginFound = true;
                    pluginHasCliSupport = qobject_cast<CliInterface *>(plugin->instance());

                    break;
                }

            if (!pluginFound) {
                std::cout << "The " << commandPlugin.toStdString() << " plugin could not be found." << std::endl;

                return true;
            } else if (!pluginHasCliSupport) {
                std::cout << "The " << commandPlugin.toStdString() << " plugin does not support the execution of commands." << std::endl;

                return true;
            }
        }
    } else {
        commandPlugin = QString();
    }

    // Make sure that we have a command name

    if (commandName.isEmpty())
        return false;

    // Make sure that we have at least one CLI-enabled plugin

    if (mLoadedCliPlugins.isEmpty()) {
        std::cout << "No plugins could be found to run the command." << std::endl;

        return true;
    }

    // Send the command to the plugin(s)

    foreach (Plugin *plugin, mLoadedCliPlugins)
        if (    commandPlugin.isEmpty()
            || !commandPlugin.compare(plugin->name())) {
            QStringList arguments = pArguments;

            arguments.removeFirst();
            // Note: since the first argument corresponds to the command
            //       itself...

            if (qobject_cast<CliInterface *>(plugin->instance())->executeCommand(commandName, arguments))
                *pRes = -1;
        }

    return true;
}

//==============================================================================

void CliApplication::help() const
{
    // Output some help

    std::cout << "Usage: " << mApp->applicationName().toStdString()
              << " [-a|--about] [-c|--command [<plugin>::]<command> <options>] [-h|--help] [-p|--plugins] [-s|--status] [-v|--version] [<files>]"
              << std::endl;
    std::cout << " -a, --about     Display some information about OpenCOR"
              << std::endl;
    std::cout << " -c, --command   Send a command to one or all the CLI plugins"
              << std::endl;
    std::cout << " -h, --help      Display this help information"
              << std::endl;
    std::cout << " -p, --plugins   Display all the CLI plugins"
              << std::endl;
    std::cout << " -s, --status    Display the status of all the plugins"
              << std::endl;
    std::cout << " -v, --version   Display the version of OpenCOR"
              << std::endl;
}

//==============================================================================

void CliApplication::plugins() const
{
    // Output some information about our CLI plugins, so first make sure that we
    // have at least one of them

    if (mLoadedCliPlugins.isEmpty()) {
        std::cout << "No CLI plugins could be found." << std::endl;

        return;
    }

    // First, we retrieve all the CLI plugins information

    QStringList pluginsInfo = QStringList();

    foreach (Plugin *plugin, mLoadedCliPlugins) {
        // Retrieve the CLI plugin and its default description

        QString pluginInfo = plugin->name();
        QString pluginDesc = pluginDescription(plugin);

        if (!pluginDesc.isEmpty())
            pluginInfo += ": "+pluginDesc;

        // Add the plugin information to our list

        pluginsInfo << pluginInfo;
    }

    // Now, we can output the plugin information in alphabetical order

    pluginsInfo.sort(Qt::CaseInsensitive);

    if (pluginsInfo.count() == 1)
        std::cout << "The following CLI plugin is available:" << std::endl;
    else
        std::cout << "The following CLI plugins are available:" << std::endl;

    foreach (const QString &pluginInfo, pluginsInfo)
        std::cout << " - " << pluginInfo.toStdString() << std::endl;
}

//==============================================================================

void CliApplication::status() const
{
    // Output the status of all the plugins that should (have) normally be(en)
    // loaded, so first make sure that we have at least one of them

    if (mPluginManager->loadedPlugins().isEmpty()) {
        std::cout << "No plugins could be found." << std::endl;

        return;
    }

    // First, we retrieve all the plugins information

    QStringList pluginsInfo = QStringList();

    foreach (Plugin *plugin, mPluginManager->plugins()) {
        // Retrieve the plugin and its status

        QString pluginInfo = plugin->name()+": ";

        // Retrieve the plugin's status

        switch (plugin->status()) {
        case Plugin::NotWanted:
            pluginInfo += "the plugin is not wanted.";

            break;
        case Plugin::NotNeeded:
            pluginInfo += "the plugin is not needed.";

            break;
        case Plugin::NotLoaded:
            pluginInfo += QString("the plugin could not be loaded due to the following problem: %1.").arg(Core::formatErrorMessage(plugin->statusErrors()));

            break;
        case Plugin::NotPlugin:
            pluginInfo += "this is not a plugin.";

            break;
        case Plugin::NotCorePlugin:
            pluginInfo += "the plugin claims to be the core plugin, but it is not.";

            break;
        case Plugin::InvalidCorePlugin:
            pluginInfo += "the plugin should be the core plugin, but it does not support the core interface.";

            break;
        case Plugin::NotCliPluginNoCliSupport:
            pluginInfo += "the plugin supports the CLI interface, but it does not claim to be CLI-capable.";

            break;
        case Plugin::NotCliPluginNoCliInterface:
            pluginInfo += "the plugin claims to be CLI-capable, but it does not support the CLI interface.";

            break;
        case Plugin::MissingOrInvalidDependencies:
            if (plugin->statusErrorsCount() == 1)
                pluginInfo += QString("the plugin could not be loaded due to the %1 plugin being missing or invalid.").arg(plugin->statusErrors());
            else
                pluginInfo += QString("the plugin could not be loaded due to missing or invalid plugins:\n%1").arg(plugin->statusErrors());

            break;
        default:
            // Plugin::Loaded

            pluginInfo += "the plugin is loaded and fully functional.";
        }

        // Add the plugin information to our list

        pluginsInfo << pluginInfo;
    }

    // Now, we can output the plugin information in alphabetical order

    pluginsInfo.sort(Qt::CaseInsensitive);

    if (pluginsInfo.count() == 1)
        std::cout << "The following plugin is available:" << std::endl;
    else
        std::cout << "The following plugins are available:" << std::endl;

    foreach (const QString &pluginInfo, pluginsInfo)
        std::cout << " - " << pluginInfo.toStdString() << std::endl;
}

//==============================================================================

void CliApplication::version() const
{
    // Output the version of OpenCOR

    std::cout << OpenCOR::version(mApp).toStdString() << std::endl;
}

//==============================================================================

bool CliApplication::run(int *pRes)
{
    // See what needs doing with the CLI options, if anything

    *pRes = 0;   // By default, everything is fine

    bool aboutOption = false;
    bool commandOption = false;
    bool helpOption = false;
    bool pluginsOption = false;
    bool statusOption = false;
    bool versionOption = false;

    QStringList commandArguments = QStringList();

    foreach (const QString &argument, mApp->arguments())
        if (!argument.compare("-a") || !argument.compare("--about")) {
            aboutOption = true;
        } else if (!argument.compare("-c") || !argument.compare("--command")) {
            commandOption = true;
        } else if (!argument.compare("-h") || !argument.compare("--help")) {
            helpOption = true;
        } else if (!argument.compare("-p") || !argument.compare("--plugins")) {
            pluginsOption = true;
        } else if (!argument.compare("-s") || !argument.compare("--status")) {
            statusOption = true;
        } else if (!argument.compare("-v") || !argument.compare("--version")) {
            versionOption = true;
        } else if (argument.startsWith("-")) {
            // The user provided at least one unknown option

            help();

            *pRes = -1;

            break;
        } else if (commandOption) {
            // Not an option, so we consider it to be part of a command

            commandArguments << argument;
        }

    // Handle the option the user requested, if any

    if (!*pRes) {
        if (aboutOption) {
            about();
        } else if (commandOption) {
            // Make sure that we have at least one argument (which would be the
            // command itself) before loading the plugins and then sending the
            // command to the plugin(s)

            if (commandArguments.isEmpty()) {
                help();
            } else {
                loadPlugins();

                if (!command(commandArguments, pRes))
                    help();
            }
        } else if (helpOption) {
            help();
        } else if (pluginsOption) {
            loadPlugins();

            plugins();
        } else if (statusOption) {
            loadPlugins();

            status();
        } else if (versionOption) {
            version();
        } else {
            // The user didn't provide any option that requires running OpenCOR
            // as a CLI application

            return false;
        }
    }

    return true;
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
