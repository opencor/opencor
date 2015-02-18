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
// Lexer for the CellML text format
//==============================================================================

#include "cellmltextviewlexer.h"
#include "qscintillawidget.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

CellmlTextViewLexer::CellmlTextViewLexer(QObject *pParent) :
    QsciLexerCustom(pParent),
    mFullText(QString()),
    mFullTextUtf8(QByteArray()),
    mEolString(QString())
{
    // Some initialisations

    mKeywordsRegEx = QRegularExpression(
                         "\\b("
                             // CellML text keywords

                             "and|as|between|case|comp|def|endcomp|enddef|"
                             "endsel|for|group|import|incl|map|model|otherwise|"
                             "sel|unit|using|var|vars|"

                             // MathML arithmetic operators

                             "abs|ceil|exp|fact|floor|ln|log|pow|root|sqr|sqrt|"

                             // MathML logical operators

                             "and|or|xor|not|"

                             // MathML calculus elements

                             "ode|"

                             // MathML trigonometric operators

                             "sin|cos|tan|sec|csc|cot|sinh|cosh|tanh|sech|csch|"
                             "coth|asin|acos|atan|asec|acsc|acot|asinh|acosh|"
                             "atanh|asech|acsch|acoth|"

                             // MathML constants

                             "true|false|nan|pi|inf|e"
                         ")\\b");

    mCellmlKeywordsRegEx = QRegularExpression(
                               "\\b("
                                    // Miscellaneous

                                   "base|encapsulation|containment"
                               ")\\b");

    mSiUnitKeywordsRegEx = QRegularExpression(
                               "\\b("
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

    mNumberRegEx = QRegularExpression("(\\d+(\\.\\d*)?|\\.\\d+)([eE][+-]?\\d*)?");
    // Note: this regular expression is not aimed at catching valid numbers, but
    //       at catching something that could become a valid number (e.g. we
    //       want to be able to catch "123e")...
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

    switch (pStyle) {
    case Default:
        return QObject::tr("Default");
    case SingleLineComment:
        return QObject::tr("Single line comment");
    case MultilineComment:
        return QObject::tr("Multiline comment");
    case Keyword:
        return QObject::tr("Keyword");
    case CellmlKeyword:
        return QObject::tr("CellML keyword");
    case Number:
        return QObject::tr("Number");
    case String:
        return QObject::tr("String");
    case ParameterBlock:
        return QObject::tr("Parameter block");
    case ParameterKeyword:
        return QObject::tr("Parameter keyword");
    case ParameterCellmlKeyword:
        return QObject::tr("Parameter CellML keyword");
    case ParameterNumber:
        return QObject::tr("Parameter number");
    case ParameterString:
        return QObject::tr("Parameter string");
    }

    return QString();
}

//==============================================================================

QColor CellmlTextViewLexer::color(int pStyle) const
{
    // Return the given style's colour

    switch (pStyle) {
    case Default:
    case ParameterBlock:
        return QColor(0x1f, 0x1f, 0x1f);
    case SingleLineComment:
    case MultilineComment:
        return QColor(0x7f, 0x7f, 0x7f);
    case Keyword:
    case ParameterKeyword:
        return QColor(0x00, 0x00, 0x7f);
    case CellmlKeyword:
    case ParameterCellmlKeyword:
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

QFont CellmlTextViewLexer::font(int pStyle) const
{
    // Return the given style's colour

    QFont res = QsciLexer::font(pStyle);

    switch (pStyle) {
    case ParameterBlock:
    case ParameterKeyword:
    case ParameterCellmlKeyword:
    case ParameterNumber:
    case ParameterString:
        res.setItalic(true);

        break;
    }

    return res;
}

//==============================================================================

void CellmlTextViewLexer::styleText(int pBytesStart, int pBytesEnd)
{
    // Make sure that we have an editor

    if (!editor())
        return;

    // Retrieve the text to style

    char *data = new char[pBytesEnd-pBytesStart+1];

    editor()->SendScintilla(QsciScintilla::SCI_GETTEXTRANGE,
                            pBytesStart, pBytesEnd, data);

    QString text = QString(data);

    delete[] data;

    // Use a default style for our text
    // Note: this is so that validString() can work properly...

    startStyling(pBytesStart);
    setStyling(pBytesEnd-pBytesStart, Default);

    // Effectively style our text

    mFullText = editor()->text();
    mFullTextUtf8 = mFullText.toUtf8();

    mEolString = qobject_cast<QScintillaSupport::QScintillaWidget *>(editor())->eolString();

    doStyleText(pBytesStart, pBytesEnd, text, false);

#ifdef QT_DEBUG
    // Make sure that the end position of the last bit of text that we styled is
    // pBytesEnd
    // Note: we need to ensure that it is the case, so that validString() can
    //       take advantage of the style of a given character in the editor...

    if (editor()->SendScintilla(QsciScintillaBase::SCI_GETENDSTYLED) != pBytesEnd)
        qFatal("FATAL ERROR | %s:%d: the styling of the text must be incremental.", __FILE__, __LINE__);
#endif
}

//==============================================================================

static const auto SingleLineCommentString = QStringLiteral("//");
static const int SingleLineCommentLength  = SingleLineCommentString.length();

//==============================================================================

static const auto StartMultilineCommentString = QStringLiteral("/*");
static const auto EndMultilineCommentString   = QStringLiteral("*/");
static const int StartMultilineCommentLength  = StartMultilineCommentString.length();
static const int EndMultilineCommentLength    = EndMultilineCommentString.length();

//==============================================================================

static const auto StartParameterBlockString = QStringLiteral("{");
static const auto EndParameterBlockString   = QStringLiteral("}");
static const int StartParameterBlockLength  = StartParameterBlockString.length();
static const int EndParameterBlockLength    = EndParameterBlockString.length();

//==============================================================================

static const auto StringString = QStringLiteral("\"");
static const int StringLength = StringString.length();

//==============================================================================

void CellmlTextViewLexer::doStyleText(const int &pBytesStart,
                                      const int &pBytesEnd,
                                      const QString &pText,
                                      const bool &pParameterBlock)
{
    // Make sure that we are given some text to style

    if (pBytesStart == pBytesEnd)
        return;

    // Check whether a /* XXX */ comment or a parameter block started before or
    // at the beginning of the given text

    int start = fullTextPosition(pBytesStart);
    int multilineCommentStartPosition = findString(StartMultilineCommentString, start, MultilineComment, false);
    int parameterBlockStartPosition = findString(StartParameterBlockString, start, ParameterBlock, false);

    multilineCommentStartPosition = (multilineCommentStartPosition == -1)?INT_MAX:multilineCommentStartPosition;
    parameterBlockStartPosition = (parameterBlockStartPosition == -1)?INT_MAX:parameterBlockStartPosition;

    if (   (multilineCommentStartPosition != INT_MAX)
        && (   (parameterBlockStartPosition == INT_MAX)
            || (multilineCommentStartPosition > parameterBlockStartPosition))) {
        // There is a previous /* XXX */ comment to style

        doStyleTextPreviousMultilineComment(multilineCommentStartPosition, pBytesStart, pBytesEnd, pText, pParameterBlock);
    } else if (   (parameterBlockStartPosition != INT_MAX)
               && (   (multilineCommentStartPosition == INT_MAX)
                   || (parameterBlockStartPosition > multilineCommentStartPosition))) {
        // There is a previous parameter block to style

        doStyleTextPreviousParameterBlock(parameterBlockStartPosition, pBytesStart, pBytesEnd, pText, pParameterBlock);
    } else {
        // Style the current text

        doStyleTextCurrent(pBytesStart, pBytesEnd, pText, pParameterBlock);
    }
}

//==============================================================================

void CellmlTextViewLexer::doStyleTextCurrent(const int &pBytesStart,
                                             const int &pBytesEnd,
                                             const QString &pText,
                                             const bool &pParameterBlock)
{
    // Make sure that we are given some text to style

    if (pBytesStart == pBytesEnd)
        return;

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
        && (stringPosition < multilineCommentStartPosition)
        && (stringPosition < parameterBlockStartPosition)) {
        // There is a string to style

        doStyleTextString(stringPosition, pBytesStart, pBytesEnd, pText,
                          pParameterBlock);
    } else if (   (singleLineCommentPosition != INT_MAX)
               && (singleLineCommentPosition < stringPosition)
               && (singleLineCommentPosition < multilineCommentStartPosition)
               && (singleLineCommentPosition < parameterBlockStartPosition)) {
        // There is a // comment to style

        doStyleTextSingleLineComment(singleLineCommentPosition, pBytesStart, pBytesEnd,
                                     pText, pParameterBlock);
    } else if (   (multilineCommentStartPosition != INT_MAX)
               && (multilineCommentStartPosition < stringPosition)
               && (multilineCommentStartPosition < singleLineCommentPosition)
               && (multilineCommentStartPosition < parameterBlockStartPosition)) {
        // There is a /* XXX */ comment to style, so first style everything that
        // is before it

        int multilineCommentStartBytesPosition = textBytesPosition(pText, multilineCommentStartPosition);

        doStyleTextCurrent(pBytesStart, pBytesStart+multilineCommentStartBytesPosition,
                           pText.left(multilineCommentStartPosition),
                           pParameterBlock);

        // Now style everything from the comment onwards
        // Note: to style everything from the comment onwards means that we will
        //       find that a /* XXX */ comment starts at the beginning of the
        //       'new' given text...

        doStyleText(pBytesStart+multilineCommentStartBytesPosition, pBytesEnd,
                    pText.right(fullTextLength(pBytesStart, pBytesEnd-pBytesStart)-multilineCommentStartPosition),
                    pParameterBlock);
    } else if (   (parameterBlockStartPosition != INT_MAX)
               && (parameterBlockStartPosition < stringPosition)
               && (parameterBlockStartPosition < singleLineCommentPosition)
               && (parameterBlockStartPosition < multilineCommentStartPosition)) {
        // There is a parameter block, so first style everything that is before
        // it

        int parameterBlockStartBytesPosition = textBytesPosition(pText, parameterBlockStartPosition);

        doStyleTextCurrent(pBytesStart, pBytesStart+parameterBlockStartBytesPosition,
                           pText.left(parameterBlockStartPosition),
                           pParameterBlock);

        // Now style everything from the parameter block onwards
        // Note: to style everything from the parameter block onwards means that
        //       we will find that a parameter block starts at the beginning of
        //       the 'new' given text...

        doStyleText(pBytesStart+parameterBlockStartBytesPosition, pBytesEnd,
                    pText.right(fullTextLength(pBytesStart, pBytesEnd-pBytesStart)-parameterBlockStartPosition),
                    pParameterBlock);
    } else {
        // Style the given text as a parameter block, if needed

        if (pParameterBlock) {
            startStyling(pBytesStart);
            setStyling(pBytesEnd-pBytesStart, ParameterBlock);
        }

        // Check whether the given text contains some keywords from various
        // categories

        if (pParameterBlock) {
            doStyleTextRegEx(pBytesStart, pText, mParameterKeywordsRegEx, ParameterKeyword);
            doStyleTextRegEx(pBytesStart, pText, mParameterValueKeywordsRegEx, ParameterCellmlKeyword);
        } else {
            doStyleTextRegEx(pBytesStart, pText, mKeywordsRegEx, Keyword);
            doStyleTextRegEx(pBytesStart, pText, mCellmlKeywordsRegEx, CellmlKeyword);
        }

        doStyleTextRegEx(pBytesStart, pText, mSiUnitKeywordsRegEx, pParameterBlock?ParameterCellmlKeyword:CellmlKeyword);

        // Check whether the given text contains some numbers

        doStyleTextNumberRegEx(pBytesStart, pText, pParameterBlock?ParameterNumber:Number);

        // Let QScintilla know that we are done with the styling of the given
        // text
        // Note: indeed, QScintilla uses the end position of the last bit of
        //       text that has been styled to determine the starting position of
        //       the next bit of text that needs to be styled (see
        //       QsciLexerCustom::handleStyleNeeded()). Now, depending on
        //       whether keywords and/or numbers have been styled, the end
        //       position may or not be pBytesEnd. So, here, we make sure that
        //       it is pBytesEnd, so that QScintilla can work optimally...

        startStyling(pBytesEnd);
    }
}

//==============================================================================

void CellmlTextViewLexer::doStyleTextSingleLineComment(const int &pPosition,
                                                       const int &pBytesStart,
                                                       const int &pBytesEnd,
                                                       const QString &pText,
                                                       const bool &pParameterBlock)
{
    // There is a // comment to style, so first style everything that is before
    // it

    int bytesPosition = fullTextBytesPosition(pPosition);

    doStyleTextCurrent(pBytesStart, pBytesStart+bytesPosition,
                       pText.left(pPosition), pParameterBlock);

    // Style the // comment itself, after having looked for the end of the line,
    // if any

    int eolPosition = pText.indexOf(mEolString, pPosition+SingleLineCommentLength);
    int eolBytesPosition = (eolPosition == -1)?-1:textBytesPosition(pText, eolPosition);
    int start = pBytesStart+bytesPosition;
    int end = (eolBytesPosition == -1)?pBytesEnd:pBytesStart+eolBytesPosition+mEolString.length();

    startStyling(start);
    setStyling(end-start, SingleLineComment);

    // Now, style everything that is after the // comment, if anything

    if (eolBytesPosition != -1) {
        doStyleText(end, pBytesEnd,
                    pText.right(fullTextLength(end, pBytesEnd-end)),
                    pParameterBlock);
    }
}

//==============================================================================

void CellmlTextViewLexer::doStyleTextPreviousMultilineComment(const int &pPosition,
                                                              const int &pBytesStart,
                                                              const int &pBytesEnd,
                                                              const QString &pText,
                                                              const bool &pParameterBlock)
{
    // A /* XXX */ comment started before or at the beginning of the given text,
    // so now look for where it ends

    int multilineCommentEndPosition = findString(EndMultilineCommentString, pPosition, MultilineComment);

    if (multilineCommentEndPosition == -1)
        multilineCommentEndPosition = mFullText.length();

    // Check whether the beginning of the given text is within the /* XXX */
    // comment

    int start = fullTextPosition(pBytesStart);

    if ((pPosition <= start) && (start <= multilineCommentEndPosition)) {
        // The beginning of the given text is within a /* XXX */ comment, so
        // style it

        int realBytesEnd = fullTextBytesPosition(multilineCommentEndPosition+EndMultilineCommentLength);
        int bytesEnd = qMin(pBytesEnd, realBytesEnd);

        startStyling(pBytesStart);
        setStyling(bytesEnd-pBytesStart, MultilineComment);

        // Now, style everything that is behind the /* XXX */ comment, if
        // anything

        if (bytesEnd != pBytesEnd) {
            doStyleText(bytesEnd, pBytesEnd,
                        pText.right(fullTextLength(bytesEnd, pBytesEnd-bytesEnd)),
                        pParameterBlock);
        }
    } else {
        // The beginning of the given text is not within a /* XXX */ comment, so
        // style it

        doStyleTextCurrent(pBytesStart, pBytesEnd, pText, pParameterBlock);
    }
}

//==============================================================================

void CellmlTextViewLexer::doStyleTextPreviousParameterBlock(const int &pPosition,
                                                            const int &pBytesStart,
                                                            const int &pBytesEnd,
                                                            const QString &pText,
                                                            const bool &pParameterBlock)
{
    // A parameter block started before or at the beginning of the given text,
    // so now look for where it ends

    int parameterBlockEndPosition = findString(EndParameterBlockString, pPosition, ParameterBlock);

    if (parameterBlockEndPosition == -1)
        parameterBlockEndPosition = mFullText.length();

    // Check whether the beginning of the given text is within a parameter block

    int start = fullTextPosition(pBytesStart);

    if ((pPosition <= start) && (start <= parameterBlockEndPosition)) {
        // The beginning of the given text is within a parameter block, so style
        // it as such

        int realBytesEnd = fullTextBytesPosition(parameterBlockEndPosition+EndParameterBlockLength);
        int bytesEnd = qMin(pBytesEnd, realBytesEnd);
        bool hasStart = pPosition == start;
        bool hasEnd = bytesEnd == realBytesEnd;

        // If possible, style the start of the parameter block

        if (hasStart) {
            startStyling(pBytesStart);
            setStyling(StartParameterBlockLength, ParameterBlock);
        }

        // Now style the contents of the parameter block itself

        int newStart = pBytesStart+(hasStart?StartParameterBlockLength:0);
        int newEnd = bytesEnd-(hasEnd?EndParameterBlockLength:0);

        doStyleTextCurrent(newStart, newEnd,
                           pText.mid(fullTextLength(pBytesStart, newStart-pBytesStart),
                                     fullTextLength(newStart, newEnd-newStart)),
                           true);

        // If possible, style the end of the parameter block, as well as what is
        // behind it

        if (hasEnd) {
            startStyling(bytesEnd-EndParameterBlockLength);
            setStyling(EndParameterBlockLength, ParameterBlock);

            doStyleText(bytesEnd, pBytesEnd,
                        pText.right(fullTextLength(bytesEnd, pBytesEnd-bytesEnd)),
                        pParameterBlock);
        }
    } else {
        // The beginning of the given text is not within a parameter block, so
        // style it

        doStyleTextCurrent(pBytesStart, pBytesEnd, pText, pParameterBlock);
    }
}

//==============================================================================

void CellmlTextViewLexer::doStyleTextString(const int &pPosition,
                                            const int &pBytesStart,
                                            const int &pBytesEnd,
                                            const QString &pText,
                                            const bool &pParameterBlock)
{
    // There is a string to style, so first style everything that is before it

    int bytesPosition = fullTextBytesPosition(pPosition);

    doStyleTextCurrent(pBytesStart, pBytesStart+bytesPosition,
                       pText.left(pPosition), pParameterBlock);

    // Now, check where the string ends, if anywhere

    int nextBytesStart = -1;
    int stringEndPosition = pText.indexOf(StringString, pPosition+StringLength);

    if (stringEndPosition != -1) {
        // The string ends somewhere, so compare its line number against that of
        // where it started

        int stringStartLineNumber, stringStartColumnNumber;
        int stringEndLineNumber, stringEndColumnNumber;
        int stringEndBytesPosition = textBytesPosition(pText, stringEndPosition);

        editor()->lineIndexFromPosition(pBytesStart+bytesPosition,
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

        if (eolBytesPosition != -1)
            nextBytesStart = pBytesStart+eolBytesPosition+mEolString.length();
    }

    // Style the string itself

    int bytesStart = pBytesStart+bytesPosition;

    startStyling(bytesStart);
    setStyling(((nextBytesStart == -1)?pBytesEnd:nextBytesStart)-bytesStart,
               pParameterBlock?ParameterString:String);

    // Style whatever is after the string

    if (nextBytesStart != -1) {
        doStyleText(nextBytesStart, pBytesEnd,
                    pText.right(fullTextLength(nextBytesStart, pBytesEnd-nextBytesStart)),
                    pParameterBlock);
    }
}

//==============================================================================

void CellmlTextViewLexer::doStyleTextRegEx(const int &pBytesStart,
                                           const QString &pText,
                                           const QRegularExpression &pRegEx,
                                           const int &pRegExStyle)
{
    // Style the given text using the given regular expression

    QRegularExpressionMatchIterator regExMatchIter = pRegEx.globalMatch(pText);
    QRegularExpressionMatch regExMatch;

    while (regExMatchIter.hasNext()) {
        regExMatch = regExMatchIter.next();

        // We have a match, so style it

        startStyling(pBytesStart+textBytesPosition(pText, regExMatch.capturedStart()));
        setStyling(textBytesLength(pText, regExMatch.capturedStart(), regExMatch.capturedLength()), pRegExStyle);
    }
}

//==============================================================================

void CellmlTextViewLexer::doStyleTextNumberRegEx(const int &pBytesStart,
                                                 const QString &pText,
                                                 const int &pRegExStyle)
{
    // Style the given text using the number regular expression

    QRegularExpressionMatchIterator regExMatchIter = mNumberRegEx.globalMatch(pText);
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
            startStyling(pBytesStart+textBytesPosition(pText, regExMatch.capturedStart()));
            setStyling(textBytesLength(pText, regExMatch.capturedStart(), regExMatch.capturedLength()), pRegExStyle);
        }
    }
}

//==============================================================================

bool CellmlTextViewLexer::validString(const int &pFrom, const int &pTo,
                                      const int &pStyle) const
{
    // Check whether the string, which range is given, is valid, i.e. is either
    // of the default or given style

    int style;

    for (int i = pFrom; i < pTo; ++i) {
        style = editor()->SendScintilla(QsciScintilla::SCI_GETSTYLEAT,
                                        fullTextBytesPosition(i));

        if ((style != Default) && (style != pStyle))
            return false;
    }

    return true;
}

//==============================================================================

int CellmlTextViewLexer::findString(const QString &pString, int pFrom,
                                    const int &pStyle, const bool &pForward)
{
    // Find forward/backward the given string starting from the given position

    int stringLength = pString.length();
    int res = pForward?pFrom:pFrom+stringLength;

    do {
        pFrom = pForward?res+stringLength:res-1;

        res = pForward?mFullText.indexOf(pString, pFrom):mFullText.lastIndexOf(pString, pFrom);
    } while ((res != -1) && !validString(res, res+stringLength, pStyle));

    return res;
}

//==============================================================================

int CellmlTextViewLexer::fullTextPosition(const int &pBytesPosition) const
{
    // Return the corresponding position within mFullText of the given
    // byte-based position within mFullTextUtf8

    return QString(mFullTextUtf8.left(pBytesPosition)).length();
}

//==============================================================================

int CellmlTextViewLexer::fullTextLength(const int &pBytesFrom,
                                        const int &pBytesLength) const
{
    // Return the corresponding length within mFullText of the given byte-based
    // length of a substring within mFullTextUtf8

    return QString(mFullTextUtf8.mid(pBytesFrom, pBytesLength)).length();
}

//==============================================================================

int CellmlTextViewLexer::fullTextBytesPosition(const int &pPosition) const
{
    // Return the byte-based value of the given position within mFullText

    return mFullText.left(pPosition).toUtf8().length();
}

//==============================================================================

int CellmlTextViewLexer::textBytesPosition(const QString &pText,
                                           const int &pPosition) const
{
    // Return the byte-based value of the given position within the given text

    return pText.left(pPosition).toUtf8().length();
}

//==============================================================================

int CellmlTextViewLexer::textBytesLength(const QString &pText, const int &pFrom,
                                         const int &pLength) const
{
    // Return the byte-based length of a substring within the given text

    return pText.mid(pFrom, pLength).toUtf8().length();
}

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
