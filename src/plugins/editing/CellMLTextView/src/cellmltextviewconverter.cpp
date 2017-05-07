/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Raw CellML to CellML Text converter
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
    mMappings(QMap<QString, QString>()),
    mMathmlNodeTypes(QMap<QString, MathmlNodeType>())
{
    // Reset our internals

    reset();

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

bool CellMLTextViewConverter::execute(const QString &pRawCellml)
{
    // Reset our internals

    reset();

    // Convert the raw CellML to CellML Text by first getting a DOM
    // representation of it

    QDomDocument domDocument;

    if (domDocument.setContent(pRawCellml, true,
                               &mErrorMessage, &mErrorLine, &mErrorColumn)) {
        // Process the DOM document's children, skipping the first one since
        // it corresponds to a CellML file's processing instruction

        for (QDomNode domNode = domDocument.firstChild().nextSibling();
             !domNode.isNull(); domNode = domNode.nextSibling()) {
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
    } else {
        mOutput = pRawCellml;

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

void CellMLTextViewConverter::reset()
{
    // Reset our internals

    mOutput = QString();
    mPrevIndent = QString();
    mIndent = QString();

    mLastOutputType = None;

    mErrorLine = -1;
    mErrorColumn = -1;
    mErrorMessage = QString();

    mWarnings = CellMLTextViewConverterWarnings();

    mRdfNodes = QDomDocument(QString());

    mTopMathmlNode = QDomNode();

    mAssignmentDone = false;
    mOldPiecewiseStatementUsed = false;
    mPiecewiseStatementUsed = false;
}

//==============================================================================

static const auto Indent = QStringLiteral("    ");

//==============================================================================

void CellMLTextViewConverter::indent(const bool &pForceTracking)
{
    // Indent our output and keep track of it, if required

    mIndent += Indent;

    if (pForceTracking)
        mPrevIndent = mIndent;
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
            mOutput += '\n';
    } else {
        if (pOutputType == Comment) {
            // When converting a comment that is within a piecewise equation,
            // mIndent will be wrong (since it will have been 'incremented'), so
            // we need to rely on the indent that we previously used

            mOutput += mPrevIndent+pString+"\n";
        } else {
            mOutput += mIndent+pString+"\n";

            mPrevIndent = mIndent;
        }
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

    QString cmetaIdValue = attributeNodeValue(pDomNode, CellMLSupport::CmetaIdNamespace, "id", false);

    if (!cmetaIdValue.isEmpty())
        return QString("{%1}").arg(cmetaIdValue);
    else
        return QString();
}

//==============================================================================

QString CellMLTextViewConverter::attributeNodeValue(const QDomNode &pDomNode,
                                                    const QString &pNamespace,
                                                    const QString &pName,
                                                    const bool &pMustBePresent) const
{
    // Return the trimmed value of the requested attribute using the given
    // namespace
    // Note: there is an issue with QDomNamedNodeMap::namedItemNS(). Indeed, if
    //       the attribute that defines the namespace is after the attribute
    //       itself, then the attribute will only be accessible without using a
    //       namespace (see https://bugreports.qt.io/browse/QTBUG-59932)...

    QDomNamedNodeMap domNodeAttributes = pDomNode.attributes();
    QDomNode attributeNode = domNodeAttributes.namedItemNS(pNamespace, pName);

    if (attributeNode.isNull())
        attributeNode = domNodeAttributes.namedItem(pName);

    QString res = attributeNode.nodeValue().trimmed();

    if (res.isEmpty())
        return pMustBePresent?"???":res;
    else
        return res;
}

//==============================================================================

QString CellMLTextViewConverter::cellmlAttributeNodeValue(const QDomNode &pDomNode,
                                                          const QString &pName,
                                                          const bool &pMustBePresent) const
{
    // Return the requested CellML attribute using the CellML 1.0 namespace or,
    // if needed, the CellML 1.1 namespace

    QString res = attributeNodeValue(pDomNode, CellMLSupport::Cellml_1_0_Namespace, pName, false);

    if (res.isEmpty())
        res = attributeNodeValue(pDomNode, CellMLSupport::Cellml_1_1_Namespace, pName, false);

    if (res.isEmpty())
        return pMustBePresent?"???":res;
    else
        return res;
}

//==============================================================================

CellMLTextViewConverter::MathmlNodeType CellMLTextViewConverter::mathmlNodeType(const QDomNode &pDomNode) const
{
    // Return the MathML type of the given node

    if (   !pDomNode.localName().compare("apply")
        && !pDomNode.namespaceURI().compare(CellMLSupport::MathmlNamespace)) {
        return mMathmlNodeTypes.value(childNode(pDomNode, 0).localName());
    } else {
        return mMathmlNodeTypes.value(pDomNode.localName());
    }
}

//==============================================================================

bool CellMLTextViewConverter::processModelNode(const QDomNode &pDomNode)
{
    // Start processing the given model node

    if (mLastOutputType == Comment)
        outputString();

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
        } else if (!processUnknownNode(domNode, false)) {
            return false;
        }
    }

    // Finish processing the given model node

    unindent();

    outputString(EndDef, "enddef;");

    return true;
}

//==============================================================================

QString CellMLTextViewConverter::processCommentString(const QString &pComment)
{
    // An empty comment will have been serialised as "<!-- -->", which means
    // that pComment will be equal to " ", in which case we need to return an
    // empty string

    if (!pComment.compare(" "))
        return QString();

    // We cannot have "--" in an XML comment, so those were replaced with
    // "&#45;&#45;", which means that we need to convert them back

    QString realComment = pComment;

    realComment.replace("&#45;&#45;", "--");

    // If newComment ends with "- ", it's because the comment ends with a dash,
    // but that a space had to be added to it to avoid problems with the end of
    // an XML comment ("-->"), so we need to chop that space to get the real
    // comment

    if (realComment.endsWith("- "))
        realComment.chop(1);

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

    if (   (mLastOutputType == Comment)
        || (mLastOutputType == Comp) || (mLastOutputType == DefBaseUnit)
        || (mLastOutputType == EndComp) || (mLastOutputType == EndDef)
        || (mLastOutputType == Equation) || (mLastOutputType == ImportComp)
        || (mLastOutputType == ImportUnit) || (mLastOutputType == Unit)
        || (mLastOutputType == Var) || (mLastOutputType == Vars)) {
        outputString();
    }

    foreach (const QString commentLine, commentLines)
        outputString(Comment, QString("//%1").arg(processCommentString(commentLine)));
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

    if (   (mLastOutputType == Comment)
        || (mLastOutputType == EndDef)) {
        outputString();
    }

    outputString(DefImport,
                 QString("def import%1 using \"%2\" for").arg(cmetaId(pDomNode))
                                                         .arg(attributeNodeValue(pDomNode, CellMLSupport::XlinkNamespace, "href")));

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
        } else if (!processUnknownNode(domNode, false)) {
            return false;
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

    if (!baseUnits.isEmpty() && baseUnits.compare("yes") && baseUnits.compare("no")) {
        mErrorLine = pDomNode.lineNumber();
        mErrorMessage = QObject::tr("A 'base_units' attribute must have a value of 'yes' or 'no'.");

        return false;
    }

    bool isBaseUnits = !baseUnits.compare("yes");

    if (!pInImportNode && !isBaseUnits) {
        if (   (mLastOutputType == Comment)
            || (mLastOutputType == DefBaseUnit) || (mLastOutputType == EndDef)
            || (mLastOutputType == Equation) || (mLastOutputType == Var)) {
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
        if (domNode.isComment()) {
            processCommentNode(domNode);
        } else if (rdfNode(domNode)) {
            processRdfNode(domNode);
        } else if (!isBaseUnits && !pInImportNode && cellmlNode(domNode, "unit")) {
            if (!processUnitNode(domNode))
                return false;
        } else if (!processUnknownNode(domNode, false)) {
            return false;
        }
    }

    // Finish processing the given units node

    if (pInImportNode) {
        if (   (mLastOutputType == Comment)
            || (mLastOutputType == ImportComp)) {
            outputString();
        }

        outputString(ImportUnit,
                     QString("unit%1 %2 using unit %3;").arg(cmetaId(pDomNode))
                                                        .arg(cellmlAttributeNodeValue(pDomNode, "name"))
                                                        .arg(cellmlAttributeNodeValue(pDomNode, "units_ref")));
    } else if (isBaseUnits) {
        if (   (mLastOutputType == Comment)
            || (mLastOutputType == EndDef) || (mLastOutputType == Equation)
            || (mLastOutputType == Var)) {
            outputString();
        }

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
        if (domNode.isComment())
            processCommentNode(domNode);
        else if (rdfNode(domNode))
            processRdfNode(domNode);
        else if (!processUnknownNode(domNode, false))
            return false;
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

    return true;
}

//==============================================================================

bool CellMLTextViewConverter::processComponentNode(const QDomNode &pDomNode,
                                                   const bool &pInImportNode)
{
    // Start processing the given component node

    if (!pInImportNode) {
        if (   (mLastOutputType == Comment)
            || (mLastOutputType == EndDef)) {
            outputString();
        }

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
            if (!processVariableNode(domNode))
                return false;
        } else if (!pInImportNode && mathmlNode(domNode, "math")) {
            if (!processMathNode(domNode))
                return false;
        } else if (!pInImportNode && cellmlNode(domNode, "reaction")) {
            if (!processReactionNode(domNode))
                return false;
        } else if (!processUnknownNode(domNode, false)) {
            return false;
        }
    }

    // Finish processing the given component node

    if (pInImportNode) {
        if (   (mLastOutputType == Comment)
            || (mLastOutputType == ImportUnit)) {
            outputString();
        }

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
        if (domNode.isComment())
            processCommentNode(domNode);
        else if (rdfNode(domNode))
            processRdfNode(domNode);
        else if (!processUnknownNode(domNode, false))
            return false;
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
        || (mLastOutputType == DefBaseUnit) || (mLastOutputType == EndDef)
        || (mLastOutputType == Equation)) {
        outputString();
    }

    outputString(Var,
                 QString("var%1 %2: %3%4;").arg(cmetaId(pDomNode))
                                           .arg(cellmlAttributeNodeValue(pDomNode, "name"))
                                           .arg(cellmlAttributeNodeValue(pDomNode, "units"))
                                           .arg(parameters.isEmpty()?QString():" {"+parameters+"}"));

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
        if (!childNodes.item(i).isComment())
            ++res;
    }

    return res;
}

//==============================================================================

QDomNode CellMLTextViewConverter::childNode(const QDomNode &pDomNode,
                                            const int &pChildNodeIndex) const
{
    // Return the nth child element of the given node

    int childNodeIndex = 0;
    QDomNodeList childNodes = pDomNode.childNodes();

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        if (!childNodes.item(i).isComment()) {
            if (childNodeIndex == pChildNodeIndex)
                return childNodes.item(i);
            else
                ++childNodeIndex;
        }
    }

    return QDomNode();
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
    int currentChildNodesCount = childNodesCount(pDomNode);

    // Basic content elements

    if (domNode.isComment()) {
        processCommentNode(domNode);

        return QString();
    } else if (mathmlNode(domNode, "apply")) {
        // Make sure that we have at least one child

        if (!currentChildNodesCount) {
            mErrorMessage = QObject::tr("An '%1' element must have at least one child element.").arg(domNode.localName());
        } else {
            domNode = childNode(domNode, 0);

            // Relational operators

            if (mathmlNode(domNode, "eq")) {
                if (currentChildNodesCount != 3) {
                    mErrorMessage = QObject::tr("An '%1' element must have two siblings.").arg(domNode.localName());
                } else if (mAssignmentDone) {
                    return processOperatorNode(" == ", pDomNode, pHasError);
                } else {
                    mAssignmentDone = true;

                    return processOperatorNode(QString(" =%1 ").arg(cmetaId(pDomNode)), pDomNode, pHasError);
                }
            } else if (   mathmlNode(domNode, "neq")
                       || mathmlNode(domNode, "lt")
                       || mathmlNode(domNode, "leq")) {
                if (currentChildNodesCount != 3)
                    mErrorMessage = QObject::tr("An '%1' element must have two siblings.").arg(domNode.localName());
                else
                    return processOperatorNode(mMappings.value(domNode.localName()), pDomNode, pHasError);
            } else if (   mathmlNode(domNode, "gt")
                       || mathmlNode(domNode, "geq")) {
                if (currentChildNodesCount != 3)
                    mErrorMessage = QObject::tr("A '%1' element must have two siblings.").arg(domNode.localName());
                else
                    return processOperatorNode(mMappings.value(domNode.localName()), pDomNode, pHasError);

            // Arythmetic operators

            } else if (   mathmlNode(domNode, "plus")
                       || mathmlNode(domNode, "minus")) {
                if (currentChildNodesCount < 2)
                    mErrorMessage = QObject::tr("A '%1' element must have at least one sibling.").arg(domNode.localName());
                else
                    return processOperatorNode(mMappings.value(domNode.localName()), pDomNode, pHasError);
            } else if (   mathmlNode(domNode, "times")
                       || mathmlNode(domNode, "divide")) {
                if (currentChildNodesCount < 3)
                    mErrorMessage = QObject::tr("A '%1' element must have at least two siblings.").arg(domNode.localName());
                else
                    return processOperatorNode(mMappings.value(domNode.localName()), pDomNode, pHasError);
            } else if (mathmlNode(domNode, "power")) {
                if (currentChildNodesCount != 3)
                    mErrorMessage = QObject::tr("A '%1' element must have two siblings.").arg(domNode.localName());
                else
                    return processPowerNode(pDomNode, pHasError);
            } else if (mathmlNode(domNode, "root")) {
                if ((currentChildNodesCount != 2) && (currentChildNodesCount != 3))
                    mErrorMessage = QObject::tr("A '%1' element must have either one or two siblings.").arg(domNode.localName());
                else
                    return processRootNode(pDomNode, pHasError);
            } else if (   mathmlNode(domNode, "abs")
                       || mathmlNode(domNode, "exp")
                       || mathmlNode(domNode, "ln")) {
                if (currentChildNodesCount != 2)
                    mErrorMessage = QObject::tr("An '%1' element must have one sibling.").arg(domNode.localName());
                else
                    return processFunctionNode(domNode.localName(), pDomNode, pHasError);
            } else if (mathmlNode(domNode, "log")) {
                if ((currentChildNodesCount != 2) && (currentChildNodesCount != 3))
                    mErrorMessage = QObject::tr("A '%1' element must have either one or two siblings.").arg(domNode.localName());
                else
                    return processLogNode(pDomNode, pHasError);
            } else if (   mathmlNode(domNode, "ceiling")
                       || mathmlNode(domNode, "floor")
                       || mathmlNode(domNode, "factorial")) {
                if (currentChildNodesCount != 2)
                    mErrorMessage = QObject::tr("A '%1' element must have one sibling.").arg(domNode.localName());
                else
                    return processFunctionNode(mMappings.value(domNode.localName()), pDomNode, pHasError);

            // Logical operators

            } else if (   mathmlNode(domNode, "and")
                       || mathmlNode(domNode, "or")) {
                if (currentChildNodesCount < 3)
                    mErrorMessage = QObject::tr("An '%1' element must have at least two siblings.").arg(domNode.localName());
                else
                    return processOperatorNode(mMappings.value(domNode.localName()), pDomNode, pHasError);
            } else if (mathmlNode(domNode, "xor")) {
                if (currentChildNodesCount < 3)
                    mErrorMessage = QObject::tr("A '%1' element must have at least two siblings.").arg(domNode.localName());
                else
                    return processOperatorNode(mMappings.value(domNode.localName()), pDomNode, pHasError);
            } else if (mathmlNode(domNode, "not")) {
                if (currentChildNodesCount != 2)
                    mErrorMessage = QObject::tr("A '%1' element must have one sibling.").arg(domNode.localName());
                else
                    return processNotNode(pDomNode, pHasError);

            // Calculus elements

            } else if (mathmlNode(domNode, "diff")) {
                if (currentChildNodesCount != 3)
                    mErrorMessage = QObject::tr("A '%1' element must have two siblings.").arg(domNode.localName());
                else
                    return processDiffNode(pDomNode, pHasError);

            // Min/max operators

            } else if (mathmlNode(domNode, "min") || mathmlNode(domNode, "max")) {
                if (currentChildNodesCount < 3)
                    mErrorMessage = QObject::tr("A '%1' element must have at least two siblings.").arg(domNode.localName());
                else
                    return processFunctionNode(domNode.localName(), pDomNode, pHasError);

            // Gcd/lcm operators

            } else if (mathmlNode(domNode, "gcd") || mathmlNode(domNode, "lcm")) {
                if (currentChildNodesCount < 3)
                    mErrorMessage = QObject::tr("A '%1' element must have at least two siblings.").arg(domNode.localName());
                else
                    return processFunctionNode(domNode.localName(), pDomNode, pHasError);

            // Trigonometric operators

            } else if (   mathmlNode(domNode,  "sin") || mathmlNode(domNode,  "cos") || mathmlNode(domNode,  "tan")
                       || mathmlNode(domNode,  "sec") || mathmlNode(domNode,  "csc") || mathmlNode(domNode,  "cot")
                       || mathmlNode(domNode, "sinh") || mathmlNode(domNode, "cosh") || mathmlNode(domNode, "tanh")
                       || mathmlNode(domNode, "sech") || mathmlNode(domNode, "csch") || mathmlNode(domNode, "coth")) {
                if (currentChildNodesCount != 2)
                    mErrorMessage = QObject::tr("A '%1' element must have one sibling.").arg(domNode.localName());
                else
                    return processFunctionNode(domNode.localName(), pDomNode, pHasError);
            } else if (   mathmlNode(domNode,  "arcsin") || mathmlNode(domNode,  "arccos") || mathmlNode(domNode,  "arctan")
                       || mathmlNode(domNode,  "arcsec") || mathmlNode(domNode,  "arccsc") || mathmlNode(domNode,  "arccot")
                       || mathmlNode(domNode, "arcsinh") || mathmlNode(domNode, "arccosh") || mathmlNode(domNode, "arctanh")
                       || mathmlNode(domNode, "arcsech") || mathmlNode(domNode, "arccsch") || mathmlNode(domNode, "arccoth")) {
                if (currentChildNodesCount != 2)
                    mErrorMessage = QObject::tr("An '%1' element must have one sibling.").arg(domNode.localName());
                else
                    return processFunctionNode(mMappings.value(domNode.localName()), pDomNode, pHasError);

            // Extra operators

            } else if (mathmlNode(domNode, "rem")) {
                if (currentChildNodesCount != 3)
                    mErrorMessage = QObject::tr("A '%1' element must have two siblings.").arg(domNode.localName());
                else
                    return processFunctionNode(domNode.localName(), pDomNode, pHasError);

            // Unknown node

            } else if (processUnknownNode(domNode, true)) {
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
                || childNode(parentNode, 0).localName().compare("eq")) {
                mErrorMessage = QObject::tr("A 'piecewise' element can only be used within a top-level 'apply' element that has an 'eq' element as its first child element.");
            } else if (!currentChildNodesCount) {
                mErrorMessage = QObject::tr("A '%1' element must have at least one child element.").arg(domNode.localName());
            } else {
                mPiecewiseStatementUsed = true;

                return processPiecewiseNode(pDomNode, pHasError);
            }
        }
    } else if (mathmlNode(domNode, "piece")) {
        if (currentChildNodesCount != 2)
            mErrorMessage = QObject::tr("A '%1' element must have two child elements.").arg(domNode.localName());
        else
            return processPieceNode(pDomNode, pHasError);
    } else if (mathmlNode(domNode, "otherwise")) {
        if (currentChildNodesCount != 1)
            mErrorMessage = QObject::tr("An '%1' element must have one child element.").arg(domNode.localName());
        else
            return processOtherwiseNode(pDomNode, pHasError);

    // Token elements

    } else if (mathmlNode(domNode, "cn")) {
        QString type = domNode.attributes().namedItem("type").nodeValue().trimmed();

        if (type.isEmpty() || !type.compare("real")) {
            // Either no type (i.e. real type by default) or real type

            if (currentChildNodesCount != 1)
                mErrorMessage = QObject::tr("A '%1' element must have a value.").arg(domNode.localName());
            else
                return childNode(domNode, 0).nodeValue().trimmed()+"{"+cellmlAttributeNodeValue(domNode, "units")+"}";
        } else if (!type.compare("e-notation")) {
            // E-notation type

            if (currentChildNodesCount != 3) {
                mErrorMessage = QObject::tr("A 'cn' element with an 'e-notation' type must have three child elements.");
            } else {
                QDomNode childNode1 = childNode(domNode, 0);
                QDomNode childNode2 = childNode(domNode, 1);
                QDomNode childNode3 = childNode(domNode, 2);

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
        if (currentChildNodesCount != 1)
            mErrorMessage = QObject::tr("A '%1' element must have a value.").arg(domNode.localName());
        else
            return childNode(domNode, 0).nodeValue().trimmed();

    // Qualifier elements

    } else if (   mathmlNode(domNode, "degree")
               || mathmlNode(domNode, "logbase")) {
        if (currentChildNodesCount != 1)
            mErrorMessage = QObject::tr("A '%1' element must have one child element.").arg(domNode.localName());
        else
            return processChildNode(pDomNode, pHasError);
    } else if (mathmlNode(domNode, "bvar")) {
        if ((currentChildNodesCount != 1) && (currentChildNodesCount != 2))
            mErrorMessage = QObject::tr("A '%1' element must have one or two child elements.").arg(domNode.localName());
        else
            return processBvarNode(pDomNode, pHasError);

    // Constants

    } else if (   mathmlNode(domNode, "true")
               || mathmlNode(domNode, "false")
               || mathmlNode(domNode, "notanumber")
               || mathmlNode(domNode, "pi")) {
        if (currentChildNodesCount == 1)
            mErrorMessage = QObject::tr("A '%1' element cannot have a child element.").arg(domNode.localName());
        else if (currentChildNodesCount)
            mErrorMessage = QObject::tr("A '%1' element cannot have child elements.").arg(domNode.localName());
        else
            return mMappings.value(domNode.localName());
    } else if (   mathmlNode(domNode, "infinity")
               || mathmlNode(domNode, "exponentiale")) {
        if (currentChildNodesCount == 1)
            mErrorMessage = QObject::tr("An '%1' element cannot have a child element.").arg(domNode.localName());
        else if (currentChildNodesCount)
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

    } else if (processUnknownNode(domNode, true)) {
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
    QDomNodeList childNodes = pDomNode.childNodes();

    indent(false);

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        res += processMathmlNode(childNodes.item(i), pHasError);

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

    QString res = QString();
    QDomNodeList childNodes = pDomNode.childNodes();
    QDomNode childNode = QDomNode();
    int childElementNodeNumber = 0;
    QString statement = QString();

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        childNode = childNodes.item(i);

        if (childNode.isComment()) {
            processCommentNode(childNode);
        } else {
            if (!childElementNodeNumber) {
                statement = processMathmlNode(childNode, pHasError);

                if (pHasError)
                    return QString();
            } else {
                QString condition = processMathmlNode(childNode, pHasError);

                if (pHasError) {
                    return QString();
                } else {
                    res += mIndent+"case "+condition+":\n";

                    indent(false);

                    res += mIndent+statement+";\n";

                    unindent();
                }
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

    QString res = QString();
    QDomNodeList childNodes = pDomNode.childNodes();
    QDomNode childNode = QDomNode();

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        childNode = childNodes.item(i);

        if (childNode.isComment()) {
            processCommentNode(childNode);
        } else {
            QString statement = processMathmlNode(childNode, pHasError);

            if (pHasError) {
                return QString();
            } else {
                res += mIndent+"otherwise:\n";

                indent(false);

                res += mIndent+statement+";\n";

                unindent();
            }
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

    QString res = QString();
    QDomNodeList childNodes = pDomNode.childNodes();
    QDomNode childNode = QDomNode();
    int childElementNodeNumber = 0;
    MathmlNodeType operatorNodeType = UnknownMathmlNode;

    if (childNodesCount(pDomNode) == 2) {

        for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
            childNode = childNodes.item(i);

            if (childNode.isComment()) {
                processCommentNode(childNode);
            } else {
                if (!childElementNodeNumber) {
                    operatorNodeType = mMathmlNodeTypes.value(childNode.localName());
                } else {
                    QString operand = processMathmlNode(childNode, pHasError);

                    if (pHasError) {
                        return QString();
                    } else {
                        if (operatorNodeType == PlusMathmlNode) {
                            res = pOperator+operand;
                        } else if (operatorNodeType == MinusMathmlNode) {
                            // Minus node

                            switch (mathmlNodeType(childNode)) {
                            case EqMathmlNode: case NeqMathmlNode: case GtMathmlNode: case LtMathmlNode: case GeqMathmlNode: case LeqMathmlNode:
                            case PlusMathmlNode: case MinusMathmlNode:
                            case AndMathmlNode: case OrMathmlNode: case XorMathmlNode:
                                res = pOperator+"("+operand+")";

                                break;
                            default:
                                res = pOperator+operand;
                            }
                        } else {
                            return QString();
                        }
                    }
                }

                ++childElementNodeNumber;
            }
        }
    } else {
        QDomNode leftOperandNode = QDomNode();
        MathmlNodeType leftOperandNodeType = UnknownMathmlNode;
        QString leftOperand = QString();

        for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
            childNode = childNodes.item(i);

            if (childNode.isComment()) {
                processCommentNode(childNode);
            } else {
                if (!childElementNodeNumber) {
                    operatorNodeType = mathmlNodeType(childNode);
                } else if (childElementNodeNumber == 1) {
                    leftOperandNode = childNode;
                    leftOperandNodeType = mathmlNodeType(leftOperandNode);
                    leftOperand = processMathmlNode(leftOperandNode, pHasError);

                    if (pHasError)
                        return QString();
                } else {
                    QDomNode rightOperandNode = childNode;
                    MathmlNodeType rightOperandNodeType = mathmlNodeType(rightOperandNode);
                    QString rightOperand = processMathmlNode(rightOperandNode, pHasError);

                    if (pHasError) {
                        return QString();
                    } else {
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
                                if (childNodesCount(rightOperandNode) > 2)
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
                                if (childNodesCount(leftOperandNode) > 2)
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
                                if (childNodesCount(rightOperandNode) > 2)
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
                                if (childNodesCount(leftOperandNode) > 2)
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
                                if (childNodesCount(rightOperandNode) > 2)
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
                                if (childNodesCount(leftOperandNode) > 2)
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
                                if (childNodesCount(rightOperandNode) > 2)
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
                                if (childNodesCount(leftOperandNode) > 2)
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
                                if (childNodesCount(rightOperandNode) > 2)
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
                                if (childNodesCount(leftOperandNode) > 2)
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
                                if (childNodesCount(rightOperandNode) > 2)
                                    rightOperand = "("+rightOperand+")";

                                break;
                            default:
                                ;
                            }

                            break;
                        default:
                            ;
                        }

                        res = leftOperand = leftOperand+pOperator+rightOperand;

                        leftOperandNodeType = operatorNodeType;
                    }
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

    QString res = QString();
    QDomNodeList childNodes = pDomNode.childNodes();
    QDomNode childNode = QDomNode();
    int childElementNodeNumber = 0;
    QString argument = QString();

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        childNode = childNodes.item(i);

        if (childNode.isComment()) {
            processCommentNode(childNode);
        } else {
            if (!childElementNodeNumber) {
                res = pFunction+"(";
            } else {
                argument = processMathmlNode(childNode, pHasError);

                if (pHasError)
                    return QString();
                else if (childElementNodeNumber == 1)
                    res += argument;
                else
                    res += ", "+argument;
            }

            ++childElementNodeNumber;
        }
    }

    if (!res.isEmpty())
        res += ')';

    return res;
}

//==============================================================================

QString CellMLTextViewConverter::processPowerNode(const QDomNode &pDomNode,
                                                  bool &pHasError)
{
    // Process the power node

    QString res = QString();
    QDomNodeList childNodes = pDomNode.childNodes();
    QDomNode childNode = QDomNode();
    int childElementNodeNumber = 0;
    QString a = QString();

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        childNode = childNodes.item(i);

        if (childNode.isComment()) {
            processCommentNode(childNode);
        } else {
            if (!childElementNodeNumber) {
                // This is the function element, so nothing to process as such

                ;
            } else if (childElementNodeNumber == 1) {
                a = processMathmlNode(childNode, pHasError);

                if (pHasError)
                    return QString();
            } else {
                QString b = processMathmlNode(childNode, pHasError);

                if (pHasError) {
                    return QString();
                } else {
                    // Determine the value of b, which we assume to be a number
                    // (i.e. something like "3{dimensionless}")
                    // Note: if b isn't a number, then n will be equal to zero,
                    //       which is what we want in that case...

                    static const QRegularExpression UnitRegEx = QRegularExpression("{[^}]*}$");

                    double n = QString(b).replace(UnitRegEx, QString()).toDouble();

                    if (n == 2.0)
                        res = "sqr("+a+")";
                    else if (n == 0.5)
                        res = "sqrt("+a+")";
                    else
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

    QString res = QString();
    QDomNodeList childNodes = pDomNode.childNodes();
    int currentChildNodesCount = childNodesCount(pDomNode);
    QDomNode childNode = QDomNode();
    int childElementNodeNumber = 0;
    QString b = QString();

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        childNode = childNodes.item(i);

        if (childNode.isComment()) {
            processCommentNode(childNode);
        } else {
            if (!childElementNodeNumber) {
                // This is the function element, so nothing to process as such

                ;
            } else if (childElementNodeNumber == 1) {
                if (currentChildNodesCount == 2) {
                    QString a = processMathmlNode(childNode, pHasError);

                    if (pHasError)
                        return QString();
                    else
                        res = "sqrt("+a+")";
                } else {
                    if (childNode.localName().compare("degree")) {
                        mErrorMessage = QObject::tr("The first sibling of a '%1' element with two siblings must be a '%2' element.").arg("root")
                                                                                                                                    .arg("degree");
                        mErrorLine = childNode.lineNumber();

                        pHasError = true;

                        return QString();
                    } else {
                        b = processMathmlNode(childNode, pHasError);

                        if (pHasError)
                            return QString();
                    }
                }
            } else {
                QString a = processMathmlNode(childNode, pHasError);

                if (pHasError) {
                    return QString();
                } else {
                    // Determine the value of b, which we assume to be a number
                    // (i.e. something like "3{dimensionless}")
                    // Note: if b isn't a number, then n will be equal to zero,
                    //       which is what we want in that case...

                    static const QRegularExpression UnitRegEx = QRegularExpression("{[^}]*}$");

                    double n = QString(b).replace(UnitRegEx, QString()).toDouble();

                    if (n == 2.0)
                        res = "sqrt("+a+")";
                    else
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

    QString res = QString();
    QDomNodeList childNodes = pDomNode.childNodes();
    int currentChildNodesCount = childNodesCount(pDomNode);
    QDomNode childNode = QDomNode();
    int childElementNodeNumber = 0;
    QString argumentOrBase = QString();

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        childNode = childNodes.item(i);

        if (childNode.isComment()) {
            processCommentNode(childNode);
        } else {
            if (!childElementNodeNumber) {
                // This is the function element, so nothing to process as such

                ;
            } else if (childElementNodeNumber == 1) {
                argumentOrBase = processMathmlNode(childNode, pHasError);

                if (pHasError)
                    return QString();
                else if (currentChildNodesCount == 2)
                    res = "log("+argumentOrBase+")";
            } else {
                QString argument = processMathmlNode(childNode, pHasError);

                if (pHasError)
                    return QString();
                else
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

    QString res = QString();
    QDomNodeList childNodes = pDomNode.childNodes();
    QDomNode childNode = QDomNode();
    int childElementNodeNumber = 0;

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        childNode = childNodes.item(i);

        if (childNode.isComment()) {
            processCommentNode(childNode);
        } else {
            if (!childElementNodeNumber) {
                // This is the function element, so nothing to process as such

                ;
            } else {
                QString operand = processMathmlNode(childNode, pHasError);

                if (pHasError) {
                    return QString();
                } else {
                    switch (mathmlNodeType(childNode)) {
                    case EqMathmlNode: case NeqMathmlNode: case GtMathmlNode: case LtMathmlNode: case GeqMathmlNode: case LeqMathmlNode:
                    case PlusMathmlNode: case MinusMathmlNode: case TimesMathmlNode: case DivideMathmlNode:
                    case AndMathmlNode: case OrMathmlNode: case XorMathmlNode:
                        res = "not("+operand+")";

                        break;
                    default:
                        res = "not "+operand;
                    }
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

    QString res = QString();
    QDomNodeList childNodes = pDomNode.childNodes();
    QDomNode childNode = QDomNode();
    int childElementNodeNumber = 0;
    QString x = QString();

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        childNode = childNodes.item(i);

        if (childNode.isComment()) {
            processCommentNode(childNode);
        } else {
            if (!childElementNodeNumber) {
                // This is the 'diff' element, so nothing to process as such

                ;
            } else if (childElementNodeNumber == 1) {
                if (childNode.localName().compare("bvar")) {
                    mErrorMessage = QObject::tr("The first sibling of a '%1' element with two siblings must be a '%2' element.").arg("diff")
                                                                                                                                .arg("bvar");
                    mErrorLine = childNode.lineNumber();

                    pHasError = true;

                    return QString();
                } else {
                    x = processMathmlNode(childNode, pHasError);

                    if (pHasError)
                        return QString();
                }
            } else {
                QString f = processMathmlNode(childNode, pHasError);

                if (pHasError)
                    return QString();
                else
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

    QString res = QString();
    QDomNodeList childNodes = pDomNode.childNodes();
    QDomNode childNode = QDomNode();

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        childNode = childNodes.item(i);

        if (childNode.isComment()) {
            processCommentNode(childNode);
        } else {
            res = processMathmlNode(childNode, pHasError);

            if (pHasError)
                return QString();
        }
    }

    return res;
}

//==============================================================================

QString CellMLTextViewConverter::processBvarNode(const QDomNode &pDomNode,
                                                 bool &pHasError)
{
    // Process the bvar node, based on its number of child elements

    QString res = QString();
    QDomNodeList childNodes = pDomNode.childNodes();
    int currentChildNodesCount = childNodesCount(pDomNode);
    QDomNode childNode = QDomNode();
    int childElementNodeNumber = 0;
    QString a = QString();

    for (int i = 0, iMax = childNodes.count(); i < iMax; ++i) {
        childNode = childNodes.item(i);

        if (childNode.isComment()) {
            processCommentNode(childNode);
        } else {
            if (!childElementNodeNumber) {
                if (currentChildNodesCount == 1)
                    res = processMathmlNode(childNode, pHasError);
                else
                    a = processMathmlNode(childNode, pHasError);

                if (pHasError)
                    return QString();
            } else {
                if (childNode.localName().compare("degree")) {
                    mErrorMessage = QObject::tr("The second child element of a '%1' element with two child elements must be a '%2' element.").arg("bvar")
                                                                                                                                             .arg("degree");
                    mErrorLine = childNode.lineNumber();

                    pHasError = true;

                    return QString();
                } else {
                    QString b = processMathmlNode(childNode, pHasError);

                    if (pHasError)
                        return QString();
                    else
                        res = a+", "+b;
                }
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

    if (   (mLastOutputType == Comment)
        || (mLastOutputType == EndDef)) {
        outputString();
    }

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
            if (!processComponentRefNode(domNode))
                return false;
        } else if (!processUnknownNode(domNode, false)) {
            return false;
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
        else if (!processUnknownNode(domNode, false))
            return false;
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

    pRelationshipRef += QString("%1%2%3%4").arg(pRelationshipRef.isEmpty()?QString():" and ")
                                           .arg(relationship)
                                           .arg(cmetaId(pDomNode))
                                           .arg(name.isEmpty()?QString():" "+name);

    return true;
}

//==============================================================================

bool CellMLTextViewConverter::processComponentRefNode(const QDomNode &pDomNode)
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
        if (domNode.isComment()) {
            processCommentNode(domNode);
        } else if (rdfNode(domNode)) {
            processRdfNode(domNode);
        } else if (cellmlNode(domNode, "component_ref")) {
            if (!processComponentRefNode(domNode))
                return false;
        } else if (!processUnknownNode(domNode, false)) {
            return false;
        }
    }

    // Finish processing the given component ref node

    if (hasComponentRefChildren) {
        unindent();

        outputString(EndComp, "endcomp;");
    } else {
        if (   (mLastOutputType == Comment)
            || (mLastOutputType == EndComp)) {
            outputString();
        }

        outputString(Comp,
                     QString("comp%1 %2;").arg(cmetaId(pDomNode))
                                          .arg(cellmlAttributeNodeValue(pDomNode, "component")));
    }

    return true;
}

//==============================================================================

bool CellMLTextViewConverter::processConnectionNode(const QDomNode &pDomNode)
{
    // Start processing the given connection node

    static const QString MapComponents = "___MAP_COMPONENTS___";

    if (   (mLastOutputType == Comment)
        || (mLastOutputType == EndDef)) {
        outputString();
    }

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
            if (!processMapVariablesNode(domNode))
                return false;
        } else if (!processUnknownNode(domNode, false)) {
            return false;
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
        else if (!processUnknownNode(domNode, false))
            return false;
    }

    // Process the given map components node

    pMapComponents = QString("between%1 %2 and %3").arg(cmetaId(pDomNode))
                                                   .arg(cellmlAttributeNodeValue(pDomNode, "component_1"))
                                                   .arg(cellmlAttributeNodeValue(pDomNode, "component_2"));

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
        if (domNode.isComment())
            processCommentNode(domNode);
        else if (rdfNode(domNode))
            processRdfNode(domNode);
        else if (!processUnknownNode(domNode, false))
            return false;
    }

    // Process the given unit node

    if (mLastOutputType == Comment)
        outputString();

    outputString(Vars,
                 QString("vars%1 %2 and %3;").arg(cmetaId(pDomNode))
                                             .arg(cellmlAttributeNodeValue(pDomNode, "variable_1"))
                                             .arg(cellmlAttributeNodeValue(pDomNode, "variable_2")));

    return true;
}

//==============================================================================

bool CellMLTextViewConverter::processUnknownNode(const QDomNode &pDomNode,
                                                 const bool &pError)
{
    // The given node is unknown, so warn the user about it

    switch (pDomNode.nodeType()) {
    case QDomNode::ElementNode:
        processUnsupportedNode(pDomNode, pError);

        if (pError)
            return false;

        break;
    case QDomNode::AttributeNode:
        mWarnings << CellMLTextViewConverterWarning(pDomNode.lineNumber(),
                                                    QObject::tr("An attribute was found in the original CellML file, but it was not processed."));

        break;
    case QDomNode::TextNode:
        mWarnings << CellMLTextViewConverterWarning(pDomNode.lineNumber(),
                                                    QObject::tr("Some text was found in the original CellML file, but it was not processed."));

        break;
    case QDomNode::CDATASectionNode:
        mWarnings << CellMLTextViewConverterWarning(pDomNode.lineNumber(),
                                                    QObject::tr("A CDATA section was found in the original CellML file, but it was not processed."));

        break;
    case QDomNode::EntityReferenceNode:
        mWarnings << CellMLTextViewConverterWarning(pDomNode.lineNumber(),
                                                    QObject::tr("An entity reference was found in the original CellML file, but it was not processed."));

        break;
    case QDomNode::EntityNode:
        mWarnings << CellMLTextViewConverterWarning(pDomNode.lineNumber(),
                                                    QObject::tr("An entity was found in the original CellML file, but it was not processed."));

        break;
    case QDomNode::ProcessingInstructionNode:
        mWarnings << CellMLTextViewConverterWarning(pDomNode.lineNumber(),
                                                    QObject::tr("A processing instruction was found in the original CellML file, but it is not known and cannot therefore be processed."));

        break;
    case QDomNode::CommentNode:
        mWarnings << CellMLTextViewConverterWarning(pDomNode.lineNumber(),
                                                    QObject::tr("A comment was found in the original CellML file, but it was not processed."));

        break;
    case QDomNode::DocumentNode:
        mWarnings << CellMLTextViewConverterWarning(pDomNode.lineNumber(),
                                                    QObject::tr("A document was found in the original CellML file, but it was not processed."));

        break;
    case QDomNode::DocumentTypeNode:
        mWarnings << CellMLTextViewConverterWarning(pDomNode.lineNumber(),
                                                    QObject::tr("A document type was found in the original CellML file, but it was not processed."));

        break;
    case QDomNode::DocumentFragmentNode:
        mWarnings << CellMLTextViewConverterWarning(pDomNode.lineNumber(),
                                                    QObject::tr("A document fragment was found in the original CellML file, but it was not processed."));

        break;
    case QDomNode::NotationNode:
        mWarnings << CellMLTextViewConverterWarning(pDomNode.lineNumber(),
                                                    QObject::tr("A notation was found in the original CellML file, but it was not processed."));

        break;
    case QDomNode::BaseNode:
        mWarnings << CellMLTextViewConverterWarning(pDomNode.lineNumber(),
                                                    QObject::tr("A base was found in the original CellML file, but it was not processed."));

        break;
    case QDomNode::CharacterDataNode:
        mWarnings << CellMLTextViewConverterWarning(pDomNode.lineNumber(),
                                                    QObject::tr("Some character data was found in the original CellML file, but it is not known and cannot therefore be processed."));

        break;
    }

    return true;
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
