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

#ifndef PRETTYCELLMLVIEWCELLMLTOPRETTYCELLMLCONVERTER_H
#define PRETTYCELLMLVIEWCELLMLTOPRETTYCELLMLCONVERTER_H

//==============================================================================

#include "corecliutils.h"

//==============================================================================

#include <QDomNode>
#include <QStringList>

//==============================================================================

namespace OpenCOR {
namespace PrettyCellMLView {

//==============================================================================

class PrettyCellMLViewCellmlToPrettyCellmlConverter
{
public:
    explicit PrettyCellMLViewCellmlToPrettyCellmlConverter();

    bool execute(const QString &pFileName);

    QString output() const;

    int errorLine() const;
    int errorColumn() const;
    QString errorMessage() const;

    bool hasWarnings() const;
    QIntList warningLines() const;
    QStringList warningMessages() const;

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

    QString mOutput;
    QString mIndent;

    OutputType mLastOutputType;

    int mErrorLine;
    int mErrorColumn;
    QString mErrorMessage;

    QIntList mWarningLines;
    QStringList mWarningMessages;

    QDomNamedNodeMap mAttributes;
    QDomDocument mRdfNodes;

    bool mAssignmentDone;

    QMap<QString, QString> mMappings;

    void indent();
    void unindent();

    void outputString(const OutputType &pOutputType = EmptyLine,
                      const QString &pString = QString());

    QString cmetaId(const QDomNode &pDomNode) const;

    bool processModelNode(const QDomNode &pDomNode);
    void processCommentNode(const QDomNode &pDomNode);
    void processRdfNode(const QDomNode &pDomNode);
    bool processImportNode(const QDomNode &pDomNode);
    bool processUnitsNode(const QDomNode &pDomNode,
                          const bool &pInImportNode = false);
    void processUnitNode(const QDomNode &pDomNode);
    bool processComponentNode(const QDomNode &pDomNode,
                              const bool &pInImportNode = false);
    void processVariableNode(const QDomNode &pDomNode);
    bool processMathNode(const QDomNode &pDomNode);
    QString processMathmlNode(const QDomNode &pDomNode, bool &pHasError);
    QString processOperatorNode(const QString &pOperator,
                                const QDomNode &pDomNode, bool &pHasError);
    QString processFunctionNode(const QString &pFunction,
                                const QDomNode &pDomNode, bool &pHasError);
    QString processPowerNode(const QDomNode &pDomNode, bool &pHasError);
    QString processRootNode(const QDomNode &pDomNode, bool &pHasError);
    QString processLogNode(const QDomNode &pDomNode, bool &pHasError);
    QString processNotNode(const QDomNode &pDomNode, bool &pHasError);
    bool processReactionNode(const QDomNode &pDomNode);
    bool processGroupNode(const QDomNode &pDomNode);
    bool processRelationshipRefNode(const QDomNode &pDomNode,
                                    QString &pRelationshipRef);
    void processComponentRefNode(const QDomNode &pDomNode);
    bool processConnectionNode(const QDomNode &pDomNode);
    bool processMapComponentsNode(const QDomNode &pDomNode,
                                  QString &pMapComponents);
    void processMapVariablesNode(const QDomNode &pDomNode);
    void processUnknownNode(const QDomNode &pDomNode);
    void processUnsupportedNode(const QDomNode &pDomNode);
};

//==============================================================================

}   // namespace PrettyCellMLView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
