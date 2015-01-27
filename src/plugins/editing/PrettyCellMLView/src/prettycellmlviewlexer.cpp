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
    mFullText(QString()),
    mEolString(QString())
{
    // Some initialisations

    mKeywordsRegEx = QRegularExpression(
                         "\\b("
                             "and|as|between|comp|def|endcomp|enddef|for|group|"
                             "import|incl|map|model|unit|using|var|vars"
                         ")\\b");

    mCellmlKeywordsRegEx = QRegularExpression(
                               "\\b("
                                    // Miscellaneous

                                   "base|encapsulation|containment|"

                                   // Standard units

                                   "ampere|becquerel|candela|celsius|coulomb|"
                                   "dimensionless|farad|gram|gray|henry|hertz|"
                                   "joule|katal|kelvin|kilogram|liter|litre|"
                                   "lumen|lux|meter|metre|mole|newton|ohm|"
                                   "pascal|radian|second|siemens|sievert|"
                                   "steradian|tesla|volt|watt|weber"
                               ")\\b");

    mParameterKeywordsRegEx = QRegularExpression(
                                  "\\b("
                                      // Unit keywords

                                      "pref|expo|mult|off|"

                                      // Variable keywords

                                      "init|pub|priv"
                                  ")\\b");

    mParameterValueKeywordsRegEx = QRegularExpression(
                                       "\\b("
                                           // Unit prefixes

                                           "yotta|zetta|exa|peta|tera|giga|"
                                           "mega|kilo|hecto|deka|deci|centi|"
                                           "milli|micro|nano|pico|femto|atto|"
                                           "zepto|yocto|"

                                           // Public/private interfaces

                                           "in|out|none"
                                       ")\\b");

    mNumberRegEx = QRegularExpression("\\b\\d*\\.?\\d+([eE][+-]?\\d+)?\\b");
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
    case Number:
        return QObject::tr("Number");
    case String:
        return QObject::tr("String");
    case ParameterGroup:
        return QObject::tr("Parameter group");
    case ParameterKeyword:
        return QObject::tr("Parameter keyword");
    case ParameterValueKeyword:
        return QObject::tr("Parameter value keyword");
    case ParameterNumber:
        return QObject::tr("Parameter number");
    case ParameterString:
        return QObject::tr("Parameter string");
    }

    return QString();
}

//==============================================================================

QColor PrettyCellmlViewLexer::color(int pStyle) const
{
    // Return the given style's colour

    switch (pStyle) {
    case Default:
    case ParameterGroup:
        return QColor(0x1f, 0x1f, 0x1f);
    case Comment:
        return QColor(0x7f, 0x7f, 0x7f);
    case Keyword:
    case ParameterKeyword:
        return QColor(0x00, 0x00, 0x7f);
    case CellmlKeyword:
    case ParameterValueKeyword:
        return QColor(0x7f, 0x00, 0x7f);
    case Number:
    case ParameterNumber:
        return QColor(0x00, 0x7f, 0x7f);
    case String:
    case ParameterString:
        return QColor(0x00, 0x7f, 0x00);
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
    case ParameterNumber:
    case ParameterString:
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

    if (text.trimmed().isEmpty())
        return;

    // Effectively style our text

    mFullText = editor()->text();
    mEolString = qobject_cast<QScintillaSupport::QScintillaWidget *>(editor())->eolString();

    doStyleText(pStart, pEnd, text, false);

    // Let QScintilla know that we are done with the styling of our text
    // Note: indeed, QScintilla uses the end position of the last bit of text
    //       that we styled (in our call to doStyleText()) to determine the
    //       position (pStart) of the next bit of text to style (see
    //       QsciLexerCustom::handleStyleNeeded()). Now, depending on how we do
    //       the styling, that end position may not be optimal at all. So, here,
    //       we set that value to pEnd, which is what end position would be if
    //       we were to style sequentially...

    startStyling(pEnd);
}

//==============================================================================

static const auto CommentString = QStringLiteral("//");
static const int CommentStringLength = CommentString.length();

//==============================================================================

static const auto StartCommentString = QStringLiteral("/*");
static const auto EndCommentString = QStringLiteral("*/");
static const int StartCommentLength = StartCommentString.length();
static const int EndCommentLength = EndCommentString.length();

//==============================================================================

static const auto StartParameterGroupString = QStringLiteral("{");
static const auto EndParameterGroupString = QStringLiteral("}");
static const int StartParameterGroupLength = StartParameterGroupString.length();
static const int EndParameterGroupLength = EndParameterGroupString.length();

//==============================================================================

static const auto StringString = QStringLiteral("\"");
static const int StringStringLength = StringString.length();

//==============================================================================

void PrettyCellmlViewLexer::doStyleText(int pStart, int pEnd, QString pText,
                                        bool pParameterGroup)
{
    // Make sure that we are given some text to style

    if (pText.trimmed().isEmpty())
        return;

    // Check whether a /* XXX */ comment or a parameter group started before or
    // at the beginning of the given text

    int multilineCommentStartPosition = mFullText.lastIndexOf(StartCommentString, pStart+StartCommentLength-1);
    int parameterGroupStartPosition = findParameterGroupString(pStart, false);

    if (multilineCommentStartPosition != -1) {
        doStyleTextPreviousMultilineComment(multilineCommentStartPosition, pStart, pEnd,
                                            pText);
    }

    if (parameterGroupStartPosition != -1) {
        doStyleTextPreviousParameterGroup(parameterGroupStartPosition, pStart,
                                          pEnd, pText, pParameterGroup);
    }

    // Check whether the given text contains a string, a // comment, a /* XXX */
    // comment or a parameter group

    int stringStartPosition = pText.indexOf(StringString);
    int singleLineCommentPosition = pText.indexOf(CommentString);
    multilineCommentStartPosition = pText.indexOf(StartCommentString);
    parameterGroupStartPosition = pText.indexOf(StartParameterGroupString);

    stringStartPosition = (stringStartPosition == -1)?INT_MAX:stringStartPosition;
    singleLineCommentPosition = (singleLineCommentPosition == -1)?INT_MAX:singleLineCommentPosition;
    multilineCommentStartPosition = (multilineCommentStartPosition == -1)?INT_MAX:multilineCommentStartPosition;
    parameterGroupStartPosition = (parameterGroupStartPosition == -1)?INT_MAX:parameterGroupStartPosition;

    if (   (stringStartPosition != INT_MAX)
        && (stringStartPosition < singleLineCommentPosition)
        && (stringStartPosition < multilineCommentStartPosition)
        && (stringStartPosition < parameterGroupStartPosition)) {
        doStyleTextString(stringStartPosition, pStart, pEnd, pText,
                          pParameterGroup);

        return;
    } else if (   (singleLineCommentPosition != INT_MAX)
               && (singleLineCommentPosition < stringStartPosition)
               && (singleLineCommentPosition < multilineCommentStartPosition)
               && (singleLineCommentPosition < parameterGroupStartPosition)) {
        doStyleTextSingleLineComment(singleLineCommentPosition, pStart, pEnd, pText,
                                     pParameterGroup);

        return;
    } else if (   (multilineCommentStartPosition != INT_MAX)
               && (multilineCommentStartPosition < stringStartPosition)
               && (multilineCommentStartPosition < singleLineCommentPosition)
               && (multilineCommentStartPosition < parameterGroupStartPosition)) {
        doStyleTextMultilineComment(multilineCommentStartPosition, pStart, pEnd, pText,
                                    pParameterGroup);

        return;
    } else if (   (parameterGroupStartPosition != INT_MAX)
               && (parameterGroupStartPosition < stringStartPosition)
               && (parameterGroupStartPosition < singleLineCommentPosition)
               && (parameterGroupStartPosition < multilineCommentStartPosition)) {
        doStyleTextParameterGroup(parameterGroupStartPosition, pStart, pEnd,
                                  pText, pParameterGroup);

        return;
    }

    // Use a default style for the given text

    startStyling(pStart);
    setStyling(pEnd-pStart, pParameterGroup?ParameterGroup:Default);

    // Check whether the given text contains keywords from various categories

    doStyleTextKeyword(pStart, pText, mKeywordsRegEx, pParameterGroup?ParameterGroup:Keyword);
    doStyleTextKeyword(pStart, pText, mCellmlKeywordsRegEx, pParameterGroup?ParameterGroup:CellmlKeyword);
    doStyleTextKeyword(pStart, pText, mParameterKeywordsRegEx, pParameterGroup?ParameterKeyword:Default);
    doStyleTextKeyword(pStart, pText, mParameterValueKeywordsRegEx, pParameterGroup?ParameterValueKeyword:Default);

    // Check whether the given text contains some numbers

    doStyleTextNumber(pStart, pText, pParameterGroup?ParameterNumber:Number);
}

//==============================================================================

void PrettyCellmlViewLexer::doStyleTextPreviousMultilineComment(const int &pPosition,
                                                                int &pStart,
                                                                int pEnd,
                                                                QString &pText)
{
    // A /* XXX */ comment started before or at the beginning of the given text,
    // so now look for where it ends

    int multilineCommentEndPosition = mFullText.indexOf(EndCommentString, pPosition+StartCommentLength);

    if (multilineCommentEndPosition == -1) {
        // The comment doesn't end as such, so consider that it 'ends' at the of
        // the full text

        multilineCommentEndPosition = mFullText.length();
    }

    if ((pPosition <= pStart) && (pStart <= multilineCommentEndPosition)) {
        // The beginning of the given text is a comment, so style it

        int realEnd = multilineCommentEndPosition+EndCommentLength;
        int end = qMin(pEnd, realEnd);

        startStyling(pStart);
        setStyling(end-pStart, Comment);

        // Get ready to style everything that is behind the comment
        // Note: if there is nothing behind the comment, then it means that end
        //       is equal to pEnd, so pText will be empty...

        pStart = end;
        pText = pText.right(pEnd-end);
    }
}

//==============================================================================

void PrettyCellmlViewLexer::doStyleTextPreviousParameterGroup(const int &pPosition,
                                                              int &pStart,
                                                              int &pEnd,
                                                              QString &pText,
                                                              bool &pParameterGroup)
{
    // A parameter group started before or at the beginning of the given text,
    // so now look for where it ends

    int parameterGroupEndPosition = findParameterGroupString(pPosition);

    if (parameterGroupEndPosition == -1)
        parameterGroupEndPosition = mFullText.length();

    if ((pPosition <= pStart) && (pStart <= parameterGroupEndPosition)) {
        // The beginning of the given text is a parameter group, so style
        // everything that is behind the parameter group, if anything

        int realEnd = parameterGroupEndPosition+EndParameterGroupLength;
        int end = qMin(pEnd, realEnd);
        bool hasEnd = end == realEnd;

        if (hasEnd)
            doStyleText(end, pEnd, pText.right(pEnd-end), pParameterGroup);

        // If possible, style the beginning and/or the end of the parameter
        // group

        bool hasBeginning = pPosition == pStart;

        if (hasBeginning) {
            startStyling(pStart);
            setStyling(StartParameterGroupLength, ParameterGroup);
        }

        if (hasEnd) {
            startStyling(end-EndParameterGroupLength);
            setStyling(EndParameterGroupLength, ParameterGroup);
        }

        // Now style the contents of the parameter group

        int newStart = pStart+(hasBeginning?StartParameterGroupLength:0);
        int newEnd = end-(hasEnd?EndParameterGroupLength:0);

        pText = pText.mid(newStart-pStart, newEnd-newStart);

        pStart = newStart;
        pEnd = newEnd;
        pParameterGroup = true;
    }
}

//==============================================================================

void PrettyCellmlViewLexer::doStyleTextString(const int &pPosition, int pStart,
                                              int pEnd, QString pText,
                                              bool pParameterGroup)
{
    // There is a string to style, so first style everything that is before it

    doStyleText(pStart, pStart+pPosition, pText.left(pPosition), pParameterGroup);

    // Now, check where the string ends and

    int stringEndPosition = pText.indexOf(StringString, pPosition+StringStringLength);

    if (stringEndPosition != -1) {
        // The string ends somewhere, so compare its line number against that of
        // where it started

        int stringStartLineNumber, stringStartColumnNumber;
        int stringEndLineNumber, stringEndColumnNumber;

        editor()->lineIndexFromPosition(pStart+pPosition,
                                        &stringStartLineNumber, &stringStartColumnNumber);
        editor()->lineIndexFromPosition(pStart+stringEndPosition,
                                        &stringEndLineNumber, &stringEndColumnNumber);

        if (stringStartLineNumber == stringEndLineNumber) {
            // The string starts and ends on the same line, so style everything
            // that is after the string

            int start = pStart+stringEndPosition+StringStringLength;

            doStyleText(start, pEnd, pText.right(pEnd-start), pParameterGroup);
        } else {
            // The string starts and ends on a different line, so consider that
            // we couldn't find the end of the string

            stringEndPosition = -1;
        }
    }

    if (stringEndPosition == -1) {
        // The string doesn't end or we found the beginning of another string on
        // a different line, so style everything that is after the end of the
        // line, if anything, on which the string started

        int eolPosition = pText.indexOf(mEolString, pPosition+StringStringLength);

        if (eolPosition != -1) {
            int start = pStart+eolPosition+mEolString.length();

            doStyleText(start, pEnd, pText.right(pEnd-start), pParameterGroup);
        }

        // Consider the end of the line as the end of the string

        stringEndPosition = eolPosition;
    }

    // Style the string itself

    int start = pStart+pPosition;

    startStyling(start);
    setStyling(((stringEndPosition == -1)?pEnd:pStart+stringEndPosition+StringStringLength)-start, pParameterGroup?ParameterString:String);
}

//==============================================================================

void PrettyCellmlViewLexer::doStyleTextSingleLineComment(const int &pPosition,
                                                         int pStart, int pEnd,
                                                         QString pText,
                                                         bool pParameterGroup)
{
    // There is a // comment to style, so first style everything that is before
    // it

    doStyleText(pStart, pStart+pPosition, pText.left(pPosition), pParameterGroup);

    // Now, style everything that is after the // comment, if anything, by
    // looking for the end of the line on which the // comment is

    int eolPosition = pText.indexOf(mEolString, pPosition+CommentStringLength);

    if (eolPosition != -1) {
        int start = pStart+eolPosition+mEolString.length();

        doStyleText(start, pEnd, pText.right(pEnd-start), pParameterGroup);
    }

    // Style the // comment itself

    int start = pStart+pPosition;

    startStyling(start);
    setStyling(((eolPosition == -1)?pEnd:pStart+eolPosition)-start, Comment);
}

//==============================================================================

void PrettyCellmlViewLexer::doStyleTextMultilineComment(const int &pPosition,
                                                        int pStart, int pEnd,
                                                        QString pText,
                                                        bool pParameterGroup)
{
    // There is a /* XXX */ comment to style, so first style everything that is
    // before it

    doStyleText(pStart, pStart+pPosition, pText.left(pPosition), pParameterGroup);

    // Now style everything from the comment onwards
    // Note: to style everything from the comment onwards means that we will
    //       find that a /* XXX */ comment starts at the beginning of the 'new'
    //       given text...

    doStyleText(pStart+pPosition, pEnd, pText.right(pEnd-pStart-pPosition), pParameterGroup);
}

//==============================================================================

void PrettyCellmlViewLexer::doStyleTextParameterGroup(const int &pPosition,
                                                      int pStart, int pEnd,
                                                      QString pText,
                                                      bool pParameterGroup)
{
    // There is a parameter group, so first style everything that is before it

    doStyleText(pStart, pStart+pPosition, pText.left(pPosition), pParameterGroup);

    // Now style everything from the parameter group onwards
    // Note: to style everything from the parameter group onwards means that we
    //       will find that a parameter group starts at the beginning of the
    //       'new' given text...

    doStyleText(pStart+pPosition, pEnd, pText.right(pEnd-pStart-pPosition), pParameterGroup);
}

//==============================================================================

void PrettyCellmlViewLexer::doStyleTextKeyword(int pStart,
                                               const QString &pText,
                                               const QRegularExpression &pKeywordsRegEx,
                                               const int &pKeywordStyle)
{
    // Style the given text with the given keyword style in the cases where a
    // match for the given regular expression is found

    QRegularExpressionMatchIterator regExMatchIter = pKeywordsRegEx.globalMatch(pText);
    QRegularExpressionMatch regExMatch;

    while (regExMatchIter.hasNext()) {
        regExMatch = regExMatchIter.next();

        // We found a keyword, so style it as such

        startStyling(pStart+regExMatch.capturedStart());
        setStyling(regExMatch.capturedLength(), pKeywordStyle);
    }
}

//==============================================================================

void PrettyCellmlViewLexer::doStyleTextNumber(int pStart, const QString &pText,
                                              const int &pNumberStyle)
{
    // Check whether the given text contains some numbers

    QRegularExpressionMatchIterator regExMatchIter = mNumberRegEx.globalMatch(pText);
    QRegularExpressionMatch regExMatch;

    while (regExMatchIter.hasNext()) {
        regExMatch = regExMatchIter.next();

        // We found a number, so style it as such

        startStyling(pStart+regExMatch.capturedStart());
        setStyling(regExMatch.capturedLength(), pNumberStyle);
    }
}

//==============================================================================

bool PrettyCellmlViewLexer::validParameterGroupString(const int &pFrom,
                                                      const int &pTo) const
{
    // Return whether the parameter group string, which range is given, is
    // within a string or a comment

    // Check whether we are within a string

    int stringPosition = mFullText.lastIndexOf(StringString, pFrom);

    if (stringPosition != -1) {
        int lineNumber, columnNumber;
        int stringStartLineNumber, stringStartColumnNumber;

        editor()->lineIndexFromPosition(pFrom, &lineNumber, &columnNumber);
        editor()->lineIndexFromPosition(stringPosition,
                                        &stringStartLineNumber, &stringStartColumnNumber);

        if (lineNumber == stringStartLineNumber) {
            // The beginning or the end of a string was found on the same line
            // as the given string, so check whether it's actually the beginning
            // or the end of a string

            int from = editor()->positionFromLineIndex(lineNumber, 0);
            bool beginningOfString = false;

            while (   ((stringPosition = mFullText.indexOf(StringString, from)) != -1)
                   &&  (stringPosition < pFrom)) {
                from = stringPosition+StringStringLength;
                beginningOfString = !beginningOfString;
            }

            if (beginningOfString)
                return false;
        }
    }

    // Check whether we are within a /* XXX */ comment

    int multilineCommentStartPosition = mFullText.lastIndexOf(StartCommentString, pFrom);

    if (multilineCommentStartPosition != -1) {
        int commentEndPosition = mFullText.indexOf(EndCommentString, multilineCommentStartPosition+StartCommentLength);

        if (commentEndPosition == -1)
            commentEndPosition = mFullText.length();

        if ((multilineCommentStartPosition < pFrom) && (pTo < commentEndPosition))
            return false;
    }

    // Check whether we are within a // comment

    int singleLineCommentPosition = mFullText.lastIndexOf(CommentString, pFrom);

    if (singleLineCommentPosition != -1) {
        int eolPosition = mFullText.indexOf(mEolString, singleLineCommentPosition+CommentStringLength);

        if (eolPosition == -1)
            eolPosition = mFullText.length();

        if ((singleLineCommentPosition < pFrom) && (pTo < eolPosition))
            return false;
    }

    return true;
}

//==============================================================================

int PrettyCellmlViewLexer::findParameterGroupString(int pFrom,
                                                    const bool &pForward)
{
    // Find forward/backward the given string starting from the given position

    QString parameterGroupString = pForward?EndParameterGroupString:StartParameterGroupString;
    int parameterGroupStringLength = pForward?EndParameterGroupLength:StartParameterGroupLength;
    int res = pForward?pFrom:pFrom+parameterGroupStringLength;

    do {
        pFrom = pForward?res+parameterGroupStringLength:res-1;

        res = pForward?mFullText.indexOf(parameterGroupString, pFrom):mFullText.lastIndexOf(parameterGroupString, pFrom);
    } while ((res != -1) && !validParameterGroupString(res, res+parameterGroupStringLength-1));

    return res;
}

//==============================================================================

}   // namespace PrettyCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
