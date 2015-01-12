/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Lexer for the pretty CellML format
//==============================================================================

#include "prettycellmlviewlexer.h"
#include "qscintillawidget.h"

//==============================================================================

namespace OpenCOR {
namespace PrettyCellMLView {

//==============================================================================

PrettyCellmlViewLexer::PrettyCellmlViewLexer(QObject *pParent) :
    QsciLexerCustom(pParent)
{
    // Some initialisations

    mKeywords = QStringList() << "as" << "comp" << "def" << "model" << "unit";
}

//==============================================================================

const char * PrettyCellmlViewLexer::language() const
{
    // Return the language for our lexer

    return "Pretty CellML";
}

//==============================================================================

QString PrettyCellmlViewLexer::description(int pStyle) const
{
    // Return the given style's description

    switch (pStyle) {
    case Default:
        return QObject::tr("Default");
    case Comment:
        return QObject::tr("Comment");
    case Keyword:
        return QObject::tr("Keyword");
    case Parameters:
        return QObject::tr("Parameters");
    }

    return QString();
}

//==============================================================================

QColor PrettyCellmlViewLexer::color(int pStyle) const
{
    // Return the given style's colour

    switch (pStyle) {
    case Default:
        return QColor(0x00, 0x00, 0x00);
    case Comment:
        return QColor(0x00, 0x7f, 0x00);
    case Keyword:
        return QColor(0x7f, 0x7f, 0x00);
    case Parameters:
        return QColor(0x00, 0x00, 0x7f);
    }

    return QsciLexerCustom::color(pStyle);
}

//==============================================================================

QFont PrettyCellmlViewLexer::font(int pStyle) const
{
    // Return the given style's colour

    QFont res = QsciLexer::font(pStyle);

    switch (pStyle) {
    case Parameters:
        res.setItalic(true);

        break;
    }

    return res;
}

//==============================================================================

void PrettyCellmlViewLexer::doStyleText(int pStart, int pEnd,
                                        const QString &pText)
{
    // Make sure that we are given some text to style

    if (pText.isEmpty())
        return;

    // Check whether the given text contains a comment

    static const QString CommentString = "//";
    static const QString StartComment = "/*";
    static const QString EndComment = "*/";

    int commentPosition = pText.indexOf(CommentString);

    if (commentPosition == -1) {
        // Default styling

        startStyling(pStart, 0x1f);
        setStyling(pEnd-pStart, Default);
    } else {
        // There is a comment to style, so first style everything that is before
        // the comment

        doStyleText(pStart, pStart+commentPosition, pText.left(commentPosition));

        // Now, style everything that is after the comment, if anything, by
        // looking for the end of the line on which the comment is

        QScintillaSupport::QScintillaWidget *currentEditor = qobject_cast<QScintillaSupport::QScintillaWidget *>(editor());
        int eolPosition = pText.indexOf(currentEditor->eolString(), commentPosition);

        if (eolPosition != -1) {
            int start = pStart+eolPosition+currentEditor->eolString().length();

            doStyleText(start, pEnd, pText.right(pEnd-start));
        }

        // Style the comment itself

        int start = pStart+commentPosition;

        startStyling(start, 0x1f);
        setStyling(((eolPosition == -1)?pEnd:pStart+eolPosition)-start, Comment);
    }
}

//==============================================================================

void PrettyCellmlViewLexer::styleText(int pStart, int pEnd)
{
    // Make sure that we have an editor

    if (!editor())
        return;

    // Retrieve the text to style

    char *data = new char[pEnd-pStart+1];

    editor()->SendScintilla(QsciScintilla::SCI_GETTEXTRANGE, pStart, pEnd, data);

    QString text = QString(data);

    delete[] data;

    if (text.isEmpty())
        return;

    // Effectively style our text

    doStyleText(pStart, pEnd, text);
}

//==============================================================================

}   // namespace PrettyCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
