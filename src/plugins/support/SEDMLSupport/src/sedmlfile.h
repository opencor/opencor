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

namespace libsedml {
    class SedAlgorithm;
    class SedDocument;
}   // namespace libsedml

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFile;
}   // namespace CellMLSupport

//==============================================================================

namespace SEDMLSupport {

//==============================================================================

static const auto OpencorNamespace    = QStringLiteral("http://www.opencor.ws/");
static const auto VariableDegree      = QStringLiteral("variableDegree");
static const auto SolverProperties    = QStringLiteral("solverProperties");
static const auto SolverProperty      = QStringLiteral("solverProperty");
static const auto SolverPropertyId    = QStringLiteral("id");
static const auto SolverPropertyValue = QStringLiteral("value");
static const auto NlaSolver           = QStringLiteral("nlaSolver");
static const auto NlaSolverName       = QStringLiteral("name");
static const auto LineProperties      = QStringLiteral("lineProperties");
static const auto LineProperty        = QStringLiteral("lineProperty");
static const auto LinePropertyId      = QStringLiteral("id");
static const auto LinePropertyValue   = QStringLiteral("value");
static const auto LineStyle           = QStringLiteral("style");
static const auto LineWidth           = QStringLiteral("width");
static const auto LineColor           = QStringLiteral("color");
static const auto SymbolProperties    = QStringLiteral("symbolProperties");
static const auto SymbolProperty      = QStringLiteral("symbolProperty");
static const auto SymbolPropertyId    = QStringLiteral("id");
static const auto SymbolPropertyValue = QStringLiteral("value");
static const auto SymbolStyle         = QStringLiteral("style");
static const auto SymbolSize          = QStringLiteral("size");
static const auto SymbolColor         = QStringLiteral("color");
static const auto SymbolFilled        = QStringLiteral("filled");
static const auto SymbolFillColor     = QStringLiteral("fillColor");

//==============================================================================

namespace Language {
    static const auto Cellml     = QStringLiteral("urn:sedml:language:cellml");
    static const auto Cellml_1_0 = QStringLiteral("urn:sedml:language:cellml.1_0");
    static const auto Cellml_1_1 = QStringLiteral("urn:sedml:language:cellml.1_1");
}

//==============================================================================

class SEDMLSUPPORT_EXPORT SedmlFile : public StandardSupport::StandardFile
{
    Q_OBJECT

public:
    explicit SedmlFile(const QString &pFileName, const QString &pOwnerFileName,
                       const bool &pNew = false);
    explicit SedmlFile(const QString &pFileName, const bool &pNew = false);
    ~SedmlFile();

    libsedml::SedDocument * sedmlDocument();

    virtual bool load();
    virtual bool save(const QString &pFileName = QString());

    bool isValid(const QString &pFileContents, SedmlFileIssues &pIssues);
    bool isValid();
    bool isSupported();

    int lineStyleValueIndex(const QString &pLineStyleValue) const;
    QString lineStyleValue(const int &pLineStyleValueIndex) const;

    int symbolStyleValueIndex(const QString &pSymbolStyleValue) const;
    QString symbolStyleValue(const int &pSymbolStyleValueIndex) const;

    CellMLSupport::CellmlFile * cellmlFile();

    SedmlFileIssues issues() const;

private:
    QString mOwnerFileName;

    libsedml::SedDocument *mSedmlDocument;

    bool mNew;
    bool mLoadingNeeded;

    QStringList mLineStyles;
    QStringList mSymbolStyles;

    CellMLSupport::CellmlFile *mCellmlFile;

    SedmlFileIssues mIssues;

    virtual void reset();

    bool algorithmSupported(const libsedml::SedAlgorithm *pSedmlAlgorithm);
};

//==============================================================================

}   // namespace SEDMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
