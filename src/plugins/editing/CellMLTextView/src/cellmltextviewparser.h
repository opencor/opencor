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
// Parser for the CellML text format
//==============================================================================

#ifndef CELLMLTEXTVIEWPARSER_H
#define CELLMLTEXTVIEWPARSER_H

//==============================================================================

#include "cellmlfile.h"
#include "cellmltextviewscanner.h"

//==============================================================================

#include <QDomDocument>
#include <QList>
#include <QString>

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

class CellmlTextViewScanner;

//==============================================================================

class CellmlTextViewParserMessage
{
public:
    explicit CellmlTextViewParserMessage(const bool &pError,
                                         const int &pLine, const int &pColumn,
                                         const QString &pMessage);

    bool isError() const;

    int line() const;
    int column() const;

    QString message() const;

private:
    bool mError;

    int mLine;
    int mColumn;

    QString mMessage;
};

//==============================================================================

typedef QList<CellmlTextViewParserMessage> CellmlTextViewParserMessages;

//==============================================================================

class CellmlTextViewParser
{
public:
    explicit CellmlTextViewParser();
    ~CellmlTextViewParser();

    bool execute(const QString &pText);

    QDomDocument domDocument() const;

    CellmlTextViewParserMessages messages() const;

private:
    CellmlTextViewScanner *mScanner;

    CellMLSupport::CellmlFile::Version mCellmlVersion;

    QDomDocument mDomDocument;
    QDomNode mDomNode;
    QDomElement mDomElement;

    CellmlTextViewParserMessages mMessages;

    QDomElement newDomElement(const QString &pElementName);

    bool tokenType(const QString &pExpectedString,
                   const CellmlTextViewScanner::TokenType &pTokenType);

    bool asToken();
    bool closingCurlyBracketToken();
    bool defToken();
    bool enddefPlusSemiColonToken();
    bool identifierToken();
    bool modelToken();
    bool openingCurlyBracketToken();
    bool semiColonToken();

    void parseComments();
    QString parseCmetaId();
};

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
