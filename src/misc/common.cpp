#include "common.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QStringList>

#include <QxtCommandOptions>

#include <iostream>

void usage(const QString& pAppBasename)
{
    std::cout << "Usage: " << pAppBasename.toAscii().constData() << " [OPTION]... [FILE]..." << std::endl;
    std::cout << "Start " << pAppBasename.toAscii().constData() << " and open the FILE(s) passed as argument(s)." << std::endl;
    std::cout << std::endl;
    std::cout << " -h, --help   Display this help message and exit" << std::endl;
    std::cout << std::endl;
    std::cout << "Mandatory or optional arguments to long options are also mandatory or optional" << std::endl;
    std::cout << "for any corresponding short options." << std::endl;
}

void initApplication(QCoreApplication& pApp)
{
    // Set the name of the organization

    pApp.setOrganizationName("World");

    // Set the name of the application

    pApp.setApplicationName(QFileInfo(pApp.applicationFilePath()).baseName());

    // Retrieve and set the version of the application

    QFile versionFile(":version");

    versionFile.open(QIODevice::ReadOnly);

    QString version = QString(versionFile.readLine()).trimmed();

    if (version.endsWith(".0"))
        // There is no actual patch information, so trim it

        version.truncate(version.length()-2);

    versionFile.close();

    pApp.setApplicationVersion(version);
}

bool consoleApplication(QCoreApplication& pApp, int& pRes)
{
    pRes = 0;   // By default, everything is fine

    // Determine the basename for OpenCOR

    QString appBasename = QFileInfo(pApp.applicationFilePath()).baseName();

    // Specify the type of command line options that are allowed
    // Note #1: we don't rely on the QxtCommandOptions::showUsage() method
    // Note #2: we don't distinguish between Windows and Linux / Mac OS X when
    //          it comes to the formatting of the command line options

    QxtCommandOptions cmdLineOptions;

    cmdLineOptions.setFlagStyle(QxtCommandOptions::DoubleDash);
    cmdLineOptions.setParamStyle(QxtCommandOptions::SpaceAndEquals);

    cmdLineOptions.add("help");
    cmdLineOptions.alias("help", "h");

    // Parse the command line options

    cmdLineOptions.parse(pApp.arguments());

    // See what needs doing with the command line options, if anything

    if (cmdLineOptions.count("help"))
    {
        // The user wants to know how to use OpenCOR from the console, so...

        usage(appBasename);

        return true;
    }
    else if (cmdLineOptions.getUnrecognizedWarning().count())
    {
        // The user provided OpenCOR with wrong command line options, so...

        usage(appBasename);

        pRes = -1;

        return true;
    }
    else
        // The user didn't provide any command line options that requires
        // running OpenCOR as a console application, so...

        return false;
}
