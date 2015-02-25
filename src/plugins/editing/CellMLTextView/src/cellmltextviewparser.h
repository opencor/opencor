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
    enum Type {
        Error,
        Warning,
        Hint,
        Information
    };

    explicit CellmlTextViewParserMessage(const Type &pType,
                                         const int &pLine, const int &pColumn,
                                         const QString &pMessage);

    Type type() const;

    int line() const;
    int column() const;

    QString message() const;

private:
    Type mType;

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

    bool execute(const QString &pText,
                 const CellMLSupport::CellmlFile::Version &pCellmlVersion = CellMLSupport::CellmlFile::Cellml_1_0);

    CellMLSupport::CellmlFile::Version cellmlVersion() const;

    QDomDocument domDocument() const;

    CellmlTextViewParserMessages messages() const;

private:
    CellmlTextViewScanner *mScanner;

    CellMLSupport::CellmlFile::Version mCellmlVersion;

    QDomDocument mDomDocument;

    CellmlTextViewParserMessages mMessages;

    QMap<QString, QString> mNamespaces;

    QDomElement newDomElement(QDomNode &pDomNode, const QString &pElementName);

    bool tokenType(QDomNode &pDomNode, const QString &pExpectedString,
                   const CellmlTextViewScanner::TokenType &pTokenType);
    bool isTokenType(QDomNode &pDomNode,
                     const CellmlTextViewScanner::TokenType &pTokenType);

    bool asToken(QDomNode &pDomNode);
    bool closingCurlyBracketToken(QDomNode &pDomNode);
    bool defToken(QDomNode &pDomNode);
    bool enddefPlusSemiColonToken(QDomNode &pDomNode);
    bool identifierToken(QDomNode &pDomNode);
    bool modelToken(QDomNode &pDomNode);
    bool openingCurlyBracketToken(QDomNode &pDomNode);
    bool semiColonToken(QDomNode &pDomNode);
    bool unitToken(QDomNode &pDomNode);

    void parseComments(QDomNode &pDomNode);
    void parseCmetaId(QDomElement &pDomElement);
    bool parseModelDefinition(QDomNode &pDomNode);
    QDomElement parseUnitsDefinition(QDomNode &pDomNode,
                                     bool &pBaseUnitsDefinition);
};

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
