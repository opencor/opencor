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

#include <QRegularExpression>

//==============================================================================

namespace OpenCOR {
namespace PrettyCellMLView {

//==============================================================================

PrettyCellmlViewLexer::PrettyCellmlViewLexer(QObject *pParent) :
    QsciLexerCustom(pParent),
    mFullText(QString())
{
    // Some initialisations

    mKeywords = QStringList() << "as" << "comp" << "def" << "enddef" << "model" << "unit";
    mCellmlKeywords = QStringList() << "base"
                                    << "ampere" << "candela" << "coulomb" << "joule" << "kelvin" << "kilogram" << "liter" << "litre" << "lumen" << "meter" << "metre" << "mole" << "newton" << "second" << "steradian" << "volt" << "watt" << "weber";
    mParameterKeywords = QStringList() << "pref" << "expo" << "mult" << "off";
    mParameterValueKeywords = QStringList() << "milli";
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
    case CellmlKeyword:
        return QObject::tr("CellML keyword");
    case ParameterGroup:
        return QObject::tr("Parameter group");
    case ParameterKeyword:
        return QObject::tr("Parameter keyword");
    case ParameterValueKeyword:
        return QObject::tr("Parameter value keyword");
    case Number:
        return QObject::tr("Number");
    }

    return QString();
}

//==============================================================================

QColor PrettyCellmlViewLexer::color(int pStyle) const
{
    // Return the given style's colour

    switch (pStyle) {
    case Default:
        return QColor(0x3f, 0x3f, 0x3f);
    case Comment:
        return QColor(0x00, 0x7f, 0x00);
    case Keyword:
    case ParameterGroup:
    case ParameterKeyword:
        return QColor(0x00, 0x00, 0x7f);
    case CellmlKeyword:
    case ParameterValueKeyword:
        return QColor(0x7f, 0x00, 0x7f);
    case Number:
        return QColor(0x7f, 0x7f, 0x00);
    }

    return QsciLexerCustom::color(pStyle);
}

//==============================================================================

QFont PrettyCellmlViewLexer::font(int pStyle) const
{
    // Return the given style's colour

    QFont res = QsciLexer::font(pStyle);

    switch (pStyle) {
    case ParameterGroup:
    case ParameterKeyword:
    case ParameterValueKeyword:
        res.setItalic(true);

        break;
    }

    return res;
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

    mFullText = editor()->text();

    doStyleText(pStart, pEnd, text);
}

//==============================================================================

void PrettyCellmlViewLexer::doStyleText(int pStart, int pEnd,
                                        const QString &pText)
{
    // Make sure that we are given some text to style

    if (pText.trimmed().isEmpty())
        return;

    // Check whether a /* XXX */ comment started before or at the beginning of
    // the given text

    static const QString StartCommentString = "/*";
    static const QString EndCommentString = "*/";
    static const int StartCommentLength = StartCommentString.length();
    static const int EndCommentLength = EndCommentString.length();

    int commentStartPosition = mFullText.lastIndexOf(StartCommentString, pStart+StartCommentLength);

    if (commentStartPosition != -1) {
        // A /* XXX */ comment started before or at the beginning of the given
        // text, so now look for where it ends

        int commentEndPosition = mFullText.indexOf(EndCommentString, commentStartPosition+StartCommentLength);

        if (commentEndPosition == -1)
            commentEndPosition = mFullText.length();

        if ((commentStartPosition <= pStart) && (pStart <= commentEndPosition)) {
            // The beginning of the given text is a comment, so style it

            int end = qMin(pEnd, commentEndPosition+EndCommentLength);

            startStyling(pStart, 0x1f);
            setStyling(end-pStart, Comment);

            // Style everything that is behind the comment, if anything

            if (end == commentEndPosition+EndCommentLength)
                doStyleText(commentEndPosition+EndCommentLength, pEnd, pText.right(pEnd-commentEndPosition-EndCommentLength));

            return;
        }
    }

    // Check whether the given text contains a // comment

    static const QString CommentString = "//";

    int commentPosition = pText.indexOf(CommentString);

    if (commentPosition != -1) {
        // There is a // comment to style, so first style everything that is
        // before the // comment

        doStyleText(pStart, pStart+commentPosition, pText.left(commentPosition));

        // Now, style everything that is after the // comment, if anything, by
        // looking for the end of the line on which the // comment is

        QString eolString = qobject_cast<QScintillaSupport::QScintillaWidget *>(editor())->eolString();
        int eolStringLength = eolString.length();
        int eolPosition = pText.indexOf(eolString, commentPosition+eolStringLength);

        if (eolPosition != -1) {
            int start = pStart+eolPosition+eolStringLength;

            doStyleText(start, pEnd, pText.right(pEnd-start));
        }

        // Style the // comment itself

        int start = pStart+commentPosition;

        startStyling(start, 0x1f);
        setStyling(((eolPosition == -1)?pEnd:pStart+eolPosition)-start, Comment);

        return;
    }

    // Check whether the given text contains a /* XXX */ comment

    commentStartPosition = pText.indexOf(StartCommentString);

    if (commentStartPosition != -1) {
        // There is a /* XXX */ comment to style, so first style everything that
        // is before the comment

        doStyleText(pStart, pStart+commentStartPosition, pText.left(commentStartPosition));

        // Now style everything from the comment onwards
        // Note: by styling everything from the comment onwards means that we
        //       will find that a /* XXX */ comment starts at the beginning of
        //       the 'new' given text...

        doStyleText(pStart+commentStartPosition, pEnd, pText.right(pEnd-pStart-commentStartPosition));

        return;
    }

    // Use a default style for the given text

    startStyling(pStart, 0x1f);
    setStyling(pEnd-pStart, Default);

    // Check whether the given text contains keywords from various categories

    doStyleTextKeyword(pStart, pText, mKeywords, Keyword);
    doStyleTextKeyword(pStart, pText, mCellmlKeywords, CellmlKeyword);
    doStyleTextKeyword(pStart, pText, mParameterKeywords, ParameterKeyword);
    doStyleTextKeyword(pStart, pText, mParameterValueKeywords, ParameterValueKeyword);
}

//==============================================================================

void PrettyCellmlViewLexer::doStyleTextKeyword(int pStart,
                                               const QString &pText,
                                               const QStringList pKeywords,
                                               const int &pKeywordStyle)
{
    // Check whether the given text contains some of the given keywords

    foreach (const QString &keyword, pKeywords) {
        QRegularExpressionMatchIterator regExMatchIter = QRegularExpression("\\b"+keyword+"\\b").globalMatch(pText);

        while (regExMatchIter.hasNext()) {
            QRegularExpressionMatch regExMatch = regExMatchIter.next();

            // We found a keyword, so style it as such

            startStyling(pStart+regExMatch.capturedStart(), 0x1f);
            setStyling(regExMatch.capturedLength(), pKeywordStyle);
        }
    }
}

//==============================================================================

}   // namespace PrettyCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
