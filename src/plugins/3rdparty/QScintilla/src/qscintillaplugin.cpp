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

    descriptions.insert("en", QString::fromUtf8("a plugin to access <a href=\"http://www.riverbankcomputing.co.uk/software/qscintilla/\">QScintilla</a>."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder à <a href=\"http://www.riverbankcomputing.co.uk/software/qscintilla/\">QScintilla</a>."));

    return new PluginInfo(PluginInfo::ThirdParty,
                          QStringList(),
                          descriptions);
}

//==============================================================================

}   // namespace QScintilla
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
