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

typedef void (*CellmlModelRuntimeInitConstsFunction)(double *, double *, double *);
typedef void (*CellmlModelRuntimeOdeRatesFunction)(double, double *, double *, double *, double *);
typedef void (*CellmlModelRuntimeDaeRatesFunction)(double, double *, double *, double *, double *, double *, double *, double *, double *);
typedef void (*CellmlModelRuntimeVariablesFunction)(double, double *, double *, double *, double *);
typedef void (*CellmlModelRuntimeDaeEssentialVariablesFunction)(double, double *, double *, double *, double *, double *, double *, double *);
typedef void (*CellmlModelRuntimeDaeRootInformationFunction)(double, double *, double *, double *, double *, double *, double *, double *);
typedef void (*CellmlModelRuntimeDaeStateInformationFunction)(double *);

//==============================================================================

struct CellmlModelRuntimeOdeFunctions
{
    CellmlModelRuntimeInitConstsFunction initConsts;
    CellmlModelRuntimeOdeRatesFunction rates;
    CellmlModelRuntimeVariablesFunction variables;
};

//==============================================================================

struct CellmlModelRuntimeDaeFunctions
{
    CellmlModelRuntimeInitConstsFunction initConsts;
    CellmlModelRuntimeDaeRatesFunction rates;
    CellmlModelRuntimeVariablesFunction variables;
    CellmlModelRuntimeDaeEssentialVariablesFunction essentialVariables;
    CellmlModelRuntimeDaeRootInformationFunction rootInformation;
    CellmlModelRuntimeDaeStateInformationFunction stateInformation;
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

    bool isValid();

    ModelType modelType();

    CellmlModelRuntimeOdeFunctions odeFunctions();
    CellmlModelRuntimeDaeFunctions daeFunctions();

    CellmlModelIssues issues();

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

    void reset();

    void couldNotGenerateModelCodeIssue();
    void unexpectedProblemDuringModelCompilationIssue();

    void checkCodeInformation(iface::cellml_services::CodeInformation *pCodeInformation);

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
