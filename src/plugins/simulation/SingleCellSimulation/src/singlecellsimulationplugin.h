//==============================================================================
// SingleCellSimulation plugin
//==============================================================================

#ifndef SINGLECELLSIMULATIONPLUGIN_H
#define SINGLECELLSIMULATIONPLUGIN_H

//==============================================================================

#include "coreinterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulation {

//==============================================================================

PLUGININFO_FUNC SingleCellSimulationPluginInfo();

//==============================================================================

class SingleCellSimulationView;

//==============================================================================

class SingleCellSimulationPlugin : public QObject, public CoreInterface,
                                   public GuiInterface, public I18nInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    explicit SingleCellSimulationPlugin();

    virtual void initialize();

    virtual void initializationsDone(const Plugins &pLoadedPlugins);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual QWidget * viewWidget(const QString & pFileName, const int &);
    virtual QString viewName(const int &pViewIndex);

    virtual void retranslateUi();

private:
    SingleCellSimulationView *mSingleCellSimulationView;
};

//==============================================================================

}   // namespace SingleCellSimulation
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
