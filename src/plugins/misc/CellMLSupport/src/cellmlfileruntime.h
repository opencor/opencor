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
namespace Computer {
    class ComputerEngine;
}   // namespace Computer

namespace CellMLSupport {

//==============================================================================

typedef void (*CellmlFileRuntimeInitializeConstantsFunction)(double *, double *, double *);
typedef void (*CellmlFileRuntimeComputeOdeRatesFunction)(double, double *, double *, double *, double *);
typedef void (*CellmlFileRuntimeComputeDaeRatesFunction)(double, double *, double *, double *, double *, double *, double *, double *, double *);
typedef void (*CellmlFileRuntimeComputeVariablesFunction)(double, double *, double *, double *, double *);
typedef void (*CellmlFileRuntimeComputeDaeEssentialVariablesFunction)(double, double *, double *, double *, double *, double *, double *, double *);
typedef void (*CellmlFileRuntimeComputeDaeRootInformationFunction)(double, double *, double *, double *, double *, double *, double *, double *);
typedef void (*CellmlFileRuntimeComputeDaeStateInformationFunction)(double *);

//==============================================================================

struct CellmlFileRuntimeOdeFunctions
{
    CellmlFileRuntimeInitializeConstantsFunction initializeConstants;
    CellmlFileRuntimeComputeOdeRatesFunction computeRates;
    CellmlFileRuntimeComputeVariablesFunction computeVariables;
};

//==============================================================================

struct CellmlFileRuntimeDaeFunctions
{
    CellmlFileRuntimeInitializeConstantsFunction initializeConstants;
    CellmlFileRuntimeComputeDaeRatesFunction computeRates;
    CellmlFileRuntimeComputeVariablesFunction computeVariables;
    CellmlFileRuntimeComputeDaeEssentialVariablesFunction computeEssentialVariables;
    CellmlFileRuntimeComputeDaeRootInformationFunction computeRootInformation;
    CellmlFileRuntimeComputeDaeStateInformationFunction computeStateInformation;
};

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileRuntime : public QObject
{
    Q_OBJECT

public:
    enum ModelType
    {
        Ode,
        Dae,
        Undefined
    };

    explicit CellmlFileRuntime();

    bool isValid() const;

    ModelType modelType() const;

    int constantsCount() const;
    int statesCount() const;
    int ratesCount() const;
    int algebraicCount() const;
    int condVarCount() const;

    CellmlFileRuntimeOdeFunctions odeFunctions() const;
    CellmlFileRuntimeDaeFunctions daeFunctions() const;

    CellmlFileIssues issues() const;

    CellmlFileRuntime * update(iface::cellml_api::Model *pModel = 0);

private:
    ModelType mModelType;

    ObjRef<iface::cellml_services::CodeInformation> mOdeCodeInformation;
    ObjRef<iface::cellml_services::IDACodeInformation> mDaeCodeInformation;

    Computer::ComputerEngine *mComputerEngine;

    CellmlFileRuntimeOdeFunctions mOdeFunctions;
    CellmlFileRuntimeDaeFunctions mDaeFunctions;

    CellmlFileIssues mIssues;

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

    void checkFunction(const QString &pFunctionName);
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
