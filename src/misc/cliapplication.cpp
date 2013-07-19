//==============================================================================
// CLI application
//==============================================================================

#include "cliapplication.h"
#include "coreinterface.h"
#include "pluginmanager.h"
#include "utils.h"

//==============================================================================

#include <iostream>

//==============================================================================

#include <QCoreApplication>
#include <QXmlStreamReader>

//==============================================================================

namespace OpenCOR {

//==============================================================================

CliApplication::CliApplication(QCoreApplication *pApp) :
    mApp(pApp),
    mPluginManager(0)
{
}

//==============================================================================

CliApplication::~CliApplication()
{
    // Delete some internal objects

    delete mPluginManager;
}

//==============================================================================

void CliApplication::usage()
{
    std::cout << "Usage: " << qPrintable(mApp->applicationName())
              << " [-a|--about] [-c|--command [<plugin>::]<command> <options>] [-h|--help] [-p|--plugins] [-v|--version] [<files>]"
              << std::endl;
    std::cout << " -a, --about     Display some information about OpenCOR"
              << std::endl;
    std::cout << " -c, --command   Send a command to one or all the plugins"
              << std::endl;
    std::cout << " -h, --help      Display this help information"
              << std::endl;
    std::cout << " -p, --plugins   Display the list of plugins"
              << std::endl;
    std::cout << " -v, --version   Display the version of OpenCOR"
              << std::endl;
}

//==============================================================================

void CliApplication::version()
{
    std::cout << qPrintable(getAppVersion(mApp)) << std::endl;
}

//==============================================================================

void CliApplication::about()
{
    version();

    std::cout << qPrintable(getOsName()) << std::endl;
    std::cout << qPrintable(getAppCopyright(false)) << std::endl;
    std::cout << std::endl;
    std::cout << qPrintable(mApp->applicationName())
              << " is a cross-platform CellML-based modelling environment"
              << " which can be" << std::endl;
    std::cout << "used to organise, edit, simulate and analyse CellML files."
              << std::endl;
}

//==============================================================================

void CliApplication::loadPlugins()
{
    // Load all the plugins by creating our plugin manager

    mPluginManager = new OpenCOR::PluginManager(mApp, false);
}

//==============================================================================

void CliApplication::plugins()
{
    // Output some information about our CLI-enabled plugins, so first make sure
    // that we have at least one of them

    Plugins loadedCliPlugins = mPluginManager->loadedCliPlugins();

    if (loadedCliPlugins.isEmpty()) {
        std::cout << "Sorry, but no plugins could be found." << std::endl;

        return;
    }

    // First, we retrieve all the plugins information

    QStringList pluginsInfo = QStringList();

    foreach (Plugin *plugin, loadedCliPlugins) {
        // Retrieve the plugin's default description, stripped out of all its
        // HTML (should it have some)
        // Note: we enclose the plugin's default description within an html tag
        //       so that the stripping out can proceed without any problem...

        QXmlStreamReader description("<html>"+plugin->info()->description()+"</html>");
        QString pluginInfo = QString();

        while (!description.atEnd())
            if (description.readNext() == QXmlStreamReader::Characters)
                pluginInfo += description.text();

        // Complete the plugin information

        if (!pluginInfo.isEmpty())
            pluginInfo.prepend(": ");

        pluginInfo.prepend(plugin->name());

        // Add the plugin information to our list

        pluginsInfo << pluginInfo;
    }

    // Now, we can output the plugin information in alphabetical order

    pluginsInfo.sort(Qt::CaseInsensitive);

    if (pluginsInfo.count() == 1)
        std::cout << "The following plugin is loaded:" << std::endl;
    else
        std::cout << "The following plugins are loaded:" << std::endl;

    foreach (const QString pluginInfo, pluginsInfo)
        std::cout << " - " << qPrintable(pluginInfo) << std::endl;
}

//==============================================================================

bool CliApplication::command(const QStringList pArguments, int *pRes)
{
    // Send a command to one or all the plugins, so first retrieve the list of
    // loaded CLI plugins

    Plugins loadedCliPlugins = mPluginManager->loadedCliPlugins();

    // Make sure that we have at least one argument

    if (!pArguments.count())
        return false;

    // Determine whether the command is to be executed by all plugins or only a
    // given plugin

    static const QString commandSeparator = "::";
    QString commandName = pArguments.first();
    QString commandPlugin = commandName;
    int commandSeparatorPosition = commandName.indexOf(commandSeparator);

    if (commandSeparatorPosition != -1) {
        commandPlugin = commandPlugin.remove(commandSeparatorPosition, commandName.length()-commandSeparatorPosition);
        commandName = commandName.remove(0, commandPlugin.length()+commandSeparator.length());

        // Make sure that the plugin to which the command is to be sent exists

        if (!commandPlugin.isEmpty()) {
            bool pluginFound = false;
            bool pluginHasCliSupport = false;

            foreach (Plugin *plugin, mPluginManager->loadedPlugins())
                if (!commandPlugin.compare(plugin->name())) {
                    pluginFound = true;
                    pluginHasCliSupport = plugin->info()->hasCliSupport();

                    break;
                }

            if (!pluginFound) {
                std::cout << "Sorry, but the " << qPrintable(commandPlugin) << " plugin could not be found." << std::endl;

                return true;
            } else if (!pluginHasCliSupport) {
                std::cout << "Sorry, but the " << qPrintable(commandPlugin) << " plugin does not support the execution of commands." << std::endl;

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

    if (loadedCliPlugins.isEmpty()) {
        std::cout << "Sorry, but no plugins could be found to run the command." << std::endl;

        return true;
    }

    // Send the command to the plugin(s)

    foreach (Plugin *plugin, loadedCliPlugins)
        if (    commandPlugin.isEmpty()
            || !commandPlugin.compare(plugin->name())) {
            int res = 0;

            QStringList arguments = pArguments;

            arguments.removeFirst();
            // Note: since the first argument corresponds to the command
            //       itself...

            qobject_cast<CoreInterface *>(plugin->instance())->runCliCommand(commandName, arguments, &res);

            if (res)
                *pRes = -1;
        }

    return true;
}

//==============================================================================

bool CliApplication::run(int *pRes)
{
    *pRes = 0;   // By default, everything is fine

    // See what needs doing with the CLI options, if anything

    bool helpOption = false;
    bool aboutOption = false;
    bool versionOption = false;
    bool pluginsOption = false;
    bool commandOption = false;

    QStringList commandArguments = QStringList();

    foreach (const QString argument, mApp->arguments())
        if (!argument.compare("-h") || !argument.compare("--help")) {
            helpOption = true;
        } else if (!argument.compare("-a") || !argument.compare("--about")) {
            aboutOption = true;
        } else if (!argument.compare("-v") || !argument.compare("--version")) {
            versionOption = true;
        } else if (!argument.compare("-p") || !argument.compare("--plugins")) {
            pluginsOption = true;
        } else if (!argument.compare("-c") || !argument.compare("--command")) {
            commandOption = true;
        } else if (argument.startsWith("-")) {
            // The user provided at least one unknown option

            usage();

            *pRes = -1;

            break;
        } else if (commandOption) {
            // Not an option, so we consider it to be part of a command

            commandArguments << argument;
        }

    // Handle the option the user requested, if any

    if (!*pRes) {
        if (helpOption) {
            usage();
        } else if (aboutOption) {
            about();
        } else if (versionOption) {
            version();
        } else if (pluginsOption) {
            loadPlugins();

            plugins();
        } else if (commandOption) {
            // Make sure that we have at least one argument (which would be the
            // command itself) before loading the plugins and then sending the
            // command to the plugin(s)

            if (commandArguments.isEmpty()) {
                usage();
            } else {
                loadPlugins();

                if (!command(commandArguments, pRes))
                    usage();
            }
        } else {
            // The user didn't provide any option which requires running OpenCOR
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
