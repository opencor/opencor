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

#include "cellmltextviewparser.h"
#include "corecliutils.h"

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

CellmlTextViewParserMessage::CellmlTextViewParserMessage(const Type &pType,
                                                         const int &pLine,
                                                         const int &pColumn,
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
    mScanner(new CellmlTextViewScanner()),
    mCellmlVersion(CellMLSupport::CellmlFile::Cellml_1_0),
    mDomDocument(QDomDocument()),
    mMessages(CellmlTextViewParserMessages()),
    mNamespaces(QMap<QString, QString>())
{
}

//==============================================================================

CellmlTextViewParser::~CellmlTextViewParser()
{
    // Delete some internal objects

    delete mScanner;
}

//==============================================================================

bool CellmlTextViewParser::execute(const QString &pText,
                                   const CellMLSupport::CellmlFile::Version &pCellmlVersion)
{
    // Get ready for the parsing

    mScanner->setText(pText);

    mCellmlVersion = CellMLSupport::CellmlFile::Cellml_1_0;

    mDomDocument = QDomDocument(QString());

    mMessages = CellmlTextViewParserMessages();

    mNamespaces = QMap<QString, QString>();

    // Expect "def"

    if (!defToken(mDomDocument))
        return false;

    // Expect "model"

    mScanner->getNextToken();

    if (!modelToken(mDomDocument))
        return false;

    // Create our model element

    QDomElement modelElement = newDomElement(mDomDocument, "model");

    // Try to parse for a cmeta:id

    mScanner->getNextToken();

    if (!parseCmetaId(modelElement))
        return false;

    // Expect an identifier

    if (!identifierToken(mDomDocument))
        return false;

    // Set our model name

    modelElement.setAttribute("name", mScanner->tokenString());

    // Expect "as"

    mScanner->getNextToken();

    if (!asToken(mDomDocument))
        return false;

    // Try to parse for some model definition itself

    mScanner->getNextToken();

    if (!parseModelDefinition(modelElement))
        return false;

    // Expect "enddef"

    if (!enddefToken(modelElement))
        return false;

    // Expect ";"

    mScanner->getNextToken();

    if (!semiColonToken(modelElement))
        return false;

    // Expect the end of the file

    mScanner->getNextToken();

    if (!tokenType(modelElement, QObject::tr("the end of the file"),
                   CellmlTextViewScanner::EofToken)) {
        return false;
    }

    // We are done, so add some processing instruction to our DOM document

    mDomDocument.insertBefore(mDomDocument.createProcessingInstruction("xml", "version='1.0'"),
                              mDomDocument.documentElement());

    // Use the given CellML version if it is higher than the one we actually
    // need

    mCellmlVersion = (pCellmlVersion > mCellmlVersion)?pCellmlVersion:mCellmlVersion;

    // Next, add the CellML namespace to our document element

    if (mCellmlVersion == CellMLSupport::CellmlFile::Cellml_1_1) {
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

    foreach (const QString &key, mNamespaces.keys())
        mDomDocument.documentElement().setAttribute(QString("xmlns:%1").arg(key), mNamespaces.value(key));

    return true;
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

CellmlTextViewParserMessages CellmlTextViewParser::messages() const
{
    // Return our messages

    return mMessages;
}

//==============================================================================

void CellmlTextViewParser::addUnexpectedTokenErrorMessage(const QString &pExpectedString,
                                                          const QString &pFoundString)
{
    // Add an error message for the given unexpected token

    mMessages << CellmlTextViewParserMessage(CellmlTextViewParserMessage::Error,
                                             mScanner->tokenLine(),
                                             mScanner->tokenColumn(),
                                             QObject::tr("%1 is expected, but %2 was found instead.").arg(pExpectedString, pFoundString));
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
    // Create and return a new identifier element with the given value

    QDomElement numberElement = mDomDocument.createElement("cn");

    numberElement.appendChild(mDomDocument.createTextNode(pNumber));
    numberElement.setAttribute("cellml:units", pUnit);

    return numberElement;
}

//==============================================================================

bool CellmlTextViewParser::tokenType(QDomNode &pDomNode,
                                     const QString &pExpectedString,
                                     const CellmlTextViewScanner::TokenTypes &pTokenTypes)
{
    // Try to parse comments, if any

    parseComments(pDomNode);

    // Check whether the current token type is the one we are after

    if (pTokenTypes.contains(mScanner->tokenType())) {
        // We have the correct token, so check whether a comment exists and, if
        // so, generate a warning for it

        if (!mScanner->tokenComment().isEmpty()) {
            mMessages << CellmlTextViewParserMessage(CellmlTextViewParserMessage::Warning,
                                                     mScanner->tokenLine(),
                                                     mScanner->tokenColumn(),
                                                     mScanner->tokenComment());
        }

        return true;
    } else if (mScanner->tokenType() == CellmlTextViewScanner::InvalidToken) {
        // This is the token we were expecting, but it is invalid

        mMessages << CellmlTextViewParserMessage(CellmlTextViewParserMessage::Error,
                                                 mScanner->tokenLine(),
                                                 mScanner->tokenColumn(),
                                                 mScanner->tokenComment());

        return false;
    } else {
        // This is not the token we were expecting, so let the user know about
        // it

        QString foundString = mScanner->tokenString();

        if (mScanner->tokenType() != CellmlTextViewScanner::EofToken)
            foundString = QString("'%1'").arg(foundString);

        addUnexpectedTokenErrorMessage(pExpectedString, foundString);

        return false;
    }
}

//==============================================================================

bool CellmlTextViewParser::tokenType(QDomNode &pDomNode,
                                     const QString &pExpectedString,
                                     const CellmlTextViewScanner::TokenType &pTokenType)
{
    // Expect the given token

    return tokenType(pDomNode, pExpectedString,
                     CellmlTextViewScanner::TokenTypes() << pTokenType);
}

//==============================================================================

bool CellmlTextViewParser::isTokenType(QDomNode &pDomNode,
                                       const CellmlTextViewScanner::TokenType &pTokenType)
{
    // Try to parse comments, if any

    parseComments(pDomNode);

    // Return whether the current token type is the one we are after

    return mScanner->tokenType() == pTokenType;
}

//==============================================================================

bool CellmlTextViewParser::andToken(QDomNode &pDomNode)
{
    // Expect "and"

    return tokenType(pDomNode, "'and'",
                     CellmlTextViewScanner::AndToken);
}

//==============================================================================

bool CellmlTextViewParser::asToken(QDomNode &pDomNode)
{
    // Expect "as"

    return tokenType(pDomNode, "'as'",
                     CellmlTextViewScanner::AsToken);
}

//==============================================================================

bool CellmlTextViewParser::betweenToken(QDomNode &pDomNode)
{
    // Expect "between"

    return tokenType(pDomNode, "'between'",
                     CellmlTextViewScanner::BetweenToken);
}

//==============================================================================

bool CellmlTextViewParser::caseToken(QDomNode &pDomNode)
{
    // Expect "case"

    return tokenType(pDomNode, "'case'",
                     CellmlTextViewScanner::CaseToken);
}

//==============================================================================

bool CellmlTextViewParser::closingBracketToken(QDomNode &pDomNode)
{
    // Expect ")"

    return tokenType(pDomNode, "')'",
                     CellmlTextViewScanner::ClosingBracketToken);
}

//==============================================================================

bool CellmlTextViewParser::closingCurlyBracketToken(QDomNode &pDomNode)
{
    // Expect "}"

    return tokenType(pDomNode, "'}'",
                     CellmlTextViewScanner::ClosingCurlyBracketToken);
}

//==============================================================================

bool CellmlTextViewParser::colonToken(QDomNode &pDomNode)
{
    // Expect ":"

    return tokenType(pDomNode, "':'",
                     CellmlTextViewScanner::ColonToken);
}

//==============================================================================

bool CellmlTextViewParser::commaToken(QDomNode &pDomNode)
{
    // Expect ","

    return tokenType(pDomNode, "','",
                     CellmlTextViewScanner::CommaToken);
}

//==============================================================================

bool CellmlTextViewParser::compToken(QDomNode &pDomNode)
{
    // Expect "comp"

    return tokenType(pDomNode, "'comp'",
                     CellmlTextViewScanner::CompToken);
}

//==============================================================================

bool CellmlTextViewParser::defToken(QDomNode &pDomNode)
{
    // Expect "def"

    return tokenType(pDomNode, "'def'",
                     CellmlTextViewScanner::DefToken);
}

//==============================================================================

bool CellmlTextViewParser::dimensionlessToken(QDomNode &pDomNode)
{
    // Expect "dimensionless"

    return tokenType(pDomNode, "'dimensionless'",
                     CellmlTextViewScanner::DimensionlessToken);
}

//==============================================================================

bool CellmlTextViewParser::endcompToken(QDomNode &pDomNode)
{
    // Expect "endcomp"

    return tokenType(pDomNode, "'endcomp'",
                     CellmlTextViewScanner::EndCompToken);
}

//==============================================================================

bool CellmlTextViewParser::enddefToken(QDomNode &pDomNode)
{
    // Expect "enddef"

    return tokenType(pDomNode, "'enddef'",
                     CellmlTextViewScanner::EndDefToken);
}

//==============================================================================

bool CellmlTextViewParser::endselToken(QDomNode &pDomNode)
{
    // Expect "endsel"

    return tokenType(pDomNode, "'endsel'",
                     CellmlTextViewScanner::EndSelToken);
}

//==============================================================================

bool CellmlTextViewParser::eqToken(QDomNode &pDomNode)
{
    // Expect "="

    return tokenType(pDomNode, "'='",
                     CellmlTextViewScanner::EqToken);
}

//==============================================================================

bool CellmlTextViewParser::forToken(QDomNode &pDomNode)
{
    // Expect "for"

    return tokenType(pDomNode, "'for'",
                     CellmlTextViewScanner::ForToken);
}

//==============================================================================

bool CellmlTextViewParser::identifierOrSiUnitToken(QDomNode &pDomNode)
{
    // Expect an identifier or an SI unit

    static CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::IdentifierToken;
    static bool needInitializeTokenTypes = true;

    if (needInitializeTokenTypes) {
        for (CellmlTextViewScanner::TokenType tokenType = CellmlTextViewScanner::FirstUnitToken;
             tokenType <= CellmlTextViewScanner::LastUnitToken;
             tokenType = CellmlTextViewScanner::TokenType(int(tokenType)+1)) {
            tokenTypes << tokenType;
        }

        needInitializeTokenTypes = false;
    }

    return tokenType(pDomNode, QObject::tr("An identifier or an SI unit (e.g. 'second')"),
                     tokenTypes);
}

//==============================================================================

bool CellmlTextViewParser::identifierToken(QDomNode &pDomNode)
{
    // Expect an identifier

    return tokenType(pDomNode, QObject::tr("An identifier"),
                     CellmlTextViewScanner::IdentifierToken);
}

//==============================================================================

bool CellmlTextViewParser::modelToken(QDomNode &pDomNode)
{
    // Expect "model"

    return tokenType(pDomNode, "'model'",
                     CellmlTextViewScanner::ModelToken);
}

//==============================================================================

bool CellmlTextViewParser::numberToken(QDomNode &pDomNode)
{
    // Expect a number

    return tokenType(pDomNode, QObject::tr("A number"),
                     CellmlTextViewScanner::NumberToken);
}

//==============================================================================

bool CellmlTextViewParser::numberValueToken(QDomNode &pDomNode, int &pSign)
{
    // Check whether we have "+" or "-"

    pSign = 0;

    if (isTokenType(pDomNode, CellmlTextViewScanner::PlusToken)) {
        pSign = 1;

        mScanner->getNextToken();
    } else if (isTokenType(pDomNode, CellmlTextViewScanner::MinusToken)) {
        pSign = -1;

        mScanner->getNextToken();
    }

    // Expect a number

    return numberToken(pDomNode);
}

//==============================================================================

bool CellmlTextViewParser::openingBracketToken(QDomNode &pDomNode)
{
    // Expect "("

    return tokenType(pDomNode, "'('",
                     CellmlTextViewScanner::OpeningBracketToken);
}

//==============================================================================

bool CellmlTextViewParser::openingCurlyBracketToken(QDomNode &pDomNode)
{
    // Expect "{"

    return tokenType(pDomNode, "'{'",
                     CellmlTextViewScanner::OpeningCurlyBracketToken);
}

//==============================================================================

bool CellmlTextViewParser::semiColonToken(QDomNode &pDomNode)
{
    // Expect ";"

    return tokenType(pDomNode, "';'",
                     CellmlTextViewScanner::SemiColonToken);
}

//==============================================================================

bool CellmlTextViewParser::strictlyPositiveIntegerNumberToken(QDomNode &pDomNode)
{
    // Check whether we have "+" or "-"

    int sign = 0;

    if (isTokenType(pDomNode, CellmlTextViewScanner::PlusToken)) {
        sign = 1;

        mScanner->getNextToken();
    } else if (isTokenType(pDomNode, CellmlTextViewScanner::MinusToken)) {
        sign = -1;

        mScanner->getNextToken();
    }

    // Expect a strictly positive integer number

    static const QString ExpectedString = QObject::tr("A strictly positive integer number");

    if (!tokenType(pDomNode, ExpectedString,
                   CellmlTextViewScanner::NumberToken)) {
        return false;
    }

    // We have got a number, but now the question is whether it is a strictly
    // positive integer one

    int number = mScanner->tokenString().toInt();

    if (sign == -1)
        number = -number;

    if (number <= 0) {
        QString foundString = mScanner->tokenString();

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
                     CellmlTextViewScanner::UnitToken);
}

//==============================================================================

bool CellmlTextViewParser::usingToken(QDomNode &pDomNode)
{
    // Expect "using"

    return tokenType(pDomNode, "'using'",
                     CellmlTextViewScanner::UsingToken);
}

//==============================================================================

void CellmlTextViewParser::parseComments(QDomNode &pDomNode)
{
    // Check whether there are some comments

    int previousLineCommentLine = 0;
    QString singleLineComments = QString();

    forever {
        if (mScanner->tokenType() == CellmlTextViewScanner::SingleLineCommentToken) {
            // Keep track of the line comment, if no previous line comments are
            // being tracked

            if (singleLineComments.isEmpty()) {
                singleLineComments = mScanner->tokenString();
            } else {
                // There is at least one other line comment that is being
                // tracked, so compare line numbers

                if (mScanner->tokenLine() == previousLineCommentLine+1) {
                    // The line comment is directly on the line following the
                    // previous line comment, so add it to the list of tracked
                    // line comments

                    singleLineComments += "\n"+mScanner->tokenString();
                } else {
                    // The line comment is not directly on the line following
                    // the previous line comment, so add the previous line
                    // comment(s) to the current node and keep track of the new
                    // line comment

                    pDomNode.appendChild(mDomDocument.createComment(singleLineComments));

                    singleLineComments = mScanner->tokenString();
                }
            }

            previousLineCommentLine = mScanner->tokenLine();
        } else if (mScanner->tokenType() == CellmlTextViewScanner::MultilineCommentToken) {
            // We simply ignore the multiline comment

            ;
        } else {
            // No (more) comment(s left), so add the tracked line comment(s) to
            // the current node, if any, and leave

            if (!singleLineComments.isEmpty())
                pDomNode.appendChild(mDomDocument.createComment(singleLineComments));

            return;
        }

        // Fetch the next token

        mScanner->getNextToken();
    }
}

//==============================================================================

bool CellmlTextViewParser::parseCmetaId(QDomElement &pDomElement)
{
    // Check whether a cmeta:id is given by checking whether the token is "{"

    QString cmetaId = QString();

    if (!isTokenType(pDomElement,
                     CellmlTextViewScanner::OpeningCurlyBracketToken)) {
        return true;
    }

    // Expect an identifier

    mScanner->getNextToken();

    if (!identifierToken(pDomElement))
        return false;

    // The identifier is our cmeta:id

    cmetaId = mScanner->tokenString();

    // Expect "}"

    mScanner->getNextToken();

    if (!closingCurlyBracketToken(pDomElement))
        return false;

    // Fetch the next token

    mScanner->getNextToken();

    // Set the cmeta:id of the current DOM element

    mNamespaces.insert("cmeta", CellMLSupport::CmetaIdNamespace);

    pDomElement.setAttribute("cmeta:id", cmetaId);

    return true;
}

//==============================================================================

bool CellmlTextViewParser::parseModelDefinition(QDomNode &pDomNode)
{
    // Loop while we have "def" and leave if we get "enddef"

    static const CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::DefToken
                                                                                                    << CellmlTextViewScanner::EndDefToken;

    while (tokenType(pDomNode, QObject::tr("'%1' or '%2'").arg("def", "enddef"),
                     tokenTypes)) {
        if (mScanner->tokenType() == CellmlTextViewScanner::DefToken) {
            // Expect a model definition

            static const CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::ImportToken
                                                                                                            << CellmlTextViewScanner::UnitToken
                                                                                                            << CellmlTextViewScanner::CompToken
                                                                                                            << CellmlTextViewScanner::GroupToken
                                                                                                            << CellmlTextViewScanner::MapToken;

            mScanner->getNextToken();

            if (tokenType(pDomNode, QObject::tr("'%1', '%2', '%3', '%4' or '%5'").arg("import", "unit", "comp", "group", "map"),
                          tokenTypes)) {
                if (mScanner->tokenType() == CellmlTextViewScanner::ImportToken) {
                    if (!parseImportDefinition(pDomNode))
                        return false;
                } else if (mScanner->tokenType() == CellmlTextViewScanner::UnitToken) {
                    if (!parseUnitsDefinition(pDomNode))
                        return false;
                } else if (mScanner->tokenType() == CellmlTextViewScanner::CompToken) {
                    if (!parseComponentDefinition(pDomNode))
                        return false;
                } else if (mScanner->tokenType() == CellmlTextViewScanner::GroupToken) {
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

            mScanner->getNextToken();
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

    mScanner->getNextToken();

    if (!parseCmetaId(importElement))
        return false;

    // Expect "using"

    if (!usingToken(importElement))
        return false;

    // Expect a string representing a URL

    mScanner->getNextToken();

    if (!tokenType(importElement, QObject::tr("A string representing a URL"),
                   CellmlTextViewScanner::StringToken)) {
        return false;
    }

    // Set the URL, after having kept track of the fact that we need CellML 1.1
    // and the XLink namespace

    mCellmlVersion = CellMLSupport::CellmlFile::Cellml_1_1;

    mNamespaces.insert("xlink", CellMLSupport::XlinkNamespace);

    importElement.setAttribute("xlink:href", mScanner->tokenString());

    // Expect "for"

    mScanner->getNextToken();

    if (!forToken(importElement))
        return false;

    // Expect an import definition, so loop while we have "unit" or "comp", or
    // leave if we get "enddef"

    static const CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::UnitToken
                                                                                                    << CellmlTextViewScanner::CompToken
                                                                                                    << CellmlTextViewScanner::EndDefToken;

    mScanner->getNextToken();

    while (tokenType(importElement, QObject::tr("'%1', '%2' or '%3'").arg("unit", "comp", "enddef"),
                     tokenTypes)) {
        if (mScanner->tokenType() == CellmlTextViewScanner::UnitToken) {
            // We are dealing with a unit import, so create our unit import
            // element

            QDomElement unitsImportElement = newDomElement(importElement, "units");

            // Try to parse for a cmeta:id

            mScanner->getNextToken();

            if (!parseCmetaId(unitsImportElement))
                return false;

            // Expect an identifier

            if (!identifierToken(importElement))
                return false;

            // Set the name of the unit

            unitsImportElement.setAttribute("name", mScanner->tokenString());

            // Expect "using"

            mScanner->getNextToken();

            if (!usingToken(importElement))
                return false;

            // Expect "unit"

            mScanner->getNextToken();

            if (!unitToken(importElement))
                return false;

            // Expect an identifier

            mScanner->getNextToken();

            if (!identifierToken(importElement))
                return false;

            // Set the name of the imported unit

            unitsImportElement.setAttribute("units_ref", mScanner->tokenString());

            // Expect ";"

            mScanner->getNextToken();

            if (!semiColonToken(importElement))
                return false;

            // Fetch the next token

            mScanner->getNextToken();
        } else if (mScanner->tokenType() == CellmlTextViewScanner::CompToken) {
            // We are dealing with a component import, so create our component
            // import element

            QDomElement componentImportElement = newDomElement(importElement, "component");

            // Try to parse for a cmeta:id

            mScanner->getNextToken();

            if (!parseCmetaId(componentImportElement))
                return false;

            // Expect an identifier

            if (!identifierToken(importElement))
                return false;

            // Set the name of the component

            componentImportElement.setAttribute("name", mScanner->tokenString());

            // Expect "using"

            mScanner->getNextToken();

            if (!usingToken(importElement))
                return false;

            // Expect "comp"

            mScanner->getNextToken();

            if (!compToken(importElement))
                return false;

            // Expect an identifier

            mScanner->getNextToken();

            if (!identifierToken(importElement))
                return false;

            // Set the name of the imported component

            componentImportElement.setAttribute("component_ref", mScanner->tokenString());

            // Expect ";"

            mScanner->getNextToken();

            if (!semiColonToken(importElement))
                return false;

            // Fetch the next token

            mScanner->getNextToken();
        } else {
            // Expect ";"

            mScanner->getNextToken();

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

    mScanner->getNextToken();

    if (!parseCmetaId(unitsElement))
        return false;

    // Expect an identifier

    if (!identifierToken(unitsElement))
        return false;

    // Set our unit's name

    unitsElement.setAttribute("name", mScanner->tokenString());

    // Expect "as"

    mScanner->getNextToken();

    if (!asToken(unitsElement))
        return false;

    // Expect a unit definition

    static const CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::BaseToken
                                                                                                    << CellmlTextViewScanner::UnitToken
                                                                                                    << CellmlTextViewScanner::EndDefToken;

    mScanner->getNextToken();

    if (!tokenType(unitsElement, QObject::tr("'%1', '%2' or '%3'").arg("base", "unit", "enddef"),
                   tokenTypes)) {
        return false;
    }

    // Check the type of unit definition we are dealing with

    if (mScanner->tokenType() == CellmlTextViewScanner::BaseToken) {
        // We are dealing with the definition of a base unit, so now expect
        // "unit"

        mScanner->getNextToken();

        if (!unitToken(unitsElement))
            return false;

        // Make our unit a base unit

        unitsElement.setAttribute("base_units", "yes");

        // Expect ";"

        mScanner->getNextToken();

        return semiColonToken(unitsElement);
    } else if (mScanner->tokenType() == CellmlTextViewScanner::UnitToken) {
        // We are dealing with a 'normal' unit definition, so loop while we have
        // "unit" or leave if we get "enddef"

        static const CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::UnitToken
                                                                                                        << CellmlTextViewScanner::EndDefToken;

        while (tokenType(unitsElement, QObject::tr("'%1' or '%2'").arg("unit", "enddef"),
                         tokenTypes)) {
            if (mScanner->tokenType() == CellmlTextViewScanner::UnitToken) {
                if (!parseUnitDefinition(unitsElement))
                    return false;
            } else {
                // Expect ";"

                mScanner->getNextToken();

                return semiColonToken(unitsElement);
            }
        }

        return false;
    } else {
        // Expect ";"

        mScanner->getNextToken();

        return semiColonToken(unitsElement);
    }
}

//==============================================================================

bool CellmlTextViewParser::parseUnitDefinition(QDomNode &pDomNode)
{
    // Create our unit element

    QDomElement unitElement = newDomElement(pDomNode, "unit");

    // Try to parse for a cmeta:id

    mScanner->getNextToken();

    if (!parseCmetaId(unitElement))
        return false;

    // Expect an identifier or an SI unit

    if (!identifierOrSiUnitToken(unitElement))
        return false;

    // Set our unit's name

    unitElement.setAttribute("units", mScanner->tokenString());

    // Expect "{" or ";"

    static const CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::OpeningCurlyBracketToken
                                                                                                    << CellmlTextViewScanner::SemiColonToken;

    mScanner->getNextToken();

    if (!tokenType(unitElement, QObject::tr("'%1' or '%2'").arg("{", ";"),
                   tokenTypes)) {
        return false;
    }

    // Check what we got exactly

    if (mScanner->tokenType() == CellmlTextViewScanner::OpeningCurlyBracketToken) {
        QList<CellmlTextViewScanner::TokenType> unitAttributesDefined = QList<CellmlTextViewScanner::TokenType>();

        forever {
            // Expect "pref", "expo", "mult" or "off"

            static const CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::PrefToken
                                                                                                            << CellmlTextViewScanner::ExpoToken
                                                                                                            << CellmlTextViewScanner::MultToken
                                                                                                            << CellmlTextViewScanner::OffToken;

            mScanner->getNextToken();

            if (!tokenType(unitElement, QObject::tr("'%1', '%2', '%3' or '%4'").arg("pref", "expo", "mult", "off"),
                           tokenTypes)) {
                return false;
            }

            // Make sure that we don't already have come across the unit
            // attribute

            if (unitAttributesDefined.contains(mScanner->tokenType())) {
                mMessages << CellmlTextViewParserMessage(CellmlTextViewParserMessage::Error,
                                                         mScanner->tokenLine(),
                                                         mScanner->tokenColumn(),
                                                         QObject::tr("The '%1' attribute has already been specified.").arg(mScanner->tokenString()));

                return false;
            } else {
                // Keep track of the fact that we have come across the unit
                // attribute

                unitAttributesDefined << mScanner->tokenType();

                CellmlTextViewScanner::TokenType unitAttributeTokenType = mScanner->tokenType();

                // Expect ":"

                mScanner->getNextToken();

                if (!colonToken(unitElement))
                    return false;

                // Check which unit attribute we are dealing with to determine
                // what to expect next

                mScanner->getNextToken();

                int sign = 0;

                if (unitAttributeTokenType == CellmlTextViewScanner::PrefToken) {
                    // Check whether we have "+" or "-"

                    if (   isTokenType(unitElement, CellmlTextViewScanner::PlusToken)
                        || isTokenType(unitElement, CellmlTextViewScanner::MinusToken)) {
                        // We are dealing with a number value

                        if (!numberValueToken(unitElement, sign))
                            return false;
                    } else {
                        // We are not dealing with a 'proper' number value, but
                        // either a number or a prefix

                        // Expect a number or a prefix

                        static CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::NumberToken;
                        static bool needInitializeTokenTypes = true;

                        if (needInitializeTokenTypes) {
                            for (CellmlTextViewScanner::TokenType tokenType = CellmlTextViewScanner::FirstPrefixToken;
                                 tokenType <= CellmlTextViewScanner::LastPrefixToken;
                                 tokenType = CellmlTextViewScanner::TokenType(int(tokenType)+1)) {
                                tokenTypes << tokenType;
                            }

                            needInitializeTokenTypes = false;
                        }

                        if (!tokenType(unitElement, QObject::tr("A number or a prefix (e.g. 'milli')"),
                                       tokenTypes)) {
                            return false;
                        }
                    }
                } else {
                    // Expect a number value

                    if (!numberValueToken(unitElement, sign))
                        return false;
                }

                // Set the attribute value

                QString unitAttributeValue = mScanner->tokenString();

                if (sign == 1)
                    unitAttributeValue = "+"+unitAttributeValue;
                else if (sign == -1)
                    unitAttributeValue = "-"+unitAttributeValue;

                if (unitAttributeTokenType == CellmlTextViewScanner::PrefToken)
                    unitElement.setAttribute("prefix", unitAttributeValue);
                else if (unitAttributeTokenType == CellmlTextViewScanner::ExpoToken)
                    unitElement.setAttribute("exponent", unitAttributeValue);
                else if (unitAttributeTokenType == CellmlTextViewScanner::MultToken)
                    unitElement.setAttribute("multiplier", unitAttributeValue);
                else
                    unitElement.setAttribute("offset", unitAttributeValue);
            }

            // Expect "," or "}"

            static const CellmlTextViewScanner::TokenTypes nextTokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::CommaToken
                                                                                                                << CellmlTextViewScanner::ClosingCurlyBracketToken;

            mScanner->getNextToken();

            if (!tokenType(unitElement, QObject::tr("'%1' or '%2'").arg(",", "}"),
                           nextTokenTypes)) {
                return false;
            }

            // Leave the loop if we got "}"

            if (mScanner->tokenType() == CellmlTextViewScanner::ClosingCurlyBracketToken)
                break;
        }

        // Expect ";"

        mScanner->getNextToken();

        if (!semiColonToken(unitElement))
            return false;
    }

    // Fetch the next token

    mScanner->getNextToken();

    return true;
}

//==============================================================================

bool CellmlTextViewParser::parseComponentDefinition(QDomNode &pDomNode)
{
    // Create our component element

    QDomElement componentElement = newDomElement(pDomNode, "component");

    // Try to parse for a cmeta:id

    mScanner->getNextToken();

    if (!parseCmetaId(componentElement))
        return false;

    // Expect an identifier

    if (!identifierToken(componentElement))
        return false;

    // Set the component's name

    componentElement.setAttribute("name", mScanner->tokenString());

    // Expect "as"

    mScanner->getNextToken();

    if (!asToken(componentElement))
        return false;

    // Loop while we have "def", "var", an identifier or "ode", or leave if we
    // get "enddef"

    static const CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::DefToken
                                                                                                    << CellmlTextViewScanner::VarToken
                                                                                                    << CellmlTextViewScanner::IdentifierToken
                                                                                                    << CellmlTextViewScanner::OdeToken
                                                                                                    << CellmlTextViewScanner::EndDefToken;

    QDomElement mathElement;
    bool hasMathElement = false;

    mScanner->getNextToken();

    while (tokenType(hasMathElement?mathElement:componentElement,
                     QObject::tr("'%1', '%2', an identifier, '%3' or '%4'").arg("def", "var", "ode", "enddef"),
                     tokenTypes)) {
        // Move trailing comment(s), if any, from mathElement to
        // componentElement, if needed
        // Note: indeed since comments we come across while looking for anything
        //       but an identifier or "ode" will be related to that other thing
        //       rather than to the math block...

        if (   hasMathElement
            && (mScanner->tokenType() != CellmlTextViewScanner::IdentifierToken)
            && (mScanner->tokenType() != CellmlTextViewScanner::OdeToken)) {
            moveTrailingComments(mathElement, componentElement);
        }

        if (mScanner->tokenType() == CellmlTextViewScanner::DefToken) {
            // Expect "unit"

            mScanner->getNextToken();

            if (unitToken(componentElement))
                if (!parseUnitsDefinition(componentElement))
                    return false;

            hasMathElement = false;
        } else if (mScanner->tokenType() == CellmlTextViewScanner::VarToken) {
            if (!parseVariableDeclaration(componentElement))
                return false;

            hasMathElement = false;
        } else if (   (mScanner->tokenType() == CellmlTextViewScanner::IdentifierToken)
                   || (mScanner->tokenType() == CellmlTextViewScanner::OdeToken)) {
            if (!hasMathElement) {
                mathElement = newDomElement(componentElement, "math");

                mathElement.setAttribute("xmlns", CellMLSupport::MathmlNamespace);

                hasMathElement = true;
            }

            if (!parseMathematicalExpression(mathElement))
                return false;
        } else {
            // Expect ";"

            mScanner->getNextToken();

            return semiColonToken(hasMathElement?mathElement:componentElement);
        }

        // Fetch the next token

        mScanner->getNextToken();
    }

    return false;
}

//==============================================================================

bool CellmlTextViewParser::parseVariableDeclaration(QDomNode &pDomNode)
{
    // Create our variable element

    QDomElement variableElement = newDomElement(pDomNode, "variable");

    // Try to parse for a cmeta:id

    mScanner->getNextToken();

    if (!parseCmetaId(variableElement))
        return false;

    // Expect an identifier

    if (!identifierToken(variableElement))
        return false;

    // Set our variable's name

    variableElement.setAttribute("name", mScanner->tokenString());

    // Expect ":"

    mScanner->getNextToken();

    if (!colonToken(variableElement))
        return false;

    // Expect an identifier or an SI unit

    mScanner->getNextToken();

    if (!identifierOrSiUnitToken(variableElement))
        return false;

    // Set our variable's unit

    variableElement.setAttribute("units", mScanner->tokenString());

    // Expect "{" or ";"

    static const CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::OpeningCurlyBracketToken
                                                                                                    << CellmlTextViewScanner::SemiColonToken;

    mScanner->getNextToken();

    if (!tokenType(variableElement, QObject::tr("'%1' or '%2'").arg("{", ";"),
                   tokenTypes)) {
        return false;
    }

    // Check what we got exactly

    if (mScanner->tokenType() == CellmlTextViewScanner::OpeningCurlyBracketToken) {
        QList<CellmlTextViewScanner::TokenType> variableAttributesDefined = QList<CellmlTextViewScanner::TokenType>();

        forever {
            // Expect "init", "pub" or "priv"

            static const CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::InitToken
                                                                                                            << CellmlTextViewScanner::PubToken
                                                                                                            << CellmlTextViewScanner::PrivToken;

            mScanner->getNextToken();

            if (!tokenType(variableElement, QObject::tr("'%1', '%2' or '%3'").arg("init", "pub", "priv"),
                           tokenTypes)) {
                return false;
            }

            // Make sure that we don't already have come across the variable
            // attribute

            if (variableAttributesDefined.contains(mScanner->tokenType())) {
                mMessages << CellmlTextViewParserMessage(CellmlTextViewParserMessage::Error,
                                                         mScanner->tokenLine(),
                                                         mScanner->tokenColumn(),
                                                         QObject::tr("The '%1' attribute has already been specified.").arg(mScanner->tokenString()));

                return false;
            } else {
                // Keep track of the fact that we have come across the variable
                // attribute

                variableAttributesDefined << mScanner->tokenType();

                CellmlTextViewScanner::TokenType unitAttributeTokenType = mScanner->tokenType();

                // Expect ":"

                mScanner->getNextToken();

                if (!colonToken(variableElement))
                    return false;

                // Check which variable attribute we are dealing with to
                // determine what to expect next

                mScanner->getNextToken();

                int sign = 0;

                if (unitAttributeTokenType == CellmlTextViewScanner::InitToken) {
                    // Check whether we have "+" or "-"

                    if (   isTokenType(variableElement, CellmlTextViewScanner::PlusToken)
                        || isTokenType(variableElement, CellmlTextViewScanner::MinusToken)) {
                        // We are dealing with a number value

                        if (!numberValueToken(variableElement, sign))
                            return false;
                    } else {
                        // We are not dealing with a 'proper' number value, but
                        // a number

                        // Expect a number

                        if (!numberToken(variableElement))
                            return false;
                    }
                } else {
                    // Expect "in", "out" or "none"

                    static const CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::InToken
                                                                                                                    << CellmlTextViewScanner::OutToken
                                                                                                                    << CellmlTextViewScanner::NoneToken;

                    if (!tokenType(variableElement, QObject::tr("'%1', '%2' or '%3'").arg("in", "out", "none"),
                                   tokenTypes)) {
                        return false;
                    }
                }

                // Set the attribute value

                QString variableAttributeValue = mScanner->tokenString();

                if (sign == 1)
                    variableAttributeValue = "+"+variableAttributeValue;
                else if (sign == -1)
                    variableAttributeValue = "-"+variableAttributeValue;

                if (unitAttributeTokenType == CellmlTextViewScanner::InitToken)
                    variableElement.setAttribute("initial_value", variableAttributeValue);
                else if (unitAttributeTokenType == CellmlTextViewScanner::PubToken)
                    variableElement.setAttribute("public_interface", variableAttributeValue);
                else
                    variableElement.setAttribute("private_interface", variableAttributeValue);
            }

            // Expect "," or "}"

            static const CellmlTextViewScanner::TokenTypes nextTokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::CommaToken
                                                                                                                << CellmlTextViewScanner::ClosingCurlyBracketToken;

            mScanner->getNextToken();

            if (!tokenType(variableElement, QObject::tr("'%1' or '%2'").arg(",", "}"),
                           nextTokenTypes))
                return false;

            // Leave the loop if we got "}"

            if (mScanner->tokenType() == CellmlTextViewScanner::ClosingCurlyBracketToken)
                break;
        }

        // Expect ";"

        mScanner->getNextToken();

        return semiColonToken(variableElement);
    } else {
        return true;
    }
}

//==============================================================================

bool CellmlTextViewParser::parseMathematicalExpression(QDomNode &pDomNode)
{
    // Check whether we got an identifier or "ode"

    QDomElement lhsElement;

    if (mScanner->tokenType() == CellmlTextViewScanner::IdentifierToken)
        lhsElement = newIdentifierElement(mScanner->tokenString());
    else
        lhsElement = parseDerivativeIdentifier(pDomNode);

    // Check whether we got a LHS element

    if (lhsElement.isNull())
        return false;

    // Expect "="

    mScanner->getNextToken();

    if (!eqToken(pDomNode))
        return false;

    // Expect either a normal or a piecewise mathematical expression

    mScanner->getNextToken();

    QDomElement rhsElement = (mScanner->tokenType() == CellmlTextViewScanner::SelToken)?
                                 parsePiecewiseMathematicalExpression(pDomNode):
                                 parseNormalMathematicalExpression(pDomNode);
//---GRY--- SHOULD WE DO SOMETHING IF rhsElement IS NULL?...

    if (rhsElement.isNull())
        return false;

    // Expect ";"

    if (!semiColonToken(pDomNode))
        return false;

    // Create and populate our apply element

    QDomElement applyElement = newDomElement(pDomNode, "apply");

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

    mScanner->getNextToken();

    if (!parseCmetaId(groupElement))
        return false;

    // Expect "as"

    if (!asToken(groupElement))
        return false;

    // Expect some group information, so loop while we have "containment" or
    // "encapsulation"

    static const CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::ContainmentToken
                                                                                                    << CellmlTextViewScanner::EncapsulationToken;

    mScanner->getNextToken();

    while (tokenType(groupElement, QObject::tr("'%1' or '%2'").arg("containment", "encapsulation"),
                     tokenTypes)) {
        if (mScanner->tokenType() == CellmlTextViewScanner::ContainmentToken) {
            // Create our relationship reference and make it a containment

            QDomElement relationshipRefElement = newDomElement(groupElement, "relationship_ref");

            relationshipRefElement.setAttribute("relationship", "containment");

            // Expect an identifier, "and" or "for"

            static const CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::IdentifierToken
                                                                                                            << CellmlTextViewScanner::AndToken
                                                                                                            << CellmlTextViewScanner::ForToken;

            mScanner->getNextToken();

            if (!tokenType(groupElement, QObject::tr("An identifier, '%1' or '%2'").arg("and", "for"),
                           tokenTypes)) {
                return false;
            }

            // Check what we got exactly

            if (mScanner->tokenType() == CellmlTextViewScanner::IdentifierToken) {
                // Set the name of the containment

                relationshipRefElement.setAttribute("name", mScanner->tokenString());

                // Fetch the next token

                mScanner->getNextToken();
            }
        } else {
            // CellmlTextViewScanner::EncapsulationToken

            // Create our relationship reference and make it an encapsulation

            QDomElement relationshipRefElement = newDomElement(groupElement, "relationship_ref");

            relationshipRefElement.setAttribute("relationship", "encapsulation");

            // Fetch the next token

            mScanner->getNextToken();
        }

        // Expect "and" or "for"

        static const CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::AndToken
                                                                                                        << CellmlTextViewScanner::ForToken;

        if (tokenType(groupElement, QObject::tr("'%1' or '%2'").arg("and", "for"),
                      tokenTypes)) {
            if (mScanner->tokenType() == CellmlTextViewScanner::ForToken)
                break;
            else
                mScanner->getNextToken();
        } else {
            return false;
        }
    }

    // Loop while we have "comp" or leave if we get "enddef"

    static const CellmlTextViewScanner::TokenTypes nextTokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::CompToken
                                                                                                        << CellmlTextViewScanner::EndDefToken;

    mScanner->getNextToken();

    while (tokenType(groupElement, QObject::tr("'%1' or '%2'").arg("comp", "enddef"),
                     nextTokenTypes)) {
        if (mScanner->tokenType() == CellmlTextViewScanner::CompToken) {
            // Recursively parse our component reference

            if (!parseComponentRefDefinition(groupElement))
                return false;
        } else {
            // Expect ";"

            mScanner->getNextToken();

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

    mScanner->getNextToken();

    if (!parseCmetaId(componentRefElement))
        return false;

    // Expect an identifier

    if (!identifierToken(componentRefElement))
        return false;

    // Set the name of the component reference

    componentRefElement.setAttribute("component", mScanner->tokenString());

    // Expect "incl" or ";"

    static const CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::InclToken
                                                                                                    << CellmlTextViewScanner::SemiColonToken;

    mScanner->getNextToken();

    if (!tokenType(componentRefElement, QObject::tr("'%1' or '%2'").arg("incl", ";"),
                   tokenTypes)) {
        return false;
    }

    // Check what we got exactly

    if (mScanner->tokenType() == CellmlTextViewScanner::InclToken) {
        // Expect at least one "comp", then loop while we have "comp" or leave
        // if we get "enddcomp"

        static const CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::CompToken
                                                                                                        << CellmlTextViewScanner::EndCompToken;

        bool firstTime = true;

        mScanner->getNextToken();

        do {
            if (firstTime) {
                firstTime = false;

                if (!compToken(componentRefElement))
                    return false;
            }

            if (mScanner->tokenType() == CellmlTextViewScanner::CompToken) {
                // Recursively parse a component reference

                if (!parseComponentRefDefinition(componentRefElement))
                    return false;
            } else {
                break;
            }
        } while (tokenType(componentRefElement, QObject::tr("'%1' or '%2'").arg("comp", "endcomp"),
                           tokenTypes));

        // Expect ";"

        mScanner->getNextToken();

        if (!semiColonToken(componentRefElement))
            return false;
    }

    // Fetch the next token

    mScanner->getNextToken();

    return true;
}

//==============================================================================

bool CellmlTextViewParser::parseMapDefinition(QDomNode &pDomNode)
{
    // Create our connection element

    QDomElement connectionElement = newDomElement(pDomNode, "connection");

    // Try to parse for a cmeta:id

    mScanner->getNextToken();

    if (!parseCmetaId(connectionElement))
        return false;

    // Expect "between"

    if (!betweenToken(connectionElement))
        return false;

    // Create our map components element

    QDomElement mapComponentsElement = newDomElement(connectionElement, "map_components");

    // Try to parse for a cmeta:id

    mScanner->getNextToken();

    if (!parseCmetaId(mapComponentsElement))
        return false;

    // Expect an identifier

    if (!identifierToken(connectionElement))
        return false;

    // Set our first component

    mapComponentsElement.setAttribute("component_1", mScanner->tokenString());

    // Expect "and"

    mScanner->getNextToken();

    if (!andToken(connectionElement))
        return false;

    // Expect an identifier

    mScanner->getNextToken();

    if (!identifierToken(connectionElement))
        return false;

    // Set our second component

    mapComponentsElement.setAttribute("component_2", mScanner->tokenString());

    // Expect "for"

    mScanner->getNextToken();

    if (!forToken(connectionElement))
        return false;

    // Expect a mapping, so loop while we have "vars" or leave if we get
    // "enddef"

    static const CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::VarsToken
                                                                                                    << CellmlTextViewScanner::EndDefToken;

    mScanner->getNextToken();

    while (tokenType(connectionElement, QObject::tr("'%1' or '%2'").arg("vars", "enddef"),
                     tokenTypes)) {
        if (mScanner->tokenType() == CellmlTextViewScanner::VarsToken) {
            // Create our map variables element

            QDomElement mapVariablesElement = newDomElement(connectionElement, "map_variables");

            // Try to parse for a cmeta:id

            mScanner->getNextToken();

            if (!parseCmetaId(mapVariablesElement))
                return false;

            // Expect an identifier

            if (!identifierToken(connectionElement))
                return false;

            // Set the name of the first variable

            mapVariablesElement.setAttribute("variable_1", mScanner->tokenString());

            // Expect "and"

            mScanner->getNextToken();

            if (!andToken(connectionElement))
                return false;

            // Expect an identifier

            mScanner->getNextToken();

            if (!identifierToken(connectionElement))
                return false;

            // Set the name of the second variable

            mapVariablesElement.setAttribute("variable_2", mScanner->tokenString());

            // Expect ";"

            mScanner->getNextToken();

            if (!semiColonToken(connectionElement))
                return false;

            // Fetch the next token

            mScanner->getNextToken();
        } else {
            // Expect ";"

            mScanner->getNextToken();

            return semiColonToken(connectionElement);
        }
    }

    return false;
}

//==============================================================================

QString CellmlTextViewParser::mathmlName(const CellmlTextViewScanner::TokenType &pTokenType) const
{
    // Return the MathML name of the given token type

    switch (pTokenType) {
    case CellmlTextViewScanner::OrToken:
        return "or";
    case CellmlTextViewScanner::AndToken:
        return "and";
    case CellmlTextViewScanner::XorToken:
        return "xor";
    case CellmlTextViewScanner::EqEqToken:
        return "eq";
    case CellmlTextViewScanner::NeqToken:
        return "neq";
    case CellmlTextViewScanner::LtToken:
        return "lt";
    case CellmlTextViewScanner::GtToken:
        return "gt";
    case CellmlTextViewScanner::LeqToken:
        return "leq";
    case CellmlTextViewScanner::GeqToken:
        return "geq";
    case CellmlTextViewScanner::PlusToken:
        return "plus";
    case CellmlTextViewScanner::MinusToken:
        return "minus";
    case CellmlTextViewScanner::TimesToken:
        return "times";
    case CellmlTextViewScanner::DivideToken:
        return "divide";
    case CellmlTextViewScanner::NotToken:
        return "not";
    default:
#ifdef QT_DEBUG
        qFatal("FATAL ERROR | %s:%d: no MathML name exists for the given token type.", __FILE__, __LINE__);
#endif

        return "???";
    }
}

//==============================================================================

QDomElement CellmlTextViewParser::parseDerivativeIdentifier(QDomNode &pDomNode)
{
    // At this stage, we have already come across "ode", so now expect "("

    mScanner->getNextToken();

    if (!openingBracketToken(pDomNode))
        return QDomElement();

    // Expect an identifier

    mScanner->getNextToken();

    if (!identifierToken(pDomNode))
        return QDomElement();

    // Keep track of our f

    QString f = mScanner->tokenString();

    // Expect ","

    mScanner->getNextToken();

    if (!commaToken(pDomNode))
        return QDomElement();

    // Expect an identifier

    mScanner->getNextToken();

    if (!identifierToken(pDomNode))
        return QDomElement();

    // Keep track of our x

    QString x = mScanner->tokenString();

    // Expect "," or ")"

    static const CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::CommaToken
                                                                                                    << CellmlTextViewScanner::ClosingBracketToken;

    mScanner->getNextToken();

    if (!tokenType(pDomNode, QObject::tr("'%1' or '%2'").arg(",", ")"),
                   tokenTypes)) {
        return QDomElement();
    }

    // Check what we got exactly

    if (mScanner->tokenType() == CellmlTextViewScanner::CommaToken) {
        // Expect a strictly positive integer number

        mScanner->getNextToken();

        if (!strictlyPositiveIntegerNumberToken(pDomNode))
            return QDomElement();

        // Keep track of our order

        QString order = mScanner->tokenString();

        // Expect "{"

        mScanner->getNextToken();

        if (!openingCurlyBracketToken(pDomNode))
            return QDomElement();

        // Expect "dimensionless"

        mScanner->getNextToken();

        if (!dimensionlessToken(pDomNode))
            return QDomElement();

        // Expect "}"

        mScanner->getNextToken();

        if (!closingCurlyBracketToken(pDomNode))
            return QDomElement();

        // Expect ")"

        mScanner->getNextToken();

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

QDomElement CellmlTextViewParser::parseMathematicalExpressionElement(QDomNode &pDomNode,
                                                                     const CellmlTextViewScanner::TokenTypes &pTokenTypes,
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

    CellmlTextViewScanner::TokenType prevOperator = CellmlTextViewScanner::UnknownToken;

    forever {
        // Expect an operator

        CellmlTextViewScanner::TokenType crtOperator = mScanner->tokenType();

        if (!pTokenTypes.contains(crtOperator))
            return res;

        // Expect an operand

        mScanner->getNextToken();

        QDomElement otherOperand = (this->*pFunction)(pDomNode);

        if (otherOperand.isNull())
            return QDomElement();

        // Update our DOM tree with our operator and operand
        // Note #1: the check against the previous operator only makes sense for
        //          n-ary operators, i.e. "plus", "times", "and", "or" and
        //          "xor"...
        // Note #2: see http://www.w3.org/TR/MathML2/chapter4.html#contm.funopqual
        //          for more information...

        static const CellmlTextViewScanner::TokenTypes nAryOperators = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::PlusToken
                                                                                                           << CellmlTextViewScanner::TimesToken
                                                                                                           << CellmlTextViewScanner::AndToken
                                                                                                           << CellmlTextViewScanner::OrToken
                                                                                                           << CellmlTextViewScanner::XorToken;

        if ((crtOperator == prevOperator) && nAryOperators.contains(crtOperator)) {
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

    return res;
}

//==============================================================================

QDomElement CellmlTextViewParser::parseNormalMathematicalExpression(QDomNode &pDomNode)
{
    // Look for "or"

    return parseMathematicalExpressionElement(pDomNode,
                                              CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::OrToken,
                                              &CellmlTextViewParser::parseNormalMathematicalExpression2);
}

//==============================================================================

QDomElement CellmlTextViewParser::parseNormalMathematicalExpression2(QDomNode &pDomNode)
{
    // Look for "and"

    return parseMathematicalExpressionElement(pDomNode,
                                              CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::AndToken,
                                              &CellmlTextViewParser::parseNormalMathematicalExpression3);
}

//==============================================================================

QDomElement CellmlTextViewParser::parseNormalMathematicalExpression3(QDomNode &pDomNode)
{
    // Look for "xor"

    return parseMathematicalExpressionElement(pDomNode,
                                              CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::XorToken,
                                              &CellmlTextViewParser::parseNormalMathematicalExpression4);
}

//==============================================================================

QDomElement CellmlTextViewParser::parseNormalMathematicalExpression4(QDomNode &pDomNode)
{
    // Look for "==" or "<>"

    return parseMathematicalExpressionElement(pDomNode,
                                              CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::EqEqToken
                                                                                  << CellmlTextViewScanner::NeqToken,
                                              &CellmlTextViewParser::parseNormalMathematicalExpression5);
}

//==============================================================================

QDomElement CellmlTextViewParser::parseNormalMathematicalExpression5(QDomNode &pDomNode)
{
    // Look for "<", ">", "<=" or ">="

    return parseMathematicalExpressionElement(pDomNode,
                                              CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::LtToken
                                                                                  << CellmlTextViewScanner::GtToken
                                                                                  << CellmlTextViewScanner::LeqToken
                                                                                  << CellmlTextViewScanner::GeqToken,
                                              &CellmlTextViewParser::parseNormalMathematicalExpression6);
}

//==============================================================================

QDomElement CellmlTextViewParser::parseNormalMathematicalExpression6(QDomNode &pDomNode)
{
    // Look for "+" or "-"

    return parseMathematicalExpressionElement(pDomNode,
                                              CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::PlusToken
                                                                                  << CellmlTextViewScanner::MinusToken,
                                              &CellmlTextViewParser::parseNormalMathematicalExpression7);
}

//==============================================================================

QDomElement CellmlTextViewParser::parseNormalMathematicalExpression7(QDomNode &pDomNode)
{
    // Look for "*" or "/"

    return parseMathematicalExpressionElement(pDomNode,
                                              CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::TimesToken
                                                                                  << CellmlTextViewScanner::DivideToken,
                                              &CellmlTextViewParser::parseNormalMathematicalExpression8);
}

//==============================================================================

QDomElement CellmlTextViewParser::parseNormalMathematicalExpression8(QDomNode &pDomNode)
{
    // Look for "not", unary "+" or unary "-"

    static const CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::NotToken
                                                                                                    << CellmlTextViewScanner::PlusToken
                                                                                                    << CellmlTextViewScanner::MinusToken;

    CellmlTextViewScanner::TokenType crtOperator = mScanner->tokenType();

    if (tokenTypes.contains(crtOperator)) {
        QDomElement operand;

        if (mScanner->tokenType() == CellmlTextViewScanner::NotToken) {
            mScanner->getNextToken();

            operand = parseNormalMathematicalExpression(pDomNode);
        } else {
            mScanner->getNextToken();

            operand = parseNormalMathematicalExpression9(pDomNode);
        }

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
//---GRY--- TO BE DONE...
    // Look for an identifier, "ode", a number, a mathematical function, an
    // opening bracket or a mathematical constant

    QDomElement res;

    if (mScanner->tokenType() == CellmlTextViewScanner::IdentifierToken) {
        // Create an identifier element

        res = newIdentifierElement(mScanner->tokenString());
    } else if (mScanner->tokenType() == CellmlTextViewScanner::OdeToken) {
        // Try to parse a derivative identifier

        res = parseDerivativeIdentifier(pDomNode);
    } else if (mScanner->tokenType() == CellmlTextViewScanner::NumberToken) {
        // 'Properly' parse the number and keep track of it
        // Note: this is useful to do in case the number is not valid (e.g. too
        //       big, too small)...

        numberToken(pDomNode);

        QString number = mScanner->tokenString();

        // Expect "{"

        mScanner->getNextToken();

        if (!openingCurlyBracketToken(pDomNode))
            return QDomElement();

        // Expect an identifier or an SI unit

        mScanner->getNextToken();

        if (!identifierOrSiUnitToken(pDomNode))
            return QDomElement();

        // Keep track of the unit for our number

        QString unit = mScanner->tokenString();

        // Expect "}"

        mScanner->getNextToken();

        if (!closingCurlyBracketToken(pDomNode))
            return QDomElement();

        // Create a number element

        res = newNumberElement(number, unit);
    } else {
        QString foundString = mScanner->tokenString();

        if (mScanner->tokenType() != CellmlTextViewScanner::EofToken)
            foundString = QString("'%1'").arg(foundString);

        addUnexpectedTokenErrorMessage("An identifier, 'ode' or a number", foundString);

        return QDomElement();
    }

    // Fetch the next token and return our result element

    mScanner->getNextToken();

    return res;
}

//==============================================================================

QDomElement CellmlTextViewParser::parsePiecewiseMathematicalExpression(QDomNode &pDomNode)
{
    // Create our piecewise element

    QDomElement piecewiseElement = newDomElement(mDomDocument, "piecewise");

    // Loop while we have "case" or "otherwise", or leave if we get "endsel"

    static const CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::CaseToken
                                                                                                    << CellmlTextViewScanner::OtherwiseToken
                                                                                                    << CellmlTextViewScanner::EndSelToken;

    bool hasOtherwiseClause = false;

    mScanner->getNextToken();

    while (tokenType(pDomNode, QObject::tr("'%1', '%2' or '%3'").arg("case", "otherwise", "endsel"),
                     tokenTypes)) {
        if (mScanner->tokenType() == CellmlTextViewScanner::EndSelToken)
            break;

        bool caseClause = mScanner->tokenType() == CellmlTextViewScanner::CaseToken;
        QDomElement conditionElement = QDomElement();

        if (caseClause) {
            // Expect a condition in the form of a normal mathematical equation

            mScanner->getNextToken();

            conditionElement = parseNormalMathematicalExpression(pDomNode);

            if (conditionElement.isNull())
                return QDomElement();
        } else if (hasOtherwiseClause) {
            mMessages << CellmlTextViewParserMessage(CellmlTextViewParserMessage::Error,
                                                     mScanner->tokenLine(),
                                                     mScanner->tokenColumn(),
                                                     QObject::tr("There can only be one 'otherwise' clause."));

            return QDomElement();
        } else {
            hasOtherwiseClause = true;

            // Fetch the next token

            mScanner->getNextToken();
        }

        // Expect ":"

        if (!colonToken(pDomNode))
            return QDomElement();

        // Expect an expression in the form of a normal mathematical equation

        mScanner->getNextToken();

        QDomElement expressionElement = parseNormalMathematicalExpression(pDomNode);

        if (expressionElement.isNull())
            return QDomElement();

        // Expect ";"

        if (!semiColonToken(pDomNode))
            return QDomElement();

        // Create and populate our piece/otherwise element, and add it to our
        // piecewise element

        QDomElement pieceOrOtherwiseElement = newDomElement(mDomDocument, caseClause?"piece":"otherwise");

        pieceOrOtherwiseElement.appendChild(expressionElement);

        if (caseClause)
            pieceOrOtherwiseElement.appendChild(conditionElement);

        piecewiseElement.appendChild(pieceOrOtherwiseElement);

        // Fetch the next token

        mScanner->getNextToken();
    }

    // Expect "endsel"

    if (!endselToken(pDomNode))
        return QDomElement();

    // Fetch the next token and return our piecewise element

    mScanner->getNextToken();

    return piecewiseElement;
}

//==============================================================================

void CellmlTextViewParser::moveTrailingComments(QDomNode &pFromDomNode,
                                                QDomNode &pToDomNode)
{
    // Move trailing comments, in any, that are in pFromDomNode to pToDomNode

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
