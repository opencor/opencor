//==============================================================================
// CellML model runtime class
//==============================================================================

#include "cellmlmodelruntime.h"

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

CellmlModelRuntime::CellmlModelRuntime() :
    mComputerEngine(0)
{
    // Initialise the runtime's properties

    reset();
}

//==============================================================================

bool CellmlModelRuntime::isValid() const
{
    // The runtime is valid if no issue was found

    return mIssues.isEmpty();
}

//==============================================================================

CellmlModelRuntime::ModelType CellmlModelRuntime::modelType() const
{
    // Return the type of model for the runtime

    return mModelType;
}

//==============================================================================

int CellmlModelRuntime::constantsCount() const
{
    // Return the number of constants in the model

    if (mModelType == Ode)
        return mOdeCodeInformation?mOdeCodeInformation->constantIndexCount():0;
    else
        return mDaeCodeInformation?mDaeCodeInformation->constantIndexCount():0;
}

//==============================================================================

int CellmlModelRuntime::statesCount() const
{
    // Return the number of states in the model

    if (mModelType == Ode)
        return mOdeCodeInformation?mOdeCodeInformation->rateIndexCount():0;
    else
        return mDaeCodeInformation?mDaeCodeInformation->rateIndexCount():0;
}

//==============================================================================

int CellmlModelRuntime::ratesCount() const
{
    // Return the number of rates in the model
    // Note: it is obviously the same as the number of states, so this function
    //       is only for user convenience...

    return statesCount();
}

//==============================================================================

int CellmlModelRuntime::algebraicCount() const
{
    // Return the number of algebraic equations in the model

    if (mModelType == Ode)
        return mOdeCodeInformation?mOdeCodeInformation->algebraicIndexCount():0;
    else
        return mDaeCodeInformation?mDaeCodeInformation->algebraicIndexCount():0;
}

//==============================================================================

int CellmlModelRuntime::condVarCount() const
{
    // Return the number of conditional variables in the model

    if (mModelType == Ode)
        return 0;
    else
        return mDaeCodeInformation?mDaeCodeInformation->conditionVariableCount():0;
}

//==============================================================================

CellmlModelRuntimeOdeFunctions CellmlModelRuntime::odeFunctions() const
{
    // Return the ODE functions

    return mOdeFunctions;
}

//==============================================================================

CellmlModelRuntimeDaeFunctions CellmlModelRuntime::daeFunctions() const
{
    // Return the DAE functions

    return mDaeFunctions;
}

//==============================================================================

CellmlModelIssues CellmlModelRuntime::issues() const
{
    // Return the issue(s)

    return mIssues;
}

//==============================================================================

void CellmlModelRuntime::resetOdeCodeInformation()
{
    // Reset the ODE code information

    /*delete mOdeCodeInformation;*/ mOdeCodeInformation = 0;
    //---GRY--- WE CANNOT delete mOdeCodeInformation AT THIS STAGE. FOR THIS, WE
    //          WOULD NEED TO USE THE CLEANED UP C++ INTERFACE (SEE THE MAIN
    //          COMMENT AT THE BEGINNING OF THE cellmlmodel.cpp FILE)
}

//==============================================================================

void CellmlModelRuntime::resetDaeCodeInformation()
{
    // Reset the DAE code information

    /*delete mDaeCodeInformation;*/ mDaeCodeInformation = 0;
    //---GRY--- WE CANNOT delete mDaeCodeInformation AT THIS STAGE. FOR THIS, WE
    //          WOULD NEED TO USE THE CLEANED UP C++ INTERFACE (SEE THE MAIN
    //          COMMENT AT THE BEGINNING OF THE cellmlmodel.cpp FILE)
}

//==============================================================================

void CellmlModelRuntime::resetOdeFunctions()
{
    // Reset the ODE functions

    mOdeFunctions.initConsts = 0;
    mOdeFunctions.rates      = 0;
    mOdeFunctions.variables  = 0;
}

//==============================================================================

void CellmlModelRuntime::resetDaeFunctions()
{
    // Reset the DEA functions

    mDaeFunctions.initConsts         = 0;
    mDaeFunctions.rates              = 0;
    mDaeFunctions.variables          = 0;
    mDaeFunctions.essentialVariables = 0;
    mDaeFunctions.rootInformation    = 0;
    mDaeFunctions.stateInformation   = 0;
}

//==============================================================================

void CellmlModelRuntime::reset(const bool &pResetIssues)
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

void CellmlModelRuntime::couldNotGenerateModelCodeIssue()
{
    mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                    tr("the model code could not be generated")));
}

//==============================================================================

void CellmlModelRuntime::unexpectedProblemDuringModelCompilationIssue()
{
    mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                    tr("an unexpected problem occurred while trying to compile the model")));
}

//==============================================================================

void CellmlModelRuntime::checkCodeInformation(iface::cellml_services::CodeInformation *pCodeInformation)
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
            mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                            tr("the model is underconstrained (i.e. some variables need to be initialised or computed)")));
        } else if (constraintLevel == iface::cellml_services::UNSUITABLY_CONSTRAINED) {
            mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                            tr("the model is unsuitably constrained (i.e. some variables could not be found and/or some equations could not be used)")));
        } else if (constraintLevel == iface::cellml_services::OVERCONSTRAINED) {
            mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                            tr("the model is overconstrained (i.e. some variables are either both initialised and computed or computed more than once)")));
        }
    } else {
        // The code generation didn't go fine, so...

        mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                        tr("a problem occurred during the compilation of the model: %1").arg(codeGenerationErrorMessage)));
    }
}

//==============================================================================

void CellmlModelRuntime::customizeCodeGenerator(iface::cellml_services::CodeGenerator *pCodeGenerator)
{
    // Customise our code generator
    // Note #1: idealy we wouly only specify what needs to be customised, but
    //          that's not the way the CellML API works, so instead we need to
    //          'customise' everything...
    // Note #2: the customised functions are 'factorof', 'quotient' and 'rem'...
    // Note #3: for things that don't need customising, we must make sure that
    //          it's in synch with CDA_CodeGenerator::makeCodeGenerationState in
    //          [CellML_API]/CCGS/sources/CCGSImplementation.cpp...

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
                                                                          L"factorof: #prec[30(900)]factorOf(#expr1, #expr2)\r\n"   // Customised version
                                                                          L"floor: #prec[H]floor(#expr1)\r\n"
                                                                          L"gcd: #prec[H]gcd_multi(#count, #exprs[, ])\r\n"
                                                                          L"geq: #prec[30]#exprs[>=]\r\n"
                                                                          L"gt: #prec[30]#exprs[>]\r\n"
                                                                          L"implies: #prec[10(950)] !#expr1 || #expr2\r\n"
                                                                          L"int: #prec[H]defint(func#unique1, VOI, CONSTANTS, RATES, STATES, ALGEBRAIC, &#bvarIndex, #lowlimit, #uplimit, "
                                                                          L"pret)#supplement double func#unique1(double VOI, "
                                                                          L"double* CONSTANTS, double* RATES, double* STATES, double* ALGEBRAIC, int* pret) { return #expr1; }\r\n"
                                                                          L"lcm: #prec[H]lcm_multi(#count, #exprs[, ])\r\n"
                                                                          L"leq: #prec[30]#exprs[<=]\r\n"
                                                                          L"ln: #prec[H]log(#expr1)\r\n"
                                                                          L"log: #prec[H]arbitrary_log(#expr1, #logbase)\r\n"
                                                                          L"lt: #prec[30]#exprs[<]\r\n"
                                                                          L"max: #prec[H]multi_max(#count, #exprs[, ])\r\n"
                                                                          L"min: #prec[H]multi_min(#count, #exprs[, ])\r\n"
                                                                          L"minus: #prec[500]#expr1 - #expr2\r\n"
                                                                          L"neq: #prec[30]#expr1 != #expr2\r\n"
                                                                          L"not: #prec[950]!#expr1\r\n"
                                                                          L"or: #prec[10]#exprs[||]\r\n"
                                                                          L"plus: #prec[500]#exprs[+]\r\n"
                                                                          L"power: #prec[H]pow(#expr1, #expr2)\r\n"
                                                                          L"quotient: #prec[1000(0)]quotient(#expr1, #expr2))\r\n"   // Customised version
                                                                          L"rem: #prec[1000(0)]rem(#expr1, #expr2)\r\n"   // Customised version
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

iface::cellml_services::CodeInformation * CellmlModelRuntime::getOdeCodeInformation(iface::cellml_api::Model *pModel)
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

iface::cellml_services::CodeInformation * CellmlModelRuntime::getDaeCodeInformation(iface::cellml_api::Model *pModel)
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

CellmlModelRuntime * CellmlModelRuntime::update(iface::cellml_api::Model *pModel)
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

    if (pModel) {
        // Retrieve the model's type
        // Note: this can be done by checking whether some equations were
        //       flagged as needing a Newton-Raphson evaluation...

        getOdeCodeInformation(pModel);

        if (mOdeCodeInformation) {
            // An ODE code information could be retrieved, so we can determine
            // the model's type

            mModelType = mOdeCodeInformation->flaggedEquations()->length()?Dae:Ode;

            // If the model is of DAE type, then we must get the 'right' code
            // information

            iface::cellml_services::CodeInformation *genericOdeCodeInformation;

            if (mModelType == Dae)
                genericOdeCodeInformation = getDaeCodeInformation(pModel);
            else
                genericOdeCodeInformation = mOdeCodeInformation;

            // Retrieve some information/code

            qDebug("---------------------------------------");
            qDebug("initConstsString():");
            qDebug("");
            qDebug("%s", qPrintable(QString::fromStdWString(genericOdeCodeInformation->initConstsString())));
            qDebug("---------------------------------------");
            qDebug("ratesString():");
            qDebug("");
            qDebug("%s", qPrintable(QString::fromStdWString(genericOdeCodeInformation->ratesString())));
            qDebug("---------------------------------------");
            qDebug("variablesString():");
            qDebug("");
            qDebug("%s", qPrintable(QString::fromStdWString(genericOdeCodeInformation->variablesString())));

            if (mModelType == Dae) {
                qDebug("---------------------------------------");
                qDebug("essentialVariablesString():");
                qDebug("");
                qDebug("%s", qPrintable(QString::fromStdWString(mDaeCodeInformation->essentialVariablesString())));
                qDebug("---------------------------------------");
                qDebug("rootInformationString():");
                qDebug("");
                qDebug("%s", qPrintable(QString::fromStdWString(mDaeCodeInformation->rootInformationString())));
                qDebug("---------------------------------------");
                qDebug("stateInformationString():");
                qDebug("");
                qDebug("%s", qPrintable(QString::fromStdWString(mDaeCodeInformation->stateInformationString())));
            }

            // Get some binary code

            mComputerEngine->addFunction(QString("void initConsts(double *CONSTANTS, double *RATES, double *STATES)\n{\n%1}").arg(QString::fromStdWString(genericOdeCodeInformation->initConstsString())));
            handleErrors("initConsts");

            if (mModelType == Ode)
                mComputerEngine->addFunction(QString("void rates(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC)\n{\n%1}").arg(QString::fromStdWString(genericOdeCodeInformation->ratesString())));
            else
                mComputerEngine->addFunction(QString("void rates(double VOI, double *CONSTANTS, double *RATES, double *OLDRATES, double *STATES, double *OLDSTATES, double *ALGEBRAIC, double *CONDVAR, double *resid)\n{\n%1}").arg(QString::fromStdWString(genericOdeCodeInformation->ratesString())));

            handleErrors("rates");

            mComputerEngine->addFunction(QString("void variables(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC)\n{\n%1}").arg(QString::fromStdWString(genericOdeCodeInformation->variablesString())));
            handleErrors("variables");

            if (mModelType == Dae) {
                mComputerEngine->addFunction(QString("void essentialVariables(double VOI, double *CONSTANTS, double *RATES, double *OLDRATES, double *STATES, double *OLDSTATES, double *ALGEBRAIC, double *CONDVAR)\n{\n%1}").arg(QString::fromStdWString(mDaeCodeInformation->essentialVariablesString())));
                handleErrors("essentialVariables");

                mComputerEngine->addFunction(QString("void rootInformation(double VOI, double *CONSTANTS, double *RATES, double *OLDRATES, double *STATES, double *OLDSTATES, double *ALGEBRAIC, double *CONDVAR)\n{\n%1}").arg(QString::fromStdWString(mDaeCodeInformation->rootInformationString())));
                handleErrors("rootInformation");

                mComputerEngine->addFunction(QString("void stateInformation(double *SI)\n{\n%1}").arg(QString::fromStdWString(mDaeCodeInformation->stateInformationString())));
                handleErrors("stateInformation");
            }

//--- TESTING --- BEGIN ---

            mComputerEngine->addFunction("void test(double *pData)\n{\n  pData[0] = pData[4];\n  pData[1] = -pow(2, 3)*1+3*5+9+1*pData[3]*pData[3]/pData[4]/1;\n  pData[2] = 5-9/7;\n}");
            handleErrors("test");

            mComputerEngine->addFunction("double test2(double *pData)\n{\n  return rem(pData[3], pData[4])*rem(5, 3)*quotient(pData[4], pData[3])*quotient(17, 5)*factorOf(15, pData[3])*factorOf(9, 3)*arbitraryLog(pData[4], pData[3])*atanh(tanh(acosh(cosh(asinh(sinh(atan(tan(acos(cos(asin(sin(factorial(pData[3])/factorial(4)*pow(0+fabs(-3)*-pData[0]-0+exp(+log(pData[1])*1)/-ceil(pData[2])/-1e6, floor(fabs(pData[3])/3+0)+1)))))))))))));\n}");
            handleErrors("test2");

            // Test our "test" and "test2" functions

            llvm::Function *testFunction  = mComputerEngine->module()->getFunction("test");
            llvm::Function *test2Function = mComputerEngine->module()->getFunction("test2");

            if (testFunction && test2Function) {
                // Initialise our array of data

                static const int dataSize = 5;
                double data[dataSize];

                for (int i = 0; i < dataSize; ++i)
                    data[i] = 0;

                data[3] = 5;
                data[4] = 7;

                // Output the contents of our original array of data

                qDebug("---------------------------------------");
                qDebug("Original data value:");
                qDebug("");

                for (int i = 0; i < dataSize; ++i)
                    qDebug("data[%s] = %s", qPrintable(QString::number(i)), qPrintable(QString::number(data[i], 'g', 19)));

                // Call our LLVM's JIT-based "test" function

                ((void (*)(double *))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(testFunction))(data);

                // Output the contents of our updated array of data

                qDebug("---------------------------------------");
                qDebug("Data values after running the \"test\" function:");
                qDebug("");

                for (int i = 0; i < dataSize; ++i)
                    qDebug("data[%s] = %s", qPrintable(QString::number(i)), qPrintable(QString::number(data[i], 'g', 19)));

                // Call our LLVM's JIT-based "test2" function

                double test2Result = ((double (*)(double *))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(test2Function))(data);

                qDebug("---------------------------------------");
                qDebug("Return value from the \"test2\" function: %s", qPrintable(QString::number(test2Result, 'g', 19)));
            }

            // Output the contents of our computer engine's module so far

            qDebug("---------------------------------------");
            qDebug("All generated code so far:");
            qDebug("");
            mComputerEngine->module()->dump();
            qDebug("---------------------------------------");

//--- TESTING --- END ---

            // Keep track of the ODE/DAE functions, but only if no issues were
            // reported

            if (mIssues.count()) {
                // Some issues were reported, so...

                reset(false);
            } else if (mModelType == Ode) {
                // ODE functions

                mOdeFunctions.initConsts = (CellmlModelRuntimeInitConstsFunction)(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("initConsts"));
                mOdeFunctions.rates      = (CellmlModelRuntimeOdeRatesFunction)(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("rates"));
                mOdeFunctions.variables  = (CellmlModelRuntimeVariablesFunction)(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("variables"));
            } else {
                // DAE functions

                mDaeFunctions.initConsts         = (CellmlModelRuntimeInitConstsFunction)(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("initConsts"));
                mDaeFunctions.rates              = (CellmlModelRuntimeDaeRatesFunction)(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("rates"));
                mDaeFunctions.variables          = (CellmlModelRuntimeVariablesFunction)(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("variables"));
                mDaeFunctions.essentialVariables = (CellmlModelRuntimeDaeEssentialVariablesFunction)(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("essentialVariables"));
                mDaeFunctions.rootInformation    = (CellmlModelRuntimeDaeRootInformationFunction)(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("rootInformation"));
                mDaeFunctions.stateInformation   = (CellmlModelRuntimeDaeStateInformationFunction)(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mComputerEngine->module()->getFunction("stateInformation"));
            }
        } else {
            // No ODE code information could be retrieved, so...

            mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                            tr("no code information could be retrieved for the model")));
        }
    }

    // We are done, so return ourselves

    return this;
}

//==============================================================================

void CellmlModelRuntime::handleErrors(const QString &pFunctionName)
{
    if (mComputerEngine->parserErrors().count()) {
        // Something went wrong with the parsing of the function, so output the
        // error(s) that was(were) found

        qDebug("");

        if (mComputerEngine->parserErrors().count() == 1)
            qDebug("An error occurred:");
        else
            qDebug("Some errors occurred:");

        foreach (const Computer::ComputerError &error,
                 mComputerEngine->parserErrors()) {
            if (error.line() && error.column())
                qDebug(" - Line %s, column %s: %s", qPrintable(QString::number(error.line())), qPrintable(QString::number(error.column())), qPrintable(error.formattedMessage()));
            else
                qDebug(" - %s", qPrintable(error.formattedMessage()));

            if (!error.extraInformation().isEmpty())
                qDebug("%s", qPrintable(error.extraInformation()));
        }

        mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                        tr("the function '%1' could not be parsed").arg(pFunctionName)));
    } else if (!mComputerEngine->error().isEmpty()) {
        // Something went wrong with the addition of the function, so output the
        // error that was found

        qDebug("");
        qDebug("An error occurred: %s", qPrintable(mComputerEngine->error().formattedMessage()));

        mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                        tr("the function '%1' could not be compiled").arg(pFunctionName)));
    } else {
        // Check that we can find the LLVM function

        llvm::Function *function = mComputerEngine->module()->getFunction(qPrintable(pFunctionName));

        if (function) {
            qDebug("---------------------------------------");
            qDebug("The '%s' function was found...", qPrintable(pFunctionName));
        } else {
            qDebug("---------------------------------------");
            qDebug("The '%s' function doesn't exist...?!", qPrintable(pFunctionName));
        }
    }
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
