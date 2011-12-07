//==============================================================================
// QScintilla class
//==============================================================================

#include "commonwidget.h"
#include "qscintilla.h"

//==============================================================================

#include "Qsci/qscilexer.h"

//==============================================================================

namespace OpenCOR {
namespace QScintillaSupport {

//==============================================================================

QScintilla::QScintilla(const QString &pContents, const bool &pReadOnly,
                       QsciLexer *pLexer, QWidget *pParent) :
    QsciScintilla(pParent)
{
    // Remove the frame around our Scintilla editor

    setFrameShape(QFrame::NoFrame);

    // Remove the margin in our Scintilla editor

    setMarginWidth(1, 0);

    // Associate a lexer to our Scintilla editor, should one be available

    if (pLexer) {
        // A lexer was provided, so specify its fonts and associate it to our
        // Scintilla editor

        QFont defaultFont = QFont(OpenCOR::Core::DefaultFontFamily,
                                  OpenCOR::Core::DefaultFontSize);

        pLexer->setDefaultFont(defaultFont);
        pLexer->setFont(defaultFont);

        setLexer(pLexer);

        // Specify the type of tree folding to be used. Some lexers may indeed
        // use that feature, so...

        setFolding(QsciScintilla::BoxedTreeFoldStyle);
    } else {
        // No lexer was provided, so simply specify a default font family and
        // size for our Scintilla editor

        setFont(QFont(OpenCOR::Core::DefaultFontFamily,
                      OpenCOR::Core::DefaultFontSize));
    }

    // Set the contents of the Scintilla editor and specify it is read-only

    setText(pContents);
    setReadOnly(pReadOnly);
}

//==============================================================================

}   // namespace QScintillaSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
