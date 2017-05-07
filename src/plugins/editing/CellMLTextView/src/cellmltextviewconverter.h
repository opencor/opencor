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

    void reset();

    void indent(const bool &pForceTracking = true);
    void unindent();

    void outputString(const OutputType &pOutputType = EmptyLine,
                      const QString &pString = QString());

    bool rdfNode(const QDomNode &pDomNode) const;
    bool cellmlNode(const QDomNode &pDomNode, const QString &pName) const;
    bool mathmlNode(const QDomNode &pDomNode, const QString &pName) const;

    QString cmetaId(const QDomNode &pDomNode) const;

    MathmlNodeType mathmlNodeType(const QDomNode &pDomNode) const;

    QString attributeNodeValue(const QDomNode &pDomNode,
                               const QString &pNamespace,
                               const QString &pName,
                               const bool &pMustBePresent = true) const;
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
    bool processUnknownNode(const QDomNode &pDomNode, const bool &pError);
    void processUnsupportedNode(const QDomNode &pDomNode, const bool &pError,
                                const QString &pExtra = QString());
};

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
