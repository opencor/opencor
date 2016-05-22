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
// SED-ML file class
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
    class SedDocument;
}

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace SEDMLSupport {

//==============================================================================

static const auto OpencorNamespace = QStringLiteral("http://www.opencor.ws/");
static const auto VariableDegree = QStringLiteral("variableDegree");
static const auto SolverProperties = QStringLiteral("solverProperties");
static const auto SolverProperty = QStringLiteral("solverProperty");
static const auto SolverPropertyId = QStringLiteral("id");
static const auto SolverPropertyValue = QStringLiteral("value");
static const auto NlaSolver = QStringLiteral("nlaSolver");
static const auto NlaSolverName = QStringLiteral("name");

//==============================================================================

namespace Language {
    static const auto Cellml = QStringLiteral("urn:sedml:language:cellml");
    static const auto Cellml_1_0 = QStringLiteral("urn:sedml:language:cellml.1_0");
    static const auto Cellml_1_1 = QStringLiteral("urn:sedml:language:cellml.1_1");
}

//==============================================================================

class SEDMLSUPPORT_EXPORT SedmlFile : public StandardSupport::StandardFile
{
    Q_OBJECT

public:
    explicit SedmlFile(const QString &pFileName, const bool &pNew = false);
    ~SedmlFile();

    libsedml::SedDocument * sedmlDocument();

    virtual bool load();
    virtual bool save(const QString &pFileName = QString());

    bool isValid(const QString &pFileContents, SedmlFileIssues &pIssues);

private:
    libsedml::SedDocument *mSedmlDocument;

    bool mNew;
    bool mLoadingNeeded;

    virtual void reset();
};

//==============================================================================

}   // namespace SEDMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
