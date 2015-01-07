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
// CellML to pretty CellML converter
//==============================================================================

#include "corecliutils.h"
#include "prettycellmlviewcellmltoprettycellmlconverter.h"

//==============================================================================

#include <QDomDocument>

//==============================================================================

namespace OpenCOR {
namespace PrettyCellMLView {

//==============================================================================

PrettyCellMLViewCellmlToPrettyCellmlConverter::PrettyCellMLViewCellmlToPrettyCellmlConverter(const QString &pFileName) :
    mFileName(pFileName),
    mOutput(QString()),
    mIndent(QString()),
    mErrorLine(-1),
    mErrorColumn(-1),
    mErrorMessage(QString())
{
}

//==============================================================================

bool PrettyCellMLViewCellmlToPrettyCellmlConverter::execute()
{
    // Convert the CellML file to pretty CellML by first getting a DOM
    // representation of the file

    QString fileContents;
    QDomDocument domDocument;

    Core::readTextFromFile(mFileName, fileContents);

    if (domDocument.setContent(fileContents, &mErrorMessage, &mErrorLine, &mErrorColumn)) {
        mOutput = QString();

        processNode(domDocument.documentElement());

        return true;
    } else {
        mOutput = fileContents;

        return false;
    }
}

//==============================================================================

QString PrettyCellMLViewCellmlToPrettyCellmlConverter::output() const
{
    // Return our output

    return mOutput;
}

//==============================================================================

int PrettyCellMLViewCellmlToPrettyCellmlConverter::errorLine() const
{
    // Return our error line

    return mErrorLine;
}

//==============================================================================

int PrettyCellMLViewCellmlToPrettyCellmlConverter::errorColumn() const
{
    // Return our error column

    return mErrorColumn;
}

//==============================================================================

QString PrettyCellMLViewCellmlToPrettyCellmlConverter::errorMessage() const
{
    // Return our error message

    return mErrorMessage;
}

//==============================================================================

static const auto Indent = QStringLiteral("    ");

//==============================================================================

void PrettyCellMLViewCellmlToPrettyCellmlConverter::indent()
{
    // Indent our output

    mIndent += Indent;
}

//==============================================================================

void PrettyCellMLViewCellmlToPrettyCellmlConverter::unindent()
{
    // Unindent our output

    mIndent.chop(Indent.size());
}

//==============================================================================

void PrettyCellMLViewCellmlToPrettyCellmlConverter::outputString(const QString &pString)
{
    // Output the given string

    if (pString.isEmpty()) {
        if (!mOutput.endsWith(Core::eolString()+Core::eolString()))
            mOutput += Core::eolString();
    } else {
        mOutput += mIndent+pString+Core::eolString();
    }
}

//==============================================================================

static const auto CmetaId = QStringLiteral("cmeta:id");

//==============================================================================

QString PrettyCellMLViewCellmlToPrettyCellmlConverter::cmetaId(const QDomNode &pDomNode) const
{
    // Return the cmeta:id, if any, of the given node

    if (pDomNode.attributes().contains(CmetaId))
        return QString("{%1}").arg(pDomNode.attributes().namedItem(CmetaId).nodeValue());
    else
        return QString();
}

//==============================================================================

void PrettyCellMLViewCellmlToPrettyCellmlConverter::processModelNode(const QDomNode &pDomNode)
{
    // Start processing the given model node

    outputString(QString("def model%1 %2 as").arg(cmetaId(pDomNode),
                                                  pDomNode.attributes().namedItem("name").nodeValue()));

    indent();

    // Process the given model node's children

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling())
        processNode(domNode);

    // Finish processing the given model node

    unindent();

    outputString("enddef;");
}

//==============================================================================

void PrettyCellMLViewCellmlToPrettyCellmlConverter::processUnitsNode(const QDomNode &pDomNode)
{
    // Start processing the given units node

    if (mOutput.endsWith(";"+Core::eolString()))
        outputString();

    outputString(QString("def unit%1 %2 as").arg(cmetaId(pDomNode),
                                                 pDomNode.attributes().namedItem("name").nodeValue()));

    indent();

    // Process the given units node's children

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling())
        processNode(domNode);

    // Finish processing the given units node

    unindent();

    outputString("enddef;");
}

//==============================================================================

void PrettyCellMLViewCellmlToPrettyCellmlConverter::processComponentNode(const QDomNode &pDomNode)
{
    // Start processing the given component node

    if (mOutput.endsWith(";"+Core::eolString()))
        outputString();

    outputString(QString("def comp%1 %2 as").arg(cmetaId(pDomNode),
                                                 pDomNode.attributes().namedItem("name").nodeValue()));

    indent();

    // Process the given component node's children

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling())
        processNode(domNode);

    // Finish processing the given component node

    unindent();

    outputString("enddef;");
}

//==============================================================================

void PrettyCellMLViewCellmlToPrettyCellmlConverter::processNode(const QDomNode &pDomNode)
{
    // Start processing the given node

    QString nodeName = pDomNode.nodeName();

    if (!nodeName.compare("model"))
        processModelNode(pDomNode);
    else if (!nodeName.compare("units"))
        processUnitsNode(pDomNode);
    else if (!nodeName.compare("component"))
        processComponentNode(pDomNode);
    else
        qWarning("Unsupported node: %s", qPrintable(pDomNode.nodeName()));
}

//==============================================================================

}   // namespace PrettyCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
