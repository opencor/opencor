/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Solver interface
//==============================================================================

#pragma once

//==============================================================================

#include "plugininfo.h"

//==============================================================================

#include <QVariant>

//==============================================================================

extern "C" void doNonLinearSolve(char *pRuntime,
                                 void (*pFunction)(double *, double *, void *),
                                 double *pParameters, int pSize,
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
    using Properties = QMap<QString, QVariant>;

    void setProperties(const Properties &pProperties);

    void emitError(const QString &pErrorMessage);

protected:
    Properties mProperties;

signals:
    void error(const QString &pErrorMessage);
};

//==============================================================================

class OdeSolver : public Solver
{
public:
    using ComputeRatesFunction = void (*)(double pVoi, double *pConstants, double *pRates, double *pStates, double *pAlgebraic);

    virtual void initialize(double pVoi, int pRatesStatesCount,
                            double *pConstants, double *pRates, double *pStates,
                            double *pAlgebraic,
                            ComputeRatesFunction pComputeRates);
    virtual void reinitialize(double pVoi);

    virtual void solve(double &pVoi, double pVoiEnd) const = 0;

protected:
    int mRatesStatesCount = 0;

    double *mConstants = nullptr;
    double *mStates = nullptr;
    double *mRates = nullptr;
    double *mAlgebraic = nullptr;

    ComputeRatesFunction mComputeRates = nullptr;
};

//==============================================================================

class NlaSolver : public Solver
{
public:
    ~NlaSolver() override;

    using ComputeSystemFunction = void (*)(double *, double *, void *);

    virtual void solve(ComputeSystemFunction pComputeSystem,
                       double *pParameters, int pSize,
                       void *pUserData = nullptr) = 0;
};

//==============================================================================

NlaSolver * nlaSolver(const QString &pRuntimeAddress);

void setNlaSolver(const QString &pRuntimeAddress, NlaSolver *pGlobalNlaSolver);
void unsetNlaSolver(const QString &pRuntimeAddress);

//==============================================================================

enum class Type {
    Nla,
    Ode
};

//==============================================================================

class Property
{
public:
    enum class Type {
        Boolean,
        Integer,
        IntegerGt0,
        IntegerGe0,
        Double,
        DoubleGe0,
        DoubleGt0,
        List
    };

    explicit Property(Type pType, const QString &pId,
                      const Descriptions &pDescriptions,
                      const QStringList &pListValues,
                      const QVariant &pDefaultValue, bool pHasVoiUnit);

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

using Properties = QList<Property>;

//==============================================================================

} // namespace Solver

//==============================================================================

extern "C" Q_DECL_EXPORT int solverInterfaceVersion();

//==============================================================================

class SolverInterface
{
public:
    virtual ~SolverInterface();

#define INTERFACE_DEFINITION
    #include "solverinterface.inl"
#undef INTERFACE_DEFINITION
};

//==============================================================================

using SolverInterfaces = QList<SolverInterface *>;

//==============================================================================

} // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::SolverInterface, "OpenCOR::SolverInterface")

//==============================================================================
// End of file
//==============================================================================
