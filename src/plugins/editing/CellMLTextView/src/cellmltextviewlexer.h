/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Lexer for the CellML Text format
//==============================================================================

#pragma once

//==============================================================================

#include <QRegularExpression>

//==============================================================================

#include "Qsci/qscilexercustom.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

class CellmlTextViewLexer : public QsciLexerCustom
{
public:
    enum {
        Default,
        SingleLineComment,
        MultilineComment,
        Keyword,
        CellmlKeyword,
        Number,
        String,
        ParameterBlock,
        ParameterKeyword,
        ParameterCellmlKeyword,
        ParameterNumber,
        ParameterString
    };

    explicit CellmlTextViewLexer(QObject *pParent);

    virtual const char * language() const;

    virtual QString description(int pStyle) const;

    virtual QColor color(int pStyle) const;
    virtual QFont font(int pStyle) const;

    virtual void styleText(int pBytesStart, int pBytesEnd);

private:
    QString mFullText;
    QByteArray mFullTextUtf8;

    QString mEolString;

    void doStyleText(const int &pBytesStart, const int &pBytesEnd,
                     const QString &pText, const bool &pParameterBlock);
    void doStyleTextCurrent(const int &pBytesStart, const int &pBytesEnd,
                            const QString &pText, const bool &pParameterBlock);
    void doStyleTextPreviousMultilineComment(const int &pPosition,
                                             const int &pBytesStart,
                                             const int &pBytesEnd,
                                             const QString &pText,
                                             const bool &pParameterBlock);
    void doStyleTextPreviousParameterBlock(const int &pPosition,
                                           const int &pBytesStart,
                                           const int &pBytesEnd,
                                           const QString &pText,
                                           const bool &pParameterBlock);
    void doStyleTextString(const int &pPosition, const int &pBytesStart,
                           const int &pBytesEnd, const QString &pText,
                           const bool &pParameterBlock);
    void doStyleTextRegEx(const int &pBytesStart, const QString &pText,
                          const QRegularExpression &pRegEx,
                          const int &pRegExStyle);
    void doStyleTextNumberRegEx(const int &pBytesStart, const QString &pText,
                                const int &pRegExStyle);

    bool validString(const int &pFrom, const int &pTo, const int &pStyle) const;
    int findString(const QString &pString, int pFrom, const int &pStyle,
                   const bool &pForward = true);

    int fullTextPosition(const int &pBytesPosition) const;
    int fullTextLength(const int &pBytesStart, const int &pBytesEnd) const;

    int fullTextBytesPosition(const int &pPosition) const;
    int textBytesPosition(const QString &pText, const int &pPosition) const;
};

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
