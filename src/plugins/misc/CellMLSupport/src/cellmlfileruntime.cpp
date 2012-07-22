//==============================================================================
// CellML file runtime class
//==============================================================================

#include "cellmlfileruntime.h"
#include "computerengine.h"

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
    mOdeCodeInformation(0),
    mDaeCodeInformation(0),
    mComputerEngine(0)
{
    // Initialise the runtime's properties

    reset();
}

//==============================================================================

CellmlFileRuntime::~CellmlFileRuntime()
{
    // Delete some internal objects
    // Note: both mOdeCodeInformation and mDaeCodeInformation get automatically
    //       deleted, if needed, so...

    delete mComputerEngine;
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
        return mOdeCodeInformation?mOdeCodeInformation->constantIndexCount():0;
    else
        return mDaeCodeInformation?mDaeCodeInformation->constantIndexCount():0;
}

//==============================================================================

int CellmlFileRuntime::statesCount() const
{
    // Return the number of states in the model

    if (mModelType == Ode)
        return mOdeCodeInformation?mOdeCodeInformation->rateIndexCount():0;
    else
        return mDaeCodeInformation?mDaeCodeInformation->rateIndexCount():0;
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
        return mOdeCodeInformation?mOdeCodeInformation->algebraicIndexCount():0;
    else
        return mDaeCodeInformation?mDaeCodeInformation->algebraicIndexCount():0;
}

//==============================================================================

int CellmlFileRuntime::condVarCount() const
{
    // Return the number of conditional variables in the model

    if (mModelType == Ode)
        return 0;
    else
        return mDaeCodeInformation?mDaeCodeInformation->conditionVariableCount():0;
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

    mOdeCodeInformation = 0;
}

//==============================================================================

void CellmlFileRuntime::resetDaeCodeInformation()
{
    // Reset the DAE code information
    // Note: setting it to zero will automatically delete the current instance,
    //       if any

    mDaeCodeInformation = 0;
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

    delete mComputerEngine;

    mComputerEngine = new Computer::ComputerEngine();

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

void CellmlFileRuntime::checkCodeInformation(iface::cellml_services::CodeInformation *pCodeInformation)
{
    if (!pCodeInformation)
        // No code information was provided, so...

        return;

    // Retrieve the code information's latest error message

    QString codeGenerationErrorMessage = QString::fromStdWString(pCodeInformation->errorMessage());

    if (codeGenerationErrorMessage.isEmpty()) {
        // The code generation went fine, so check the model's constraint level

        iface::cellml_services::ModelConstraintLevel constraintLevel = pCodeInformation->constraintLevel();

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

iface::cellml_services::CodeInformation * CellmlFileRuntime::getOdeCodeInformation(iface::cellml_api::Model *pModel)
{
    // Get a code generator bootstrap and create an ODE code generator

    ObjRef<iface::cellml_services::CodeGeneratorBootstrap> codeGeneratorBootstrap = CreateCodeGeneratorBootstrap();
    ObjRef<iface::cellml_services::CodeGenerator> codeGenerator = codeGeneratorBootstrap->createCodeGenerator();

    // Generate some code for the model (i.e. 'compile' the model)

    try {
        mOdeCodeInformation = codeGenerator->generateCode(pModel);

        // Check that the code generation went fine

        checkCodeInformation(mOdeCodeInformation);
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

    return mOdeCodeInformation;
}

//==============================================================================

iface::cellml_services::CodeInformation * CellmlFileRuntime::getDaeCodeInformation(iface::cellml_api::Model *pModel)
{
    // Get a code generator bootstrap and create a DAE code generator

    ObjRef<iface::cellml_services::CodeGeneratorBootstrap> codeGeneratorBootstrap = CreateCodeGeneratorBootstrap();
    ObjRef<iface::cellml_services::IDACodeGenerator> codeGenerator = codeGeneratorBootstrap->createIDACodeGenerator();

    // Generate some code for the model (i.e. 'compile' the model)

    try {
        mDaeCodeInformation = codeGenerator->generateIDACode(pModel);

        // Check that the code generation went fine

        checkCodeInformation(mDaeCodeInformation);
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

    return mDaeCodeInformation;
}

//==============================================================================

CellmlFileRuntime * CellmlFileRuntime::update(iface::cellml_api::Model *pModel)
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

    if (!pModel)
        // No model was provided, so...

        return this;

    // Retrieve the model's type
    // Note: this can be done by checking whether some equations were flagged
    //       as needing a Newton-Raphson evaluation...

#ifdef QT_DEBUG
    QTime time;

    time.start();
#endif

    getOdeCodeInformation(pModel);

#ifdef QT_DEBUG
    qDebug(" - CellML ODE code information time: %s s", qPrintable(QString::number(0.001*time.elapsed(), 'g', 3)));
#endif

    if (!mOdeCodeInformation) {
        // No ODE code information could be retrieved, so...

        mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                   tr("no code information could be retrieved for the model"));

        return this;
    }

    // An ODE code information could be retrieved, so we can determine the
    // model's type

    mModelType = mOdeCodeInformation->flaggedEquations()->length()?Dae:Ode;

    // If the model is of DAE type, then we must get the 'right' code
    // information

    iface::cellml_services::CodeInformation *genericCodeInformation;

    if (mModelType == Ode)
        genericCodeInformation = mOdeCodeInformation;
    else
#ifdef QT_DEBUG
    {
        time.restart();
#endif

        genericCodeInformation = getDaeCodeInformation(pModel);

#ifdef QT_DEBUG
        qDebug(" - CellML DAE code information time: %s s", qPrintable(QString::number(0.001*time.elapsed(), 'g', 3)));
    }
#endif

    // Get some binary code

#ifdef QT_DEBUG
    time.restart();
#endif

    QString initializeConstantsString = QString::fromStdWString(genericCodeInformation->initConstsString());

    // Get rid of any call to rootfind_<ID>
    // Note: for some DAE models, the CellML API generates code similar to
    //       rootfind_<ID>(VOI, CONSTANTS, RATES, STATES, ALGEBRAIC, pret);\r\n
    //       which we are not (currently at least) supporting, so...
    //---GRY--- THE CALL TO rootfind_<ID> IS TO DO A LevMar CALL, BUT THIS
    //          SHOULDN'T BE NECESSARY WHEN USING A 'PROPER' DAE SOLVER (E.G.
    //          IDA). INSTEAD, THE SOLVER SHOULD PROVIDE A WAY TO DO SOMETHING
    //          SIMILAR TO A LevMar. USING IDA, THE WAY TO DO IT IS PROBABLY BY
    //          MAKING A CALL TO IDACalcIC()?...

    initializeConstantsString.remove(QRegExp("rootfind_[0-9]+\\(VOI, CONSTANTS, RATES, STATES, ALGEBRAIC, pret\\);\\r\\n"));

    addAndCheckFunction("initializeConstants",
                        QString("void initializeConstants(double *CONSTANTS, double *RATES, double *STATES)\n{\n%1}").arg(initializeConstantsString));

    if (mModelType == Ode)
        addAndCheckFunction("computeRates",
                            QString("void computeRates(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC)\n{\n%1}").arg(QString::fromStdWString(mOdeCodeInformation->ratesString())));
    else
        addAndCheckFunction("computeResiduals",
                            QString("void computeResiduals(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC, double *CONDVAR, double *resid)\n{\n%1}").arg(QString::fromStdWString(mDaeCodeInformation->ratesString())));

    addAndCheckFunction("computeVariables",
                        QString("void computeVariables(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC)\n{\n%1}").arg(QString::fromStdWString(genericCodeInformation->variablesString())));

    if (mModelType == Dae) {
        addAndCheckFunction("computeEssentialVariables",
                            QString("void computeEssentialVariables(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC, double *CONDVAR)\n{\n%1}").arg(QString::fromStdWString(mDaeCodeInformation->essentialVariablesString())));
        addAndCheckFunction("computeRootInformation",
                            QString("void computeRootInformation(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC, double *CONDVAR)\n{\n%1}").arg(QString::fromStdWString(mDaeCodeInformation->rootInformationString())));
        addAndCheckFunction("computeStateInformation",
                            QString("void computeStateInformation(double *SI)\n{\n%1}").arg(QString::fromStdWString(mDaeCodeInformation->stateInformationString())));
    }

#ifdef QT_DEBUG
    qDebug(" - CellML binary code time: %s s", qPrintable(QString::number(0.001*time.elapsed(), 'g', 3)));
#endif

    // Keep track of the ODE/DAE functions, but only if no issues were reported

    if (mIssues.count()) {
        // Some issues were reported, so...

        reset(false);
    } else if (mModelType == Ode) {
        // ODE functions

        mOdeFunctions.initializeConstants = (InitializeConstantsFunction) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("initializeConstants"));
        mOdeFunctions.computeRates        = (ComputeOdeRatesFunction) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("computeRates"));
        mOdeFunctions.computeVariables    = (ComputeVariablesFunction) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("computeVariables"));
    } else {
        // DAE functions

        mDaeFunctions.initializeConstants       = (InitializeConstantsFunction) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("initializeConstants"));
        mDaeFunctions.computeResiduals          = (ComputeDaeResidualsFunction) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("computeResiduals"));
        mDaeFunctions.computeVariables          = (ComputeVariablesFunction) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("computeVariables"));
        mDaeFunctions.computeEssentialVariables = (ComputeDaeEssentialVariablesFunction) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("computeEssentialVariables"));
        mDaeFunctions.computeRootInformation    = (ComputeDaeRootInformationFunction) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("computeRootInformation"));
        mDaeFunctions.computeStateInformation   = (ComputeDaeStateInformationFunction) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("computeStateInformation"));
    }

    // We are done, so return ourselves

    return this;
}

//==============================================================================

void CellmlFileRuntime::addAndCheckFunction(const QString &pFunctionName,
                                            const QString &pFunctionBody)
{
    // Add the function to our computer engine

#ifdef QT_DEBUG
    mComputerEngine->addFunction(pFunctionName, pFunctionBody, true);
#else
    mComputerEngine->addFunction(pFunctionName, pFunctionBody);
#endif

    // Check that everything went fine

    if (!mComputerEngine->error().isEmpty())
        // Something went wrong, so output the error that was found

        mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                   QString("%1").arg(mComputerEngine->error().message()));
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
