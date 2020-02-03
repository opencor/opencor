/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Lexer for the CellML Text format
//==============================================================================

#include "cellmltextviewlexer.h"
#include "qscintillawidget.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

CellmlTextViewLexer::CellmlTextViewLexer(QObject *pParent) :
    QsciLexerCustom(pParent)
{
}

//==============================================================================

const char * CellmlTextViewLexer::language() const
{
    // Return the language for our lexer

    return "CellML Text";
}

//==============================================================================

QString CellmlTextViewLexer::description(int pStyle) const
{
    // Return the given style's description

    switch (Style(pStyle)) {
    case Style::Default:
        return QObject::tr("Default");
    case Style::SingleLineComment:
        return QObject::tr("Single line comment");
    case Style::MultilineComment:
        return QObject::tr("Multiline comment");
    case Style::Keyword:
        return QObject::tr("Keyword");
    case Style::CellmlKeyword:
        return QObject::tr("CellML keyword");
    case Style::Number:
        return QObject::tr("Number");
    case Style::String:
        return QObject::tr("String");
    case Style::ParameterBlock:
        return QObject::tr("Parameter block");
    case Style::ParameterKeyword:
        return QObject::tr("Parameter keyword");
    case Style::ParameterCellmlKeyword:
        return QObject::tr("Parameter CellML keyword");
    case Style::ParameterNumber:
        return QObject::tr("Parameter number");
    case Style::ParameterString:
        return QObject::tr("Parameter string");
    }

    return {};
}

//==============================================================================

QColor CellmlTextViewLexer::color(int pStyle) const
{
    // Return the given style's colour

    switch (Style(pStyle)) {
    case Style::Default:
    case Style::ParameterBlock:
        return { 0x1f, 0x1f, 0x1f };
    case Style::SingleLineComment:
    case Style::MultilineComment:
        return { 0x7f, 0x7f, 0x7f };
    case Style::Keyword:
    case Style::ParameterKeyword:
        return { 0x00, 0x00, 0x7f };
    case Style::CellmlKeyword:
    case Style::ParameterCellmlKeyword:
        return { 0x7f, 0x00, 0x7f };
    case Style::Number:
    case Style::ParameterNumber:
        return { 0x00, 0x7f, 0x7f };
    case Style::String:
    case Style::ParameterString:
        return { 0x00, 0x7f, 0x00 };
    }

    return QsciLexerCustom::color(pStyle);
}

//==============================================================================

QFont CellmlTextViewLexer::font(int pStyle) const
{
    // Return the given style's colour

    QFont res = QsciLexer::font(pStyle);

    res.setItalic(   (Style(pStyle) == Style::ParameterBlock)
                  || (Style(pStyle) == Style::ParameterKeyword)
                  || (Style(pStyle) == Style::ParameterCellmlKeyword)
                  || (Style(pStyle) == Style::ParameterNumber)
                  || (Style(pStyle) == Style::ParameterString));

    return res;
}

//==============================================================================

static const int StyleChunk = 32768;

//==============================================================================

void CellmlTextViewLexer::styleText(int pStart, int pEnd)
{
#ifdef QT_DEBUG
    // Make sure that we have an editor

    if (editor() == nullptr) {
        qFatal("FATAL ERROR | %s:%d: styling can only be performed with an editor.", __FILE__, __LINE__);
    }
#endif

    // Keep track of some information

    mFullText = editor()->text().toUtf8();
    mEolString = qobject_cast<QScintillaWidget::QScintillaWidget *>(editor())->eolString();

    // Style the text in small chunks (to reduce memory usage, which can quickly
    // become ridiculous the first time we are styling a big CellML file)

    int start = pStart;
    int end;

    forever {
        // Use a default style for our chunk of text
        // Note: this is so that validString() can work properly...

        end = qMin(start+StyleChunk, pEnd);

        applyStyle(start, end, Style::Default);

        // Retrieve the chunk of text to style and style it

        auto data = new char[end-start+1] {};

        editor()->SendScintilla(QsciScintilla::SCI_GETTEXTRANGE,
                                start, end, data);

        styleText(start, end, data, false);

        delete[] data;

#ifdef QT_DEBUG
        // Make sure that the end position of the last bit of chunk of text that
        // we styled is end
        // Note: we need to ensure that it is the case, so that validString()
        //       can take advantage of the style of a given character in the
        //       editor...

        if (editor()->SendScintilla(QsciScintillaBase::SCI_GETENDSTYLED) != end) {
            qFatal("FATAL ERROR | %s:%d: the styling of the text must be incremental.", __FILE__, __LINE__);
        }
#endif

        // Was it our last chunk of text?

        if (end == pEnd) {
            break;
        }

        // Get ready for the next chunk of text

        start = end;
    }

    // Let people know that we are done with our styling

    emit done();
}

//==============================================================================

static const char *SingleLineCommentString = "//";
static const int SingleLineCommentLength   = 2;

//==============================================================================

static const char *StartMultilineCommentString = "/*";
static const char *EndMultilineCommentString   = "*/";
static const int EndMultilineCommentLength     = 2;

//==============================================================================

static const char *StartParameterBlockString = "{";
static const char *EndParameterBlockString   = "}";
static const int StartParameterBlockLength   = 1;
static const int EndParameterBlockLength     = 1;

//==============================================================================

static const char *StringString = R"(")";
static const int StringLength   = 1;

//==============================================================================

void CellmlTextViewLexer::applyStyle(int pStart, int pEnd, Style pStyle)
{
    // Apply the given style to the given chunk of text

    startStyling(pStart);
    setStyling(pEnd-pStart, int(pStyle));
}

//==============================================================================

void CellmlTextViewLexer::styleText(int pStart, int pEnd,
                                    const QByteArray &pText,
                                    bool pParameterBlock)
{
    // Make sure that we are given some text to style

    if (pStart == pEnd) {
        return;
    }

    // Check whether a /* XXX */ comment or a parameter block started before or
    // at the beginning of the given text

    int multilineCommentStartPosition = findString(StartMultilineCommentString, pStart, Style::MultilineComment, false);
    int parameterBlockStartPosition = findString(StartParameterBlockString, pStart, Style::ParameterBlock, false);

    multilineCommentStartPosition = (multilineCommentStartPosition == -1)?
                                        INT_MAX:
                                        multilineCommentStartPosition;
    parameterBlockStartPosition = (parameterBlockStartPosition == -1)?
                                      INT_MAX:
                                      parameterBlockStartPosition;

    int multilineCommentEndPosition = (multilineCommentStartPosition == INT_MAX)?
                                          INT_MAX:
                                          findString(EndMultilineCommentString, multilineCommentStartPosition, Style::MultilineComment);

    multilineCommentEndPosition = (multilineCommentEndPosition == -1)?
                                      INT_MAX:
                                      multilineCommentEndPosition;

    if (   (multilineCommentStartPosition != INT_MAX)
        && (   (parameterBlockStartPosition == INT_MAX)
            || (   (multilineCommentStartPosition > parameterBlockStartPosition)
                && (multilineCommentEndPosition > pStart)))) {
        // There is a previous /* XXX */ comment to style

        styleTextPreviousMultilineComment(multilineCommentStartPosition,
                                          pStart, pEnd,
                                          pText, pParameterBlock);
    } else {
        int parameterBlockEndPosition = (parameterBlockStartPosition == INT_MAX)?
                                            INT_MAX:
                                            findString(EndParameterBlockString, parameterBlockStartPosition, Style::ParameterBlock);

        parameterBlockEndPosition = (parameterBlockEndPosition == -1)?
                                        INT_MAX:
                                        parameterBlockEndPosition;

        if (   (parameterBlockStartPosition != INT_MAX)
            && (   (multilineCommentStartPosition == INT_MAX)
                || (   (parameterBlockStartPosition > multilineCommentStartPosition)
                    && (parameterBlockEndPosition > pStart)))) {
            // There is a previous parameter block to style

            styleTextPreviousParameterBlock(parameterBlockStartPosition,
                                            pStart, pEnd,
                                            pText, pParameterBlock);
        } else {
            // Style the current text

            styleTextCurrent(pStart, pEnd, pText,
                             (parameterBlockStartPosition < pStart) && (parameterBlockEndPosition > pStart));
        }
    }
}

//==============================================================================

void CellmlTextViewLexer::styleTextCurrent(int pStart, int pEnd,
                                           const QByteArray &pText,
                                           bool pParameterBlock)
{
    // Make sure that we are given some text to style

    if (pStart == pEnd) {
        return;
    }

    // Check whether the given text contains a string, a // comment, a /* XXX */
    // comment or a parameter block

    int stringPosition = pText.indexOf(StringString);
    int singleLineCommentPosition = pText.indexOf(SingleLineCommentString);
    int multilineCommentStartPosition = pText.indexOf(StartMultilineCommentString);

    stringPosition = (stringPosition == -1)?
                         INT_MAX:
                         stringPosition;
    singleLineCommentPosition = (singleLineCommentPosition == -1)?
                                    INT_MAX:
                                    singleLineCommentPosition;
    multilineCommentStartPosition = (multilineCommentStartPosition == -1)?
                                        INT_MAX:
                                        multilineCommentStartPosition;

    if (   (stringPosition != INT_MAX)
        && (stringPosition < singleLineCommentPosition)
        && (stringPosition < multilineCommentStartPosition)) {
        // There is a string to style

        styleTextString(stringPosition, pStart, pEnd, pText, pParameterBlock);
    } else if (   (singleLineCommentPosition != INT_MAX)
               && (singleLineCommentPosition < stringPosition)
               && (singleLineCommentPosition < multilineCommentStartPosition)) {
        // There is a // comment to style, so first style everything that is
        // before it

        styleTextCurrent(pStart, pStart+singleLineCommentPosition,
                         pText.left(singleLineCommentPosition),
                         pParameterBlock);

        // Style the // comment itself, after having looked for the end of the
        // line, if any

        int eolPosition = pText.indexOf(mEolString, singleLineCommentPosition+SingleLineCommentLength);
        int end = (eolPosition == -1)?pEnd:pStart+eolPosition+mEolString.length();

        applyStyle(pStart+singleLineCommentPosition, end, Style::SingleLineComment);

        // Now, style everything that is after the // comment, if anything

        if (eolPosition != -1) {
            styleText(end, pEnd, pText.right(pEnd-end), pParameterBlock);
        }
    } else if (   (multilineCommentStartPosition != INT_MAX)
               && (multilineCommentStartPosition < stringPosition)
               && (multilineCommentStartPosition < singleLineCommentPosition)) {
        // There is a /* XXX */ comment to style, so first style everything that
        // is before it

        styleTextCurrent(pStart, pStart+multilineCommentStartPosition,
                         pText.left(multilineCommentStartPosition),
                         pParameterBlock);

        // Now style everything from the comment onwards
        // Note #1: to style everything from the comment onwards means that we
        //          will find that a /* XXX */ comment starts at the beginning
        //          of the 'new' given text...
        // Note #2: for the parameter block value, we check whether a parameter
        //          block was started before the /* XXX */ comment line since
        //          we need to finish styling it (which is effectively done in
        //          styleTextPreviousMultilineComment())...

        int multilineCommentParameterBlockStartPosition = findString(StartParameterBlockString, pStart+multilineCommentStartPosition, Style::ParameterBlock, false);
        int multilineCommentParameterBlockEndPosition = findString(EndParameterBlockString, multilineCommentParameterBlockStartPosition, Style::ParameterBlock);

        multilineCommentParameterBlockStartPosition = (multilineCommentParameterBlockStartPosition == -1)?
                                                          INT_MAX:
                                                          multilineCommentParameterBlockStartPosition;
        multilineCommentParameterBlockEndPosition = (multilineCommentParameterBlockEndPosition == -1)?
                                                        INT_MAX:
                                                        multilineCommentParameterBlockEndPosition;

        int absoluteMultilineCommentStartPosition = pStart+multilineCommentStartPosition;

        styleText(pStart+multilineCommentStartPosition, pEnd,
                  pText.right(pEnd-pStart-multilineCommentStartPosition),
                     (multilineCommentParameterBlockStartPosition < absoluteMultilineCommentStartPosition)
                  && (absoluteMultilineCommentStartPosition < multilineCommentParameterBlockEndPosition));
    } else {
        int parameterBlockStartPosition = pText.indexOf(StartParameterBlockString);

        parameterBlockStartPosition = (parameterBlockStartPosition == -1)?
                                          INT_MAX:
                                          parameterBlockStartPosition;

        if (   (parameterBlockStartPosition != INT_MAX)
            && (parameterBlockStartPosition < stringPosition)
            && (parameterBlockStartPosition < singleLineCommentPosition)
            && (parameterBlockStartPosition < multilineCommentStartPosition)) {
            // There is a parameter block, so first style everything that is
            // before it

            styleTextCurrent(pStart, pStart+parameterBlockStartPosition,
                             pText.left(parameterBlockStartPosition),
                             pParameterBlock);

            // Now style everything from the parameter block onwards
            // Note: to style everything from the parameter block onwards means
            //       that we will find that a parameter block starts at the
            //       beginning of the 'new' given text...

            styleText(pStart+parameterBlockStartPosition, pEnd,
                      pText.right(pEnd-pStart-parameterBlockStartPosition),
                      pParameterBlock);
        } else {
            // Style the given text as a parameter block, if needed

            if (pParameterBlock) {
                applyStyle(pStart, pEnd, Style::ParameterBlock);
            }

            // Check whether the given text contains some keywords from various
            // categories

            static const QRegularExpression KeywordsRegEx = QRegularExpression(
                "\\b("
                    // CellML Text keywords

                    "and|as|between|case|comp|def|endcomp|enddef|endsel|for|"
                    "group|import|incl|map|model|otherwise|sel|unit|using|var|"
                    "vars|"

                    // MathML arithmetic operators

                    "abs|ceil|exp|fact|floor|ln|log|pow|root|sqr|sqrt|"

                    // MathML logical operators

                    "and|or|xor|not|"

                    // MathML calculus elements

                    "ode|"

                    // MathML min/max operators

                    "min|max|"

                    // MathML gcd/lcm operators

                    "gcd|lcm|"

                    // MathML trigonometric operators

                    "sin|cos|tan|sec|csc|cot|sinh|cosh|tanh|sech|csch|coth|"
                    "asin|acos|atan|asec|acsc|acot|asinh|acosh|atanh|asech|"
                    "acsch|acoth|"

                    // MathML constants

                    "true|false|nan|pi|inf|e|"

                    // Extra operators

                    "rem"
                ")\\b");

            static const QRegularExpression CellmlKeywordsRegEx = QRegularExpression(
                "\\b("
                     // Miscellaneous

                    "base|encapsulation|containment"
                ")\\b");

            static const QRegularExpression ParameterKeywordsRegEx = QRegularExpression(
                "\\b("
                    // Unit keywords

                    "pref|expo|mult|off|"

                    // Variable keywords

                    "init|pub|priv"
                ")\\b");

            static const QRegularExpression ParameterCellmlKeywordsRegEx = QRegularExpression(
                "\\b("
                    // Unit prefixes

                    "yotta|zetta|exa|peta|tera|giga|mega|kilo|hecto|deka|deci|"
                    "centi|milli|micro|nano|pico|femto|atto|zepto|yocto|"

                    // Public/private interfaces

                    "in|out|none"
                ")\\b");

            static const QRegularExpression SiUnitKeywordsRegEx = QRegularExpression(
                "\\b("
                    // Standard units

                    "ampere|becquerel|candela|celsius|coulomb|dimensionless|"
                    "farad|gram|gray|henry|hertz|joule|katal|kelvin|kilogram|"
                    "liter|litre|lumen|lux|meter|metre|mole|newton|ohm|pascal|"
                    "radian|second|siemens|sievert|steradian|tesla|volt|watt|"
                    "weber"
                ")\\b");

            if (pParameterBlock) {
                styleTextRegEx(pStart, pText, ParameterKeywordsRegEx, Style::ParameterKeyword);
                styleTextRegEx(pStart, pText, ParameterCellmlKeywordsRegEx, Style::ParameterCellmlKeyword);
            } else {
                styleTextRegEx(pStart, pText, KeywordsRegEx, Style::Keyword);
                styleTextRegEx(pStart, pText, CellmlKeywordsRegEx, Style::CellmlKeyword);
            }

            styleTextRegEx(pStart, pText, SiUnitKeywordsRegEx,
                           pParameterBlock?
                               Style::ParameterCellmlKeyword:
                               Style::CellmlKeyword);

            // Check whether the given text contains some numbers

            styleTextNumber(pStart, pText, pParameterBlock?
                                               Style::ParameterNumber:
                                               Style::Number);

            // Let QScintilla know that we are done with the styling of the
            // given text
            // Note: indeed, QScintilla uses the end position of the last bit of
            //       text that has been styled to determine the starting
            //       position of the next bit of text that needs to be styled
            //       (see QsciLexerCustom::handleStyleNeeded()). Now, depending
            //       on whether keywords and/or numbers have been styled, the
            //       end position may or not be pEnd. So, here, we make sure
            //       that it is, so that QScintilla can work as it should...

            startStyling(pEnd);
        }
    }
}

//==============================================================================

void CellmlTextViewLexer::styleTextPreviousMultilineComment(int pPosition,
                                                            int pStart,
                                                            int pEnd,
                                                            const QByteArray &pText,
                                                            bool pParameterBlock)
{
    // A /* XXX */ comment started before or at the beginning of the given text,
    // so now look for where it ends

    int multilineCommentEndPosition = findString(EndMultilineCommentString, pPosition, Style::MultilineComment);

    if (multilineCommentEndPosition == -1) {
        multilineCommentEndPosition = mFullText.length();
    }

    // Check whether the beginning of the given text is within the /* XXX */
    // comment

    if ((pPosition <= pStart) && (pStart <= multilineCommentEndPosition)) {
        // The beginning of the given text is within a /* XXX */ comment, so
        // style it

        int realEnd = multilineCommentEndPosition+EndMultilineCommentLength;
        int end = qMin(pEnd, realEnd);

        applyStyle(pStart, end, Style::MultilineComment);

        // Now, style everything that is behind the /* XXX */ comment, if
        // anything

        if (end != pEnd) {
            if (pParameterBlock) {
                // Our /* XXX */ comment is within a parameter block, so finish
                // styling our parameter block

                styleTextPreviousParameterBlock(end, end, pEnd, pText.right(pEnd-end), false);
            } else {
                styleText(end, pEnd, pText.right(pEnd-end), pParameterBlock);
            }
        }
    } else {
        // The beginning of the given text is not within a /* XXX */ comment, so
        // style it

        styleTextCurrent(pStart, pEnd, pText, pParameterBlock);
    }
}

//==============================================================================

void CellmlTextViewLexer::styleTextPreviousParameterBlock(int pPosition,
                                                          int pStart, int pEnd,
                                                          const QByteArray &pText,
                                                          bool pParameterBlock)
{
    // A parameter block started before or at the beginning of the given text,
    // so now look for where it ends

    int parameterBlockEndPosition = findString(EndParameterBlockString, pPosition, Style::ParameterBlock);

    if (parameterBlockEndPosition == -1) {
        parameterBlockEndPosition = mFullText.length();
    }

    // Check whether the beginning of the given text is within a parameter block

    if ((pPosition <= pStart) && (pStart <= parameterBlockEndPosition)) {
        // The beginning of the given text is within a parameter block, so style
        // it as such

        int realEnd = parameterBlockEndPosition+EndParameterBlockLength;
        int end = qMin(pEnd, realEnd);
        bool hasStart =    (pPosition == pStart)
                        && (mFullText.mid(pPosition, StartParameterBlockLength) == StartParameterBlockString);
        bool hasEnd = end == realEnd;

        // If needed, style the start of the parameter block

        if (hasStart) {
            applyStyle(pStart, pStart+StartParameterBlockLength, Style::ParameterBlock);
        }

        // Now style the contents of the parameter block itself

        int newStart = pStart+(hasStart?StartParameterBlockLength:0);
        int newEnd = end-(hasEnd?EndParameterBlockLength:0);

        styleTextCurrent(newStart, newEnd, pText.mid(newStart-pStart, newEnd-newStart), true);

        // If needed, style the end of the parameter block, as well as what is
        // behind it

        if (hasEnd) {
            applyStyle(end-EndParameterBlockLength, end, Style::ParameterBlock);

            styleText(end, pEnd, pText.right(pEnd-end), pParameterBlock);
        }
    } else {
        // The beginning of the given text is not within a parameter block, so
        // style it

        styleTextCurrent(pStart, pEnd, pText, pParameterBlock);
    }
}

//==============================================================================

void CellmlTextViewLexer::styleTextString(int pPosition, int pStart, int pEnd,
                                          const QByteArray &pText,
                                          bool pParameterBlock)
{
    // There is a string to style, so first style everything that is before it

    int position = pStart+pPosition;

    styleTextCurrent(pStart, position, pText.left(pPosition), pParameterBlock);

    // Now, check where the string ends, if anywhere

    int nextStart = -1;
    int stringEndPosition = pText.indexOf(StringString, pPosition+StringLength);

    if (stringEndPosition != -1) {
        // The string ends somewhere, so compare its line number against that of
        // where it started

        int stringStartLineNumber, stringStartColumnNumber;
        int stringEndLineNumber, stringEndColumnNumber;

        editor()->lineIndexFromPosition(position,
                                        &stringStartLineNumber, &stringStartColumnNumber);
        editor()->lineIndexFromPosition(pStart+stringEndPosition,
                                        &stringEndLineNumber, &stringEndColumnNumber);

        if (stringStartLineNumber == stringEndLineNumber) {
            // The string starts and ends on the same line, so get ready to
            // style everything that is after the string

            nextStart = pStart+stringEndPosition+StringLength;
        } else {
            // The string starts and ends on a different line, so consider that
            // we couldn't find the end of the string

            stringEndPosition = -1;
        }
    }

    if (stringEndPosition == -1) {
        // The string doesn't end or we found the beginning of another string on
        // a different line, so get ready to style everything that is after the
        // end of the line, if anything, on which the string started

        int eolPosition = pText.indexOf(mEolString, pPosition+StringLength);

        if (eolPosition != -1) {
            nextStart = pStart+eolPosition+mEolString.length();
        }
    }

    // Style the string itself

    applyStyle(position,
               (nextStart == -1)?
                   pEnd:
                   nextStart,
               pParameterBlock?
                   Style::ParameterString:
                   Style::String);

    // Style whatever is after the string

    if (nextStart != -1) {
        styleText(nextStart, pEnd, pText.right(pEnd-nextStart), pParameterBlock);
    }
}

//==============================================================================

void CellmlTextViewLexer::styleTextRegEx(int pStart, const QByteArray &pText,
                                         const QRegularExpression &pRegEx,
                                         Style pStyle)
{
    // Style the given text using the given regular expression

    QRegularExpressionMatchIterator regExMatchIter = pRegEx.globalMatch(pText);

    while (regExMatchIter.hasNext()) {
        // We have a match, so style it

        QRegularExpressionMatch regExMatch = regExMatchIter.next();
        int capturedStart = regExMatch.capturedStart();

        applyStyle(pStart+capturedStart, pStart+capturedStart+regExMatch.capturedLength(), pStyle);
    }
}

//==============================================================================

void CellmlTextViewLexer::styleTextNumber(int pStart, const QByteArray &pText,
                                          Style pStyle)
{
    // Style the given text using the number regular expression

    static const QRegularExpression NumberRegEx = QRegularExpression(R"((\d+(\.\d*)?|\.\d+)([eE][+-]?\d*)?)");
    // Note: this regular expression is not aimed at catching valid numbers, but
    //       at catching something that could become a valid number (e.g. we
    //       want to be able to catch "123e")...

    QRegularExpressionMatchIterator regExMatchIter = NumberRegEx.globalMatch(pText);

    while (regExMatchIter.hasNext()) {
        // We have a match, so style it, but only if:
        //  - The character in front of the match is not in [0-9a-zA-Z_] and is
        //    part of the ASCII table
        //  - The character following the match is not in [a-zA-Z_.] and is part
        //    of the ASCII table

        QRegularExpressionMatch regExMatch = regExMatchIter.next();
        int capturedStart = regExMatch.capturedStart();
        int capturedLength = regExMatch.capturedLength();
        int prevCharPos = pStart+capturedStart-1;
        int nextCharPos = prevCharPos+capturedLength+1;

        char prevChar = (prevCharPos >= 0)?
                            mFullText[prevCharPos]:
                            0;
        char nextChar = (nextCharPos < mFullText.length())?
                            mFullText[nextCharPos]:
                            0;

        if (   (    (prevChar  <  48)
                || ((prevChar  >  57) && (prevChar <  65))
                || ((prevChar  >  90) &&  (prevChar  <  95))
                ||  (prevChar ==  96)
                ||  (prevChar  > 122))
            && (    (nextChar  <  46)
                || ((nextChar  >  46) && (nextChar <  65))
                || ((nextChar  >  90) &&  (nextChar  <  95))
                ||  (nextChar ==  96)
                ||  (nextChar  > 122))) {
            applyStyle(pStart+capturedStart, pStart+capturedStart+capturedLength, pStyle);
        }
    }
}

//==============================================================================

bool CellmlTextViewLexer::validString(int pFrom, int pTo, Style pStyle) const
{
    // Check whether the string, which range is given, is valid, i.e. is either
    // of the default or given style

    for (int i = pFrom; i < pTo; ++i) {
        Style style = Style(editor()->SendScintilla(QsciScintilla::SCI_GETSTYLEAT, i));

        if ((style != Style::Default) && (style != pStyle)) {
            return false;
        }
    }

    return true;
}

//==============================================================================

int CellmlTextViewLexer::findString(const QByteArray &pString, int pFrom,
                                    Style pStyle, bool pForward)
{
    // Find forward/backward the given string starting from the given position

    int stringLength = pString.length();
    int res = pForward?pFrom:pFrom+1;

    do {
        pFrom = pForward?res+stringLength:res-1;

        res = pForward?mFullText.indexOf(pString, pFrom):mFullText.lastIndexOf(pString, pFrom);
    } while ((res != -1) && !validString(res, res+stringLength, pStyle));

    return res;
}

//==============================================================================

} // namespace CellMLTextView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
