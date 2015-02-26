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

    if (defToken(mDomDocument)) {
        // Expect "model"

        mScanner->getNextToken();

        if (modelToken(mDomDocument)) {
            // Create our model element

            QDomElement modelElement = newDomElement(mDomDocument, "model");

            // Try to parse for a cmeta:id

            mScanner->getNextToken();

            parseCmetaId(modelElement);

            // Expect an identifier

            if (identifierToken(mDomDocument)) {
                // Set our model name

                modelElement.setAttribute("name", mScanner->tokenString());

                // Expect "as"

                mScanner->getNextToken();

                if (asToken(mDomDocument)) {
                    // Try to parse for some model definition itself

                    mScanner->getNextToken();

                    if (parseModelDefinition(modelElement)) {
                        // Expect "enddef;"

                        if (enddefPlusSemiColonToken(modelElement)) {
                            // Expect the end of the file

                            mScanner->getNextToken();

                            if (tokenType(modelElement, QObject::tr("the end of the file"),
                                          CellmlTextViewScanner::EofToken)) {
                                // We are done, so add some processing
                                // instruction to our DOM document

                                mDomDocument.insertBefore(mDomDocument.createProcessingInstruction("xml", "version='1.0'"),
                                                          mDomDocument.documentElement());

                                // Use the given CellML version if it is higher
                                // than the one we actually need

                                mCellmlVersion = (pCellmlVersion > mCellmlVersion)?pCellmlVersion:mCellmlVersion;

                                // Next, add the CellML namespace to our
                                // document element

                                switch (mCellmlVersion) {
                                case CellMLSupport::CellmlFile::Cellml_1_1:
                                    mDomDocument.documentElement().setAttribute("xmlns", CellMLSupport::Cellml_1_1_Namespace);
                                    mDomDocument.documentElement().setAttribute("xmlns:cellml", CellMLSupport::Cellml_1_1_Namespace);

                                    break;
                                default:
                                    // CellMLSupport::CellmlFile::Cellml_1_0

                                    mDomDocument.documentElement().setAttribute("xmlns", CellMLSupport::Cellml_1_0_Namespace);
                                    mDomDocument.documentElement().setAttribute("xmlns:cellml", CellMLSupport::Cellml_1_0_Namespace);
                                }

                                // Finally, add the other namespaces that we
                                // need
                                // Note: ideally, we wouldn't have to do this,
                                //       but this would mean using the NS
                                //       version of various methods (e.g.
                                //       setAttributeNS() rather than
                                //       setAttribute()). However, this results
                                //       in namespace information being
                                //       referenced all over the place, which is
                                //       really not what we want since that
                                //       unnecessarily pollutes things...

                                foreach (const QString &key, mNamespaces.keys())
                                    mDomDocument.documentElement().setAttribute(QString("xmlns:%1").arg(key), mNamespaces.value(key));

                                return true;
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
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
    } else {
        // This is not the token we were expecting, so let the user know about
        // it

        QString foundString = mScanner->tokenString();

        if (mScanner->tokenType() != CellmlTextViewScanner::EofToken)
            foundString = QString("'%1'").arg(foundString);

        mMessages << CellmlTextViewParserMessage(CellmlTextViewParserMessage::Error,
                                                 mScanner->tokenLine(),
                                                 mScanner->tokenColumn(),
                                                 QObject::tr("%1 is expected, but %2 was found instead.").arg(Core::formatMessage(pExpectedString, false),
                                                                                                              foundString));
    }

    return false;
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

bool CellmlTextViewParser::enddefPlusSemiColonToken(QDomNode &pDomNode)
{
    // Expect "enddef"

    if (tokenType(pDomNode, "'enddef'",
                  CellmlTextViewScanner::EndDefToken)) {
        // Expect ";"

        mScanner->getNextToken();

        return semiColonToken(pDomNode);
    } else {
        return false;
    }
}

//==============================================================================

bool CellmlTextViewParser::forToken(QDomNode &pDomNode)
{
    // Expect "for"

    return tokenType(pDomNode, "'for'",
                     CellmlTextViewScanner::ForToken);
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

        mScanner->getNextToken();
    }
}

//==============================================================================

void CellmlTextViewParser::parseCmetaId(QDomElement &pDomElement)
{
    // Check whether a cmeta:id is given by checking whether the token is "{"

    QString cmetaId = QString();

    if (isTokenType(pDomElement, CellmlTextViewScanner::OpeningCurlyBracketToken)) {
        // Expect an identifier

        mScanner->getNextToken();

        if (identifierToken(pDomElement)) {
            // The identifier is our cmeta:id

            cmetaId = mScanner->tokenString();

            // Expect "}"

            mScanner->getNextToken();

            if (closingCurlyBracketToken(pDomElement))
                mScanner->getNextToken();
        }
    }

    // Set the cmeta:id of the current DOM element

    if (!cmetaId.isEmpty()) {
        mNamespaces.insert("cmeta", CellMLSupport::CmetaIdNamespace);

        pDomElement.setAttribute("cmeta:id", cmetaId);
    }
}

//==============================================================================

bool CellmlTextViewParser::parseModelDefinition(QDomNode &pDomNode)
{
    // Loop while we have "def" and leave if we get "enddef"

    static CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::DefToken
                                                                                              << CellmlTextViewScanner::EndDefToken;

    while (tokenType(pDomNode, QObject::tr("'%1' or '%2'").arg("def", "enddef"),
                     tokenTypes)) {
        switch (mScanner->tokenType()) {
        case CellmlTextViewScanner::DefToken: {
            // Expect a model definition

            static CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::ImportToken
                                                                                                      << CellmlTextViewScanner::UnitToken
                                                                                                      << CellmlTextViewScanner::CompToken
                                                                                                      << CellmlTextViewScanner::GroupToken
                                                                                                      << CellmlTextViewScanner::MapToken;

            mScanner->getNextToken();

            bool baseUnitsDefinition = false;
            QDomElement domElement;

            if (tokenType(pDomNode, QObject::tr("'%1', '%2', '%3', '%4' or '%5'").arg("import", "unit", "comp", "group", "map"),
                          tokenTypes)) {
                switch (mScanner->tokenType()) {
                case CellmlTextViewScanner::ImportToken:
                    domElement = parseImportDefinition(pDomNode);

                    break;
                case CellmlTextViewScanner::UnitToken:
                    domElement = parseUnitsDefinition(pDomNode, baseUnitsDefinition);

                    break;
                case CellmlTextViewScanner::CompToken:
//---GRY--- TO BE DONE...

                    break;
                case CellmlTextViewScanner::GroupToken:
//---GRY--- TO BE DONE...

                    break;
                default:
                    // CellmlTextViewScanner::MapToken

                    domElement = parseMapDefinition(pDomNode);
                }
            } else {
                return false;
            }

            if (!domElement.isNull()) {
                // Expect ";" or "enddef;"

                if (   ( baseUnitsDefinition && semiColonToken(pDomNode))
                    || (!baseUnitsDefinition && enddefPlusSemiColonToken(domElement))) {
                    mScanner->getNextToken();
                } else {
                    return false;
                }
            } else {
                return false;
            }

            break;
        }
        default:
            // CellmlTextViewScanner::EndDefToken

            return true;
        }
    }

    return false;
}

//==============================================================================

QDomElement CellmlTextViewParser::parseImportDefinition(QDomNode &pDomNode)
{
    // Create our import element

    QDomElement importElement = newDomElement(pDomNode, "import");

    // Try to parse for a cmeta:id

    mScanner->getNextToken();

    parseCmetaId(importElement);

    // Expect "using"

    if (usingToken(importElement)) {
        // Expect a string representing a URL

        mScanner->getNextToken();

        if (tokenType(importElement, QObject::tr("A string representing a URL"),
                      CellmlTextViewScanner::StringToken)) {
            // Set the URL, after having kept track of the fact that we need
            // CellML 1.1 and the XLink namespace

            mCellmlVersion = CellMLSupport::CellmlFile::Cellml_1_1;

            mNamespaces.insert("xlink", CellMLSupport::XlinkNamespace);

            importElement.setAttribute("xlink:href", mScanner->tokenString());

            // Expect "for"

            mScanner->getNextToken();

            if (forToken(importElement)) {
                // Expect an import definition, so loop while we have "unit" or
                // "comp", or leave if we get "enddef"

                static CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::UnitToken
                                                                                                          << CellmlTextViewScanner::CompToken
                                                                                                          << CellmlTextViewScanner::EndDefToken;

                mScanner->getNextToken();

                while (tokenType(importElement, QObject::tr("'%1', '%2' or '%3'").arg("unit", "comp", "enddef"),
                                 tokenTypes)) {
                    switch (mScanner->tokenType()) {
                    case CellmlTextViewScanner::UnitToken: {
                        // We are dealing with a unit import, so create our unit
                        // import element

                        QDomElement unitsImportElement = newDomElement(importElement, "units");

                        // Try to parse for a cmeta:id

                        mScanner->getNextToken();

                        parseCmetaId(unitsImportElement);

                        // Expect an identifier

                        if (identifierToken(unitsImportElement)) {
                            // Set the name of the unit

                            unitsImportElement.setAttribute("name", mScanner->tokenString());

                            // Expect "using"

                            mScanner->getNextToken();

                            if (usingToken(unitsImportElement)) {
                                // Expect "unit"

                                mScanner->getNextToken();

                                if (unitToken(unitsImportElement)) {
                                    // Expect an identifier

                                    mScanner->getNextToken();

                                    if (identifierToken(unitsImportElement)) {
                                        // Set the name of the imported unit

                                        unitsImportElement.setAttribute("units_ref", mScanner->tokenString());

                                        // Expect ";"

                                        mScanner->getNextToken();

                                        if (semiColonToken(unitsImportElement))
                                            mScanner->getNextToken();
                                    }
                                }
                            }
                        }

                        break;
                    }
                    case CellmlTextViewScanner::CompToken: {
                        // We are dealing with a component import, so create our
                        // component import element

                        QDomElement componentImportElement = newDomElement(importElement, "component");

                        // Try to parse for a cmeta:id

                        mScanner->getNextToken();

                        parseCmetaId(componentImportElement);

                        // Expect an identifier

                        if (identifierToken(componentImportElement)) {
                            // Set the name of the component

                            componentImportElement.setAttribute("name", mScanner->tokenString());

                            // Expect "using"

                            mScanner->getNextToken();

                            if (usingToken(componentImportElement)) {
                                // Expect "comp"

                                mScanner->getNextToken();

                                if (compToken(componentImportElement)) {
                                    // Expect an identifier

                                    mScanner->getNextToken();

                                    if (identifierToken(componentImportElement)) {
                                        // Set the name of the imported
                                        // component

                                        componentImportElement.setAttribute("component_ref", mScanner->tokenString());

                                        // Expect ";"

                                        mScanner->getNextToken();

                                        if (semiColonToken(componentImportElement))
                                            mScanner->getNextToken();
                                    }
                                }
                            }
                        }

                        break;
                    }
                    default:
                        // CellmlTextViewScanner::EndDefToken

                        return importElement;
                    }
                }
            }
        }
    }

    return QDomElement();
}

//==============================================================================

QDomElement CellmlTextViewParser::parseUnitsDefinition(QDomNode &pDomNode,
                                                       bool &pBaseUnitsDefinition)
{
    // Create our units element

    QDomElement unitsElement = newDomElement(pDomNode, "units");

    // Try to parse for a cmeta:id

    mScanner->getNextToken();

    parseCmetaId(unitsElement);

    // Expect an identifier

    if (identifierToken(unitsElement)) {
        // Set our unit's name

        unitsElement.setAttribute("name", mScanner->tokenString());

        // Expect "as"

        mScanner->getNextToken();

        if (asToken(unitsElement)) {
            // Expect a unit definition

            static CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::BaseToken
                                                                                                      << CellmlTextViewScanner::UnitToken
                                                                                                      << CellmlTextViewScanner::EndDefToken;

            mScanner->getNextToken();

            if (tokenType(unitsElement, QObject::tr("'%1', '%2' or '%3'").arg("base", "unit", "enddef"),
                          tokenTypes)) {
                switch (mScanner->tokenType()) {
                case CellmlTextViewScanner::BaseToken:
                    // We are dealing with the definition of a base unit, so now
                    // expect "unit"

                    mScanner->getNextToken();

                    if (unitToken(unitsElement)) {
                        // Make our unit a base unit

                        pBaseUnitsDefinition = true;

                        unitsElement.setAttribute("base_units", "yes");

                        mScanner->getNextToken();

                        return unitsElement;
                    }

                    break;
                case CellmlTextViewScanner::UnitToken: {
                    // We are dealing with a 'normal' unit definition, so loop
                    // while we have "unit" or leave if we get "enddef"

                    static CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::UnitToken
                                                                                                              << CellmlTextViewScanner::EndDefToken;

                    while (tokenType(pDomNode, QObject::tr("'%1' or '%2'").arg("unit", "enddef"),
                                     tokenTypes)) {
                        switch (mScanner->tokenType()) {
                        case CellmlTextViewScanner::UnitToken:
                            if (!parseUnitDefinition(unitsElement))
                                return QDomElement();

                            break;
                        default:
                            // CellmlTextViewScanner::EndDefToken

                            return unitsElement;
                        }
                    }

                    return QDomElement();
                }
                default:
                    // CellmlTextViewScanner::EndDefToken

                    return unitsElement;
                }
            }
        }
    }

    return QDomElement();
}

//==============================================================================

bool CellmlTextViewParser::parseUnitDefinition(QDomNode &pDomNode)
{
    // Create our unit element

    QDomElement unitElement = newDomElement(pDomNode, "unit");

    // Try to parse for a cmeta:id

    mScanner->getNextToken();

    parseCmetaId(unitElement);

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

    if (tokenType(unitElement, QObject::tr("An identifier or an SI unit (e.g. 'second')"),
                  tokenTypes)) {
        // Set our unit's name

        unitElement.setAttribute("units", mScanner->tokenString());

        // Expect "{" or ";"

        static CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::OpeningCurlyBracketToken
                                                                                                  << CellmlTextViewScanner::SemiColonToken;

        mScanner->getNextToken();

        if (tokenType(pDomNode, QObject::tr("'%1' or '%2'").arg("{", ";"),
                      tokenTypes)) {
            switch (mScanner->tokenType()) {
            case CellmlTextViewScanner::OpeningCurlyBracketToken: {
                QList<CellmlTextViewScanner::TokenType> unitAttributesDefined = QList<CellmlTextViewScanner::TokenType>();

                do {
                    // Expect "pref", "expo", "mult" or "off"

                    static CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::PrefToken
                                                                                                              << CellmlTextViewScanner::ExpoToken
                                                                                                              << CellmlTextViewScanner::MultToken
                                                                                                              << CellmlTextViewScanner::OffToken;

                    mScanner->getNextToken();

                    if (tokenType(unitElement, QObject::tr("'%1', '%2', '%3' or '%4'").arg("pref", "expo", "mult", "off"),
                                  tokenTypes)) {
                        // Make sure that we don't already have come across the
                        // unit attribute

                        if (unitAttributesDefined.contains(mScanner->tokenType())) {
                            mMessages << CellmlTextViewParserMessage(CellmlTextViewParserMessage::Error,
                                                                     mScanner->tokenLine(),
                                                                     mScanner->tokenColumn(),
                                                                     QObject::tr("The '%1' attribute has already been specified.").arg(mScanner->tokenString()));

                            return false;
                        } else {
                            // Keep track of the fact that we have come across
                            // the unit attribute

                            unitAttributesDefined << mScanner->tokenType();

                            CellmlTextViewScanner::TokenType unitAttributeTokenType = mScanner->tokenType();

                            // Expect ":"

                            mScanner->getNextToken();

                            if (!colonToken(unitElement))
                                return false;

                            // Check which unit attribute we are dealing with to
                            // determine what to expect next

                            mScanner->getNextToken();

                            int sign;

                            if (unitAttributeTokenType == CellmlTextViewScanner::PrefToken) {
                                // Check whether we have "+" or "-"

                                if (   isTokenType(pDomNode, CellmlTextViewScanner::PlusToken)
                                    || isTokenType(pDomNode, CellmlTextViewScanner::MinusToken)) {
                                    // We are dealing with a number value

                                    if (!numberValueToken(pDomNode, sign))
                                        return false;
                                } else {
                                    // We are not dealing with a 'proper' number
                                    // value, but either a number or a prefix

                                    sign = 0;

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

                            switch (unitAttributeTokenType) {
                            case CellmlTextViewScanner::PrefToken:
                                unitElement.setAttribute("prefix", unitAttributeValue);

                                break;
                            case CellmlTextViewScanner::ExpoToken:
                                unitElement.setAttribute("exponent", unitAttributeValue);

                                break;
                            case CellmlTextViewScanner::MultToken:
                                unitElement.setAttribute("multiplier", unitAttributeValue);

                                break;
                            default:
                                // CellmlTextViewScanner::OffToken

                                unitElement.setAttribute("offset", unitAttributeValue);
                            }
                        }
                    } else {
                        return false;
                    }

                    mScanner->getNextToken();
                } while (isTokenType(pDomNode, CellmlTextViewScanner::CommaToken));

                // Expect "}"

                if (!closingCurlyBracketToken(unitElement))
                    return false;

                // Expect ";"

                mScanner->getNextToken();

                if (semiColonToken(unitElement)) {
                    mScanner->getNextToken();

                    return true;
                }

                break;
            }
            default:
                // CellmlTextViewScanner::SemiColonToken

                mScanner->getNextToken();

                return true;
            }
        }
    }

    return false;
}

//==============================================================================

QDomElement CellmlTextViewParser::parseMapDefinition(QDomNode &pDomNode)
{
    // Create our connection element

    QDomElement connectionElement = newDomElement(pDomNode, "connection");

    // Try to parse for a cmeta:id

    mScanner->getNextToken();

    parseCmetaId(connectionElement);

    // Expect "between"

    if (betweenToken(connectionElement)) {
        // Expect an identifier

        mScanner->getNextToken();

        if (identifierToken(connectionElement)) {
            // Keep track of the first component

            QString component1 = mScanner->tokenString();

            // Expect "and"

            mScanner->getNextToken();

            if (andToken(connectionElement)) {
                // Expect an identifier

                mScanner->getNextToken();

                if (identifierToken(connectionElement)) {
                    // Keep track of the second component

                    QString component2 = mScanner->tokenString();

                    // Expect "for"

                    mScanner->getNextToken();

                    if (forToken(connectionElement)) {
                        // Create our map components element and set its two
                        // component attributes

                        QDomElement mapComponentsElement = newDomElement(connectionElement, "map_components");

                        mapComponentsElement.setAttribute("component_1", component1);
                        mapComponentsElement.setAttribute("component_2", component2);

                        // Expect a mapping, so loop while we have "vars" or
                        // leave if we get "enddef"

                        static CellmlTextViewScanner::TokenTypes tokenTypes = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::VarsToken
                                                                                                                  << CellmlTextViewScanner::EndDefToken;

                        mScanner->getNextToken();

                        while (tokenType(connectionElement, QObject::tr("'%1' or '%2'").arg("vars", "enddef"),
                                         tokenTypes)) {
                            switch (mScanner->tokenType()) {
                            case CellmlTextViewScanner::VarsToken: {
                                // Create our map variables element

                                QDomElement mapVariablesElement = newDomElement(connectionElement, "map_variables");

                                // Try to parse for a cmeta:id

                                mScanner->getNextToken();

                                parseCmetaId(mapVariablesElement);

                                // Expect an identifier

                                if (identifierToken(mapVariablesElement)) {
                                    // Set the name of the first variable

                                    mapVariablesElement.setAttribute("variable_1", mScanner->tokenString());

                                    // Expect "and"

                                    mScanner->getNextToken();

                                    if (andToken(mapVariablesElement)) {
                                        // Expect an identifier

                                        mScanner->getNextToken();

                                        if (identifierToken(mapVariablesElement)) {
                                            // Set the name of the second
                                            // variable

                                            mapVariablesElement.setAttribute("variable_2", mScanner->tokenString());

                                            // Expect ";"

                                            mScanner->getNextToken();

                                            if (semiColonToken(mapVariablesElement))
                                                mScanner->getNextToken();
                                        }
                                    }
                                }

                                break;
                            }
                            default:
                                // CellmlTextViewScanner::EndDefToken

                                return connectionElement;
                            }
                        }
                    }
                }
            }
        }
    }

    return QDomElement();
}

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
