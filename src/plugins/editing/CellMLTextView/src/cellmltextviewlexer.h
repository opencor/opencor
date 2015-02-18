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

#ifndef CELLMLTEXTVIEWLEXER_H
#define CELLMLTEXTVIEWLEXER_H

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

    QRegularExpression mKeywordsRegEx;
    QRegularExpression mCellmlKeywordsRegEx;
    QRegularExpression mSiUnitKeywordsRegEx;
    QRegularExpression mParameterKeywordsRegEx;
    QRegularExpression mParameterValueKeywordsRegEx;
    QRegularExpression mNumberRegEx;

    void doStyleText(const int &pBytesStart, const int &pBytesEnd,
                     const QString &pText, const bool &pParameterBlock);
    void doStyleTextCurrent(const int &pBytesStart, const int &pBytesEnd,
                            const QString &pText, const bool &pParameterBlock);
    void doStyleTextSingleLineComment(const int &pPosition,
                                      const int &pBytesStart,
                                      const int &pBytesEnd,
                                      const QString &pText,
                                      const bool &pParameterBlock);
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
    int fullTextLength(const int &pBytesFrom, const int &pBytesLength) const;

    int fullTextBytesPosition(const int &pPosition) const;
    int textBytesPosition(const QString &pText, const int &pPosition) const;
    int textBytesLength(const QString &pText, const int &pFrom,
                        const int &pLength) const;
};

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
