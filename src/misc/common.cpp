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
              << " [-a|--about] [-h|--help] [-v|--version] [<files>]"
              << std::endl;
    std::cout << " -a, --about     Display OpenCOR about information"
              << std::endl;
    std::cout << " -h, --help      Display this help information"
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

bool consoleApplication(QCoreApplication *pApp, int *pRes)
{
    *pRes = 0;   // By default, everything is fine

    // See what needs doing with the command line options, if anything

    bool helpOption = false;
    bool aboutOption = false;
    bool versionOption = false;

    foreach (const QString argument, pApp->arguments())
        if (!argument.compare("-h") || !argument.compare("--help")) {
            helpOption = true;
        } else if (!argument.compare("-a") || !argument.compare("--about")) {
            aboutOption = true;
        } else if (!argument.compare("-v") || !argument.compare("--version")) {
            versionOption = true;
        } else if (argument.startsWith('-')) {
            // The user provided at least one unknown option

            usage(pApp);

            *pRes = -1;

            break;
        }

    // Handle the option the user requested, if any

    if (!*pRes) {
        if (helpOption)
            usage(pApp);
        else if (aboutOption)
            about(pApp);
        else if (versionOption)
            version(pApp);
        else
            // The user didn't provide any command line option which requires
            // running OpenCOR as a console application

            return false;
    }

    return true;
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
