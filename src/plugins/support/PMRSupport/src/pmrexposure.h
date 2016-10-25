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
// PMR exposure
//==============================================================================

#pragma once

//==============================================================================

#include "pmrsupportglobal.h"

//==============================================================================

#include <QObject>
#include <QList>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QVector>

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

class PmrWorkspace;

//==============================================================================

class PMRSUPPORT_EXPORT PmrExposure : public QObject
{
    Q_OBJECT

public:
    explicit PmrExposure(const QString &pUrl, const QString &pName,
                        QObject *pParent);

    static bool compare(const PmrExposure *pFirst, const PmrExposure *pSecond);

    QString name() const;
    QString url() const;

    int fileUrlsLeftCount() const;
    void setFileUrlsLeftCount(const int &count);

    void addExposureFile(const QString &pFileName);
    void addOtherFile(const QString &pFileName);
    const QStringList exposureFileList() const;

    PmrWorkspace * workspace() const;
    void setWorkspace(PmrWorkspace *pWorkspace);

    QString toHtml() const;

private:
    QString mName;
    QString mUrl;

    QStringList mExposureFileList;
    int mFileUrlsLeftCount;
    PmrWorkspace *mWorkspace;
};

//==============================================================================

class PMRSUPPORT_EXPORT PmrExposures : public QList<PmrExposure *>
{
public:
    void add(const QString &pUrl, const QString &pName, QObject *pParent);
};

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
