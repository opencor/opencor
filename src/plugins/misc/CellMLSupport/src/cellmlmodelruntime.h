//==============================================================================
// CellML model runtime class
//==============================================================================

#ifndef CELLMLMODELRUNTIME_H
#define CELLMLMODELRUNTIME_H

//==============================================================================

#include "cellmlmodelissue.h"
#include "cellmlsupportglobal.h"
#include "computerengine.h"

//==============================================================================

#include <QList>
#include <QObject>

//==============================================================================

#include "cellml-api-cxx-support.hpp"

#include "IfaceCCGS.hxx"
#include "IfaceCellML_APISPEC.hxx"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

typedef void (*CellmlModelRuntimeInitializeConstantsFunction)(double *, double *, double *);
typedef void (*CellmlModelRuntimeComputeOdeRatesFunction)(double, double *, double *, double *, double *);
typedef void (*CellmlModelRuntimeComputeDaeRatesFunction)(double, double *, double *, double *, double *, double *, double *, double *, double *);
typedef void (*CellmlModelRuntimeComputeVariablesFunction)(double, double *, double *, double *, double *);
typedef void (*CellmlModelRuntimeComputeDaeEssentialVariablesFunction)(double, double *, double *, double *, double *, double *, double *, double *);
typedef void (*CellmlModelRuntimeComputeDaeRootInformationFunction)(double, double *, double *, double *, double *, double *, double *, double *);
typedef void (*CellmlModelRuntimeComputeDaeStateInformationFunction)(double *);

//==============================================================================

struct CellmlModelRuntimeOdeFunctions
{
    CellmlModelRuntimeInitializeConstantsFunction initializeConstants;
    CellmlModelRuntimeComputeOdeRatesFunction computeRates;
    CellmlModelRuntimeComputeVariablesFunction computeVariables;
};

//==============================================================================

struct CellmlModelRuntimeDaeFunctions
{
    CellmlModelRuntimeInitializeConstantsFunction initializeConstants;
    CellmlModelRuntimeComputeDaeRatesFunction computeRates;
    CellmlModelRuntimeComputeVariablesFunction computeVariables;
    CellmlModelRuntimeComputeDaeEssentialVariablesFunction computeEssentialVariables;
    CellmlModelRuntimeComputeDaeRootInformationFunction computeRootInformation;
    CellmlModelRuntimeComputeDaeStateInformationFunction computeStateInformation;
};

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlModelRuntime : public QObject
{
    Q_OBJECT

public:
    enum ModelType
    {
        Ode,
        Dae,
        Undefined
    };

    explicit CellmlModelRuntime();

    bool isValid() const;

    ModelType modelType() const;

    int constantsCount() const;
    int statesCount() const;
    int ratesCount() const;
    int algebraicCount() const;
    int condVarCount() const;

    CellmlModelRuntimeOdeFunctions odeFunctions() const;
    CellmlModelRuntimeDaeFunctions daeFunctions() const;

    CellmlModelIssues issues() const;

    CellmlModelRuntime * update(iface::cellml_api::Model *pModel);

private:
    ModelType mModelType;

    ObjRef<iface::cellml_services::CodeInformation> mOdeCodeInformation;
    ObjRef<iface::cellml_services::IDACodeInformation> mDaeCodeInformation;

    Computer::ComputerEngine *mComputerEngine;

    CellmlModelRuntimeOdeFunctions mOdeFunctions;
    CellmlModelRuntimeDaeFunctions mDaeFunctions;

    CellmlModelIssues mIssues;

    void resetOdeCodeInformation();
    void resetDaeCodeInformation();

    void resetOdeFunctions();
    void resetDaeFunctions();

    void reset(const bool &pResetIssues = true);

    void couldNotGenerateModelCodeIssue();
    void unexpectedProblemDuringModelCompilationIssue();

    void checkCodeInformation(iface::cellml_services::CodeInformation *pCodeInformation);

    void customizeCodeGenerator(iface::cellml_services::CodeGenerator *pCodeGenerator);

    iface::cellml_services::CodeInformation * getOdeCodeInformation(iface::cellml_api::Model *pModel);
    iface::cellml_services::CodeInformation * getDaeCodeInformation(iface::cellml_api::Model *pModel);

    void handleErrors(const QString &pFunctionName);
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
