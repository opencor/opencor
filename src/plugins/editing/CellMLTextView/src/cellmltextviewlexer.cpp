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

    mNumberRegEx = QRegularExpression("\\b\\d*\\.?\\d+([eE][+-]?\\d+)?\\b");
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
    case ParameterGroup:
        return QObject::tr("Parameter group");
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
    case ParameterGroup:
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
    case ParameterGroup:
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

void CellmlTextViewLexer::styleText(int pStart, int pEnd)
{
    // Make sure that we have an editor

    if (!editor())
        return;

    // Retrieve the text to style

    char *data = new char[pEnd-pStart+1];

    editor()->SendScintilla(QsciScintilla::SCI_GETTEXTRANGE, pStart, pEnd, data);

    QString text = QString(data);

    delete[] data;

    // Use a default style for our text
    // Note: this is so that validString() can work properly...

    startStyling(pStart);
    setStyling(pEnd-pStart, Default);

    // Effectively style our text

    mFullText = editor()->text();
    mEolString = qobject_cast<QScintillaSupport::QScintillaWidget *>(editor())->eolString();

    doStyleText(pStart, pEnd, text, false);

#ifdef QT_DEBUG
    // Make sure that the end position of the last bit of text that we styled is
    // pEnd
    // Note: we need to ensure that it is the case, so that validString() can
    //       take advantage of the style of a given character in the editor...

    if (editor()->SendScintilla(QsciScintillaBase::SCI_GETENDSTYLED) != pEnd)
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

static const auto StartParameterGroupString = QStringLiteral("{");
static const auto EndParameterGroupString   = QStringLiteral("}");
static const int StartParameterGroupLength  = StartParameterGroupString.length();
static const int EndParameterGroupLength    = EndParameterGroupString.length();

//==============================================================================

static const auto StringString = QStringLiteral("\"");
static const int StringLength = StringString.length();

//==============================================================================

void CellmlTextViewLexer::doStyleText(int pStart, int pEnd, QString pText,
                                      bool pParameterGroup)
{
    // Make sure that we are given some text to style

    if (pStart == pEnd)
        return;

    // Check whether a /* XXX */ comment or a parameter group started before or
    // at the beginning of the given text

    int multilineCommentStartPosition = findString(StartMultilineCommentString, pStart, MultilineComment, false);
    int parameterGroupStartPosition = findString(StartParameterGroupString, pStart, ParameterGroup, false);

    multilineCommentStartPosition = (multilineCommentStartPosition == -1)?INT_MAX:multilineCommentStartPosition;
    parameterGroupStartPosition = (parameterGroupStartPosition == -1)?INT_MAX:parameterGroupStartPosition;

    if (   (multilineCommentStartPosition != INT_MAX)
        && (   (parameterGroupStartPosition == INT_MAX)
            || (multilineCommentStartPosition > parameterGroupStartPosition))) {
        // There is a previous /* XXX */ comment to style

        doStyleTextPreviousMultilineComment(multilineCommentStartPosition, pStart, pEnd, pText, pParameterGroup);
    } else if (   (parameterGroupStartPosition != INT_MAX)
               && (   (multilineCommentStartPosition == INT_MAX)
                   || (parameterGroupStartPosition > multilineCommentStartPosition))) {
        // There is a previous parameter group to style

        doStyleTextPreviousParameterGroup(parameterGroupStartPosition, pStart, pEnd, pText, pParameterGroup);
    } else {
        // Style the current text

        doStyleTextCurrent(pStart, pEnd, pText, pParameterGroup);
    }
}

//==============================================================================

void CellmlTextViewLexer::doStyleTextCurrent(int pStart, int pEnd,
                                             QString pText,
                                             bool pParameterGroup)
{
    // Make sure that we are given some text to style

    if (pStart == pEnd)
        return;

    // Check whether the given text contains a string, a // comment, a /* XXX */
    // comment or a parameter group

    int stringPosition = pText.indexOf(StringString);
    int singleLineCommentPosition = pText.indexOf(SingleLineCommentString);
    int multilineCommentStartPosition = pText.indexOf(StartMultilineCommentString);
    int parameterGroupStartPosition = pText.indexOf(StartParameterGroupString);

    stringPosition = (stringPosition == -1)?INT_MAX:stringPosition;
    singleLineCommentPosition = (singleLineCommentPosition == -1)?INT_MAX:singleLineCommentPosition;
    multilineCommentStartPosition = (multilineCommentStartPosition == -1)?INT_MAX:multilineCommentStartPosition;
    parameterGroupStartPosition = (parameterGroupStartPosition == -1)?INT_MAX:parameterGroupStartPosition;

    if (   (stringPosition != INT_MAX)
        && (stringPosition < singleLineCommentPosition)
        && (stringPosition < multilineCommentStartPosition)
        && (stringPosition < parameterGroupStartPosition)) {
        // There is a string to style

        doStyleTextString(stringPosition, pStart, pEnd, pText,
                          pParameterGroup);
    } else if (   (singleLineCommentPosition != INT_MAX)
               && (singleLineCommentPosition < stringPosition)
               && (singleLineCommentPosition < multilineCommentStartPosition)
               && (singleLineCommentPosition < parameterGroupStartPosition)) {
        // There is a // comment to style

        doStyleTextSingleLineComment(singleLineCommentPosition, pStart, pEnd,
                                     pText, pParameterGroup);
    } else if (   (multilineCommentStartPosition != INT_MAX)
               && (multilineCommentStartPosition < stringPosition)
               && (multilineCommentStartPosition < singleLineCommentPosition)
               && (multilineCommentStartPosition < parameterGroupStartPosition)) {
        // There is a /* XXX */ comment to style, so first style everything that
        // is before it

        doStyleTextCurrent(pStart, pStart+multilineCommentStartPosition,
                           pText.left(multilineCommentStartPosition),
                           pParameterGroup);

        // Now style everything from the comment onwards
        // Note: to style everything from the comment onwards means that we will
        //       find that a /* XXX */ comment starts at the beginning of the
        //       'new' given text...

        doStyleText(pStart+multilineCommentStartPosition, pEnd,
                    pText.right(pEnd-pStart-multilineCommentStartPosition),
                    pParameterGroup);
    } else if (   (parameterGroupStartPosition != INT_MAX)
               && (parameterGroupStartPosition < stringPosition)
               && (parameterGroupStartPosition < singleLineCommentPosition)
               && (parameterGroupStartPosition < multilineCommentStartPosition)) {
        // There is a parameter group, so first style everything that is before
        // it

        doStyleTextCurrent(pStart, pStart+parameterGroupStartPosition,
                           pText.left(parameterGroupStartPosition),
                           pParameterGroup);

        // Now style everything from the parameter group onwards
        // Note: to style everything from the parameter group onwards means that
        //       we will find that a parameter group starts at the beginning of
        //       the 'new' given text...

        doStyleText(pStart+parameterGroupStartPosition, pEnd,
                    pText.right(pEnd-pStart-parameterGroupStartPosition),
                    pParameterGroup);
    } else {
        // Style the given text as a parameter group, if needed

        if (pParameterGroup) {
            startStyling(pStart);
            setStyling(pEnd-pStart, ParameterGroup);
        }

        // Check whether the given text contains some keywords from various
        // categories

        if (pParameterGroup) {
            doStyleTextRegEx(pStart, pText, mParameterKeywordsRegEx, ParameterKeyword);
            doStyleTextRegEx(pStart, pText, mParameterValueKeywordsRegEx, ParameterCellmlKeyword);
        } else {
            doStyleTextRegEx(pStart, pText, mKeywordsRegEx, Keyword);
            doStyleTextRegEx(pStart, pText, mCellmlKeywordsRegEx, CellmlKeyword);
        }

        doStyleTextRegEx(pStart, pText, mSiUnitKeywordsRegEx, pParameterGroup?ParameterCellmlKeyword:CellmlKeyword);

        // Check whether the given text contains some numbers

        doStyleTextRegEx(pStart, pText, mNumberRegEx, pParameterGroup?ParameterNumber:Number);

        // Let QScintilla know that we are done with the styling of the given
        // text
        // Note: indeed, QScintilla uses the end position of the last bit of
        //       text that has been styled to determine the starting position of
        //       the next bit of text that needs to be styled (see
        //       QsciLexerCustom::handleStyleNeeded()). Now, depending on
        //       whether keywords and/or numbers have been styled, the end
        //       position may or not be pEnd. So, here, we make sure that it is
        //       pEnd, so that QScintilla can work optimally...

        startStyling(pEnd);
    }
}

//==============================================================================

void CellmlTextViewLexer::doStyleTextPreviousMultilineComment(const int &pPosition,
                                                              int pStart,
                                                              int pEnd,
                                                              QString pText,
                                                              bool pParameterGroup)
{
    // A /* XXX */ comment started before or at the beginning of the given text,
    // so now look for where it ends

    int multilineCommentEndPosition = findString(EndMultilineCommentString, pPosition, MultilineComment);

    if (multilineCommentEndPosition == -1)
        multilineCommentEndPosition = mFullText.length();

    // Check whether the beginning of the given text is within the /* XXX */
    // comment

    if ((pPosition <= pStart) && (pStart <= multilineCommentEndPosition)) {
        // The beginning of the given text is within a /* XXX */ comment, so
        // style it

        int realEnd = multilineCommentEndPosition+EndMultilineCommentLength;
        int end = qMin(pEnd, realEnd);

        startStyling(pStart);
        setStyling(end-pStart, MultilineComment);

        // Now, style everything that is behind the /* XXX */ comment, if
        // anything

        if (end != pEnd)
            doStyleText(end, pEnd, pText.right(pEnd-end), pParameterGroup);
    } else {
        // The beginning of the given text is not within a /* XXX */ comment, so
        // style it

        doStyleTextCurrent(pStart, pEnd, pText, pParameterGroup);
    }
}

//==============================================================================

void CellmlTextViewLexer::doStyleTextPreviousParameterGroup(const int &pPosition,
                                                            int pStart,
                                                            int pEnd,
                                                            QString pText,
                                                            bool pParameterGroup)
{
    // A parameter group started before or at the beginning of the given text,
    // so now look for where it ends

    int parameterGroupEndPosition = findString(EndParameterGroupString, pPosition, ParameterGroup);

    if (parameterGroupEndPosition == -1)
        parameterGroupEndPosition = mFullText.length();

    // Check whether the beginning of the given text is within a parameter group

    if ((pPosition <= pStart) && (pStart <= parameterGroupEndPosition)) {
        // The beginning of the given text is within a parameter group, so style
        // it as such

        int realEnd = parameterGroupEndPosition+EndParameterGroupLength;
        int end = qMin(pEnd, realEnd);
        bool hasStart = pPosition == pStart;
        bool hasEnd = end == realEnd;

        // If possible, style the start of the parameter group

        if (hasStart) {
            startStyling(pStart);
            setStyling(StartParameterGroupLength, ParameterGroup);
        }

        // Now style the contents of the parameter group itself

        int newStart = pStart+(hasStart?StartParameterGroupLength:0);
        int newEnd = end-(hasEnd?EndParameterGroupLength:0);

        doStyleTextCurrent(newStart, newEnd,
                           pText.mid(newStart-pStart, newEnd-newStart), true);

        // If possible, style the end of the parameter group, as well as what is
        // behind it

        if (hasEnd) {
            startStyling(end-EndParameterGroupLength);
            setStyling(EndParameterGroupLength, ParameterGroup);

            doStyleText(end, pEnd, pText.right(pEnd-end), pParameterGroup);
        }
    } else {
        // The beginning of the given text is not within a parameter group, so
        // style it

        doStyleTextCurrent(pStart, pEnd, pText, pParameterGroup);
    }
}

//==============================================================================

void CellmlTextViewLexer::doStyleTextString(const int &pPosition, int pStart,
                                            int pEnd, QString pText,
                                            bool pParameterGroup)
{
    // There is a string to style, so first style everything that is before it

    doStyleTextCurrent(pStart, pStart+pPosition, pText.left(pPosition),
                       pParameterGroup);

    // Now, check where the string ends, if anywhere

    int nextStart = -1;
    int stringEndPosition = pText.indexOf(StringString, pPosition+StringLength);

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

        if (eolPosition != -1)
            nextStart = pStart+eolPosition+mEolString.length();
    }

    // Style the string itself

    int start = pStart+pPosition;

    startStyling(start);
    setStyling(((nextStart == -1)?pEnd:nextStart)-start, pParameterGroup?ParameterString:String);

    // Style whatever is after the string

    if (nextStart != -1)
        doStyleText(nextStart, pEnd, pText.right(pEnd-nextStart), pParameterGroup);
}

//==============================================================================

void CellmlTextViewLexer::doStyleTextSingleLineComment(const int &pPosition,
                                                       int pStart, int pEnd,
                                                       QString pText,
                                                       bool pParameterGroup)
{
    // There is a // comment to style, so first style everything that is before
    // it

    doStyleTextCurrent(pStart, pStart+pPosition, pText.left(pPosition),
                       pParameterGroup);

    // Style the // comment itself, after having look for the end of the line,
    // if any

    int eolPosition = pText.indexOf(mEolString, pPosition+SingleLineCommentLength);
    int start = pStart+pPosition;
    int end = (eolPosition == -1)?pEnd:pStart+eolPosition+mEolString.length();

    startStyling(start);
    setStyling(end-start, SingleLineComment);

    // Now, style everything that is after the // comment, if anything

    if (eolPosition != -1)
        doStyleText(end, pEnd, pText.right(pEnd-end), pParameterGroup);
}

//==============================================================================

void CellmlTextViewLexer::doStyleTextRegEx(int pStart, const QString &pText,
                                           const QRegularExpression &pRegEx,
                                           const int &pRegExStyle)
{
    // Style the given text using the given regular expression

    QRegularExpressionMatchIterator regExMatchIter = pRegEx.globalMatch(pText);
    QRegularExpressionMatch regExMatch;

    while (regExMatchIter.hasNext()) {
        regExMatch = regExMatchIter.next();

        // We have a match, so style it

        startStyling(pStart+regExMatch.capturedStart());
        setStyling(regExMatch.capturedLength(), pRegExStyle);
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
        style = editor()->SendScintilla(QsciScintilla::SCI_GETSTYLEAT, i);

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

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
