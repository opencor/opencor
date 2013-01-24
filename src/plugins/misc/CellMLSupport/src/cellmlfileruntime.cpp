//==============================================================================
// CellML file runtime class
//==============================================================================

#include "cellmlfile.h"
#include "cellmlfileruntime.h"
#include "compilerengine.h"
#include "compilermath.h"

//==============================================================================

#include <QRegularExpression>
#include <QTime>

//==============================================================================

#include "CCGSBootstrap.hpp"

//==============================================================================

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/DynamicLibrary.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileRuntimeModelParameter::CellmlFileRuntimeModelParameter(CellmlFileVariable *pVariable,
                                                                 const ModelParameterType &pType,
                                                                 const int &pIndex) :
    mVariable(pVariable),
    mType(pType),
    mIndex(pIndex)
{
}

//==============================================================================

CellmlFileVariable * CellmlFileRuntimeModelParameter::variable() const
{
    // Return our variable

    return mVariable;
}

//==============================================================================

CellmlFileRuntimeModelParameter::ModelParameterType CellmlFileRuntimeModelParameter::type() const
{
    // Return our type

    return mType;
}

//==============================================================================

int CellmlFileRuntimeModelParameter::index() const
{
    // Return our index

    return mIndex;
}

//==============================================================================

CellmlFileRuntime::CellmlFileRuntime() :
    mCellmlApiOdeCodeInformation(0),
    mCellmlApiDaeCodeInformation(0),
    mVariableOfIntegration(0),
    mCompilerEngine(0),
    mModelParameters(CellmlFileRuntimeModelParameters())
{
    // Reset (initialise, here) our properties

    reset();
}

//==============================================================================

CellmlFileRuntime::~CellmlFileRuntime()
{
    // Reset our properties

    reset();
}

//==============================================================================

bool CellmlFileRuntime::isValid() const
{
    // The runtime is valid if no issues were found

    return mIssues.isEmpty();
}

//==============================================================================

CellmlFileRuntime::ModelType CellmlFileRuntime::modelType() const
{
    // Return the type of model the runtime is for

    return mModelType;
}

//==============================================================================

bool CellmlFileRuntime::needNlaSolver() const
{
    // Return whether the model needs a non-linear algebraic solver

    return mAtLeastOneNlaSystem;
}

//==============================================================================

int CellmlFileRuntime::constantsCount() const
{
    // Return the number of constants in the model

    if (mModelType == Ode)
        return mCellmlApiOdeCodeInformation?mCellmlApiOdeCodeInformation->constantIndexCount():0;
    else
        return mCellmlApiDaeCodeInformation?mCellmlApiDaeCodeInformation->constantIndexCount():0;
}

//==============================================================================

int CellmlFileRuntime::statesCount() const
{
    // Return the number of states in the model

    if (mModelType == Ode)
        return mCellmlApiOdeCodeInformation?mCellmlApiOdeCodeInformation->rateIndexCount():0;
    else
        return mCellmlApiDaeCodeInformation?mCellmlApiDaeCodeInformation->rateIndexCount():0;
}

//==============================================================================

int CellmlFileRuntime::ratesCount() const
{
    // Return the number of rates in the model
    // Note: it is obviously the same as the number of states, so this function
    //       is only for user convenience...

    return statesCount();
}

//==============================================================================

int CellmlFileRuntime::algebraicCount() const
{
    // Return the number of algebraic equations in the model

    if (mModelType == Ode)
        return mCellmlApiOdeCodeInformation?mCellmlApiOdeCodeInformation->algebraicIndexCount():0;
    else
        return mCellmlApiDaeCodeInformation?mCellmlApiDaeCodeInformation->algebraicIndexCount():0;
}

//==============================================================================

int CellmlFileRuntime::condVarCount() const
{
    // Return the number of conditional variables in the model

    if (mModelType == Ode)
        return 0;
    else
        return mCellmlApiDaeCodeInformation?mCellmlApiDaeCodeInformation->conditionVariableCount():0;
}

//==============================================================================

CellmlFileRuntime::OdeFunctions CellmlFileRuntime::odeFunctions() const
{
    // Return the ODE functions

    return mOdeFunctions;
}

//==============================================================================

CellmlFileRuntime::DaeFunctions CellmlFileRuntime::daeFunctions() const
{
    // Return the DAE functions

    return mDaeFunctions;
}

//==============================================================================

CellmlFileIssues CellmlFileRuntime::issues() const
{
    // Return the issue(s)

    return mIssues;
}

//==============================================================================

void CellmlFileRuntime::resetOdeCodeInformation()
{
    // Reset the ODE code information
    // Note: setting it to zero will automatically delete the current instance,
    //       if any

    mCellmlApiOdeCodeInformation = 0;
}

//==============================================================================

void CellmlFileRuntime::resetDaeCodeInformation()
{
    // Reset the DAE code information
    // Note: setting it to zero will automatically delete the current instance,
    //       if any

    mCellmlApiDaeCodeInformation = 0;
}

//==============================================================================

void CellmlFileRuntime::resetOdeFunctions()
{
    // Reset the ODE functions

    mOdeFunctions.initializeConstants = 0;
    mOdeFunctions.computeRates        = 0;
    mOdeFunctions.computeVariables    = 0;
}

//==============================================================================

void CellmlFileRuntime::resetDaeFunctions()
{
    // Reset the DEA functions

    mDaeFunctions.initializeConstants       = 0;
    mDaeFunctions.computeResiduals          = 0;
    mDaeFunctions.computeVariables          = 0;
    mDaeFunctions.computeEssentialVariables = 0;
    mDaeFunctions.computeRootInformation    = 0;
    mDaeFunctions.computeStateInformation   = 0;
}

//==============================================================================

void CellmlFileRuntime::reset(const bool &pResetIssues)
{
    // Reset all of the runtime's properties

    mModelType = Undefined;
    mAtLeastOneNlaSystem = false;

    resetOdeCodeInformation();
    resetDaeCodeInformation();

    mVariableOfIntegration = 0;

    delete mCompilerEngine;

    mCompilerEngine = new Compiler::CompilerEngine();

    resetOdeFunctions();
    resetDaeFunctions();

    if (pResetIssues)
        mIssues.clear();

    foreach (CellmlFileRuntimeModelParameter *modelParameter, mModelParameters)
        delete modelParameter;

    mModelParameters.clear();
}

//==============================================================================

void CellmlFileRuntime::couldNotGenerateModelCodeIssue()
{
    mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                               tr("the model code could not be generated"));
}

//==============================================================================

void CellmlFileRuntime::unexpectedProblemDuringModelCompilationIssue()
{
    mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                               tr("an unexpected problem occurred while trying to compile the model"));
}

//==============================================================================

void CellmlFileRuntime::checkCodeInformation(iface::cellml_services::CodeInformation *pCellmlApiCodeInformation)
{
    if (!pCellmlApiCodeInformation)
        // No code information was provided, so...

        return;

    // Retrieve the code information's latest error message

    QString codeGenerationErrorMessage = QString::fromStdWString(pCellmlApiCodeInformation->errorMessage());

    if (codeGenerationErrorMessage.isEmpty()) {
        // The code generation went fine, so check the model's constraint level

        iface::cellml_services::ModelConstraintLevel constraintLevel = pCellmlApiCodeInformation->constraintLevel();

        if (constraintLevel == iface::cellml_services::UNDERCONSTRAINED) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       tr("the model is underconstrained (i.e. some variables need to be initialised or computed)"));
        } else if (constraintLevel == iface::cellml_services::UNSUITABLY_CONSTRAINED) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       tr("the model is unsuitably constrained (i.e. some variables could not be found and/or some equations could not be used)"));
        } else if (constraintLevel == iface::cellml_services::OVERCONSTRAINED) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       tr("the model is overconstrained (i.e. some variables are either both initialised and computed or computed more than once)"));
        }
    } else {
        // The code generation didn't work, so...

        mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                   tr("a problem occurred during the compilation of the model"));
    }
}

//==============================================================================

iface::cellml_services::CodeInformation * CellmlFileRuntime::getOdeCodeInformation(iface::cellml_api::Model *pCellmlApiModel)
{
    // Get a code generator bootstrap and create an ODE code generator

    ObjRef<iface::cellml_services::CodeGeneratorBootstrap> codeGeneratorBootstrap = CreateCodeGeneratorBootstrap();
    ObjRef<iface::cellml_services::CodeGenerator> codeGenerator = codeGeneratorBootstrap->createCodeGenerator();

    // Generate some code for the model (i.e. 'compile' the model)

    try {
        mCellmlApiOdeCodeInformation = codeGenerator->generateCode(pCellmlApiModel);

        // Check that the code generation went fine

        checkCodeInformation(mCellmlApiOdeCodeInformation);
    } catch (iface::cellml_api::CellMLException &) {
        couldNotGenerateModelCodeIssue();
    } catch (...) {
        unexpectedProblemDuringModelCompilationIssue();
    }

    // Check the outcome of the ODE code generation

    if (mIssues.count())
        // Something went wrong at some point, so...

        resetOdeCodeInformation();

    // We are done, so return our code information

    return mCellmlApiOdeCodeInformation;
}

//==============================================================================

iface::cellml_services::CodeInformation * CellmlFileRuntime::getDaeCodeInformation(iface::cellml_api::Model *pCellmlApiModel)
{
    // Get a code generator bootstrap and create a DAE code generator

    ObjRef<iface::cellml_services::CodeGeneratorBootstrap> codeGeneratorBootstrap = CreateCodeGeneratorBootstrap();
    ObjRef<iface::cellml_services::IDACodeGenerator> codeGenerator = codeGeneratorBootstrap->createIDACodeGenerator();

    // Generate some code for the model (i.e. 'compile' the model)

    try {
        mCellmlApiDaeCodeInformation = codeGenerator->generateIDACode(pCellmlApiModel);

        // Check that the code generation went fine

        checkCodeInformation(mCellmlApiDaeCodeInformation);
    } catch (iface::cellml_api::CellMLException &) {
        couldNotGenerateModelCodeIssue();
    } catch (...) {
        unexpectedProblemDuringModelCompilationIssue();
    }

    // Check the outcome of the DAE code generation

    if (mIssues.count())
        // Something went wrong at some point, so...

        resetDaeCodeInformation();

    // We are done, so return our code information

    return mCellmlApiDaeCodeInformation;
}

//==============================================================================

QString CellmlFileRuntime::functionCode(const QString &pFunctionSignature,
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

CellmlFileRuntime * CellmlFileRuntime::update(CellmlFile *pCellmlFile)
{
    // Reset the runtime's properties

    reset();

    // Check that the model is either a 'simple' ODE model or a DAE model
    // Note #1: we don't check whether a model is valid, since all we want is to
    //          update its runtime (which has nothing to do with editing or even
    //          validating a model), so if it can be done then great otherwise
    //          tough luck (so to speak)...
    // Note #2: in order to do so, we need to get a 'normal' code generator (as
    //          opposed to an IDA, i.e. DAE, code generator) since if the model
    //          is correctly constrained, then we can check whether some of its
    //          equations were flagged as needing a Newton-Raphson evaluation,
    //          in which case we would be dealing with a DAE model...
    // Note #3: ideally, there would be a more convenient way to determine the
    //          type of a model, but well... there isn't, so...

    iface::cellml_api::Model *cellmlApiModel = pCellmlFile->cellmlApiModel();

    if (!cellmlApiModel)
        // No model was provided, so...

        return this;

    // Retrieve the model's type
    // Note: this can be done by checking whether some equations were flagged
    //       as needing a Newton-Raphson evaluation...

#ifdef QT_DEBUG
    QTime time;

    time.start();
#endif

    getOdeCodeInformation(cellmlApiModel);

#ifdef QT_DEBUG
    qDebug(" - CellML ODE code information time: %s s", qPrintable(QString::number(0.001*time.elapsed(), 'g', 3)));
#endif

    if (!mCellmlApiOdeCodeInformation)
        return this;

    // An ODE code information could be retrieved, so we can determine the
    // model's type

    mModelType = mCellmlApiOdeCodeInformation->flaggedEquations()->length()?Dae:Ode;

    // If the model is of DAE type, then we don't want the ODE-specific code
    // information, but the DAE-specific code information

    iface::cellml_services::CodeInformation *genericCodeInformation;

    if (mModelType == Ode)
        genericCodeInformation = mCellmlApiOdeCodeInformation;
    else
#ifdef QT_DEBUG
    {
        time.restart();
#endif

        genericCodeInformation = getDaeCodeInformation(cellmlApiModel);

#ifdef QT_DEBUG
        qDebug(" - CellML DAE code information time: %s s", qPrintable(QString::number(0.001*time.elapsed(), 'g', 3)));
    }
#endif

    // Retrieve the variable of integration, if any

    ObjRef<iface::cellml_services::ComputationTargetIterator> computationTargetIterator = mCellmlApiOdeCodeInformation->iterateTargets();

    forever {
        ObjRef<iface::cellml_services::ComputationTarget> computationTarget = computationTargetIterator->nextComputationTarget();

        if (!computationTarget)
            // We couldn't find a variable of integration, so...

            break;

        if (computationTarget->type() == iface::cellml_services::VARIABLE_OF_INTEGRATION) {
            // We have found our variable of integration, so retrieve our
            // internal representation of it and leave our loop

            ObjRef<iface::cellml_api::CellMLVariable> variable = computationTarget->variable();

            mVariableOfIntegration = pCellmlFile->component(QString::fromStdWString(variable->componentName()))->variable(QString::fromStdWString(variable->name()));

            break;
        }
    }

#ifdef QT_DEBUG
    if (mVariableOfIntegration)
        qDebug(" - Variable of integration: %s [unit: %s] [component: %s]",
               qPrintable(mVariableOfIntegration->name()),
               qPrintable(mVariableOfIntegration->unit()),
               qPrintable(mVariableOfIntegration->component()));
    else
        qDebug(" - Variable of integration: none");
#endif

    // Retrieve all the model parameters

    computationTargetIterator = mCellmlApiOdeCodeInformation->iterateTargets();

    forever {
        ObjRef<iface::cellml_services::ComputationTarget> computationTarget = computationTargetIterator->nextComputationTarget();

        if (!computationTarget)
            // No more model paramaters, so...

            break;

        // Determine the type of the model parameter
        // Note: ideally, we would use computationTarget->type(), but it will
        //       return iface::cellml_services::ALGEBRAIC for both a rate and a
        //       'proper' algebraic variable, so...

        QString modelParamName = QString::fromStdWString(computationTarget->name());

        static const QString indexRegExp = "0|[1-9][0-9]*";

        QRegularExpression constantRegExp("^CONSTANTS\\["+indexRegExp+"\\]$");
        QRegularExpression stateRegExp("^STATES\\["+indexRegExp+"\\]$");
        QRegularExpression algebraicRegExp("^ALGEBRAIC\\["+indexRegExp+"\\]$");

        CellmlFileRuntimeModelParameter::ModelParameterType modelParameterType = CellmlFileRuntimeModelParameter::Undefined;

        if (constantRegExp.match(modelParamName).hasMatch())
            modelParameterType = CellmlFileRuntimeModelParameter::Constant;
        else if (stateRegExp.match(modelParamName).hasMatch())
            modelParameterType = CellmlFileRuntimeModelParameter::State;
        else if (algebraicRegExp.match(modelParamName).hasMatch())
            modelParameterType = CellmlFileRuntimeModelParameter::Algebraic;

        if (modelParameterType != CellmlFileRuntimeModelParameter::Undefined) {
            ObjRef<iface::cellml_api::CellMLVariable> variable = computationTarget->variable();

            mModelParameters.append(new CellmlFileRuntimeModelParameter(pCellmlFile->component(QString::fromStdWString(variable->componentName()))->variable(QString::fromStdWString(variable->name())),
                                                                        modelParameterType,
                                                                        computationTarget->assignedIndex()));
        }
    }

#ifdef QT_DEBUG
    if (mModelParameters.isEmpty()) {
        qDebug(" - Model parameters: none");
    } else {
        qDebug(" - Model parameters:");

        foreach (CellmlFileRuntimeModelParameter *modelParameter, mModelParameters) {
            QString modelParameterType = QString();

            switch (modelParameter->type()) {
            case CellmlFileRuntimeModelParameter::Constant:
                modelParameterType = "constant";

                break;
            case CellmlFileRuntimeModelParameter::State:
                modelParameterType = "state";

                break;
            case CellmlFileRuntimeModelParameter::Algebraic:
                modelParameterType = "algebraic";

                break;
            default:
                modelParameterType = "???";
            }

            qDebug("    - %s [unit: %s] [component: %s] [type: %s] [index: %d]",
                   qPrintable(modelParameter->variable()->name()),
                   qPrintable(modelParameter->variable()->unit()),
                   qPrintable(modelParameter->variable()->component()),
                   qPrintable(modelParameterType),
                   modelParameter->index());
        }
    }
#endif

    // Generate the model code, after having prepended to it all the external
    // functions which may, or not, be needed
    // Note: indeed, we cannot include header files since we don't (and don't
    //       want in order to avoid complications) deploy them with OpenCOR. So,
    //       instead, we must declare as external functions all the functions
    //       which we would normally use through header files...

    QString modelCode = "extern double fabs(double);\n"
                        "\n"
                        "extern double exp(double);\n"
                        "extern double log(double);\n"
                        "\n"
                        "extern double ceil(double);\n"
                        "extern double floor(double);\n"
                        "\n"
                        "extern double factorial(double);\n"
                        "\n"
                        "extern double sin(double);\n"
                        "extern double cos(double);\n"
                        "extern double tan(double);\n"
                        "extern double sinh(double);\n"
                        "extern double cosh(double);\n"
                        "extern double tanh(double);\n"
                        "extern double asin(double);\n"
                        "extern double acos(double);\n"
                        "extern double atan(double);\n"
                        "extern double asinh(double);\n"
                        "extern double acosh(double);\n"
                        "extern double atanh(double);\n"
                        "\n"
                        "extern double arbitrary_log(double, double);\n"
                        "\n"
                        "extern double pow(double, double);\n"
                        "\n"
                        "extern double gcd_multi(int, ...);\n"
                        "extern double lcm_multi(int, ...);\n"
                        "extern double multi_max(int, ...);\n"
                        "extern double multi_min(int, ...);\n"
                        "\n";

    QString functionsString = QString::fromStdWString(genericCodeInformation->functionsString());

    if (!functionsString.isEmpty()) {
        // We will need to solve at least one non-linear algebraic system, so...

        mAtLeastOneNlaSystem = true;

        modelCode += "struct rootfind_info\n"
                     "{\n"
                     "    double aVOI;\n"
                     "\n"
                     "    double *aCONSTANTS;\n"
                     "    double *aRATES;\n"
                     "    double *aSTATES;\n"
                     "    double *aALGEBRAIC;\n"
                     "\n"
                     "    int *aPRET;\n"
                     "};\n"
                     "\n"
                     "extern void do_nonlinearsolve(void (*)(double *, double *, void*), double *, int *, int, void *);\n";
        modelCode += "\n";
        modelCode += functionsString;
        modelCode += "\n";
    }

    modelCode += functionCode("int initializeConstants(double *CONSTANTS, double *RATES, double *STATES)",
                              QString::fromStdWString(genericCodeInformation->initConstsString()),
                              true);
    modelCode += "\n";

    if (mModelType == Ode)
        modelCode += functionCode("int computeRates(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC)",
                                  QString::fromStdWString(mCellmlApiOdeCodeInformation->ratesString()));
    else
        modelCode += functionCode("int computeResiduals(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC, double *CONDVAR, double *resid)",
                                  QString::fromStdWString(mCellmlApiDaeCodeInformation->ratesString()));

    modelCode += "\n";
    modelCode += functionCode("int computeVariables(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC)",
                              QString::fromStdWString(genericCodeInformation->variablesString()));

    if (mModelType == Dae) {
        modelCode += "\n";
        modelCode += functionCode("int computeEssentialVariables(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC, double *CONDVAR)",
                                  QString::fromStdWString(mCellmlApiDaeCodeInformation->essentialVariablesString()));
        modelCode += "\n";
        modelCode += functionCode("int computeRootInformation(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC, double *CONDVAR)",
                                  QString::fromStdWString(mCellmlApiDaeCodeInformation->rootInformationString()));
        modelCode += "\n";
        modelCode += functionCode("int computeStateInformation(double *SI)",
                                  QString::fromStdWString(mCellmlApiDaeCodeInformation->stateInformationString()));
    }

    // Remove any '\r' character from our model code
    // Note: this is only so that it looks better on Windows when we need to
    //       debug things...

#if defined(Q_OS_WIN) && defined(QT_DEBUG)
    modelCode.replace("\r", "");
#endif

    // Compile the model code and check that everything went fine

#ifdef QT_DEBUG
    time.restart();
#endif

    if (!mCompilerEngine->compileCode(modelCode))
        // Something went wrong, so output the error that was found

        mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                   QString("%1").arg(mCompilerEngine->error()));

#ifdef QT_DEBUG
    qDebug(" - CellML code compilation time: %s s", qPrintable(QString::number(0.001*time.elapsed(), 'g', 3)));
#endif

    // Keep track of the ODE/DAE functions, but only if no issues were reported

    if (mIssues.count()) {
        // Some issues were reported, so...

        reset(false);
    } else {
        // Add the symbol of any required external function, if any

        if (mAtLeastOneNlaSystem)
            llvm::sys::DynamicLibrary::AddSymbol("do_nonlinearsolve",
                                                 (void *) (intptr_t) do_nonlinearsolve);

        // Retrieve the ODE/DAE functions

        if (mModelType == Ode) {
            mOdeFunctions.initializeConstants = (InitializeConstantsFunction) (intptr_t) mCompilerEngine->getFunction("initializeConstants");
            mOdeFunctions.computeRates        = (ComputeOdeRatesFunction) (intptr_t) mCompilerEngine->getFunction("computeRates");
            mOdeFunctions.computeVariables    = (ComputeVariablesFunction) (intptr_t) mCompilerEngine->getFunction("computeVariables");
        } else {
            mDaeFunctions.initializeConstants       = (InitializeConstantsFunction) (intptr_t) mCompilerEngine->getFunction("initializeConstants");
            mDaeFunctions.computeResiduals          = (ComputeDaeResidualsFunction) (intptr_t) mCompilerEngine->getFunction("computeResiduals");
            mDaeFunctions.computeVariables          = (ComputeVariablesFunction) (intptr_t) mCompilerEngine->getFunction("computeVariables");
            mDaeFunctions.computeEssentialVariables = (ComputeDaeEssentialVariablesFunction) (intptr_t) mCompilerEngine->getFunction("computeEssentialVariables");
            mDaeFunctions.computeRootInformation    = (ComputeDaeRootInformationFunction) (intptr_t) mCompilerEngine->getFunction("computeRootInformation");
            mDaeFunctions.computeStateInformation   = (ComputeDaeStateInformationFunction) (intptr_t) mCompilerEngine->getFunction("computeStateInformation");
        }
    }

    // We are done, so return ourselves

    return this;
}

//==============================================================================

CellmlFileVariable * CellmlFileRuntime::variableOfIntegration() const
{
    // Return our variable of integration, if any

    return mVariableOfIntegration;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
