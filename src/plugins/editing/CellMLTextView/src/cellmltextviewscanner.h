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
// Scanner for the CellML text format
//==============================================================================

#ifndef CELLMLTEXTVIEWSCANNER_H
#define CELLMLTEXTVIEWSCANNER_H

//==============================================================================

#include <QString>

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

class CellmlTextViewScannerToken
{
public:
    enum Symbol {
        Unknown,
        Def
    };

    explicit CellmlTextViewScannerToken();

    Symbol symbol() const;

    int line() const;
    int column() const;

    QString string() const;

private:
    Symbol mSymbol;

    int mLine;
    int mColumn;

    QString mString;
};

//==============================================================================

class CellmlTextViewScanner
{
public:
    explicit CellmlTextViewScanner();

    void setText(const QString &pText);

    CellmlTextViewScannerToken token() const;

private:
    QString mText;

    CellmlTextViewScannerToken mToken;
};

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
