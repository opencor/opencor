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

CellmlModelRuntime::CellmlModelRuntime()
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

    mIssues.clear();
}

//==============================================================================

void CellmlModelRuntime::couldNotCompileModelIssue()
{
    mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                    tr("the model could not be compiled")));
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
        couldNotCompileModelIssue();
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
        couldNotCompileModelIssue();
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
            qDebug("");
            qDebug("Number of states/rates: %s", QString::number(genericCodeInformation->rateIndexCount()).toLatin1().constData());
            qDebug("Number of algebraic variables: %s", QString::number(genericCodeInformation->algebraicIndexCount()).toLatin1().constData());
            qDebug("Number of constants: %s", QString::number(genericCodeInformation->constantIndexCount()).toLatin1().constData());
            qDebug("");
            qDebug("---------------------------------------");
            qDebug("\ninitConstsString():");
            qDebug(QString::fromStdWString(genericCodeInformation->initConstsString()).toLatin1().constData());
            qDebug("---------------------------------------");
            qDebug("\nratesString():");
            qDebug(QString::fromStdWString(genericCodeInformation->ratesString()).toLatin1().constData());
            qDebug("---------------------------------------");
            qDebug("\nvariablesString():");
            qDebug(QString::fromStdWString(genericCodeInformation->variablesString()).toLatin1().constData());
            qDebug("---------------------------------------");
            qDebug("\nfunctionsString():");
            qDebug(QString::fromStdWString(genericCodeInformation->functionsString()).toLatin1().constData());
            qDebug("---------------------------------------");

            if (mModelType == Dae) {
                qDebug("\nessentialVariablesString():");
                qDebug(QString::fromStdWString(mDaeCodeInformation->essentialVariablesString()).toLatin1().constData());
                qDebug("---------------------------------------");
                qDebug("\nstateInformationString():");
                qDebug(QString::fromStdWString(mDaeCodeInformation->stateInformationString()).toLatin1().constData());
                qDebug("---------------------------------------");
                qDebug("\nrootInformationString():");
                qDebug(QString::fromStdWString(mDaeCodeInformation->rootInformationString()).toLatin1().constData());
                qDebug("---------------------------------------");
            }

            // Get some binary code using the Computer plugin

            Computer::ComputerEngine computerEngine;

            computerEngine.addFunction(QString("void initConsts(double *STATES, double *CONSTANTS) { %1 }").arg(QString::fromStdWString(genericCodeInformation->initConstsString())));
            handleErrors(computerEngine, "initConsts");

            computerEngine.addFunction(QString("void rates(double *ALGEBRAIC, double *RATES, double *STATES, double *CONSTANTS) { %1 }").arg(QString::fromStdWString(genericCodeInformation->ratesString())));
            handleErrors(computerEngine, "rates");

            // Output the contents of our computer engine's module so far

            qDebug("---------------------------------------");
            qDebug("All generated code so far:");
            computerEngine.module()->dump();
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

void CellmlModelRuntime::handleErrors(Computer::ComputerEngine &pComputerEngine,
                                      const QString &pFunctionName)
{
    if (pComputerEngine.parserErrors().count()) {
        // Something went wrong with the parsing of the function, so output the
        // error(s) that was(were) found

        qDebug("---------------------------------------");

        if (pComputerEngine.parserErrors().count() == 1)
            qDebug("An error occurred:");
        else
            qDebug("Some errors occurred:");

        foreach (const Computer::ComputerError &error,
                 pComputerEngine.parserErrors()) {
            if (error.line() && error.column())
                qDebug(QString(" - Line %1, column %2: %3").arg(QString::number(error.line()), QString::number(error.column()), error.formattedMessage()).toLatin1().constData());
            else
                qDebug(QString(" - %1").arg(error.formattedMessage()).toLatin1().constData());

            if (!error.extraInformation().isEmpty())
                qDebug(error.extraInformation().toLatin1().constData());
        }

        qDebug("---------------------------------------");

        mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                        tr("the model could not be compiled")));
    } else if (!pComputerEngine.error().isEmpty()) {
        // Something went wrong with the addition of the function, so
        // output the error that was found

        qDebug("---------------------------------------");

        qDebug(QString("An error occurred: %1").arg(pComputerEngine.error().formattedMessage()).toLatin1().constData());

        qDebug("---------------------------------------");

        mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                        tr("the model could not be compiled")));
    } else {
        // Test the compiled function using LLVM's JIT

        llvm::Function *function = pComputerEngine.module()->getFunction(pFunctionName.toLatin1().constData());

        if (function) {
            qDebug(QString("The '%1' function was found...").arg(pFunctionName).toLatin1().constData());

//            // Initialise our array of data

//            double data[6];

//            data[0] = 100;
//            data[1] = 101;
//            data[2] = 102;
//            data[3] = 103;
//            data[4] = 104;
//            data[5] = 105;

//            // Call our LLVM's JIT-based function

//            ((void (*)(double *))(intptr_t) pComputerEngine.executionEngine()->getPointerToFunction(function))(data);

//            // Output the contents of our data array

//            qDebug(QString("Data[0]: %1").arg(QString::number(data[0])).toLatin1().constData());
//            qDebug(QString("Data[1]: %1").arg(QString::number(data[1])).toLatin1().constData());
//            qDebug(QString("Data[2]: %1").arg(QString::number(data[2])).toLatin1().constData());
//            qDebug(QString("Data[3]: %1").arg(QString::number(data[3])).toLatin1().constData());
//            qDebug(QString("Data[4]: %1").arg(QString::number(data[4])).toLatin1().constData());
//            qDebug(QString("Data[5]: %1").arg(QString::number(data[5])).toLatin1().constData());
        } else {
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
