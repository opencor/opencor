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
// Standard file manager
//==============================================================================

#ifndef STANDARDFILEMANAGER_H
#define STANDARDFILEMANAGER_H

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

typedef QMap<QString, QObject *> StandardFiles;

//==============================================================================

class STANDARDSUPPORT_EXPORT StandardFileManager : public QObject
{
    Q_OBJECT

public:
    QObject *standardFile(const QString &pFileName);

    virtual bool isStandardFile(const QString &pFileName) const = 0;

protected:
    virtual QObject * newStandardFile(const QString &pFileName) const = 0;
    virtual void deleteStandardFile(QObject *pStandardFile) const = 0;

private:
    StandardFiles mStandardFiles;

    explicit StandardFileManager();
    ~StandardFileManager();

private Q_SLOTS:
    void manageFile(const QString &pFileName);
    void unmanageFile(const QString &pFileName);

    void reloadFile(const QString &pFileName);

    void renameFile(const QString &pOldFileName, const QString &pNewFileName);
};

//==============================================================================

}   // namespace StandardSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
