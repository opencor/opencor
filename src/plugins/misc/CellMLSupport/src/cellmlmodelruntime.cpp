//==============================================================================
// CellML model runtime class
//==============================================================================

#include "cellmlmodelruntime.h"

//==============================================================================

#include <QDebug>

//==============================================================================

#include "cellml-api-cxx-support.hpp"

#include "IfaceCCGS.hxx"

#include "CCGSBootstrap.hpp"

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

void CellmlModelRuntime::reset()
{
    // Reset all of the runtime's properties

//---GRY--- TO BE DONE...

    mModelType = OdeModel;
}

//==============================================================================

CellmlModelRuntime * CellmlModelRuntime::update(iface::cellml_api::Model *pModel,
                                                const bool &pValidModel)
{
    // Reset the runtime's properties

    reset();

    // Check that the model is valid

    if (pValidModel) {
        // The model is valid, so get a code generator bootstrap and then create
        // a code generator

        ObjRef<iface::cellml_services::CodeGeneratorBootstrap> codeGeneratorBootstrap = CreateCodeGeneratorBootstrap();
        ObjRef<iface::cellml_services::CodeGenerator> codeGenerator = codeGeneratorBootstrap->createCodeGenerator();

        // Generate some code for the model (i.e. 'compile' the model)

        ObjRef<iface::cellml_services::CodeInformation> codeInformation;

        try {
            codeInformation = codeGenerator->generateCode(pModel);
        } catch (iface::cellml_api::CellMLException &) {
            mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                            tr("the model could not be compiled")));

            return this;
        } catch (...) {
            mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                            tr("an unexpected problem occurred while trying to compile the model")));

            return this;
        }

        // Check that the code generation went fine

        QString codeGenerationErrorMessage = QString::fromStdWString(codeInformation->errorMessage());

        if (!codeGenerationErrorMessage.isEmpty()) {
            mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                            tr("a problem occurred during the compilation of the model: %1").arg(codeGenerationErrorMessage.toLatin1().constData())));

            return this;
        }

        // Now, we have the code information, so we can check whether the
        // model's level of constraint

        iface::cellml_services::ModelConstraintLevel constraintLevel = codeInformation->constraintLevel();

        if (constraintLevel == iface::cellml_services::UNDERCONSTRAINED) {
            mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                            tr("the model is underconstrained (i.e. some variables need to be initialised or computed)")));

            return this;
        } else if (constraintLevel == iface::cellml_services::UNSUITABLY_CONSTRAINED) {
            mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                            tr("the model is unsuitably constrained (i.e. some variables could not be found and/or some equations could not be used)")));

            return this;
        } else if (constraintLevel == iface::cellml_services::OVERCONSTRAINED) {
            mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                            tr("the model is overconstrained (i.e. some variables are either both initialised and computed or computed more than once)")));

            return this;
        }

        // The model is correctly constrained

        // Determine whether the model is a 'simple' ODE model or a DAE model
        // Note: this can be done by checking whether some equations were
        //       flagged as needing a Newton-Raphson evaluation...

        mModelType = codeInformation->flaggedEquations()->length()?DaeModel:OdeModel;



//        qDebug("---------------------------------------");
//        qDebug("\nRequired functions:");
//        qDebug(QString::fromStdWString(codeInformation->functionsString()).toLatin1().constData());
//        qDebug("---------------------------------------");
//        qDebug("\nInitialisations:");
//        qDebug(QString::fromStdWString(codeInformation->initConstsString()).toLatin1().constData());
//        qDebug("---------------------------------------");
//        qDebug("\nVariable evaluations:");
//        qDebug(QString::fromStdWString(codeInformation->variablesString()).toLatin1().constData());
//        qDebug("---------------------------------------");



        // Just testing the generation of 'C code'...

//qDebug() << QString::fromStdWString(getModelAsCCode(pModel));
    } else {
        // The model is not valid, so...

        mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                        tr("the model is not valid")));
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
