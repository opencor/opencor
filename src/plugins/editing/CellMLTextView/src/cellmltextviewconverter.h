/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Raw CellML to CellML Text converter
//==============================================================================

#pragma once

//==============================================================================

#include <QDomNode>
#include <QMap>
#include <QStringList>

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

class CellMLTextViewConverterWarning
{
public:
    explicit CellMLTextViewConverterWarning(const int &pLine,
                                            const QString &pMessage);

    int line() const;

    QString message() const;

private:
    int mLine;

    QString mMessage;
};

//==============================================================================

typedef QList<CellMLTextViewConverterWarning> CellMLTextViewConverterWarnings;

//==============================================================================

class CellMLTextViewConverter
{
public:
    explicit CellMLTextViewConverter();

    bool execute(const QString &pRawCellml);

    QString output() const;

    int errorLine() const;
    int errorColumn() const;
    QString errorMessage() const;

    bool hasWarnings() const;
    CellMLTextViewConverterWarnings warnings() const;

    QDomDocument rdfNodes() const;

private:
    enum OutputType {
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

    enum MathmlNodeType {
        UnknownMathmlNode,
        EqMathmlNode, NeqMathmlNode, LtMathmlNode, LeqMathmlNode, GeqMathmlNode, GtMathmlNode,
        PlusMathmlNode, MinusMathmlNode, TimesMathmlNode, DivideMathmlNode,
        AndMathmlNode, OrMathmlNode, XorMathmlNode
    };

    QString mOutput;
    QString mPrevIndent;
    QString mIndent;

    OutputType mLastOutputType;

    int mErrorLine;
    int mErrorColumn;
    QString mErrorMessage;

    CellMLTextViewConverterWarnings mWarnings;

    QDomNamedNodeMap mAttributes;
    QDomDocument mRdfNodes;

    QDomNode mTopMathmlNode;

    bool mAssignmentDone;

    bool mOldPiecewiseStatementUsed;
    bool mPiecewiseStatementUsed;

    QMap<QString, QString> mMappings;
    QMap<QString, MathmlNodeType> mMathmlNodeTypes;

    void indent(const bool &pForceTracking = true);
    void unindent();

    void outputString(const OutputType &pOutputType = EmptyLine,
                      const QString &pString = QString());

    bool rdfNode(const QDomNode &pDomNode) const;
    bool documentationNode(const QDomNode &pDomNode) const;
    bool cellmlNode(const QDomNode &pDomNode, const QString &pName) const;
    bool mathmlNode(const QDomNode &pDomNode, const QString &pName) const;

    QString cmetaId(const QDomNode &pDomNode) const;

    MathmlNodeType mathmlNodeType(const QDomNode &pDomNode) const;

    QString cellmlAttributeNodeValue(const QDomNode &pDomNode,
                                     const QString &pName,
                                     const bool &pMustBePresent = true) const;

    bool processModelNode(const QDomNode &pDomNode);
    QString processCommentString(const QString &pComment);
    void processCommentNode(const QDomNode &pDomNode);
    void processRdfNode(const QDomNode &pDomNode);
    bool processImportNode(const QDomNode &pDomNode);
    bool processUnitsNode(const QDomNode &pDomNode,
                          const bool &pInImportNode = false);
    bool processUnitNode(const QDomNode &pDomNode);
    bool processComponentNode(const QDomNode &pDomNode,
                              const bool &pInImportNode = false);
    bool processVariableNode(const QDomNode &pDomNode);
    bool processMathNode(const QDomNode &pDomNode);
    int childNodesCount(const QDomNode &pDomNode) const;
    QDomNode childNode(const QDomNode &pDomNode,
                       const int &pChildNodeIndex) const;
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
    bool processUnknownNode(const QDomNode &pDomNode);
    void processUnsupportedNode(const QDomNode &pDomNode, const bool &pError,
                                const QString &pExtra = QString());
};

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
