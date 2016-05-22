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
// Standard file manager
//==============================================================================

#pragma once

//==============================================================================

#include "standardfile.h"
#include "standardsupportglobal.h"

//==============================================================================

#include <QMap>
#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace StandardSupport {

//==============================================================================

typedef QMap<QString, QObject *> Files;

//==============================================================================

class STANDARDSUPPORT_EXPORT StandardFileManager : public QObject
{
    Q_OBJECT

public:
    bool isFile(const QString &pFileName);

    QObject * file(const QString &pFileName);

protected:
    Files mFiles;

    explicit StandardFileManager();
    ~StandardFileManager();

    virtual bool canLoadFile(const QString &pFileName) const = 0;

    virtual QObject * newFile(const QString &pFileName) const = 0;

private Q_SLOTS:
    void manageFile(const QString &pFileName);
    void unmanageFile(const QString &pFileName);

    void reloadFile(const QString &pFileName, const bool &pFileChanged);
    void renameFile(const QString &pOldFileName, const QString &pNewFileName);

    void saveFile(const QString &pFileName);
};

//==============================================================================

}   // namespace StandardSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
