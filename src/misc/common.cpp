//==============================================================================
// Some common methods between the command line and GUI version of OpenCOR
//==============================================================================

#include "common.h"
#include "utils.h"

//==============================================================================

#include <iostream>

//==============================================================================

#include <QCoreApplication>
#include <QFileInfo>
#include <QStringList>

//==============================================================================

namespace OpenCOR {

//==============================================================================

void usage(QCoreApplication *pApp)
{
    std::cout << "Usage: " << qPrintable(pApp->applicationName())
              << " [-a|--about] [-c|--command [<plugin>::]<command> <options>] [-h|--help] [-p|--plugins] [-v|--version] [<files>]"
              << std::endl;
    std::cout << " -a, --about     Display OpenCOR about information"
              << std::endl;
    std::cout << " -c, --command   Execute a given command"
              << std::endl;
    std::cout << " -h, --help      Display this help information"
              << std::endl;
    std::cout << " -p, --plugins   Display the list of available plugins"
              << std::endl;
    std::cout << " -v, --version   Display OpenCOR version information"
              << std::endl;
}

//==============================================================================

void version(QCoreApplication *pApp)
{
    std::cout << qPrintable(getAppVersion(pApp)) << std::endl;
}

//==============================================================================

void about(QCoreApplication *pApp)
{
    version(pApp);

    std::cout << qPrintable(getOsName()) << std::endl;
    std::cout << qPrintable(getAppCopyright(false)) << std::endl;
    std::cout << std::endl;
    std::cout << qPrintable(pApp->applicationName())
              << " is a cross-platform CellML-based modelling environment"
              << " which can be" << std::endl;
    std::cout << "used to organise, edit, simulate and analyse CellML files."
              << std::endl;
}

//==============================================================================

void plugins()
{
//---GRY--- TO BE DONE...

    std::cout << "The following plugins are available:" << std::endl;
    std::cout << " - ..." << std::endl;
}

//==============================================================================

int command(const QStringList pArguments)
{
//---GRY--- TO BE DONE...

    // Make sure that we have at least one argument

    if (!pArguments.count())
        return -1;

    // Determine whether the command is to be executed by all plugins or only a
    // given plugin

    static const QString commandSeparator = "::";
    QString commandName = pArguments.first();
    QString commandPlugin = commandName;
    int commandSeparatorPosition = commandName.indexOf(commandSeparator);

    if (commandSeparatorPosition != -1) {
        commandPlugin = commandPlugin.remove(commandSeparatorPosition, commandName.length()-commandSeparatorPosition);
        commandName = commandName.remove(0, commandPlugin.length()+commandSeparator.length());
    } else {
        commandPlugin = QString();
    }

    // Make sure that we have a command name

    if (commandName.isEmpty())
        return -1;

    // Some debug information...

    std::cout << "A command is to be executed:" << std::endl;
    std::cout << " - Target: " << qPrintable(commandPlugin.isEmpty()?"all":commandPlugin) << std::endl;
    std::cout << " - Command: " << qPrintable(commandName) << std::endl;

    int iMax = pArguments.count();

    if (iMax == 1) {
        std::cout << " - Options: /" << std::endl;
    } else {
        std::cout << " - Options:" << std::endl;

        for (int i = 1, iMax = pArguments.count(); i < iMax; ++i)
            std::cout << "    - " << qPrintable(pArguments.at(i)) << std::endl;
    }

    // Everything went fine, so...

    return 0;
}

//==============================================================================

void error(QCoreApplication *pApp, const QString &pMsg)
{
    version(pApp);

    std::cout << std::endl;
    std::cout << "Error: " << qPrintable(pMsg) << std::endl;
}

//==============================================================================

void initApplication(QCoreApplication *pApp)
{
    // Set the name of the application

    pApp->setApplicationName(QFileInfo(pApp->applicationFilePath()).baseName());

    // Retrieve and set the version of the application

    QFile versionFile(":version");

    versionFile.open(QIODevice::ReadOnly);

    QString version = QString(versionFile.readLine()).trimmed();

    if (version.endsWith(".0"))
        // There is no actual patch information, so trim it

        version.chop(2);

    versionFile.close();

    pApp->setApplicationVersion(version);
}

//==============================================================================

bool cliApplication(QCoreApplication *pApp, int *pRes)
{
    *pRes = 0;   // By default, everything is fine

    // See what needs doing with the command line options, if anything

    bool helpOption = false;
    bool aboutOption = false;
    bool versionOption = false;
    bool pluginsOption = false;
    bool commandOption = false;

    QStringList commandArguments = QStringList();

    foreach (const QString argument, pApp->arguments())
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
        } else if (argument.startsWith('-')) {
            // The user provided at least one unknown option

            usage(pApp);

            *pRes = -1;

            break;
        } else if (commandOption) {
            // Not an option, so we consider it to be part of a command

            commandArguments << argument;
        }

    // Handle the option the user requested, if any

    if (!*pRes) {
        if (helpOption) {
            usage(pApp);
        } else if (aboutOption) {
            about(pApp);
        } else if (versionOption) {
            version(pApp);
        } else if (pluginsOption) {
            plugins();
        } else if (commandOption) {
            *pRes = command(commandArguments);

            if (*pRes)
                usage(pApp);
        } else {
            // The user didn't provide any command line option which requires
            // running OpenCOR as a CLI application

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
