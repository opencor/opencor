//==============================================================================
// Some useful test-related functions
//==============================================================================

#include "plugin.h"
#include "testutils.h"

//==============================================================================

#ifdef Q_OS_MAC
    #pragma GCC diagnostic ignored "-Wunused-private-field"
#endif

#include <QtTest/QtTest>

#ifdef Q_OS_MAC
    #pragma GCC diagnostic warning "-Wunused-private-field"
#endif

//==============================================================================

namespace OpenCOR {

//==============================================================================

void loadPlugin(const QString &pPluginName)
{
    QStringList plugins = QStringList() << OpenCOR::Plugin::requiredPlugins(".", pPluginName)
                                        << pPluginName;

    foreach (const QString &plugin, plugins) {
        QPluginLoader pluginLoader(OpenCOR::PluginPrefix+plugin+OpenCOR::PluginExtension);

        QVERIFY2(pluginLoader.load(),
                 qPrintable(QString("the %1 plugin could not be loaded").arg(plugin)));
    }
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
