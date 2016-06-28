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
// PMR workspace
//==============================================================================

#pragma once

//==============================================================================

#include "pmrsupportglobal.h"

//==============================================================================

#include <QList>
#include <QObject>
#include <QSet>
#include <QString>
#include <QVector>

//==============================================================================

#include "git2.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

class PMRSUPPORT_EXPORT PmrWorkspace : public QObject
{
    Q_OBJECT

public:
    explicit PmrWorkspace(QObject *parent=0);
    PmrWorkspace(const QString &pUrl, const QString &pName, QObject *parent);

    static bool compare(const PmrWorkspace *pFirst, const PmrWorkspace *pSecond);

    static QString getUrlFromFolder(const QString &pFolder);

    bool isLocal(void) const;
    bool isNull(void) const;

    void setCredentials(const QString &pUsername, const QString &pPassword);
    void setDescription(const QString &pDescription);
    void setName(const QString &pName);
    void setOwner(const QString &pOwner);
    void setPath(const QString &pPath);

    bool isOwned(void) const;
    void setOwned(const bool &pOwned);

    const QString &description(void) const;
    const QString &name(void) const;
    const QString &owner(void) const;
    const QString &path(void) const;
    const QString &url(void) const;

    void clone(const QString &pDirName);

    const QString gitStatus(void) const;
    const QString gitStatus(const QString &pPath) const;

//    void pull();
//    void push();

    static QString WorkspacesDirectory();

private:
    bool mOwned;

    QString mDescription;
    QString mName;
    QString mOwner;
    QString mPassword;
    QString mPath;
    QString mUrl;
    QString mUsername;

    static int certificate_check_cb(git_cert *cert, int valid, const char *host, void *payload);

    static void checkout_progress_cb(const char *path, size_t completed_steps, size_t total_steps,
                                     void *payload);
    static int transfer_progress_cb(const git_transfer_progress *stats, void *payload);

    void emitProgress(const double &pProgress);

Q_SIGNALS:
    void progress(const double &pProgress);
    void warning(const QString &pMessage) const;

    void workspaceCloneFinished(void);
    void workspacePulled(PmrWorkspace *pWorkspace);
    void workspacePushed(PmrWorkspace *pWorkspace);
};

//==============================================================================
//==============================================================================

#ifdef _MSC_VER
template class PMRSUPPORT_EXPORT QSet<PmrWorkspace *>;
template class PMRSUPPORT_EXPORT QVector<PmrWorkspace *>;
#endif

class PMRSUPPORT_EXPORT PmrWorkspaceList : public QList<PmrWorkspace *>
{
public:
    PmrWorkspaceList();

    void add(const QString &pUrl, const QString &pName, QObject *parent);
};

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
