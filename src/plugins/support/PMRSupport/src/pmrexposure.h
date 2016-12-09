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
#ifdef Q_OS_WIN
    #include <QSet>
    #include <QVector>
#endif

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

    static bool compare(const PmrExposure *pExposure1,
                        const PmrExposure *pExposure2);

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

class PMRSUPPORT_EXPORT PmrExposures : public QList<PmrExposure *>
{
public:
    explicit PmrExposures();
    ~PmrExposures();

    void add(const QString &pUrl, const QString &pName, QObject *parent);
};

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
