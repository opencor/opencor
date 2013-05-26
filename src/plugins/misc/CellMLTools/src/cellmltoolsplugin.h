//==============================================================================
// CellMLTools plugin
//==============================================================================

#ifndef CELLMLTOOLSPLUGIN_H
#define CELLMLTOOLSPLUGIN_H

//==============================================================================

#include "cellmltoolsglobal.h"
#include "coreinterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLTools {

//==============================================================================

PLUGININFO_FUNC CellMLToolsPluginInfo();

//==============================================================================

static const QString CellmlMimeType = "application/cellml+xml";
static const QString CellmlFileExtension = "cellml";

//==============================================================================

class CellMLToolsPlugin : public QObject, public CoreInterface,
                          public GuiInterface, public I18nInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.CellMLToolsPlugin" FILE "cellmltoolsplugin.json")

    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    virtual void initialize();

    virtual void updateGui(Plugin *pViewPlugin);

    virtual void retranslateUi();

private:
    QMenu *mCellmlExportToMenu;

    QAction *mExportToCellml10Action;
    QAction *mExportToCellml11Action;

private Q_SLOTS:
    void exportToCellml10();
    void exportToCellml11();
};

//==============================================================================

}   // namespace CellMLTools
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
