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

    mKeywords = QStringList() << "as" << "comp" << "def" << "enddef" << "model"
                              << "unit";
    mCellmlKeywords = QStringList() << "base"
                                    // Standard units
                                    << "ampere" << "becquerel" << "candela"
                                    << "celsius" << "coulomb" << "dimensionless"
                                    << "farad" << "gram" << "gray" << "henry"
                                    << "hertz" << "joule" << "katal" << "kelvin"
                                    << "kilogram" << "liter" << "litre"
                                    << "lumen" << "lux" << "meter" << "metre"
                                    << "mole" << "newton" << "ohm" << "pascal"
                                    << "radian" << "second" << "siemens"
                                    << "sievert" << "steradian" << "tesla"
                                    << "volt" << "watt" << "weber";
    mParameterKeywords = QStringList() << "pref" << "expo" << "mult" << "off";
    mParameterValueKeywords = QStringList() // Prefixes
                                            << "yotta" << "zetta" << "exa"
                                            << "peta" << "tera" << "giga"
                                            << "mega" << "kilo" << "hecto"
                                            << "deka" << "deci" << "centi"
                                            << "milli" << "micro" << "nano"
                                            << "pico" << "femto" << "atto"
                                            << "zepto" << "yocto";
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
    case ParameterGroup:
        return QObject::tr("Parameter group");
    case ParameterKeyword:
        return QObject::tr("Parameter keyword");
    case ParameterValueKeyword:
        return QObject::tr("Parameter value keyword");
    case ParameterNumber:
        return QObject::tr("Parameter number");
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
        return QColor(0x00, 0x7f, 0x00);
    case Keyword:
    case ParameterKeyword:
        return QColor(0x00, 0x00, 0x7f);
    case CellmlKeyword:
    case ParameterValueKeyword:
        return QColor(0x7f, 0x00, 0x7f);
    case Number:
    case ParameterNumber:
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
    case ParameterNumber:
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
    mEolString = qobject_cast<QScintillaSupport::QScintillaWidget *>(editor())->eolString();

    doStyleText(pStart, pEnd, text, false);
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

void PrettyCellmlViewLexer::doStyleText(int pStart, int pEnd, QString pText,
                                        bool pParameterGroup)
{
    // Make sure that we are given some text to style

    if (pText.trimmed().isEmpty())
        return;

    // Check whether a /* XXX */ comment started before or at the beginning of
    // the given text

    int commentStartPosition = mFullText.lastIndexOf(StartCommentString, pStart+StartCommentLength-1);

    if (commentStartPosition != -1) {
        // A /* XXX */ comment started before or at the beginning of the given
        // text, so now look for where it ends

        int commentEndPosition = mFullText.indexOf(EndCommentString, commentStartPosition+StartCommentLength);

        if (commentEndPosition == -1) {
            // The comment doesn't end as such, so consider that it 'ends' at
            // the of the full text

            commentEndPosition = mFullText.length();
        }

        if ((commentStartPosition <= pStart) && (pStart <= commentEndPosition)) {
            // The beginning of the given text is a comment, so style it

            int realEnd = commentEndPosition+EndCommentLength;
            int end = qMin(pEnd, realEnd);

            startStyling(pStart, 0x1f);
            setStyling(end-pStart, Comment);

            // Get ready to style everything that is behind the comment, if
            // anything

            if (end == realEnd) {
                pStart = end;
                pText = pText.right(pEnd-end);
            } else {
                return;
            }
        }
    }

    // Check whether a parameter group started before or at the beginning of the
    // given text

    int parameterGroupStartPosition = findString(StartParameterGroupString, pStart+StartParameterGroupLength-1, false);

    if (parameterGroupStartPosition != -1) {
        // A parameter group started before or at the beginning of the given
        // text, so now look for where it ends

        int parameterGroupEndPosition = findString(EndParameterGroupString, parameterGroupStartPosition+StartParameterGroupLength);

        if (parameterGroupEndPosition == -1)
            parameterGroupEndPosition = mFullText.length();

        if ((parameterGroupStartPosition <= pStart) && (pStart <= parameterGroupEndPosition)) {
            // The beginning of the given text is a parameter group, so style
            // everything that is behind the parameter group, if anything

            int realEnd = parameterGroupEndPosition+EndParameterGroupLength;
            int end = qMin(pEnd, realEnd);
            bool hasEnd = end == realEnd;

            if (hasEnd)
                doStyleText(end, pEnd, pText.right(pEnd-end), pParameterGroup);

            // If possible, style the beginning and/or the end of the parameter
            // group

            bool hasBeginning = parameterGroupStartPosition == pStart;

            if (hasBeginning) {
                startStyling(pStart, 0x1f);
                setStyling(StartParameterGroupLength, ParameterGroup);
            }

            if (hasEnd) {
                startStyling(end-EndParameterGroupLength, 0x1f);
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

    // Check whether the given text contains a // comment

    int commentPosition = pText.indexOf(CommentString);

    if (commentPosition != -1) {
        // There is a // comment to style, so first style everything that is
        // before it

        doStyleText(pStart, pStart+commentPosition, pText.left(commentPosition),
                    pParameterGroup);

        // Now, style everything that is after the // comment, if anything, by
        // looking for the end of the line on which the // comment is

        int eolPosition = pText.indexOf(mEolString, commentPosition+CommentStringLength);

        if (eolPosition != -1) {
            int start = pStart+eolPosition+mEolString.length();

            doStyleText(start, pEnd, pText.right(pEnd-start), pParameterGroup);
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
        // is before it

        doStyleText(pStart, pStart+commentStartPosition,
                    pText.left(commentStartPosition), pParameterGroup);

        // Now style everything from the comment onwards
        // Note: to style everything from the comment onwards means that we will
        //       find that a /* XXX */ comment starts at the beginning of the
        //       'new' given text...

        doStyleText(pStart+commentStartPosition, pEnd,
                    pText.right(pEnd-pStart-commentStartPosition),
                    pParameterGroup);

        return;
    }

    // Check whether the given text contains a parameter group

    parameterGroupStartPosition = pText.indexOf(StartParameterGroupString);

    if (parameterGroupStartPosition != -1) {
        // There is a parameter group, so first style everything that is before
        // it

        doStyleText(pStart, pStart+parameterGroupStartPosition,
                    pText.left(parameterGroupStartPosition), pParameterGroup);

        // Now style everything from the parameter group onwards
        // Note: to style everything from the parameter group onwards means that
        //       we will find that a parameter group starts at the beginning of
        //       the 'new' given text...

        doStyleText(pStart+parameterGroupStartPosition, pEnd,
                    pText.right(pEnd-pStart-parameterGroupStartPosition),
                    pParameterGroup);

        return;
    }

    // Use a default style for the given text

    startStyling(pStart, 0x1f);
    setStyling(pEnd-pStart, pParameterGroup?ParameterGroup:Default);

    // Check whether the given text contains keywords from various categories

    doStyleTextKeyword(pStart, pText, mKeywords, pParameterGroup?ParameterGroup:Keyword);
    doStyleTextKeyword(pStart, pText, mCellmlKeywords, pParameterGroup?ParameterGroup:CellmlKeyword);
    doStyleTextKeyword(pStart, pText, mParameterKeywords, pParameterGroup?ParameterKeyword:Default);
    doStyleTextKeyword(pStart, pText, mParameterValueKeywords, pParameterGroup?ParameterValueKeyword:Default);

    // Check whether the given text contains some numbers
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

bool PrettyCellmlViewLexer::stringWithinComment(const int &pFrom,
                                                const int &pTo) const
{
    // Return whether the given string located at the given location is within
    // a comment

    // Check whether we are within a /* XXX */ comment

    int commentStartPosition = mFullText.lastIndexOf(StartCommentString, pFrom);

    if (commentStartPosition != -1) {
        int commentEndPosition = mFullText.indexOf(EndCommentString, commentStartPosition+StartCommentLength);

        if (commentEndPosition == -1)
            commentEndPosition = mFullText.length();

        if ((commentStartPosition < pFrom) && (pTo < commentEndPosition))
            return true;
    }

    // Check whether we are within a // comment

    int commentPosition = mFullText.lastIndexOf(CommentString, pFrom);

    if (commentPosition != -1) {
        int eolPosition = mFullText.indexOf(mEolString, commentPosition+CommentStringLength);

        if (eolPosition == -1)
            eolPosition = mFullText.length();

        if ((commentPosition < pFrom) && (pTo < eolPosition))
            return true;
    }

    return false;
}

//==============================================================================

int PrettyCellmlViewLexer::findString(const QString &pString, int pFrom,
                                      const bool &pForward)
{
    // Find forward/backward the given string starting from the given position

    int stringLength = pString.length();
    int res = pForward?pFrom-stringLength:pFrom+1;

    do {
        pFrom = pForward?res+stringLength:res-1;

        res = pForward?mFullText.indexOf(pString, pFrom):mFullText.lastIndexOf(pString, pFrom);
    } while ((res != -1) && stringWithinComment(res, res+stringLength-1));

    return res;
}

//==============================================================================

}   // namespace PrettyCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
