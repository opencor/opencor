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

    descriptions.insert("en", QString::fromUtf8("a plugin to support <a href=\"http://www.riverbankcomputing.co.uk/software/qscintilla/\">QScintilla</a>."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour supporter <a href=\"http://www.riverbankcomputing.co.uk/software/qscintilla/\">QScintilla</a>."));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::Miscellaneous,
                          false,
                          QStringList() << "Core" << "QScintilla",
                          descriptions);
}

//==============================================================================

}   // namespace QScintillaSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
