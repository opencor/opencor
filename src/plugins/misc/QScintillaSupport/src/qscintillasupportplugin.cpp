//==============================================================================
// QScintillaSupport plugin
//==============================================================================

#include "qscintillasupportplugin.h"

//==============================================================================

namespace OpenCOR {
namespace QScintillaSupport {

//==============================================================================

PLUGININFO_FUNC QScintillaSupportPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to support <a href=\"http://www.riverbankcomputing.co.uk/software/qscintilla/\">QScintilla</a>");
    descriptions.insert("fr", "Une extension pour supporter <a href=\"http://www.riverbankcomputing.co.uk/software/qscintilla/\">QScintilla</a>");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::Gui,
                      PluginInfo::Miscellaneous,
                      false,
                      QStringList() << "Core" << "QScintilla",
                      descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(QScintillaSupport, QScintillaSupportPlugin)

//==============================================================================

}   // namespace QScintillaSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
