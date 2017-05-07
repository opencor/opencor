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
// Parser for the CellML Text format
//==============================================================================

#pragma once

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

class CellmlTextViewParserMessage
{
public:
    enum Type {
        Error,
        Warning
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

class CellmlTextViewParser;

//==============================================================================

typedef QDomElement (CellmlTextViewParser::*ParseNormalMathematicalExpressionFunction)(QDomNode &pDomNode);

//==============================================================================

class CellmlTextViewParser
{
public:
    enum StatementType {
        Unknown,
        Normal,
        PiecewiseSel,
        PiecewiseCase,
        PiecewiseOtherwise,
        PiecewiseEndSel
    };

    explicit CellmlTextViewParser();

    bool execute(const QString &pCellmlText,
                 const CellMLSupport::CellmlFile::Version &pCellmlVersion);
    bool execute(const QString &pCellmlText, const bool &pFullParsing = true);

    CellMLSupport::CellmlFile::Version cellmlVersion() const;

    QDomDocument domDocument() const;

    CellmlTextViewParserMessages messages() const;

    StatementType statementType() const;

private:
    CellmlTextViewScanner mScanner;

    CellMLSupport::CellmlFile::Version mCellmlVersion;

    QDomDocument mDomDocument;

    CellmlTextViewParserMessages mMessages;

    QMap<QString, QString> mNamespaces;

    StatementType mStatementType;

    void initialize(const QString &pCellmlText);

    void addUnexpectedTokenErrorMessage(const QString &pExpectedString,
                                        const QString &pFoundString);

    QDomElement newDomElement(QDomNode &pDomNode, const QString &pElementName);

    QDomElement newIdentifierElement(const QString &pValue);
    QDomElement newDerivativeElement(const QString &pF, const QString &pX);
    QDomElement newDerivativeElement(const QString &pF, const QString &pX,
                                     const QString &pOrder);
    QDomElement newNumberElement(const QString &pNumber, const QString &pUnit);
    QDomElement newMathematicalConstantElement(const CellmlTextViewScanner::TokenType &pTokenType);
    QDomElement newMathematicalFunctionElement(const CellmlTextViewScanner::TokenType &pTokenType,
                                               const QList<QDomElement> &pArgumentElements);

    CellmlTextViewScanner::TokenTypes rangeOfTokenTypes(const CellmlTextViewScanner::TokenType &pFromTokenType,
                                                        const CellmlTextViewScanner::TokenType &pToTokenType);

    bool tokenType(QDomNode &pDomNode, const QString &pExpectedString,
                   const CellmlTextViewScanner::TokenTypes &pTokenTypes);
    bool tokenType(QDomNode &pDomNode, const QString &pExpectedString,
                   const CellmlTextViewScanner::TokenType &pTokenType);
    bool isTokenType(QDomNode &pDomNode,
                     const CellmlTextViewScanner::TokenType &pTokenType);

    bool andToken(QDomNode &pDomNode);
    bool asToken(QDomNode &pDomNode);
    bool betweenToken(QDomNode &pDomNode);
    bool caseToken(QDomNode &pDomNode);
    bool closingBracketToken(QDomNode &pDomNode);
    bool closingCurlyBracketToken(QDomNode &pDomNode);
    bool colonToken(QDomNode &pDomNode);
    bool commaToken(QDomNode &pDomNode);
    bool compToken(QDomNode &pDomNode);
    bool defToken(QDomNode &pDomNode);
    bool dimensionlessToken(QDomNode &pDomNode);
    bool endcompToken(QDomNode &pDomNode);
    bool enddefToken(QDomNode &pDomNode);
    bool endselToken(QDomNode &pDomNode);
    bool eqToken(QDomNode &pDomNode);
    bool forToken(QDomNode &pDomNode);
    bool identifierOrSiUnitToken(QDomNode &pDomNode);
    bool identifierToken(QDomNode &pDomNode);
    bool cmetaIdToken(QDomNode &pDomNode);
    bool modelToken(QDomNode &pDomNode);
    bool numberToken(QDomNode &pDomNode);
    bool numberValueToken(QDomNode &pDomNode, int &pSign);
    bool openingBracketToken(QDomNode &pDomNode);
    bool openingCurlyBracketToken(QDomNode &pDomNode);
    bool semiColonToken(QDomNode &pDomNode);
    bool strictlyPositiveIntegerNumberToken(QDomNode &pDomNode);
    bool unitToken(QDomNode &pDomNode);
    bool usingToken(QDomNode &pDomNode);

    QString processCommentString(const QString &pComment);
    void parseComments(QDomNode &pDomNode);
    bool parseCmetaId(QDomElement &pDomElement);
    bool parseModelDefinition(QDomNode &pDomNode);
    bool parseImportDefinition(QDomNode &pDomNode);
    bool parseUnitsDefinition(QDomNode &pDomNode);
    bool parseUnitDefinition(QDomNode &pDomNode);
    bool parseComponentDefinition(QDomNode &pDomNode);
    bool parseVariableDeclaration(QDomNode &pDomNode);
    bool parseMathematicalExpression(QDomNode &pDomNode,
                                     const bool &pFullParsing = true);
    bool parseGroupDefinition(QDomNode &pDomNode);
    bool parseComponentRefDefinition(QDomNode &pDomNode);
    bool parseMapDefinition(QDomNode &pDomNode);

    QString mathmlName(const CellmlTextViewScanner::TokenType &pTokenType) const;

    QDomElement parseDerivativeIdentifier(QDomNode &pDomNode);
    QDomElement parseNumber(QDomNode &pDomNode);
    QDomElement parseMathematicalFunction(QDomNode &pDomNode,
                                          const bool &pOneArgument,
                                          const bool &pTwoArguments,
                                          const bool &pMoreArguments);
    QDomElement parseParenthesizedMathematicalExpression(QDomNode &pDomNode);

    QDomElement parseMathematicalExpressionElement(QDomNode &pDomNode,
                                                   const CellmlTextViewScanner::TokenTypes &pTokenTypes,
                                                   ParseNormalMathematicalExpressionFunction pFunction);
    QDomElement parseNormalMathematicalExpression(QDomNode &pDomNode);
    QDomElement parseNormalMathematicalExpression2(QDomNode &pDomNode);
    QDomElement parseNormalMathematicalExpression3(QDomNode &pDomNode);
    QDomElement parseNormalMathematicalExpression4(QDomNode &pDomNode);
    QDomElement parseNormalMathematicalExpression5(QDomNode &pDomNode);
    QDomElement parseNormalMathematicalExpression6(QDomNode &pDomNode);
    QDomElement parseNormalMathematicalExpression7(QDomNode &pDomNode);
    QDomElement parseNormalMathematicalExpression8(QDomNode &pDomNode);
    QDomElement parseNormalMathematicalExpression9(QDomNode &pDomNode);
    QDomElement parsePiecewiseMathematicalExpression(QDomNode &pDomNode);

    void moveTrailingComments(QDomNode &pFromDomNode, QDomNode &pToDomNode);
};

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
