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
// CLI application
//==============================================================================

#include "cliapplication.h"
#include "cliinterface.h"
#include "cliutils.h"
#include "plugininterface.h"
#include "pluginmanager.h"
#include "solverinterface.h"

//==============================================================================

#include <iostream>

//==============================================================================

#include <QCoreApplication>
#include <QDir>
#include <QSettings>

//==============================================================================

namespace OpenCOR {

//==============================================================================

CliApplication::CliApplication(int &pArgC, char *pArgV[]) // NOLINT(hicpp-avoid-c-arrays, modernize-avoid-c-arrays)
{
    // Create our CLI application

    mCliApplication = new QCoreApplication(pArgC, pArgV);
}

//==============================================================================

CliApplication::~CliApplication()
{
    // Keep track of the settings of our various plugins

    QSettings settings;

    settings.beginGroup(SettingsPlugins);
        for (auto plugin : mLoadedPluginPlugins) {
            settings.beginGroup(plugin->name());
                qobject_cast<PluginInterface *>(plugin->instance())->saveSettings(settings);
            settings.endGroup();
        }
    settings.endGroup();

    // Finalise our loaded plugins, if any

    for (auto plugin : mLoadedPluginPlugins) {
        qobject_cast<PluginInterface *>(plugin->instance())->finalizePlugin();
    }

    // Delete some internal objects

    delete mCliApplication;
    delete mPluginManager;
}

//==============================================================================

void CliApplication::loadPlugins()
{
    // Load all the plugins by creating our plugin manager

    mPluginManager = new PluginManager(false);

    // Retrieve some categories of plugins

    for (auto plugin : mPluginManager->loadedPlugins()) {
        if (qobject_cast<CliInterface *>(plugin->instance()) != nullptr) {
            mLoadedCliPlugins << plugin;
        }

        if (qobject_cast<PluginInterface *>(plugin->instance()) != nullptr) {
            mLoadedPluginPlugins << plugin;
        }

        if (qobject_cast<SolverInterface *>(plugin->instance()) != nullptr) {
            mLoadedSolverPlugins << plugin;
        }
    }

    // Initialise the plugins themselves

    for (auto plugin : mLoadedPluginPlugins) {
        qobject_cast<PluginInterface *>(plugin->instance())->initializePlugin();
    }

    // Let our various plugins know that all of them have been initialised

    for (auto plugin : mLoadedPluginPlugins) {
        qobject_cast<PluginInterface *>(plugin->instance())->pluginsInitialized(mPluginManager->loadedPlugins());
    }

    // Retrieve the settings of our various plugins

    QSettings settings;

    settings.beginGroup(SettingsPlugins);
        for (auto plugin : mLoadedPluginPlugins) {
            settings.beginGroup(plugin->name());
                qobject_cast<PluginInterface *>(plugin->instance())->loadSettings(settings);
            settings.endGroup();
        }
    settings.endGroup();
}

//==============================================================================

void CliApplication::includePlugins(const QStringList &pPluginNames,
                                    bool pInclude) const
{
    // Retrieve all the plugins that are available

    QString pluginsDir = QCoreApplication::libraryPaths().first()+"/"+qAppName();
    QFileInfoList fileInfoList = QDir(pluginsDir).entryInfoList(QStringList("*"+PluginExtension), QDir::Files);
    QStringList availablePluginNames = QStringList();

    for (const auto &fileInfo : fileInfoList) {
        availablePluginNames << Plugin::name(fileInfo.canonicalFilePath());
    }

    // Include/exclude the given plugins to/from the GUI version of OpenCOR,
    // after having sorted them and made sure that they actually exist

    QStringList pluginNames = pPluginNames;

    std::sort(pluginNames.begin(), pluginNames.end());

    for (const auto &pluginName : pluginNames) {
        QString status;

        if (availablePluginNames.contains(pluginName)) {
            // Make sure that the plugin is selectable before including/excluding it

            QString errorMessage;
            PluginInfo *pluginInfo = Plugin::info(Plugin::fileName(pluginsDir, pluginName), &errorMessage);

            if (pluginInfo != nullptr) {
                if (pluginInfo->isSelectable()) {
                    Plugin::setLoad(pluginName, pInclude);

                    status = QString("%1 the GUI version of OpenCOR").arg(pInclude?
                                                                              "included to":
                                                                              "excluded from");
                } else {
                    status = QString("cannot be directly %1").arg(pInclude?
                                                                      "included":
                                                                      "excluded");
                }
            } else {
                status = QString("plugin information not found%1").arg(errorMessage.isEmpty()?
                                                                           QString():
                                                                           QString(" (%1)").arg(errorMessage));
            }
        } else {
            status = "unknown plugin";
        }

        std::cout << " - " << pluginName.toStdString() << ": " << status.toStdString() << "." << std::endl;
    }
}

//==============================================================================

void CliApplication::about() const
{
    // Output some information about OpenCOR

    version();

    std::cout << prettyProductName().toStdString() << std::endl;
    std::cout << copyright().toStdString() << std::endl;
    std::cout << std::endl;
    std::cout << applicationDescription(false).toStdString() << std::endl;
}

//==============================================================================

bool CliApplication::command(const QString &pCommand,
                             const QStringList &pArguments, int &pRes) const
{
    // Determine whether the command is to be executed by all the CLI plugins or
    // only a given CLI plugin

    static const QString CommandSeparator = "::";

    QString commandName = pCommand;
    QString commandPlugin = commandName;
    int commandSeparatorPosition = commandName.indexOf(CommandSeparator);

    if (commandSeparatorPosition != -1) {
        commandPlugin = commandPlugin.remove(commandSeparatorPosition, commandName.length()-commandSeparatorPosition);
        commandName = commandName.remove(0, commandPlugin.length()+CommandSeparator.length());
    } else {
        commandName = QString();
    }

    // Make sure that the plugin to which the command is to be sent exists

    if (!commandPlugin.isEmpty()) {
        bool pluginFound = false;
        bool pluginHasCliSupport = false;

        for (auto plugin : mPluginManager->loadedPlugins()) {
            if (commandPlugin == plugin->name()) {
                pluginFound = true;
                pluginHasCliSupport = qobject_cast<CliInterface *>(plugin->instance()) != nullptr;

                break;
            }
        }

        if (!pluginFound) {
            std::cout << "The " << commandPlugin.toStdString() << " plugin could not be found." << std::endl;

            pRes = -1;

            return true;
        }

        if (!pluginHasCliSupport) {
            std::cout << "The " << commandPlugin.toStdString() << " plugin does not support the execution of commands." << std::endl;

            pRes = -1;

            return true;
        }
    }

    // Make sure that we have at least one CLI-enabled plugin

    if (mLoadedCliPlugins.isEmpty()) {
        std::cout << "No plugins could be found to run the command." << std::endl;

        pRes = -1;

        return true;
    }

    // Send the command to the plugin(s)

    bool firstPlugin = true;

    for (auto plugin : mLoadedCliPlugins) {
        if (    commandPlugin.isEmpty()
            || (commandPlugin == plugin->name())) {
            if (firstPlugin) {
                firstPlugin = false;
            } else {
                std::cout << std::endl;
            }

            if (!qobject_cast<CliInterface *>(plugin->instance())->executeCommand(commandName, pArguments, pRes)) {
                pRes = -1;
            }
        }
    }

    return true;
}

//==============================================================================

void CliApplication::exclude(const QStringList &pPluginNames) const
{
    // Exclude the given plugins from the GUI version of OpenCOR

    includePlugins(pPluginNames, false);
}

//==============================================================================

void CliApplication::help() const
{
    // Output some help

    std::cout << "Usage: " << qAppName().toStdString()
              << " [-a|--about] [-c|--command [<plugin>]::<command> [<argument> ...]] [-e|--exclude <plugins>] [-h|--help] [-i|--include <plugins>] [-p|--plugins] [-r|--reset] [-s|--status] [-v|--version] [<files>]"
              << std::endl;
    std::cout << " -a, --about     Display some information about OpenCOR"
              << std::endl;
    std::cout << " -c, --command   Send a command to one or all the CLI plugins"
              << std::endl;
    std::cout << " -e, --exclude   Exclude the given plugin(s)"
              << std::endl;
    std::cout << " -h, --help      Display this help information"
              << std::endl;
    std::cout << " -i, --include   Include the given plugin(s)"
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

void CliApplication::include(const QStringList &pPluginNames) const
{
    // Include the given plugins to the GUI version of OpenCOR

    includePlugins(pPluginNames);
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

    for (auto plugin : mLoadedCliPlugins) {
        // Retrieve the CLI plugin and its default description

        QString pluginInfo = plugin->name();
        QString pluginDesc = plainString(plugin->info()->description());

        if (!pluginDesc.isEmpty()) {
            pluginInfo += ": "+pluginDesc;
        }

        // Add the plugin information to our list

        pluginsInfo << pluginInfo;
    }

    // Now, we can output the plugin information in alphabetical order

    pluginsInfo.sort(Qt::CaseInsensitive);

    if (pluginsInfo.count() == 1) {
        std::cout << "The following CLI plugin is available:" << std::endl;
    } else {
        std::cout << "The following CLI plugins are available:" << std::endl;
    }

    for (const auto &pluginInfo : pluginsInfo) {
        std::cout << " - " << pluginInfo.toStdString() << std::endl;
    }
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

    for (auto plugin : mPluginManager->plugins()) {
        // Retrieve the plugin and its status

        QString pluginInfo = plugin->name()+": ";

        // Retrieve the plugin's status

        switch (plugin->status()) {
        case Plugin::Status::NotWanted:
            pluginInfo += "the plugin is not wanted.";

            break;
        case Plugin::Status::NotNeeded:
            pluginInfo += "the plugin is not needed.";

            break;
        case Plugin::Status::Loaded:

            pluginInfo += "the plugin is loaded and fully functional.";

            break;
        case Plugin::Status::NotLoaded:
            pluginInfo += QString("the plugin could not be loaded due to the following problem: %1.").arg(formatMessage(plugin->statusErrors()));

            break;
        case Plugin::Status::NotPlugin:
            pluginInfo += "this is not a plugin.";

            break;
        case Plugin::Status::OldPlugin:
            pluginInfo += "the plugin could not be loaded (one or several of the interfaces it supports are too old).";

            break;
        case Plugin::Status::NotCorePlugin:
            pluginInfo += "the plugin claims to be the core plugin, but it is not.";

            break;
        case Plugin::Status::InvalidCorePlugin:
            pluginInfo += "the plugin claims to be the core plugin, but it does not support the core interface.";

            break;
        case Plugin::Status::NeededSelectablePlugin:
            pluginInfo += "the plugin claims to be selectable, but it is needed.";

            break;
        case Plugin::Status::NotCliPluginNoCliSupport:
            pluginInfo += "the plugin supports the CLI interface, but it does not claim to be CLI-capable.";

            break;
        case Plugin::Status::NotCliPluginNoCliInterface:
            pluginInfo += "the plugin claims to be CLI-capable, but it does not support the CLI interface.";

            break;
        case Plugin::Status::MissingOrInvalidDependencies:
            if (plugin->statusErrorsCount() == 1) {
                pluginInfo += QString("the plugin could not be loaded due to the %1 plugin being missing or invalid.").arg(plugin->statusErrors());
            } else {
                pluginInfo += QString("the plugin could not be loaded due to missing or invalid plugins:\n%1").arg(plugin->statusErrors());
            }

            break;
        }

        // Add the plugin information to our list

        pluginsInfo << pluginInfo;
    }

    // Now, we can output the plugin information in alphabetical order

    pluginsInfo.sort(Qt::CaseInsensitive);

    if (pluginsInfo.count() == 1) {
        std::cout << "The following plugin is available:" << std::endl;
    } else {
        std::cout << "The following plugins are available:" << std::endl;
    }

    for (const auto &pluginInfo : pluginsInfo) {
        std::cout << " - " << pluginInfo.toStdString() << std::endl;
    }
}

//==============================================================================

void CliApplication::version() const
{
    // Output the version of OpenCOR

    std::cout << OpenCOR::version().toStdString() << std::endl;
}

//==============================================================================

bool CliApplication::run(int &pRes)
{
    // See what needs doing with the CLI options, if anything

    pRes = 0;   // By default, everything is fine

    enum Option {
        NoOption,
        AboutOption,
        CommandOption,
        ExcludeOption,
        HelpOption,
        IncludeOption,
        PluginsOption,
        ResetOption,
        StatusOption,
        VersionOption
    };

    Option option = NoOption;

    QStringList appArguments = qApp->arguments();
    QStringList arguments = QStringList();

    appArguments.removeFirst();
    // Note: we remove the first argument since it corresponds to the full path
    //       to our executable, which we are not interested in...

    static const QString A = "-a"; static const QString About   = "--about";
    static const QString C = "-c"; static const QString Command = "--command";
    static const QString E = "-e"; static const QString Exclude = "--exclude";
    static const QString H = "-h"; static const QString Help    = "--help";
    static const QString I = "-i"; static const QString Include = "--include";
    static const QString P = "-p"; static const QString Plugins = "--plugins";
    static const QString R = "-r"; static const QString Reset   = "--reset";
    static const QString S = "-s"; static const QString Status  = "--status";
    static const QString V = "-v"; static const QString Version = "--version";

    for (const auto &appArgument : appArguments) {
        if (   (option == CommandOption)
            || (option == ExcludeOption)
            || (option == IncludeOption)) {
            // Keep track of the arguments passed to a command, an exclude or an
            // include call

            arguments << appArgument;
        } else if ((appArgument == A) || (appArgument == About)) {
            if (option == NoOption) {
                option = AboutOption;
            } else {
                pRes = -1;
            }
        } else if ((appArgument == C) || (appArgument == Command)) {
            if (option == NoOption) {
                option = CommandOption;
            } else {
                pRes = -1;
            }
        } else if ((appArgument == E) || (appArgument == Exclude)) {
            if (option == NoOption) {
                option = ExcludeOption;
            } else {
                pRes = -1;
            }
        } else if ((appArgument == H) || (appArgument == Help)) {
            if (option == NoOption) {
                option = HelpOption;
            } else {
                pRes = -1;
            }
        } else if ((appArgument == I) || (appArgument == Include)) {
            if (option == NoOption) {
                option = IncludeOption;
            } else {
                pRes = -1;
            }
        } else if ((appArgument == P) || (appArgument == Plugins)) {
            if (option == NoOption) {
                option = PluginsOption;
            } else {
                pRes = -1;
            }
        } else if ((appArgument == R) || (appArgument == Reset)) {
            if (option == NoOption) {
                option = ResetOption;
            } else {
                pRes = -1;
            }
        } else if ((appArgument == S) || (appArgument == Status)) {
            if (option == NoOption) {
                option = StatusOption;
            } else {
                pRes = -1;
            }
        } else if ((appArgument == V) || (appArgument == Version)) {
            if (option == NoOption) {
                option = VersionOption;
            } else {
                pRes = -1;
            }
        } else if (appArgument.startsWith('-')) {
            // The user provided at least one unknown option

            pRes = -1;

            break;
        }
    }

    // Handle the option the user requested, if any

    if (pRes == 0) {
        switch (option) {
        case NoOption:
            return false;
        case AboutOption:
            if (appArguments.count() != 1) {
                pRes = -1;

                help();
            } else {
                about();
            }

            break;
        case CommandOption:
            // Make sure that we have at least one argument (which would be the
            // command itself) before loading the plugins and then sending the
            // command to the plugin(s)

            if (arguments.isEmpty()) {
                pRes = -1;

                help();
            } else {
                loadPlugins();

                QString command = arguments.first();

                arguments.removeFirst();

                if (!CliApplication::command(command, arguments, pRes)) {
                    pRes = -1;

                    help();
                }
            }

            break;
        case ExcludeOption:
            // Make sure that we have at least one argument (i.e. the name of a
            // plugin)

            if (arguments.isEmpty()) {
                pRes = -1;

                help();
            } else {
                exclude(arguments);
            }

            break;
        case HelpOption:
            if (appArguments.count() != 1) {
                pRes = -1;

                help();
            } else {
                help();
            }

            break;
        case IncludeOption:
            // Make sure that we have at least one argument (i.e. the name of a
            // plugin)

            if (arguments.isEmpty()) {
                pRes = -1;

                help();
            } else {
                include(arguments);
            }

            break;
        case PluginsOption:
            if (appArguments.count() != 1) {
                pRes = -1;

                help();
            } else {
                loadPlugins();

                plugins();
            }

            break;
        case ResetOption:
            if (appArguments.count() != 1) {
                pRes = -1;

                help();
            } else {
                reset();
            }

            break;
        case StatusOption:
            if (appArguments.count() != 1) {
                pRes = -1;

                help();
            } else {
                loadPlugins();

                status();
            }

            break;
        case VersionOption:
            if (appArguments.count() != 1) {
                pRes = -1;

                help();
            } else {
                version();
            }

            break;
        }
    } else {
        help();
    }

    return true;
}

//==============================================================================

} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
