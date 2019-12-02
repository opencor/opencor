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

void CellmlTextViewLexer::styleText(int pBytesStart, int pBytesEnd)
{
    // Make sure that we have an editor

    if (editor() == nullptr) {
        return;
    }

    // Retrieve the text to style

    auto data = new char[pBytesEnd-pBytesStart+1] {};

    editor()->SendScintilla(QsciScintilla::SCI_GETTEXTRANGE,
                            pBytesStart, pBytesEnd, data);

    QString text = QString(data);

    delete[] data;

    // Use a default style for our text
    // Note: this is so that validString() can work properly...

    startStyling(pBytesStart);
    setStyling(pBytesEnd-pBytesStart, int(Style::Default));

    // Effectively style our text

    mFullText = editor()->text();
    mFullTextUtf8 = mFullText.toUtf8();

    mEolString = qobject_cast<QScintillaWidget::QScintillaWidget *>(editor())->eolString();

    styleText(pBytesStart, pBytesEnd, text, false);

#ifdef QT_DEBUG
    // Make sure that the end position of the last bit of text that we styled is
    // pBytesEnd
    // Note: we need to ensure that it is the case, so that validString() can
    //       take advantage of the style of a given character in the editor...

    if (editor()->SendScintilla(QsciScintillaBase::SCI_GETENDSTYLED) != pBytesEnd) {
        qFatal("FATAL ERROR | %s:%d: the styling of the text must be incremental.", __FILE__, __LINE__);
    }
#endif

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

void CellmlTextViewLexer::styleText(int pBytesStart, int pBytesEnd,
                                    const QString &pText, bool pParameterBlock)
{
    // Make sure that we are given some text to style

    if (pBytesStart == pBytesEnd) {
        return;
    }

    // Check whether a /* XXX */ comment or a parameter block started before or
    // at the beginning of the given text

    int start = fullTextPosition(pBytesStart);
    int multilineCommentStartPosition = findString(StartMultilineCommentString, start, int(Style::MultilineComment), false);
    int parameterBlockStartPosition = findString(StartParameterBlockString, start, int(Style::ParameterBlock), false);

    multilineCommentStartPosition = (multilineCommentStartPosition == -1)?INT_MAX:multilineCommentStartPosition;
    parameterBlockStartPosition = (parameterBlockStartPosition == -1)?INT_MAX:parameterBlockStartPosition;

    int multilineCommentEndPosition = (multilineCommentStartPosition == INT_MAX)?INT_MAX:findString(EndMultilineCommentString, multilineCommentStartPosition, int(Style::MultilineComment));
    int parameterBlockEndPosition = (parameterBlockStartPosition == INT_MAX)?INT_MAX:findString(EndParameterBlockString, parameterBlockStartPosition, int(Style::ParameterBlock));

    multilineCommentEndPosition = (multilineCommentEndPosition == -1)?INT_MAX:multilineCommentEndPosition;
    parameterBlockEndPosition = (parameterBlockEndPosition == -1)?INT_MAX:parameterBlockEndPosition;

    if (   (multilineCommentStartPosition != INT_MAX)
        && (   (parameterBlockStartPosition == INT_MAX)
            || (   (multilineCommentStartPosition > parameterBlockStartPosition)
                && (multilineCommentEndPosition > start)))) {
        // There is a previous /* XXX */ comment to style

        styleTextPreviousMultilineComment(multilineCommentStartPosition,
                                          pBytesStart, pBytesEnd,
                                          pText, pParameterBlock);
    } else if (   (parameterBlockStartPosition != INT_MAX)
               && (   (multilineCommentStartPosition == INT_MAX)
                   || (   (parameterBlockStartPosition > multilineCommentStartPosition)
                       && (parameterBlockEndPosition > start)))) {
        // There is a previous parameter block to style

        styleTextPreviousParameterBlock(parameterBlockStartPosition,
                                        pBytesStart, pBytesEnd,
                                        pText, pParameterBlock);
    } else {
        // Style the current text

        styleTextCurrent(pBytesStart, pBytesEnd, pText,
                         (parameterBlockStartPosition < start) && (parameterBlockEndPosition > start));
    }
}

//==============================================================================

void CellmlTextViewLexer::styleTextCurrent(int pBytesStart, int pBytesEnd,
                                           const QString &pText,
                                           bool pParameterBlock)
{
    // Make sure that we are given some text to style

    if (pBytesStart == pBytesEnd) {
        return;
    }

    // Check whether the given text contains a string, a // comment, a /* XXX */
    // comment or a parameter block

    int stringPosition = pText.indexOf(StringString);
    int singleLineCommentPosition = pText.indexOf(SingleLineCommentString);
    int multilineCommentStartPosition = pText.indexOf(StartMultilineCommentString);
    int parameterBlockStartPosition = pText.indexOf(StartParameterBlockString);

    stringPosition = (stringPosition == -1)?INT_MAX:stringPosition;
    singleLineCommentPosition = (singleLineCommentPosition == -1)?INT_MAX:singleLineCommentPosition;
    multilineCommentStartPosition = (multilineCommentStartPosition == -1)?INT_MAX:multilineCommentStartPosition;
    parameterBlockStartPosition = (parameterBlockStartPosition == -1)?INT_MAX:parameterBlockStartPosition;

    if (   (stringPosition != INT_MAX)
        && (stringPosition < singleLineCommentPosition)
        && (stringPosition < multilineCommentStartPosition)) {
        // There is a string to style

        styleTextString(stringPosition, pBytesStart, pBytesEnd, pText, pParameterBlock);
    } else if (   (singleLineCommentPosition != INT_MAX)
               && (singleLineCommentPosition < stringPosition)
               && (singleLineCommentPosition < multilineCommentStartPosition)) {
        // There is a // comment to style, so first style everything that is
        // before it

        int singleLineCommentBytesPosition = fullTextBytesPosition(fullTextPosition(pBytesStart)+singleLineCommentPosition);

        styleTextCurrent(pBytesStart, singleLineCommentBytesPosition,
                         pText.left(singleLineCommentPosition),
                         pParameterBlock);

        // Style the // comment itself, after having looked for the end of the
        // line, if any

        int eolPosition = pText.indexOf(mEolString, singleLineCommentPosition+SingleLineCommentLength);
        int eolBytesPosition = (eolPosition == -1)?-1:textBytesPosition(pText, eolPosition);
        int end = (eolBytesPosition == -1)?pBytesEnd:pBytesStart+eolBytesPosition+mEolString.length();

        startStyling(singleLineCommentBytesPosition);
        setStyling(end-singleLineCommentBytesPosition, int(Style::SingleLineComment));

        // Now, style everything that is after the // comment, if anything

        if (eolBytesPosition != -1) {
            styleText(end, pBytesEnd,
                      pText.right(fullTextLength(end, pBytesEnd)),
                      pParameterBlock);
        }
    } else if (   (multilineCommentStartPosition != INT_MAX)
               && (multilineCommentStartPosition < stringPosition)
               && (multilineCommentStartPosition < singleLineCommentPosition)) {
        // There is a /* XXX */ comment to style, so first style everything that
        // is before it

        int multilineCommentStartBytesPosition = fullTextBytesPosition(fullTextPosition(pBytesStart)+multilineCommentStartPosition);

        styleTextCurrent(pBytesStart, multilineCommentStartBytesPosition,
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

        int start = fullTextPosition(pBytesStart);
        int multilineCommentParameterBlockStartPosition = findString(StartParameterBlockString, start+multilineCommentStartPosition, int(Style::ParameterBlock), false);
        int multilineCommentParameterBlockEndPosition = findString(EndParameterBlockString, multilineCommentParameterBlockStartPosition, int(Style::ParameterBlock));

        multilineCommentParameterBlockStartPosition = (multilineCommentParameterBlockStartPosition == -1)?INT_MAX:multilineCommentParameterBlockStartPosition;
        multilineCommentParameterBlockEndPosition = (multilineCommentParameterBlockEndPosition == -1)?INT_MAX:multilineCommentParameterBlockEndPosition;

        int absoluteMultilineCommentStartPosition = start+multilineCommentStartPosition;

        styleText(multilineCommentStartBytesPosition, pBytesEnd,
                  pText.right(fullTextLength(multilineCommentStartBytesPosition, pBytesEnd)),
                     (multilineCommentParameterBlockStartPosition < absoluteMultilineCommentStartPosition)
                  && (absoluteMultilineCommentStartPosition < multilineCommentParameterBlockEndPosition));
    } else if (   (parameterBlockStartPosition != INT_MAX)
               && (parameterBlockStartPosition < stringPosition)
               && (parameterBlockStartPosition < singleLineCommentPosition)
               && (parameterBlockStartPosition < multilineCommentStartPosition)) {
        // There is a parameter block, so first style everything that is before
        // it

        int parameterBlockStartBytesPosition = fullTextBytesPosition(fullTextPosition(pBytesStart)+parameterBlockStartPosition);

        styleTextCurrent(pBytesStart, parameterBlockStartBytesPosition,
                         pText.left(parameterBlockStartPosition),
                         pParameterBlock);

        // Now style everything from the parameter block onwards
        // Note: to style everything from the parameter block onwards means that
        //       we will find that a parameter block starts at the beginning of
        //       the 'new' given text...

        styleText(parameterBlockStartBytesPosition, pBytesEnd,
                  pText.right(fullTextLength(parameterBlockStartBytesPosition, pBytesEnd)),
                  pParameterBlock);
    } else {
        // Style the given text as a parameter block, if needed

        if (pParameterBlock) {
            startStyling(pBytesStart);
            setStyling(pBytesEnd-pBytesStart, int(Style::ParameterBlock));
        }

        // Check whether the given text contains some keywords from various
        // categories

        static const QRegularExpression KeywordsRegEx = QRegularExpression(
            "\\b("
                // CellML Text keywords

                "and|as|between|case|comp|def|endcomp|enddef|endsel|for|group|"
                "import|incl|map|model|otherwise|sel|unit|using|var|vars|"

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

                "sin|cos|tan|sec|csc|cot|sinh|cosh|tanh|sech|csch|coth|asin|"
                "acos|atan|asec|acsc|acot|asinh|acosh|atanh|asech|acsch|acoth|"

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

                "ampere|becquerel|candela|celsius|coulomb|dimensionless|farad|"
                "gram|gray|henry|hertz|joule|katal|kelvin|kilogram|liter|litre|"
                "lumen|lux|meter|metre|mole|newton|ohm|pascal|radian|second|"
                "siemens|sievert|steradian|tesla|volt|watt|weber"
            ")\\b");

        if (pParameterBlock) {
            styleTextRegEx(pBytesStart, pText, ParameterKeywordsRegEx, int(Style::ParameterKeyword));
            styleTextRegEx(pBytesStart, pText, ParameterCellmlKeywordsRegEx, int(Style::ParameterCellmlKeyword));
        } else {
            styleTextRegEx(pBytesStart, pText, KeywordsRegEx, int(Style::Keyword));
            styleTextRegEx(pBytesStart, pText, CellmlKeywordsRegEx, int(Style::CellmlKeyword));
        }

        styleTextRegEx(pBytesStart, pText, SiUnitKeywordsRegEx,
                       pParameterBlock?
                           int(Style::ParameterCellmlKeyword):
                           int(Style::CellmlKeyword));

        // Check whether the given text contains some numbers

        styleTextNumberRegEx(pBytesStart, pText,
                             pParameterBlock?
                                 int(Style::ParameterNumber):
                                 int(Style::Number));

        // Let QScintilla know that we are done with the styling of the given
        // text
        // Note: indeed, QScintilla uses the end position of the last bit of
        //       text that has been styled to determine the starting position of
        //       the next bit of text that needs to be styled (see
        //       QsciLexerCustom::handleStyleNeeded()). Now, depending on
        //       whether keywords and/or numbers have been styled, the end
        //       position may or not be pBytesEnd. So, here, we make sure that
        //       it is, so that QScintilla can work as it should...

        startStyling(pBytesEnd);
    }
}

//==============================================================================

void CellmlTextViewLexer::styleTextPreviousMultilineComment(int pPosition,
                                                            int pBytesStart,
                                                            int pBytesEnd,
                                                            const QString &pText,
                                                            bool pParameterBlock)
{
    // A /* XXX */ comment started before or at the beginning of the given text,
    // so now look for where it ends

    int multilineCommentEndPosition = findString(EndMultilineCommentString, pPosition, int(Style::MultilineComment));

    if (multilineCommentEndPosition == -1) {
        multilineCommentEndPosition = mFullText.length();
    }

    // Check whether the beginning of the given text is within the /* XXX */
    // comment

    int start = fullTextPosition(pBytesStart);

    if ((pPosition <= start) && (start <= multilineCommentEndPosition)) {
        // The beginning of the given text is within a /* XXX */ comment, so
        // style it

        int realBytesEnd = fullTextBytesPosition(multilineCommentEndPosition)+EndMultilineCommentLength;
        int bytesEnd = qMin(pBytesEnd, realBytesEnd);

        startStyling(pBytesStart);
        setStyling(bytesEnd-pBytesStart, int(Style::MultilineComment));

        // Now, style everything that is behind the /* XXX */ comment, if
        // anything

        if (bytesEnd != pBytesEnd) {
            if (pParameterBlock) {
                // Our /* XXX */ comment is within a parameter block, so finish
                // styling our parameter block

                styleTextPreviousParameterBlock(fullTextPosition(bytesEnd), bytesEnd, pBytesEnd,
                                                pText.right(fullTextLength(bytesEnd, pBytesEnd)),
                                                false);
            } else {
                styleText(bytesEnd, pBytesEnd,
                          pText.right(fullTextLength(bytesEnd, pBytesEnd)),
                          pParameterBlock);
            }
        }
    } else {
        // The beginning of the given text is not within a /* XXX */ comment, so
        // style it

        styleTextCurrent(pBytesStart, pBytesEnd, pText, pParameterBlock);
    }
}

//==============================================================================

void CellmlTextViewLexer::styleTextPreviousParameterBlock(int pPosition,
                                                          int pBytesStart,
                                                          int pBytesEnd,
                                                          const QString &pText,
                                                          bool pParameterBlock)
{
    // A parameter block started before or at the beginning of the given text,
    // so now look for where it ends

    int parameterBlockEndPosition = findString(EndParameterBlockString, pPosition, int(Style::ParameterBlock));

    if (parameterBlockEndPosition == -1) {
        parameterBlockEndPosition = mFullText.length();
    }

    // Check whether the beginning of the given text is within a parameter block

    int start = fullTextPosition(pBytesStart);

    if ((pPosition <= start) && (start <= parameterBlockEndPosition)) {
        // The beginning of the given text is within a parameter block, so style
        // it as such

        int realBytesEnd = fullTextBytesPosition(parameterBlockEndPosition)+EndParameterBlockLength;
        int bytesEnd = qMin(pBytesEnd, realBytesEnd);
        bool hasStart =    (pPosition == start)
                        && (mFullText.mid(pPosition, StartParameterBlockLength) == StartParameterBlockString);
        bool hasEnd = bytesEnd == realBytesEnd;

        // If needed, style the start of the parameter block

        if (hasStart) {
            startStyling(pBytesStart);
            setStyling(StartParameterBlockLength, int(Style::ParameterBlock));
        }

        // Now style the contents of the parameter block itself

        int newBytesStart = pBytesStart+(hasStart?StartParameterBlockLength:0);
        int newBytesEnd = bytesEnd-(hasEnd?EndParameterBlockLength:0);

        styleTextCurrent(newBytesStart, newBytesEnd,
                         pText.mid(fullTextLength(pBytesStart, newBytesStart),
                                   fullTextLength(newBytesStart, newBytesEnd)),
                         true);

        // If needed, style the end of the parameter block, as well as what is
        // behind it

        if (hasEnd) {
            startStyling(bytesEnd-EndParameterBlockLength);
            setStyling(EndParameterBlockLength, int(Style::ParameterBlock));

            styleText(bytesEnd, pBytesEnd,
                      pText.right(fullTextLength(bytesEnd, pBytesEnd)),
                      pParameterBlock);
        }
    } else {
        // The beginning of the given text is not within a parameter block, so
        // style it

        styleTextCurrent(pBytesStart, pBytesEnd, pText, pParameterBlock);
    }
}

//==============================================================================

void CellmlTextViewLexer::styleTextString(int pPosition, int pBytesStart,
                                          int pBytesEnd, const QString &pText,
                                          bool pParameterBlock)
{
    // There is a string to style, so first style everything that is before it

    int bytesPosition = fullTextBytesPosition(fullTextPosition(pBytesStart)+pPosition);

    styleTextCurrent(pBytesStart, bytesPosition, pText.left(pPosition), pParameterBlock);

    // Now, check where the string ends, if anywhere

    int nextBytesStart = -1;
    int stringEndPosition = pText.indexOf(StringString, pPosition+StringLength);

    if (stringEndPosition != -1) {
        // The string ends somewhere, so compare its line number against that of
        // where it started

        int stringStartLineNumber, stringStartColumnNumber;
        int stringEndLineNumber, stringEndColumnNumber;
        int stringEndBytesPosition = textBytesPosition(pText, stringEndPosition);

        editor()->lineIndexFromPosition(bytesPosition,
                                        &stringStartLineNumber, &stringStartColumnNumber);
        editor()->lineIndexFromPosition(pBytesStart+stringEndBytesPosition,
                                        &stringEndLineNumber, &stringEndColumnNumber);

        if (stringStartLineNumber == stringEndLineNumber) {
            // The string starts and ends on the same line, so get ready to
            // style everything that is after the string

            nextBytesStart = pBytesStart+stringEndBytesPosition+StringLength;
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
        int eolBytesPosition = (eolPosition == -1)?-1:textBytesPosition(pText, eolPosition);

        if (eolBytesPosition != -1) {
            nextBytesStart = pBytesStart+eolBytesPosition+mEolString.length();
        }
    }

    // Style the string itself

    startStyling(bytesPosition);
    setStyling(((nextBytesStart == -1)?pBytesEnd:nextBytesStart)-bytesPosition,
               pParameterBlock?
                   int(Style::ParameterString):
                   int(Style::String));

    // Style whatever is after the string

    if (nextBytesStart != -1) {
        styleText(nextBytesStart, pBytesEnd,
                  pText.right(fullTextLength(nextBytesStart, pBytesEnd)),
                  pParameterBlock);
    }
}

//==============================================================================

void CellmlTextViewLexer::styleTextRegEx(int pBytesStart,
                                         const QString &pText,
                                         const QRegularExpression &pRegEx,
                                         int pRegExStyle)
{
    // Style the given text using the given regular expression

    QRegularExpressionMatchIterator regExMatchIter = pRegEx.globalMatch(pText);
    QRegularExpressionMatch regExMatch;

    while (regExMatchIter.hasNext()) {
        regExMatch = regExMatchIter.next();

        // We have a match, so style it

        int matchBytesStart = textBytesPosition(pText, regExMatch.capturedStart());

        startStyling(pBytesStart+matchBytesStart);
        setStyling(textBytesPosition(pText, regExMatch.capturedStart()+regExMatch.capturedLength())-matchBytesStart,
                   pRegExStyle);
    }
}

//==============================================================================

void CellmlTextViewLexer::styleTextNumberRegEx(int pBytesStart,
                                               const QString &pText,
                                               int pRegExStyle)
{
    // Style the given text using the number regular expression

    static const QRegularExpression NumberRegEx = QRegularExpression(R"((\d+(\.\d*)?|\.\d+)([eE][+-]?\d*)?)");
    // Note: this regular expression is not aimed at catching valid numbers, but
    //       at catching something that could become a valid number (e.g. we
    //       want to be able to catch "123e")...

    QRegularExpressionMatchIterator regExMatchIter = NumberRegEx.globalMatch(pText);
    QRegularExpressionMatch regExMatch;

    while (regExMatchIter.hasNext()) {
        regExMatch = regExMatchIter.next();

        // We have a match, so style it, but only if:
        //  - The character in front of the match is not in [0-9a-zA-Z_] and is
        //    part of the ASCII table
        //  - The character following the match is not in [a-zA-Z_.] and is part
        //    of the ASCII table

        int prevCharPos = fullTextPosition(pBytesStart)+regExMatch.capturedStart()-1;
        int nextCharPos = prevCharPos+regExMatch.capturedLength()+1;

        ushort prevChar = ((prevCharPos >= 0)?mFullText[prevCharPos]:QChar()).unicode();
        ushort nextChar = ((nextCharPos < mFullText.length())?mFullText[nextCharPos]:QChar()).unicode();

        if ((       (prevChar  <  48) || ((prevChar  >  57) && (prevChar <  65))
                || ((prevChar  >  90) &&  (prevChar  <  95))
                ||  (prevChar ==  96) || ((prevChar  > 122) && (prevChar < 128)))
            && (    (nextChar  <  46) || ((nextChar  >  46) && (nextChar <  65))
                || ((nextChar  >  90) &&  (nextChar  <  95))
                ||  (nextChar ==  96) || ((nextChar  > 122) && (nextChar < 128)))) {
            int matchBytesStart = textBytesPosition(pText, regExMatch.capturedStart());

            startStyling(pBytesStart+matchBytesStart);
            setStyling(textBytesPosition(pText, regExMatch.capturedStart()+regExMatch.capturedLength())-matchBytesStart,
                       pRegExStyle);
        }
    }
}

//==============================================================================

bool CellmlTextViewLexer::validString(int pFrom, int pTo, int pStyle) const
{
    // Check whether the string, which range is given, is valid, i.e. is either
    // of the default or given style

    qint64 style;

    for (int i = pFrom; i < pTo; ++i) {
        style = editor()->SendScintilla(QsciScintilla::SCI_GETSTYLEAT,
                                        fullTextBytesPosition(i));

        if ((style != int(Style::Default)) && (style != pStyle)) {
            return false;
        }
    }

    return true;
}

//==============================================================================

int CellmlTextViewLexer::findString(const QString &pString, int pFrom,
                                    int pStyle, bool pForward)
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

int CellmlTextViewLexer::fullTextPosition(int pBytesPosition) const
{
    // Return the corresponding position within mFullText of the given
    // byte-based position within mFullTextUtf8

    return QString(mFullTextUtf8.left(pBytesPosition)).length();
}

//==============================================================================

int CellmlTextViewLexer::fullTextLength(int pBytesStart, int pBytesEnd) const
{
    // Return the corresponding length within mFullText of the given byte-based
    // length of a substring within mFullTextUtf8

    return fullTextPosition(pBytesEnd)-fullTextPosition(pBytesStart);
}

//==============================================================================

int CellmlTextViewLexer::fullTextBytesPosition(int pPosition) const
{
    // Return the byte-based value of the given position within mFullText

    return mFullText.left(pPosition).toUtf8().length();
}

//==============================================================================

int CellmlTextViewLexer::textBytesPosition(const QString &pText,
                                           int pPosition) const
{
    // Return the byte-based value of the given position within the given text

    return pText.left(pPosition).toUtf8().length();
}

//==============================================================================

} // namespace CellMLTextView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
