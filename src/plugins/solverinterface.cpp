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
// Solver interface
//==============================================================================

#include "solverinterface.h"

//==============================================================================

#include <QCoreApplication>

//==============================================================================

void doNonLinearSolve(char *pRuntime,
                      void (*pFunction)(double *, double *, void *),
                      double *pParameters, int *pRes, int pSize,
                      void *pUserData)
{
    // Retrieve the NLA solver which we should use

    OpenCOR::Solver::NlaSolver *nlaSolver = OpenCOR::Solver::nlaSolver(pRuntime);

    if (nlaSolver) {
        // We have found our NLA solver, so initialise it

        nlaSolver->initialize(pFunction, pParameters, pSize, pUserData);

        // Now, we can solve our NLA system

        nlaSolver->solve();

        *pRes = 1;
    } else {
        // We couldn't retrieve an NLA solver...
        // Note: this should never happen, but we never know...

        qWarning("WARNING | %s:%d: no NLA solver could be found.", __FILE__, __LINE__);

        *pRes = 0;
    }
}

//==============================================================================

namespace OpenCOR {

//==============================================================================

extern "C" Q_DECL_EXPORT int solverInterfaceVersion()
{
    // Version of the solver interface

    return 1;
}

//==============================================================================

namespace Solver {

//==============================================================================

Solver::Solver() :
    mProperties(Properties())
{
}

//==============================================================================

void Solver::setProperties(const Properties &pProperties)
{
    // Set a property's value, but only if it is a valid property

    mProperties = pProperties;
}

//==============================================================================

void Solver::emitError(const QString &pErrorMessage)
{
    // Let people know that an error occured, but first reformat the error a
    // bit, if needed

    QString errorMessage = QString();

    if (pErrorMessage.startsWith("Newton"))
        errorMessage = pErrorMessage;
    else if (!pErrorMessage.isEmpty())
        errorMessage = pErrorMessage[0].toLower()+pErrorMessage.right(pErrorMessage.size()-1);

    if (!errorMessage.right(3).compare("..."))
        emit error(errorMessage.left(errorMessage.size()-3));
    else if (!errorMessage.right(1).compare("."))
        emit error(errorMessage.left(errorMessage.size()-1));
    else
        emit error(errorMessage);
}

//==============================================================================

VoiSolver::VoiSolver() :
    Solver(),
    mRatesStatesCount(0),
    mConstants(0),
    mStates(0),
    mRates(0),
    mAlgebraic(0)
{
}

//==============================================================================

OdeSolver::OdeSolver() :
    VoiSolver(),
    mComputeRates(0)
{
}

//==============================================================================

void OdeSolver::initialize(const double &pVoiStart,
                           const int &pRatesStatesCount, double *pConstants,
                           double *pRates, double *pStates, double *pAlgebraic,
                           ComputeRatesFunction pComputeRates)
{
    Q_UNUSED(pVoiStart);

    // Initialise the ODE solver

    mRatesStatesCount = pRatesStatesCount;

    mConstants = pConstants;
    mRates = pRates;
    mStates = pStates;
    mAlgebraic = pAlgebraic;

    mComputeRates = pComputeRates;
}

//==============================================================================

DaeSolver::DaeSolver() :
    VoiSolver(),
    mCondVarCount(0),
    mOldRates(0),
    mOldStates(0),
    mCondVar(0)
{
}

//==============================================================================

DaeSolver::~DaeSolver()
{
    // Delete some internal objects

    delete[] mOldRates;
    delete[] mOldStates;
}

//==============================================================================

void DaeSolver::initialize(const double &pVoiStart, const double &pVoiEnd,
                           const int &pRatesStatesCount,
                           const int &pCondVarCount, double *pConstants,
                           double *pRates, double *pStates, double *pAlgebraic,
                           double *pCondVar,
                           ComputeEssentialVariablesFunction pComputeEssentialVariables,
                           ComputeResidualsFunction pComputeResiduals,
                           ComputeRootInformationFunction pComputeRootInformation,
                           ComputeStateInformationFunction pComputeStateInformation)
{
    Q_UNUSED(pVoiStart);
    Q_UNUSED(pVoiEnd);
    Q_UNUSED(pComputeEssentialVariables);
    Q_UNUSED(pComputeResiduals);
    Q_UNUSED(pComputeRootInformation);
    Q_UNUSED(pComputeStateInformation);

    // Initialise the DAE solver

    mRatesStatesCount = pRatesStatesCount;
    mCondVarCount = pCondVarCount;

    mConstants = pConstants;
    mRates = pRates;
    mStates = pStates;
    mAlgebraic = pAlgebraic;
    mCondVar = pCondVar;

    delete[] mOldRates;
    delete[] mOldStates;

    mOldRates = new double[pRatesStatesCount];
    mOldStates = new double[pRatesStatesCount];

    memcpy(mOldRates, pRates, pRatesStatesCount*SizeOfDouble);
    memcpy(mOldStates, pStates, pRatesStatesCount*SizeOfDouble);
}

//==============================================================================

NlaSolver::NlaSolver() :
    mComputeSystem(0),
    mParameters(0),
    mSize(0),
    mUserData(0)
{
}

//==============================================================================

void NlaSolver::initialize(ComputeSystemFunction pComputeSystem,
                           double *pParameters, int pSize, void *pUserData)
{
    // Initialise ourselves

    mComputeSystem = pComputeSystem;

    mParameters = pParameters;
    mSize = pSize;
    mUserData = pUserData;
}

//==============================================================================

NlaSolver * nlaSolver(const QString &pRuntimeAddress)
{
    // Return the runtime's NLA solver

    QVariant res = qApp->property(pRuntimeAddress.toUtf8().constData());

    return res.isValid()?static_cast<NlaSolver *>((void *) res.toULongLong()):0;
}

//==============================================================================

void setNlaSolver(const QString &pRuntimeAddress, NlaSolver *pGlobalNlaSolver)
{
    // Keep track of the runtime's NLA solver

    qApp->setProperty(pRuntimeAddress.toUtf8().constData(),
                      qulonglong(pGlobalNlaSolver));
}

//==============================================================================

void unsetNlaSolver(const QString &pRuntimeAddress)
{
    // Stop tracking the runtime's NLA solver

    qApp->setProperty(pRuntimeAddress.toUtf8().constData(), QVariant::Invalid);
}

//==============================================================================

Property::Property(const Property::Type &pType, const QString &pId,
                   const Descriptions &pDescriptions,
                   const QStringList &pListValues,
                   const QVariant &pDefaultValue,
                   const bool &pHasVoiUnit) :
    mType(pType),
    mId(pId),
    mDescriptions(pDescriptions),
    mListValues(pListValues),
    mDefaultValue(pDefaultValue),
    mHasVoiUnit(pHasVoiUnit)
{
}

//==============================================================================

Property::Type Property::type() const
{
    // Return our type

    return mType;
}

//==============================================================================

QString Property::id() const
{
    // Return our id

    return mId;
}

//==============================================================================

Descriptions Property::descriptions() const
{
    // Return our descriptions

    return mDescriptions;
}

//==============================================================================

QStringList Property::listValues() const
{
    // Return our list values

    return mListValues;
}

//==============================================================================

QVariant Property::defaultValue() const
{
    // Return our default value

    return mDefaultValue;
}

//==============================================================================

bool Property::hasVoiUnit() const
{
    // Return whether we have a VOI unit

    return mHasVoiUnit;
}

//==============================================================================

}   // namespace Solver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
