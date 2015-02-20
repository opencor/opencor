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
// Raw CellML to CellML text converter
//==============================================================================

#include "cellmlfile.h"
#include "corecliutils.h"
#include "cellmltextviewconverter.h"

//==============================================================================

#include <QDomDocument>
#include <QRegularExpression>

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

CellMLTextViewConverterWarning::CellMLTextViewConverterWarning(const int &pLine,
                                                               const QString &pMessage) :
    mLine(pLine),
    mMessage(pMessage)
{
}

//==============================================================================

int CellMLTextViewConverterWarning::line() const
{
    // Return our line number

    return mLine;
}

//==============================================================================

QString CellMLTextViewConverterWarning::message() const
{
    // Return our message

    return mMessage;
}

//==============================================================================

CellMLTextViewConverter::CellMLTextViewConverter() :
    mOutput(QString()),
    mIndent(QString()),
    mLastOutputType(None),
    mErrorLine(-1),
    mErrorColumn(-1),
    mErrorMessage(QString()),
    mWarnings(CellMLTextViewConverterWarnings()),
    mRdfNodes(QDomDocument()),
    mTopMathmlNode(QDomNode()),
    mAssignmentDone(false),
    mOldPiecewiseStatementUsed(false),
    mPiecewiseStatementUsed(false),
    mMappings(QMap<QString, QString>()),
    mMathmlNodeTypes(QMap<QString, MathmlNodeType>())
{
    // Mappings for relational operators

                                           mMathmlNodeTypes.insert("eq", EqMathmlNode);
    mMappings.insert("neq", " <> ");       mMathmlNodeTypes.insert("neq", NeqMathmlNode);
    mMappings.insert("lt", " < ");         mMathmlNodeTypes.insert("lt", LtMathmlNode);
    mMappings.insert("leq", " <= ");       mMathmlNodeTypes.insert("leq", LeqMathmlNode);
    mMappings.insert("geq", " >= ");       mMathmlNodeTypes.insert("geq", GeqMathmlNode);
    mMappings.insert("gt", " > ");         mMathmlNodeTypes.insert("gt", GtMathmlNode);

    // Mappings for arithmetic operators

    mMappings.insert("plus", "+");         mMathmlNodeTypes.insert("plus", PlusMathmlNode);
    mMappings.insert("minus", "-");        mMathmlNodeTypes.insert("minus", MinusMathmlNode);
    mMappings.insert("times", "*");        mMathmlNodeTypes.insert("times", TimesMathmlNode);
    mMappings.insert("divide", "/");       mMathmlNodeTypes.insert("divide", DivideMathmlNode);
    mMappings.insert("ceiling", "ceil");
    mMappings.insert("floor", "floor");
    mMappings.insert("factorial", "fact");

    // Mappings for arithmetic operators

    mMappings.insert("and", " and ");      mMathmlNodeTypes.insert("and", AndMathmlNode);
    mMappings.insert("or", " or ");        mMathmlNodeTypes.insert("or", OrMathmlNode);
    mMappings.insert("xor", " xor ");      mMathmlNodeTypes.insert("xor", XorMathmlNode);

    // Mappings for constants

    mMappings.insert("true", "true");
    mMappings.insert("false", "false");
    mMappings.insert("notanumber", "nan");
    mMappings.insert("pi", "pi");
    mMappings.insert("infinity", "inf");
    mMappings.insert("exponentiale", "e");

    // Mappings for trigonometric operators

    mMappings.insert("arcsin", "asin");
    mMappings.insert("arccos", "acos");
    mMappings.insert("arctan", "atan");
    mMappings.insert("arcsec", "asec");
    mMappings.insert("arccsc", "acsc");
    mMappings.insert("arccot", "acot");
    mMappings.insert("arcsinh", "asinh");
    mMappings.insert("arccosh", "acosh");
    mMappings.insert("arctanh", "atanh");
    mMappings.insert("arcsech", "asech");
    mMappings.insert("arccsch", "acsch");
    mMappings.insert("arccoth", "acoth");
}

//==============================================================================

bool CellMLTextViewConverter::execute(const QString &pFileName)
{
    // Convert the CellML file to CellML text by first getting a DOM
    // representation of the file

    QString fileContents;
    QDomDocument domDocument;

    Core::readTextFromFile(pFileName, fileContents);

    if (domDocument.setContent(fileContents, true,
                               &mErrorMessage, &mErrorLine, &mErrorColumn)) {
        mOutput = QString();
        mIndent = QString();

        mLastOutputType = None;

        mWarnings = CellMLTextViewConverterWarnings();

        mRdfNodes = QDomDocument(QString());

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

QString CellMLTextViewConverter::output() const
{
    // Return our output

    return mOutput;
}

//==============================================================================

int CellMLTextViewConverter::errorLine() const
{
    // Return our error line

    return mErrorLine;
}

//==============================================================================

int CellMLTextViewConverter::errorColumn() const
{
    // Return our error column

    return mErrorColumn;
}

//==============================================================================

QString CellMLTextViewConverter::errorMessage() const
{
    // Return our error message

    return mErrorMessage;
}

//==============================================================================

bool CellMLTextViewConverter::hasWarnings() const
{
    // Return whether we have warnings

    return mWarnings.count();
}

//==============================================================================

CellMLTextViewConverterWarnings CellMLTextViewConverter::warnings() const
{
    // Return our warnings

    return mWarnings;
}

//==============================================================================

QDomDocument CellMLTextViewConverter::rdfNodes() const
{
    // Return our RDF nodes

    return mRdfNodes;
}

//==============================================================================

static const auto Indent = QStringLiteral("    ");

//==============================================================================

void CellMLTextViewConverter::indent()
{
    // Indent our output

    mIndent += Indent;
}

//==============================================================================

void CellMLTextViewConverter::unindent()
{
    // Unindent our output

    mIndent.chop(Indent.size());
}

//==============================================================================

void CellMLTextViewConverter::outputString(const OutputType &pOutputType,
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

bool CellMLTextViewConverter::rdfNode(const QDomNode &pDomNode) const
{
    // Return whether the given node is an RDF node

    return    !pDomNode.localName().compare("RDF")
           && !pDomNode.namespaceURI().compare(CellMLSupport::RdfNamespace);
}

//==============================================================================

bool CellMLTextViewConverter::cellmlNode(const QDomNode &pDomNode,
                                         const QString &pName) const
{
    // Return whether the given node is a CellML node with the given name

    return    !pDomNode.localName().compare(pName)
           && (   !pDomNode.namespaceURI().compare(CellMLSupport::Cellml_1_0_Namespace)
               || !pDomNode.namespaceURI().compare(CellMLSupport::Cellml_1_1_Namespace));
}

//==============================================================================

bool CellMLTextViewConverter::mathmlNode(const QDomNode &pDomNode,
                                         const QString &pName) const
{
    // Return whether the given node is a MathML node with the given name

    return    !pDomNode.localName().compare(pName)
           && !pDomNode.namespaceURI().compare(CellMLSupport::MathmlNamespace);
}

//==============================================================================

QString CellMLTextViewConverter::cmetaId(const QDomNode &pDomNode) const
{
    // Return the converted cmeta:id, if any, of the given node

    QDomNamedNodeMap domNodeAttributes = pDomNode.attributes();
    QDomNode cmetaIdAttributeNode = domNodeAttributes.namedItemNS(CellMLSupport::CmetaIdNamespace, "id");

    if (cmetaIdAttributeNode.isNull()) {
        // We couldn't find an id in the cmeta:id namespace, so try without the
        // namespace
        // Note: this seems to be an issue QDomNode & Co. Indeed, it would seem
        //       that if the attribute that defines the cmeta:id namespace is
        //       after the cmeta:id attribute (the case with the model element),
        //       then the cmeta:id will only be accessible without using a
        //       namespace...

        cmetaIdAttributeNode = domNodeAttributes.namedItem("id");
    }

    if (!cmetaIdAttributeNode.isNull())
        return QString("{%1}").arg(cmetaIdAttributeNode.nodeValue().trimmed());
    else
        return QString();
}

//==============================================================================

QString CellMLTextViewConverter::cellmlAttributeNodeValue(const QDomNode &pDomNode,
                                                          const QString &pName,
                                                          const bool &pMustBePresent) const
{
    // Return the trimmed value of the requested CellML attribute using the
    // CellML 1.0 namespace or, if needed, the CellML 1.1 namespace or even
    // without using a namespace (see cmetaId() for why not using a namespace)

    QDomNamedNodeMap domNodeAttributes = pDomNode.attributes();
    QDomNode cellmlAttributeNode = domNodeAttributes.namedItemNS(CellMLSupport::Cellml_1_0_Namespace, pName);

    if (cellmlAttributeNode.isNull()) {
        cellmlAttributeNode = domNodeAttributes.namedItemNS(CellMLSupport::Cellml_1_1_Namespace, pName);

        if (cellmlAttributeNode.isNull())
            cellmlAttributeNode = domNodeAttributes.namedItem(pName);
    }

    if (!cellmlAttributeNode.isNull())
        return cellmlAttributeNode.nodeValue().trimmed();
    else
        return pMustBePresent?"???":QString();
}

//==============================================================================

CellMLTextViewConverter::MathmlNodeType CellMLTextViewConverter::mathmlNodeType(const QDomNode &pDomNode) const
{
    // Return the MathML type of the given node

    if (   !pDomNode.localName().compare("apply")
        && !pDomNode.namespaceURI().compare(CellMLSupport::MathmlNamespace)) {
        return mMathmlNodeTypes.value(pDomNode.childNodes().item(0).localName());
    } else {
        return mMathmlNodeTypes.value(pDomNode.localName());
    }
}

//==============================================================================

bool CellMLTextViewConverter::processModelNode(const QDomNode &pDomNode)
{
    // Start processing the given model node

    outputString(DefModel,
                 QString("def model%1 %2 as").arg(cmetaId(pDomNode))
                                             .arg(cellmlAttributeNodeValue(pDomNode, "name")));

    indent();

    // Keep track of the model's attributes

    mAttributes = pDomNode.attributes();

    // Process the given model node's children

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        if (domNode.isComment()) {
            processCommentNode(domNode);
        } else if (rdfNode(domNode)) {
            processRdfNode(domNode);
        } else if (cellmlNode(domNode, "import")) {
            if (!processImportNode(domNode))
                return false;
        } else if (cellmlNode(domNode, "units")) {
            if (!processUnitsNode(domNode))
                return false;
        } else if (cellmlNode(domNode, "component")) {
            if (!processComponentNode(domNode))
                return false;
        } else if (cellmlNode(domNode, "group")) {
            if (!processGroupNode(domNode))
                return false;
        } else if (cellmlNode(domNode, "connection")) {
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

void CellMLTextViewConverter::processCommentNode(const QDomNode &pDomNode)
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
        outputString(Comment, QString("//%1").arg(commentLine));
}

//==============================================================================

void CellMLTextViewConverter::processRdfNode(const QDomNode &pDomNode)
{
    // Process the given RDF node by keeping track of it

    mRdfNodes.appendChild(pDomNode.cloneNode());
}

//==============================================================================

bool CellMLTextViewConverter::processImportNode(const QDomNode &pDomNode)
{
    // Start processing the given import node

    if ((mLastOutputType == Comment) || (mLastOutputType == EndDef))
        outputString();

    outputString(DefImport,
                 QString("def import%1 using \"%2\" for").arg(cmetaId(pDomNode))
                                                         .arg(pDomNode.attributes().namedItemNS(CellMLSupport::XlinkNamespace, "href").nodeValue().trimmed()));

    indent();

    // Process the given import node's children

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        if (domNode.isComment()) {
            processCommentNode(domNode);
        } else if (rdfNode(domNode)) {
            processRdfNode(domNode);
        } else if (cellmlNode(domNode, "units")) {
            if (!processUnitsNode(domNode, true))
                return false;
        } else if (cellmlNode(domNode, "component")) {
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

bool CellMLTextViewConverter::processUnitsNode(const QDomNode &pDomNode,
                                               const bool &pInImportNode)
{
    // Start processing the given units node

    QString baseUnits = cellmlAttributeNodeValue(pDomNode, "base_units", false);

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
                                                .arg(cellmlAttributeNodeValue(pDomNode, "name")));

        indent();
    }

    // Process the given units node's children

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        if (domNode.isComment())
            processCommentNode(domNode);
        else if (rdfNode(domNode))
            processRdfNode(domNode);
        else if (!isBaseUnits && !pInImportNode && cellmlNode(domNode, "unit"))
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
                                                        .arg(cellmlAttributeNodeValue(pDomNode, "name"))
                                                        .arg(cellmlAttributeNodeValue(pDomNode, "units_ref")));
    } else if (isBaseUnits) {
        if ((mLastOutputType == Comment) || (mLastOutputType == EndDef))
            outputString();

        outputString(DefBaseUnit,
                     QString("def unit%1 %2 as base unit;").arg(cmetaId(pDomNode))
                                                           .arg(cellmlAttributeNodeValue(pDomNode, "name")));
    } else {
        unindent();

        outputString(EndDef, "enddef;");
    }

    return true;
}

//==============================================================================

void CellMLTextViewConverter::processUnitNode(const QDomNode &pDomNode)
{
    // Process the given unit node's children
    // Note #1: unlike for most nodes, we process the node's children before
    //          processing the node itself since it only outputs one line, so
    //          it's not like we can put a comment within a def...enddef;
    //          statement...
    // Note #2: when parsed back, comments will not be in the unit node anymore,
    //          but in its parent...

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        if (domNode.isComment())
            processCommentNode(domNode);
        else if (rdfNode(domNode))
            processRdfNode(domNode);
        else
            processUnknownNode(domNode);
    }

    // Process the given unit node

    QString parameters = QString();
    QString prefix = cellmlAttributeNodeValue(pDomNode, "prefix", false);
    QString exponent = cellmlAttributeNodeValue(pDomNode, "exponent", false);
    QString multiplier = cellmlAttributeNodeValue(pDomNode, "multiplier", false);
    QString offset = cellmlAttributeNodeValue(pDomNode, "offset", false);

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
                                        .arg(cellmlAttributeNodeValue(pDomNode, "units"))
                                        .arg(parameters.isEmpty()?QString():" {"+parameters+"}"));
}

//==============================================================================

bool CellMLTextViewConverter::processComponentNode(const QDomNode &pDomNode,
                                                   const bool &pInImportNode)
{
    // Start processing the given component node

    if (!pInImportNode) {
        if ((mLastOutputType == Comment) || (mLastOutputType == EndDef))
            outputString();

        outputString(DefComp,
                     QString("def comp%1 %2 as").arg(cmetaId(pDomNode))
                                                .arg(cellmlAttributeNodeValue(pDomNode, "name")));

        indent();
    }

    // Process the given component node's children

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        if (domNode.isComment()) {
            processCommentNode(domNode);
        } else if (rdfNode(domNode)) {
            processRdfNode(domNode);
        } else if (!pInImportNode && cellmlNode(domNode, "units")) {
            if (!processUnitsNode(domNode))
                return false;
        } else if (!pInImportNode && cellmlNode(domNode, "variable")) {
            processVariableNode(domNode);
        } else if (!pInImportNode && mathmlNode(domNode, "math")) {
            if (!processMathNode(domNode))
                return false;
        } else if (!pInImportNode && cellmlNode(domNode, "reaction")) {
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
                                                        .arg(cellmlAttributeNodeValue(pDomNode, "name"))
                                                        .arg(cellmlAttributeNodeValue(pDomNode, "component_ref")));
    } else {
        unindent();

        outputString(EndDef, "enddef;");
    }

    return true;
}

//==============================================================================

void CellMLTextViewConverter::processVariableNode(const QDomNode &pDomNode)
{
    // Process the given variable node's children
    // Note #1: unlike for most nodes, we process the node's children before
    //          processing the node itself since it only outputs one line, so
    //          it's not like we can put a comment within a def...enddef;
    //          statement...
    // Note #2: when parsed back, comments will not be in the variable node
    //          anymore, but in its parent...

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        if (domNode.isComment())
            processCommentNode(domNode);
        else if (rdfNode(domNode))
            processRdfNode(domNode);
        else
            processUnknownNode(domNode);
    }

    // Process the given variable node

    QString parameters = QString();
    QString initialValue = cellmlAttributeNodeValue(pDomNode, "initial_value", false);
    QString publicInterface = cellmlAttributeNodeValue(pDomNode, "public_interface", false);
    QString privateInterface = cellmlAttributeNodeValue(pDomNode, "private_interface", false);

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
                                           .arg(cellmlAttributeNodeValue(pDomNode, "name"))
                                           .arg(cellmlAttributeNodeValue(pDomNode, "units"))
                                           .arg(parameters.isEmpty()?QString():" {"+parameters+"}"));
}

//==============================================================================

bool CellMLTextViewConverter::processMathNode(const QDomNode &pDomNode)
{
    // Process the given math node's children

    QString equation;
    bool hasError;

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        mOldPiecewiseStatementUsed = mPiecewiseStatementUsed;
        mAssignmentDone = mPiecewiseStatementUsed = hasError = false;
        mTopMathmlNode = domNode;
        equation = processMathmlNode(domNode, hasError);

        if (hasError) {
            return false;
        } else if (!equation.isEmpty()) {
            // Note: should one or several warnings be generated, then it may be
            //       possible that no equation has been generated, hence our
            //       check...

            if (   (mLastOutputType == Comment)
                || (mLastOutputType == DefBaseUnit)
                || (mLastOutputType == EndDef)
                || (mLastOutputType == Var)
                ||  mPiecewiseStatementUsed
                || (mPiecewiseStatementUsed != mOldPiecewiseStatementUsed)) {
                outputString();
            }

            outputString(Equation, equation+";");
        }
    }

    return true;
}

//==============================================================================

QString CellMLTextViewConverter::processMathmlNode(const QDomNode &pDomNode,
                                                   bool &pHasError)
{
    // Make sure that we are not coming here with an error

    if (pHasError)
        return QString();

    // Process the given MathML node and its children, if any

    QDomNode domNode = pDomNode;
    QDomNodeList childNodes = domNode.childNodes();
    int childNodesCount = childNodes.count();

    // Basic content elements

    if (mathmlNode(domNode, "apply")) {
        // Make sure that we have at least one child

        if (!childNodesCount) {
            mErrorMessage = QObject::tr("An '%1' element must have at least one child element.").arg(domNode.localName());
        } else {
            domNode = childNodes.item(0);

            // Relational operators

            if (mathmlNode(domNode, "eq")) {
                if (childNodesCount != 3) {
                    mErrorMessage = QObject::tr("An '%1' element must have two siblings.").arg(domNode.localName());
                } else if (mAssignmentDone) {
                    return processOperatorNode(" == ", pDomNode, pHasError);
                } else {
                    mAssignmentDone = true;

                    return processOperatorNode(" = ", pDomNode, pHasError);
                }
            } else if (   mathmlNode(domNode, "neq")
                       || mathmlNode(domNode, "lt")
                       || mathmlNode(domNode, "leq")) {
                if (childNodesCount != 3)
                    mErrorMessage = QObject::tr("An '%1' element must have two siblings.").arg(domNode.localName());
                else
                    return processOperatorNode(mMappings.value(domNode.localName()), pDomNode, pHasError);
            } else if (   mathmlNode(domNode, "gt")
                       || mathmlNode(domNode, "geq")) {
                if (childNodesCount != 3)
                    mErrorMessage = QObject::tr("A '%1' element must have two siblings.").arg(domNode.localName());
                else
                    return processOperatorNode(mMappings.value(domNode.localName()), pDomNode, pHasError);

            // Arythmetic operators

            } else if (   mathmlNode(domNode, "plus")
                       || mathmlNode(domNode, "minus")) {
                if (childNodesCount < 2)
                    mErrorMessage = QObject::tr("A '%1' element must have at least one sibling.").arg(domNode.localName());
                else
                    return processOperatorNode(mMappings.value(domNode.localName()), pDomNode, pHasError);
            } else if (   mathmlNode(domNode, "times")
                       || mathmlNode(domNode, "divide")) {
                if (childNodesCount < 3)
                    mErrorMessage = QObject::tr("A '%1' element must have at least two siblings.").arg(domNode.localName());
                else
                    return processOperatorNode(mMappings.value(domNode.localName()), pDomNode, pHasError);
            } else if (mathmlNode(domNode, "power")) {
                if (childNodesCount != 3)
                    mErrorMessage = QObject::tr("A '%1' element must have two siblings.").arg(domNode.localName());
                else
                    return processPowerNode(pDomNode, pHasError);
            } else if (mathmlNode(domNode, "root")) {
                if ((childNodesCount != 2) && (childNodesCount != 3))
                    mErrorMessage = QObject::tr("A '%1' element must have either one or two siblings.").arg(domNode.localName());
                else
                    return processRootNode(pDomNode, pHasError);
            } else if (   mathmlNode(domNode, "abs")
                       || mathmlNode(domNode, "exp")
                       || mathmlNode(domNode, "ln")) {
                if (childNodesCount != 2)
                    mErrorMessage = QObject::tr("An '%1' element must have one sibling.").arg(domNode.localName());
                else
                    return processFunctionNode(domNode.localName(), pDomNode, pHasError);
            } else if (mathmlNode(domNode, "log")) {
                if ((childNodesCount != 2) && (childNodesCount != 3))
                    mErrorMessage = QObject::tr("A '%1' element must have either one or two siblings.").arg(domNode.localName());
                else
                    return processLogNode(pDomNode, pHasError);
            } else if (   mathmlNode(domNode, "ceiling")
                       || mathmlNode(domNode, "floor")
                       || mathmlNode(domNode, "factorial")) {
                if (childNodesCount != 2)
                    mErrorMessage = QObject::tr("A '%1' element must have one sibling.").arg(domNode.localName());
                else
                    return processFunctionNode(mMappings.value(domNode.localName()), pDomNode, pHasError);

            // Logical operators

            } else if (   mathmlNode(domNode, "and")
                       || mathmlNode(domNode, "or")) {
                if (childNodesCount < 3)
                    mErrorMessage = QObject::tr("An '%1' element must have at least two siblings.").arg(domNode.localName());
                else
                    return processOperatorNode(mMappings.value(domNode.localName()), pDomNode, pHasError);
            } else if (mathmlNode(domNode, "xor")) {
                if (childNodesCount < 3)
                    mErrorMessage = QObject::tr("A '%1' element must have at least two siblings.").arg(domNode.localName());
                else
                    return processOperatorNode(mMappings.value(domNode.localName()), pDomNode, pHasError);
            } else if (mathmlNode(domNode, "not")) {
                if (childNodesCount != 2)
                    mErrorMessage = QObject::tr("A '%1' element must have one sibling.").arg(domNode.localName());
                else
                    return processNotNode(pDomNode, pHasError);

            // Calculus elements

            } else if (mathmlNode(domNode, "diff")) {
                if (childNodesCount != 3)
                    mErrorMessage = QObject::tr("A '%1' element must have two siblings.").arg(domNode.localName());
                else
                    return processDiffNode(pDomNode, pHasError);

            // Trigonometric operators

            } else if (   mathmlNode(domNode,  "sin") || mathmlNode(domNode,  "cos") || mathmlNode(domNode,  "tan")
                       || mathmlNode(domNode,  "sec") || mathmlNode(domNode,  "csc") || mathmlNode(domNode,  "cot")
                       || mathmlNode(domNode, "sinh") || mathmlNode(domNode, "cosh") || mathmlNode(domNode, "tanh")
                       || mathmlNode(domNode, "sech") || mathmlNode(domNode, "csch") || mathmlNode(domNode, "coth")) {
                if (childNodesCount != 2)
                    mErrorMessage = QObject::tr("A '%1' element must have one sibling.").arg(domNode.localName());
                else
                    return processFunctionNode(domNode.localName(), pDomNode, pHasError);
            } else if (   mathmlNode(domNode,  "arcsin") || mathmlNode(domNode,  "arccos") || mathmlNode(domNode,  "arctan")
                       || mathmlNode(domNode,  "arcsec") || mathmlNode(domNode,  "arccsc") || mathmlNode(domNode,  "arccot")
                       || mathmlNode(domNode, "arcsinh") || mathmlNode(domNode, "arccosh") || mathmlNode(domNode, "arctanh")
                       || mathmlNode(domNode, "arcsech") || mathmlNode(domNode, "arccsch") || mathmlNode(domNode, "arccoth")) {
                if (childNodesCount != 2)
                    mErrorMessage = QObject::tr("An '%1' element must have one sibling.").arg(domNode.localName());
                else
                    return processFunctionNode(mMappings.value(domNode.localName()), pDomNode, pHasError);

            // Unknown node

            } else {
                processUnknownNode(domNode);

                return QString();
            }
        }
    } else if (mathmlNode(domNode, "piecewise")) {
        if (mPiecewiseStatementUsed) {
            mErrorMessage = QObject::tr("A 'piecewise' element cannot be used within another 'piecewise' element.");
        } else {
            QDomNode parentNode = domNode.parentNode();

            if (   (parentNode != mTopMathmlNode)
                || parentNode.localName().compare("apply")
                || parentNode.childNodes().item(0).localName().compare("eq")) {
                mErrorMessage = QObject::tr("A 'piecewise' element can only be used within a top-level 'apply' element that has an 'eq' element as its first child element.");
            } else if (!childNodesCount) {
                mErrorMessage = QObject::tr("A '%1' element must have at least one child element.").arg(domNode.localName());
            } else {
                mPiecewiseStatementUsed = true;

                return processPiecewiseNode(pDomNode, pHasError);
            }
        }
    } else if (mathmlNode(domNode, "piece")) {
        if (childNodesCount != 2)
            mErrorMessage = QObject::tr("A '%1' element must have two child elements.").arg(domNode.localName());
        else
            return processPieceNode(pDomNode, pHasError);
    } else if (mathmlNode(domNode, "otherwise")) {
        if (childNodesCount != 1)
            mErrorMessage = QObject::tr("An '%1' element must have one child element.").arg(domNode.localName());
        else
            return processOtherwiseNode(pDomNode, pHasError);

    // Token elements

    } else if (mathmlNode(domNode, "cn")) {
        QString type = domNode.attributes().namedItem("type").nodeValue().trimmed();

        if (type.isEmpty() || !type.compare("real")) {
            // Either no type (i.e. real type by default) or real type

            if (childNodesCount != 1)
                mErrorMessage = QObject::tr("A '%1' element must have a value.").arg(domNode.localName());
            else
                return childNodes.item(0).nodeValue().trimmed()+"{"+cellmlAttributeNodeValue(domNode, "units")+"}";
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
                } else if (childNode2.localName().compare("sep")) {
                    mErrorMessage = QObject::tr("The name of the second child element of a 'cn' element with an 'e-notation' type must be 'sep'.");
                } else if (childNode3.nodeType() != QDomNode::TextNode) {
                    mErrorMessage = QObject::tr("The third child element of a 'cn' element with an 'e-notation' type must be of 'text' type.");
                } else {
                    return childNode1.nodeValue().trimmed()+"e"+childNode3.nodeValue().trimmed()+"{"+cellmlAttributeNodeValue(domNode, "units")+"}";
                }
            }
        } else if (   !type.compare("integer")
                   || !type.compare("rational")
                   || !type.compare("complex-polar")
                   || !type.compare("complex-cartesian")
                   || !type.compare("constant")) {
            // A known, but unsupported type

            mErrorMessage = QObject::tr("The 'cn' element uses a '%1' type that is unsupported.").arg(type);
        } else {
            // An unknown type

            mErrorMessage = QObject::tr("The 'cn' element uses a '%1' type that is unknown.").arg(type);
        }
    } else if (mathmlNode(domNode, "ci")) {
        if (childNodesCount != 1)
            mErrorMessage = QObject::tr("A '%1' element must have a value.").arg(domNode.localName());
        else
            return childNodes.item(0).nodeValue().trimmed();

    // Qualifier elements

    } else if (   mathmlNode(domNode, "degree")
               || mathmlNode(domNode, "logbase")) {
        if (childNodesCount != 1)
            mErrorMessage = QObject::tr("A '%1' element must have one child element.").arg(domNode.localName());
        else
            return processMathmlNode(childNodes.item(0), pHasError);
    } else if (mathmlNode(domNode, "bvar")) {
        if ((childNodesCount != 1) && (childNodesCount != 2))
            mErrorMessage = QObject::tr("A '%1' element must have one or two child elements.").arg(domNode.localName());
        else
            return processBvarNode(pDomNode, pHasError);

    // Constants

    } else if (   mathmlNode(domNode, "true")
               || mathmlNode(domNode, "false")
               || mathmlNode(domNode, "notanumber")
               || mathmlNode(domNode, "pi")) {
        if (childNodesCount == 1)
            mErrorMessage = QObject::tr("A '%1' element cannot have a child element.").arg(domNode.localName());
        else if (childNodesCount)
            mErrorMessage = QObject::tr("A '%1' element cannot have child elements.").arg(domNode.localName());
        else
            return mMappings.value(domNode.localName());
    } else if (   mathmlNode(domNode, "infinity")
               || mathmlNode(domNode, "exponentiale")) {
        if (childNodesCount == 1)
            mErrorMessage = QObject::tr("An '%1' element cannot have a child element.").arg(domNode.localName());
        else if (childNodesCount)
            mErrorMessage = QObject::tr("An '%1' element cannot have child elements.").arg(domNode.localName());
        else
            return mMappings.value(domNode.localName());

    // Semantics and annotation elements

    } else if (mathmlNode(domNode, "semantics")) {
        processUnsupportedNode(domNode, false);

        return QString();
    } else if (   mathmlNode(domNode, "annotation")
               || mathmlNode(domNode, "annotation-xml")) {
        processUnsupportedNode(domNode, false, "n");

        return QString();

    // Unknown node

    } else {
        processUnknownNode(domNode);

        return QString();
    }

    mErrorLine = domNode.lineNumber();

    pHasError = true;

    return QString();
}

//==============================================================================

QString CellMLTextViewConverter::processPiecewiseNode(const QDomNode &pDomNode,
                                                      bool &pHasError)
{
    // Process the piecewise node

    QString res = "sel\n";

    indent();

    for (int i = 0, iMax = pDomNode.childNodes().count(); i < iMax; ++i) {
        res += processMathmlNode(pDomNode.childNodes().item(i), pHasError);

        if (pHasError)
            return QString();
    }

    unindent();

    return res+mIndent+"endsel";
}

//==============================================================================

QString CellMLTextViewConverter::processPieceNode(const QDomNode &pDomNode,
                                                  bool &pHasError)
{
    // Process the piece node

    QString statement = processMathmlNode(pDomNode.childNodes().item(0), pHasError);

    if (pHasError) {
        return QString();
    } else {
        QString condition = processMathmlNode(pDomNode.childNodes().item(1), pHasError);

        if (pHasError) {
            return QString();
        } else {
            QString res = mIndent+"case "+condition+":\n";

            indent();

            res += mIndent+statement+";\n";

            unindent();

            return res;
        }
    }
}

//==============================================================================

QString CellMLTextViewConverter::processOtherwiseNode(const QDomNode &pDomNode,
                                                      bool &pHasError)
{
    // Process the otherwise node

    QString statement = processMathmlNode(pDomNode.childNodes().item(0), pHasError);

    if (pHasError) {
        return QString();
    } else {
        QString res = mIndent+"otherwise:\n";

        indent();

        res += mIndent+statement+";\n";

        unindent();

        return res;
    }
}

//==============================================================================

QString CellMLTextViewConverter::processOperatorNode(const QString &pOperator,
                                                     const QDomNode &pDomNode,
                                                     bool &pHasError)
{
    // Process the operator node, based on its number of siblings

    QDomNodeList childNodes = pDomNode.childNodes();
    int childNodesCount = childNodes.count();

    if (childNodesCount == 2) {
        QDomNode operandNode = childNodes.item(1);
        QString operand = processMathmlNode(operandNode, pHasError);

        if (pHasError) {
            return QString();
        } else {
            if (mMathmlNodeTypes.value(childNodes.item(0).localName()) == PlusMathmlNode) {
                return operand;
            } else {
                // Minus node

                switch (mathmlNodeType(operandNode)) {
                case EqMathmlNode: case NeqMathmlNode: case GtMathmlNode: case LtMathmlNode: case GeqMathmlNode: case LeqMathmlNode:
                case PlusMathmlNode: case MinusMathmlNode:
                case AndMathmlNode: case OrMathmlNode: case XorMathmlNode:
                    return pOperator+"("+operand+")";

                    break;
                default:
                    return pOperator+operand;
                }
            }
        }
    } else {
        MathmlNodeType operatorNodeType = mathmlNodeType(childNodes.item(0));
        QDomNode leftOperandNode = childNodes.item(1);
        MathmlNodeType leftOperandNodeType = mathmlNodeType(leftOperandNode);
        QString leftOperand = processMathmlNode(leftOperandNode, pHasError);

        if (pHasError) {
            return QString();
        } else {
            QDomNode rightOperandNode;
            QString rightOperand;
            MathmlNodeType rightOperandNodeType;

            for (int i = 2; i < childNodesCount; ++i) {
                rightOperandNode = childNodes.item(i);
                rightOperand = processMathmlNode(rightOperandNode, pHasError);

                if (pHasError) {
                    return QString();
                } else {
                    rightOperandNodeType = mathmlNodeType(rightOperandNode);

                    switch (operatorNodeType) {
                    case PlusMathmlNode:
                        switch (leftOperandNodeType) {
                        case EqMathmlNode: case NeqMathmlNode: case GtMathmlNode: case LtMathmlNode: case GeqMathmlNode: case LeqMathmlNode:
                        case AndMathmlNode: case OrMathmlNode: case XorMathmlNode:
                            leftOperand = "("+leftOperand+")";

                            break;
                        default:
                            ;
                        }

                        switch (rightOperandNodeType) {
                        case EqMathmlNode: case NeqMathmlNode: case GtMathmlNode: case LtMathmlNode: case GeqMathmlNode: case LeqMathmlNode:
                        case AndMathmlNode: case OrMathmlNode: case XorMathmlNode:
                            rightOperand = "("+rightOperand+")";

                            break;
                        default:
                            ;
                        }

                        break;
                    case MinusMathmlNode:
                        switch (leftOperandNodeType) {
                        case EqMathmlNode: case NeqMathmlNode: case GtMathmlNode: case LtMathmlNode: case GeqMathmlNode: case LeqMathmlNode:
                        case AndMathmlNode: case OrMathmlNode: case XorMathmlNode:
                            leftOperand = "("+leftOperand+")";

                            break;
                        default:
                            ;
                        }

                        switch (rightOperandNodeType) {
                        case EqMathmlNode: case NeqMathmlNode: case GtMathmlNode: case LtMathmlNode: case GeqMathmlNode: case LeqMathmlNode:
                        case MinusMathmlNode:
                        case AndMathmlNode: case OrMathmlNode: case XorMathmlNode:
                            rightOperand = "("+rightOperand+")";

                            break;
                        case PlusMathmlNode:
                            if (rightOperandNode.childNodes().count() > 2)
                                rightOperand = "("+rightOperand+")";

                            break;
                        default:
                            ;
                        }

                        break;
                    case TimesMathmlNode:
                        switch (leftOperandNodeType) {
                        case EqMathmlNode: case NeqMathmlNode: case GtMathmlNode: case LtMathmlNode: case GeqMathmlNode: case LeqMathmlNode:
                        case AndMathmlNode: case OrMathmlNode: case XorMathmlNode:
                            leftOperand = "("+leftOperand+")";

                            break;
                        case PlusMathmlNode: case MinusMathmlNode:
                            if (leftOperandNode.childNodes().count() > 2)
                                leftOperand = "("+leftOperand+")";

                            break;
                        default:
                            ;
                        }

                        switch (rightOperandNodeType) {
                        case EqMathmlNode: case NeqMathmlNode: case GtMathmlNode: case LtMathmlNode: case GeqMathmlNode: case LeqMathmlNode:
                        case AndMathmlNode: case OrMathmlNode: case XorMathmlNode:
                            rightOperand = "("+rightOperand+")";

                            break;
                        case PlusMathmlNode: case MinusMathmlNode:
                            if (rightOperandNode.childNodes().count() > 2)
                                rightOperand = "("+rightOperand+")";

                            break;
                        default:
                            ;
                        }

                        break;
                    case DivideMathmlNode:
                        switch (leftOperandNodeType) {
                        case EqMathmlNode: case NeqMathmlNode: case GtMathmlNode: case LtMathmlNode: case GeqMathmlNode: case LeqMathmlNode:
                        case AndMathmlNode: case OrMathmlNode: case XorMathmlNode:
                            leftOperand = "("+leftOperand+")";

                            break;
                        case PlusMathmlNode: case MinusMathmlNode:
                            if (leftOperandNode.childNodes().count() > 2)
                                leftOperand = "("+leftOperand+")";

                            break;
                        default:
                            ;
                        }

                        switch (rightOperandNodeType) {
                        case EqMathmlNode: case NeqMathmlNode: case GtMathmlNode: case LtMathmlNode: case GeqMathmlNode: case LeqMathmlNode:
                        case TimesMathmlNode: case DivideMathmlNode:
                        case AndMathmlNode: case OrMathmlNode: case XorMathmlNode:
                            rightOperand = "("+rightOperand+")";

                            break;
                        case PlusMathmlNode: case MinusMathmlNode:
                            if (rightOperandNode.childNodes().count() > 2)
                                rightOperand = "("+rightOperand+")";

                            break;
                        default:
                            ;
                        }

                        break;
                    case AndMathmlNode:
                        switch (leftOperandNodeType) {
                        case EqMathmlNode: case NeqMathmlNode: case GtMathmlNode: case LtMathmlNode: case GeqMathmlNode: case LeqMathmlNode:
                        case OrMathmlNode: case XorMathmlNode:
                            leftOperand = "("+leftOperand+")";

                            break;
                        case PlusMathmlNode: case MinusMathmlNode:
                            if (leftOperandNode.childNodes().count() > 2)
                                leftOperand = "("+leftOperand+")";

                            break;
                        default:
                            ;
                        }

                        switch (rightOperandNodeType) {
                        case EqMathmlNode: case NeqMathmlNode: case GtMathmlNode: case LtMathmlNode: case GeqMathmlNode: case LeqMathmlNode:
                        case OrMathmlNode: case XorMathmlNode:
                            rightOperand = "("+rightOperand+")";

                            break;
                        case PlusMathmlNode: case MinusMathmlNode:
                            if (rightOperandNode.childNodes().count() > 2)
                                rightOperand = "("+rightOperand+")";

                            break;
                        default:
                            ;
                        }

                        break;
                    case OrMathmlNode:
                        switch (leftOperandNodeType) {
                        case EqMathmlNode: case NeqMathmlNode: case GtMathmlNode: case LtMathmlNode: case GeqMathmlNode: case LeqMathmlNode:
                        case AndMathmlNode: case XorMathmlNode:
                            leftOperand = "("+leftOperand+")";

                            break;
                        case PlusMathmlNode: case MinusMathmlNode:
                            if (leftOperandNode.childNodes().count() > 2)
                                leftOperand = "("+leftOperand+")";

                            break;
                        default:
                            ;
                        }

                        switch (rightOperandNodeType) {
                        case EqMathmlNode: case NeqMathmlNode: case GtMathmlNode: case LtMathmlNode: case GeqMathmlNode: case LeqMathmlNode:
                        case AndMathmlNode: case XorMathmlNode:
                            rightOperand = "("+rightOperand+")";

                            break;
                        case PlusMathmlNode: case MinusMathmlNode:
                            if (rightOperandNode.childNodes().count() > 2)
                                rightOperand = "("+rightOperand+")";

                            break;
                        default:
                            ;
                        }

                        break;
                    case XorMathmlNode:
                        switch (leftOperandNodeType) {
                        case EqMathmlNode: case NeqMathmlNode: case GtMathmlNode: case LtMathmlNode: case GeqMathmlNode: case LeqMathmlNode:
                        case AndMathmlNode: case OrMathmlNode:
                            leftOperand = "("+leftOperand+")";

                            break;
                        case PlusMathmlNode: case MinusMathmlNode:
                            if (leftOperandNode.childNodes().count() > 2)
                                leftOperand = "("+leftOperand+")";

                            break;
                        default:
                            ;
                        }

                        switch (rightOperandNodeType) {
                        case EqMathmlNode: case NeqMathmlNode: case GtMathmlNode: case LtMathmlNode: case GeqMathmlNode: case LeqMathmlNode:
                        case AndMathmlNode: case OrMathmlNode:
                            rightOperand = "("+rightOperand+")";

                            break;
                        case PlusMathmlNode: case MinusMathmlNode:
                            if (rightOperandNode.childNodes().count() > 2)
                                rightOperand = "("+rightOperand+")";

                            break;
                        default:
                            ;
                        }

                        break;
                    default:
                        ;
                    }

                    leftOperand = leftOperand+pOperator+rightOperand;

                    leftOperandNodeType = operatorNodeType;
                }
            }

            return leftOperand;
        }
    }
}

//==============================================================================

QString CellMLTextViewConverter::processFunctionNode(const QString &pFunction,
                                                     const QDomNode &pDomNode,
                                                     bool &pHasError)
{
    // Process the function node

    QString argument = processMathmlNode(pDomNode.childNodes().item(1), pHasError);

    if (pHasError)
        return QString();
    else
        return pFunction+"("+argument+")";
}

//==============================================================================

QString CellMLTextViewConverter::processPowerNode(const QDomNode &pDomNode,
                                                  bool &pHasError)
{
    // Process the power node

    QString a = processMathmlNode(pDomNode.childNodes().item(1), pHasError);

    if (pHasError) {
        return QString();
    } else {
        QString b = processMathmlNode(pDomNode.childNodes().item(2), pHasError);

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

QString CellMLTextViewConverter::processRootNode(const QDomNode &pDomNode,
                                                 bool &pHasError)
{
    // Process the root node, based on its number of arguments

    if (pDomNode.childNodes().count() == 2) {
        QString a = processMathmlNode(pDomNode.childNodes().item(1), pHasError);

        if (pHasError)
            return QString();
        else
            return "sqrt("+a+")";
    } else {
        QDomNode domNode = pDomNode.childNodes().item(1);

        if (domNode.localName().compare("degree")){
            mErrorMessage = QObject::tr("The first sibling of a '%1' element with two siblings must be a '%2' element.").arg("root")
                                                                                                                        .arg("degree");
            mErrorLine = domNode.lineNumber();

            pHasError = true;

            return QString();
        } else {
            QString b = processMathmlNode(domNode, pHasError);

            if (pHasError) {
                return QString();
            } else {
                QString a = processMathmlNode(pDomNode.childNodes().item(2), pHasError);

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

QString CellMLTextViewConverter::processLogNode(const QDomNode &pDomNode,
                                                bool &pHasError)
{
    // Process the log node

    QString argumentOrBase = processMathmlNode(pDomNode.childNodes().item(1), pHasError);

    if (pHasError) {
        return QString();
    } else {
        if (pDomNode.childNodes().count() == 2) {
            return "log("+argumentOrBase+")";
        } else {
            QString argument = processMathmlNode(pDomNode.childNodes().item(2), pHasError);

            if (pHasError)
                return QString();
            else
                return "log("+argument+", "+argumentOrBase+")";
        }
    }
}

//==============================================================================

QString CellMLTextViewConverter::processNotNode(const QDomNode &pDomNode,
                                                bool &pHasError)
{
    // Process the not node

    QDomNode domNode = pDomNode.childNodes().item(1);
    QString operand = processMathmlNode(domNode, pHasError);

    if (pHasError) {
        return QString();
    } else {
        switch (mathmlNodeType(domNode)) {
        case EqMathmlNode: case NeqMathmlNode: case GtMathmlNode: case LtMathmlNode: case GeqMathmlNode: case LeqMathmlNode:
        case PlusMathmlNode: case MinusMathmlNode: case TimesMathmlNode: case DivideMathmlNode:
        case AndMathmlNode: case OrMathmlNode: case XorMathmlNode:
            return "not("+operand+")";

            break;
        default:
            return "not "+operand;
        }
    }
}

//==============================================================================

QString CellMLTextViewConverter::processDiffNode(const QDomNode &pDomNode,
                                                 bool &pHasError)
{
    // Process the diff node

    QString f = processMathmlNode(pDomNode.childNodes().item(2), pHasError);

    if (pHasError) {
        return QString();
    } else {
        QDomNode domNode = pDomNode.childNodes().item(1);

        if (domNode.localName().compare("bvar")){
            mErrorMessage = QObject::tr("The first sibling of a '%1' element with two siblings must be a '%2' element.").arg("diff")
                                                                                                                        .arg("bvar");
            mErrorLine = domNode.lineNumber();

            pHasError = true;

            return QString();
        } else {
            QString x = processMathmlNode(pDomNode.childNodes().item(1), pHasError);

            if (pHasError)
                return QString();
            else
                return "ode("+f+", "+x+")";
        }
    }
}

//==============================================================================

QString CellMLTextViewConverter::processBvarNode(const QDomNode &pDomNode,
                                                 bool &pHasError)
{
    // Process the bvar node, based on its number of child elements

    if (pDomNode.childNodes().count() == 1) {
        return processMathmlNode(pDomNode.childNodes().item(0), pHasError);
    } else {
        QDomNode domNode = pDomNode.childNodes().item(1);

        if (domNode.localName().compare("degree")){
            mErrorMessage = QObject::tr("The second child element of a '%1' element with two child elements must be a '%2' element.").arg("bvar")
                                                                                                                                     .arg("degree");
            mErrorLine = domNode.lineNumber();

            pHasError = true;

            return QString();
        } else {
            QString b = processMathmlNode(domNode, pHasError);

            if (pHasError) {
                return QString();
            } else {
                QString a = processMathmlNode(pDomNode.childNodes().item(0), pHasError);

                if (pHasError)
                    return QString();
                else
                    return a+", "+b;
            }
        }
    }
}

//==============================================================================

bool CellMLTextViewConverter::processReactionNode(const QDomNode &pDomNode)
{
    // We don't support reaction elements

    processUnsupportedNode(pDomNode, true);

    return false;
}

//==============================================================================

bool CellMLTextViewConverter::processGroupNode(const QDomNode &pDomNode)
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

    QString relationshipReference = QString();

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        if (domNode.isComment()) {
            processCommentNode(domNode);
        } else if (rdfNode(domNode)) {
            processRdfNode(domNode);
        } else if (cellmlNode(domNode, "relationship_ref")) {
            if (!processRelationshipRefNode(domNode, relationshipReference))
                return false;
        } else if (cellmlNode(domNode, "component_ref")) {
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

bool CellMLTextViewConverter::processRelationshipRefNode(const QDomNode &pDomNode,
                                                         QString &pRelationshipRef)
{
    // Process the given relationship ref node's children
    // Note #1: unlike for most nodes, we process the node's children before
    //          processing the node itself since it doesn't output any line as
    //          such, so it's not like we can put a comment within a
    //          def...enddef; statement...
    // Note #2: when parsed back, comments will not be in the relationship ref
    //          node anymore, but in its parent...

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        if (domNode.isComment())
            processCommentNode(domNode);
        else if (rdfNode(domNode))
            processRdfNode(domNode);
        else
            processUnknownNode(domNode);
    }

    // Process the given relationship ref node

    QString relationship = cellmlAttributeNodeValue(pDomNode, "relationship");
    QString name = cellmlAttributeNodeValue(pDomNode, "name", false);
    bool isEncapsulation = false;

    if (   !pDomNode.namespaceURI().compare(CellMLSupport::Cellml_1_0_Namespace)
        || !pDomNode.namespaceURI().compare(CellMLSupport::Cellml_1_1_Namespace)) {
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

void CellMLTextViewConverter::processComponentRefNode(const QDomNode &pDomNode)
{
    // Determine whether the given component ref node has component ref children

    bool hasComponentRefChildren = false;

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling())  {
        if (!domNode.localName().compare("component_ref")) {
            hasComponentRefChildren = true;

            break;
        }
    }

    // Start processing the given component ref node

    if (hasComponentRefChildren) {
        if (   (mLastOutputType == Comment)
            || (mLastOutputType == Comp) || (mLastOutputType == EndComp))
            outputString();

        outputString(CompIncl,
                     QString("comp%1 %2 incl").arg(cmetaId(pDomNode))
                                              .arg(cellmlAttributeNodeValue(pDomNode, "component")));

        indent();
    }

    // Process the given component ref node's children

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        if (domNode.isComment())
            processCommentNode(domNode);
        else if (rdfNode(domNode))
            processRdfNode(domNode);
        else if (cellmlNode(domNode, "component_ref"))
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
                                          .arg(cellmlAttributeNodeValue(pDomNode, "component")));
    }

}

//==============================================================================

bool CellMLTextViewConverter::processConnectionNode(const QDomNode &pDomNode)
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

    QString mapComponents = QString();

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        if (domNode.isComment()) {
            processCommentNode(domNode);
        } else if (rdfNode(domNode)) {
            processRdfNode(domNode);
        } else if (cellmlNode(domNode, "map_components")) {
            if (!processMapComponentsNode(domNode, mapComponents))
                return false;
        } else if (cellmlNode(domNode, "map_variables")) {
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

bool CellMLTextViewConverter::processMapComponentsNode(const QDomNode &pDomNode,
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

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        if (domNode.isComment())
            processCommentNode(domNode);
        else if (rdfNode(domNode))
            processRdfNode(domNode);
        else
            processUnknownNode(domNode);
    }

    // Process the given map components node

    pMapComponents = QString("between%1 %2 and %3").arg(cmetaId(pDomNode))
                                                   .arg(cellmlAttributeNodeValue(pDomNode, "component_1"))
                                                   .arg(cellmlAttributeNodeValue(pDomNode, "component_2"));

    return true;
}

//==============================================================================

void CellMLTextViewConverter::processMapVariablesNode(const QDomNode &pDomNode)
{
    // Process the given map variables node's children
    // Note #1: unlike for most nodes, we process the node's children before
    //          processing the node itself since it only outputs one line, so
    //          it's not like we can put a comment within a def...enddef;
    //          statement...
    // Note #2: when parsed back, comments will not be in the map variables node
    //          anymore, but in its parent...

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        if (domNode.isComment())
            processCommentNode(domNode);
        else if (rdfNode(domNode))
            processRdfNode(domNode);
        else
            processUnknownNode(domNode);
    }

    // Process the given unit node

    if (mLastOutputType == Comment)
        outputString();

    outputString(Vars,
                 QString("vars%1 %2 and %3;").arg(cmetaId(pDomNode))
                                             .arg(cellmlAttributeNodeValue(pDomNode, "variable_1"))
                                             .arg(cellmlAttributeNodeValue(pDomNode, "variable_2")));
}

//==============================================================================

void CellMLTextViewConverter::processUnknownNode(const QDomNode &pDomNode)
{
    // The given node is unknown, so warn the user about it

    mWarnings << CellMLTextViewConverterWarning(pDomNode.lineNumber(),
                                                QObject::tr("A '%1' element was found%2, but it is not known and cannot therefore be processed.").arg(pDomNode.prefix().isEmpty()?
                                                                                                                                                          pDomNode.localName():
                                                                                                                                                          pDomNode.prefix()+":"+pDomNode.localName()));
}

//==============================================================================

void CellMLTextViewConverter::processUnsupportedNode(const QDomNode &pDomNode,
                                                     const bool &pError,
                                                     const QString &pExtra)
{
    // The given node is known, but we don't support it, so consider it as an
    // error or a warning, depending on the case

    int lineNumber = pDomNode.lineNumber();
    QString message = QObject::tr("A%1 '%2' element was found in the original CellML file, but it is not supported and cannot therefore be processed.").arg(pExtra)
                                                                                                                                                       .arg(pDomNode.prefix().isEmpty()?
                                                                                                                                                                pDomNode.localName():
                                                                                                                                                                pDomNode.prefix()+":"+pDomNode.localName());

    if (pError) {
        mErrorLine = lineNumber;
        mErrorMessage = message;
    } else {
        mWarnings << CellMLTextViewConverterWarning(lineNumber, message);
    }
}

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
