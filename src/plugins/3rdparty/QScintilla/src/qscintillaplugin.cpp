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

    descriptions.insert("en", QString::fromUtf8("A plugin to access <a href=\"http://www.riverbankcomputing.co.uk/software/qscintilla/\">QScintilla</a>"));
    descriptions.insert("fr", QString::fromUtf8("Une extension pour accéder à <a href=\"http://www.riverbankcomputing.co.uk/software/qscintilla/\">QScintilla</a>"));

    return new PluginInfo(PluginInfo::FormatVersion001,
                          PluginInfo::Gui,
                          PluginInfo::ThirdParty,
                          false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

}   // namespace QJson
}   // namespace QScintilla

//==============================================================================
// End of file
//==============================================================================
