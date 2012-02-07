//==============================================================================
// CellMLAnnotation plugin
//==============================================================================

#ifndef CELLMLANNOTATIONPLUGIN_H
#define CELLMLANNOTATIONPLUGIN_H

//==============================================================================

#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotation {

//==============================================================================

PLUGININFO_FUNC CellMLAnnotationPluginInfo();

//==============================================================================

class CellMLAnnotationPlugin : public QObject, public GuiInterface,
                               public I18nInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    explicit CellMLAnnotationPlugin();

    virtual QWidget * newViewWidget(const QString &pFileName);
    virtual QString viewName(const int &pViewIndex);
};

//==============================================================================

}   // namespace CellMLAnnotation
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
