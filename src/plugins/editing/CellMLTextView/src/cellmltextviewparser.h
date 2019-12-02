/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

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
    enum class Type {
        Error,
        Warning
    };

    explicit CellmlTextViewParserMessage(Type pType, int pLine, int pColumn,
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

using CellmlTextViewParserMessages = QList<CellmlTextViewParserMessage>;

//==============================================================================

class CellmlTextViewParser;

//==============================================================================

using ParseNormalMathematicalExpressionFunction = QDomElement (CellmlTextViewParser::*)(QDomNode &pDomNode);

//==============================================================================

class CellmlTextViewParser : public QObject
{
    Q_OBJECT

public:
    enum class Statement {
        Unknown,
        Normal,
        PiecewiseSel,
        PiecewiseCase,
        PiecewiseOtherwise,
        PiecewiseEndSel
    };

    bool execute(const QString &pCellmlText,
                 CellMLSupport::CellmlFile::Version pCellmlVersion);
    bool execute(const QString &pCellmlText, bool pFullParsing = true);

    CellMLSupport::CellmlFile::Version cellmlVersion() const;

    QDomDocument domDocument() const;
    QDomElement modelElement() const;

    CellmlTextViewParserMessages messages() const;

    Statement statement() const;

private:
    CellmlTextViewScanner mScanner;

    CellMLSupport::CellmlFile::Version mCellmlVersion = CellMLSupport::CellmlFile::Version::Cellml_1_0;

    QDomDocument mDomDocument;
    QDomElement mModelElement;

    CellmlTextViewParserMessages mMessages;

    QMap<QString, QString> mNamespaces;

    Statement mStatement = Statement::Unknown;

    void initialize(const QString &pCellmlText);

    void addUnexpectedTokenErrorMessage(const QString &pExpectedString,
                                        const QString &pFoundString);

    QDomElement newDomElement(QDomNode &pDomNode, const QString &pElementName);

    QDomElement newIdentifierElement(const QString &pValue);
    QDomElement newDerivativeElement(const QString &pF, const QString &pX);
    QDomElement newDerivativeElement(const QString &pF, const QString &pX,
                                     const QString &pOrder);
    QDomElement newNumberElement(const QString &pNumber, const QString &pUnit);
    QDomElement newMathematicalConstantElement(CellmlTextViewScanner::Token pTokenType);
    QDomElement newMathematicalFunctionElement(CellmlTextViewScanner::Token pTokenType,
                                               const QList<QDomElement> &pArgumentElements);

    CellmlTextViewScanner::Tokens rangeOfTokens(CellmlTextViewScanner::Token pFromTokenType,
                                                CellmlTextViewScanner::Token pToTokenType);

    bool tokenType(QDomNode &pDomNode, const QString &pExpectedString,
                   const CellmlTextViewScanner::Tokens &pTokens);
    bool tokenType(QDomNode &pDomNode, const QString &pExpectedString,
                   CellmlTextViewScanner::Token pTokenType);
    bool isTokenType(QDomNode &pDomNode,
                     CellmlTextViewScanner::Token pTokenType);

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
                                     bool pFullParsing = true);
    bool parseGroupDefinition(QDomNode &pDomNode);
    bool parseComponentRefDefinition(QDomNode &pDomNode);
    bool parseMapDefinition(QDomNode &pDomNode);

    QString mathmlName(CellmlTextViewScanner::Token pTokenType) const;

    QDomElement parseDerivativeIdentifier(QDomNode &pDomNode);
    QDomElement parseNumber(QDomNode &pDomNode);
    QDomElement parseMathematicalFunction(QDomNode &pDomNode, bool pOneArgument,
                                          bool pTwoArguments,
                                          bool pMoreArguments);
    QDomElement parseParenthesizedMathematicalExpression(QDomNode &pDomNode);

    QDomElement parseMathematicalExpressionElement(QDomNode &pDomNode,
                                                   const CellmlTextViewScanner::Tokens &pTokens,
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

} // namespace CellMLTextView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
