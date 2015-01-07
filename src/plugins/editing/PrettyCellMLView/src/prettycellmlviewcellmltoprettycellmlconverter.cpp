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
        mIndent = QString();

        if (!processNode(domDocument.documentElement())) {
            mOutput = fileContents;

            return false;
        } else {
            return true;
        }
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

bool PrettyCellMLViewCellmlToPrettyCellmlConverter::processModelNode(const QDomNode &pDomNode)
{
    // Start processing the given model node

    outputString(QString("def model%1 %2 as").arg(cmetaId(pDomNode),
                                                  pDomNode.attributes().namedItem("name").nodeValue()));

    indent();

    // Process the given model node's children

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling())
        if (!processNode(domNode))
            return false;

    // Finish processing the given model node

    unindent();

    outputString("enddef;");

    return true;
}

//==============================================================================

bool PrettyCellMLViewCellmlToPrettyCellmlConverter::processUnitsNode(const QDomNode &pDomNode)
{
    // Start processing the given units node

    QString baseUnits = pDomNode.attributes().namedItem("base_units").nodeValue();

    if (baseUnits.compare(QString()) && baseUnits.compare("yes") && baseUnits.compare("no")) {
        mErrorLine = pDomNode.lineNumber();
        mErrorMessage = QObject::tr("the 'base_units' attribute must have a value equal to 'yes' or 'no'.");

        return false;
    }

    bool isBaseUnits = !baseUnits.compare("yes");

    if (    mOutput.endsWith("enddef;"+Core::eolString())
        || (mOutput.endsWith("as base unit;"+Core::eolString()) && !isBaseUnits)) {
        outputString();
    }

    outputString(QString("def unit%1 %2 as%3").arg(cmetaId(pDomNode))
                                              .arg(pDomNode.attributes().namedItem("name").nodeValue())
                                              .arg(isBaseUnits?" base unit;":QString()));

    if (!isBaseUnits) {
        indent();

        // Process the given units node's children

        for (QDomNode domNode = pDomNode.firstChild();
             !domNode.isNull(); domNode = domNode.nextSibling())
            if (!processNode(domNode))
                return false;

        // Finish processing the given units node

        unindent();

        outputString("enddef;");
    }

    return true;
}

//==============================================================================

bool PrettyCellMLViewCellmlToPrettyCellmlConverter::processUnitNode(const QDomNode &pDomNode)
{
    // Process the given unit node

    QString parameters = QString();

    if (!pDomNode.attributes().namedItem("prefix").nodeValue().isEmpty())
        parameters += "pref: "+pDomNode.attributes().namedItem("prefix").nodeValue();

    if (!pDomNode.attributes().namedItem("exponent").nodeValue().isEmpty()) {
        if (!parameters.isEmpty())
            parameters += ", ";

        parameters += "expo: "+pDomNode.attributes().namedItem("exponent").nodeValue();
    }

    if (!pDomNode.attributes().namedItem("multiplier").nodeValue().isEmpty()) {
        if (!parameters.isEmpty())
            parameters += ", ";

        parameters += "mult: "+pDomNode.attributes().namedItem("multiplier").nodeValue();
    }

    if (!pDomNode.attributes().namedItem("offset").nodeValue().isEmpty()) {
        if (!parameters.isEmpty())
            parameters += ", ";

        parameters += "off: "+pDomNode.attributes().namedItem("offset").nodeValue();
    }

    outputString(QString("unit%1 %2%3;").arg(cmetaId(pDomNode))
                                        .arg(pDomNode.attributes().namedItem("units").nodeValue())
                                        .arg(parameters.isEmpty()?QString():" {"+parameters+"}"));

    return true;
}

//==============================================================================

bool PrettyCellMLViewCellmlToPrettyCellmlConverter::processComponentNode(const QDomNode &pDomNode)
{
    // Start processing the given component node

    if (mOutput.endsWith("enddef;"+Core::eolString()))
        outputString();

    outputString(QString("def comp%1 %2 as").arg(cmetaId(pDomNode),
                                                 pDomNode.attributes().namedItem("name").nodeValue()));

    indent();

    // Process the given component node's children

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling())
        if (!processNode(domNode))
            return false;

    // Finish processing the given component node

    unindent();

    outputString("enddef;");

    return true;
}

//==============================================================================

bool PrettyCellMLViewCellmlToPrettyCellmlConverter::processNode(const QDomNode &pDomNode)
{
    // Start processing the given node

    QString nodeName = pDomNode.nodeName();

    if (!nodeName.compare("model"))
        return processModelNode(pDomNode);
    else if (!nodeName.compare("units"))
        return processUnitsNode(pDomNode);
    else if (!nodeName.compare("unit"))
        return processUnitNode(pDomNode);
    else if (!nodeName.compare("component"))
        return processComponentNode(pDomNode);

    qWarning("Unsupported node: %s", qPrintable(pDomNode.nodeName()));

    return true;
}

//==============================================================================

}   // namespace PrettyCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
