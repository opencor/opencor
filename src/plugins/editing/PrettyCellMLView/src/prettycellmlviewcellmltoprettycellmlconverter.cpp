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
#include <QRegularExpression>

//==============================================================================

namespace OpenCOR {
namespace PrettyCellMLView {

//==============================================================================

PrettyCellMLViewCellmlToPrettyCellmlConverter::PrettyCellMLViewCellmlToPrettyCellmlConverter() :
    mOutput(QString()),
    mIndent(QString()),
    mLastOutputType(None),
    mErrorLine(-1),
    mErrorColumn(-1),
    mErrorMessage(QString()),
    mWarningLines(QIntList()),
    mWarningMessages(QStringList()),
    mRdfNodes(QDomDocument()),
    mAssignmentDone(false),
    mMappings(QMap<QString, QString>())
{
    // Mappings for relational operators

    mMappings.insert("neq", " <> ");
    mMappings.insert("gt", " > ");
    mMappings.insert("lt", " < ");
    mMappings.insert("geq", " >= ");
    mMappings.insert("leq", " <= ");

    // Mappings for arithmetic operators

    mMappings.insert("plus", "+");
    mMappings.insert("minus", "-");
    mMappings.insert("times", "*");
    mMappings.insert("divide", "/");
    mMappings.insert("ceiling", "ceil");
    mMappings.insert("floor", "floor");
    mMappings.insert("factorial", "fact");

    // Mappings for arithmetic operators

    mMappings.insert("and", " and ");
    mMappings.insert("or", " or ");
    mMappings.insert("xor", " xor ");
}

//==============================================================================

bool PrettyCellMLViewCellmlToPrettyCellmlConverter::execute(const QString &pFileName)
{
    // Convert the CellML file to pretty CellML by first getting a DOM
    // representation of the file

    QString fileContents;
    QDomDocument domDocument;

    Core::readTextFromFile(pFileName, fileContents);

    if (domDocument.setContent(fileContents, &mErrorMessage, &mErrorLine, &mErrorColumn)) {
        mOutput = QString();
        mIndent = QString();

        mLastOutputType = None;

        mWarningLines = QIntList();
        mWarningMessages = QStringList();

        mRdfNodes = QDomDocument("RDF Nodes");

        if (!processModelNode(domDocument.documentElement())) {
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

bool PrettyCellMLViewCellmlToPrettyCellmlConverter::hasWarnings() const
{
    // Return whether we have warning messages

    return mWarningMessages.count();
}

//==============================================================================

QIntList PrettyCellMLViewCellmlToPrettyCellmlConverter::warningLines() const
{
    // Return our warning lines

    return mWarningLines;
}

//==============================================================================

QStringList PrettyCellMLViewCellmlToPrettyCellmlConverter::warningMessages() const
{
    // Return our warning messages

    return mWarningMessages;
}

//==============================================================================

QDomDocument PrettyCellMLViewCellmlToPrettyCellmlConverter::rdfNodes() const
{
    // Return our RDF nodes

    return mRdfNodes;
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

void PrettyCellMLViewCellmlToPrettyCellmlConverter::outputString(const OutputType &pOutputType,
                                                                 const QString &pString)
{
    // Output the given string

    if (pString.isEmpty()) {
        if (mLastOutputType != EmptyLine)
            mOutput += "\n";
    } else {
        mOutput += mIndent+pString+"\n";
    }

    mLastOutputType = pOutputType;
}

//==============================================================================

QString PrettyCellMLViewCellmlToPrettyCellmlConverter::cmetaId(const QDomNode &pDomNode) const
{
    // Return the cmeta:id, if any, of the given node

    static const QString CmetaId = "cmeta:id";

    if (pDomNode.attributes().contains(CmetaId))
        return QString("{%1}").arg(pDomNode.attributes().namedItem(CmetaId).nodeValue().trimmed());
    else
        return QString();
}

//==============================================================================

bool PrettyCellMLViewCellmlToPrettyCellmlConverter::processModelNode(const QDomNode &pDomNode)
{
    // Start processing the given model node

    outputString(DefModel,
                 QString("def model%1 %2 as").arg(cmetaId(pDomNode))
                                             .arg(pDomNode.attributes().namedItem("name").nodeValue().trimmed()));

    indent();

    // Keep track of the model's attributes

    mAttributes = pDomNode.attributes();

    // Process the given model node's children

    QString nodeName;

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        nodeName = domNode.nodeName();

        if (!nodeName.compare("#comment")) {
            processCommentNode(domNode);
        } else if (!nodeName.compare("rdf:RDF")) {
            processRdfNode(domNode);
        } else if (!nodeName.compare("import")) {
            if (!processImportNode(domNode))
                return false;
        } else if (!nodeName.compare("units")) {
            if (!processUnitsNode(domNode))
                return false;
        } else if (!nodeName.compare("component")) {
            if (!processComponentNode(domNode))
                return false;
        } else if (!nodeName.compare("group")) {
            if (!processGroupNode(domNode))
                return false;
        } else if (!nodeName.compare("connection")) {
            if (!processConnectionNode(domNode))
                return false;
        } else {
            processUnknownNode(domNode);
        }
    }

    // Finish processing the given model node

    unindent();

    outputString(EndDef, "enddef;");

    return true;
}

//==============================================================================

void PrettyCellMLViewCellmlToPrettyCellmlConverter::processCommentNode(const QDomNode &pDomNode)
{
    // Process the given comment node

    QStringList commentLines = pDomNode.nodeValue().remove("\r").split("\n");
    // Note #1: we don't know which end of line string is used by the file, so
    //          the above code ensures that we can handle both "\r\n" on Windows
    //          and "\n" on Linux / OS X...
    // Note #2: unlike for other nodes, we don't trim its value since leading
    //          spaces may be used in a comment...

    if (   (mLastOutputType == Comment)
        || (mLastOutputType == ImportUnit) || (mLastOutputType == ImportComp)
        || (mLastOutputType == DefBaseUnit) || (mLastOutputType == EndDef)
        || (mLastOutputType == Unit) || (mLastOutputType == Var)
        || (mLastOutputType == Comp) || (mLastOutputType == EndComp)
        || (mLastOutputType == Vars)) {
        outputString();
    }

    foreach (const QString commentLine, commentLines)
        outputString(Comment, QString("// %1").arg(commentLine));
}

//==============================================================================

void PrettyCellMLViewCellmlToPrettyCellmlConverter::processRdfNode(const QDomNode &pDomNode)
{
    // Process the given RDF node by keeping track of it

    mRdfNodes.appendChild(pDomNode.cloneNode());
}

//==============================================================================

bool PrettyCellMLViewCellmlToPrettyCellmlConverter::processImportNode(const QDomNode &pDomNode)
{
    // Start processing the given import node

    if ((mLastOutputType == Comment) || (mLastOutputType == EndDef))
        outputString();

    outputString(DefImport,
                 QString("def import%1 using \"%2\" for").arg(cmetaId(pDomNode))
                                                         .arg(pDomNode.attributes().namedItem("xlink:href").nodeValue().trimmed()));

    indent();

    // Process the given import node's children

    QString nodeName;

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        nodeName = domNode.nodeName();

        if (!nodeName.compare("#comment")) {
            processCommentNode(domNode);
        } else if (!nodeName.compare("rdf:RDF")) {
            processRdfNode(domNode);
        } else if (!nodeName.compare("units")) {
            if (!processUnitsNode(domNode, true))
                return false;
        } else if (!nodeName.compare("component")) {
            if (!processComponentNode(domNode, true))
                return false;
        } else {
            processUnknownNode(domNode);
        }
    }

    // Finish processing the given group node

    unindent();

    outputString(EndDef, "enddef;");

    return true;
}

//==============================================================================

bool PrettyCellMLViewCellmlToPrettyCellmlConverter::processUnitsNode(const QDomNode &pDomNode,
                                                                     const bool &pInImportNode)
{
    // Start processing the given units node

    QString baseUnits = pDomNode.attributes().namedItem("base_units").nodeValue().trimmed();

    if (baseUnits.compare(QString()) && baseUnits.compare("yes") && baseUnits.compare("no")) {
        mErrorLine = pDomNode.lineNumber();
        mErrorMessage = QObject::tr("A 'base_units' attribute must have a value of 'yes' or 'no'.");

        return false;
    }

    bool isBaseUnits = !baseUnits.compare("yes");

    if (!pInImportNode && !isBaseUnits) {
        if (   (mLastOutputType == Comment) || (mLastOutputType == EndDef)
            || (mLastOutputType == DefBaseUnit)) {
            outputString();
        }

        outputString(DefUnit,
                     QString("def unit%1 %2 as").arg(cmetaId(pDomNode))
                                                .arg(pDomNode.attributes().namedItem("name").nodeValue().trimmed()));

        indent();
    }

    // Process the given units node's children

    QString nodeName;

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        nodeName = domNode.nodeName();

        if (!nodeName.compare("#comment"))
            processCommentNode(domNode);
        else if (!nodeName.compare("rdf:RDF"))
            processRdfNode(domNode);
        else if (!isBaseUnits && !pInImportNode && !nodeName.compare("unit"))
            processUnitNode(domNode);
        else
            processUnknownNode(domNode);
    }

    // Finish processing the given units node

    if (pInImportNode) {
        if ((mLastOutputType == Comment) || (mLastOutputType == ImportComp))
            outputString();

        outputString(ImportUnit,
                     QString("unit%1 %2 using unit %3;").arg(cmetaId(pDomNode))
                                                        .arg(pDomNode.attributes().namedItem("name").nodeValue().trimmed())
                                                        .arg(pDomNode.attributes().namedItem("units_ref").nodeValue().trimmed()));
    } else if (isBaseUnits) {
        if ((mLastOutputType == Comment) || (mLastOutputType == EndDef))
            outputString();

        outputString(DefBaseUnit,
                     QString("def unit%1 %2 as base unit;").arg(cmetaId(pDomNode))
                                                           .arg(pDomNode.attributes().namedItem("name").nodeValue().trimmed()));
    } else {
        unindent();

        outputString(EndDef, "enddef;");
    }

    return true;
}

//==============================================================================

void PrettyCellMLViewCellmlToPrettyCellmlConverter::processUnitNode(const QDomNode &pDomNode)
{
    // Process the given unit node's children
    // Note #1: unlike for most nodes, we process the node's children before
    //          processing the node itself since it only outputs one line, so
    //          it's not like we can put a comment within a def...enddef;
    //          statement...
    // Note #2: when parsed back, comments will not be in the unit node anymore,
    //          but in its parent...

    QString nodeName;

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        nodeName = domNode.nodeName();

        if (!nodeName.compare("#comment"))
            processCommentNode(domNode);
        else if (!nodeName.compare("rdf:RDF"))
            processRdfNode(domNode);
        else
            processUnknownNode(domNode);
    }

    // Process the given unit node

    QString parameters = QString();
    QString prefix = pDomNode.attributes().namedItem("prefix").nodeValue().trimmed();
    QString exponent = pDomNode.attributes().namedItem("exponent").nodeValue().trimmed();
    QString multiplier = pDomNode.attributes().namedItem("multiplier").nodeValue().trimmed();
    QString offset = pDomNode.attributes().namedItem("offset").nodeValue().trimmed();

    if (!prefix.isEmpty())
        parameters += "pref: "+prefix;

    if (!exponent.isEmpty())
        parameters += (parameters.isEmpty()?QString():", ")+"expo: "+exponent;

    if (!multiplier.isEmpty())
        parameters += (parameters.isEmpty()?QString():", ")+"mult: "+multiplier;

    if (!offset.isEmpty())
        parameters += (parameters.isEmpty()?QString():", ")+"off: "+offset;

    if (mLastOutputType == Comment)
        outputString();

    outputString(Unit,
                 QString("unit%1 %2%3;").arg(cmetaId(pDomNode))
                                        .arg(pDomNode.attributes().namedItem("units").nodeValue().trimmed())
                                        .arg(parameters.isEmpty()?QString():" {"+parameters+"}"));
}

//==============================================================================

bool PrettyCellMLViewCellmlToPrettyCellmlConverter::processComponentNode(const QDomNode &pDomNode,
                                                                         const bool &pInImportNode)
{
    // Start processing the given component node

    if (!pInImportNode) {
        if ((mLastOutputType == Comment) || (mLastOutputType == EndDef))
            outputString();

        outputString(DefComp,
                     QString("def comp%1 %2 as").arg(cmetaId(pDomNode))
                                                .arg(pDomNode.attributes().namedItem("name").nodeValue().trimmed()));

        indent();
    }

    // Process the given component node's children

    QString nodeName;

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        nodeName = domNode.nodeName();

        if (!nodeName.compare("#comment")) {
            processCommentNode(domNode);
        } else if (!nodeName.compare("rdf:RDF")) {
            processRdfNode(domNode);
        } else if (!pInImportNode && !nodeName.compare("units")) {
            if (!processUnitsNode(domNode))
                return false;
        } else if (!pInImportNode && !nodeName.compare("variable")) {
            processVariableNode(domNode);
        } else if (!pInImportNode && !nodeName.compare("math")) {
            if (!processMathNode(domNode))
                return false;
        } else if (!pInImportNode && !nodeName.compare("reaction")) {
            if (!processReactionNode(domNode))
                return false;
        } else {
            processUnknownNode(domNode);
        }
    }

    // Finish processing the given component node

    if (pInImportNode) {
        if ((mLastOutputType == Comment) || (mLastOutputType == ImportUnit))
            outputString();

        outputString(ImportComp,
                     QString("comp%1 %2 using comp %3;").arg(cmetaId(pDomNode))
                                                        .arg(pDomNode.attributes().namedItem("name").nodeValue().trimmed())
                                                        .arg(pDomNode.attributes().namedItem("component_ref").nodeValue().trimmed()));
    } else {
        unindent();

        outputString(EndDef, "enddef;");
    }

    return true;
}

//==============================================================================

void PrettyCellMLViewCellmlToPrettyCellmlConverter::processVariableNode(const QDomNode &pDomNode)
{
    // Process the given variable node's children
    // Note #1: unlike for most nodes, we process the node's children before
    //          processing the node itself since it only outputs one line, so
    //          it's not like we can put a comment within a def...enddef;
    //          statement...
    // Note #2: when parsed back, comments will not be in the variable node
    //          anymore, but in its parent...

    QString nodeName;

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        nodeName = domNode.nodeName();

        if (!nodeName.compare("#comment"))
            processCommentNode(domNode);
        else if (!nodeName.compare("rdf:RDF"))
            processRdfNode(domNode);
        else
            processUnknownNode(domNode);
    }

    // Process the given variable node

    QString parameters = QString();
    QString initialValue = pDomNode.attributes().namedItem("initial_value").nodeValue().trimmed();
    QString publicInterface = pDomNode.attributes().namedItem("public_interface").nodeValue().trimmed();
    QString privateInterface = pDomNode.attributes().namedItem("private_interface").nodeValue().trimmed();

    if (!initialValue.isEmpty())
        parameters += "init: "+initialValue;

    if (!publicInterface.isEmpty())
        parameters += (parameters.isEmpty()?QString():", ")+"pub: "+publicInterface;

    if (!privateInterface.isEmpty())
        parameters += (parameters.isEmpty()?QString():", ")+"priv: "+privateInterface;

    if (   (mLastOutputType == Comment)
        || (mLastOutputType == DefBaseUnit) || (mLastOutputType == EndDef)) {
        outputString();
    }

    outputString(Var,
                 QString("var%1 %2: %3%4;").arg(cmetaId(pDomNode))
                                           .arg(pDomNode.attributes().namedItem("name").nodeValue().trimmed())
                                           .arg(pDomNode.attributes().namedItem("units").nodeValue().trimmed())
                                           .arg(parameters.isEmpty()?QString():" {"+parameters+"}"));
}

//==============================================================================

bool PrettyCellMLViewCellmlToPrettyCellmlConverter::processMathNode(const QDomNode &pDomNode)
{
    // Process the given math node's children

    QString equation;
    bool hasError;

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        mAssignmentDone = hasError = false;
        equation = processMathmlNode(domNode, hasError);

        if (hasError) {
            return false;
        } else {
            if (   (mLastOutputType == Comment)
                || (mLastOutputType == DefBaseUnit) || (mLastOutputType == EndDef)
                || (mLastOutputType == Var)) {
                outputString();
            }

            outputString(Equation, equation+";");
        }
    }

    return true;
}

//==============================================================================

QString PrettyCellMLViewCellmlToPrettyCellmlConverter::processMathmlNode(const QDomNode &pDomNode,
                                                                         bool &pHasError)
{
    // Make sure that we are not coming here with an error

    if (pHasError)
        return QString();

    // Process the given MathML node and its children, if any

    QDomNode domNode = pDomNode;
    QString nodeName = domNode.nodeName();
    QDomNodeList childNodes = domNode.childNodes();
    int childNodesCount = childNodes.count();

    // Basic content elements

    if (!nodeName.compare("apply")) {
        // Make sure that we have at least one child

        if (!childNodesCount) {
            mErrorMessage = QObject::tr("An 'apply' element must have at least one child element.");
        } else {
            domNode = childNodes.item(0);
            nodeName = domNode.nodeName();

            // Relational operators

            if (!nodeName.compare("eq")) {
                if (childNodesCount != 3) {
                    mErrorMessage = QObject::tr("An '%1' operator must have two operands.").arg(nodeName);
                } else if (mAssignmentDone) {
                    return processOperatorNode(" == ", pDomNode, pHasError);
                } else {
                    mAssignmentDone = true;

                    return processOperatorNode(" = ", pDomNode, pHasError);
                }
            } else if (   !nodeName.compare("neq")
                       || !nodeName.compare("lt") || !nodeName.compare("leq")) {
                if (childNodesCount != 3)
                    mErrorMessage = QObject::tr("An '%1' operator must have two operands.").arg(nodeName);
                else
                    return processOperatorNode(mMappings.value(nodeName), pDomNode, pHasError);
            } else if (   !nodeName.compare("gt") || !nodeName.compare("geq")) {
                if (childNodesCount != 3)
                    mErrorMessage = QObject::tr("A '%1' operator must have two operands.").arg(nodeName);
                else
                    return processOperatorNode(mMappings.value(nodeName), pDomNode, pHasError);

            // Arythmetic operators

            } else if (!nodeName.compare("plus") || !nodeName.compare("minus")) {
                if (childNodesCount < 2)
                    mErrorMessage = QObject::tr("A '%1' operator must have at least one operand.").arg(nodeName);
                else
                    return processOperatorNode(mMappings.value(nodeName), pDomNode, pHasError);
            } else if (!nodeName.compare("times") || !nodeName.compare("divide")) {
                if (childNodesCount < 3)
                    mErrorMessage = QObject::tr("A '%1' operator must have at least two operands.").arg(nodeName);
                else
                    return processOperatorNode(mMappings.value(nodeName), pDomNode, pHasError);
            } else if (!nodeName.compare("power")) {
                if (childNodesCount != 3)
                    mErrorMessage = QObject::tr("A '%1' function must have two arguments.").arg(nodeName);
                else
                    return processPowerNode(pDomNode, pHasError);
            } else if (!nodeName.compare("root")) {
                if ((childNodesCount != 2) && (childNodesCount != 3))
                    mErrorMessage = QObject::tr("A '%1' function must have either one or two arguments.").arg(nodeName);
                else
                    return processRootNode(pDomNode, pHasError);
            } else if (   !nodeName.compare("abs") || !nodeName.compare("exp")
                       || !nodeName.compare("ln")) {
                if (childNodesCount != 2)
                    mErrorMessage = QObject::tr("An '%1' function must have one argument.").arg(nodeName);
                else
                    return processFunctionNode(nodeName, pDomNode, pHasError);
            } else if (!nodeName.compare("log")) {
                if ((childNodesCount != 2) && (childNodesCount != 3))
                    mErrorMessage = QObject::tr("A '%1' function must have either one or two arguments.").arg(nodeName);
                else
                    return processLogNode(pDomNode, pHasError);
            } else if (   !nodeName.compare("ceiling")
                       || !nodeName.compare("floor")
                       || !nodeName.compare("factorial")) {
                if (childNodesCount != 2)
                    mErrorMessage = QObject::tr("A '%1' function must have one argument.").arg(nodeName);
                else
                    return processFunctionNode(mMappings.value(nodeName), pDomNode, pHasError);

            // Logical operators

            } else if (!nodeName.compare("and") || !nodeName.compare("or")) {
                if (childNodesCount < 3)
                    mErrorMessage = QObject::tr("An '%1' operator must have at least two operands.").arg(nodeName);
                else
                    return processOperatorNode(mMappings.value(nodeName), pDomNode, pHasError);
            } else if (!nodeName.compare("xor")) {
                if (childNodesCount < 3)
                    mErrorMessage = QObject::tr("A '%1' operator must have at least two operands.").arg(nodeName);
                else
                    return processOperatorNode(mMappings.value(nodeName), pDomNode, pHasError);
            } else if (!nodeName.compare("not")) {
                if (childNodesCount != 2)
                    mErrorMessage = QObject::tr("A '%1' operator must have one operand.").arg(nodeName);
                else
                    return processNotNode(pDomNode, pHasError);

            // Calculus elements

            } else if (!nodeName.compare("diff")) {
                if (childNodesCount != 3)
                    mErrorMessage = QObject::tr("A '%1' operator must have two qualifiers.").arg(nodeName);
                else
                    return processDiffNode(pDomNode, pHasError);

            // Unsupported node

            } else {
                processUnsupportedNode(domNode);
            }
        }

    // Token elements

    } else if (!nodeName.compare("cn")) {
        QString type = domNode.attributes().namedItem("type").nodeValue().trimmed();

        if (type.isEmpty() || !type.compare("real")) {
            // Either no type (i.e. real type by default) or real type

            if (childNodesCount != 1)
                mErrorMessage = QObject::tr("A '%1' element must have a value.").arg(nodeName);
            else
                return childNodes.item(0).nodeValue().trimmed()+"{"+domNode.attributes().namedItem("cellml:units").nodeValue().trimmed()+"}";
        } else if (!type.compare("e-notation")) {
            // E-notation type

            if (childNodesCount != 3) {
                mErrorMessage = QObject::tr("A 'cn' element with an 'e-notation' type must have three child elements.");
            } else {
                QDomNode childNode1 = childNodes.item(0);
                QDomNode childNode2 = childNodes.item(1);
                QDomNode childNode3 = childNodes.item(2);

                if (childNode1.nodeType() != QDomNode::TextNode) {
                    mErrorMessage = QObject::tr("The first child element of a 'cn' element with an 'e-notation' type must be of 'text' type.");
                } else if (childNode2.nodeType() != QDomNode::ElementNode) {
                    mErrorMessage = QObject::tr("The second child element of a 'cn' element with an 'e-notation' type must be of 'element' type.");
                } else if (childNode2.nodeName().compare("sep")) {
                    mErrorMessage = QObject::tr("The name of the second child element of a 'cn' element with an 'e-notation' type must be 'sep'.");
                } else if (childNode3.nodeType() != QDomNode::TextNode) {
                    mErrorMessage = QObject::tr("The third child element of a 'cn' element with an 'e-notation' type must be of 'text' type.");
                } else {
                    return childNode1.nodeValue().trimmed()+"e"+childNode3.nodeValue().trimmed()+"{"+domNode.attributes().namedItem("cellml:units").nodeValue().trimmed()+"}";
                }
            }
        } else if (   !type.compare("integer")
                   || !type.compare("rational")
                   || !type.compare("complex-polar")
                   || !type.compare("complex-cartesian")
                   || !type.compare("constant")) {
            // A known, but unsupported type

            mErrorMessage = QObject::tr("The 'cn' element uses a '%1' type, which is unsupported.").arg(type);
        } else {
            // An unknown type

            mErrorMessage = QObject::tr("The 'cn' element uses a '%1' type, which is unknown.").arg(type);
        }
    } else if (!nodeName.compare("ci")) {
        if (childNodesCount != 1)
            mErrorMessage = QObject::tr("A '%1' element must have a value.").arg(nodeName);
        else
            return childNodes.item(0).nodeValue().trimmed();

    // Qualifier elements

    } else if (!nodeName.compare("degree") || !nodeName.compare("logbase")) {
        if (childNodesCount != 1)
            mErrorMessage = QObject::tr("A '%1' element must have one child element.").arg(nodeName);
        else
            return processMathmlNode(childNodes.item(0), pHasError);
    } else if (!nodeName.compare("bvar")) {
        if ((childNodesCount != 1) && (childNodesCount != 2))
            mErrorMessage = QObject::tr("A '%1' element must have one or two child elements.").arg(nodeName);
        else
            return processBvarNode(pDomNode, pHasError);

    // Unsupported node

    } else {
        processUnsupportedNode(domNode);
    }

    mErrorLine = domNode.lineNumber();

    pHasError = true;

    return QString();
}

//==============================================================================

QString PrettyCellMLViewCellmlToPrettyCellmlConverter::processOperatorNode(const QString &pOperator,
                                                                           const QDomNode &pDomNode,
                                                                           bool &pHasError)
{
    // Process the operator node, based on its number of operands

    if (pDomNode.childNodes().count() == 2) {
        QString operand = processMathmlNode(pDomNode.childNodes().at(1), pHasError);

        if (pHasError)
            return QString();
        else if (!pDomNode.childNodes().at(0).nodeName().compare("plus"))
            return operand;
        else
            return pOperator+operand;
    } else {
        QString firstOperand = processMathmlNode(pDomNode.childNodes().at(1), pHasError);

        if (pHasError) {
            return QString();
        } else {
            QString res = firstOperand;
            QString otherOperand;

            for (int i = 2, imax = pDomNode.childNodes().count(); i < imax; ++i) {
                otherOperand = processMathmlNode(pDomNode.childNodes().at(i), pHasError);

                if (pHasError)
                    return QString();
                else
                    res += pOperator+otherOperand;
            }

            return res;
        }
    }
}

//==============================================================================

QString PrettyCellMLViewCellmlToPrettyCellmlConverter::processFunctionNode(const QString &pFunction,
                                                                           const QDomNode &pDomNode,
                                                                           bool &pHasError)
{
    // Process the function node

    QString argument = processMathmlNode(pDomNode.childNodes().at(1), pHasError);

    if (pHasError)
        return QString();
    else
        return pFunction+"("+argument+")";
}

//==============================================================================

QString PrettyCellMLViewCellmlToPrettyCellmlConverter::processPowerNode(const QDomNode &pDomNode,
                                                                        bool &pHasError)
{
    // Process the power node

    QString a = processMathmlNode(pDomNode.childNodes().at(1), pHasError);

    if (pHasError) {
        return QString();
    } else {
        QString b = processMathmlNode(pDomNode.childNodes().at(2), pHasError);

        if (pHasError) {
            return QString();
        } else {
            // Determine the value of b, which we assume to be a number (i.e.
            // something like "3{dimensionless}")
            // Note: if b isn't a number, then n will be equal to zero, which is
            //       what we want in that case...

            double n = QString(b).replace(QRegularExpression("{[^}]*}$"), QString()).toDouble();

            if (n == 2.0)
                return "sqr("+a+")";
            else if (n == 0.5)
                return "sqrt("+a+")";
            else
                return "pow("+a+", "+b+")";
        }
    }
}

//==============================================================================

QString PrettyCellMLViewCellmlToPrettyCellmlConverter::processRootNode(const QDomNode &pDomNode,
                                                                       bool &pHasError)
{
    // Process the root node, based on its number of arguments

    if (pDomNode.childNodes().count() == 2) {
        QString a = processMathmlNode(pDomNode.childNodes().at(1), pHasError);

        if (pHasError)
            return QString();
        else
            return "sqrt("+a+")";
    } else {
        QDomNode domNode = pDomNode.childNodes().at(1);

        if (domNode.nodeName().compare("degree")){
            mErrorMessage = QObject::tr("The first sibling of a 'root' element with two siblings must be a 'degree' element.");
            mErrorLine = domNode.lineNumber();

            pHasError = true;

            return QString();
        } else {
            QString b = processMathmlNode(domNode, pHasError);

            if (pHasError) {
                return QString();
            } else {
                QString a = processMathmlNode(pDomNode.childNodes().at(2), pHasError);

                if (pHasError) {
                    return QString();
                } else {
                    // Determine the value of b, which we assume to be a number
                    // (i.e. something like "3{dimensionless}")
                    // Note: if b isn't a number, then n will be equal to zero,
                    //       which is what we want in that case...

                    double n = QString(b).replace(QRegularExpression("{[^}]*}$"), QString()).toDouble();

                    if (n == 2.0)
                        return "sqrt("+a+")";
                    else
                        return "root("+a+", "+b+")";
                }
            }
        }
    }
}

//==============================================================================

QString PrettyCellMLViewCellmlToPrettyCellmlConverter::processLogNode(const QDomNode &pDomNode,
                                                                      bool &pHasError)
{
    // Process the log node

    QString argumentOrBase = processMathmlNode(pDomNode.childNodes().at(1), pHasError);

    if (pHasError) {
        return QString();
    } else {
        if (pDomNode.childNodes().count() == 2) {
            return "log("+argumentOrBase+")";
        } else {
            QString argument = processMathmlNode(pDomNode.childNodes().at(2), pHasError);

            if (pHasError)
                return QString();
            else
                return "log("+argument+", "+argumentOrBase+")";
        }
    }
}

//==============================================================================

QString PrettyCellMLViewCellmlToPrettyCellmlConverter::processNotNode(const QDomNode &pDomNode,
                                                                      bool &pHasError)
{
//---GRY--- NEED TO HANDLE IT BETTER, I.E.
//              a = not b;
//          BUT
//              a = not(b or c);

    // Process the not node

    QString operand = processMathmlNode(pDomNode.childNodes().at(1), pHasError);

    if (pHasError)
        return QString();
    else
        return "not("+operand+")";
}

//==============================================================================

QString PrettyCellMLViewCellmlToPrettyCellmlConverter::processDiffNode(const QDomNode &pDomNode,
                                                                       bool &pHasError)
{
    // Process the diff node

    QString f = processMathmlNode(pDomNode.childNodes().at(2), pHasError);

    if (pHasError) {
        return QString();
    } else {
        QDomNode domNode = pDomNode.childNodes().at(1);

        if (domNode.nodeName().compare("bvar")){
            mErrorMessage = QObject::tr("The first qualifier of a 'diff' operator must be a 'bvar' element.");
            mErrorLine = domNode.lineNumber();

            pHasError = true;

            return QString();
        } else {
            QString x = processMathmlNode(pDomNode.childNodes().at(1), pHasError);

            if (pHasError)
                return QString();
            else
                return "ode("+f+", "+x+")";
        }
    }
}

//==============================================================================

QString PrettyCellMLViewCellmlToPrettyCellmlConverter::processBvarNode(const QDomNode &pDomNode,
                                                                       bool &pHasError)
{
    // Process the bvar node, based on its number of child elements

    if (pDomNode.childNodes().count() == 1) {
        return processMathmlNode(pDomNode.childNodes().at(0), pHasError);
    } else {
        QDomNode domNode = pDomNode.childNodes().at(0);

        if (domNode.nodeName().compare("degree")){
            mErrorMessage = QObject::tr("The first child element of a 'bvar' element with two child elements must be a 'degree' element.");
            mErrorLine = domNode.lineNumber();

            pHasError = true;

            return QString();
        } else {
            QString b = processMathmlNode(domNode, pHasError);

            if (pHasError) {
                return QString();
            } else {
                QString a = processMathmlNode(pDomNode.childNodes().at(1), pHasError);

                if (pHasError)
                    return QString();
                else
                    return a+", "+b;
            }
        }
    }
}

//==============================================================================

bool PrettyCellMLViewCellmlToPrettyCellmlConverter::processReactionNode(const QDomNode &pDomNode)
{
    // We don't support reaction elements

    processUnsupportedNode(pDomNode);

    return false;
}

//==============================================================================

bool PrettyCellMLViewCellmlToPrettyCellmlConverter::processGroupNode(const QDomNode &pDomNode)
{
    // Start processing the given group node

    static const QString RelationshipRef = "___RELATIONSHIP_REF___";

    if ((mLastOutputType == Comment) || (mLastOutputType == EndDef))
        outputString();

    outputString(DefGroup,
                 QString("def group%1 as %2 for").arg(cmetaId(pDomNode))
                                                 .arg(RelationshipRef));

    indent();

    // Process the given group node's children

    QString nodeName;
    QString relationshipReference = QString();

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        nodeName = domNode.nodeName();

        if (!nodeName.compare("#comment")) {
            processCommentNode(domNode);
        } else if (!nodeName.compare("rdf:RDF")) {
            processRdfNode(domNode);
        } else if (!nodeName.compare("relationship_ref")) {
            if (!processRelationshipRefNode(domNode, relationshipReference))
                return false;
        } else if (!nodeName.compare("component_ref")) {
            processComponentRefNode(domNode);
        } else {
            processUnknownNode(domNode);
        }
    }

    // Finish processing the given group node

    mOutput.replace(RelationshipRef, relationshipReference);

    unindent();

    outputString(EndDef, "enddef;");

    return true;
}

//==============================================================================

bool PrettyCellMLViewCellmlToPrettyCellmlConverter::processRelationshipRefNode(const QDomNode &pDomNode,
                                                                               QString &pRelationshipRef)
{
    // Process the given relationship ref node's children
    // Note #1: unlike for most nodes, we process the node's children before
    //          processing the node itself since it doesn't output any line as
    //          such, so it's not like we can put a comment within a
    //          def...enddef; statement...
    // Note #2: when parsed back, comments will not be in the relationship ref
    //          node anymore, but in its parent...

    QString nodeName;

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        nodeName = domNode.nodeName();

        if (!nodeName.compare("#comment"))
            processCommentNode(domNode);
        else if (!nodeName.compare("rdf:RDF"))
            processRdfNode(domNode);
        else
            processUnknownNode(domNode);
    }

    // Process the given relationship ref node

    QString relationship = pDomNode.attributes().namedItem("relationship").nodeValue().trimmed();
    QString name = pDomNode.attributes().namedItem("name").nodeValue().trimmed();
    bool isEncapsulation = false;

    if (pDomNode.namespaceURI().isEmpty()) {
        if (!relationship.compare("encapsulation")) {
            isEncapsulation = true;
        } else if (relationship.compare("containment")) {
            mErrorLine = pDomNode.lineNumber();
            mErrorMessage = QObject::tr("A 'relationship' attribute in the CellML namespace must have a value of 'encapsulation' or 'containment'.");

            return false;
        }
    }

    if (isEncapsulation && !name.isEmpty()) {
        mErrorLine = pDomNode.lineNumber();
        mErrorMessage = QObject::tr("A 'relationship_ref' element with a 'relationship' attribute value of 'encapsulation' must not define a 'name' attribute.");

        return false;
    }

    pRelationshipRef += QString("%1%2%3%4").arg(pRelationshipRef.size()?" and ":QString())
                                           .arg(relationship)
                                           .arg(cmetaId(pDomNode))
                                           .arg(name.size()?" "+name:QString());

    return true;
}

//==============================================================================

void PrettyCellMLViewCellmlToPrettyCellmlConverter::processComponentRefNode(const QDomNode &pDomNode)
{
    // Determine whether the given component ref node has component ref children

    bool hasComponentRefChildren = false;

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling())  {
        if (!domNode.nodeName().compare("component_ref")) {
            hasComponentRefChildren = true;

            break;
        }
    }

    // Start processing the given component ref node

    if (hasComponentRefChildren) {
        if ((mLastOutputType == Comment) || (mLastOutputType == EndComp))
            outputString();

        outputString(CompIncl,
                     QString("comp%1 %2 incl").arg(cmetaId(pDomNode))
                                              .arg(pDomNode.attributes().namedItem("component").nodeValue().trimmed()));

        indent();
    }

    // Process the given component ref node's children

    QString nodeName;

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        nodeName = domNode.nodeName();

        if (!nodeName.compare("#comment"))
            processCommentNode(domNode);
        else if (!nodeName.compare("rdf:RDF"))
            processRdfNode(domNode);
        else if (!nodeName.compare("component_ref"))
            processComponentRefNode(domNode);
        else
            processUnknownNode(domNode);
    }

    // Finish processing the given group node

    if (hasComponentRefChildren) {
        unindent();

        outputString(EndComp, "endcomp;");
    } else {
        if ((mLastOutputType == Comment) || (mLastOutputType == EndComp))
            outputString();

        outputString(Comp,
                     QString("comp%1 %2;").arg(cmetaId(pDomNode))
                                          .arg(pDomNode.attributes().namedItem("component").nodeValue().trimmed()));
    }

}

//==============================================================================

bool PrettyCellMLViewCellmlToPrettyCellmlConverter::processConnectionNode(const QDomNode &pDomNode)
{
    // Start processing the given connection node

    static const QString MapComponents = "___MAP_COMPONENTS___";

    if ((mLastOutputType == Comment) || (mLastOutputType == EndDef))
        outputString();

    outputString(DefMap,
                 QString("def map%1 %2 for").arg(cmetaId(pDomNode))
                                            .arg(MapComponents));

    indent();

    // Process the given connection node's children

    QString nodeName;
    QString mapComponents = QString();

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        nodeName = domNode.nodeName();

        if (!nodeName.compare("#comment")) {
            processCommentNode(domNode);
        } else if (!nodeName.compare("rdf:RDF")) {
            processRdfNode(domNode);
        } else if (!nodeName.compare("map_components")) {
            if (!processMapComponentsNode(domNode, mapComponents))
                return false;
        } else if (!nodeName.compare("map_variables")) {
            processMapVariablesNode(domNode);
        } else {
            processUnknownNode(domNode);
        }
    }

    // Finish processing the given group node

    mOutput.replace(MapComponents, mapComponents);

    unindent();

    outputString(EndDef, "enddef;");

    return true;
}

//==============================================================================

bool PrettyCellMLViewCellmlToPrettyCellmlConverter::processMapComponentsNode(const QDomNode &pDomNode,
                                                                             QString &pMapComponents)
{
    // Make sure that we haven't already processed a map components node

    if (!pMapComponents.isEmpty()) {
        mErrorLine = pDomNode.lineNumber();
        mErrorMessage = QObject::tr("A 'connection' element must contain exactly one 'map_components' element.");

        return false;
    }

    // Process the given map components node's children
    // Note #1: unlike for most nodes, we process the node's children before
    //          processing the node itself since it doesn't output any line as
    //          such, so it's not like we can put a comment within a
    //          def...enddef; statement...
    // Note #2: when parsed back, comments will not be in the map components
    //          node anymore, but in its parent...

    QString nodeName;

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        nodeName = domNode.nodeName();

        if (!nodeName.compare("#comment"))
            processCommentNode(domNode);
        else if (!nodeName.compare("rdf:RDF"))
            processRdfNode(domNode);
        else
            processUnknownNode(domNode);
    }

    // Process the given map components node

    pMapComponents = QString("between%1 %2 and %3").arg(cmetaId(pDomNode))
                                                   .arg(pDomNode.attributes().namedItem("component_1").nodeValue().trimmed())
                                                   .arg(pDomNode.attributes().namedItem("component_2").nodeValue().trimmed());

    return true;
}

//==============================================================================

void PrettyCellMLViewCellmlToPrettyCellmlConverter::processMapVariablesNode(const QDomNode &pDomNode)
{
    // Process the given map variables node's children
    // Note #1: unlike for most nodes, we process the node's children before
    //          processing the node itself since it only outputs one line, so
    //          it's not like we can put a comment within a def...enddef;
    //          statement...
    // Note #2: when parsed back, comments will not be in the map variables node
    //          anymore, but in its parent...

    QString nodeName;

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        nodeName = domNode.nodeName();

        if (!nodeName.compare("#comment"))
            processCommentNode(domNode);
        else if (!nodeName.compare("rdf:RDF"))
            processRdfNode(domNode);
        else
            processUnknownNode(domNode);
    }

    // Process the given unit node

    if (mLastOutputType == Comment)
        outputString();

    outputString(Vars,
                 QString("vars%1 %2 and %3;").arg(cmetaId(pDomNode))
                                             .arg(pDomNode.attributes().namedItem("variable_1").nodeValue().trimmed())
                                             .arg(pDomNode.attributes().namedItem("variable_2").nodeValue().trimmed()));
}

//==============================================================================

void PrettyCellMLViewCellmlToPrettyCellmlConverter::processUnknownNode(const QDomNode &pDomNode)
{
    // The given node is unknown, so warn the user about it

    mWarningLines << pDomNode.lineNumber();
    mWarningMessages << QObject::tr("A '%1' element was found%2, but it is not known and cannot therefore be processed.").arg(pDomNode.nodeName());
}

//==============================================================================

void PrettyCellMLViewCellmlToPrettyCellmlConverter::processUnsupportedNode(const QDomNode &pDomNode)
{
    // The given node is unknown, yet maybe we should know about it, so consider
    // it as an error

    mErrorLine = pDomNode.lineNumber();
    mErrorMessage = QObject::tr("A '%1' element was found in the original CellML file, but it is not supported and cannot therefore be processed.").arg(pDomNode.nodeName());
}

//==============================================================================

}   // namespace PrettyCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
