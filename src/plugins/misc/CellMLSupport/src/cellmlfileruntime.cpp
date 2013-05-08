//==============================================================================
// CellML file runtime class
//==============================================================================

#include "cellmlfile.h"
#include "cellmlfileruntime.h"

//==============================================================================

#include <QRegularExpression>
#include <QStringList>

//==============================================================================

#include "CCGSBootstrap.hpp"
#include "CISBootstrap.hpp"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellMLFileRuntimeCompiledModelParameter::CellMLFileRuntimeCompiledModelParameter
(
 const ModelParameterType &pType,
 const int &pIndex
) :
    mType(pType),
    mIndex(pIndex)
{
}

void
CellMLFileRuntimeCompiledModelParameter::update
(
 const ModelParameterType &pType,
 const int &pIndex
)
{
    mType = pType;
    mIndex = pIndex;
}

//==============================================================================

CellMLFileRuntimeModelParameter::CellMLFileRuntimeModelParameter
(
 iface::cellml_api::CellMLVariable* pVariable,
 int pDegree
) :
    mVariable(pVariable), mDegree(pDegree)
{
}

QString CellMLFileRuntimeModelParameter::name() const
{
    return QString::fromStdWString(mVariable->name());
}

QString CellMLFileRuntimeModelParameter::unit() const
{
    return QString::fromStdWString(mVariable->unitsName());
}

// Retrieves the component in which the variable is declared.
// Note: If the component is imported, the name of the component given on the
//   uppermost import element is returned.
// Note: This is not guaranteed to be unique for the same component in a CellML
//   1.1 model, because you can have a case like this:
//    Model A: Component A
//             Import Component B from Model B as Component B.
//    Model B: Component A
//             Component B
//             Component A is encapsulated under Component B.
//    If Model A, Component A and Model B, Component B both have variables, this
//    function will call them both "A".
QString CellMLFileRuntimeModelParameter::component() const
{
    ObjRef<iface::cellml_api::CellMLComponent> component =
        QueryInterface(mVariable->parentElement());
    forever {
        ObjRef<iface::cellml_api::CellMLElement> model = component->parentElement();
        ObjRef<iface::cellml_api::CellMLImport> import =
            QueryInterface(model->parentElement());
        if (!import)
            return QString::fromStdWString(component->name());

        // The component has been imported, so retrieve the import's
        // components, and check which one has the name of variable's
        // component
        ObjRef<iface::cellml_api::ImportComponentSet> importComponents =
            import->components();
        ObjRef<iface::cellml_api::ImportComponentIterator> importComponentsIterator =
            importComponents->iterateImportComponents();
        ObjRef<iface::cellml_api::ImportComponent> importComponent;
        for (importComponent = importComponentsIterator->nextImportComponent();
             importComponent;
             importComponent = importComponentsIterator->nextImportComponent()) {
            if (importComponent->componentRef() == component->name())
                break;
        }
        if (!importComponent)
            // This means it was not directly imported (e.g. an encapsulation
            // parent was imported).
            return QString::fromStdWString(component->name());
        
        component = importComponent;
    }
}

void
CellMLFileRuntimeModelParameter::DAEData
(
 const CellMLFileRuntimeCompiledModelParameter::ModelParameterType &pType,
 const int &pIndex
)
{
    if (!mDAEData)
        mDAEData =
          QSharedPointer<CellMLFileRuntimeCompiledModelParameter>
          (new CellMLFileRuntimeCompiledModelParameter(pType, pIndex));
    else
        mDAEData->update(pType, pIndex);
}

void
CellMLFileRuntimeModelParameter::ODEData
(
 const CellMLFileRuntimeCompiledModelParameter::ModelParameterType &pType,
 const int &pIndex
)
{
    if (!mODEData)
        mODEData =
          QSharedPointer<CellMLFileRuntimeCompiledModelParameter>
          (new CellMLFileRuntimeCompiledModelParameter(pType, pIndex));
    else
        mODEData->update(pType, pIndex);
}

//==============================================================================

CellMLFileRuntime::CellMLFileRuntime() :
    mVariableOfIntegration(0),
    mModelParameters(CellMLFileRuntimeModelParameters())
{
    // Reset (initialise, here) our properties
    reset(true);
}

//==============================================================================

CellMLFileRuntime::~CellMLFileRuntime()
{
    // Reset our properties
    reset(false);
}

//==============================================================================

QString CellMLFileRuntime::address() const
{
    // Return our address as a string

    QString res;

    res.sprintf("%p", this);

    return res;
}

//==============================================================================

bool CellMLFileRuntime::isValid() const
{
    // The runtime is valid if no issues were found

    return mIssues.isEmpty();
}

//==============================================================================

//==============================================================================

CellMLFileIssues CellMLFileRuntime::issues() const
{
    // Return the issue(s)

    return mIssues;
}

//==============================================================================

CellMLFileRuntimeModelParameters CellMLFileRuntime::modelParameters() const
{
    // Return the model parameter(s)

    return mModelParameters;
}

//==============================================================================

void CellMLFileRuntime::resetODECodeInformation()
{
  mODEModel = NULL;
}

//==============================================================================

void CellMLFileRuntime::resetDAECodeInformation()
{
    mDAEModel = NULL;
}

//==============================================================================

void CellMLFileRuntime::reset(const bool &pResetIssues)
{
    // Reset all of the runtime's properties
    resetODECodeInformation();
    resetDAECodeInformation();

    if (pResetIssues)
        mIssues.clear();

    mVariableOfIntegration.clear();
    mModelParameters.clear();
}

//==============================================================================

void CellMLFileRuntime::couldNotGenerateModelCodeIssue()
{
    mIssues << CellMLFileIssue(CellMLFileIssue::Error,
                               tr("the model code could not be generated"));
}

//==============================================================================

void CellMLFileRuntime::unexpectedProblemDuringModelCompilationIssue()
{
    mIssues << CellMLFileIssue(CellMLFileIssue::Error,
                               tr("an unexpected problem occurred while trying to compile the model"));
}

//==============================================================================

void CellMLFileRuntime::ensureODECompiledModel(bool pDebug)
{
    if (mODEModel && pDebug == mODECompiledForDebug)
        return;

    ObjRef<iface::cellml_services::CellMLIntegrationService> intService =
        CreateIntegrationService();
    mODECompiledForDebug = pDebug;

    try {
        mODEModel = mODECompiledForDebug ?
            intService->compileDebugModelODE(mModel) :
            intService->compileModelODE(mModel);
    } catch (iface::cellml_api::CellMLException &) {
        mIssues << CellMLFileIssue(CellMLFileIssue::Error,
                                   QString::fromStdWString(intService->lastError()));
    } catch (...) {
        unexpectedProblemDuringModelCompilationIssue();
    }

    // If mIssues is non-empty, there was a problem generating / compiling code.
    if (!mIssues.empty())
        resetODECodeInformation();
}

//==============================================================================

void CellMLFileRuntime::ensureDAECompiledModel(bool pDebug)
{
    if (mODEModel && pDebug == mDAECompiledForDebug)
        return;

    ObjRef<iface::cellml_services::CellMLIntegrationService> intService =
        CreateIntegrationService();
    mODECompiledForDebug = pDebug;

    try {
        mDAEModel = mODECompiledForDebug ?
            intService->compileDebugModelDAE(mModel) :
            intService->compileModelDAE(mModel);
    } catch (iface::cellml_api::CellMLException &) {
        mIssues << CellMLFileIssue(CellMLFileIssue::Error,
                                   QString::fromStdWString(intService->lastError()));
    } catch (...) {
        unexpectedProblemDuringModelCompilationIssue();
    }

    // If mIssues is non-empty, there was a problem generating / compiling code.
    if (!mIssues.empty())
        resetDAECodeInformation();
}

//==============================================================================

QString CellMLFileRuntime::functionCode(const QString &pFunctionSignature,
                                        const QString &pFunctionBody,
                                        const bool &pHasDefines)
{
    QString res = pFunctionSignature+"\n"
                  "{\n";

    if (pFunctionBody.isEmpty()) {
        res += "    return 0;\n";
    } else {
        res += "    int ret = 0;\n"
                     "    int *pret = &ret;\n"
                     "\n";

        if (pHasDefines)
            res += "#define VOI 0.0\n"
                   "#define ALGEBRAIC 0\n"
                   "\n";

        res += pFunctionBody;

        if (!pFunctionBody.endsWith("\n"))
            res += "\n";

        if (pHasDefines)
            res += "\n"
                   "#undef ALGEBRAIC\n"
                   "#undef VOI\n";

        res += "\n"
               "    return ret;\n";
    }

    res += "}\n";

    return res;
}

//==============================================================================

bool sortModelParameters(QSharedPointer<CellMLFileRuntimeModelParameter> pModelParameter1,
                         QSharedPointer<CellMLFileRuntimeModelParameter> pModelParameter2)
{
    // Determine which of the two model parameters should be first
    // Note: the two comparisons which result we return are case insensitive,
    //       so that it's easier for people to search a model parameter...

    if (!pModelParameter1->component().compare(pModelParameter2->component())) {
        // The model parameters are in the same component, so check their
        // name

        if (!pModelParameter1->name().compare(pModelParameter2->name()))
            // The model parameters have the same name, so check their degree

            return pModelParameter1->degree() < pModelParameter2->degree();
        else
            // The model parameters have different names, so...

            return pModelParameter1->name().compare(pModelParameter2->name(), Qt::CaseInsensitive) < 0;
    } else {
        // The model parameters are in different components, so...

        return pModelParameter1->component().compare(pModelParameter2->component(), Qt::CaseInsensitive) < 0;
    }
}


    /*
        // Determine the type of the model parameter
        CellMLFileRuntimeCompiledModelParameter::ModelParameterType modelParameterType;

        switch (computationTarget->type()) {
        case iface::cellml_services::VARIABLE_OF_INTEGRATION:
            modelParameterType = CellMLFileRuntimeCompiledModelParameter::Voi;
            break;
        case iface::cellml_services::CONSTANT: {
            ObjRef<iface::cellml_api::CellMLVariable> variable = computationTarget->variable();
            if (variable->initialValue().empty())
                // The computed target doesn't have an initial value, so it must
                // be a 'computed' constant
                modelParameterType = CellMLFileRuntimeCompiledModelParameter::ComputedConstant;
            else
                // The computed target has an initial value, so it must be a
                // 'proper' constant
                modelParameterType = CellMLFileRuntimeCompiledModelParameter::Constant;
            break;
        }
        case iface::cellml_services::STATE_VARIABLE:
        case iface::cellml_services::PSEUDOSTATE_VARIABLE:
            modelParameterType = CellMLFileRuntimeCompiledModelParameter::State;
            break;
        case iface::cellml_services::ALGEBRAIC:
            // We are dealing with either a 'proper' algebraic variable or a
            // rate variable
            // Note: if the variable's degree is equal to zero, then we are
            //       dealing with a 'proper' algebraic variable otherwise we
            //       are dealing with a rate variable...
            if (computationTarget->degree())
                modelParameterType = CellMLFileRuntimeCompiledModelParameter::Rate;
            else
                modelParameterType = CellMLFileRuntimeCompiledModelParameter::Algebraic;
            break;
        default:
            // We are dealing with a type of computed target which is of no
            // interest to us, so...
            modelParameterType = CellMLFileRuntimeCompiledModelParameter::Undefined;
        }
     */

//==============================================================================

// Update a model.
// Note #1: we don't check whether a model is valid, since all we want is to
//          update its runtime (which has nothing to do with editing or even
//          validating a model), so if it can be done then great otherwise
//          tough luck (so to speak)...
CellMLFileRuntime * CellMLFileRuntime::update(CellMLFile *pCellMLFile)
{
    // Reset the runtime's properties
    reset(true);

    mModel = pCellMLFile->model();

    if (!mModel)
        return this;

    // We generate a code information solely to iterate the computation targets
    // in the model.
    ObjRef<iface::cellml_services::CodeGeneratorBootstrap> cgb(CreateCodeGeneratorBootstrap());
    ObjRef<iface::cellml_services::CodeGenerator> cg(cgb->createCodeGenerator());
    ObjRef<iface::cellml_services::CodeInformation> codeInfo(cg->generateCode(mModel));
    if (!codeInfo)
        return this;

    ObjRef<iface::cellml_services::ComputationTargetIterator> cti =
        codeInfo->iterateTargets();
    for (ObjRef<iface::cellml_services::ComputationTarget> computationTarget =
             cti->nextComputationTarget(); computationTarget;
         computationTarget = cti->nextComputationTarget()) {
        ObjRef<iface::cellml_api::CellMLVariable> variable = computationTarget->variable();

        // Keep track of the model parameter

        QSharedPointer<CellMLFileRuntimeModelParameter> modelParameter =
            QSharedPointer<CellMLFileRuntimeModelParameter>
            (new CellMLFileRuntimeModelParameter(variable, computationTarget->degree()));

        if (computationTarget->type() == iface::cellml_services::VARIABLE_OF_INTEGRATION)
            mVariableOfIntegration = modelParameter;
        else
            mModelParameters.append(modelParameter);
    }

    qSort(mModelParameters.begin(), mModelParameters.end(), sortModelParameters);

    // We are done, so return ourselves
    return this;
}

//==============================================================================

QSharedPointer<CellMLFileRuntimeModelParameter> CellMLFileRuntime::variableOfIntegration() const
{
    // Return our variable of integration, if any.
    return mVariableOfIntegration;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
