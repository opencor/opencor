//==============================================================================
// Some common methods between the CLI and GUI version of OpenCOR
//==============================================================================

#include "cliapplication.h"
#include "common.h"
#include "utils.h"

//==============================================================================

#include <iostream>

//==============================================================================

#include <QCoreApplication>
#include <QFileInfo>

//==============================================================================

namespace OpenCOR {

//==============================================================================

void error(QCoreApplication *pApp, const QString &pMsg)
{
    std::cout << qPrintable(getAppVersion(pApp)) << std::endl;
    std::cout << std::endl;
    std::cout << "Error: " << qPrintable(pMsg) << std::endl;
}

//==============================================================================

void initApplication(QCoreApplication *pApp)
{
    // Set the name of the application

    pApp->setApplicationName(QFileInfo(pApp->applicationFilePath()).baseName());

    // Retrieve and set the version of the application

    QFile versionFile(":app_version");

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
    // Create our CLI application object

    CliApplication *cliApp = new CliApplication(pApp);

    // Run our CLI application

    return cliApp->run(pRes);
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
