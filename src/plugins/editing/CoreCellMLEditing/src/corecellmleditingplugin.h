#ifndef CORECELLMLEDITINGPLUGIN_H
#define CORECELLMLEDITINGPLUGIN_H

#include "coreinterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

namespace OpenCOR {
namespace CoreCellMLEditing {

PLUGININFO_FUNC CoreCellMLEditingPluginInfo();

class CoreCellMLEditingPlugin : public QObject, public CoreInterface,
                                public GuiInterface, public I18nInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    virtual void initialize();

protected:
    virtual void retranslateUi();

    virtual void updateActions();

private:
    QAction *mFileNewCellml1_0Action;
    QAction *mFileNewCellml1_1Action;
};

} }

#endif
