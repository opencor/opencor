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
// CellML file exporter
//==============================================================================

#pragma once

//==============================================================================

#include <QString>

//==============================================================================

#include "cellmlapibegin.h"
    #include "IfaceCellML_APISPEC.hxx"
#include "cellmlapiend.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CellmlFileExporter
{
public:
    explicit CellmlFileExporter();

    bool result() const;

    QString errorMessage() const;

protected:
    bool mResult;

    QString mErrorMessage;

    bool saveModel(iface::cellml_api::Model *pModel, const QString &pFileName);
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
