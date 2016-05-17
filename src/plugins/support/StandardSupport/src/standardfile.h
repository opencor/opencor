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
// Standard file class
//==============================================================================

#pragma once

//==============================================================================

#include "standardsupportglobal.h"

//==============================================================================

#include <QObject>
#include <QString>

//==============================================================================

namespace OpenCOR {
namespace StandardSupport {

//==============================================================================

class STANDARDSUPPORT_EXPORT StandardFile : public QObject
{
    Q_OBJECT

public:
    explicit StandardFile(const QString &pFileName);

    virtual bool load() = 0;
    bool reload();

    virtual bool save(const QString &pFileName = QString()) = 0;

    QString fileName() const;
    void setFileName(const QString &pFileName);

protected:
    QString mFileName;

    virtual void reset() = 0;
};

//==============================================================================

}   // namespace StandardSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
