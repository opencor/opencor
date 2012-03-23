//==============================================================================
// CellMLAnnotationView plugin
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWPLUGIN_H
#define CELLMLANNOTATIONVIEWPLUGIN_H

//==============================================================================

#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

PLUGININFO_FUNC CellMLAnnotationViewPluginInfo();

//==============================================================================

class CellMLAnnotationViewPlugin : public QObject, public GuiInterface,
                                   public I18nInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    explicit CellMLAnnotationViewPlugin();

    virtual QWidget * newViewWidget(const QString &pFileName);
    virtual QString viewName(const int &pViewIndex);
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
