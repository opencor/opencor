//==============================================================================
// CellML model runtime class
//==============================================================================

#include "cellmlmodelruntime.h"

//==============================================================================

#include "CCGSBootstrap.hpp"

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

bool CellmlModelRuntime::isValid()
{
    // The runtime is valid if no issue was found

    return mIssues.isEmpty();
}

//==============================================================================

CellmlModelRuntime::ModelType CellmlModelRuntime::modelType()
{
    // Return the type of model for the runtime

    return mModelType;
}

//==============================================================================

Computer::ComputerEngine * CellmlModelRuntime::computerEngine()
{
    // Return the computer engine

    return mComputerEngine;
}

//==============================================================================

QList<CellmlModelIssue> CellmlModelRuntime::issues()
{
    // Return the issue(s)

    return mIssues;
}

//==============================================================================

void CellmlModelRuntime::resetOdeCodeInformation()
{
    /*delete mOdeCodeInformation;*/ mOdeCodeInformation = 0;
    //---GRY--- WE CANNOT delete mOdeCodeInformation AT THIS STAGE. FOR THIS, WE
    //          WOULD NEED TO USE THE CLEANED UP C++ INTERFACE (SEE THE MAIN
    //          COMMENT AT THE BEGINNING OF THE cellmlmodel.cpp FILE)
}

//==============================================================================

void CellmlModelRuntime::resetDaeCodeInformation()
{
    /*delete mDaeCodeInformation;*/ mDaeCodeInformation = 0;
    //---GRY--- WE CANNOT delete mDaeCodeInformation AT THIS STAGE. FOR THIS, WE
    //          WOULD NEED TO USE THE CLEANED UP C++ INTERFACE (SEE THE MAIN
    //          COMMENT AT THE BEGINNING OF THE cellmlmodel.cpp FILE)
}

//==============================================================================

void CellmlModelRuntime::reset()
{
    // Reset all of the runtime's properties

    mModelType = Undefined;

    resetOdeCodeInformation();
    resetDaeCodeInformation();

    delete mComputerEngine;

    mComputerEngine = new Computer::ComputerEngine;

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
                                        tr("a problem occurred during the compilation of the model: %1").arg(codeGenerationErrorMessage.toLatin1().constData())));
    }
}

//==============================================================================

iface::cellml_services::CodeInformation * CellmlModelRuntime::getOdeCodeInformation(iface::cellml_api::Model *pModel)
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

iface::cellml_services::CodeInformation * CellmlModelRuntime::getDaeCodeInformation(iface::cellml_api::Model *pModel)
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

CellmlModelRuntime * CellmlModelRuntime::update(iface::cellml_api::Model *pModel,
                                                const bool &pValidModel)
{
    // Reset the runtime's properties

    reset();

    // Check that the model is valid

    if (pValidModel) {
        // The model is valid, so make sure that it's either a 'simple' ODE
        // model or a DAE model
        // Note #1: in order to do so, we need to get a 'normal' code generator
        //          (as opposed to an IDA, i.e. DAE, code generator) since if
        //          the model is correctly constrained, then we can check
        //          whether some of its equations were flagged as needing a
        //          Newton-Raphson evaluation, in which case we would be dealing
        //          with a DAE model...
        // Note #2: ideally, there would be a more convenient way to determine
        //          the type of a model, but well... there isn't, so...

        getOdeCodeInformation(pModel);

        // Retrieve the model's type
        // Note: this can be done by checking whether some equations were
        //       flagged as needing a Newton-Raphson evaluation...

        if (mOdeCodeInformation) {
            // An ODE code information could be retrieved, so we can determine
            // the model's type

            mModelType = mOdeCodeInformation->flaggedEquations()->length()?Dae:Ode;

            // If the model is of DAE type, then we must get the 'right' code
            // information

            iface::cellml_services::CodeInformation *genericCodeInformation;

            if (mModelType == Dae)
                genericCodeInformation = getDaeCodeInformation(pModel);
            else
                genericCodeInformation = mOdeCodeInformation;

            // Retrieve some information/code

            qDebug("---------------------------------------");
            qDebug("initConstsString():");
            qDebug("");
            qDebug(QString::fromStdWString(genericCodeInformation->initConstsString()).toLatin1().constData());
            qDebug("---------------------------------------");
            qDebug("ratesString():");
            qDebug("");
            qDebug(QString::fromStdWString(genericCodeInformation->ratesString()).toLatin1().constData());
            qDebug("---------------------------------------");
            qDebug("variablesString():");
            qDebug("");
            qDebug(QString::fromStdWString(genericCodeInformation->variablesString()).toLatin1().constData());
            qDebug("---------------------------------------");
            qDebug("functionsString():");
            qDebug("");
            qDebug(QString::fromStdWString(genericCodeInformation->functionsString()).toLatin1().constData());

            if (mModelType == Dae) {
                qDebug("---------------------------------------");
                qDebug("essentialVariablesString():");
                qDebug("");
                qDebug(QString::fromStdWString(mDaeCodeInformation->essentialVariablesString()).toLatin1().constData());
                qDebug("---------------------------------------");
                qDebug("stateInformationString():");
                qDebug("");
                qDebug(QString::fromStdWString(mDaeCodeInformation->stateInformationString()).toLatin1().constData());
                qDebug("---------------------------------------");
                qDebug("rootInformationString():");
                qDebug("");
                qDebug(QString::fromStdWString(mDaeCodeInformation->rootInformationString()).toLatin1().constData());
            }

            // Get some binary code using the Computer plugin

            mComputerEngine->addFunction(QString("void initConsts(double *CONSTANTS, double *RATES, double *STATES)\n{\n%1}").arg(QString::fromStdWString(genericCodeInformation->initConstsString())));
            handleErrors("initConsts");

            mComputerEngine->addFunction(QString("void rates(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC)\n{\n%1}").arg(QString::fromStdWString(genericCodeInformation->ratesString())));
            handleErrors("rates");

            mComputerEngine->addFunction(QString("void variables(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC)\n{\n%1}").arg(QString::fromStdWString(genericCodeInformation->variablesString())));
            handleErrors("variables");

            if (mModelType == Dae) {
                mComputerEngine->addFunction(QString("void essentialVariables(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC)\n{\n%1}").arg(QString::fromStdWString(mDaeCodeInformation->essentialVariablesString())));
                handleErrors("essentialVariables");

                mComputerEngine->addFunction(QString("void stateInformation(double *SI)\n{\n%1}").arg(QString::fromStdWString(mDaeCodeInformation->stateInformationString())));
                handleErrors("stateInformation");
            }

            mComputerEngine->addFunction("void test(double *pData)\n{\n  pData[0] = pData[4];\n  pData[1] = -pow(2, 3)*1+3*5+9+1*pData[3]*pData[3]/pData[4]/1;\n  pData[2] = 5-9/7;\n}");
            handleErrors("test");

            mComputerEngine->addFunction("double test2(double *pData)\n{\n  return pow(0+-3*-pData[0]-0+exp(+pData[1]*1)/-pData[2]/-1e6, pData[3]/3+0);\n}");
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
                    qDebug(QString("data[%1] = %2").arg(QString::number(i), QString::number(data[i])).toLatin1().constData());

                // Call our LLVM's JIT-based "test" function

                ((void (*)(double *))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(testFunction))(data);

                // Output the contents of our updated array of data

                qDebug("---------------------------------------");
                qDebug("Data values after running the \"test\" function:");
                qDebug("");

                for (int i = 0; i < dataSize; ++i)
                    qDebug(QString("data[%1] = %2").arg(QString::number(i), QString::number(data[i])).toLatin1().constData());

                // Call our LLVM's JIT-based "test2" function

                double test2Result = ((double (*)(double *))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(test2Function))(data);

                qDebug("---------------------------------------");
                qDebug(QString("Return value from the \"test2\" function: %1").arg(QString::number(test2Result)).toLatin1().constData());
            }

            // Output the contents of our computer engine's module so far

            qDebug("---------------------------------------");
            qDebug("All generated code so far:");
            qDebug("");
            mComputerEngine->module()->dump();
            qDebug("---------------------------------------");
        } else {
            // No ODE code information could be retrieved, so...

            mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                            tr("no code information could be retrieved for the model")));
        }
    } else {
        // The model is not valid, so...

        mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                        tr("the model is not valid")));
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
                qDebug(QString(" - Line %1, column %2: %3").arg(QString::number(error.line()), QString::number(error.column()), error.formattedMessage()).toLatin1().constData());
            else
                qDebug(QString(" - %1").arg(error.formattedMessage()).toLatin1().constData());

            if (!error.extraInformation().isEmpty())
                qDebug(error.extraInformation().toLatin1().constData());
        }

        mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                        tr("the function '%1' could not be parsed").arg(pFunctionName)));
    } else if (!mComputerEngine->error().isEmpty()) {
        // Something went wrong with the addition of the function, so output the
        // error that was found

        qDebug("");
        qDebug(QString("An error occurred: %1").arg(mComputerEngine->error().formattedMessage()).toLatin1().constData());

        mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                        tr("the function '%1' could not be compiled").arg(pFunctionName)));
    } else {
        // Check that we can find the LLVM function

        llvm::Function *function = mComputerEngine->module()->getFunction(pFunctionName.toLatin1().constData());

        if (function) {
            qDebug("---------------------------------------");
            qDebug(QString("The '%1' function was found...").arg(pFunctionName).toLatin1().constData());
        } else {
            qDebug("---------------------------------------");
            qDebug(QString("The '%1' function doesn't exist...?!").arg(pFunctionName).toLatin1().constData());
        }
    }
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
