//==============================================================================
// CLI application
//==============================================================================

#ifndef CLIAPPLICATION_H
#define CLIAPPLICATION_H

//==============================================================================

#include <QStringList>

//==============================================================================

class QCoreApplication;

//==============================================================================

namespace OpenCOR {

//==============================================================================

class PluginManager;

//==============================================================================

class CliApplication {
public:
    explicit CliApplication(QCoreApplication *pApp);
    ~CliApplication();

    bool run(int *pRes);

private:
    QCoreApplication *mApp;

    PluginManager *mPluginManager;

    void usage();

    void version();
    void about();

    void loadPlugins();
    void plugins();

    bool command(const QStringList pArguments, int *pRes);
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
