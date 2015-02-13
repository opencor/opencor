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

#include <QDomDocument>
#include <QList>
#include <QString>

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

class CellmlTextViewScanner;

//==============================================================================

class CellmlTextViewParserError
{
public:
    explicit CellmlTextViewParserError(const int &pLine, const int &pColumn,
                                       const QString &pMessage);

    int line() const;
    int column() const;

    QString message() const;

private:
    int mLine;
    int mColumn;

    QString mMessage;
};

//==============================================================================

typedef QList<CellmlTextViewParserError> CellmlTextViewParserErrors;

//==============================================================================

class CellmlTextViewParser
{
public:
    explicit CellmlTextViewParser();
    ~CellmlTextViewParser();

    bool execute(const QString &pText);

    QDomDocument domDocument() const;

    CellmlTextViewParserErrors errors() const;

private:
    CellmlTextViewScanner *mScanner;

    QDomDocument mDomDocument;

    CellmlTextViewParserErrors mErrors;

    void addError(const QString &pExpected);
};

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
