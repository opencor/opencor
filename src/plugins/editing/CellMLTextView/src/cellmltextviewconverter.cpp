/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Raw CellML to CellML Text converter
//==============================================================================

#include "cellmlfile.h"
#include "cellmltextviewconverter.h"
#include "corecliutils.h"

//==============================================================================

#include <QDomDocument>
#include <QRegularExpression>

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

CellMLTextViewConverterWarning::CellMLTextViewConverterWarning(const QString &pMessage,
                                                               int pLineNumber,
                                                               int pColumnNumber) :
    mMessage(pMessage),
    mLineNumber(pLineNumber),
    mColumnNumber(pColumnNumber)
{
}

//==============================================================================

QString CellMLTextViewConverterWarning::message() const
{
    // Return our message

    return mMessage;
}

//==============================================================================

int CellMLTextViewConverterWarning::lineNumber() const
{
    // Return our line number

    return mLineNumber;
}

//==============================================================================

int CellMLTextViewConverterWarning::columnNumber() const
{
    // Return our column number

    return mColumnNumber;
}

//==============================================================================

CellMLTextViewConverter::CellMLTextViewConverter()
{
    // Reset our internals

    reset();

    // Mappings for relational operators

                                           mMathmlNodes.insert("eq", MathmlNode::Eq);
    mMappings.insert("neq", " <> ");       mMathmlNodes.insert("neq", MathmlNode::Neq);
    mMappings.insert("lt", " < ");         mMathmlNodes.insert("lt", MathmlNode::Lt);
    mMappings.insert("leq", " <= ");       mMathmlNodes.insert("leq", MathmlNode::Leq);
    mMappings.insert("geq", " >= ");       mMathmlNodes.insert("geq", MathmlNode::Geq);
    mMappings.insert("gt", " > ");         mMathmlNodes.insert("gt", MathmlNode::Gt);

    // Mappings for arithmetic operators

    mMappings.insert("plus", "+");         mMathmlNodes.insert("plus", MathmlNode::Plus);
    mMappings.insert("minus", "-");        mMathmlNodes.insert("minus", MathmlNode::Minus);
    mMappings.insert("times", "*");        mMathmlNodes.insert("times", MathmlNode::Times);
    mMappings.insert("divide", "/");       mMathmlNodes.insert("divide", MathmlNode::Divide);
    mMappings.insert("ceiling", "ceil");
    mMappings.insert("floor", "floor");
    mMappings.insert("factorial", "fact");

    // Mappings for arithmetic operators

    mMappings.insert("and", " and ");      mMathmlNodes.insert("and", MathmlNode::And);
    mMappings.insert("or", " or ");        mMathmlNodes.insert("or", MathmlNode::Or);
    mMappings.insert("xor", " xor ");      mMathmlNodes.insert("xor", MathmlNode::Xor);

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

bool CellMLTextViewConverter::execute(const QString &pRawCellml)
{
    // Reset our internals

    reset();

    // Convert the raw CellML to CellML Text by first getting a DOM
    // representation of it

    QDomDocument domDocument;

    if (domDocument.setContent(pRawCellml, true,
                               &mErrorMessage, &mErrorLine, &mErrorColumn)) {
        // Process the DOM document's children, skipping the first node if it is
        // an XML processing instruction

        QDomNode domNode = domDocument.firstChild();

        if (domNode.isProcessingInstruction() && (domNode.nodeName() == "xml")) {
            domNode = domNode.nextSibling();
        }

        for (; !domNode.isNull(); domNode = domNode.nextSibling()) {
            if (domNode.isComment()) {
                processCommentNode(domNode);
            } else if (rdfNode(domNode)) {
                processRdfNode(domNode);
            } else if (cellmlNode(domNode, "model")) {
                if (!processModelNode(domNode)) {
                    mOutput = pRawCellml;

                    return false;
                }
            } else if (!processUnknownNode(domNode, true)) {
                mOutput = pRawCellml;

                return false;
            }
        }

        return true;
    }

    mOutput = pRawCellml;

    return false;
}

//==============================================================================

QString CellMLTextViewConverter::output() const
{
    // Return our output

    return mOutput;
}

//==============================================================================

QString CellMLTextViewConverter::errorMessage() const
{
    // Return our error message

    return mErrorMessage;
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

bool CellMLTextViewConverter::hasWarnings() const
{
    // Return whether we have warnings

    return !mWarnings.isEmpty();
}

//==============================================================================

CellMLTextViewConverterWarnings CellMLTextViewConverter::warnings() const
{
    // Return our warnings

    return mWarnings;
}

//==============================================================================

QDomNode CellMLTextViewConverter::documentationNode() const
{
    // Return our documentation ndoe

    return mDocumentationNode;
}

//==============================================================================

QDomDocument CellMLTextViewConverter::rdfNodes() const
{
    // Return our RDF nodes

    return mRdfNodes;
}

//==============================================================================

void CellMLTextViewConverter::reset()
{
    // Reset our internals

    mOutput = QString();
    mPrevIndent = QString();
    mIndent = QString();

    mLastOutput = Output::None;

    mErrorMessage = QString();
    mErrorLine = -1;
    mErrorColumn = -1;

    mWarnings = CellMLTextViewConverterWarnings();

    mModelNode = QDomNode();
    mDocumentationNode = QDomNode();
    mTopMathmlNode = QDomNode();

    mRdfNodes = QDomDocument(QString());

    mAssignmentDone = false;
    mOldPiecewiseStatementUsed = false;
    mPiecewiseStatementUsed = false;
}

//==============================================================================

static const char *Indent   = "    ";
static const int IndentSize = 4;

//==============================================================================

void CellMLTextViewConverter::indent(bool pForceTracking)
{
    // Indent our output and keep track of it, if required

    mIndent += Indent;

    if (pForceTracking) {
        mPrevIndent = mIndent;
    }
}

//==============================================================================

void CellMLTextViewConverter::unindent()
{
    // Unindent our output

    mIndent.chop(IndentSize);
}

//==============================================================================

void CellMLTextViewConverter::outputString(Output pOutputType,
                                           const QString &pString)
{
    // Output the given string

    if (pString.isEmpty()) {
        if (mLastOutput != Output::EmptyLine) {
            mOutput += '\n';
        }
    } else {
        if (pOutputType == Output::Comment) {
            // When converting a comment that is within a piecewise equation,
            // mIndent will be wrong (since it will have been 'incremented'), so
            // we need to rely on the indent that we previously used

            mOutput += mPrevIndent+pString+"\n";
        } else {
            mOutput += mIndent+pString+"\n";

            mPrevIndent = mIndent;
        }
    }

    mLastOutput = pOutputType;
}

//==============================================================================

bool CellMLTextViewConverter::rdfNode(const QDomNode &pDomNode) const
{
    // Return whether the given node is an RDF node

    static const QString Rdf = "RDF";

    return    (pDomNode.localName() == Rdf)
           && (pDomNode.namespaceURI() == CellMLSupport::RdfNamespace);
}

//==============================================================================

bool CellMLTextViewConverter::cellmlNode(const QDomNode &pDomNode,
                                         const QString &pName) const
{
    // Return whether the given node is a CellML node with the given name

    return    (pDomNode.localName() == pName)
           && (   (pDomNode.namespaceURI() == CellMLSupport::Cellml_1_0_Namespace)
               || (pDomNode.namespaceURI() == CellMLSupport::Cellml_1_1_Namespace));
}

//==============================================================================

bool CellMLTextViewConverter::mathmlNode(const QDomNode &pDomNode,
                                         const QString &pName) const
{
    // Return whether the given node is a MathML node with the given name

    return    (pDomNode.localName() == pName)
           && (pDomNode.namespaceURI() == CellMLSupport::MathmlNamespace);
}

//==============================================================================

QString CellMLTextViewConverter::cmetaId(const QDomNode &pDomNode) const
{
    // Return the converted cmeta:id, if any, of the given node

    QString cmetaIdValue = attributeNodeValue(pDomNode, CellMLSupport::CmetaIdNamespace, "id", false);

    if (!cmetaIdValue.isEmpty()) {
        return QString("{%1}").arg(cmetaIdValue);
    }

    return {};
}

//==============================================================================

QString CellMLTextViewConverter::attributeNodeValue(const QDomNode &pDomNode,
                                                    const QString &pNamespace,
                                                    const QString &pName,
                                                    bool pMustBePresent) const
{
    // Return the trimmed value of the requested attribute using the given
    // namespace
    // Note: there is an issue with QDomNamedNodeMap::namedItemNS(). Indeed, if
    //       the attribute that defines the namespace is after the attribute
    //       itself, then the attribute will only be accessible without using a
    //       namespace (see https://bugreports.qt.io/browse/QTBUG-59932)...

    QDomNamedNodeMap domNodeAttributes = pDomNode.attributes();
    QDomNode attributeNode = domNodeAttributes.namedItemNS(pNamespace, pName);

    if (attributeNode.isNull()) {
        attributeNode = domNodeAttributes.namedItem(pName);
    }

    QString res = attributeNode.nodeValue().trimmed();

    if (res.isEmpty()) {
        return pMustBePresent?"???":res;
    }

    return res;
}

//==============================================================================

QString CellMLTextViewConverter::cellmlAttributeNodeValue(const QDomNode &pDomNode,
                                                          const QString &pName,
                                                          bool pMustBePresent) const
{
    // Return the requested CellML attribute using the CellML 1.0 namespace or,
    // if needed, the CellML 1.1 namespace

    QString res = attributeNodeValue(pDomNode, CellMLSupport::Cellml_1_0_Namespace, pName, false);

    if (res.isEmpty()) {
        res = attributeNodeValue(pDomNode, CellMLSupport::Cellml_1_1_Namespace, pName, false);
    }

    if (res.isEmpty()) {
        return pMustBePresent?"???":res;
    }

    return res;
}

//==============================================================================

CellMLTextViewConverter::MathmlNode CellMLTextViewConverter::mathmlNode(const QDomNode &pDomNode) const
{
    // Return the MathML type of the given node

    static const QString Apply = "apply";

    if (   (pDomNode.localName() == Apply)
        && (pDomNode.namespaceURI() == CellMLSupport::MathmlNamespace)) {
        return mMathmlNodes.value(childNode(pDomNode, 0).localName());
    }

    return mMathmlNodes.value(pDomNode.localName());
}

//==============================================================================

bool CellMLTextViewConverter::isRelationalOperator(MathmlNode pOperandNodeType) const
{
    // Return whether the given operand node type is a relational operator

    return    (pOperandNodeType == MathmlNode::Eq)
           || (pOperandNodeType == MathmlNode::Neq)
           || (pOperandNodeType == MathmlNode::Gt)
           || (pOperandNodeType == MathmlNode::Lt)
           || (pOperandNodeType == MathmlNode::Geq)
           || (pOperandNodeType == MathmlNode::Leq);
}

//==============================================================================

bool CellMLTextViewConverter::isPlusOperator(MathmlNode pOperandNodeType) const
{
    // Return whether the given operand node type is a plus operator

    return pOperandNodeType == MathmlNode::Plus;
}

//==============================================================================

bool CellMLTextViewConverter::isMinusOperator(MathmlNode pOperandNodeType) const
{
    // Return whether the given operand node type is a minus operator

    return pOperandNodeType == MathmlNode::Minus;
}

//==============================================================================

bool CellMLTextViewConverter::isTimesOperator(MathmlNode pOperandNodeType) const
{
    // Return whether the given operand node type is a times operator

    return pOperandNodeType == MathmlNode::Times;
}

//==============================================================================

bool CellMLTextViewConverter::isDivideOperator(MathmlNode pOperandNodeType) const
{
    // Return whether the given operand node type is a times operator

    return pOperandNodeType == MathmlNode::Divide;
}

//==============================================================================

bool CellMLTextViewConverter::isLogicalOrBitwiseOperator(MathmlNode pOperandNodeType) const
{
    // Return whether the given operand node type is a logical or bitwise
    // operator

    return    (pOperandNodeType == MathmlNode::And)
           || (pOperandNodeType == MathmlNode::Or)
           || (pOperandNodeType == MathmlNode::Xor);
}

//==============================================================================

bool CellMLTextViewConverter::isAndOperator(MathmlNode pOperandNodeType) const
{
    // Return whether the given operand node type is an and operator

    return pOperandNodeType == MathmlNode::And;
}

//==============================================================================

bool CellMLTextViewConverter::isOrOperator(MathmlNode pOperandNodeType) const
{
    // Return whether the given operand node type is an or operator

    return pOperandNodeType == MathmlNode::Or;
}

//==============================================================================

bool CellMLTextViewConverter::isXorOperator(MathmlNode pOperandNodeType) const
{
    // Return whether the given operand node type is a xor operator

    return pOperandNodeType == MathmlNode::Xor;
}

//==============================================================================

bool CellMLTextViewConverter::processModelNode(const QDomNode &pDomNode)
{
    // Start processing the given model node

    if (mLastOutput == Output::Comment) {
        outputString();
    }

    outputString(Output::DefModel,
                 QString("def model%1 %2 as").arg(cmetaId(pDomNode),
                                                  cellmlAttributeNodeValue(pDomNode, "name")));

    indent();

    // Keep track of the given model node and of its attributes

    mModelNode = pDomNode;
    mAttributes = pDomNode.attributes();

    // Process the given model node's children

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        if (domNode.isComment()) {
            processCommentNode(domNode);
        } else if (rdfNode(domNode)) {
            processRdfNode(domNode);
        } else if (cellmlNode(domNode, "import")) {
            if (!processImportNode(domNode)) {
                return false;
            }
        } else if (cellmlNode(domNode, "units")) {
            if (!processUnitsNode(domNode)) {
                return false;
            }
        } else if (cellmlNode(domNode, "component")) {
            if (!processComponentNode(domNode)) {
                return false;
            }
        } else if (cellmlNode(domNode, "group")) {
            if (!processGroupNode(domNode)) {
                return false;
            }
        } else if (cellmlNode(domNode, "connection")) {
            if (!processConnectionNode(domNode)) {
                return false;
            }
        } else if (!processUnknownNode(domNode, false)) {
            return false;
        }
    }

    // Finish processing the given model node

    unindent();

    outputString(Output::EndDef, "enddef;");

    return true;
}

//==============================================================================

QString CellMLTextViewConverter::processCommentString(const QString &pComment)
{
    // An empty comment will have been serialised as "<!-- -->", which means
    // that pComment will be equal to " ", in which case we need to return an
    // empty string

    static const QString EmptyComment = " ";

    if (pComment == EmptyComment) {
        return {};
    }

    // We cannot have "--" in an XML comment, so those were replaced with
    // "&#45;&#45;", which means that we need to convert them back

    QString realComment = pComment;

    realComment.replace("&#45;&#45;", "--");

    // If newComment ends with "- ", it's because the comment ends with a dash,
    // but that a space had to be added to it to avoid problems with the end of
    // an XML comment ("-->"), so we need to chop that space to get the real
    // comment

    if (realComment.endsWith("- ")) {
        realComment.chop(1);
    }

    // We are all done, so we can now return our real comment

    return realComment;
}

//==============================================================================

void CellMLTextViewConverter::processCommentNode(const QDomNode &pDomNode)
{
    // Process the given comment node

    QStringList commentLines = pDomNode.nodeValue().remove('\r').split('\n');
    // Note #1: we don't know which end of line string is used by the file, so
    //          the above code ensures that we can handle both "\r\n" on Windows
    //          and "\n" on Linux/macOS...
    // Note #2: unlike for other nodes, we don't trim its value since leading
    //          spaces may be used in a comment...

    if (   (mLastOutput == Output::Comment)
        || (mLastOutput == Output::Comp) || (mLastOutput == Output::DefBaseUnit)
        || (mLastOutput == Output::EndComp) || (mLastOutput == Output::EndDef)
        || (mLastOutput == Output::Equation) || (mLastOutput == Output::ImportComp)
        || (mLastOutput == Output::ImportUnit) || (mLastOutput == Output::Unit)
        || (mLastOutput == Output::Var) || (mLastOutput == Output::Vars)) {
        outputString();
    }

    for (const auto &commentLine : commentLines) {
        outputString(Output::Comment, QString("//%1").arg(processCommentString(commentLine)));
    }
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

    if (   (mLastOutput == Output::Comment)
        || (mLastOutput == Output::EndDef)) {
        outputString();
    }

    outputString(Output::DefImport,
                 QString(R"(def import%1 using "%2" for)").arg(cmetaId(pDomNode),
                                                               attributeNodeValue(pDomNode, CellMLSupport::XlinkNamespace, "href")));

    indent();

    // Process the given import node's children

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        if (domNode.isComment()) {
            processCommentNode(domNode);
        } else if (rdfNode(domNode)) {
            processRdfNode(domNode);
        } else if (cellmlNode(domNode, "units")) {
            if (!processUnitsNode(domNode, true)) {
                return false;
            }
        } else if (cellmlNode(domNode, "component")) {
            if (!processComponentNode(domNode, true)) {
                return false;
            }
        } else if (!processUnknownNode(domNode, false)) {
            return false;
        }
    }

    // Finish processing the given group node

    unindent();

    outputString(Output::EndDef, "enddef;");

    return true;
}

//==============================================================================

bool CellMLTextViewConverter::processUnitsNode(const QDomNode &pDomNode,
                                               bool pInImportNode)
{
    // Start processing the given units node

    static const QString Yes = "yes";
    static const QString No  = "no";

    QString baseUnits = cellmlAttributeNodeValue(pDomNode, "base_units", false);

    if (!baseUnits.isEmpty() && (baseUnits != Yes) && (baseUnits != No)) {
        mErrorMessage = tr("A 'base_units' attribute must have a value of 'yes' or 'no'.");
        mErrorLine = pDomNode.lineNumber();
        mErrorColumn = pDomNode.columnNumber();

        return false;
    }

    bool isBaseUnits = baseUnits == Yes;

    if (!pInImportNode && !isBaseUnits) {
        if (   (mLastOutput == Output::Comment)
            || (mLastOutput == Output::DefBaseUnit) || (mLastOutput == Output::EndDef)
            || (mLastOutput == Output::Equation) || (mLastOutput == Output::Var)) {
            outputString();
        }

        outputString(Output::DefUnit,
                     QString("def unit%1 %2 as").arg(cmetaId(pDomNode),
                                                     cellmlAttributeNodeValue(pDomNode, "name")));

        indent();
    }

    // Process the given units node's children

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        if (domNode.isComment()) {
            processCommentNode(domNode);
        } else if (rdfNode(domNode)) {
            processRdfNode(domNode);
        } else if (!isBaseUnits && !pInImportNode && cellmlNode(domNode, "unit")) {
            if (!processUnitNode(domNode)) {
                return false;
            }
        } else if (!processUnknownNode(domNode, false)) {
            return false;
        }
    }

    // Finish processing the given units node

    if (pInImportNode) {
        if (   (mLastOutput == Output::Comment)
            || (mLastOutput == Output::ImportComp)) {
            outputString();
        }

        outputString(Output::ImportUnit,
                     QString("unit%1 %2 using unit %3;").arg(cmetaId(pDomNode),
                                                             cellmlAttributeNodeValue(pDomNode, "name"),
                                                             cellmlAttributeNodeValue(pDomNode, "units_ref")));
    } else if (isBaseUnits) {
        if (   (mLastOutput == Output::Comment)
            || (mLastOutput == Output::EndDef)
            || (mLastOutput == Output::Equation)
            || (mLastOutput == Output::Var)) {
            outputString();
        }

        outputString(Output::DefBaseUnit,
                     QString("def unit%1 %2 as base unit;").arg(cmetaId(pDomNode),
                                                                cellmlAttributeNodeValue(pDomNode, "name")));
    } else {
        unindent();

        outputString(Output::EndDef, "enddef;");
    }

    return true;
}

//==============================================================================

bool CellMLTextViewConverter::processUnitNode(const QDomNode &pDomNode)
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
        if (domNode.isComment()) {
            processCommentNode(domNode);
        } else if (rdfNode(domNode)) {
            processRdfNode(domNode);
        } else if (!processUnknownNode(domNode, false)) {
            return false;
        }
    }

    // Process the given unit node

    QString parameters;
    QString prefix = cellmlAttributeNodeValue(pDomNode, "prefix", false);
    QString exponent = cellmlAttributeNodeValue(pDomNode, "exponent", false);
    QString multiplier = cellmlAttributeNodeValue(pDomNode, "multiplier", false);
    QString offset = cellmlAttributeNodeValue(pDomNode, "offset", false);

    if (!prefix.isEmpty()) {
        parameters += "pref: "+prefix;
    }

    if (!exponent.isEmpty()) {
        parameters += (parameters.isEmpty()?QString():", ")+"expo: "+exponent;
    }

    if (!multiplier.isEmpty()) {
        parameters += (parameters.isEmpty()?QString():", ")+"mult: "+multiplier;
    }

    if (!offset.isEmpty()) {
        parameters += (parameters.isEmpty()?QString():", ")+"off: "+offset;
    }

    if (mLastOutput == Output::Comment) {
        outputString();
    }

    outputString(Output::Unit,
                 QString("unit%1 %2%3;").arg(cmetaId(pDomNode),
                                             cellmlAttributeNodeValue(pDomNode, "units"),
                                             parameters.isEmpty()?QString():" {"+parameters+"}"));

    return true;
}

//==============================================================================

bool CellMLTextViewConverter::processComponentNode(const QDomNode &pDomNode,
                                                   bool pInImportNode)
{
    // Start processing the given component node

    if (!pInImportNode) {
        if (   (mLastOutput == Output::Comment)
            || (mLastOutput == Output::EndDef)) {
            outputString();
        }

        outputString(Output::DefComp,
                     QString("def comp%1 %2 as").arg(cmetaId(pDomNode),
                                                     cellmlAttributeNodeValue(pDomNode, "name")));

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
            if (!processUnitsNode(domNode)) {
                return false;
            }
        } else if (!pInImportNode && cellmlNode(domNode, "variable")) {
            if (!processVariableNode(domNode)) {
                return false;
            }
        } else if (!pInImportNode && mathmlNode(domNode, "math")) {
            if (!processMathNode(domNode)) {
                return false;
            }
        } else if (!pInImportNode && cellmlNode(domNode, "reaction")) {
            if (!processReactionNode(domNode)) {
                return false;
            }
        } else if (!processUnknownNode(domNode, false)) {
            return false;
        }
    }

    // Finish processing the given component node

    if (pInImportNode) {
        if (   (mLastOutput == Output::Comment)
            || (mLastOutput == Output::ImportUnit)) {
            outputString();
        }

        outputString(Output::ImportComp,
                     QString("comp%1 %2 using comp %3;").arg(cmetaId(pDomNode),
                                                             cellmlAttributeNodeValue(pDomNode, "name"),
                                                             cellmlAttributeNodeValue(pDomNode, "component_ref")));
    } else {
        unindent();

        outputString(Output::EndDef, "enddef;");
    }

    return true;
}

//==============================================================================

bool CellMLTextViewConverter::processVariableNode(const QDomNode &pDomNode)
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
        if (domNode.isComment()) {
            processCommentNode(domNode);
        } else if (rdfNode(domNode)) {
            processRdfNode(domNode);
        } else if (!processUnknownNode(domNode, false)) {
            return false;
        }
    }

    // Process the given variable node

    QString parameters;
    QString initialValue = cellmlAttributeNodeValue(pDomNode, "initial_value", false);
    QString publicInterface = cellmlAttributeNodeValue(pDomNode, "public_interface", false);
    QString privateInterface = cellmlAttributeNodeValue(pDomNode, "private_interface", false);

    if (!initialValue.isEmpty()) {
        parameters += "init: "+initialValue;
    }

    if (!publicInterface.isEmpty()) {
        parameters += (parameters.isEmpty()?QString():", ")+"pub: "+publicInterface;
    }

    if (!privateInterface.isEmpty()) {
        parameters += (parameters.isEmpty()?QString():", ")+"priv: "+privateInterface;
    }

    if (   (mLastOutput == Output::Comment)
        || (mLastOutput == Output::DefBaseUnit)
        || (mLastOutput == Output::EndDef)
        || (mLastOutput == Output::Equation)) {
        outputString();
    }

    outputString(Output::Var,
                 QString("var%1 %2: %3%4;").arg(cmetaId(pDomNode),
                                                cellmlAttributeNodeValue(pDomNode, "name"),
                                                cellmlAttributeNodeValue(pDomNode, "units"),
                                                parameters.isEmpty()?QString():" {"+parameters+"}"));

    return true;
}

//==============================================================================

bool CellMLTextViewConverter::processMathNode(const QDomNode &pDomNode)
{
    // Process the given math node's children

    QString equation;
    bool hasError;

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        if (domNode.isComment()) {
            processCommentNode(domNode);
        } else if (rdfNode(domNode)) {
            processRdfNode(domNode);
        } else {
            mOldPiecewiseStatementUsed = mPiecewiseStatementUsed;
            mAssignmentDone = mPiecewiseStatementUsed = hasError = false;
            mTopMathmlNode = domNode;
            equation = processMathmlNode(domNode, hasError);

            if (hasError) {
                return false;
            }

            if (!equation.isEmpty()) {
                // Note: should one or several warnings be generated, then it
                //       may be possible that no equation has been generated,
                //       hence our check...

                if (   (mLastOutput == Output::Comment)
                    || (mLastOutput == Output::DefBaseUnit)
                    || (mLastOutput == Output::EndDef)
                    || (mLastOutput == Output::Var)
                    ||  mPiecewiseStatementUsed
                    || (mPiecewiseStatementUsed != mOldPiecewiseStatementUsed)) {
                    outputString();
                }

                outputString(Output::Equation, equation+";");
            }
        }
    }

    return true;
}

//==============================================================================

int CellMLTextViewConverter::childNodesCount(const QDomNode &pDomNode) const
{
    // Return the number of child elements in the given node

    int res = 0;
    QDomNodeList childNodes = pDomNode.childNodes();

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        if (!childNodes.item(i).isComment()) {
            ++res;
        }
    }

    return res;
}

//==============================================================================

QDomNode CellMLTextViewConverter::childNode(const QDomNode &pDomNode,
                                            int pChildNodeIndex) const
{
    // Return the nth child element of the given node

    int childNodeIndex = 0;
    QDomNodeList childNodes = pDomNode.childNodes();

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        if (!childNodes.item(i).isComment()) {
            if (childNodeIndex == pChildNodeIndex) {
                return childNodes.item(i);
            }

            ++childNodeIndex;
        }
    }

    return {};
}

//==============================================================================

QString CellMLTextViewConverter::processMathmlNode(const QDomNode &pDomNode,
                                                   bool &pHasError)
{
    // Make sure that we are not coming here with an error

    if (pHasError) {
        return {};
    }

    // Process the given MathML node and its children, if any

    QDomNode domNode = pDomNode;
    int currentChildNodesCount = childNodesCount(pDomNode);

    // Basic content elements

    if (domNode.isComment()) {
        processCommentNode(domNode);

        return {};
    }

    if (mathmlNode(domNode, "apply")) {
        // Make sure that we have at least one child

        if (currentChildNodesCount == 0) {
            mErrorMessage = tr("An '%1' element must have at least one child element.").arg(domNode.localName());
        } else {
            domNode = childNode(domNode, 0);

            // Relational operators

            if (mathmlNode(domNode, "eq")) {
                if (currentChildNodesCount != 3) {
                    mErrorMessage = tr("An '%1' element must have two siblings.").arg(domNode.localName());
                } else if (mAssignmentDone) {
                    return processOperatorNode(" == ", pDomNode, pHasError);
                } else {
                    mAssignmentDone = true;

                    return processOperatorNode(QString(" =%1 ").arg(cmetaId(pDomNode)), pDomNode, pHasError);
                }
            } else if (   mathmlNode(domNode, "neq")
                       || mathmlNode(domNode, "lt")
                       || mathmlNode(domNode, "leq")) {
                if (currentChildNodesCount != 3) {
                    mErrorMessage = tr("An '%1' element must have two siblings.").arg(domNode.localName());
                } else {
                    return processOperatorNode(mMappings.value(domNode.localName()), pDomNode, pHasError);
                }
            } else if (   mathmlNode(domNode, "gt")
                       || mathmlNode(domNode, "geq")) {
                if (currentChildNodesCount != 3) {
                    mErrorMessage = tr("A '%1' element must have two siblings.").arg(domNode.localName());
                } else {
                    return processOperatorNode(mMappings.value(domNode.localName()), pDomNode, pHasError);
                }

            // Arithmetic operators

            } else if (   mathmlNode(domNode, "plus")
                       || mathmlNode(domNode, "minus")) {
                if (currentChildNodesCount < 2) {
                    mErrorMessage = tr("A '%1' element must have at least one sibling.").arg(domNode.localName());
                } else {
                    return processOperatorNode(mMappings.value(domNode.localName()), pDomNode, pHasError);
                }
            } else if (   mathmlNode(domNode, "times")
                       || mathmlNode(domNode, "divide")) {
                if (currentChildNodesCount < 3) {
                    mErrorMessage = tr("A '%1' element must have at least two siblings.").arg(domNode.localName());
                } else {
                    return processOperatorNode(mMappings.value(domNode.localName()), pDomNode, pHasError);
                }
            } else if (mathmlNode(domNode, "power")) {
                if (currentChildNodesCount != 3) {
                    mErrorMessage = tr("A '%1' element must have two siblings.").arg(domNode.localName());
                } else {
                    return processPowerNode(pDomNode, pHasError);
                }
            } else if (mathmlNode(domNode, "root")) {
                if ((currentChildNodesCount != 2) && (currentChildNodesCount != 3)) {
                    mErrorMessage = tr("A '%1' element must have either one or two siblings.").arg(domNode.localName());
                } else {
                    return processRootNode(pDomNode, pHasError);
                }
            } else if (   mathmlNode(domNode, "abs")
                       || mathmlNode(domNode, "exp")
                       || mathmlNode(domNode, "ln")) {
                if (currentChildNodesCount != 2) {
                    mErrorMessage = tr("An '%1' element must have one sibling.").arg(domNode.localName());
                } else {
                    return processFunctionNode(domNode.localName(), pDomNode, pHasError);
                }
            } else if (mathmlNode(domNode, "log")) {
                if ((currentChildNodesCount != 2) && (currentChildNodesCount != 3)) {
                    mErrorMessage = tr("A '%1' element must have either one or two siblings.").arg(domNode.localName());
                } else {
                    return processLogNode(pDomNode, pHasError);
                }
            } else if (   mathmlNode(domNode, "ceiling")
                       || mathmlNode(domNode, "floor")
                       || mathmlNode(domNode, "factorial")) {
                if (currentChildNodesCount != 2) {
                    mErrorMessage = tr("A '%1' element must have one sibling.").arg(domNode.localName());
                } else {
                    return processFunctionNode(mMappings.value(domNode.localName()), pDomNode, pHasError);
                }

            // Logical operators

            } else if (   mathmlNode(domNode, "and")
                       || mathmlNode(domNode, "or")) {
                if (currentChildNodesCount < 3) {
                    mErrorMessage = tr("An '%1' element must have at least two siblings.").arg(domNode.localName());
                } else {
                    return processOperatorNode(mMappings.value(domNode.localName()), pDomNode, pHasError);
                }
            } else if (mathmlNode(domNode, "xor")) {
                if (currentChildNodesCount < 3) {
                    mErrorMessage = tr("A '%1' element must have at least two siblings.").arg(domNode.localName());
                } else {
                    return processOperatorNode(mMappings.value(domNode.localName()), pDomNode, pHasError);
                }
            } else if (mathmlNode(domNode, "not")) {
                if (currentChildNodesCount != 2) {
                    mErrorMessage = tr("A '%1' element must have one sibling.").arg(domNode.localName());
                } else {
                    return processNotNode(pDomNode, pHasError);
                }

            // Calculus elements

            } else if (mathmlNode(domNode, "diff")) {
                if (currentChildNodesCount != 3) {
                    mErrorMessage = tr("A '%1' element must have two siblings.").arg(domNode.localName());
                } else {
                    return processDiffNode(pDomNode, pHasError);
                }

            // Min/max operators

            } else if (mathmlNode(domNode, "min") || mathmlNode(domNode, "max")) {
                if (currentChildNodesCount < 3) {
                    mErrorMessage = tr("A '%1' element must have at least two siblings.").arg(domNode.localName());
                } else {
                    return processFunctionNode(domNode.localName(), pDomNode, pHasError);
                }

            // Gcd/lcm operators

            } else if (mathmlNode(domNode, "gcd") || mathmlNode(domNode, "lcm")) {
                if (currentChildNodesCount < 3) {
                    mErrorMessage = tr("A '%1' element must have at least two siblings.").arg(domNode.localName());
                } else {
                    return processFunctionNode(domNode.localName(), pDomNode, pHasError);
                }

            // Trigonometric operators

            } else if (   mathmlNode(domNode,  "sin") || mathmlNode(domNode,  "cos") || mathmlNode(domNode,  "tan")
                       || mathmlNode(domNode,  "sec") || mathmlNode(domNode,  "csc") || mathmlNode(domNode,  "cot")
                       || mathmlNode(domNode, "sinh") || mathmlNode(domNode, "cosh") || mathmlNode(domNode, "tanh")
                       || mathmlNode(domNode, "sech") || mathmlNode(domNode, "csch") || mathmlNode(domNode, "coth")) {
                if (currentChildNodesCount != 2) {
                    mErrorMessage = tr("A '%1' element must have one sibling.").arg(domNode.localName());
                } else {
                    return processFunctionNode(domNode.localName(), pDomNode, pHasError);
                }
            } else if (   mathmlNode(domNode,  "arcsin") || mathmlNode(domNode,  "arccos") || mathmlNode(domNode,  "arctan")
                       || mathmlNode(domNode,  "arcsec") || mathmlNode(domNode,  "arccsc") || mathmlNode(domNode,  "arccot")
                       || mathmlNode(domNode, "arcsinh") || mathmlNode(domNode, "arccosh") || mathmlNode(domNode, "arctanh")
                       || mathmlNode(domNode, "arcsech") || mathmlNode(domNode, "arccsch") || mathmlNode(domNode, "arccoth")) {
                if (currentChildNodesCount != 2) {
                    mErrorMessage = tr("An '%1' element must have one sibling.").arg(domNode.localName());
                } else {
                    return processFunctionNode(mMappings.value(domNode.localName()), pDomNode, pHasError);
                }

            // Extra operators

            } else if (mathmlNode(domNode, "rem")) {
                if (currentChildNodesCount != 3) {
                    mErrorMessage = tr("A '%1' element must have two siblings.").arg(domNode.localName());
                } else {
                    return processFunctionNode(domNode.localName(), pDomNode, pHasError);
                }

            // Unknown node

            } else if (processUnknownNode(domNode, true)) {
                return {};
            }
        }

    // Piecewise statement

    } else if (mathmlNode(domNode, "piecewise")) {
        if (mPiecewiseStatementUsed) {
            mErrorMessage = tr("A 'piecewise' element cannot be used within another 'piecewise' element.");
        } else {
            static const QString Apply = "apply";
            static const QString Eq    = "eq";

            QDomNode parentNode = domNode.parentNode();

            if (   (parentNode != mTopMathmlNode)
                || (parentNode.localName() != Apply)
                || (childNode(parentNode, 0).localName() != Eq)) {
                mErrorMessage = tr("A 'piecewise' element can only be used within a top-level 'apply' element that has an 'eq' element as its first child element.");
            } else if (currentChildNodesCount == 0) {
                mErrorMessage = tr("A '%1' element must have at least one child element.").arg(domNode.localName());
            } else {
                mPiecewiseStatementUsed = true;

                return processPiecewiseNode(pDomNode, pHasError);
            }
        }
    } else if (mathmlNode(domNode, "piece")) {
        if (currentChildNodesCount != 2) {
            mErrorMessage = tr("A '%1' element must have two child elements.").arg(domNode.localName());
        } else {
            return processPieceNode(pDomNode, pHasError);
        }
    } else if (mathmlNode(domNode, "otherwise")) {
        if (currentChildNodesCount != 1) {
            mErrorMessage = tr("An '%1' element must have one child element.").arg(domNode.localName());
        } else {
            return processOtherwiseNode(pDomNode, pHasError);
        }

    // Token elements

    } else if (mathmlNode(domNode, "cn")) {
        static const QString Real             = "real";
        static const QString Enotation        = "e-notation";
        static const QString Integer          = "integer";
        static const QString Rational         = "rational";
        static const QString ComplexPolar     = "complex-polar";
        static const QString ComplexCartesian = "complex-cartesian";
        static const QString Constant         = "constant";

        QString type = domNode.attributes().namedItem("type").nodeValue().trimmed();

        if (type.isEmpty() || (type == Real)) {
            // Either no type (i.e. real type by default) or real type

            if (currentChildNodesCount != 1) {
                mErrorMessage = tr("A '%1' element must have a value.").arg(domNode.localName());
            } else {
                return childNode(domNode, 0).nodeValue().trimmed()+"{"+cellmlAttributeNodeValue(domNode, "units")+"}";
            }
        } else if (type == Enotation) {
            // E-notation type

            if (currentChildNodesCount != 3) {
                mErrorMessage = tr("A 'cn' element with an 'e-notation' type must have three child elements.");
            } else {
                static const QString Sep = "sep";

                QDomNode childNode1 = childNode(domNode, 0);
                QDomNode childNode2 = childNode(domNode, 1);
                QDomNode childNode3 = childNode(domNode, 2);

                if (childNode1.nodeType() != QDomNode::TextNode) {
                    mErrorMessage = tr("The first child element of a 'cn' element with an 'e-notation' type must be of 'text' type.");
                } else if (childNode2.nodeType() != QDomNode::ElementNode) {
                    mErrorMessage = tr("The second child element of a 'cn' element with an 'e-notation' type must be of 'element' type.");
                } else if (childNode2.localName() != Sep) {
                    mErrorMessage = tr("The name of the second child element of a 'cn' element with an 'e-notation' type must be 'sep'.");
                } else if (childNode3.nodeType() != QDomNode::TextNode) {
                    mErrorMessage = tr("The third child element of a 'cn' element with an 'e-notation' type must be of 'text' type.");
                } else {
                    return childNode1.nodeValue().trimmed()+"e"+childNode3.nodeValue().trimmed()+"{"+cellmlAttributeNodeValue(domNode, "units")+"}";
                }
            }
        } else if (   (type == Integer)
                   || (type == Rational)
                   || (type == ComplexPolar)
                   || (type == ComplexCartesian)
                   || (type == Constant)) {
            // A known, but unsupported type

            mErrorMessage = tr("The 'cn' element uses a '%1' type that is unsupported.").arg(type);
        } else {
            // An unknown type

            mErrorMessage = tr("The 'cn' element uses a '%1' type that is unknown.").arg(type);
        }
    } else if (mathmlNode(domNode, "ci")) {
        if (currentChildNodesCount != 1) {
            mErrorMessage = tr("A '%1' element must have a value.").arg(domNode.localName());
        } else {
            return childNode(domNode, 0).nodeValue().trimmed();
        }

    // Qualifier elements

    } else if (   mathmlNode(domNode, "degree")
               || mathmlNode(domNode, "logbase")) {
        if (currentChildNodesCount != 1) {
            mErrorMessage = tr("A '%1' element must have one child element.").arg(domNode.localName());
        } else {
            return processChildNode(pDomNode, pHasError);
        }
    } else if (mathmlNode(domNode, "bvar")) {
        if ((currentChildNodesCount != 1) && (currentChildNodesCount != 2)) {
            mErrorMessage = tr("A '%1' element must have one or two child elements.").arg(domNode.localName());
        } else {
            return processBvarNode(pDomNode, pHasError);
        }

    // Constants

    } else if (   mathmlNode(domNode, "true")
               || mathmlNode(domNode, "false")
               || mathmlNode(domNode, "notanumber")
               || mathmlNode(domNode, "pi")) {
        if (currentChildNodesCount == 1) {
            mErrorMessage = tr("A '%1' element cannot have a child element.").arg(domNode.localName());
        } else if (currentChildNodesCount != 0) {
            mErrorMessage = tr("A '%1' element cannot have child elements.").arg(domNode.localName());
        } else {
            return mMappings.value(domNode.localName());
        }
    } else if (   mathmlNode(domNode, "infinity")
               || mathmlNode(domNode, "exponentiale")) {
        if (currentChildNodesCount == 1) {
            mErrorMessage = tr("An '%1' element cannot have a child element.").arg(domNode.localName());
        } else if (currentChildNodesCount != 0) {
            mErrorMessage = tr("An '%1' element cannot have child elements.").arg(domNode.localName());
        } else {
            return mMappings.value(domNode.localName());
        }

    // Semantics and annotation elements

    } else if (mathmlNode(domNode, "semantics")) {
        processUnsupportedNode(domNode, false);

        return {};
    } else if (   mathmlNode(domNode, "annotation")
               || mathmlNode(domNode, "annotation-xml")) {
        processUnsupportedNode(domNode, false, "n");

        return {};

    // Unknown node

    } else if (processUnknownNode(domNode, true)) {
        return {};
    }

    mErrorLine = domNode.lineNumber();
    mErrorColumn = domNode.columnNumber();

    pHasError = true;

    return {};
}

//==============================================================================

QString CellMLTextViewConverter::processPiecewiseNode(const QDomNode &pDomNode,
                                                      bool &pHasError)
{
    // Process the piecewise node

    QString res = "sel\n";
    QDomNodeList childNodes = pDomNode.childNodes();

    indent(false);

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        res += processMathmlNode(childNodes.item(i), pHasError);

        if (pHasError) {
            return {};
        }
    }

    unindent();

    return res+mIndent+"endsel";
}

//==============================================================================

QString CellMLTextViewConverter::processPieceNode(const QDomNode &pDomNode,
                                                  bool &pHasError)
{
    // Process the piece node

    QString res;
    QDomNodeList childNodes = pDomNode.childNodes();
    QDomNode childNode;
    int childElementNodeNumber = 0;
    QString statement;

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        childNode = childNodes.item(i);

        if (childNode.isComment()) {
            processCommentNode(childNode);
        } else {
            if (childElementNodeNumber == 0) {
                statement = processMathmlNode(childNode, pHasError);

                if (pHasError) {
                    return {};
                }
            } else {
                QString condition = processMathmlNode(childNode, pHasError);

                if (pHasError) {
                    return {};
                }

                res += mIndent+"case "+condition+":\n";

                indent(false);

                res += mIndent+statement+";\n";

                unindent();
            }

            ++childElementNodeNumber;
        }
    }

    return res;
}

//==============================================================================

QString CellMLTextViewConverter::processOtherwiseNode(const QDomNode &pDomNode,
                                                      bool &pHasError)
{
    // Process the otherwise node

    QString res;
    QDomNodeList childNodes = pDomNode.childNodes();
    QDomNode childNode;

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        childNode = childNodes.item(i);

        if (childNode.isComment()) {
            processCommentNode(childNode);
        } else {
            QString statement = processMathmlNode(childNode, pHasError);

            if (pHasError) {
                return {};
            }

            res += mIndent+"otherwise:\n";

            indent(false);

            res += mIndent+statement+";\n";

            unindent();
        }
    }

    return res;
}

//==============================================================================

QString CellMLTextViewConverter::processOperatorNode(const QString &pOperator,
                                                     const QDomNode &pDomNode,
                                                     bool &pHasError)
{
    // Process the operator node, based on its number of siblings

    QString res;
    QDomNodeList childNodes = pDomNode.childNodes();
    QDomNode childNode;
    int childElementNodeNumber = 0;
    MathmlNode operatorNodeType = MathmlNode::Unknown;

    if (childNodesCount(pDomNode) == 2) {
        for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
            childNode = childNodes.item(i);

            if (childNode.isComment()) {
                processCommentNode(childNode);
            } else {
                if (childElementNodeNumber == 0) {
                    operatorNodeType = mMathmlNodes.value(childNode.localName());
                } else {
                    QString operand = processMathmlNode(childNode, pHasError);

                    if (pHasError) {
                        return {};
                    }

                    if (isPlusOperator(operatorNodeType)) {
                        res = pOperator+operand;
                    } else if (isMinusOperator(operatorNodeType)) {
                        // Minus node

                        MathmlNode mathmlNode = CellMLTextViewConverter::mathmlNode(childNode);

                        if (   isRelationalOperator(mathmlNode)
                            || isPlusOperator(mathmlNode)
                            || isMinusOperator(mathmlNode)
                            || isLogicalOrBitwiseOperator(mathmlNode)) {
                            res = pOperator+"("+operand+")";
                        } else {
                            res = pOperator+operand;
                        }
                    } else {
                        return {};
                    }
                }

                ++childElementNodeNumber;
            }
        }
    } else {
        QDomNode leftOperandNode;
        MathmlNode leftOperandNodeType = MathmlNode::Unknown;
        QString leftOperand;

        for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
            childNode = childNodes.item(i);

            if (childNode.isComment()) {
                processCommentNode(childNode);
            } else {
                if (childElementNodeNumber == 0) {
                    operatorNodeType = mathmlNode(childNode);
                } else if (childElementNodeNumber == 1) {
                    leftOperandNode = childNode;
                    leftOperandNodeType = mathmlNode(leftOperandNode);
                    leftOperand = processMathmlNode(leftOperandNode, pHasError);

                    if (pHasError) {
                        return {};
                    }
                } else {
                    const QDomNode &rightOperandNode = childNode;
                    MathmlNode rightOperandNodeType = mathmlNode(rightOperandNode);
                    QString rightOperand = processMathmlNode(rightOperandNode, pHasError);

                    if (pHasError) {
                        return {};
                    }

                    // Determine whether parentheses should be added around the
                    // left and/or right operands, and this based on the
                    // precedence of the operators used in CellML, which are
                    // listed below from higher to lower precedence:
                    //  1. Parentheses                           [Left to right]
                    //  2. Unary PLUS, Unary MINUS, NOT          [Right to left]
                    //  3. TIMES, DIVIDE                         [Left to right]
                    //  4. PLUS, MINUS                           [Left to right]
                    //  5. LT, LEQ, GT, GEQ                      [Left to right]
                    //  6. EQEQ, NEQ                             [Left to right]
                    //  7. XOR (bitwise)                         [Left to right]
                    //  8. AND (logical)                         [Left to right]
                    //  9. OR (logical)                          [Left to right]

                    if (isPlusOperator(operatorNodeType)) {
                        if (   isRelationalOperator(leftOperandNodeType)
                            || isLogicalOrBitwiseOperator(leftOperandNodeType)) {
                            leftOperand = "("+leftOperand+")";
                        }

                        if (   isRelationalOperator(rightOperandNodeType)
                            || isLogicalOrBitwiseOperator(rightOperandNodeType)) {
                            rightOperand = "("+rightOperand+")";
                        }
                    } else if (isMinusOperator(operatorNodeType)) {
                        if (   isRelationalOperator(leftOperandNodeType)
                            || isLogicalOrBitwiseOperator(leftOperandNodeType)) {
                            leftOperand = "("+leftOperand+")";
                        }

                        if (   isRelationalOperator(rightOperandNodeType)
                            || isMinusOperator(rightOperandNodeType)
                            || isLogicalOrBitwiseOperator(rightOperandNodeType)) {
                            rightOperand = "("+rightOperand+")";
                        } else if (isPlusOperator(rightOperandNodeType)) {
                            if (childNodesCount(rightOperandNode) > 2) {
                                rightOperand = "("+rightOperand+")";
                            }
                        }
                    } else if (isTimesOperator(operatorNodeType)) {
                        if (   isRelationalOperator(leftOperandNodeType)
                            || isLogicalOrBitwiseOperator(leftOperandNodeType)) {
                            leftOperand = "("+leftOperand+")";
                        } else if (   isPlusOperator(leftOperandNodeType)
                                   || isMinusOperator(leftOperandNodeType)) {
                            if (childNodesCount(leftOperandNode) > 2) {
                                leftOperand = "("+leftOperand+")";
                            }
                        }

                        if (   isRelationalOperator(rightOperandNodeType)
                            || isLogicalOrBitwiseOperator(rightOperandNodeType)) {
                            rightOperand = "("+rightOperand+")";
                        } else if (   isPlusOperator(rightOperandNodeType)
                                   || isMinusOperator(rightOperandNodeType)) {
                            if (childNodesCount(rightOperandNode) > 2) {
                                rightOperand = "("+rightOperand+")";
                            }
                        }
                    } else if (isDivideOperator(operatorNodeType)) {
                        if (   isRelationalOperator(leftOperandNodeType)
                            || isLogicalOrBitwiseOperator(leftOperandNodeType)) {
                            leftOperand = "("+leftOperand+")";
                        } else if (   isPlusOperator(leftOperandNodeType)
                                   || isMinusOperator(leftOperandNodeType)) {
                            if (childNodesCount(leftOperandNode) > 2) {
                                leftOperand = "("+leftOperand+")";
                            }
                        }

                        if (   isRelationalOperator(rightOperandNodeType)
                            || isTimesOperator(rightOperandNodeType)
                            || isDivideOperator(rightOperandNodeType)
                            || isLogicalOrBitwiseOperator(rightOperandNodeType)) {
                            rightOperand = "("+rightOperand+")";
                        } else if (   isPlusOperator(rightOperandNodeType)
                                   || isMinusOperator(rightOperandNodeType)) {
                            if (childNodesCount(rightOperandNode) > 2) {
                                rightOperand = "("+rightOperand+")";
                            }
                        }
                    } else if (isAndOperator(operatorNodeType)) {
                        if (   isRelationalOperator(leftOperandNodeType)
                            || isOrOperator(leftOperandNodeType)
                            || isXorOperator(leftOperandNodeType)) {
                            leftOperand = "("+leftOperand+")";
                        } else if (   isPlusOperator(leftOperandNodeType)
                                   || isMinusOperator(leftOperandNodeType)) {
                            if (childNodesCount(leftOperandNode) > 2) {
                                leftOperand = "("+leftOperand+")";
                            }
                        }

                        if (   isRelationalOperator(rightOperandNodeType)
                            || isOrOperator(rightOperandNodeType)
                            || isXorOperator(rightOperandNodeType)) {
                            rightOperand = "("+rightOperand+")";
                        } else if (   isPlusOperator(rightOperandNodeType)
                                   || isMinusOperator(rightOperandNodeType)) {
                            if (childNodesCount(rightOperandNode) > 2) {
                                rightOperand = "("+rightOperand+")";
                            }
                        }
                    } else if (isOrOperator(operatorNodeType)) {
                        if (   isRelationalOperator(leftOperandNodeType)
                            || isAndOperator(leftOperandNodeType)
                            || isXorOperator(leftOperandNodeType)) {
                            leftOperand = "("+leftOperand+")";
                        } else if (   isPlusOperator(leftOperandNodeType)
                                   || isMinusOperator(leftOperandNodeType)) {
                            if (childNodesCount(leftOperandNode) > 2) {
                                leftOperand = "("+leftOperand+")";
                            }
                        }

                        if (   isRelationalOperator(rightOperandNodeType)
                            || isAndOperator(rightOperandNodeType)
                            || isXorOperator(rightOperandNodeType)) {
                            rightOperand = "("+rightOperand+")";
                        } else if (   isPlusOperator(rightOperandNodeType)
                                   || isMinusOperator(rightOperandNodeType)) {
                            if (childNodesCount(rightOperandNode) > 2) {
                                rightOperand = "("+rightOperand+")";
                            }
                        }
                    } else if (isXorOperator(operatorNodeType)) {
                        if (   isRelationalOperator(leftOperandNodeType)
                            || isAndOperator(leftOperandNodeType)
                            || isOrOperator(leftOperandNodeType)) {
                            leftOperand = "("+leftOperand+")";
                        } else if (   isPlusOperator(leftOperandNodeType)
                                   || isMinusOperator(leftOperandNodeType)) {
                            if (childNodesCount(leftOperandNode) > 2) {
                                leftOperand = "("+leftOperand+")";
                            }
                        }

                        if (   isRelationalOperator(rightOperandNodeType)
                            || isAndOperator(rightOperandNodeType)
                            || isOrOperator(rightOperandNodeType)) {
                            rightOperand = "("+rightOperand+")";
                        } else if (   isPlusOperator(rightOperandNodeType)
                                   || isMinusOperator(rightOperandNodeType)) {
                            if (childNodesCount(rightOperandNode) > 2) {
                                rightOperand = "("+rightOperand+")";
                            }
                        }
                    }

                    res = leftOperand = leftOperand+pOperator+rightOperand;

                    leftOperandNodeType = operatorNodeType;
                }

                ++childElementNodeNumber;
            }
        }
    }

    return res;
}

//==============================================================================

QString CellMLTextViewConverter::processFunctionNode(const QString &pFunction,
                                                     const QDomNode &pDomNode,
                                                     bool &pHasError)
{
    // Process the one-parameter function node

    QString res;
    QDomNodeList childNodes = pDomNode.childNodes();
    QDomNode childNode;
    int childElementNodeNumber = 0;
    QString argument;

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        childNode = childNodes.item(i);

        if (childNode.isComment()) {
            processCommentNode(childNode);
        } else {
            if (childElementNodeNumber == 0) {
                res = pFunction+"(";
            } else {
                argument = processMathmlNode(childNode, pHasError);

                if (pHasError) {
                    return {};
                }

                if (childElementNodeNumber == 1) {
                    res += argument;
                } else {
                    res += ", "+argument;
                }
            }

            ++childElementNodeNumber;
        }
    }

    if (!res.isEmpty()) {
        res += ')';
    }

    return res;
}

//==============================================================================

QString CellMLTextViewConverter::processPowerNode(const QDomNode &pDomNode,
                                                  bool &pHasError)
{
    // Process the power node

    QString res;
    QDomNodeList childNodes = pDomNode.childNodes();
    QDomNode childNode;
    int childElementNodeNumber = 0;
    QString a;

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        childNode = childNodes.item(i);

        if (childNode.isComment()) {
            processCommentNode(childNode);
        } else {
            if (childElementNodeNumber == 0) {
                // This is the function element, so nothing to process as such

                ;
            } else if (childElementNodeNumber == 1) {
                a = processMathmlNode(childNode, pHasError);

                if (pHasError) {
                    return {};
                }
            } else {
                QString b = processMathmlNode(childNode, pHasError);

                if (pHasError) {
                    return {};
                }

                // Determine the value of b, which we assume to be a number
                // (i.e. something like "3{dimensionless}")
                // Note: if b isn't a number, then n will be equal to zero,
                //       which is what we want in that case...

                static const QRegularExpression UnitRegEx = QRegularExpression("{[^}]*}$");

                double n = QString(b).replace(UnitRegEx, {}).toDouble();

                if (n == 2.0) {
                    res = "sqr("+a+")";
                } else if (n == 0.5) {
                    res = "sqrt("+a+")";
                } else {
                    res = "pow("+a+", "+b+")";
                }
            }

            ++childElementNodeNumber;
        }
    }

    return res;
}

//==============================================================================

QString CellMLTextViewConverter::processRootNode(const QDomNode &pDomNode,
                                                 bool &pHasError)
{
    // Process the root node, based on its number of arguments

    QString res;
    QDomNodeList childNodes = pDomNode.childNodes();
    int currentChildNodesCount = childNodesCount(pDomNode);
    QDomNode childNode;
    int childElementNodeNumber = 0;
    QString b;

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        childNode = childNodes.item(i);

        if (childNode.isComment()) {
            processCommentNode(childNode);
        } else {
            if (childElementNodeNumber == 0) {
                // This is the function element, so nothing to process as such

                ;
            } else if (childElementNodeNumber == 1) {
                if (currentChildNodesCount == 2) {
                    QString a = processMathmlNode(childNode, pHasError);

                    if (pHasError) {
                        return {};
                    }

                    res = "sqrt("+a+")";
                } else {
                    static const QString Degree = "degree";

                    if (childNode.localName() != Degree) {
                        mErrorMessage = tr("The first sibling of a '%1' element with two siblings must be a '%2' element.").arg("root",
                                                                                                                                "degree");
                        mErrorLine = childNode.lineNumber();
                        mErrorColumn = childNode.columnNumber();

                        pHasError = true;

                        return {};
                    }

                    b = processMathmlNode(childNode, pHasError);

                    if (pHasError) {
                        return {};
                    }
                }
            } else {
                QString a = processMathmlNode(childNode, pHasError);

                if (pHasError) {
                    return {};
                }

                // Determine the value of b, which we assume to be a number
                // (i.e. something like "3{dimensionless}")
                // Note: if b isn't a number, then n will be equal to zero,
                //       which is what we want in that case...

                static const QRegularExpression UnitRegEx = QRegularExpression("{[^}]*}$");

                double n = QString(b).replace(UnitRegEx, {}).toDouble();

                if (n == 2.0) {
                    res = "sqrt("+a+")";
                } else {
                    res = "root("+a+", "+b+")";
                }
            }

            ++childElementNodeNumber;
        }
    }

    return res;
}

//==============================================================================

QString CellMLTextViewConverter::processLogNode(const QDomNode &pDomNode,
                                                bool &pHasError)
{
    // Process the log node

    QString res;
    QDomNodeList childNodes = pDomNode.childNodes();
    int currentChildNodesCount = childNodesCount(pDomNode);
    QDomNode childNode;
    int childElementNodeNumber = 0;
    QString argumentOrBase;

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        childNode = childNodes.item(i);

        if (childNode.isComment()) {
            processCommentNode(childNode);
        } else {
            if (childElementNodeNumber == 0) {
                // This is the function element, so nothing to process as such

                ;
            } else if (childElementNodeNumber == 1) {
                argumentOrBase = processMathmlNode(childNode, pHasError);

                if (pHasError) {
                    return {};
                }

                if (currentChildNodesCount == 2) {
                    res = "log("+argumentOrBase+")";
                }
            } else {
                QString argument = processMathmlNode(childNode, pHasError);

                if (pHasError) {
                    return {};
                }

                res = "log("+argument+", "+argumentOrBase+")";
            }

            ++childElementNodeNumber;
        }
    }

    return res;
}

//==============================================================================

QString CellMLTextViewConverter::processNotNode(const QDomNode &pDomNode,
                                                bool &pHasError)
{
    // Process the not node

    QString res;
    QDomNodeList childNodes = pDomNode.childNodes();
    QDomNode childNode;
    int childElementNodeNumber = 0;

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        childNode = childNodes.item(i);

        if (childNode.isComment()) {
            processCommentNode(childNode);
        } else {
            if (childElementNodeNumber == 0) {
                // This is the function element, so nothing to process as such

                ;
            } else {
                QString operand = processMathmlNode(childNode, pHasError);

                if (pHasError) {
                    return {};
                }

                MathmlNode mathmlNode = CellMLTextViewConverter::mathmlNode(childNode);

                if (   isRelationalOperator(mathmlNode)
                    || isPlusOperator(mathmlNode) || isMinusOperator(mathmlNode) || isTimesOperator(mathmlNode) || isDivideOperator(mathmlNode)
                    || isLogicalOrBitwiseOperator(mathmlNode)) {
                    res = "not("+operand+")";
                } else {
                    res = "not "+operand;
                }
            }

            ++childElementNodeNumber;
        }
    }

    return res;
}

//==============================================================================

QString CellMLTextViewConverter::processDiffNode(const QDomNode &pDomNode,
                                                 bool &pHasError)
{
    // Process the diff node

    QString res;
    QDomNodeList childNodes = pDomNode.childNodes();
    QDomNode childNode;
    int childElementNodeNumber = 0;
    QString x;

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        childNode = childNodes.item(i);

        if (childNode.isComment()) {
            processCommentNode(childNode);
        } else {
            if (childElementNodeNumber == 0) {
                // This is the 'diff' element, so nothing to process as such

                ;
            } else if (childElementNodeNumber == 1) {
                static const QString Bvar = "bvar";

                if (childNode.localName() != Bvar) {
                    mErrorMessage = tr("The first sibling of a '%1' element with two siblings must be a '%2' element.").arg("diff",
                                                                                                                            "bvar");
                    mErrorLine = childNode.lineNumber();
                    mErrorColumn = childNode.columnNumber();

                    pHasError = true;

                    return {};
                }

                x = processMathmlNode(childNode, pHasError);

                if (pHasError) {
                    return {};
                }
            } else {
                QString f = processMathmlNode(childNode, pHasError);

                if (pHasError) {
                    return {};
                }

                res = "ode("+f+", "+x+")";
            }

            ++childElementNodeNumber;
        }
    }

    return res;
}

//==============================================================================

QString CellMLTextViewConverter::processChildNode(const QDomNode &pDomNode,
                                                  bool &pHasError)
{
    // Process the node's child

    QString res;
    QDomNodeList childNodes = pDomNode.childNodes();
    QDomNode childNode;

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        childNode = childNodes.item(i);

        if (childNode.isComment()) {
            processCommentNode(childNode);
        } else {
            res = processMathmlNode(childNode, pHasError);

            if (pHasError) {
                return {};
            }
        }
    }

    return res;
}

//==============================================================================

QString CellMLTextViewConverter::processBvarNode(const QDomNode &pDomNode,
                                                 bool &pHasError)
{
    // Process the bvar node, based on its number of child elements

    QString res;
    QDomNodeList childNodes = pDomNode.childNodes();
    int currentChildNodesCount = childNodesCount(pDomNode);
    QDomNode childNode;
    int childElementNodeNumber = 0;
    QString a;

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        childNode = childNodes.item(i);

        if (childNode.isComment()) {
            processCommentNode(childNode);
        } else {
            if (childElementNodeNumber == 0) {
                if (currentChildNodesCount == 1) {
                    res = processMathmlNode(childNode, pHasError);
                } else {
                    a = processMathmlNode(childNode, pHasError);
                }

                if (pHasError) {
                    return {};
                }
            } else {
                static const QString Degree = "degree";

                if (childNode.localName() != Degree) {
                    mErrorMessage = tr("The second child element of a '%1' element with two child elements must be a '%2' element.").arg("bvar",
                                                                                                                                         "degree");
                    mErrorLine = childNode.lineNumber();
                    mErrorColumn = childNode.columnNumber();

                    pHasError = true;

                    return {};
                }

                QString b = processMathmlNode(childNode, pHasError);

                if (pHasError) {
                    return {};
                }

                res = a+", "+b;
            }

            ++childElementNodeNumber;
        }
    }

    return res;
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

    if (   (mLastOutput == Output::Comment)
        || (mLastOutput == Output::EndDef)) {
        outputString();
    }

    outputString(Output::DefGroup,
                 QString("def group%1 as %2 for").arg(cmetaId(pDomNode),
                                                      RelationshipRef));

    indent();

    // Process the given group node's children

    QString relationshipReference;

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        if (domNode.isComment()) {
            processCommentNode(domNode);
        } else if (rdfNode(domNode)) {
            processRdfNode(domNode);
        } else if (cellmlNode(domNode, "relationship_ref")) {
            if (!processRelationshipRefNode(domNode, relationshipReference)) {
                return false;
            }
        } else if (cellmlNode(domNode, "component_ref")) {
            if (!processComponentRefNode(domNode)) {
                return false;
            }
        } else if (!processUnknownNode(domNode, false)) {
            return false;
        }
    }

    // Finish processing the given group node

    mOutput.replace(RelationshipRef, relationshipReference);

    unindent();

    outputString(Output::EndDef, "enddef;");

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
        if (domNode.isComment()) {
            processCommentNode(domNode);
        } else if (rdfNode(domNode)) {
            processRdfNode(domNode);
        } else if (!processUnknownNode(domNode, false)) {
            return false;
        }
    }

    // Process the given relationship ref node

    static const QString Encapsulation = "encapsulation";
    static const QString Containment   = "containment";

    QString relationship = cellmlAttributeNodeValue(pDomNode, "relationship");
    QString name = cellmlAttributeNodeValue(pDomNode, "name", false);
    bool isEncapsulation = false;

    if (   (pDomNode.namespaceURI() == CellMLSupport::Cellml_1_0_Namespace)
        || (pDomNode.namespaceURI() == CellMLSupport::Cellml_1_1_Namespace)) {
        if (relationship == Encapsulation) {
            isEncapsulation = true;
        } else if (relationship != Containment) {
            mErrorMessage = tr("A 'relationship' attribute in the CellML namespace must have a value of 'encapsulation' or 'containment'.");
            mErrorLine = pDomNode.lineNumber();
            mErrorColumn = pDomNode.columnNumber();

            return false;
        }
    }

    if (isEncapsulation && !name.isEmpty()) {
        mErrorMessage = tr("A 'relationship_ref' element with a 'relationship' attribute value of 'encapsulation' must not define a 'name' attribute.");
        mErrorLine = pDomNode.lineNumber();
        mErrorColumn = pDomNode.columnNumber();

        return false;
    }

    pRelationshipRef += QString("%1%2%3%4").arg(pRelationshipRef.isEmpty()?QString():" and ",
                                                relationship,
                                                cmetaId(pDomNode),
                                                name.isEmpty()?QString():" "+name);

    return true;
}

//==============================================================================

bool CellMLTextViewConverter::processComponentRefNode(const QDomNode &pDomNode)
{
    // Determine whether the given component ref node has component ref children

    static const QString ComponentRef = "component_ref";

    bool hasComponentRefChildren = false;

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling())  {
        if (domNode.localName() == ComponentRef) {
            hasComponentRefChildren = true;

            break;
        }
    }

    // Start processing the given component ref node

    if (hasComponentRefChildren) {
        if (   (mLastOutput == Output::Comment)
            || (mLastOutput == Output::Comp)
            || (mLastOutput == Output::EndComp)) {
            outputString();
        }

        outputString(Output::CompIncl,
                     QString("comp%1 %2 incl").arg(cmetaId(pDomNode),
                                                   cellmlAttributeNodeValue(pDomNode, "component")));

        indent();
    }

    // Process the given component ref node's children

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        if (domNode.isComment()) {
            processCommentNode(domNode);
        } else if (rdfNode(domNode)) {
            processRdfNode(domNode);
        } else if (cellmlNode(domNode, "component_ref")) {
            if (!processComponentRefNode(domNode)) {
                return false;
            }
        } else if (!processUnknownNode(domNode, false)) {
            return false;
        }
    }

    // Finish processing the given component ref node

    if (hasComponentRefChildren) {
        unindent();

        outputString(Output::EndComp, "endcomp;");
    } else {
        if (   (mLastOutput == Output::Comment)
            || (mLastOutput == Output::EndComp)) {
            outputString();
        }

        outputString(Output::Comp,
                     QString("comp%1 %2;").arg(cmetaId(pDomNode),
                                               cellmlAttributeNodeValue(pDomNode, "component")));
    }

    return true;
}

//==============================================================================

bool CellMLTextViewConverter::processConnectionNode(const QDomNode &pDomNode)
{
    // Start processing the given connection node

    static const QString MapComponents = "___MAP_COMPONENTS___";

    if (   (mLastOutput == Output::Comment)
        || (mLastOutput == Output::EndDef)) {
        outputString();
    }

    outputString(Output::DefMap,
                 QString("def map%1 %2 for").arg(cmetaId(pDomNode),
                                                 MapComponents));

    indent();

    // Process the given connection node's children

    QString mapComponents;

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        if (domNode.isComment()) {
            processCommentNode(domNode);
        } else if (rdfNode(domNode)) {
            processRdfNode(domNode);
        } else if (cellmlNode(domNode, "map_components")) {
            if (!processMapComponentsNode(domNode, mapComponents)) {
                return false;
            }
        } else if (cellmlNode(domNode, "map_variables")) {
            if (!processMapVariablesNode(domNode)) {
                return false;
            }
        } else if (!processUnknownNode(domNode, false)) {
            return false;
        }
    }

    // Finish processing the given group node

    mOutput.replace(MapComponents, mapComponents);

    unindent();

    outputString(Output::EndDef, "enddef;");

    return true;
}

//==============================================================================

bool CellMLTextViewConverter::processMapComponentsNode(const QDomNode &pDomNode,
                                                       QString &pMapComponents)
{
    // Make sure that we haven't already processed a map components node

    if (!pMapComponents.isEmpty()) {
        mErrorMessage = tr("A 'connection' element must contain exactly one 'map_components' element.");
        mErrorLine = pDomNode.lineNumber();
        mErrorColumn = pDomNode.columnNumber();

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
        if (domNode.isComment()) {
            processCommentNode(domNode);
        } else if (rdfNode(domNode)) {
            processRdfNode(domNode);
        } else if (!processUnknownNode(domNode, false)) {
            return false;
        }
    }

    // Process the given map components node

    pMapComponents = QString("between%1 %2 and %3").arg(cmetaId(pDomNode),
                                                        cellmlAttributeNodeValue(pDomNode, "component_1"),
                                                        cellmlAttributeNodeValue(pDomNode, "component_2"));

    return true;
}

//==============================================================================

bool CellMLTextViewConverter::processMapVariablesNode(const QDomNode &pDomNode)
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
        if (domNode.isComment()) {
            processCommentNode(domNode);
        } else if (rdfNode(domNode)) {
            processRdfNode(domNode);
        } else if (!processUnknownNode(domNode, false)) {
            return false;
        }
    }

    // Process the given unit node

    if (mLastOutput == Output::Comment) {
        outputString();
    }

    outputString(Output::Vars,
                 QString("vars%1 %2 and %3;").arg(cmetaId(pDomNode),
                                                  cellmlAttributeNodeValue(pDomNode, "variable_1"),
                                                  cellmlAttributeNodeValue(pDomNode, "variable_2")));

    return true;
}

//==============================================================================

bool CellMLTextViewConverter::processUnknownNode(const QDomNode &pDomNode,
                                                 bool pError)
{
    // The given node is unknown, so warn the user about it

    switch (pDomNode.nodeType()) {
    case QDomNode::ElementNode:
        processUnsupportedNode(pDomNode, pError);

        if (pError) {
            return false;
        }

        break;
    case QDomNode::AttributeNode:
        mWarnings << CellMLTextViewConverterWarning(tr("An attribute was found in the original CellML file, but it was not processed."),
                                                    pDomNode.lineNumber(),
                                                    pDomNode.columnNumber());

        break;
    case QDomNode::TextNode:
        mWarnings << CellMLTextViewConverterWarning(tr("Some text was found in the original CellML file, but it was not processed."),
                                                    pDomNode.lineNumber(),
                                                    pDomNode.columnNumber());

        break;
    case QDomNode::CDATASectionNode:
        mWarnings << CellMLTextViewConverterWarning(tr("A CDATA section was found in the original CellML file, but it was not processed."),
                                                    pDomNode.lineNumber(),
                                                    pDomNode.columnNumber());

        break;
    case QDomNode::EntityReferenceNode:
        mWarnings << CellMLTextViewConverterWarning(tr("An entity reference was found in the original CellML file, but it was not processed."),
                                                    pDomNode.lineNumber(),
                                                    pDomNode.columnNumber());

        break;
    case QDomNode::EntityNode:
        mWarnings << CellMLTextViewConverterWarning(tr("An entity was found in the original CellML file, but it was not processed."),
                                                    pDomNode.lineNumber(),
                                                    pDomNode.columnNumber());

        break;
    case QDomNode::ProcessingInstructionNode:
        mWarnings << CellMLTextViewConverterWarning(tr("A processing instruction was found in the original CellML file, but it is not known and cannot therefore be processed."),
                                                    pDomNode.lineNumber(),
                                                    pDomNode.columnNumber());

        break;
    case QDomNode::CommentNode:
        mWarnings << CellMLTextViewConverterWarning(tr("A comment was found in the original CellML file, but it was not processed."),
                                                    pDomNode.lineNumber(),
                                                    pDomNode.columnNumber());

        break;
    case QDomNode::DocumentNode:
        mWarnings << CellMLTextViewConverterWarning(tr("A document was found in the original CellML file, but it was not processed."),
                                                    pDomNode.lineNumber(),
                                                    pDomNode.columnNumber());

        break;
    case QDomNode::DocumentTypeNode:
        mWarnings << CellMLTextViewConverterWarning(tr("A document type was found in the original CellML file, but it was not processed."),
                                                    pDomNode.lineNumber(),
                                                    pDomNode.columnNumber());

        break;
    case QDomNode::DocumentFragmentNode:
        mWarnings << CellMLTextViewConverterWarning(tr("A document fragment was found in the original CellML file, but it was not processed."),
                                                    pDomNode.lineNumber(),
                                                    pDomNode.columnNumber());

        break;
    case QDomNode::NotationNode:
        mWarnings << CellMLTextViewConverterWarning(tr("A notation was found in the original CellML file, but it was not processed."),
                                                    pDomNode.lineNumber(),
                                                    pDomNode.columnNumber());

        break;
    case QDomNode::BaseNode:
        mWarnings << CellMLTextViewConverterWarning(tr("A base was found in the original CellML file, but it was not processed."),
                                                    pDomNode.lineNumber(),
                                                    pDomNode.columnNumber());

        break;
    case QDomNode::CharacterDataNode:
        mWarnings << CellMLTextViewConverterWarning(tr("Some character data was found in the original CellML file, but it is not known and cannot therefore be processed."),
                                                    pDomNode.lineNumber(),
                                                    pDomNode.columnNumber());

        break;
    }

    return true;
}

//==============================================================================

void CellMLTextViewConverter::processUnsupportedNode(const QDomNode &pDomNode,
                                                     bool pError,
                                                     const QString &pExtra)
{
    // The given node is known, but we don't support it, so consider it as an
    // error or a warning, depending on the case

    QString message = tr("A%1 '%2' element was found in the original CellML file, but it is not supported and cannot therefore be processed.").arg(pExtra,
                                                                                                                                                   pDomNode.prefix().isEmpty()?
                                                                                                                                                       pDomNode.localName():
                                                                                                                                                       pDomNode.prefix()+":"+pDomNode.localName());
    int lineNumber = pDomNode.lineNumber();
    int columnNumber = pDomNode.columnNumber();

    if (pError) {
        mErrorMessage = message;
        mErrorLine = lineNumber;
        mErrorColumn = columnNumber;
    } else {
        mWarnings << CellMLTextViewConverterWarning(message, lineNumber, columnNumber);
    }

    // Keep track of the give node, if it is a child of the model element and if
    // it is in the tmp-documentation namespace

    if (   (pDomNode.parentNode() == mModelNode)
        && (pDomNode.namespaceURI() == CellMLSupport::TmpDocumentationNamespace)) {
        mDocumentationNode = pDomNode.cloneNode();
    }
}

//==============================================================================

} // namespace CellMLTextView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
