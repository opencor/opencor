//==============================================================================
// SingleCellView plugin
//==============================================================================

#ifndef SINGLECELLVIEWPLUGIN_H
#define SINGLECELLVIEWPLUGIN_H

//==============================================================================

#include "coreinterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

PLUGININFO_FUNC SingleCellViewPluginInfo();

//==============================================================================

class SingleCellViewWidget;

//==============================================================================

class SingleCellViewPlugin : public QObject, public CoreInterface,
                             public GuiInterface, public I18nInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.SingleCellViewPlugin" FILE "singlecellviewplugin.json")

    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    explicit SingleCellViewPlugin();

    virtual void initialize();

    virtual void initializationsDone(const Plugins &pLoadedPlugins);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual QWidget * viewWidget(const QString &pFileName);
    virtual QWidget * removeViewWidget(const QString &pFileName);
    virtual QString viewName() const;

    virtual QIcon fileTabIcon(const QString &pFileName) const;

    virtual void fileOpened(const QString &pFileName);
    virtual void fileRenamed(const QString &pOldFileName,
                             const QString &pNewFileName);

    virtual void retranslateUi();

private:
    SingleCellViewWidget *mViewWidget;
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
