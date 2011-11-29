//==============================================================================
// CoreSimulation plugin
//==============================================================================

#ifndef CORESIMULATIONPLUGIN_H
#define CORESIMULATIONPLUGIN_H

//==============================================================================

#include "coreinterface.h"
#include "guiinterface.h"
#include "plugininfo.h"

//==============================================================================

#include <QVector>

//==============================================================================

class QwtPlot;

//==============================================================================

namespace OpenCOR {
namespace CoreSimulation {

//==============================================================================

PLUGININFO_FUNC CoreSimulationPluginInfo();

class CoreSimulationPlugin : public QObject, public CoreInterface,
                             public GuiInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)

public:
    virtual void initialize();

    virtual QWidget * viewWidget(const QString &, const int &);

private:
    QwtPlot *mSimulationView;

    QVector<double> mXData;
    QVector<double> mYData;

    void testCellml();
    void testLlvmJit();

    void usePrecomputedTestCellmlResults();
};

//==============================================================================

}   // namespace CoreSimulation
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
