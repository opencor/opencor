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

#pragma once

//==============================================================================

#include <QDomNode>
#include <QMap>
#include <QObject>
#include <QStringList>

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

class CellMLTextViewConverterWarning
{
public:
    explicit CellMLTextViewConverterWarning(const QString &pMessage,
                                            int pLineNumber, int pColumnNumber);

    QString message() const;
    int lineNumber() const;
    int columnNumber() const;

private:
    QString mMessage;
    int mLineNumber;
    int mColumnNumber;
};

//==============================================================================

using CellMLTextViewConverterWarnings = QList<CellMLTextViewConverterWarning>;

//==============================================================================

class CellMLTextViewConverter : public QObject
{
    Q_OBJECT

public:
    explicit CellMLTextViewConverter();

    bool execute(const QString &pRawCellml);

    QString output() const;

    QString errorMessage() const;
    int errorLine() const;
    int errorColumn() const;

    bool hasWarnings() const;
    CellMLTextViewConverterWarnings warnings() const;

    QDomNode documentationNode() const;
    QDomDocument rdfNodes() const;

private:
    enum class Output {
        None,
        EmptyLine,
        DefModel,
        Comment,
        DefImport,
        ImportUnit,
        ImportComp,
        DefUnit,
        DefBaseUnit,
        Unit,
        DefComp,
        Var,
        Equation,
        DefGroup,
        CompIncl,
        Comp,
        EndComp,
        DefMap,
        Vars,
        EndDef
    };

    enum class MathmlNode {
        Unknown,
        Eq, Neq, Lt, Leq, Geq, Gt,
        Plus, Minus, Times, Divide,
        And, Or, Xor
    };

    QString mOutput;
    QString mPrevIndent;
    QString mIndent;

    Output mLastOutput = Output::None;

    QString mErrorMessage;
    int mErrorLine = 0;
    int mErrorColumn = 0;

    CellMLTextViewConverterWarnings mWarnings;

    QDomNode mModelNode;
    QDomNode mDocumentationNode;
    QDomNode mTopMathmlNode;

    QDomNamedNodeMap mAttributes;
    QDomDocument mRdfNodes;

    bool mAssignmentDone = false;

    bool mOldPiecewiseStatementUsed = false;
    bool mPiecewiseStatementUsed = false;

    QMap<QString, QString> mMappings;
    QMap<QString, MathmlNode> mMathmlNodes;

    void reset();

    void indent(bool pForceTracking = true);
    void unindent();

    void outputString(Output pOutputType = Output::EmptyLine,
                      const QString &pString = {});

    bool rdfNode(const QDomNode &pDomNode) const;
    bool cellmlNode(const QDomNode &pDomNode, const QString &pName) const;
    bool mathmlNode(const QDomNode &pDomNode, const QString &pName) const;

    QString cmetaId(const QDomNode &pDomNode) const;

    MathmlNode mathmlNode(const QDomNode &pDomNode) const;

    QString attributeNodeValue(const QDomNode &pDomNode,
                               const QString &pNamespace,
                               const QString &pName,
                               bool pMustBePresent = true) const;
    QString cellmlAttributeNodeValue(const QDomNode &pDomNode,
                                     const QString &pName,
                                     bool pMustBePresent = true) const;

    bool isRelationalOperator(MathmlNode pOperandNodeType) const;
    bool isPlusOperator(MathmlNode pOperandNodeType) const;
    bool isMinusOperator(MathmlNode pOperandNodeType) const;
    bool isTimesOperator(MathmlNode pOperandNodeType) const;
    bool isDivideOperator(MathmlNode pOperandNodeType) const;
    bool isLogicalOrBitwiseOperator(MathmlNode pOperandNodeType) const;
    bool isAndOperator(MathmlNode pOperandNodeType) const;
    bool isOrOperator(MathmlNode pOperandNodeType) const;
    bool isXorOperator(MathmlNode pOperandNodeType) const;

    bool processModelNode(const QDomNode &pDomNode);
    QString processCommentString(const QString &pComment);
    void processCommentNode(const QDomNode &pDomNode);
    void processRdfNode(const QDomNode &pDomNode);
    bool processImportNode(const QDomNode &pDomNode);
    bool processUnitsNode(const QDomNode &pDomNode, bool pInImportNode = false);
    bool processUnitNode(const QDomNode &pDomNode);
    bool processComponentNode(const QDomNode &pDomNode,
                              bool pInImportNode = false);
    bool processVariableNode(const QDomNode &pDomNode);
    bool processMathNode(const QDomNode &pDomNode);
    int childNodesCount(const QDomNode &pDomNode) const;
    QDomNode childNode(const QDomNode &pDomNode, int pChildNodeIndex) const;
    QString processMathmlNode(const QDomNode &pDomNode, bool &pHasError);
    QString processPiecewiseNode(const QDomNode &pDomNode, bool &pHasError);
    QString processPieceNode(const QDomNode &pDomNode, bool &pHasError);
    QString processOtherwiseNode(const QDomNode &pDomNode, bool &pHasError);
    QString processOperatorNode(const QString &pOperator,
                                const QDomNode &pDomNode, bool &pHasError);
    QString processFunctionNode(const QString &pFunction,
                                const QDomNode &pDomNode, bool &pHasError);
    QString processPowerNode(const QDomNode &pDomNode, bool &pHasError);
    QString processRootNode(const QDomNode &pDomNode, bool &pHasError);
    QString processLogNode(const QDomNode &pDomNode, bool &pHasError);
    QString processNotNode(const QDomNode &pDomNode, bool &pHasError);
    QString processDiffNode(const QDomNode &pDomNode, bool &pHasError);
    QString processChildNode(const QDomNode &pDomNode, bool &pHasError);
    QString processBvarNode(const QDomNode &pDomNode, bool &pHasError);
    bool processReactionNode(const QDomNode &pDomNode);
    bool processGroupNode(const QDomNode &pDomNode);
    bool processRelationshipRefNode(const QDomNode &pDomNode,
                                    QString &pRelationshipRef);
    bool processComponentRefNode(const QDomNode &pDomNode);
    bool processConnectionNode(const QDomNode &pDomNode);
    bool processMapComponentsNode(const QDomNode &pDomNode,
                                  QString &pMapComponents);
    bool processMapVariablesNode(const QDomNode &pDomNode);
    bool processUnknownNode(const QDomNode &pDomNode, bool pError);
    void processUnsupportedNode(const QDomNode &pDomNode, bool pError,
                                const QString &pExtra = {});
};

//==============================================================================

} // namespace CellMLTextView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
