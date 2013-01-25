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
        State,
        Algebraic,
        Undefined
    };

    explicit CellmlFileRuntimeModelParameter(const QString &pName,
                                             const QString &pUnit,
                                             const QString &pComponent,
                                             const ModelParameterType &pType,
                                             const int &pIndex);

    QString name() const;
    QString unit() const;
    QString component() const;
    ModelParameterType type() const;
    int index() const;

private:
    QString mName;
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

    typedef int (*InitializeConstantsFunction)(double *, double *, double *);
    typedef int (*ComputeOdeRatesFunction)(double, double *, double *, double *, double *);
    typedef int (*ComputeDaeResidualsFunction)(double, double *, double *, double *, double *, double *, double *);
    typedef int (*ComputeVariablesFunction)(double, double *, double *, double *, double *);
    typedef int (*ComputeDaeEssentialVariablesFunction)(double, double *, double *, double *, double *, double *);
    typedef int (*ComputeDaeRootInformationFunction)(double, double *, double *, double *, double *, double *);
    typedef int (*ComputeDaeStateInformationFunction)(double *);

    struct OdeFunctions
    {
        InitializeConstantsFunction initializeConstants;
        ComputeOdeRatesFunction computeRates;
        ComputeVariablesFunction computeVariables;
    };

    struct DaeFunctions
    {
        InitializeConstantsFunction initializeConstants;
        ComputeDaeResidualsFunction computeResiduals;
        ComputeVariablesFunction computeVariables;
        ComputeDaeEssentialVariablesFunction computeEssentialVariables;
        ComputeDaeRootInformationFunction computeRootInformation;
        ComputeDaeStateInformationFunction computeStateInformation;
    };

    explicit CellmlFileRuntime();
    ~CellmlFileRuntime();

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

    OdeFunctions odeFunctions() const;
    DaeFunctions daeFunctions() const;

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

    OdeFunctions mOdeFunctions;
    DaeFunctions mDaeFunctions;

    CellmlFileIssues mIssues;

    CellmlFileRuntimeModelParameter *mVariableOfIntegration;
    CellmlFileRuntimeModelParameters mModelParameters;

    void resetOdeCodeInformation();
    void resetDaeCodeInformation();

    void resetOdeFunctions();
    void resetDaeFunctions();

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
