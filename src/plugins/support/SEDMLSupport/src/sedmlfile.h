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
// SED-ML file
//==============================================================================

#pragma once

//==============================================================================

#include "sedmlfileissue.h"
#include "sedmlsupportglobal.h"
#include "standardfile.h"

//==============================================================================

#include <QString>

//==============================================================================

#include "libsbmlbegin.h"
    #include "sbml/math/ASTNode.h"
#include "libsbmlend.h"

//==============================================================================

namespace libsedml {
    class SedDocument;
    class SedListOfAlgorithmParameters;
} // namespace libsedml

//==============================================================================

namespace OpenCOR {

//==============================================================================

class SolverInterface;

//==============================================================================

namespace CellMLSupport {
    class CellmlFile;
} // namespace CellMLSupport

//==============================================================================

namespace SEDMLSupport {

//==============================================================================

static const auto OpencorNamespace = QStringLiteral("http://www.opencor.ws/");
static const auto VariableDegree   = QStringLiteral("variableDegree");
static const auto SolverProperties = QStringLiteral("solverProperties");
static const auto SolverProperty   = QStringLiteral("solverProperty");
static const auto Id               = QStringLiteral("id");
static const auto Value            = QStringLiteral("value");
static const auto NlaSolver        = QStringLiteral("nlaSolver");
static const auto Name             = QStringLiteral("name");
static const auto Properties       = QStringLiteral("properties");
static const auto GridLines        = QStringLiteral("gridLines");
static const auto PointCoordinates = QStringLiteral("pointCoordinates");
static const auto SurroundingArea  = QStringLiteral("surroundingArea");
static const auto XAxis            = QStringLiteral("xAxis");
static const auto YAxis            = QStringLiteral("yAxis");
static const auto ZoomRegion       = QStringLiteral("zoomRegion");
static const auto Line             = QStringLiteral("line");
static const auto Symbol           = QStringLiteral("symbol");
static const auto BackgroundColor  = QStringLiteral("backgroundColor");
static const auto Color            = QStringLiteral("color");
static const auto FillColor        = QStringLiteral("fillColor");
static const auto Filled           = QStringLiteral("filled");
static const auto FontColor        = QStringLiteral("fontColor");
static const auto FontSize         = QStringLiteral("fontSize");
static const auto ForegroundColor  = QStringLiteral("foregroundColor");
static const auto Height           = QStringLiteral("height");
static const auto Legend           = QStringLiteral("legend");
static const auto LogarithmicScale = QStringLiteral("logarithmicScale");
static const auto Selected         = QStringLiteral("selected");
static const auto Size             = QStringLiteral("size");
static const auto Style            = QStringLiteral("style");
static const auto Title            = QStringLiteral("title");
static const auto Width            = QStringLiteral("width");

//==============================================================================

namespace Language {
    static const auto Cellml     = QStringLiteral("urn:sedml:language:cellml");
    static const auto Cellml_1_0 = QStringLiteral("urn:sedml:language:cellml.1_0");
    static const auto Cellml_1_1 = QStringLiteral("urn:sedml:language:cellml.1_1");
} // namespace Language

//==============================================================================

class SEDMLSUPPORT_EXPORT SedmlFile : public StandardSupport::StandardFile
{
    Q_OBJECT

public:
    explicit SedmlFile(const QString &pFileName, const QString &pOwnerFileName,
                       bool pNew = false);
    explicit SedmlFile(const QString &pFileName, bool pNew = false);
    ~SedmlFile() override;

    libsedml::SedDocument * sedmlDocument();

    bool load() override;
    bool save(const QString &pFileName = QString()) override;

    bool update(const QString &pFileName = QString());

    void forceNew();

    bool isValid(const QString &pFileContents, SedmlFileIssues &pIssues);
    bool isValid();
    bool isSupported();

    CellMLSupport::CellmlFile * cellmlFile();

    SedmlFileIssues issues() const;

private:
    QString mOwnerFileName;

    libsedml::SedDocument *mSedmlDocument = nullptr;

    bool mLoadingNeeded = true;

    CellMLSupport::CellmlFile *mCellmlFile = nullptr;

    SedmlFileIssues mIssues;

    bool mUpdated = false;

    void reset() override;

    bool isSedmlFile() const;

    bool hasErrors() const;

    SolverInterface * solverInterface(const QString &pKisaoId);

    bool validAlgorithmParameters(const libsedml::SedListOfAlgorithmParameters *pSedmlAlgorithmParameters,
                                  SolverInterface *pSolverInterface);

    bool validListPropertyValue(const libsbml::XMLNode &pPropertyNode,
                                const QString &pPropertyNodeValue,
                                const QString &pPropertyName,
                                const QStringList &pValuesList);
    bool validColorPropertyValue(const libsbml::XMLNode &pPropertyNode,
                                 const QString &pPropertyNodeValue,
                                 const QString &pPropertyName);
};

//==============================================================================

} // namespace SEDMLSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
