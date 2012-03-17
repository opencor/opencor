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

    typedef void (*InitializeConstantsFunction)(double *, double *, double *);
    typedef void (*ComputeOdeRatesFunction)(double, double *, double *, double *, double *);
    typedef void (*ComputeDaeResidualsFunction)(double, double *, double *, double *, double *, double *, double *, double *, double *);
    typedef void (*ComputeVariablesFunction)(double, double *, double *, double *, double *);
    typedef void (*ComputeDaeEssentialVariablesFunction)(double, double *, double *, double *, double *, double *, double *, double *);
    typedef void (*ComputeDaeRootInformationFunction)(double, double *, double *, double *, double *, double *, double *, double *);
    typedef void (*ComputeDaeStateInformationFunction)(double *);

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

    bool isValid() const;

    ModelType modelType() const;

    int constantsCount() const;
    int statesCount() const;
    int ratesCount() const;
    int algebraicCount() const;
    int condVarCount() const;

    OdeFunctions odeFunctions() const;
    DaeFunctions daeFunctions() const;

    CellmlFileIssues issues() const;

    CellmlFileRuntime * update(iface::cellml_api::Model *pModel = 0);

private:
    ModelType mModelType;

    ObjRef<iface::cellml_services::CodeInformation> mOdeCodeInformation;
    ObjRef<iface::cellml_services::IDACodeInformation> mDaeCodeInformation;

    Computer::ComputerEngine *mComputerEngine;

    OdeFunctions mOdeFunctions;
    DaeFunctions mDaeFunctions;

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
