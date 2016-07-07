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
    explicit PmrExposure(QObject *parent=0);
    PmrExposure(const QString &pUrl, const QString &pName, QObject *parent);

    static bool compare(const PmrExposure *pFirst, const PmrExposure *pSecond);

    bool isNull() const;

    QString name() const;
    QString url() const;

    int fileUrlsLeftCount(void) const;
    void setFileUrlsLeftCount(const int &count);

    void addExposureFile(const QString &pFileName);
    void addOtherFile(const QString &pFileName);
    const QStringList exposureFileList(void) const;

    PmrWorkspace *workspace(void) const;
    void setWorkspace(PmrWorkspace *pWorkspace);

    QString toHtml(void) const;

private:
    QString mName;
    QString mUrl;

    QStringList mExposureFileList;
    int mFileUrlsLeftCount;
    PmrWorkspace *mWorkspace;
};

//==============================================================================

#ifdef _MSC_VER
template class PMRSUPPORT_EXPORT QSet<PmrExposure *>;
template class PMRSUPPORT_EXPORT QVector<PmrExposure *>;
#endif

class PMRSUPPORT_EXPORT PmrExposureList : public QList<PmrExposure *>
{
public:
    PmrExposureList();

    void add(const QString &pUrl, const QString &pName, QObject *parent);
};

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
