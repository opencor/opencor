//==============================================================================
// CellML file runtime class
//==============================================================================

#ifndef CELLMLFILERUNTIME_H
#define CELLMLFILERUNTIME_H

//==============================================================================

#include "cellmlfileissue.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QList>
#include <QObject>

//==============================================================================

#include "cellml-api-cxx-support.hpp"

#include "IfaceCCGS.hxx"
#include "IfaceCellML_APISPEC.hxx"

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Compiler {
    class CompilerEngine;
}   // namespace Compiler

namespace CellMLSupport {

//==============================================================================

class CellmlFile;

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileRuntimeParameter
{
public:
    enum ParameterType {
        Voi,
        Constant,
        ComputedConstant,
        Rate,
        State,
        Algebraic,
        Undefined
    };

    explicit CellmlFileRuntimeParameter(const QString &pName,
                                        const int &pDegree,
                                        const QString &pUnit,
                                        const QString &pComponent,
                                        const ParameterType &pType,
                                        const int &pIndex);

    QString name() const;
    int degree() const;
    QString unit() const;
    QString component() const;
    ParameterType type() const;
    int index() const;

    QString formattedName() const;
    QString fullyFormattedName() const;

    QString formattedUnit(const QString &pVoiUnit) const;

private:
    QString mName;
    int mDegree;
    QString mUnit;
    QString mComponent;
    ParameterType mType;
    int mIndex;
};

//==============================================================================

typedef QList<CellmlFileRuntimeParameter *> CellmlFileRuntimeParameters;

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileRuntime : public QObject
{
    Q_OBJECT

public:
    enum ModelType {
        Ode,
        Dae,
        Undefined
    };

    typedef int (*InitializeConstantsFunction)(double *CONSTANTS, double *RATES, double *STATES);

    typedef int (*ComputeComputedConstantsFunction)(double *CONSTANTS, double *RATES, double *STATES);

    typedef int (*ComputeOdeRatesFunction)(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC);
    typedef int (*ComputeOdeVariablesFunction)(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC);

    typedef int (*ComputeDaeEssentialVariablesFunction)(double VOI, double *CONSTANTS, double *RATES, double *OLDRATES, double *STATES, double *OLDSTATES, double *ALGEBRAIC, double *CONDVAR);
    typedef int (*ComputeDaeResidualsFunction)(double VOI, double *CONSTANTS, double *RATES, double *OLDRATES, double *STATES, double *OLDSTATES, double *ALGEBRAIC, double *CONDVAR, double *resid);
    typedef int (*ComputeDaeRootInformationFunction)(double VOI, double *CONSTANTS, double *RATES, double *OLDRATES, double *STATES, double *OLDSTATES, double *ALGEBRAIC, double *CONDVAR);
    typedef int (*ComputeDaeStateInformationFunction)(double *SI);
    typedef int (*ComputeDaeVariablesFunction)(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC, double *CONDVAR);

    explicit CellmlFileRuntime();
    ~CellmlFileRuntime();

    QString address() const;

    bool isValid() const;

    ModelType modelType() const;

    bool needOdeSolver() const;
    bool needDaeSolver() const;
    bool needNlaSolver() const;

    int constantsCount() const;
    int statesCount() const;
    int ratesCount() const;
    int algebraicCount() const;
    int condVarCount() const;

    InitializeConstantsFunction initializeConstants() const;

    ComputeComputedConstantsFunction computeComputedConstants() const;

    ComputeOdeRatesFunction computeOdeRates() const;
    ComputeOdeVariablesFunction computeOdeVariables() const;

    ComputeDaeEssentialVariablesFunction computeDaeEssentialVariables() const;
    ComputeDaeResidualsFunction computeDaeResiduals() const;
    ComputeDaeRootInformationFunction computeDaeRootInformation() const;
    ComputeDaeStateInformationFunction computeDaeStateInformation() const;
    ComputeDaeVariablesFunction computeDaeVariables() const;

    CellmlFileIssues issues() const;

    CellmlFileRuntimeParameters parameters() const;

    CellmlFileRuntime * update(CellmlFile *pCellmlFile);

    CellmlFileRuntimeParameter * variableOfIntegration() const;

private:
    ModelType mModelType;
    bool mAtLeastOneNlaSystem;

    ObjRef<iface::cellml_services::CodeInformation> mOdeCodeInformation;
    ObjRef<iface::cellml_services::IDACodeInformation> mDaeCodeInformation;

    Compiler::CompilerEngine *mCompilerEngine;

    CellmlFileIssues mIssues;

    CellmlFileRuntimeParameter *mVariableOfIntegration;
    CellmlFileRuntimeParameters mParameters;

    InitializeConstantsFunction mInitializeConstants;

    ComputeComputedConstantsFunction mComputeComputedConstants;

    ComputeOdeRatesFunction mComputeOdeRates;
    ComputeOdeVariablesFunction mComputeOdeVariables;

    ComputeDaeEssentialVariablesFunction mComputeDaeEssentialVariables;
    ComputeDaeResidualsFunction mComputeDaeResiduals;
    ComputeDaeRootInformationFunction mComputeDaeRootInformation;
    ComputeDaeStateInformationFunction mComputeDaeStateInformation;
    ComputeDaeVariablesFunction mComputeDaeVariables;

    void resetOdeCodeInformation();
    void resetDaeCodeInformation();

    void resetFunctions();

    void reset(const bool &pRecreateCompilerEngine, const bool &pResetIssues);

    void couldNotGenerateModelCodeIssue();
    void unexpectedProblemDuringModelCompilationIssue();

    void checkCodeInformation(iface::cellml_services::CodeInformation *pCodeInformation);

    void getOdeCodeInformation(iface::cellml_api::Model *pModel);
    void getDaeCodeInformation(iface::cellml_api::Model *pModel);

    QString functionCode(const QString &pFunctionSignature,
                         const QString &pFunctionBody,
                         const bool &pHasDefines = false);
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
