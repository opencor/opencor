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

#ifndef PRETTYCELLMLVIEWLEXER_H
#define PRETTYCELLMLVIEWLEXER_H

//==============================================================================

#include <QRegularExpression>

//==============================================================================

#include "Qsci/qscilexercustom.h"

//==============================================================================

namespace OpenCOR {
namespace PrettyCellMLView {

//==============================================================================

class PrettyCellmlViewLexer : public QsciLexerCustom
{
public:
    enum {
        Default,
        Comment,
        Keyword,
        CellmlKeyword,
        Number,
        String,
        ParameterGroup,
        ParameterKeyword,
        ParameterValueKeyword,
        ParameterNumber,
        ParameterString
    };

    explicit PrettyCellmlViewLexer(QObject *pParent);

    virtual const char * language() const;

    virtual QString description(int pStyle) const;

    virtual QColor color(int pStyle) const;
    virtual QFont font(int pStyle) const;

    virtual void styleText(int pStart, int pEnd);

private:
    QString mFullText;
    QString mEolString;

    QRegularExpression mKeywordsRegEx;
    QRegularExpression mCellmlKeywordsRegEx;
    QRegularExpression mParameterKeywordsRegEx;
    QRegularExpression mParameterValueKeywordsRegEx;
    QRegularExpression mNumberRegEx;

    void doStyleText(int pStart, int pEnd, QString pText, bool pParameterGroup);
    void doStyleTextPreviousMultilineComment(const int &pPosition, int &pStart,
                                             int pEnd, QString &pText);
    void doStyleTextPreviousParameterGroup(const int &pPosition, int &pStart,
                                           int &pEnd, QString &pText,
                                           bool &pParameterGroup);
    void doStyleTextString(const int &pPosition, int pStart, int pEnd,
                           QString pText, bool pParameterGroup);
    void doStyleTextSingleLineComment(const int &pPosition, int pStart,
                                      int pEnd, QString pText,
                                      bool pParameterGroup);
    void doStyleTextRegEx(int pStart, const QString &pText,
                          const QRegularExpression &pRegEx,
                          const int &pRegExStyle);

    bool validString(const int &pFrom, const int &pTo) const;
    int findString(const QString &pString, int pFrom,
                   const bool &pForward = true);
};

//==============================================================================

}   // namespace PrettyCellMLView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
