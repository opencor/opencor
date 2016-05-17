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
// CellML file manager
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfile.h"
#include "cellmlsupportglobal.h"
#include "standardfilemanager.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

typedef QMap<QString, CellmlFile *> CellmlFiles;

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileManager : public StandardSupport::StandardFileManager
{
    Q_OBJECT

public:
    static CellmlFileManager * instance();

    bool isCellmlFile(const QString &pFileName) const;

    CellmlFile * cellmlFile(const QString &pFileName);

protected:
    virtual bool canLoadFile(const QString &pFileName) const;

    virtual QObject * newFile(const QString &pFileName) const;
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
