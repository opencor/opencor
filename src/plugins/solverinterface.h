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

#pragma once

//==============================================================================

#include "plugininfo.h"

//==============================================================================

#include <QList>
#include <QVariant>

//==============================================================================

extern "C" void doNonLinearSolve(char *pRuntime,
                                 void (*pFunction)(double *, double *, void *),
                                 double *pParameters, int *pRes, int pSize,
                                 void *pUserData);

//==============================================================================

namespace OpenCOR {
namespace Solver {

//==============================================================================

enum {
    SizeOfDouble = sizeof(double)
};

//==============================================================================

class Solver : public QObject
{
    Q_OBJECT

public:
    typedef QMap<QString, QVariant> Properties;

    explicit Solver();

    void setProperties(const Properties &pProperties);

    void emitError(const QString &pErrorMessage);

protected:
    Properties mProperties;

Q_SIGNALS:
    void error(const QString &pErrorMessage);
};

//==============================================================================

class VoiSolver : public Solver
{
public:
    explicit VoiSolver();

    virtual void solve(double &pVoi, const double &pVoiEnd) const = 0;

protected:
    int mRatesStatesCount;

    double *mConstants;
    double *mStates;
    double *mRates;
    double *mAlgebraic;
};

//==============================================================================

class OdeSolver : public VoiSolver
{
public:
    typedef int (*ComputeRatesFunction)(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC);

    explicit OdeSolver();

    virtual void initialize(const double &pVoiStart,
                            const int &pRatesStatesCount, double *pConstants,
                            double *pRates, double *pStates, double *pAlgebraic,
                            ComputeRatesFunction pComputeRates);

protected:
    ComputeRatesFunction mComputeRates;
};

//==============================================================================

class DaeSolver : public VoiSolver
{
public:
    typedef int (*ComputeEssentialVariablesFunction)(double VOI, double *CONSTANTS, double *RATES, double *OLDRATES, double *STATES, double *OLDSTATES, double *ALGEBRAIC, double *CONDVAR);
    typedef int (*ComputeResidualsFunction)(double VOI, double *CONSTANTS, double *RATES, double *OLDRATES, double *STATES, double *OLDSTATES, double *ALGEBRAIC, double *CONDVAR, double *resid);
    typedef int (*ComputeRootInformationFunction)(double VOI, double *CONSTANTS, double *RATES, double *OLDRATES, double *STATES, double *OLDSTATES, double *ALGEBRAIC, double *CONDVAR);
    typedef int (*ComputeStateInformationFunction)(double *SI);

    explicit DaeSolver();
    ~DaeSolver();

    virtual void initialize(const double &pVoiStart, const double &pVoiEnd,
                            const int &pRatesStatesCount,
                            const int &pCondVarCount, double *pConstants,
                            double *pRates, double *pStates, double *pAlgebraic,
                            double *pCondVar,
                            ComputeEssentialVariablesFunction pComputeEssentialVariables,
                            ComputeResidualsFunction pComputeResiduals,
                            ComputeRootInformationFunction pComputeRootInformation,
                            ComputeStateInformationFunction pComputeStateInformation);

protected:
    int mCondVarCount;

    double *mOldRates;
    double *mOldStates;
    double *mCondVar;
};

//==============================================================================

class NlaSolver : public Solver
{
public:
    typedef void (*ComputeSystemFunction)(double *, double *, void *);

    explicit NlaSolver();

    virtual void initialize(ComputeSystemFunction pComputeSystem,
                            double *pParameters, int pSize,
                            void *pUserData = 0);

    virtual void solve() const = 0;

private:
    ComputeSystemFunction mComputeSystem;

    double *mParameters;
    int mSize;
    void *mUserData;
};

//==============================================================================

NlaSolver * nlaSolver(const QString &pRuntimeAddress);

void setNlaSolver(const QString &pRuntimeAddress, NlaSolver *pGlobalNlaSolver);
void unsetNlaSolver(const QString &pRuntimeAddress);

//==============================================================================

enum Type {
    Dae,
    Nla,
    Ode
};

//==============================================================================

class Property
{
public:
    enum Type {
        Boolean,
        Integer,
        Double,
        List
    };

    explicit Property(const Type &pType, const QString &pId,
                      const Descriptions &pDescriptions,
                      const QStringList &pListValues,
                      const QVariant &pDefaultValue,
                      const bool &pHasVoiUnit);

    Type type() const;
    QString id() const;
    Descriptions descriptions() const;
    QStringList listValues() const;
    QVariant defaultValue() const;
    bool hasVoiUnit() const;

private:
    Type mType;
    QString mId;
    Descriptions mDescriptions;
    QStringList mListValues;
    QVariant mDefaultValue;
    bool mHasVoiUnit;
};

//==============================================================================

typedef QList<Property> Properties;

//==============================================================================

}   // namespace Solver

//==============================================================================

class SolverInterface
{
public:
#define INTERFACE_DEFINITION
    #include "solverinterface.inl"
#undef INTERFACE_DEFINITION
};

//==============================================================================

typedef QList<SolverInterface *> SolverInterfaces;

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::SolverInterface, "OpenCOR::SolverInterface")

//==============================================================================
// End of file
//==============================================================================
