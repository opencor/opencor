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

#include "cellmltextviewparser.h"
#include "corecliutils.h"
#include "qscintillawidget.h"

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

CellmlTextViewParserMessage::CellmlTextViewParserMessage(Type pType, int pLine,
                                                         int pColumn,
                                                         const QString &pMessage) :
    mType(pType),
    mLine(pLine),
    mColumn(pColumn),
    mMessage(pMessage)
{
}

//==============================================================================

CellmlTextViewParserMessage::Type CellmlTextViewParserMessage::type() const
{
    // Return our type

    return mType;
}

//==============================================================================

int CellmlTextViewParserMessage::line() const
{
    // Return our line number

    return mLine;
}

//==============================================================================

int CellmlTextViewParserMessage::column() const
{
    // Return our column number

    return mColumn;
}

//==============================================================================

QString CellmlTextViewParserMessage::message() const
{
    // Return our message

    return mMessage;
}

//==============================================================================

CellmlTextViewParser::CellmlTextViewParser() :
    mCellmlVersion(CellMLSupport::CellmlFile::Version::Cellml_1_0),
    mDomDocument(QDomDocument()),
    mModelElement(QDomElement()),
    mMessages(CellmlTextViewParserMessages()),
    mNamespaces(QMap<QString, QString>())
{
}

//==============================================================================

bool CellmlTextViewParser::execute(const QString &pCellmlText,
                                   CellMLSupport::CellmlFile::Version pCellmlVersion)
{
    // Get ready for the parsing of a model definition

    initialize(pCellmlText);

    // Expect "def"

    if (!defToken(mDomDocument))
        return false;

    // Expect "model"

    mScanner.getNextToken();

    if (!modelToken(mDomDocument))
        return false;

    // Create our model element

    mModelElement = newDomElement(mDomDocument, "model");

    // Try to parse for a cmeta:id

    mScanner.getNextToken();

    if (!parseCmetaId(mModelElement))
        return false;

    // Expect an identifier

    if (!identifierToken(mDomDocument))
        return false;

    // Set our model name

    mModelElement.setAttribute("name", mScanner.string());

    // Expect "as"

    mScanner.getNextToken();

    if (!asToken(mDomDocument))
        return false;

    // Try to parse for some model definition itself

    mScanner.getNextToken();

    if (!parseModelDefinition(mModelElement))
        return false;

    // Expect "enddef"

    if (!enddefToken(mModelElement))
        return false;

    // Expect ";"

    mScanner.getNextToken();

    if (!semiColonToken(mModelElement))
        return false;

    // Expect the end of the file

    mScanner.getNextToken();

    if (!tokenType(mModelElement, tr("the end of the file"),
                   CellmlTextViewScanner::Token::Eof)) {
        return false;
    }

    // We are done, so add some processing instruction to our DOM document and
    // this before any other DOM node (including comments)

    mDomDocument.insertBefore(mDomDocument.createProcessingInstruction("xml", "version='1.0'"),
                              mDomDocument.firstChild());

    // Use the given CellML version if it is higher than the one we actually
    // need

    mCellmlVersion = (pCellmlVersion > mCellmlVersion)?pCellmlVersion:mCellmlVersion;

    // Next, add the CellML namespace to our document element

    if (mCellmlVersion == CellMLSupport::CellmlFile::Version::Cellml_1_1) {
        mDomDocument.documentElement().setAttribute("xmlns", CellMLSupport::Cellml_1_1_Namespace);
        mDomDocument.documentElement().setAttribute("xmlns:cellml", CellMLSupport::Cellml_1_1_Namespace);
    } else {
        mDomDocument.documentElement().setAttribute("xmlns", CellMLSupport::Cellml_1_0_Namespace);
        mDomDocument.documentElement().setAttribute("xmlns:cellml", CellMLSupport::Cellml_1_0_Namespace);
    }

    // Finally, add the other namespaces that we need
    // Note: ideally, we wouldn't have to do this, but this would mean using the
    //       NS version of various methods (e.g. setAttributeNS() rather than
    //       setAttribute()). However, this results in namespace information
    //       being referenced all over the place, which is really not what we
    //       want since that unnecessarily pollutes things...

    for (const auto &key : mNamespaces.keys())
        mDomDocument.documentElement().setAttribute(QString("xmlns:%1").arg(key), mNamespaces.value(key));

    return true;
}

//==============================================================================

bool CellmlTextViewParser::execute(const QString &pCellmlText,
                                   bool pFullParsing)
{
    // Get ready for the parsing of a mathematical expression

    initialize(pCellmlText);

    // Either fully parse or partially parse a mathematical expression

    if (pFullParsing) {
        // Parse a mathematical expression

        static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::IdentifierOrCmetaId
                                                                                            << CellmlTextViewScanner::Token::Ode;

        if (tokenType(mDomDocument, tr("An identifier or '%1'").arg("ode"),
                      Tokens)) {
            if (!parseMathematicalExpression(mDomDocument))
                return false;
        } else {
            return false;
        }

        // Expect the end of the mathematical expression

        mScanner.getNextToken();

        return mScanner.token() == CellmlTextViewScanner::Token::Eof;
    } else {
        // Partially parse a mathematical expression

        static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::IdentifierOrCmetaId
                                                                                            << CellmlTextViewScanner::Token::Ode
                                                                                            << CellmlTextViewScanner::Token::Case
                                                                                            << CellmlTextViewScanner::Token::Otherwise
                                                                                            << CellmlTextViewScanner::Token::EndSel;

        if (tokenType(mDomDocument, tr("An identifier, '%1', '%2', '%3' or '%4'").arg("ode")
                                                                                 .arg("case")
                                                                                 .arg("otherwise")
                                                                                 .arg("endsel"),
                      Tokens)) {
            if (mScanner.token() == CellmlTextViewScanner::Token::Case) {
                mStatement = Statement::PiecewiseCase;

                return true;
            } else if (mScanner.token() == CellmlTextViewScanner::Token::Otherwise) {
                mStatement = Statement::PiecewiseOtherwise;

                return true;
            } if (mScanner.token() == CellmlTextViewScanner::Token::EndSel) {
                mStatement = Statement::PiecewiseEndSel;

                return true;
            }

            return parseMathematicalExpression(mDomDocument, pFullParsing);
        }

        return false;
    }
}

//==============================================================================

CellMLSupport::CellmlFile::Version CellmlTextViewParser::cellmlVersion() const
{
    // Return our CellML version

    return mCellmlVersion;
}

//==============================================================================

QDomDocument CellmlTextViewParser::domDocument() const
{
    // Return our DOM document

    return mDomDocument;
}

//==============================================================================

QDomElement CellmlTextViewParser::modelElement() const
{
    // Return our model element

    return mModelElement;
}

//==============================================================================

CellmlTextViewParserMessages CellmlTextViewParser::messages() const
{
    // Return our messages

    return mMessages;
}

//==============================================================================

CellmlTextViewParser::Statement CellmlTextViewParser::statement() const
{
    // Return our statement type

    return mStatement;
}

//==============================================================================

void CellmlTextViewParser::initialize(const QString &pCellmlText)
{
    // Initialize ourselves with the given CellML Text string

    mScanner.setText(pCellmlText);

    mCellmlVersion = CellMLSupport::CellmlFile::Version::Cellml_1_0;

    mDomDocument = QDomDocument(QString());

    mMessages = CellmlTextViewParserMessages();

    mNamespaces.clear();

    mStatement = Statement::Unknown;
}

//==============================================================================

void CellmlTextViewParser::addUnexpectedTokenErrorMessage(const QString &pExpectedString,
                                                          const QString &pFoundString)
{
    // Add an error message for the given unexpected token

    mMessages << CellmlTextViewParserMessage(CellmlTextViewParserMessage::Type::Error,
                                             mScanner.line(),
                                             mScanner.column(),
                                             tr("%1 is expected, but %2 was found instead.").arg(pExpectedString)
                                                                                            .arg(pFoundString));
}

//==============================================================================

QDomElement CellmlTextViewParser::newDomElement(QDomNode &pDomNode,
                                                const QString &pElementName)
{
    // Create a new DOM element with the given name and append it to the given
    // DOM node before returning it

    QDomElement domElement = mDomDocument.createElement(pElementName);

    pDomNode.appendChild(domElement);

    return domElement;
}

//==============================================================================

QDomElement CellmlTextViewParser::newIdentifierElement(const QString &pValue)
{
    // Create and return a new identifier element with the given value

    QDomElement identifierElement = mDomDocument.createElement("ci");

    identifierElement.appendChild(mDomDocument.createTextNode(pValue));

    return identifierElement;
}

//==============================================================================

QDomElement CellmlTextViewParser::newDerivativeElement(const QString &pF,
                                                       const QString &pX)
{
    // Create and return a new derivative element with the given parameters

    QDomElement derivativeElement = mDomDocument.createElement("apply");
    QDomElement bvarElement = mDomDocument.createElement("bvar");

    derivativeElement.appendChild(mDomDocument.createElement("diff"));
    derivativeElement.appendChild(bvarElement);
    derivativeElement.appendChild(newIdentifierElement(pF));

    bvarElement.appendChild(newIdentifierElement(pX));

    return derivativeElement;
}

//==============================================================================

QDomElement CellmlTextViewParser::newDerivativeElement(const QString &pF,
                                                       const QString &pX,
                                                       const QString &pOrder)
{
    // Create and return a new derivative element with the given parameters

    QDomElement derivativeElement = mDomDocument.createElement("apply");
    QDomElement bvarElement = mDomDocument.createElement("bvar");
    QDomElement degreeElement = mDomDocument.createElement("degree");
    QDomElement cnElement = mDomDocument.createElement("cn");

    derivativeElement.appendChild(mDomDocument.createElement("diff"));
    derivativeElement.appendChild(bvarElement);
    derivativeElement.appendChild(newIdentifierElement(pF));

    bvarElement.appendChild(newIdentifierElement(pX));
    bvarElement.appendChild(degreeElement);

    degreeElement.appendChild(cnElement);

    cnElement.appendChild(mDomDocument.createTextNode(pOrder));
    cnElement.setAttribute("cellml:units", "dimensionless");

    return derivativeElement;
}

//==============================================================================

QDomElement CellmlTextViewParser::newNumberElement(const QString &pNumber,
                                                   const QString &pUnit)
{
    // Create and return a new number element with the given value

    QDomElement numberElement = mDomDocument.createElement("cn");
    int ePos = pNumber.toUpper().indexOf('E');

    if (ePos != -1) {
        numberElement.setAttribute("type", "e-notation");

        numberElement.appendChild(mDomDocument.createTextNode(pNumber.left(ePos)));
        numberElement.appendChild(mDomDocument.createElement("sep"));
        numberElement.appendChild(mDomDocument.createTextNode(pNumber.right(pNumber.length()-ePos-1)));
    } else {
        numberElement.appendChild(mDomDocument.createTextNode(pNumber));
    }

    numberElement.setAttribute("cellml:units", pUnit);

    return numberElement;
}

//==============================================================================

QDomElement CellmlTextViewParser::newMathematicalConstantElement(CellmlTextViewScanner::Token pTokenType)
{
    // Create and return a new mathematical constant element for the given token
    // typewith the given value

    return mDomDocument.createElement(mathmlName(pTokenType));
}

//==============================================================================

QDomElement CellmlTextViewParser::newMathematicalFunctionElement(CellmlTextViewScanner::Token pTokenType,
                                                                 const QList<QDomElement> &pArgumentElements)
{
    // Create and return a new mathematical function element for the given token
    // and arguments

    QDomElement mathematicalFunctionElement = mDomDocument.createElement("apply");

    mathematicalFunctionElement.appendChild(mDomDocument.createElement(mathmlName(pTokenType)));

    if (pArgumentElements.count() == 2) {
        if (pTokenType == CellmlTextViewScanner::Token::Log) {
            QDomElement logBaseElement = mDomDocument.createElement("logbase");

            logBaseElement.appendChild(pArgumentElements[1]);
            mathematicalFunctionElement.appendChild(logBaseElement);
        } else if (pTokenType == CellmlTextViewScanner::Token::Root) {
            QDomElement degreeElement = mDomDocument.createElement("degree");

            degreeElement.appendChild(pArgumentElements[1]);
            mathematicalFunctionElement.appendChild(degreeElement);
        }
    }

    mathematicalFunctionElement.appendChild(pArgumentElements[0]);

    if (pArgumentElements.count() == 1) {
        if (pTokenType == CellmlTextViewScanner::Token::Sqr)
            mathematicalFunctionElement.appendChild(newNumberElement("2", "dimensionless"));
    } else if (   (pTokenType >= CellmlTextViewScanner::Token::FirstTwoOrMoreArgumentMathematicalFunction)
               && (pTokenType <= CellmlTextViewScanner::Token::LastTwoOrMoreArgumentMathematicalFunction)) {
        for (int i = 1, iMax = pArgumentElements.count(); i < iMax; ++i)
            mathematicalFunctionElement.appendChild(pArgumentElements[i]);
    } else if (   (pTokenType != CellmlTextViewScanner::Token::Log)
               && (pTokenType != CellmlTextViewScanner::Token::Root)) {
        mathematicalFunctionElement.appendChild(pArgumentElements[1]);
    }

    return mathematicalFunctionElement;
}

//==============================================================================

CellmlTextViewScanner::Tokens CellmlTextViewParser::rangeOfTokens(CellmlTextViewScanner::Token pFromTokenType,
                                                                  CellmlTextViewScanner::Token pToTokenType)
{
    // Return a range of token types

    CellmlTextViewScanner::Tokens tokens = CellmlTextViewScanner::Tokens();

    for (CellmlTextViewScanner::Token tokenType = pFromTokenType;
         tokenType <= pToTokenType;
         tokenType = CellmlTextViewScanner::Token(int(tokenType)+1)) {
        tokens << tokenType;
    }

    return tokens;
}

//==============================================================================

bool CellmlTextViewParser::tokenType(QDomNode &pDomNode,
                                     const QString &pExpectedString,
                                     const CellmlTextViewScanner::Tokens &pTokens)
{
    // Try to parse comments, if any

    parseComments(pDomNode);

    // Check whether the current token type is the one we are after

    if (pTokens.contains(mScanner.token())) {
        // We have the correct token, so check whether a comment exists and, if
        // so, generate a warning for it

        if (!mScanner.comment().isEmpty()) {
            mMessages << CellmlTextViewParserMessage(CellmlTextViewParserMessage::Type::Warning,
                                                     mScanner.line(),
                                                     mScanner.column(),
                                                     mScanner.comment());
        }

        return true;
    } else if (mScanner.token() == CellmlTextViewScanner::Token::Invalid) {
        // This is the token we were expecting, but it is invalid

        mMessages << CellmlTextViewParserMessage(CellmlTextViewParserMessage::Type::Error,
                                                 mScanner.line(),
                                                 mScanner.column(),
                                                 mScanner.comment());

        return false;
    } else {
        // This is not the token we were expecting, so let the user know about
        // it

        QString foundString = mScanner.string();

        if (mScanner.token() != CellmlTextViewScanner::Token::Eof)
            foundString = QString("'%1'").arg(QScintillaSupport::QScintillaWidget::specials(foundString));

        addUnexpectedTokenErrorMessage(pExpectedString, foundString);

        return false;
    }
}

//==============================================================================

bool CellmlTextViewParser::tokenType(QDomNode &pDomNode,
                                     const QString &pExpectedString,
                                     CellmlTextViewScanner::Token pTokenType)
{
    // Expect the given token

    return tokenType(pDomNode, pExpectedString,
                     CellmlTextViewScanner::Tokens() << pTokenType);
}

//==============================================================================

bool CellmlTextViewParser::isTokenType(QDomNode &pDomNode,
                                       CellmlTextViewScanner::Token pTokenType)
{
    // Try to parse comments, if any

    parseComments(pDomNode);

    // Return whether the current token type is the one we are after

    return mScanner.token() == pTokenType;
}

//==============================================================================

bool CellmlTextViewParser::andToken(QDomNode &pDomNode)
{
    // Expect "and"

    return tokenType(pDomNode, "'and'",
                     CellmlTextViewScanner::Token::And);
}

//==============================================================================

bool CellmlTextViewParser::asToken(QDomNode &pDomNode)
{
    // Expect "as"

    return tokenType(pDomNode, "'as'",
                     CellmlTextViewScanner::Token::As);
}

//==============================================================================

bool CellmlTextViewParser::betweenToken(QDomNode &pDomNode)
{
    // Expect "between"

    return tokenType(pDomNode, "'between'",
                     CellmlTextViewScanner::Token::Between);
}

//==============================================================================

bool CellmlTextViewParser::caseToken(QDomNode &pDomNode)
{
    // Expect "case"

    return tokenType(pDomNode, "'case'",
                     CellmlTextViewScanner::Token::Case);
}

//==============================================================================

bool CellmlTextViewParser::closingBracketToken(QDomNode &pDomNode)
{
    // Expect ")"

    return tokenType(pDomNode, "')'",
                     CellmlTextViewScanner::Token::ClosingBracket);
}

//==============================================================================

bool CellmlTextViewParser::closingCurlyBracketToken(QDomNode &pDomNode)
{
    // Expect "}"

    return tokenType(pDomNode, "'}'",
                     CellmlTextViewScanner::Token::ClosingCurlyBracket);
}

//==============================================================================

bool CellmlTextViewParser::colonToken(QDomNode &pDomNode)
{
    // Expect ":"

    return tokenType(pDomNode, "':'",
                     CellmlTextViewScanner::Token::Colon);
}

//==============================================================================

bool CellmlTextViewParser::commaToken(QDomNode &pDomNode)
{
    // Expect ","

    return tokenType(pDomNode, "','",
                     CellmlTextViewScanner::Token::Comma);
}

//==============================================================================

bool CellmlTextViewParser::compToken(QDomNode &pDomNode)
{
    // Expect "comp"

    return tokenType(pDomNode, "'comp'",
                     CellmlTextViewScanner::Token::Comp);
}

//==============================================================================

bool CellmlTextViewParser::defToken(QDomNode &pDomNode)
{
    // Expect "def"

    return tokenType(pDomNode, "'def'",
                     CellmlTextViewScanner::Token::Def);
}

//==============================================================================

bool CellmlTextViewParser::dimensionlessToken(QDomNode &pDomNode)
{
    // Expect "dimensionless"

    return tokenType(pDomNode, "'dimensionless'",
                     CellmlTextViewScanner::Token::Dimensionless);
}

//==============================================================================

bool CellmlTextViewParser::endcompToken(QDomNode &pDomNode)
{
    // Expect "endcomp"

    return tokenType(pDomNode, "'endcomp'",
                     CellmlTextViewScanner::Token::EndComp);
}

//==============================================================================

bool CellmlTextViewParser::enddefToken(QDomNode &pDomNode)
{
    // Expect "enddef"

    return tokenType(pDomNode, "'enddef'",
                     CellmlTextViewScanner::Token::EndDef);
}

//==============================================================================

bool CellmlTextViewParser::endselToken(QDomNode &pDomNode)
{
    // Expect "endsel"

    return tokenType(pDomNode, "'endsel'",
                     CellmlTextViewScanner::Token::EndSel);
}

//==============================================================================

bool CellmlTextViewParser::eqToken(QDomNode &pDomNode)
{
    // Expect "="

    return tokenType(pDomNode, "'='",
                     CellmlTextViewScanner::Token::Eq);
}

//==============================================================================

bool CellmlTextViewParser::forToken(QDomNode &pDomNode)
{
    // Expect "for"

    return tokenType(pDomNode, "'for'",
                     CellmlTextViewScanner::Token::For);
}

//==============================================================================

bool CellmlTextViewParser::identifierOrSiUnitToken(QDomNode &pDomNode)
{
    // Expect an identifier or an SI unit

    static CellmlTextViewScanner::Tokens tokens = CellmlTextViewScanner::Tokens();
    static bool needInitializeTokens = true;

    if (needInitializeTokens) {
        tokens = rangeOfTokens(CellmlTextViewScanner::Token::FirstUnit,
                               CellmlTextViewScanner::Token::LastUnit) << CellmlTextViewScanner::Token::IdentifierOrCmetaId;

        needInitializeTokens = false;
    }

    return tokenType(pDomNode, tr("An identifier or an SI unit (e.g. 'second')"),
                     tokens);
}

//==============================================================================

bool CellmlTextViewParser::identifierToken(QDomNode &pDomNode)
{
    // Expect an identifier

    return tokenType(pDomNode, tr("An identifier"),
                     CellmlTextViewScanner::Token::IdentifierOrCmetaId);
}

//==============================================================================

bool CellmlTextViewParser::cmetaIdToken(QDomNode &pDomNode)
{
    // Expect a cmeta:id

    static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::IdentifierOrCmetaId
                                                                                        << CellmlTextViewScanner::Token::ProperCmetaId;

    return tokenType(pDomNode, tr("A cmeta:id"),
                     Tokens);
}

//==============================================================================

bool CellmlTextViewParser::modelToken(QDomNode &pDomNode)
{
    // Expect "model"

    return tokenType(pDomNode, "'model'",
                     CellmlTextViewScanner::Token::Model);
}

//==============================================================================

bool CellmlTextViewParser::numberToken(QDomNode &pDomNode)
{
    // Expect a number

    return tokenType(pDomNode, tr("A number"),
                     CellmlTextViewScanner::Token::Number);
}

//==============================================================================

bool CellmlTextViewParser::numberValueToken(QDomNode &pDomNode, int &pSign)
{
    // Check whether we have "+" or "-"

    pSign = 0;

    if (isTokenType(pDomNode, CellmlTextViewScanner::Token::Plus)) {
        pSign = 1;

        mScanner.getNextToken();
    } else if (isTokenType(pDomNode, CellmlTextViewScanner::Token::Minus)) {
        pSign = -1;

        mScanner.getNextToken();
    }

    // Expect a number

    return numberToken(pDomNode);
}

//==============================================================================

bool CellmlTextViewParser::openingBracketToken(QDomNode &pDomNode)
{
    // Expect "("

    return tokenType(pDomNode, "'('",
                     CellmlTextViewScanner::Token::OpeningBracket);
}

//==============================================================================

bool CellmlTextViewParser::openingCurlyBracketToken(QDomNode &pDomNode)
{
    // Expect "{"

    return tokenType(pDomNode, "'{'",
                     CellmlTextViewScanner::Token::OpeningCurlyBracket);
}

//==============================================================================

bool CellmlTextViewParser::semiColonToken(QDomNode &pDomNode)
{
    // Expect ";"

    return tokenType(pDomNode, "';'",
                     CellmlTextViewScanner::Token::SemiColon);
}

//==============================================================================

bool CellmlTextViewParser::strictlyPositiveIntegerNumberToken(QDomNode &pDomNode)
{
    // Check whether we have "+" or "-"

    int sign = 0;

    if (isTokenType(pDomNode, CellmlTextViewScanner::Token::Plus)) {
        sign = 1;

        mScanner.getNextToken();
    } else if (isTokenType(pDomNode, CellmlTextViewScanner::Token::Minus)) {
        sign = -1;

        mScanner.getNextToken();
    }

    // Expect a strictly positive integer number

    static const QString ExpectedString = tr("A strictly positive integer number");

    if (!tokenType(pDomNode, ExpectedString,
                   CellmlTextViewScanner::Token::Number)) {
        return false;
    }

    // We have got a number, but now the question is whether it is a strictly
    // positive integer one

    int number = mScanner.string().toInt();

    if (sign == -1)
        number = -number;

    if (number <= 0) {
        QString foundString = mScanner.string();

        if (sign == 1)
            foundString = "+"+foundString;
        else if (sign == -1)
            foundString = "-"+foundString;

        addUnexpectedTokenErrorMessage(ExpectedString, QString("'%1'").arg(foundString));

        return false;
    } else {
        return true;
    }
}

//==============================================================================

bool CellmlTextViewParser::unitToken(QDomNode &pDomNode)
{
    // Expect "unit"

    return tokenType(pDomNode, "'unit'",
                     CellmlTextViewScanner::Token::Unit);
}

//==============================================================================

bool CellmlTextViewParser::usingToken(QDomNode &pDomNode)
{
    // Expect "using"

    return tokenType(pDomNode, "'using'",
                     CellmlTextViewScanner::Token::Using);
}

//==============================================================================

QString CellmlTextViewParser::processCommentString(const QString &pComment)
{
    // From https://www.w3.org/TR/xml/#sec-comments, we can see that XML
    // comments cannot have "--" within them, yet we want to allow them in a
    // comment and one way to allow this is by replacing all occurrences of "--"
    // with its corresponding decimal HTML entity

    QString realComment = pComment;

    return realComment.replace("--", "&#45;&#45;");
}

//==============================================================================

void CellmlTextViewParser::parseComments(QDomNode &pDomNode)
{
    // Check whether there are some comments

    int prevLineCommentLine = 0;
    QString singleLineComments = QString();

    forever {
        if (mScanner.token() == CellmlTextViewScanner::Token::SingleLineComment) {
            // Keep track of the line comment, if no previous line comments are
            // being tracked

            if (!prevLineCommentLine) {
                singleLineComments = processCommentString(mScanner.string());
            } else {
                // There is at least one other line comment that is being
                // tracked, so compare line numbers

                if (mScanner.line() == prevLineCommentLine+1) {
                    // The line comment is directly on the line following the
                    // previous line comment, so add it to the list of tracked
                    // line comments

                    singleLineComments += "\n"+processCommentString(mScanner.string());
                } else {
                    // The line comment is not directly on the line following
                    // the previous line comment, so add the previous line
                    // comment(s) to the current node and keep track of the new
                    // line comment

                    pDomNode.appendChild(mDomDocument.createComment(singleLineComments.isEmpty()?" ":singleLineComments));

                    singleLineComments = processCommentString(mScanner.string());
                }
            }

            prevLineCommentLine = mScanner.line();
        } else if (mScanner.token() == CellmlTextViewScanner::Token::MultilineComment) {
            // We simply ignore the multiline comment

            ;
        } else {
            // No (more) comment(s left), so add the tracked line comment(s) to
            // the current node, if any, and leave

            if (prevLineCommentLine)
                pDomNode.appendChild(mDomDocument.createComment(singleLineComments.isEmpty()?" ":singleLineComments));

            return;
        }

        // Fetch the next token

        mScanner.getNextToken();
    }
}

//==============================================================================

bool CellmlTextViewParser::parseCmetaId(QDomElement &pDomElement)
{
    // Check whether a cmeta:id is given by checking whether the token is "{"

    QString cmetaId = QString();

    if (!isTokenType(pDomElement,
                     CellmlTextViewScanner::Token::OpeningCurlyBracket)) {
        return true;
    }

    // Expect a cmeta:id

    mScanner.getNextToken();

    if (!cmetaIdToken(pDomElement))
        return false;

    cmetaId = mScanner.string();

    // Expect "}"

    mScanner.getNextToken();

    if (!closingCurlyBracketToken(pDomElement))
        return false;

    // Fetch the next token

    mScanner.getNextToken();

    // Set the cmeta:id of the current DOM element, which in the case of a
    // MathML element must be done via the MathML id attribute (so we need to
    // check whether the current DOM element is within the MathML namespace)

    bool withinMathmlNamespace = false;
    QDomNode parentNode = pDomElement.parentNode();

    while (!parentNode.isNull()) {
        QDomNode xmlnsAttributeNode = parentNode.attributes().namedItem("xmlns");

        if (   !xmlnsAttributeNode.isNull()
            && !xmlnsAttributeNode.nodeValue().compare(CellMLSupport::MathmlNamespace)) {
            withinMathmlNamespace = true;

            break;
        } else {
            parentNode = parentNode.parentNode();
        }
    }

    if (withinMathmlNamespace) {
        pDomElement.setAttribute("id", cmetaId);
    } else {
        mNamespaces.insert("cmeta", CellMLSupport::CmetaIdNamespace);

        pDomElement.setAttribute("cmeta:id", cmetaId);
    }

    return true;
}

//==============================================================================

bool CellmlTextViewParser::parseModelDefinition(QDomNode &pDomNode)
{
    // Loop while we have "def" and leave if we get "enddef"

    static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Def
                                                                                        << CellmlTextViewScanner::Token::EndDef;

    while (tokenType(pDomNode, tr("'%1' or '%2'").arg("def")
                                                 .arg("enddef"),
                     Tokens)) {
        if (mScanner.token() == CellmlTextViewScanner::Token::Def) {
            // Expect a model definition

            static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Import
                                                                                                << CellmlTextViewScanner::Token::Unit
                                                                                                << CellmlTextViewScanner::Token::Comp
                                                                                                << CellmlTextViewScanner::Token::Group
                                                                                                << CellmlTextViewScanner::Token::Map;

            mScanner.getNextToken();

            if (tokenType(pDomNode, tr("'%1', '%2', '%3', '%4' or '%5'").arg("import")
                                                                        .arg("unit")
                                                                        .arg("comp")
                                                                        .arg("group")
                                                                        .arg("map"),
                          Tokens)) {
                if (mScanner.token() == CellmlTextViewScanner::Token::Import) {
                    if (!parseImportDefinition(pDomNode))
                        return false;
                } else if (mScanner.token() == CellmlTextViewScanner::Token::Unit) {
                    if (!parseUnitsDefinition(pDomNode))
                        return false;
                } else if (mScanner.token() == CellmlTextViewScanner::Token::Comp) {
                    if (!parseComponentDefinition(pDomNode))
                        return false;
                } else if (mScanner.token() == CellmlTextViewScanner::Token::Group) {
                    if (!parseGroupDefinition(pDomNode))
                        return false;
                } else {
                    if (!parseMapDefinition(pDomNode))
                        return false;
                }
            } else {
                return false;
            }

            // Fetch the next token

            mScanner.getNextToken();
        } else {
            return true;
        }
    }

    return false;
}

//==============================================================================

bool CellmlTextViewParser::parseImportDefinition(QDomNode &pDomNode)
{
    // Create our import element

    QDomElement importElement = newDomElement(pDomNode, "import");

    // Try to parse for a cmeta:id

    mScanner.getNextToken();

    if (!parseCmetaId(importElement))
        return false;

    // Expect "using"

    if (!usingToken(importElement))
        return false;

    // Expect a string representing a URL

    mScanner.getNextToken();

    if (!tokenType(importElement, tr("A string representing a URL"),
                   CellmlTextViewScanner::Token::String)) {
        return false;
    }

    // Set the URL, after having kept track of the fact that we need CellML 1.1
    // and the XLink namespace

    mCellmlVersion = CellMLSupport::CellmlFile::Version::Cellml_1_1;

    mNamespaces.insert("xlink", CellMLSupport::XlinkNamespace);

    importElement.setAttribute("xlink:href", mScanner.string());

    // Expect "for"

    mScanner.getNextToken();

    if (!forToken(importElement))
        return false;

    // Expect an import definition, so loop while we have "unit" or "comp", or
    // leave if we get "enddef"

    static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Unit
                                                                                        << CellmlTextViewScanner::Token::Comp
                                                                                        << CellmlTextViewScanner::Token::EndDef;

    mScanner.getNextToken();

    while (tokenType(importElement, tr("'%1', '%2' or '%3'").arg("unit")
                                                            .arg("comp")
                                                            .arg("enddef"),
                     Tokens)) {
        if (mScanner.token() == CellmlTextViewScanner::Token::Unit) {
            // We are dealing with a unit import, so create our unit import
            // element

            QDomElement unitsImportElement = newDomElement(importElement, "units");

            // Try to parse for a cmeta:id

            mScanner.getNextToken();

            if (!parseCmetaId(unitsImportElement))
                return false;

            // Expect an identifier

            if (!identifierToken(importElement))
                return false;

            // Set the name of the unit

            unitsImportElement.setAttribute("name", mScanner.string());

            // Expect "using"

            mScanner.getNextToken();

            if (!usingToken(importElement))
                return false;

            // Expect "unit"

            mScanner.getNextToken();

            if (!unitToken(importElement))
                return false;

            // Expect an identifier

            mScanner.getNextToken();

            if (!identifierToken(importElement))
                return false;

            // Set the name of the imported unit

            unitsImportElement.setAttribute("units_ref", mScanner.string());

            // Expect ";"

            mScanner.getNextToken();

            if (!semiColonToken(importElement))
                return false;

            // Fetch the next token

            mScanner.getNextToken();
        } else if (mScanner.token() == CellmlTextViewScanner::Token::Comp) {
            // We are dealing with a component import, so create our component
            // import element

            QDomElement componentImportElement = newDomElement(importElement, "component");

            // Try to parse for a cmeta:id

            mScanner.getNextToken();

            if (!parseCmetaId(componentImportElement))
                return false;

            // Expect an identifier

            if (!identifierToken(importElement))
                return false;

            // Set the name of the component

            componentImportElement.setAttribute("name", mScanner.string());

            // Expect "using"

            mScanner.getNextToken();

            if (!usingToken(importElement))
                return false;

            // Expect "comp"

            mScanner.getNextToken();

            if (!compToken(importElement))
                return false;

            // Expect an identifier

            mScanner.getNextToken();

            if (!identifierToken(importElement))
                return false;

            // Set the name of the imported component

            componentImportElement.setAttribute("component_ref", mScanner.string());

            // Expect ";"

            mScanner.getNextToken();

            if (!semiColonToken(importElement))
                return false;

            // Fetch the next token

            mScanner.getNextToken();
        } else {
            // Expect ";"

            mScanner.getNextToken();

            return semiColonToken(importElement);
        }
    }

    return false;
}

//==============================================================================

bool CellmlTextViewParser::parseUnitsDefinition(QDomNode &pDomNode)
{
    // Create our units element

    QDomElement unitsElement = newDomElement(pDomNode, "units");

    // Try to parse for a cmeta:id

    mScanner.getNextToken();

    if (!parseCmetaId(unitsElement))
        return false;

    // Expect an identifier

    if (!identifierToken(unitsElement))
        return false;

    // Set our unit's name

    unitsElement.setAttribute("name", mScanner.string());

    // Expect "as"

    mScanner.getNextToken();

    if (!asToken(unitsElement))
        return false;

    // Expect a unit definition

    static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Base
                                                                                        << CellmlTextViewScanner::Token::Unit
                                                                                        << CellmlTextViewScanner::Token::EndDef;

    mScanner.getNextToken();

    if (!tokenType(unitsElement, tr("'%1', '%2' or '%3'").arg("base")
                                                         .arg("unit")
                                                         .arg("enddef"),
                   Tokens)) {
        return false;
    }

    // Check the type of unit definition we are dealing with

    if (mScanner.token() == CellmlTextViewScanner::Token::Base) {
        // We are dealing with the definition of a base unit, so now expect
        // "unit"

        mScanner.getNextToken();

        if (!unitToken(unitsElement))
            return false;

        // Make our unit a base unit

        unitsElement.setAttribute("base_units", "yes");

        // Expect ";"

        mScanner.getNextToken();

        return semiColonToken(unitsElement);
    } else if (mScanner.token() == CellmlTextViewScanner::Token::Unit) {
        // We are dealing with a 'normal' unit definition, so loop while we have
        // "unit" or leave if we get "enddef"

        static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Unit
                                                                                            << CellmlTextViewScanner::Token::EndDef;

        while (tokenType(unitsElement, tr("'%1' or '%2'").arg("unit")
                                                         .arg("enddef"),
                         Tokens)) {
            if (mScanner.token() == CellmlTextViewScanner::Token::Unit) {
                if (!parseUnitDefinition(unitsElement))
                    return false;
            } else {
                // Expect ";"

                mScanner.getNextToken();

                return semiColonToken(unitsElement);
            }
        }

        return false;
    } else {
        // Expect ";"

        mScanner.getNextToken();

        return semiColonToken(unitsElement);
    }
}

//==============================================================================

bool CellmlTextViewParser::parseUnitDefinition(QDomNode &pDomNode)
{
    // Create our unit element

    QDomElement unitElement = newDomElement(pDomNode, "unit");

    // Try to parse for a cmeta:id

    mScanner.getNextToken();

    if (!parseCmetaId(unitElement))
        return false;

    // Expect an identifier or an SI unit

    if (!identifierOrSiUnitToken(unitElement))
        return false;

    // Set our unit's name

    unitElement.setAttribute("units", mScanner.string());

    // Expect "{" or ";"

    static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::OpeningCurlyBracket
                                                                                        << CellmlTextViewScanner::Token::SemiColon;

    mScanner.getNextToken();

    if (!tokenType(unitElement, tr("'%1' or '%2'").arg("{")
                                                  .arg(";"),
                   Tokens)) {
        return false;
    }

    // Check what we got exactly

    if (mScanner.token() == CellmlTextViewScanner::Token::OpeningCurlyBracket) {
        QList<CellmlTextViewScanner::Token> unitAttributesDefined = QList<CellmlTextViewScanner::Token>();

        forever {
            // Expect "pref", "expo", "mult" or "off"

            static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Pref
                                                                                                << CellmlTextViewScanner::Token::Expo
                                                                                                << CellmlTextViewScanner::Token::Mult
                                                                                                << CellmlTextViewScanner::Token::Off;

            mScanner.getNextToken();

            if (!tokenType(unitElement, tr("'%1', '%2', '%3' or '%4'").arg("pref")
                                                                      .arg("expo")
                                                                      .arg("mult")
                                                                      .arg("off"),
                           Tokens)) {
                return false;
            }

            // Make sure that we don't already have come across the unit
            // attribute

            if (unitAttributesDefined.contains(mScanner.token())) {
                mMessages << CellmlTextViewParserMessage(CellmlTextViewParserMessage::Type::Error,
                                                         mScanner.line(),
                                                         mScanner.column(),
                                                         tr("The '%1' attribute has already been specified.").arg(mScanner.string()));

                return false;
            } else {
                // Keep track of the fact that we have come across the unit
                // attribute

                unitAttributesDefined << mScanner.token();

                CellmlTextViewScanner::Token unitAttributeTokenType = mScanner.token();

                // Expect ":"

                mScanner.getNextToken();

                if (!colonToken(unitElement))
                    return false;

                // Check which unit attribute we are dealing with to determine
                // what to expect next

                mScanner.getNextToken();

                int sign = 0;

                if (unitAttributeTokenType == CellmlTextViewScanner::Token::Pref) {
                    // Check whether we have "+" or "-"

                    if (   isTokenType(unitElement, CellmlTextViewScanner::Token::Plus)
                        || isTokenType(unitElement, CellmlTextViewScanner::Token::Minus)) {
                        // We are dealing with a number value

                        if (!numberValueToken(unitElement, sign))
                            return false;
                    } else {
                        // We are not dealing with a 'proper' number value, but
                        // either a number or a prefix

                        // Expect a number or a prefix

                        static CellmlTextViewScanner::Tokens tokens = CellmlTextViewScanner::Tokens();
                        static bool needInitializeTokens = true;

                        if (needInitializeTokens) {
                            tokens = rangeOfTokens(CellmlTextViewScanner::Token::FirstPrefix,
                                                   CellmlTextViewScanner::Token::LastPrefix) << CellmlTextViewScanner::Token::Number;

                            needInitializeTokens = false;
                        }

                        if (!tokenType(unitElement, tr("A number or a prefix (e.g. 'milli')"),
                                       tokens)) {
                            return false;
                        }
                    }
                } else {
                    // Expect a number value

                    if (!numberValueToken(unitElement, sign))
                        return false;
                }

                // Set the attribute value

                QString unitAttributeValue = mScanner.string();

                if (sign == 1)
                    unitAttributeValue = "+"+unitAttributeValue;
                else if (sign == -1)
                    unitAttributeValue = "-"+unitAttributeValue;

                if (unitAttributeTokenType == CellmlTextViewScanner::Token::Pref)
                    unitElement.setAttribute("prefix", unitAttributeValue);
                else if (unitAttributeTokenType == CellmlTextViewScanner::Token::Expo)
                    unitElement.setAttribute("exponent", unitAttributeValue);
                else if (unitAttributeTokenType == CellmlTextViewScanner::Token::Mult)
                    unitElement.setAttribute("multiplier", unitAttributeValue);
                else
                    unitElement.setAttribute("offset", unitAttributeValue);
            }

            // Expect "," or "}"

            static const CellmlTextViewScanner::Tokens NextTokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Comma
                                                                                                    << CellmlTextViewScanner::Token::ClosingCurlyBracket;

            mScanner.getNextToken();

            if (!tokenType(unitElement, tr("'%1' or '%2'").arg(",")
                                                          .arg("}"),
                           NextTokens)) {
                return false;
            }

            // Leave the loop if we got "}"

            if (mScanner.token() == CellmlTextViewScanner::Token::ClosingCurlyBracket)
                break;
        }

        // Expect ";"

        mScanner.getNextToken();

        if (!semiColonToken(unitElement))
            return false;
    }

    // Fetch the next token

    mScanner.getNextToken();

    return true;
}

//==============================================================================

bool CellmlTextViewParser::parseComponentDefinition(QDomNode &pDomNode)
{
    // Create our component element

    QDomElement componentElement = newDomElement(pDomNode, "component");

    // Try to parse for a cmeta:id

    mScanner.getNextToken();

    if (!parseCmetaId(componentElement))
        return false;

    // Expect an identifier

    if (!identifierToken(componentElement))
        return false;

    // Set the component's name

    componentElement.setAttribute("name", mScanner.string());

    // Expect "as"

    mScanner.getNextToken();

    if (!asToken(componentElement))
        return false;

    // Loop while we have "def", "var", an identifier or "ode", or leave if we
    // get "enddef"

    static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Def
                                                                                        << CellmlTextViewScanner::Token::Var
                                                                                        << CellmlTextViewScanner::Token::IdentifierOrCmetaId
                                                                                        << CellmlTextViewScanner::Token::Ode
                                                                                        << CellmlTextViewScanner::Token::EndDef;

    QDomElement mathElement;
    bool hasMathElement = false;

    mScanner.getNextToken();

    while (tokenType(hasMathElement?mathElement:componentElement,
                     tr("'%1', '%2', an identifier, '%3' or '%4'").arg("def")
                                                                  .arg("var")
                                                                  .arg("ode")
                                                                  .arg("enddef"),
                     Tokens)) {
        // Move trailing comment(s), if any, from mathElement to
        // componentElement, if needed
        // Note: indeed since comments we come across while looking for anything
        //       but an identifier or "ode" will be related to that other thing
        //       rather than to the math block...

        if (   hasMathElement
            && (mScanner.token() != CellmlTextViewScanner::Token::IdentifierOrCmetaId)
            && (mScanner.token() != CellmlTextViewScanner::Token::Ode)) {
            moveTrailingComments(mathElement, componentElement);
        }

        if (mScanner.token() == CellmlTextViewScanner::Token::Def) {
            // Expect "unit"

            mScanner.getNextToken();

            if (    unitToken(componentElement)
                && !parseUnitsDefinition(componentElement)) {
                return false;
            }

            hasMathElement = false;
        } else if (mScanner.token() == CellmlTextViewScanner::Token::Var) {
            if (!parseVariableDeclaration(componentElement))
                return false;

            hasMathElement = false;
        } else if (   (mScanner.token() == CellmlTextViewScanner::Token::IdentifierOrCmetaId)
                   || (mScanner.token() == CellmlTextViewScanner::Token::Ode)) {
            if (!hasMathElement) {
                mathElement = newDomElement(componentElement, "math");

                mathElement.setAttribute("xmlns", CellMLSupport::MathmlNamespace);

                hasMathElement = true;
            }

            if (!parseMathematicalExpression(mathElement))
                return false;
        } else {
            // Expect ";"

            mScanner.getNextToken();

            return semiColonToken(hasMathElement?mathElement:componentElement);
        }

        // Fetch the next token

        mScanner.getNextToken();
    }

    return false;
}

//==============================================================================

bool CellmlTextViewParser::parseVariableDeclaration(QDomNode &pDomNode)
{
    // Create our variable element

    QDomElement variableElement = newDomElement(pDomNode, "variable");

    // Try to parse for a cmeta:id

    mScanner.getNextToken();

    if (!parseCmetaId(variableElement))
        return false;

    // Expect an identifier

    if (!identifierToken(variableElement))
        return false;

    // Set our variable's name

    variableElement.setAttribute("name", mScanner.string());

    // Expect ":"

    mScanner.getNextToken();

    if (!colonToken(variableElement))
        return false;

    // Expect an identifier or an SI unit

    mScanner.getNextToken();

    if (!identifierOrSiUnitToken(variableElement))
        return false;

    // Set our variable's unit

    variableElement.setAttribute("units", mScanner.string());

    // Expect "{" or ";"

    static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::OpeningCurlyBracket
                                                                                        << CellmlTextViewScanner::Token::SemiColon;

    mScanner.getNextToken();

    if (!tokenType(variableElement, tr("'%1' or '%2'").arg("{")
                                                      .arg(";"),
                   Tokens)) {
        return false;
    }

    // Check what we got exactly

    if (mScanner.token() == CellmlTextViewScanner::Token::OpeningCurlyBracket) {
        QList<CellmlTextViewScanner::Token> variableAttributesDefined = QList<CellmlTextViewScanner::Token>();

        forever {
            // Expect "init", "pub" or "priv"

            static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Init
                                                                                                << CellmlTextViewScanner::Token::Pub
                                                                                                << CellmlTextViewScanner::Token::Priv;

            mScanner.getNextToken();

            if (!tokenType(variableElement, tr("'%1', '%2' or '%3'").arg("init")
                                                                    .arg("pub")
                                                                    .arg("priv"),
                           Tokens)) {
                return false;
            }

            // Make sure that we don't already have come across the variable
            // attribute

            if (variableAttributesDefined.contains(mScanner.token())) {
                mMessages << CellmlTextViewParserMessage(CellmlTextViewParserMessage::Type::Error,
                                                         mScanner.line(),
                                                         mScanner.column(),
                                                         tr("The '%1' attribute has already been specified.").arg(mScanner.string()));

                return false;
            } else {
                // Keep track of the fact that we have come across the variable
                // attribute

                variableAttributesDefined << mScanner.token();

                CellmlTextViewScanner::Token unitAttributeTokenType = mScanner.token();

                // Expect ":"

                mScanner.getNextToken();

                if (!colonToken(variableElement))
                    return false;

                // Check which variable attribute we are dealing with to
                // determine what to expect next

                mScanner.getNextToken();

                int sign = 0;

                if (unitAttributeTokenType == CellmlTextViewScanner::Token::Init) {
                    // Check whether we have "+" or "-"

                    if (   isTokenType(variableElement, CellmlTextViewScanner::Token::Plus)
                        || isTokenType(variableElement, CellmlTextViewScanner::Token::Minus)) {
                        // We are dealing with a number value

                        if (!numberValueToken(variableElement, sign))
                            return false;
                    } else {
                        // We are not dealing with a 'proper' number value, but
                        // either a number or an identifier

                        // Expect a number or an identifier

                        static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Number
                                                                                                            << CellmlTextViewScanner::Token::IdentifierOrCmetaId;

                        if (!tokenType(variableElement, tr("A number or an identifier"),
                                       Tokens)) {
                            return false;
                        }

                        // If we got an identifier, then we need to kept track
                        // of the fact that we need CellML 1.1

                        if (mScanner.token() == CellmlTextViewScanner::Token::IdentifierOrCmetaId)
                            mCellmlVersion = CellMLSupport::CellmlFile::Version::Cellml_1_1;
                    }
                } else {
                    // Expect "in", "out" or "none"

                    static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::In
                                                                                                        << CellmlTextViewScanner::Token::Out
                                                                                                        << CellmlTextViewScanner::Token::None;

                    if (!tokenType(variableElement, tr("'%1', '%2' or '%3'").arg("in")
                                                                            .arg("out")
                                                                            .arg("none"),
                                   Tokens)) {
                        return false;
                    }
                }

                // Set the attribute value

                QString variableAttributeValue = mScanner.string();

                if (sign == 1)
                    variableAttributeValue = "+"+variableAttributeValue;
                else if (sign == -1)
                    variableAttributeValue = "-"+variableAttributeValue;

                if (unitAttributeTokenType == CellmlTextViewScanner::Token::Init)
                    variableElement.setAttribute("initial_value", variableAttributeValue);
                else if (unitAttributeTokenType == CellmlTextViewScanner::Token::Pub)
                    variableElement.setAttribute("public_interface", variableAttributeValue);
                else
                    variableElement.setAttribute("private_interface", variableAttributeValue);
            }

            // Expect "," or "}"

            static const CellmlTextViewScanner::Tokens NextTokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Comma
                                                                                                    << CellmlTextViewScanner::Token::ClosingCurlyBracket;

            mScanner.getNextToken();

            if (!tokenType(variableElement, tr("'%1' or '%2'").arg(",")
                                                              .arg("}"),
                           NextTokens))
                return false;

            // Leave the loop if we got "}"

            if (mScanner.token() == CellmlTextViewScanner::Token::ClosingCurlyBracket)
                break;
        }

        // Expect ";"

        mScanner.getNextToken();

        return semiColonToken(variableElement);
    } else {
        return true;
    }
}

//==============================================================================

bool CellmlTextViewParser::parseMathematicalExpression(QDomNode &pDomNode,
                                                       bool pFullParsing)
{
    // Check whether we have got an identifier or "ode"

    QDomElement lhsElement = QDomElement();

    if (mScanner.token() == CellmlTextViewScanner::Token::IdentifierOrCmetaId)
        lhsElement = newIdentifierElement(mScanner.string());
    else if (mScanner.token() == CellmlTextViewScanner::Token::Ode)
        lhsElement = parseDerivativeIdentifier(pDomNode);

    // Check whether we have got an LHS element

    if (lhsElement.isNull())
        return false;

    // Expect "="

    mScanner.getNextToken();

    if (!eqToken(pDomNode))
        return false;

    // Create our apply element

    QDomElement applyElement = newDomElement(pDomNode, "apply");

    // Try to parse for a cmeta:id

    mScanner.getNextToken();

    if (!parseCmetaId(applyElement))
        return pFullParsing?false:true;

    // At this stage, we are done when it comes to partial parsing

    if (!pFullParsing) {
        mStatement = (mScanner.token() == CellmlTextViewScanner::Token::Sel)?Statement::PiecewiseSel:Statement::Normal;

        return true;
    }

    // Expect either a normal or a piecewise mathematical expression

    QDomElement rhsElement = isTokenType(applyElement, CellmlTextViewScanner::Token::Sel)?
                                 parsePiecewiseMathematicalExpression(pDomNode):
                                 parseNormalMathematicalExpression(pDomNode);

    if (rhsElement.isNull())
        return false;

    // Expect ";"

    if (!semiColonToken(pDomNode))
        return false;

    // Populate our apply element

    newDomElement(applyElement, "eq");

    applyElement.appendChild(lhsElement);
    applyElement.appendChild(rhsElement);

    return true;
}

//==============================================================================

bool CellmlTextViewParser::parseGroupDefinition(QDomNode &pDomNode)
{
    // Create our group element

    QDomElement groupElement = newDomElement(pDomNode, "group");

    // Try to parse for a cmeta:id

    mScanner.getNextToken();

    if (!parseCmetaId(groupElement))
        return false;

    // Expect "as"

    if (!asToken(groupElement))
        return false;

    // Expect some group information, so loop while we have "containment" or
    // "encapsulation"

    static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Containment
                                                                                        << CellmlTextViewScanner::Token::Encapsulation;

    mScanner.getNextToken();

    while (tokenType(groupElement, tr("'%1' or '%2'").arg("containment")
                                                     .arg("encapsulation"),
                     Tokens)) {
        if (mScanner.token() == CellmlTextViewScanner::Token::Containment) {
            // Create our relationship reference and make it a containment

            QDomElement relationshipRefElement = newDomElement(groupElement, "relationship_ref");

            relationshipRefElement.setAttribute("relationship", "containment");

            // Try to parse for a cmeta:id

            mScanner.getNextToken();

            if (!parseCmetaId(relationshipRefElement))
                return false;

            // Expect an identifier, "and" or "for"

            static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::IdentifierOrCmetaId
                                                                                                << CellmlTextViewScanner::Token::And
                                                                                                << CellmlTextViewScanner::Token::For;

            if (!tokenType(groupElement, tr("An identifier, '%1' or '%2'").arg("and")
                                                                          .arg("for"),
                           Tokens)) {
                return false;
            }

            // Check what we got exactly

            if (mScanner.token() == CellmlTextViewScanner::Token::IdentifierOrCmetaId) {
                // Set the name of the containment

                relationshipRefElement.setAttribute("name", mScanner.string());

                // Fetch the next token

                mScanner.getNextToken();
            }
        } else {
            // CellmlTextViewScanner::TokenType::EncapsulationToken

            // Create our relationship reference and make it an encapsulation

            QDomElement relationshipRefElement = newDomElement(groupElement, "relationship_ref");

            relationshipRefElement.setAttribute("relationship", "encapsulation");

            // Try to parse for a cmeta:id

            mScanner.getNextToken();

            if (!parseCmetaId(relationshipRefElement))
                return false;
        }

        // Expect "and" or "for"

        static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::And
                                                                                            << CellmlTextViewScanner::Token::For;

        if (tokenType(groupElement, tr("'%1' or '%2'").arg("and")
                                                      .arg("for"),
                      Tokens)) {
            if (mScanner.token() == CellmlTextViewScanner::Token::For)
                break;
            else
                mScanner.getNextToken();
        } else {
            return false;
        }
    }

    // Loop while we have "comp" or leave if we get "enddef"

    static const CellmlTextViewScanner::Tokens NextTokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Comp
                                                                                            << CellmlTextViewScanner::Token::EndDef;

    mScanner.getNextToken();

    while (tokenType(groupElement, tr("'%1' or '%2'").arg("comp")
                                                     .arg("enddef"),
                     NextTokens)) {
        if (mScanner.token() == CellmlTextViewScanner::Token::Comp) {
            // Recursively parse our component reference

            if (!parseComponentRefDefinition(groupElement))
                return false;
        } else {
            // Expect ";"

            mScanner.getNextToken();

            return semiColonToken(groupElement);
        }
    }

    return false;
}

//==============================================================================

bool CellmlTextViewParser::parseComponentRefDefinition(QDomNode &pDomNode)
{
    // Create our component reference element

    QDomElement componentRefElement = newDomElement(pDomNode, "component_ref");

    // Try to parse for a cmeta:id

    mScanner.getNextToken();

    if (!parseCmetaId(componentRefElement))
        return false;

    // Expect an identifier

    if (!identifierToken(componentRefElement))
        return false;

    // Set the name of the component reference

    componentRefElement.setAttribute("component", mScanner.string());

    // Expect "incl" or ";"

    static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Incl
                                                                                        << CellmlTextViewScanner::Token::SemiColon;

    mScanner.getNextToken();

    if (!tokenType(componentRefElement, tr("'%1' or '%2'").arg("incl")
                                                          .arg(";"),
                   Tokens)) {
        return false;
    }

    // Check what we got exactly

    if (mScanner.token() == CellmlTextViewScanner::Token::Incl) {
        // Expect at least one "comp", then loop while we have "comp" or leave
        // if we get "enddcomp"

        static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Comp
                                                                                            << CellmlTextViewScanner::Token::EndComp;

        bool firstTime = true;

        mScanner.getNextToken();

        do {
            if (firstTime) {
                firstTime = false;

                if (!compToken(componentRefElement))
                    return false;
            }

            if (mScanner.token() == CellmlTextViewScanner::Token::Comp) {
                // Recursively parse a component reference

                if (!parseComponentRefDefinition(componentRefElement))
                    return false;
            } else {
                break;
            }
        } while (tokenType(componentRefElement, tr("'%1' or '%2'").arg("comp")
                                                                  .arg("endcomp"),
                           Tokens));

        // Expect ";"

        mScanner.getNextToken();

        if (!semiColonToken(componentRefElement))
            return false;
    }

    // Fetch the next token

    mScanner.getNextToken();

    return true;
}

//==============================================================================

bool CellmlTextViewParser::parseMapDefinition(QDomNode &pDomNode)
{
    // Create our connection element

    QDomElement connectionElement = newDomElement(pDomNode, "connection");

    // Try to parse for a cmeta:id

    mScanner.getNextToken();

    if (!parseCmetaId(connectionElement))
        return false;

    // Expect "between"

    if (!betweenToken(connectionElement))
        return false;

    // Create our map components element

    QDomElement mapComponentsElement = newDomElement(connectionElement, "map_components");

    // Try to parse for a cmeta:id

    mScanner.getNextToken();

    if (!parseCmetaId(mapComponentsElement))
        return false;

    // Expect an identifier

    if (!identifierToken(connectionElement))
        return false;

    // Set our first component

    mapComponentsElement.setAttribute("component_1", mScanner.string());

    // Expect "and"

    mScanner.getNextToken();

    if (!andToken(connectionElement))
        return false;

    // Expect an identifier

    mScanner.getNextToken();

    if (!identifierToken(connectionElement))
        return false;

    // Set our second component

    mapComponentsElement.setAttribute("component_2", mScanner.string());

    // Expect "for"

    mScanner.getNextToken();

    if (!forToken(connectionElement))
        return false;

    // Expect a mapping, so loop while we have "vars" or leave if we get
    // "enddef"

    static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Vars
                                                                                        << CellmlTextViewScanner::Token::EndDef;

    mScanner.getNextToken();

    while (tokenType(connectionElement, tr("'%1' or '%2'").arg("vars")
                                                          .arg("enddef"),
                     Tokens)) {
        if (mScanner.token() == CellmlTextViewScanner::Token::Vars) {
            // Create our map variables element

            QDomElement mapVariablesElement = newDomElement(connectionElement, "map_variables");

            // Try to parse for a cmeta:id

            mScanner.getNextToken();

            if (!parseCmetaId(mapVariablesElement))
                return false;

            // Expect an identifier

            if (!identifierToken(connectionElement))
                return false;

            // Set the name of the first variable

            mapVariablesElement.setAttribute("variable_1", mScanner.string());

            // Expect "and"

            mScanner.getNextToken();

            if (!andToken(connectionElement))
                return false;

            // Expect an identifier

            mScanner.getNextToken();

            if (!identifierToken(connectionElement))
                return false;

            // Set the name of the second variable

            mapVariablesElement.setAttribute("variable_2", mScanner.string());

            // Expect ";"

            mScanner.getNextToken();

            if (!semiColonToken(connectionElement))
                return false;

            // Fetch the next token

            mScanner.getNextToken();
        } else {
            // Expect ";"

            mScanner.getNextToken();

            return semiColonToken(connectionElement);
        }
    }

    return false;
}

//==============================================================================

QString CellmlTextViewParser::mathmlName(CellmlTextViewScanner::Token pTokenType) const
{
    // Return the MathML name of the given token type

    switch (pTokenType) {
    case CellmlTextViewScanner::Token::And:
        return "and";
    case CellmlTextViewScanner::Token::Or:
        return "or";
    case CellmlTextViewScanner::Token::Xor:
        return "xor";
    case CellmlTextViewScanner::Token::Not:
        return "not";
    case CellmlTextViewScanner::Token::Abs:
        return "abs";
    case CellmlTextViewScanner::Token::Ceil:
        return "ceiling";
    case CellmlTextViewScanner::Token::Exp:
        return "exp";
    case CellmlTextViewScanner::Token::Fact:
        return "factorial";
    case CellmlTextViewScanner::Token::Floor:
        return "floor";
    case CellmlTextViewScanner::Token::Ln:
        return "ln";
    case CellmlTextViewScanner::Token::Sqr:
        return "power";
    case CellmlTextViewScanner::Token::Sqrt:
        return "root";
    case CellmlTextViewScanner::Token::Min:
        return "min";
    case CellmlTextViewScanner::Token::Max:
        return "max";
    case CellmlTextViewScanner::Token::Gcd:
        return "gcd";
    case CellmlTextViewScanner::Token::Lcm:
        return "lcm";
    case CellmlTextViewScanner::Token::Sin:
        return "sin";
    case CellmlTextViewScanner::Token::Cos:
        return "cos";
    case CellmlTextViewScanner::Token::Tan:
        return "tan";
    case CellmlTextViewScanner::Token::Sec:
        return "sec";
    case CellmlTextViewScanner::Token::Csc:
        return "csc";
    case CellmlTextViewScanner::Token::Cot:
        return "cot";
    case CellmlTextViewScanner::Token::Sinh:
        return "sinh";
    case CellmlTextViewScanner::Token::Cosh:
        return "cosh";
    case CellmlTextViewScanner::Token::Tanh:
        return "tanh";
    case CellmlTextViewScanner::Token::Sech:
        return "sech";
    case CellmlTextViewScanner::Token::Csch:
        return "csch";
    case CellmlTextViewScanner::Token::Coth:
        return "coth";
    case CellmlTextViewScanner::Token::Asin:
        return "arcsin";
    case CellmlTextViewScanner::Token::Acos:
        return "arccos";
    case CellmlTextViewScanner::Token::Atan:
        return "arctan";
    case CellmlTextViewScanner::Token::Asec:
        return "arcsec";
    case CellmlTextViewScanner::Token::Acsc:
        return "arccsc";
    case CellmlTextViewScanner::Token::Acot:
        return "arccot";
    case CellmlTextViewScanner::Token::Asinh:
        return "arcsinh";
    case CellmlTextViewScanner::Token::Acosh:
        return "arccosh";
    case CellmlTextViewScanner::Token::Atanh:
        return "arctanh";
    case CellmlTextViewScanner::Token::Asech:
        return "arcsech";
    case CellmlTextViewScanner::Token::Acsch:
        return "arccsch";
    case CellmlTextViewScanner::Token::Acoth:
        return "arccoth";
    case CellmlTextViewScanner::Token::Log:
        return "log";
    case CellmlTextViewScanner::Token::Pow:
        return "power";
    case CellmlTextViewScanner::Token::Rem:
        return "rem";
    case CellmlTextViewScanner::Token::Root:
        return "root";
    case CellmlTextViewScanner::Token::True:
        return "true";
    case CellmlTextViewScanner::Token::False:
        return "false";
    case CellmlTextViewScanner::Token::Nan:
        return "notanumber";
    case CellmlTextViewScanner::Token::Pi:
        return "pi";
    case CellmlTextViewScanner::Token::Inf:
        return "infinity";
    case CellmlTextViewScanner::Token::E:
        return "exponentiale";
    case CellmlTextViewScanner::Token::EqEq:
        return "eq";
    case CellmlTextViewScanner::Token::Neq:
        return "neq";
    case CellmlTextViewScanner::Token::Lt:
        return "lt";
    case CellmlTextViewScanner::Token::Leq:
        return "leq";
    case CellmlTextViewScanner::Token::Gt:
        return "gt";
    case CellmlTextViewScanner::Token::Geq:
        return "geq";
    case CellmlTextViewScanner::Token::Plus:
        return "plus";
    case CellmlTextViewScanner::Token::Minus:
        return "minus";
    case CellmlTextViewScanner::Token::Times:
        return "times";
    case CellmlTextViewScanner::Token::Divide:
        return "divide";
    default:
#ifdef QT_DEBUG
        qFatal("FATAL ERROR | %s:%d: no MathML name exists for the given token type.", __FILE__, __LINE__);

    #ifdef Q_OS_WIN
        return "???";
    #endif
#else
        return "???";
#endif
    }
}

//==============================================================================

QDomElement CellmlTextViewParser::parseDerivativeIdentifier(QDomNode &pDomNode)
{
    // At this stage, we have already come across "ode", so now expect "("

    mScanner.getNextToken();

    if (!openingBracketToken(pDomNode))
        return QDomElement();

    // Expect an identifier

    mScanner.getNextToken();

    if (!identifierToken(pDomNode))
        return QDomElement();

    // Keep track of our f

    QString f = mScanner.string();

    // Expect ","

    mScanner.getNextToken();

    if (!commaToken(pDomNode))
        return QDomElement();

    // Expect an identifier

    mScanner.getNextToken();

    if (!identifierToken(pDomNode))
        return QDomElement();

    // Keep track of our x

    QString x = mScanner.string();

    // Expect "," or ")"

    static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Comma
                                                                                        << CellmlTextViewScanner::Token::ClosingBracket;

    mScanner.getNextToken();

    if (!tokenType(pDomNode, tr("'%1' or '%2'").arg(",")
                                               .arg(")"),
                   Tokens)) {
        return QDomElement();
    }

    // Check what we got exactly

    if (mScanner.token() == CellmlTextViewScanner::Token::Comma) {
        // Expect a strictly positive integer number

        mScanner.getNextToken();

        if (!strictlyPositiveIntegerNumberToken(pDomNode))
            return QDomElement();

        // Keep track of our order

        QString order = mScanner.string();

        // Expect "{"

        mScanner.getNextToken();

        if (!openingCurlyBracketToken(pDomNode))
            return QDomElement();

        // Expect "dimensionless"

        mScanner.getNextToken();

        if (!dimensionlessToken(pDomNode))
            return QDomElement();

        // Expect "}"

        mScanner.getNextToken();

        if (!closingCurlyBracketToken(pDomNode))
            return QDomElement();

        // Expect ")"

        mScanner.getNextToken();

        if (!closingBracketToken(pDomNode))
            return QDomElement();

        // Return a derivative element with an order

        return newDerivativeElement(f, x, order);
    } else {
        // Return a derivative element with no order

        return newDerivativeElement(f, x);
    }
}

//==============================================================================

QDomElement CellmlTextViewParser::parseNumber(QDomNode &pDomNode)
{
    // Keep track of the number
    // Note: this is useful to do in case the number is not valid (e.g. too big,
    //       too small)...

    QString number = mScanner.string();

    // Expect "{"

    mScanner.getNextToken();

    if (!openingCurlyBracketToken(pDomNode))
        return QDomElement();

    // Expect an identifier or an SI unit

    mScanner.getNextToken();

    if (!identifierOrSiUnitToken(pDomNode))
        return QDomElement();

    // Keep track of the unit for our number

    QString unit = mScanner.string();

    // Expect "}"

    mScanner.getNextToken();

    if (!closingCurlyBracketToken(pDomNode))
        return QDomElement();

    // Return a number element

    return newNumberElement(number, unit);
}

//==============================================================================

QDomElement CellmlTextViewParser::parseMathematicalFunction(QDomNode &pDomNode,
                                                            bool pOneArgument,
                                                            bool pTwoArguments,
                                                            bool pMoreArguments)
{
    // Keep track of the mathematical function

    CellmlTextViewScanner::Token tokenType = mScanner.token();

    // Expect "("

    mScanner.getNextToken();

    if (!openingBracketToken(pDomNode))
        return QDomElement();

    // Try to parse the first argument as a normal mathematical expression

    mScanner.getNextToken();

    QList<QDomElement> argumentElements = QList<QDomElement>();
    QDomElement argumentElement = parseNormalMathematicalExpression(pDomNode);

    if (argumentElement.isNull())
        return QDomElement();
    else
        argumentElements << argumentElement;

    // Check wheter we expect or might expect a second argument

    if (   (   pOneArgument && pTwoArguments
            && isTokenType(pDomNode, CellmlTextViewScanner::Token::Comma))
        || (!pOneArgument && pTwoArguments)) {
        // Expect ",", should we always be expecting a second argument

        if (!pOneArgument && pTwoArguments && !commaToken(pDomNode))
            return QDomElement();

        // Try to parse the second argument as a normal mathematical expression

        mScanner.getNextToken();

        argumentElement = parseNormalMathematicalExpression(pDomNode);

        if (argumentElement.isNull())
            return QDomElement();
        else
            argumentElements << argumentElement;
    }

    // Check wheter we might expect more arguments

    while (   pTwoArguments && pMoreArguments
           && isTokenType(pDomNode, CellmlTextViewScanner::Token::Comma)) {
        // Try to parse the nth argument as a normal mathematical expression

        mScanner.getNextToken();

        argumentElement = parseNormalMathematicalExpression(pDomNode);

        if (argumentElement.isNull())
            return QDomElement();
        else
            argumentElements << argumentElement;
    }

    // Expect ")"

    if (!closingBracketToken(pDomNode))
        return QDomElement();

    // Return a mathematical function element

    return newMathematicalFunctionElement(tokenType, argumentElements);
}

//==============================================================================

QDomElement CellmlTextViewParser::parseParenthesizedMathematicalExpression(QDomNode &pDomNode)
{
    // Try to parse a normal mathematical expression

    mScanner.getNextToken();

    QDomElement res = parseNormalMathematicalExpression(pDomNode);

    if (res.isNull())
        return QDomElement();

    // Expect ")"

    if (!closingBracketToken(pDomNode))
        return QDomElement();

    // Return our normal mathematical expression

    return res;
}

//==============================================================================

QDomElement CellmlTextViewParser::parseMathematicalExpressionElement(QDomNode &pDomNode,
                                                                     const CellmlTextViewScanner::Tokens &pTokens,
                                                                     ParseNormalMathematicalExpressionFunction pFunction)
{
    // Try to parse something of the form:
    //
    //     <operand1> [<operator> <operand2> <operator> <operand3> ...]
    //
    // with <operandn> the result of a call to pFunction

    // Expect an operand

    QDomElement res = (this->*pFunction)(pDomNode);

    if (res.isNull())
        return QDomElement();

    // Loop while we have a valid operator and operand

    CellmlTextViewScanner::Token prevOperator = CellmlTextViewScanner::Token::Unknown;

    forever {
        // Try to parse comments, if any

        parseComments(pDomNode);

        // Expect an operator

        CellmlTextViewScanner::Token crtOperator = mScanner.token();

        if (!pTokens.contains(crtOperator))
            return res;

        // Expect an operand

        mScanner.getNextToken();

        QDomElement otherOperand = (this->*pFunction)(pDomNode);

        if (otherOperand.isNull())
            return QDomElement();

        // Update our DOM tree with our operator and operand
        // Note #1: the check against the previous operator only makes sense for
        //          n-ary operators, i.e. "plus", "times", "and", "or" and
        //          "xor"...
        // Note #2: see http://www.w3.org/TR/MathML2/chapter4.html#contm.funopqual
        //          for more information...

        static const CellmlTextViewScanner::Tokens NaryOperators = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Plus
                                                                                                   << CellmlTextViewScanner::Token::Times
                                                                                                   << CellmlTextViewScanner::Token::And
                                                                                                   << CellmlTextViewScanner::Token::Or
                                                                                                   << CellmlTextViewScanner::Token::Xor;

        if ((crtOperator == prevOperator) && NaryOperators.contains(crtOperator)) {
            res.appendChild(otherOperand);
        } else {
            // Create an apply element and populate it with our operator and two
            // operands

            QDomElement applyElement = mDomDocument.createElement("apply");

            applyElement.appendChild(mDomDocument.createElement(mathmlName(crtOperator)));
            applyElement.appendChild(res);
            applyElement.appendChild(otherOperand);

            // Make our apply element our new result element

            res = applyElement;
        }

        // Keep track of our operator

        prevOperator = crtOperator;
    }
}

//==============================================================================

QDomElement CellmlTextViewParser::parseNormalMathematicalExpression(QDomNode &pDomNode)
{
    // Look for "or"

    return parseMathematicalExpressionElement(pDomNode,
                                              CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Or,
                                              &CellmlTextViewParser::parseNormalMathematicalExpression2);
}

//==============================================================================

QDomElement CellmlTextViewParser::parseNormalMathematicalExpression2(QDomNode &pDomNode)
{
    // Look for "and"

    return parseMathematicalExpressionElement(pDomNode,
                                              CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::And,
                                              &CellmlTextViewParser::parseNormalMathematicalExpression3);
}

//==============================================================================

QDomElement CellmlTextViewParser::parseNormalMathematicalExpression3(QDomNode &pDomNode)
{
    // Look for "xor"

    return parseMathematicalExpressionElement(pDomNode,
                                              CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Xor,
                                              &CellmlTextViewParser::parseNormalMathematicalExpression4);
}

//==============================================================================

QDomElement CellmlTextViewParser::parseNormalMathematicalExpression4(QDomNode &pDomNode)
{
    // Look for "==" or "<>"

    return parseMathematicalExpressionElement(pDomNode,
                                              CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::EqEq
                                                                              << CellmlTextViewScanner::Token::Neq,
                                              &CellmlTextViewParser::parseNormalMathematicalExpression5);
}

//==============================================================================

QDomElement CellmlTextViewParser::parseNormalMathematicalExpression5(QDomNode &pDomNode)
{
    // Look for "<", ">", "<=" or ">="

    return parseMathematicalExpressionElement(pDomNode,
                                              CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Lt
                                                                              << CellmlTextViewScanner::Token::Gt
                                                                              << CellmlTextViewScanner::Token::Leq
                                                                              << CellmlTextViewScanner::Token::Geq,
                                              &CellmlTextViewParser::parseNormalMathematicalExpression6);
}

//==============================================================================

QDomElement CellmlTextViewParser::parseNormalMathematicalExpression6(QDomNode &pDomNode)
{
    // Look for "+" or "-"

    return parseMathematicalExpressionElement(pDomNode,
                                              CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Plus
                                                                              << CellmlTextViewScanner::Token::Minus,
                                              &CellmlTextViewParser::parseNormalMathematicalExpression7);
}

//==============================================================================

QDomElement CellmlTextViewParser::parseNormalMathematicalExpression7(QDomNode &pDomNode)
{
    // Look for "*" or "/"

    return parseMathematicalExpressionElement(pDomNode,
                                              CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Times
                                                                              << CellmlTextViewScanner::Token::Divide,
                                              &CellmlTextViewParser::parseNormalMathematicalExpression8);
}

//==============================================================================

QDomElement CellmlTextViewParser::parseNormalMathematicalExpression8(QDomNode &pDomNode)
{
    // Try to parse comments, if any

    parseComments(pDomNode);

    // Look for "not", unary "+" or unary "-"

    static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Not
                                                                                        << CellmlTextViewScanner::Token::Plus
                                                                                        << CellmlTextViewScanner::Token::Minus;

    CellmlTextViewScanner::Token crtOperator = mScanner.token();

    if (Tokens.contains(crtOperator)) {
        QDomElement operand;

        if (mScanner.token() == CellmlTextViewScanner::Token::Not) {
            mScanner.getNextToken();

            operand = parseNormalMathematicalExpression(pDomNode);
        } else {
            mScanner.getNextToken();

            operand = parseNormalMathematicalExpression8(pDomNode);
        }

        if (operand.isNull())
            return QDomElement();

        // Create and return an apply element that has been populated with our
        // operator and operand

        QDomElement res = mDomDocument.createElement("apply");

        res.appendChild(mDomDocument.createElement(mathmlName(crtOperator)));
        res.appendChild(operand);

        return res;
    } else {
        return parseNormalMathematicalExpression9(pDomNode);
    }
}

//==============================================================================

QDomElement CellmlTextViewParser::parseNormalMathematicalExpression9(QDomNode &pDomNode)
{
    // Look for an identifier, "ode", a number, a mathematical constant, a
    // mathematical function or an opening bracket

    QDomElement res;

    static CellmlTextViewScanner::Tokens mahematicalConstantTokens = CellmlTextViewScanner::Tokens();
    static CellmlTextViewScanner::Tokens oneArgumentMathematicalFunctionTokens = CellmlTextViewScanner::Tokens();
    static CellmlTextViewScanner::Tokens oneOrTwoArgumentMathematicalFunctionTokens = CellmlTextViewScanner::Tokens();
    static CellmlTextViewScanner::Tokens twoArgumentMathematicalFunctionTokens = CellmlTextViewScanner::Tokens();
    static CellmlTextViewScanner::Tokens twoOrMoreArgumentMathematicalFunctionTokens = CellmlTextViewScanner::Tokens();
    static bool needInitializeTokens = true;

    if (needInitializeTokens) {
        mahematicalConstantTokens = rangeOfTokens(CellmlTextViewScanner::Token::FirstMathematicalConstant,
                                                  CellmlTextViewScanner::Token::LastMathematicalConstant);
        oneArgumentMathematicalFunctionTokens = rangeOfTokens(CellmlTextViewScanner::Token::FirstOneArgumentMathematicalFunction,
                                                              CellmlTextViewScanner::Token::LastOneArgumentMathematicalFunction);
        oneOrTwoArgumentMathematicalFunctionTokens = rangeOfTokens(CellmlTextViewScanner::Token::FirstOneOrTwoArgumentMathematicalFunction,
                                                                   CellmlTextViewScanner::Token::LastOneOrTwoArgumentMathematicalFunction);
        twoArgumentMathematicalFunctionTokens = rangeOfTokens(CellmlTextViewScanner::Token::FirstTwoArgumentMathematicalFunction,
                                                              CellmlTextViewScanner::Token::LastTwoArgumentMathematicalFunction);
        twoOrMoreArgumentMathematicalFunctionTokens = rangeOfTokens(CellmlTextViewScanner::Token::FirstTwoOrMoreArgumentMathematicalFunction,
                                                                    CellmlTextViewScanner::Token::LastTwoOrMoreArgumentMathematicalFunction);

        needInitializeTokens = false;
    }

    if (mScanner.token() == CellmlTextViewScanner::Token::IdentifierOrCmetaId) {
        // Create an identifier element

        res = newIdentifierElement(mScanner.string());
    } else if (mScanner.token() == CellmlTextViewScanner::Token::Ode) {
        // Try to parse a derivative identifier

        res = parseDerivativeIdentifier(pDomNode);
    } else if (mScanner.token() == CellmlTextViewScanner::Token::Number) {
        // Try to parse a number

        res = parseNumber(pDomNode);
    } else if (mahematicalConstantTokens.contains(mScanner.token())) {
        // Create a mathematical constant element

        res = newMathematicalConstantElement(mScanner.token());
    } else if (oneArgumentMathematicalFunctionTokens.contains(mScanner.token())) {
        // Try to parse a one-argument mathematical function

        res = parseMathematicalFunction(pDomNode, true, false, false);
    } else if (oneOrTwoArgumentMathematicalFunctionTokens.contains(mScanner.token())) {
        // Try to parse a one- or two-argument mathematical function

        res = parseMathematicalFunction(pDomNode, true, true, false);
    } else if (twoArgumentMathematicalFunctionTokens.contains(mScanner.token())) {
        // Try to parse a two-argument mathematical function

        res = parseMathematicalFunction(pDomNode, false, true, false);
    } else if (twoOrMoreArgumentMathematicalFunctionTokens.contains(mScanner.token())) {
        // Try to parse a two-or-more argument mathematical function

        res = parseMathematicalFunction(pDomNode, false, true, true);
    } else if (mScanner.token() == CellmlTextViewScanner::Token::OpeningBracket) {
        // Try to parse a parenthesised mathematical expression

        res = parseParenthesizedMathematicalExpression(pDomNode);
    } else {
        QString foundString = mScanner.string();

        if (mScanner.token() != CellmlTextViewScanner::Token::Eof)
            foundString = QString("'%1'").arg(foundString);

        addUnexpectedTokenErrorMessage(tr("An identifier, 'ode', a number, a mathematical function, a mathematical constant or '('"), foundString);

        return QDomElement();
    }

    // Fetch the next token and return our result element

    mScanner.getNextToken();

    return res;
}

//==============================================================================

QDomElement CellmlTextViewParser::parsePiecewiseMathematicalExpression(QDomNode &pDomNode)
{
    // Create our piecewise element

    QDomElement piecewiseElement = newDomElement(mDomDocument, "piecewise");

    // Loop while we have "case" or "otherwise", or leave if we get "endsel"

    static const CellmlTextViewScanner::Tokens Tokens = CellmlTextViewScanner::Tokens() << CellmlTextViewScanner::Token::Case
                                                                                        << CellmlTextViewScanner::Token::Otherwise
                                                                                        << CellmlTextViewScanner::Token::EndSel;

    bool hasOtherwiseClause = false;

    mScanner.getNextToken();

    while (tokenType(piecewiseElement, tr("'%1', '%2' or '%3'").arg("case")
                                                               .arg("otherwise")
                                                               .arg("endsel"),
                     Tokens)) {
        if (mScanner.token() == CellmlTextViewScanner::Token::EndSel)
            break;

        bool caseClause = mScanner.token() == CellmlTextViewScanner::Token::Case;
        QDomElement conditionElement = QDomElement();

        if (caseClause) {
            // Expect a condition in the form of a normal mathematical equation

            mScanner.getNextToken();

            conditionElement = parseNormalMathematicalExpression(piecewiseElement);

            if (conditionElement.isNull())
                return QDomElement();
        } else if (hasOtherwiseClause) {
            mMessages << CellmlTextViewParserMessage(CellmlTextViewParserMessage::Type::Error,
                                                     mScanner.line(),
                                                     mScanner.column(),
                                                     tr("There can only be one 'otherwise' clause."));

            return QDomElement();
        } else {
            hasOtherwiseClause = true;

            // Fetch the next token

            mScanner.getNextToken();
        }

        // Expect ":"

        if (!colonToken(piecewiseElement))
            return QDomElement();

        // Expect an expression in the form of a normal mathematical equation

        mScanner.getNextToken();

        QDomElement expressionElement = parseNormalMathematicalExpression(piecewiseElement);

        if (expressionElement.isNull())
            return QDomElement();

        // Expect ";"

        if (!semiColonToken(piecewiseElement))
            return QDomElement();

        // Create and populate our piece/otherwise element, and add it to our
        // piecewise element

        QDomElement pieceOrOtherwiseElement = newDomElement(mDomDocument, caseClause?"piece":"otherwise");

        pieceOrOtherwiseElement.appendChild(expressionElement);

        if (caseClause)
            pieceOrOtherwiseElement.appendChild(conditionElement);

        piecewiseElement.appendChild(pieceOrOtherwiseElement);

        // Fetch the next token

        mScanner.getNextToken();
    }

    // Expect "endsel"

    if (!endselToken(pDomNode))
        return QDomElement();

    // Fetch the next token and return our piecewise element

    mScanner.getNextToken();

    return piecewiseElement;
}

//==============================================================================

void CellmlTextViewParser::moveTrailingComments(QDomNode &pFromDomNode,
                                                QDomNode &pToDomNode)
{
    // Move trailing comments, if any, from pFromDomNode to pToDomNode

    if (!pFromDomNode.hasChildNodes())
        return;

    int i = pFromDomNode.childNodes().count()-1;

    while (pFromDomNode.childNodes().item(i).isComment())
        --i;

    for (++i; i != pFromDomNode.childNodes().count();)
        pToDomNode.appendChild(pFromDomNode.childNodes().item(i));
}

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
