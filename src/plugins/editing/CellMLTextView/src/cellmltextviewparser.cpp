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

bool CellmlTextViewParser::asToken(QDomNode &pDomNode)
{
    // Expect "as"

    return tokenType(pDomNode, "'as'", CellmlTextViewScanner::AsToken);
}

//==============================================================================

bool CellmlTextViewParser::closingCurlyBracketToken(QDomNode &pDomNode)
{
    // Expect "}"

    return tokenType(pDomNode, "'}'", CellmlTextViewScanner::ClosingCurlyBracketToken);
}

//==============================================================================

bool CellmlTextViewParser::defToken(QDomNode &pDomNode)
{
    // Expect "def"

    return tokenType(pDomNode, "'def'", CellmlTextViewScanner::DefToken);
}

//==============================================================================

bool CellmlTextViewParser::enddefPlusSemiColonToken(QDomNode &pDomNode)
{
    // Expect "enddef"

    if (tokenType(pDomNode, "'enddef'", CellmlTextViewScanner::EndDefToken)) {
        // Expect ";"

        mScanner->getNextToken();

        return semiColonToken(pDomNode);
    } else {
        return false;
    }
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

bool CellmlTextViewParser::unitIdentifierToken(QDomNode &pDomNode)
{
    // Expect an identifier or an SI unit

    static CellmlTextViewScanner::TokenTypes unitIdentifierTokens = CellmlTextViewScanner::TokenTypes() << CellmlTextViewScanner::IdentifierToken;
    static bool needInitializeUnitIdentifierTokens = true;

    if (needInitializeUnitIdentifierTokens) {
        for (CellmlTextViewScanner::TokenType unitIdentifierToken = CellmlTextViewScanner::FirstUnitToken;
             unitIdentifierToken <= CellmlTextViewScanner::LastUnitToken;
             unitIdentifierToken = CellmlTextViewScanner::TokenType(int(unitIdentifierToken)+1)) {
            unitIdentifierTokens << unitIdentifierToken;
        }

        needInitializeUnitIdentifierTokens = false;
    }

    return tokenType(pDomNode, QObject::tr("A unit identifier"),
                     unitIdentifierTokens);
}

//==============================================================================

bool CellmlTextViewParser::unitToken(QDomNode &pDomNode)
{
    // Expect "unit"

    return tokenType(pDomNode, "'unit'",
                     CellmlTextViewScanner::UnitToken);
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
    // Loop while we don't have "enddef" or EOF

    while (   !isTokenType(pDomNode, CellmlTextViewScanner::EndDefToken)
           && !isTokenType(pDomNode, CellmlTextViewScanner::EofToken)) {
        // Expect "def"

        if (defToken(pDomNode)) {
            // Expect "import", "unit", "comp", "group" or "map"

            mScanner->getNextToken();

            bool baseUnitsDefinition = false;
            QDomElement domElement;

            if (isTokenType(pDomNode, CellmlTextViewScanner::ImportToken)) {
//---GRY--- TO BE DONE...
            } else if (isTokenType(pDomNode, CellmlTextViewScanner::UnitToken)) {
                domElement = parseUnitsDefinition(pDomNode, baseUnitsDefinition);
            } else if (isTokenType(pDomNode, CellmlTextViewScanner::CompToken)) {
//---GRY--- TO BE DONE...
            } else if (isTokenType(pDomNode, CellmlTextViewScanner::GroupToken)) {
//---GRY--- TO BE DONE...
            } else if (isTokenType(pDomNode, CellmlTextViewScanner::MapToken)) {
//---GRY--- TO BE DONE...
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
        } else {
            return false;
        }
    }

    return true;
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
            // Check whether we have "base"

            mScanner->getNextToken();

            if (isTokenType(unitsElement, CellmlTextViewScanner::BaseToken)) {
                // We are dealing with the definition of base unit, so expect
                // "unit"

                mScanner->getNextToken();

                if (unitToken(unitsElement)) {
                    // Make our unit a base unit

                    pBaseUnitsDefinition = true;

                    unitsElement.setAttribute("base_units", "yes");

                    mScanner->getNextToken();

                    return unitsElement;
                }
            } else {
                // We are not dealing with the definition of a base unit, so
                // expect "unit" and then loop while we have "unit"

                if (unitToken(unitsElement)) {
                    do {
                        if (!parseUnitDefinition(unitsElement))
                            return QDomElement();
                    } while (isTokenType(unitsElement, CellmlTextViewScanner::UnitToken));

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

    // Expect a unit identifier

    if (unitIdentifierToken(unitElement)) {
        // Set our unit's name

        unitElement.setAttribute("units", mScanner->tokenString());

        // Expect ";"

        if (semiColonToken(unitElement)) {
            mScanner->getNextToken();

            return true;
        }
    }

    return false;
}

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
