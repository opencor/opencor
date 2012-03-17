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
    mComputerEngine(0)
{
    // Initialise the runtime's properties

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

    delete mOdeCodeInformation; mOdeCodeInformation = 0;
}

//==============================================================================

void CellmlFileRuntime::resetDaeCodeInformation()
{
    // Reset the DAE code information

    delete mDaeCodeInformation; mDaeCodeInformation = 0;
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

    mComputerEngine = new Computer::ComputerEngine;

    resetOdeFunctions();
    resetDaeFunctions();

    if (pResetIssues)
        mIssues.clear();
}

//==============================================================================

void CellmlFileRuntime::couldNotGenerateModelCodeIssue()
{
    mIssues.append(CellmlFileIssue(CellmlFileIssue::Error,
                                   tr("the model code could not be generated")));
}

//==============================================================================

void CellmlFileRuntime::unexpectedProblemDuringModelCompilationIssue()
{
    mIssues.append(CellmlFileIssue(CellmlFileIssue::Error,
                                   tr("an unexpected problem occurred while trying to compile the model")));
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
            mIssues.append(CellmlFileIssue(CellmlFileIssue::Error,
                                           tr("the model is underconstrained (i.e. some variables need to be initialised or computed)")));
        } else if (constraintLevel == iface::cellml_services::UNSUITABLY_CONSTRAINED) {
            mIssues.append(CellmlFileIssue(CellmlFileIssue::Error,
                                           tr("the model is unsuitably constrained (i.e. some variables could not be found and/or some equations could not be used)")));
        } else if (constraintLevel == iface::cellml_services::OVERCONSTRAINED) {
            mIssues.append(CellmlFileIssue(CellmlFileIssue::Error,
                                           tr("the model is overconstrained (i.e. some variables are either both initialised and computed or computed more than once)")));
        }
    } else {
        // The code generation didn't go fine, so...

        mIssues.append(CellmlFileIssue(CellmlFileIssue::Error,
                                       tr("a problem occurred during the compilation of the model: %1").arg(codeGenerationErrorMessage)));
    }
}

//==============================================================================

void CellmlFileRuntime::customizeCodeGenerator(iface::cellml_services::CodeGenerator *pCodeGenerator)
{
    // Customise our code generator
    // Note #1: idealy we wouly only specify what needs to be customised, but
    //          that's not the way the CellML API works, so instead we need to
    //          'customise' everything...
    // Note #2: the customised elements are 'gcd', 'int', 'lcm', 'log', 'max',
    //          'min', 'quotient' and 'rem'...
    // Note #3: for things that don't need customising, we must make sure that
    //          it's in synch with CDA_CodeGenerator::makeCodeGenerationState in
    //          [CellML_API]/CCGS/sources/CCGSImplementation.cpp...
    // Note #4: we don't currently support definite integrals...

    ObjRef<iface::cellml_services::MaLaESBootstrap> malaesBootstrap = CreateMaLaESBootstrap();
    ObjRef<iface::cellml_services::MaLaESTransform> malaesTransform = malaesBootstrap->compileTransformer(
                                                                          L"opengroup: (\r\n"
                                                                          L"closegroup: )\r\n"
                                                                          L"abs: #prec[H]fabs(#expr1)\r\n"
                                                                          L"and: #prec[20]#exprs[&&]\r\n"
                                                                          L"arccos: #prec[H]acos(#expr1)\r\n"
                                                                          L"arccosh: #prec[H]acosh(#expr1)\r\n"
                                                                          L"arccot: #prec[1000(900)]atan(1.0/#expr1)\r\n"
                                                                          L"arccoth: #prec[1000(900)]atanh(1.0/#expr1)\r\n"
                                                                          L"arccsc: #prec[1000(900)]asin(1/#expr1)\r\n"
                                                                          L"arccsch: #prec[1000(900)]asinh(1/#expr1)\r\n"
                                                                          L"arcsec: #prec[1000(900)]acos(1/#expr1)\r\n"
                                                                          L"arcsech: #prec[1000(900)]acosh(1/#expr1)\r\n"
                                                                          L"arcsin: #prec[H]asin(#expr1)\r\n"
                                                                          L"arcsinh: #prec[H]asinh(#expr1)\r\n"
                                                                          L"arctan: #prec[H]atan(#expr1)\r\n"
                                                                          L"arctanh: #prec[H]atanh(#expr1)\r\n"
                                                                          L"ceiling: #prec[H]ceil(#expr1)\r\n"
                                                                          L"cos: #prec[H]cos(#expr1)\r\n"
                                                                          L"cosh: #prec[H]cosh(#expr1)\r\n"
                                                                          L"cot: #prec[900(0)]1.0/tan(#expr1)\r\n"
                                                                          L"coth: #prec[900(0)]1.0/tanh(#expr1)\r\n"
                                                                          L"csc: #prec[900(0)]1.0/sin(#expr1)\r\n"
                                                                          L"csch: #prec[900(0)]1.0/sinh(#expr1)\r\n"
                                                                          L"diff: #lookupDiffVariable\r\n"
                                                                          L"divide: #prec[900]#expr1/#expr2\r\n"
                                                                          L"eq: #prec[30]#exprs[==]\r\n"
                                                                          L"exp: #prec[H]exp(#expr1)\r\n"
                                                                          L"factorial: #prec[H]factorial(#expr1)\r\n"
                                                                          L"factorof: #prec[30(900)]#expr1 % #expr2 == 0\r\n"
                                                                          L"floor: #prec[H]floor(#expr1)\r\n"
                                                                          L"gcd: #prec[H]gcd(#count, #exprs[, ])\r\n"                           // Customised version
                                                                          L"geq: #prec[30]#exprs[>=]\r\n"
                                                                          L"gt: #prec[30]#exprs[>]\r\n"
                                                                          L"implies: #prec[10(950)] !#expr1 || #expr2\r\n"
                                                                          L"int: #prec[H]defint(#lowlimit, #uplimit, #expr1, #bvarIndex)\r\n"   // Customised version
                                                                          L"lcm: #prec[H]lcm(#count, #exprs[, ])\r\n"                           // Customised version
                                                                          L"leq: #prec[30]#exprs[<=]\r\n"
                                                                          L"ln: #prec[H]log(#expr1)\r\n"
                                                                          L"log: #prec[H]arbitraryLog(#expr1, #logbase)\r\n"                    // Customised version
                                                                          L"lt: #prec[30]#exprs[<]\r\n"
                                                                          L"max: #prec[H]max(#count, #exprs[, ])\r\n"                           // Customised version
                                                                          L"min: #prec[H]min(#count, #exprs[, ])\r\n"                           // Customised version
                                                                          L"minus: #prec[500]#expr1 - #expr2\r\n"
                                                                          L"neq: #prec[30]#expr1 != #expr2\r\n"
                                                                          L"not: #prec[950]!#expr1\r\n"
                                                                          L"or: #prec[10]#exprs[||]\r\n"
                                                                          L"plus: #prec[500]#exprs[+]\r\n"
                                                                          L"power: #prec[H]pow(#expr1, #expr2)\r\n"
                                                                          L"quotient: #prec[1000(0)]quot(#expr1, #expr2))\r\n"                  // Customised version
                                                                          L"rem: #prec[1000(0)]rem(#expr1, #expr2)\r\n"                         // Customised version
                                                                          L"root: #prec[1000(900)] pow(#expr1, 1.0 / #degree)\r\n"
                                                                          L"sec: #prec[900(0)]1.0 / cos(#expr1)\r\n"
                                                                          L"sech: #prec[900(0)]1.0 / cosh(#expr1)\r\n"
                                                                          L"sin: #prec[H] sin(#expr1)\r\n"
                                                                          L"sinh: #prec[H] sinh(#expr1)\r\n"
                                                                          L"tan: #prec[H] tan(#expr1)\r\n"
                                                                          L"tanh: #prec[H] tanh(#expr1)\r\n"
                                                                          L"times: #prec[900] #exprs[*]\r\n"
                                                                          L"unary_minus: #prec[950]- #expr1\r\n"
                                                                          L"units_conversion: #prec[500(900)]#expr1*#expr2 + #expr3\r\n"
                                                                          L"units_conversion_factor: #prec[900]#expr1*#expr2\r\n"
                                                                          L"units_conversion_offset: #prec[500]#expr1+#expr2\r\n"
                                                                          L"xor: #prec[25(30)] (#expr1 != 0) ^ (#expr2 != 0)\r\n"
                                                                          L"piecewise_first_case: #prec[1000(5)](#expr1 ? #expr2 : \r\n"
                                                                          L"piecewise_extra_case: #prec[1000(5)]#expr1 ? #expr2 : \r\n"
                                                                          L"piecewise_otherwise: #prec[1000(5)]#expr1)\r\n"
                                                                          L"piecewise_no_otherwise: #prec[1000(5)]0.0/0.0)\r\n"
                                                                          L"eulergamma: #prec[999]0.577215664901533\r\n"
                                                                          L"exponentiale: #prec[999]2.71828182845905\r\n"
                                                                          L"false: #prec[999]0.0\r\n"
                                                                          L"infinity: #prec[900]1.0/0.0\r\n"
                                                                          L"notanumber: #prec[999]0.0/0.0\r\n"
                                                                          L"pi: #prec[999] 3.14159265358979\r\n"
                                                                          L"true: #prec[999]1.0\r\n"
                                                                      );

    pCodeGenerator->transform(malaesTransform);
}

//==============================================================================

iface::cellml_services::CodeInformation * CellmlFileRuntime::getOdeCodeInformation(iface::cellml_api::Model *pModel)
{
    // Get a code generator bootstrap, create an ODE code generator and
    // customise it

    ObjRef<iface::cellml_services::CodeGeneratorBootstrap> codeGeneratorBootstrap = CreateCodeGeneratorBootstrap();
    ObjRef<iface::cellml_services::CodeGenerator> codeGenerator = codeGeneratorBootstrap->createCodeGenerator();

    customizeCodeGenerator(codeGenerator);

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
    // Get a code generator bootstrap, create a DAE code generator and customise
    // it

    ObjRef<iface::cellml_services::CodeGeneratorBootstrap> codeGeneratorBootstrap = CreateCodeGeneratorBootstrap();
    ObjRef<iface::cellml_services::IDACodeGenerator> codeGenerator = codeGeneratorBootstrap->createIDACodeGenerator();

    customizeCodeGenerator(codeGenerator);

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

QTime time;

time.start();

    getOdeCodeInformation(pModel);

qDebug(" - CellML ODE code information time: %s s", qPrintable(QString::number(0.001*time.elapsed(), 'g', 3)));

    if (!mOdeCodeInformation) {
        // No ODE code information could be retrieved, so...

        mIssues.append(CellmlFileIssue(CellmlFileIssue::Error,
                                       tr("no code information could be retrieved for the model")));

        return this;
    }

    // An ODE code information could be retrieved, so we can determine the
    // model's type

    mModelType = mOdeCodeInformation->flaggedEquations()->length()?Dae:Ode;

    // If the model is of DAE type, then we must get the 'right' code
    // information

    iface::cellml_services::CodeInformation *genericOdeCodeInformation;

    if (mModelType == Ode)
        genericOdeCodeInformation = mOdeCodeInformation;
    else
{
time.restart();

        genericOdeCodeInformation = getDaeCodeInformation(pModel);

qDebug(" - CellML DAE code information time: %s s", qPrintable(QString::number(0.001*time.elapsed(), 'g', 3)));
}

    // Get some binary code

time.restart();

    mComputerEngine->addFunction(QString("void initializeConstants(double *CONSTANTS, double *RATES, double *STATES)\n{\n%1}").arg(QString::fromStdWString(genericOdeCodeInformation->initConstsString())));
    checkFunction("initializeConstants");

    if (mModelType == Ode)
        mComputerEngine->addFunction(QString("void computeRates(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC)\n{\n%1}").arg(QString::fromStdWString(genericOdeCodeInformation->ratesString())));
    else
        mComputerEngine->addFunction(QString("void computeResiduals(double VOI, double *CONSTANTS, double *RATES, double *OLDRATES, double *STATES, double *OLDSTATES, double *ALGEBRAIC, double *CONDVAR, double *resid)\n{\n%1}").arg(QString::fromStdWString(genericOdeCodeInformation->ratesString())));

    checkFunction("computeRates");

    mComputerEngine->addFunction(QString("void computeVariables(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC)\n{\n%1}").arg(QString::fromStdWString(genericOdeCodeInformation->variablesString())));
    checkFunction("computeVariables");

    if (mModelType == Dae) {
        mComputerEngine->addFunction(QString("void computeEssentialVariables(double VOI, double *CONSTANTS, double *RATES, double *OLDRATES, double *STATES, double *OLDSTATES, double *ALGEBRAIC, double *CONDVAR)\n{\n%1}").arg(QString::fromStdWString(mDaeCodeInformation->essentialVariablesString())));
        checkFunction("computeEssentialVariables");

        mComputerEngine->addFunction(QString("void computeRootInformation(double VOI, double *CONSTANTS, double *RATES, double *OLDRATES, double *STATES, double *OLDSTATES, double *ALGEBRAIC, double *CONDVAR)\n{\n%1}").arg(QString::fromStdWString(mDaeCodeInformation->rootInformationString())));
        checkFunction("computeRootInformation");

        mComputerEngine->addFunction(QString("void computeStateInformation(double *SI)\n{\n%1}").arg(QString::fromStdWString(mDaeCodeInformation->stateInformationString())));
        checkFunction("computeStateInformation");
    }

qDebug(" - CellML binary code time: %s s", qPrintable(QString::number(0.001*time.elapsed(), 'g', 3)));

    // Keep track of the ODE/DAE functions, but only if no issues were reported

    if (mIssues.count()) {
        // Some issues were reported, so...

        reset(false);
    } else if (mModelType == Ode) {
        // ODE functions

        mOdeFunctions.initializeConstants = (InitializeConstantsFunction)(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("initializeConstants"));
        mOdeFunctions.computeRates        = (ComputeOdeRatesFunction)(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("computeRates"));
        mOdeFunctions.computeVariables    = (ComputeVariablesFunction)(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("computeVariables"));
    } else {
        // DAE functions

        mDaeFunctions.initializeConstants       = (InitializeConstantsFunction)(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("initializeConstants"));
        mDaeFunctions.computeResiduals          = (ComputeDaeResidualsFunction)(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("computeRates"));
        mDaeFunctions.computeVariables          = (ComputeVariablesFunction)(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("computeVariables"));
        mDaeFunctions.computeEssentialVariables = (ComputeDaeEssentialVariablesFunction)(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("computeEssentialVariables"));
        mDaeFunctions.computeRootInformation    = (ComputeDaeRootInformationFunction)(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("computeRootInformation"));
        mDaeFunctions.computeStateInformation   = (ComputeDaeStateInformationFunction)(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("computeStateInformation"));
    }

    // We are done, so return ourselves

    return this;
}

//==============================================================================

void CellmlFileRuntime::checkFunction(const QString &pFunctionName)
{
    if (!mComputerEngine->parserError().isEmpty()) {
        // Something went wrong with the parsing of the function, so output the
        // error(s) that was(were) found

        mIssues.append(CellmlFileIssue(CellmlFileIssue::Error,
                                       tr("the '%1' function could not be parsed").arg(pFunctionName)));

        mIssues.append(CellmlFileIssue(CellmlFileIssue::Error,
                                       mComputerEngine->parserError().message(),
                                       mComputerEngine->parserError().line(),
                                       mComputerEngine->parserError().column()));
    } else if (!mComputerEngine->error().isEmpty()) {
        // Something went wrong with the addition of the function, so output the
        // error that was found

        mIssues.append(CellmlFileIssue(CellmlFileIssue::Error,
                                       tr("the '%1' function could not be compiled (%2)").arg(pFunctionName, mComputerEngine->error().message())));
    }
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
