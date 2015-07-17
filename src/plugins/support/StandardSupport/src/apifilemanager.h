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
// API file manager
//==============================================================================

#ifndef APIFILEMANAGER_H
#define APIFILEMANAGER_H

//==============================================================================

#include "apifile.h"
#include "apisupportglobal.h"

//==============================================================================

#include <QMap>
#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace APISupport {

//==============================================================================

typedef QMap<QString, ApiFile *> ApiFiles;

//==============================================================================

class APISUPPORT_EXPORT ApiFileManager : public QObject
{
    Q_OBJECT

public:
    ApiFile * apiFile(const QString &pFileName);

    virtual bool isApiFile(const QString &pFileName) const = 0;

private:
    ApiFiles mApiFiles;

    explicit ApiFileManager();
    ~ApiFileManager();

private Q_SLOTS:
    void manageFile(const QString &pFileName);
    void unmanageFile(const QString &pFileName);

    void reloadFile(const QString &pFileName);

    void renameFile(const QString &pOldFileName, const QString &pNewFileName);
};

//==============================================================================

}   // namespace APISupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
