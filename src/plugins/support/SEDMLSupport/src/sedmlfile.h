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
// SED-ML file class
//==============================================================================

#ifndef SEDMLFILE_H
#define SEDMLFILE_H

//==============================================================================

#include "sedmlfileissue.h"
#include "sedmlsupportglobal.h"
#include "standardfile.h"

//==============================================================================

#include <QString>

//==============================================================================

namespace OpenCOR {
namespace SEDMLSupport {

//==============================================================================

static const auto OpencorNamespace = QStringLiteral("http://www.opencor.ws/");

//==============================================================================

namespace Language {
    static const auto Cellml_1_0 = QStringLiteral("urn:sedml:language:cellml.1_0");
    static const auto Cellml_1_1 = QStringLiteral("urn:sedml:language:cellml.1_1");
}

//==============================================================================

class SEDMLSUPPORT_EXPORT SedmlFile : public StandardSupport::StandardFile
{
    Q_OBJECT

public:
    explicit SedmlFile(const QString &pFileName);

    virtual bool load();
    virtual bool save(const QString &pNewFileName = QString());

    bool isValid(const QString &pFileContents, SedmlFileIssues &pIssues);
};

//==============================================================================

}   // namespace SEDMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
