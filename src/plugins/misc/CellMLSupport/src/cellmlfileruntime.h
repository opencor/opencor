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
#include <QSharedPointer>

//==============================================================================

#include "cellml-api-cxx-support.hpp"

#include "IfaceCCGS.hxx"
#include "IfaceCIS.hxx"
#include "IfaceCellML_APISPEC.hxx"

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Compiler {
    class CompilerEngine;
}   // namespace Compiler

namespace CellMLSupport {

//==============================================================================

class CellMLFile;

//==============================================================================

class CELLMLSUPPORT_EXPORT CellMLFileRuntimeCompiledModelParameter : public QSharedData
{
public:
    enum ModelParameterType {
        Voi,
        Constant,
        ComputedConstant,
        Rate,
        State,
        Algebraic,
        Undefined
    };

    explicit CellMLFileRuntimeCompiledModelParameter
        (const ModelParameterType &pType, const int &pIndex);
    void update(const ModelParameterType &pType, const int &pIndex);

    ModelParameterType type() const { return mType; }
    int index() const { return mIndex; }

private:
    ModelParameterType mType;
    int mIndex;
};

class CELLMLSUPPORT_EXPORT CellMLFileRuntimeModelParameter : public QSharedData
{
public:
    explicit CellMLFileRuntimeModelParameter(iface::cellml_api::CellMLVariable* pVariable,
                                             int );
    void DAEData(const CellMLFileRuntimeCompiledModelParameter::ModelParameterType &pType, const int &pIndex);
    void ODEData(const CellMLFileRuntimeCompiledModelParameter::ModelParameterType &pType, const int &pIndex);
    QSharedPointer<CellMLFileRuntimeCompiledModelParameter> DAEData() {
        return mDAEData;
    }
    QSharedPointer<CellMLFileRuntimeCompiledModelParameter> ODEData() {
        return mODEData;
    }

    QString name() const;
    int degree() const { return mDegree; }
    QString unit() const;
    QString component() const;

    iface::cellml_api::CellMLVariable* variable() const { return mVariable; }

private:
    ObjRef<iface::cellml_api::CellMLVariable> mVariable;
    int mDegree;
    QSharedPointer<CellMLFileRuntimeCompiledModelParameter> mDAEData, mODEData;
};

//==============================================================================

typedef QList<QSharedPointer<CellMLFileRuntimeModelParameter> > CellMLFileRuntimeModelParameters;

//==============================================================================

class CELLMLSUPPORT_EXPORT CellMLFileRuntime : public QObject
{
    Q_OBJECT

public:
    explicit CellMLFileRuntime();
    ~CellMLFileRuntime();

    QString address() const;
    bool isValid() const;

    CellMLFileIssues issues() const;
    CellMLFileRuntimeModelParameters modelParameters() const;
    CellMLFileRuntime * update(CellMLFile *pCellMLFile);
    QSharedPointer<CellMLFileRuntimeModelParameter> variableOfIntegration() const;
    iface::cellml_services::ODESolverCompiledModel* odeCompiledModel() {
      return mODEModel;
    }
    iface::cellml_services::DAESolverCompiledModel* daeCompiledModel() {
      return mDAEModel;
    }

    void ensureODECompiledModel(bool pDebug = false);
    void ensureDAECompiledModel(bool pDebug = false);
private:
    ObjRef<iface::cellml_api::Model> mModel;
    bool mODECompiledForDebug; // Only valid if mODEModel != null
    ObjRef<iface::cellml_services::ODESolverCompiledModel> mODEModel;
    bool mDAECompiledForDebug; // Only valid if mDAEModel != null
    ObjRef<iface::cellml_services::DAESolverCompiledModel> mDAEModel;

    CellMLFileIssues mIssues;

    QSharedPointer<CellMLFileRuntimeModelParameter> mVariableOfIntegration;
    CellMLFileRuntimeModelParameters mModelParameters;

    void reset(const bool &pResetIssues);
    void resetODECodeInformation();
    void resetDAECodeInformation();

    void couldNotGenerateModelCodeIssue();
    void unexpectedProblemDuringModelCompilationIssue();

    void checkCodeInformation(iface::cellml_services::CodeInformation *pCodeInformation);

    QString functionCode(const QString &pFunctionSignature,
                         const QString &pFunctionBody,
                         const bool &pHasDefines = false);

    void compiledParamsFromCodeInformation(void (CellMLFileRuntimeModelParameter::*setter)(const CellMLFileRuntimeCompiledModelParameter::ModelParameterType &pType, const int &pIndex), iface::cellml_services::CodeInformation*);
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
