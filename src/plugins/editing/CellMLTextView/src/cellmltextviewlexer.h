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

    const char * language() const override;

    QString description(int pStyle) const override;

    QColor color(int pStyle) const override;
    QFont font(int pStyle) const override;

    void styleText(int pBytesStart, int pBytesEnd) override;

private:
    QString mFullText;
    QByteArray mFullTextUtf8;

    QString mEolString;

    void doStyleText(int pBytesStart, int pBytesEnd, const QString &pText,
                     bool pParameterBlock);
    void doStyleTextCurrent(int pBytesStart, int pBytesEnd,
                            const QString &pText, bool pParameterBlock);
    void doStyleTextPreviousMultilineComment(int pPosition, int pBytesStart,
                                             int pBytesEnd,
                                             const QString &pText,
                                             bool pParameterBlock);
    void doStyleTextPreviousParameterBlock(int pPosition, int pBytesStart,
                                           int pBytesEnd, const QString &pText,
                                           bool pParameterBlock);
    void doStyleTextString(int pPosition, int pBytesStart, int pBytesEnd,
                           const QString &pText, bool pParameterBlock);
    void doStyleTextRegEx(int pBytesStart, const QString &pText,
                          const QRegularExpression &pRegEx,
                          int pRegExStyle);
    void doStyleTextNumberRegEx(int pBytesStart, const QString &pText,
                                int pRegExStyle);

    bool validString(int pFrom, int pTo, int pStyle) const;
    int findString(const QString &pString, int pFrom, int pStyle,
                   bool pForward = true);

    int fullTextPosition(int pBytesPosition) const;
    int fullTextLength(int pBytesStart, int pBytesEnd) const;

    int fullTextBytesPosition(int pPosition) const;
    int textBytesPosition(const QString &pText, int pPosition) const;
};

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
