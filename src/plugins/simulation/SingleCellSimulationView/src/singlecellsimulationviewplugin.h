//==============================================================================
// SingleCellSimulationView plugin
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWPLUGIN_H
#define SINGLECELLSIMULATIONVIEWPLUGIN_H

//==============================================================================

#include "coreinterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

PLUGININFO_FUNC SingleCellSimulationViewPluginInfo();

//==============================================================================

class SingleCellSimulationViewWidget;

//==============================================================================

class SingleCellSimulationViewPlugin : public QObject, public CoreInterface,
                                       public GuiInterface, public I18nInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    explicit SingleCellSimulationViewPlugin();

    virtual void initialize();

    virtual void initializationsDone(const Plugins &pLoadedPlugins);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual QWidget * viewWidget(const QString &pFileName);
    virtual QString viewName();

    virtual void retranslateUi();

private:
    SingleCellSimulationViewWidget *mViewWidget;
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
