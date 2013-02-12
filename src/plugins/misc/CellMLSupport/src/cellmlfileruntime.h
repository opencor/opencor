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

class CELLMLSUPPORT_EXPORT CellmlFileRuntimeModelParameter
{
public:
    enum ModelParameterType {
        Voi,
        Constant,
        ComputedConstant,
        State,
        Rate,
        Algebraic,
        Undefined
    };

    explicit CellmlFileRuntimeModelParameter(const QString &pName,
                                             const int &pDegree,
                                             const QString &pUnit,
                                             const QString &pComponent,
                                             const ModelParameterType &pType,
                                             const int &pIndex);

    QString name() const;
    int degree() const;
    QString unit() const;
    QString component() const;
    ModelParameterType type() const;
    int index() const;

private:
    QString mName;
    int mDegree;
    QString mUnit;
    QString mComponent;
    ModelParameterType mType;
    int mIndex;
};

//==============================================================================

typedef QList<CellmlFileRuntimeModelParameter *> CellmlFileRuntimeModelParameters;

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
    typedef int (*ComputeEssentialVariablesFunction)(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC, double *CONDVAR);
    typedef int (*ComputeRatesFunction)(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC);
    typedef int (*ComputeResidualsFunction)(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC, double *CONDVAR, double *resid);
    typedef int (*ComputeRootInformationFunction)(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC, double *CONDVAR);
    typedef int (*ComputeStateInformationFunction)(double *SI);
    typedef int (*ComputeVariablesFunction)(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC);

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
    ComputeEssentialVariablesFunction computeEssentialVariables() const;
    ComputeRatesFunction computeRates() const;
    ComputeResidualsFunction computeResiduals() const;
    ComputeRootInformationFunction computeRootInformation() const;
    ComputeStateInformationFunction computeStateInformation() const;
    ComputeVariablesFunction computeVariables() const;

    CellmlFileIssues issues() const;

    CellmlFileRuntimeModelParameters modelParameters() const;

    CellmlFileRuntime * update(OpenCOR::CellMLSupport::CellmlFile *pCellmlFile);

    CellmlFileRuntimeModelParameter * variableOfIntegration() const;

private:
    ModelType mModelType;
    bool mAtLeastOneNlaSystem;

    ObjRef<iface::cellml_services::CodeInformation> mCellmlApiOdeCodeInformation;
    ObjRef<iface::cellml_services::IDACodeInformation> mCellmlApiDaeCodeInformation;

    Compiler::CompilerEngine *mCompilerEngine;

    CellmlFileIssues mIssues;

    CellmlFileRuntimeModelParameter *mVariableOfIntegration;
    CellmlFileRuntimeModelParameters mModelParameters;

    InitializeConstantsFunction mInitializeConstants;

    ComputeComputedConstantsFunction mComputeComputedConstants;
    ComputeEssentialVariablesFunction mComputeEssentialVariables;
    ComputeRatesFunction mComputeRates;
    ComputeResidualsFunction mComputeResiduals;
    ComputeRootInformationFunction mComputeRootInformation;
    ComputeStateInformationFunction mComputeStateInformation;
    ComputeVariablesFunction mComputeVariables;

    void resetOdeCodeInformation();
    void resetDaeCodeInformation();

    void resetFunctions();

    void reset(const bool &pResetIssues = true);

    void couldNotGenerateModelCodeIssue();
    void unexpectedProblemDuringModelCompilationIssue();

    void checkCodeInformation(iface::cellml_services::CodeInformation *pCellmlApiCodeInformation);

    iface::cellml_services::CodeInformation * getOdeCodeInformation(iface::cellml_api::Model *pCellmlApiModel);
    iface::cellml_services::CodeInformation * getDaeCodeInformation(iface::cellml_api::Model *pCellmlApiModel);

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
