//==============================================================================
// QScintilla plugin
//==============================================================================

#include "qscintillaplugin.h"

//==============================================================================

namespace OpenCOR {
namespace QScintilla {

//==============================================================================

PLUGININFO_FUNC QScintillaPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to get access to <a href=\"http://www.riverbankcomputing.co.uk/software/qscintilla/\">QScintilla</a>");
    descriptions.insert("fr", "Une extension pour avoir accès à <a href=\"http://www.riverbankcomputing.co.uk/software/qscintilla/\">QScintilla</a>");

    return new PluginInfo(PluginInfo::V001,
                          PluginInfo::Gui,
                          PluginInfo::ThirdParty,
                          false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(QScintilla, QScintillaPlugin)

//==============================================================================

}   // namespace QJson
}   // namespace QScintilla

//==============================================================================
// End of file
//==============================================================================
