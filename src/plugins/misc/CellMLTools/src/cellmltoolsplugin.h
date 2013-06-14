//==============================================================================
// CellMLTools plugin
//==============================================================================

#ifndef CELLMLTOOLSPLUGIN_H
#define CELLMLTOOLSPLUGIN_H

//==============================================================================

#include "cellmlfile.h"
#include "cellmltoolsglobal.h"
#include "coreinterface.h"
#include "fileinterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLTools {

//==============================================================================

PLUGININFO_FUNC CellMLToolsPluginInfo();

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

    virtual void initializationsDone(const Plugins &pLoadedPlugins);

    virtual void runCliCommand(const QString &pCommand,
                              const QStringList &pArguments, int *pRes);

    virtual void updateGui(Plugin *pViewPlugin, const QString &pFileName);

    virtual void retranslateUi();

private:
    FileTypes mCellmlFileTypes;

    QString mFileName;

    QMenu *mCellmlFileExportToMenu;

    QAction *mExportToCellml10Action;
    QAction *mExportToCellml11Action;

    void exportTo(const CellMLSupport::CellmlFile::Format &pFormat);

    void runHelpCommand();
    void runExportCommand(const QStringList &pArguments, int *pRes);

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
