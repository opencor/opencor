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

CellmlTextViewParserMessage::CellmlTextViewParserMessage(const bool &pError,
                                                         const int &pLine,
                                                         const int &pColumn,
                                                         const QString &pMessage) :
    mError(pError),
    mLine(pLine),
    mColumn(pColumn),
    mMessage(pMessage)
{
}

//==============================================================================

bool CellmlTextViewParserMessage::isError() const
{
    // Return whether we are an error

    return mError;
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
    mDomDocument(QDomDocument()),
    mDomNode(QDomNode()),
    mDomElement(QDomElement()),
    mMessages(CellmlTextViewParserMessages())
{
}

//==============================================================================

CellmlTextViewParser::~CellmlTextViewParser()
{
    // Delete some internal objects

    delete mScanner;
}

//==============================================================================

bool CellmlTextViewParser::execute(const QString &pText)
{
    // Get ready for the parsing

    mScanner->setText(pText);

    mDomDocument = QDomDocument(QString());
    mDomNode = mDomDocument;
    mDomElement = QDomElement();

    mMessages = CellmlTextViewParserMessages();

    // Expect "def"

    if (defToken()) {
        // Expect "model"

        mScanner->getNextToken();

        if (modelToken()) {
            // Try to parse for a cmeta:id

            mScanner->getNextToken();

            QString cmetaId = parseCmetaId();

            // Expect an identifier

            if (identifierToken()) {
                // Create our model node

                mDomNode = mDomElement = newDomElement("model");

                mDomElement.setAttribute("name", mScanner->tokenString());

                if (!cmetaId.isEmpty())
                    mDomElement.setAttribute("cmeta:id", cmetaId);

                // Expect "as"

                mScanner->getNextToken();

                if (asToken()) {
                    // Expect "enddef;"

                    mScanner->getNextToken();

                    if (enddefPlusSemiColonToken()) {
                        // Expect EOF

                        mScanner->getNextToken();

                        return tokenType("the end of the file", CellmlTextViewScanner::EofToken);
                    }
                }
            }
        }
    }

    return false;
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

QDomElement CellmlTextViewParser::newDomElement(const QString &pElementName)
{
    // Create a new DOM element with the given name and make it our new mDomNode

    QDomElement domElement = mDomDocument.createElement(pElementName);

    mDomNode.appendChild(domElement);

    return domElement;
}

//==============================================================================

bool CellmlTextViewParser::tokenType(const QString &pExpectedString,
                                     const CellmlTextViewScanner::TokenType &pTokenType)
{
    // Look for and handle comments, if any

//---GRY--- TO BE DONE...

    // Check whether the current token type is the one we are after

    if (mScanner->tokenType() == pTokenType) {
        // We have the correct token, so check whether a comment exists and, if
        // so, generate a warning for it

        if (!mScanner->tokenComment().isEmpty()) {
            mMessages << CellmlTextViewParserMessage(false,
                                                     mScanner->tokenLine(),
                                                     mScanner->tokenColumn(),
                                                     mScanner->tokenComment());
        }

        return true;
    } else if (mScanner->tokenType() == CellmlTextViewScanner::InvalidToken) {
        // This is the token we were expecting, but it is invalid

        mMessages << CellmlTextViewParserMessage(true,
                                                 mScanner->tokenLine(),
                                                 mScanner->tokenColumn(),
                                                 mScanner->tokenComment());
    } else {
        // This is not the token we were expecting, so let the user know about
        // it

        QString foundString = mScanner->tokenString();

        if (mScanner->tokenType() != CellmlTextViewScanner::EofToken)
            foundString = QString("'%1'").arg(foundString);

        mMessages << CellmlTextViewParserMessage(true,
                                                 mScanner->tokenLine(),
                                                 mScanner->tokenColumn(),
                                                 QObject::tr("%1 is expected, but %2 was found instead.").arg(Core::formatMessage(pExpectedString, false),
                                                                                                              foundString));
    }

    return false;
}

//==============================================================================

bool CellmlTextViewParser::asToken()
{
    // Expect "as"

    return tokenType("'as'", CellmlTextViewScanner::AsToken);
}

//==============================================================================

bool CellmlTextViewParser::closingCurlyBracketToken()
{
    // Expect "}"

    return tokenType("'}'", CellmlTextViewScanner::ClosingCurlyBracketToken);
}

//==============================================================================

bool CellmlTextViewParser::defToken()
{
    // Expect "def"

    return tokenType("'def'", CellmlTextViewScanner::DefToken);
}

//==============================================================================

bool CellmlTextViewParser::enddefPlusSemiColonToken()
{
    // Expect "enddef"

    if (tokenType("'enddef'", CellmlTextViewScanner::EndDefToken)) {
        // Expect ";"

        mScanner->getNextToken();

        return semiColonToken();
    } else {
        return false;
    }
}

//==============================================================================

bool CellmlTextViewParser::identifierToken()
{
    // Expect an identifier

    return tokenType("An identifier", CellmlTextViewScanner::IdentifierToken);
}

//==============================================================================

bool CellmlTextViewParser::modelToken()
{
    // Expect "model"

    return tokenType("'model'", CellmlTextViewScanner::ModelToken);
}

//==============================================================================

bool CellmlTextViewParser::openingCurlyBracketToken()
{
    // Expect "{"

    return tokenType("'{'", CellmlTextViewScanner::OpeningCurlyBracketToken);
}

//==============================================================================

bool CellmlTextViewParser::semiColonToken()
{
    // Expect ";"

    return tokenType("';'", CellmlTextViewScanner::SemiColonToken);
}

//==============================================================================

QString CellmlTextViewParser::parseCmetaId()
{
    // Check whether a cmeta:id is given

    QString res = QString();

    // Check whether the next token is "{"

    if (mScanner->tokenType() == CellmlTextViewScanner::OpeningCurlyBracketToken) {
        // Expect an identifier

        mScanner->getNextToken();

        if (identifierToken()) {
            // The identifier is our cmeta:id

            res = mScanner->tokenString();

            // Expect "}"

            mScanner->getNextToken();

            if (closingCurlyBracketToken())
                mScanner->getNextToken();
        }
    }

    return res;
}

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
