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
#include "pluginmanager.h"

//==============================================================================

#include <iostream>

//==============================================================================

#include <Qt>

//==============================================================================

#include <QCoreApplication>
#include <QSettings>

//==============================================================================

namespace OpenCOR {

//==============================================================================

CliApplication::CliApplication() :
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

    mPluginManager = new PluginManager(false);

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

    return plainString(pPlugin->info()->description());
}

//==============================================================================

void CliApplication::about() const
{
    // Output some information about OpenCOR

    version();

    std::cout << QSysInfo::prettyProductName().toStdString() << std::endl;
    std::cout << copyright().toStdString() << std::endl;
    std::cout << std::endl;
    std::cout << applicationDescription(false).toStdString() << std::endl;
    std::cout << std::endl;
    std::cout << applicationBuildInformation(false).toStdString() << std::endl;
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

    foreach (Plugin *plugin, mLoadedCliPlugins) {
        if (    commandPlugin.isEmpty()
            || !commandPlugin.compare(plugin->name())) {
            QStringList arguments = pArguments;

            arguments.removeFirst();
            // Note: since the first argument corresponds to the command
            //       itself...

            if (qobject_cast<CliInterface *>(plugin->instance())->executeCommand(commandName, arguments))
                *pRes = -1;
        }
    }

    return true;
}

//==============================================================================

void CliApplication::help() const
{
    // Output some help

    std::cout << "Usage: " << qAppName().toStdString()
              << " [-a|--about] [-c|--command [<plugin>::]<command> <options>] [-h|--help] [-p|--plugins] [-r|--reset] [-s|--status] [-v|--version] [<files>]"
              << std::endl;
    std::cout << " -a, --about     Display some information about OpenCOR"
              << std::endl;
    std::cout << " -c, --command   Send a command to one or all the CLI plugins"
              << std::endl;
    std::cout << " -h, --help      Display this help information"
              << std::endl;
    std::cout << " -p, --plugins   Display all the CLI plugins"
              << std::endl;
    std::cout << " -r, --reset     Reset all your settings"
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

void CliApplication::reset() const
{
    QSettings settings;

    settings.clear();

    std::cout << "All your settings have been reset." << std::endl;
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
        case Plugin::Loaded:

            pluginInfo += "the plugin is loaded and fully functional.";

            break;
        case Plugin::NotLoaded:
            pluginInfo += QString("the plugin could not be loaded due to the following problem: %1.").arg(formatMessage(plugin->statusErrors()));

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

    std::cout << OpenCOR::version().toStdString() << std::endl;
}

//==============================================================================

bool CliApplication::run(int *pRes)
{
    // See what needs doing with the CLI options, if anything

    *pRes = 0;   // By default, everything is fine

    enum Option {
        NoOption,
        AboutOption,
        CommandOption,
        HelpOption,
        PluginsOption,
        ResetOption,
        StatusOption,
        VersionOption
    };

    Option option = NoOption;

    QStringList appArguments = qApp->arguments();
    QStringList commandArguments = QStringList();

    appArguments.removeFirst();
    // Note: we remove the first argument since it corresponds to the full path
    //       to our executable, which we are not interested in...

    foreach (const QString &appArgument, appArguments) {
        if (!appArgument.compare("-a") || !appArgument.compare("--about")) {
            if (option == NoOption) {
                option = AboutOption;
            } else {
                *pRes = -1;
            }
        } else if (!appArgument.compare("-c") || !appArgument.compare("--command")) {
            if (option == NoOption) {
                option = CommandOption;
            } else {
                *pRes = -1;
            }
        } else if (!appArgument.compare("-h") || !appArgument.compare("--help")) {
            if (option == NoOption) {
                option = HelpOption;
            } else {
                *pRes = -1;
            }
        } else if (!appArgument.compare("-p") || !appArgument.compare("--plugins")) {
            if (option == NoOption) {
                option = PluginsOption;
            } else {
                *pRes = -1;
            }
        } else if (!appArgument.compare("-r") || !appArgument.compare("--reset")) {
            if (option == NoOption) {
                option = ResetOption;
            } else {
                *pRes = -1;
            }
        } else if (!appArgument.compare("-s") || !appArgument.compare("--status")) {
            if (option == NoOption) {
                option = StatusOption;
            } else {
                *pRes = -1;
            }
        } else if (!appArgument.compare("-v") || !appArgument.compare("--version")) {
            if (option == NoOption) {
                option = VersionOption;
            } else {
                *pRes = -1;
            }
        } else if (appArgument.startsWith("-")) {
            // The user provided at least one unknown option

            *pRes = -1;

            break;
        } else if (option == CommandOption) {
            // Not an option, so we consider it to be part of a command

            commandArguments << appArgument;
        }
    }

    // Handle the option the user requested, if any

    if (!*pRes) {
        switch (option) {
        case NoOption:
            return false;
        case AboutOption:
            about();

            break;
        case CommandOption:
            // Make sure that we have at least one argument (which would be the
            // command itself) before loading the plugins and then sending the
            // command to the plugin(s)

            if (commandArguments.isEmpty()) {
                *pRes = -1;

                help();
            } else {
                loadPlugins();

                if (!command(commandArguments, pRes)) {
                    *pRes = -1;

                    help();
                }
            }

            break;
        case HelpOption:
            help();

            break;
        case PluginsOption:
            loadPlugins();

            plugins();

            break;
        case ResetOption:
            reset();

            break;
        case StatusOption:
            loadPlugins();

            status();

            break;
        case VersionOption:
            version();

            break;
        }
    } else {
        help();
    }

    return true;
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
