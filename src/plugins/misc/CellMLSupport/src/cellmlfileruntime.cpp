//==============================================================================
// CellML file runtime class
//==============================================================================

#include "cellmlfileruntime.h"
#include "compilerengine.h"

//==============================================================================

#include <QTime>

//==============================================================================

#include "CCGSBootstrap.hpp"
#include "MaLaESBootstrap.hpp"

//==============================================================================

#include "llvm/Module.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileRuntime::CellmlFileRuntime() :
    mCellmlApiOdeCodeInformation(0),
    mCellmlApiDaeCodeInformation(0),
    mCompilerEngine(0)
{
    // Initialise the runtime's properties

    reset();
}

//==============================================================================

CellmlFileRuntime::~CellmlFileRuntime()
{
    // Delete some internal objects
    // Note: both mCellmlApiOdeCodeInformation and mCellmlApiDaeCodeInformation
    //       get automatically deleted, if needed, so...

    delete mCompilerEngine;
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

    resetOdeCodeInformation();
    resetDaeCodeInformation();

    delete mCompilerEngine;

    mCompilerEngine = new Compiler::CompilerEngine();

    resetOdeFunctions();
    resetDaeFunctions();

    if (pResetIssues)
        mIssues.clear();
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
        // The code generation didn't go fine, so...

        mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                   tr("a problem occurred during the compilation of the model: %1").arg(codeGenerationErrorMessage));
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

CellmlFileRuntime * CellmlFileRuntime::update(iface::cellml_api::Model *pCellmlApiModel)
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

    if (!pCellmlApiModel)
        // No model was provided, so...

        return this;

    // Retrieve the model's type
    // Note: this can be done by checking whether some equations were flagged
    //       as needing a Newton-Raphson evaluation...

#ifdef QT_DEBUG
    QTime time;

    time.start();
#endif

    getOdeCodeInformation(pCellmlApiModel);

#ifdef QT_DEBUG
    qDebug(" - CellML ODE code information time: %s s.", qPrintable(QString::number(0.001*time.elapsed(), 'g', 3)));
#endif

    if (!mCellmlApiOdeCodeInformation) {
        // No ODE code information could be retrieved, so...

        mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                   tr("no code information could be retrieved for the model"));

        return this;
    }

    // An ODE code information could be retrieved, so we can determine the
    // model's type

    mModelType = mCellmlApiOdeCodeInformation->flaggedEquations()->length()?Dae:Ode;

    // If the model is of DAE type, then we must get the 'right' code
    // information

    iface::cellml_services::CodeInformation *genericCodeInformation;

    if (mModelType == Ode)
        genericCodeInformation = mCellmlApiOdeCodeInformation;
    else
#ifdef QT_DEBUG
    {
        time.restart();
#endif

        genericCodeInformation = getDaeCodeInformation(pCellmlApiModel);

#ifdef QT_DEBUG
        qDebug(" - CellML DAE code information time: %s s.", qPrintable(QString::number(0.001*time.elapsed(), 'g', 3)));
    }
#endif

    // Generate the model code, after having prepended to it all the external
    // functions which may, or not, be needed, as well as the functions that may
    // be needed by DAE models
    // Note: indeed, we cannot include header files since we don't (and don't
    //       want to avoid complications) deploy them with OpenCOR. So, instead,
    //       we must declare as external functions all the functions which we
    //       would normally use through header files...

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
                        "\n"
                        "struct rootfind_info\n"
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
                        "extern void do_nonlinearsolve(void (*)(double *, double *, void*), double *, int *, int, void *);\n"
                        "\n";

    modelCode += QString::fromStdWString(genericCodeInformation->functionsString());
    modelCode += "\n";
    modelCode += QString("int initializeConstants(double *CONSTANTS, double *RATES, double *STATES)\n"
                         "{\n"
                         "    int ret = 0;\n"
                         "    int *pret = &ret;\n"
                         "\n"
                         "#define VOI 0.0\n"
                         "#define ALGEBRAIC 0\n"
                         "\n"
                         "%1"
                         "\n"
                         "#undef ALGEBRAIC\n"
                         "#undef VOI\n"
                         "\n"
                         "    return ret;\n"
                         "}\n"
                         "\n").arg(QString::fromStdWString(genericCodeInformation->initConstsString()));

    if (mModelType == Ode)
        modelCode += QString("int computeRates(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC)\n"
                             "{\n"
                             "    int ret = 0;\n"
                             "    int *pret = &ret;\n"
                             "\n"
                             "%1"
                             "\n"
                             "    return ret;\n"
                             "}\n"
                             "\n").arg(QString::fromStdWString(mCellmlApiOdeCodeInformation->ratesString()));
    else
        modelCode += QString("int computeResiduals(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC, double *CONDVAR, double *resid)\n"
                             "{\n"
                             "    int ret = 0;\n"
                             "    int *pret = &ret;\n"
                             "\n"
                             "%1"
                             "\n"
                             "    return ret;\n"
                             "}\n"
                             "\n").arg(QString::fromStdWString(mCellmlApiDaeCodeInformation->ratesString()));

    modelCode += QString("int computeVariables(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC)\n"
                         "{\n"
                         "    int ret = 0;\n"
                         "    int *pret = &ret;\n"
                         "\n"
                         "%1"
                         "\n"
                         "    return ret;\n"
                         "}\n").arg(QString::fromStdWString(genericCodeInformation->variablesString()));

    if (mModelType == Dae) {
        modelCode += "\n";
        modelCode += QString("int computeEssentialVariables(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC, double *CONDVAR)\n"
                             "{\n"
                             "    int ret = 0;\n"
                             "    int *pret = &ret;\n"
                             "\n"
                             "%1"
                             "\n"
                             "    return ret;\n"
                             "}\n"
                             "\n").arg(QString::fromStdWString(mCellmlApiDaeCodeInformation->essentialVariablesString()));
        modelCode += QString("int computeRootInformation(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC, double *CONDVAR)\n"
                             "{\n"
                             "    int ret = 0;\n"
                             "    int *pret = &ret;\n"
                             "\n"
                             "%1"
                             "\n"
                             "    return ret;\n"
                             "}\n"
                             "\n").arg(QString::fromStdWString(mCellmlApiDaeCodeInformation->rootInformationString()));
        modelCode += QString("int computeStateInformation(double *SI)\n"
                             "{\n"
                             "    int ret = 0;\n"
                             "    int *pret = &ret;\n"
                             "\n"
                             "%1"
                             "\n"
                             "    return ret;\n"
                             "}\n").arg(QString::fromStdWString(mCellmlApiDaeCodeInformation->stateInformationString()));
    }

    // Compile the model code and check that everything went fine

#ifdef QT_DEBUG
    time.restart();
#endif

#ifdef QT_DEBUG
    if (!mCompilerEngine->compileCode(modelCode, true))
#else
    if (!mCompilerEngine->compileCode(modelCode))
#endif
        // Something went wrong, so output the error that was found

        mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                   QString("%1").arg(mCompilerEngine->error()));

#ifdef QT_DEBUG
    qDebug(" - CellML code compilation time: %s s.", qPrintable(QString::number(0.001*time.elapsed(), 'g', 3)));
#endif

    // Keep track of the ODE/DAE functions, but only if no issues were reported

    if (mIssues.count()) {
        // Some issues were reported, so...

        reset(false);
    } else if (mModelType == Ode) {
        // ODE functions

        mOdeFunctions.initializeConstants = (InitializeConstantsFunction) (intptr_t) mCompilerEngine->getFunction("initializeConstants");
        mOdeFunctions.computeRates        = (ComputeOdeRatesFunction) (intptr_t) mCompilerEngine->getFunction("computeRates");
        mOdeFunctions.computeVariables    = (ComputeVariablesFunction) (intptr_t) mCompilerEngine->getFunction("computeVariables");

        Q_ASSERT(mOdeFunctions.initializeConstants);
        Q_ASSERT(mOdeFunctions.computeRates);
        Q_ASSERT(mOdeFunctions.computeVariables);
    } else {
        // DAE functions

        mDaeFunctions.initializeConstants       = (InitializeConstantsFunction) (intptr_t) mCompilerEngine->getFunction("initializeConstants");
        mDaeFunctions.computeResiduals          = (ComputeDaeResidualsFunction) (intptr_t) mCompilerEngine->getFunction("computeResiduals");
        mDaeFunctions.computeVariables          = (ComputeVariablesFunction) (intptr_t) mCompilerEngine->getFunction("computeVariables");
        mDaeFunctions.computeEssentialVariables = (ComputeDaeEssentialVariablesFunction) (intptr_t) mCompilerEngine->getFunction("computeEssentialVariables");
        mDaeFunctions.computeRootInformation    = (ComputeDaeRootInformationFunction) (intptr_t) mCompilerEngine->getFunction("computeRootInformation");
        mDaeFunctions.computeStateInformation   = (ComputeDaeStateInformationFunction) (intptr_t) mCompilerEngine->getFunction("computeStateInformation");

        Q_ASSERT(mDaeFunctions.initializeConstants);
        Q_ASSERT(mDaeFunctions.computeResiduals);
        Q_ASSERT(mDaeFunctions.computeVariables);
        Q_ASSERT(mDaeFunctions.computeEssentialVariables);
        Q_ASSERT(mDaeFunctions.computeRootInformation);
        Q_ASSERT(mDaeFunctions.computeStateInformation);
    }

    // We are done, so return ourselves

    return this;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
