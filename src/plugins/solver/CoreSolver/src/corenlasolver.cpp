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
// Core NLA solver class
//==============================================================================

#include "corenlasolver.h"
#include "settings.h"

//==============================================================================

#include <QSettings>

//==============================================================================

void doNonLinearSolve(char *pRuntime,
                      void (*pFunction)(double *, double *, void *),
                      double *pParameters, int *pRes, int pSize,
                      void *pUserData)
{
    // Retrieve the NLA solver which we should use

    OpenCOR::CoreSolver::CoreNlaSolver *nlaSolver = OpenCOR::CoreSolver::nlaSolver(pRuntime);

    if (nlaSolver) {
        // We have found our NLA solver, so initialise it

        nlaSolver->initialize(pFunction, pParameters, pSize, pUserData);

        // Now, we can solve our NLA system

        nlaSolver->solve();

        *pRes = 1;
    } else {
        // We couldn't retrieve an NLA solver...
        // Note: this should never happen, but we never know...

        qWarning("doNonLinearSolve(): no NLA solver could be found...");

        *pRes = 0;
    }
}

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

CoreNlaSolver::CoreNlaSolver() :
    mComputeSystem(0),
    mParameters(0),
    mSize(0),
    mUserData(0)
{
}

//==============================================================================

void CoreNlaSolver::initialize(ComputeSystemFunction pComputeSystem,
                               double *pParameters, int pSize, void *pUserData)
{
    // Initialise ourselves

    mComputeSystem = pComputeSystem;

    mParameters = pParameters;
    mSize = pSize;
    mUserData = pUserData;
}

//==============================================================================

static const auto SettingsGlobal = QStringLiteral("Global");

//==============================================================================

CoreNlaSolver * nlaSolver(const QString &pRuntime)
{
    // Return the runtime's NLA solver

    QSettings settings(SettingsOrganization, SettingsApplication);
    qulonglong res;

    settings.beginGroup(SettingsGlobal);
        res = settings.value(pRuntime, 0).toULongLong();
    settings.endGroup();

    // Return the NLA solver

    return static_cast<CoreNlaSolver *>((void *) res);
}

//==============================================================================

void setNlaSolver(const QString &pRuntime, CoreNlaSolver *pGlobalNlaSolver)
{
    // Keep track of the runtime's NLA solver

    QSettings settings(SettingsOrganization, SettingsApplication);

    settings.beginGroup(SettingsGlobal);
        settings.setValue(pRuntime, QString::number(qulonglong(pGlobalNlaSolver)));
        // Note #1: for some reasons, on OS X, QSettings doesn't handle
        //          qulonglong values properly, so we do it through a QString
        //          value instead...
        // Note #2: see https://bugreports.qt.io/browse/QTBUG-29681 for more
        //          information...
    settings.endGroup();
}

//==============================================================================

void unsetNlaSolver(const QString &pRuntime)
{
    // Stop tracking the runtime's NLA solver

    QSettings settings(SettingsOrganization, SettingsApplication);

    settings.beginGroup(SettingsGlobal);
        settings.remove(pRuntime);
    settings.endGroup();
}

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
