//==============================================================================
// RawCellMLView plugin
//==============================================================================

#ifndef RAWCELLMLVIEWPLUGIN_H
#define RAWCELLMLVIEWPLUGIN_H

//==============================================================================

#include "coreinterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace RawCellMLView {

//==============================================================================

PLUGININFO_FUNC RawCellMLViewPluginInfo();

//==============================================================================

class RawCellmlViewWidget;

//==============================================================================

class RawCellMLViewPlugin : public QObject, public CoreInterface,
                            public GuiInterface, public I18nInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    explicit RawCellMLViewPlugin();

    virtual void initialize();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual QWidget * viewWidget(const QString &pFileName);
    virtual bool deleteViewWidget(const QString &pFileName);
    virtual QString viewName();

private:
    RawCellmlViewWidget *mViewWidget;
};

//==============================================================================

}   // namespace RawCellMLView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
