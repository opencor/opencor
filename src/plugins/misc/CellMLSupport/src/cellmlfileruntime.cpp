//==============================================================================
// CellML file runtime class
//==============================================================================

#include "cellmlfile.h"
#include "cellmlfileruntime.h"
#include "compilerengine.h"
#include "compilermath.h"

//==============================================================================

#include <QRegularExpression>
#include <QStringList>
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

CellmlFileRuntimeModelParameter::CellmlFileRuntimeModelParameter(const QString &pName,
                                                                 const int &pDegree,
                                                                 const QString &pUnit,
                                                                 const QString &pComponent,
                                                                 const ModelParameterType &pType,
                                                                 const int &pIndex) :
    mName(pName),
    mDegree(pDegree),
    mUnit(pUnit),
    mComponent(pComponent),
    mType(pType),
    mIndex(pIndex)
{
}

//==============================================================================

QString CellmlFileRuntimeModelParameter::name() const
{
    // Return our name

    return mName;
}

//==============================================================================

int CellmlFileRuntimeModelParameter::degree() const
{
    // Return our degree

    return mDegree;
}

//==============================================================================

QString CellmlFileRuntimeModelParameter::unit() const
{
    // Return our unit

    return mUnit;
}

//==============================================================================

QString CellmlFileRuntimeModelParameter::component() const
{
    // Return our component

    return mComponent;
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
    mCompilerEngine(0),
    mVariableOfIntegration(0),
    mModelParameters(CellmlFileRuntimeModelParameters())
{
    // Reset (initialise, here) our properties

    reset(true, true);
}

//==============================================================================

CellmlFileRuntime::~CellmlFileRuntime()
{
    // Reset our properties

    reset(false, false);
}

//==============================================================================

QString CellmlFileRuntime::address() const
{
    // Return our address as a string

    QString res;

    res.sprintf("%p", this);

    return res;
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

bool CellmlFileRuntime::needOdeSolver() const
{
    // Return whether the model needs an ODE solver

    return mModelType == Ode;
}

//==============================================================================

bool CellmlFileRuntime::needDaeSolver() const
{
    // Return whether the model needs a DAE solver

    return mModelType == Dae;
}

//==============================================================================

bool CellmlFileRuntime::needNlaSolver() const
{
    // Return whether the model needs an NLA solver

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

CellmlFileRuntime::InitializeConstantsFunction CellmlFileRuntime::initializeConstants() const
{
    // Return the initializeConstants function

    return mInitializeConstants;
}

//==============================================================================

CellmlFileRuntime::ComputeComputedConstantsFunction CellmlFileRuntime::computeComputedConstants() const
{
    // Return the computeComputedConstants function

    return mComputeComputedConstants;
}

//==============================================================================

CellmlFileRuntime::ComputeEssentialVariablesFunction CellmlFileRuntime::computeEssentialVariables() const
{
    // Return the computeEssentialVariables function

    return mComputeEssentialVariables;
}

//==============================================================================

CellmlFileRuntime::ComputeRatesFunction CellmlFileRuntime::computeRates() const
{
    // Return the computeRates function

    return mComputeRates;
}

//==============================================================================

CellmlFileRuntime::ComputeResidualsFunction CellmlFileRuntime::computeResiduals() const
{
    // Return the computeResiduals function

    return mComputeResiduals;
}

//==============================================================================

CellmlFileRuntime::ComputeRootInformationFunction CellmlFileRuntime::computeRootInformation() const
{
    // Return the computeRootInformation function

    return mComputeRootInformation;
}

//==============================================================================

CellmlFileRuntime::ComputeStateInformationFunction CellmlFileRuntime::computeStateInformation() const
{
    // Return the computeStateInformation function

    return mComputeStateInformation;
}

//==============================================================================

CellmlFileRuntime::ComputeVariablesFunction CellmlFileRuntime::computeVariables() const
{
    // Return the computeVariables function

    return mComputeVariables;
}

//==============================================================================

CellmlFileIssues CellmlFileRuntime::issues() const
{
    // Return the issue(s)

    return mIssues;
}

//==============================================================================

CellmlFileRuntimeModelParameters CellmlFileRuntime::modelParameters() const
{
    // Return the model parameter(s)

    return mModelParameters;
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

void CellmlFileRuntime::resetFunctions()
{
    // Reset the functions

    mInitializeConstants = 0;

    mComputeComputedConstants = 0;
    mComputeEssentialVariables = 0;
    mComputeRates = 0;
    mComputeResiduals = 0;
    mComputeRootInformation = 0;
    mComputeStateInformation = 0;
    mComputeVariables = 0;
}

//==============================================================================

void CellmlFileRuntime::reset(const bool &pRecreateCompilerEngine,
                              const bool &pResetIssues)
{
    // Reset all of the runtime's properties

    mModelType = Undefined;
    mAtLeastOneNlaSystem = false;

    resetOdeCodeInformation();
    resetDaeCodeInformation();

    delete mCompilerEngine;

    if (pRecreateCompilerEngine)
        mCompilerEngine = new Compiler::CompilerEngine();
    else
        mCompilerEngine = 0;

    resetFunctions();

    if (pResetIssues)
        mIssues.clear();

    delete mVariableOfIntegration;

    mVariableOfIntegration = 0;

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

bool sortModelParameters(CellmlFileRuntimeModelParameter *pModelParameter1,
                         CellmlFileRuntimeModelParameter *pModelParameter2)
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

//==============================================================================

CellmlFileRuntime * CellmlFileRuntime::update(CellmlFile *pCellmlFile)
{
    // Reset the runtime's properties

    reset(true, true);

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

    // Retrieve all the model parameters and sort them by component/variable
    // name

    ObjRef<iface::cellml_services::ComputationTargetIterator> computationTargetIterator = genericCodeInformation->iterateTargets();

    forever {
        ObjRef<iface::cellml_services::ComputationTarget> computationTarget = computationTargetIterator->nextComputationTarget();

        if (!computationTarget)
            // No more model paramaters, so...

            break;

        // Determine the type of the model parameter

        CellmlFileRuntimeModelParameter::ModelParameterType modelParameterType;

        switch (computationTarget->type()) {
        case iface::cellml_services::VARIABLE_OF_INTEGRATION:
            modelParameterType = CellmlFileRuntimeModelParameter::Voi;

            break;
        case iface::cellml_services::CONSTANT: {
            // We are dealing with a constant, but the question is whether that
            // constant is a 'proper' constant or a 'computed' constant and this
            // can be determined by checking whether the computed target has an
            // initial value
            // Note: normally, we would retrieve the corresponding
            //       CellmlFileVariable object, but it may be that the constant
            //       was imported and is therefore not defined in the current
            //       CellML file, in which case there won't be any
            //       CellmlFileVariable object, so instead we rely on the CellML
            //       API version of a variable...

            ObjRef<iface::cellml_api::CellMLVariable> variable = computationTarget->variable();

            if (QString::fromStdWString(variable->initialValue()).isEmpty())
                // The computed target doesn't have an initial value, so it must
                // be a 'computed' constant

                modelParameterType = CellmlFileRuntimeModelParameter::ComputedConstant;
            else
                // The computed target has an initial value, so it must be a
                // 'proper' constant

                modelParameterType = CellmlFileRuntimeModelParameter::Constant;

            break;
        }
        case iface::cellml_services::STATE_VARIABLE:
        case iface::cellml_services::PSEUDOSTATE_VARIABLE:
            modelParameterType = CellmlFileRuntimeModelParameter::State;

            break;
        case iface::cellml_services::ALGEBRAIC:
            // We are dealing with either a 'proper' algebraic variable or a
            // rate variable
            // Note: if the variable's degree is equal to zero, then we are
            //       dealing with a 'proper' algebraic variable otherwise we
            //       are dealing with a rate variable...

            if (computationTarget->degree())
                modelParameterType = CellmlFileRuntimeModelParameter::Rate;
            else
                modelParameterType = CellmlFileRuntimeModelParameter::Algebraic;

            break;
        default:
            // We are dealing with a type of computed target which is of no
            // interest to us, so...

            modelParameterType = CellmlFileRuntimeModelParameter::Undefined;
        }

        // Keep track of the model parameter, should its type be known

        if (modelParameterType != CellmlFileRuntimeModelParameter::Undefined) {
            // Note: we cannot keep track of the model parameter using a pointer
            //       to a CellmlFileVariable object since our CellmlFileVariable
            //       objects are for the current CellML file only. In other
            //       words, it would only for models that don't have imports
            //       while we need a solution that works for any model, hence we
            //       we use CellmlFileRuntimeModelParameter instead...

            ObjRef<iface::cellml_api::CellMLVariable> variable = computationTarget->variable();
            CellmlFileRuntimeModelParameter *modelParameter = new CellmlFileRuntimeModelParameter(QString::fromStdWString(variable->name()),
                                                                                                  computationTarget->degree(),
                                                                                                  QString::fromStdWString(variable->unitsName()),
                                                                                                  QString::fromStdWString(variable->componentName()),
                                                                                                  modelParameterType,
                                                                                                  computationTarget->assignedIndex());

            if (modelParameterType == CellmlFileRuntimeModelParameter::Voi)
                mVariableOfIntegration = modelParameter;
            else
                mModelParameters.append(modelParameter);
        }
    }

    qSort(mModelParameters.begin(), mModelParameters.end(), sortModelParameters);

#ifdef QT_DEBUG
    if (mVariableOfIntegration)
        qDebug(" - Variable of integration: %s [unit: %s] [component: %s]",
               qPrintable(mVariableOfIntegration->name()),
               qPrintable(mVariableOfIntegration->unit()),
               qPrintable(mVariableOfIntegration->component()));
    else
        qDebug(" - Variable of integration: none");

    if (mModelParameters.isEmpty()) {
        qDebug(" - Model parameters: none");
    } else {
        qDebug(" - Model parameters:");

        QString component = QString();

        foreach (CellmlFileRuntimeModelParameter *modelParameter, mModelParameters)
            if (   (modelParameter->type() != CellmlFileRuntimeModelParameter::Voi)
                && (modelParameter->type() != CellmlFileRuntimeModelParameter::Undefined)) {
                QString crtComponent = modelParameter->component();

                if (modelParameter->component().compare(component)) {
                    qDebug("    - %s:", qPrintable(crtComponent));

                    component = crtComponent;
                }

                QString modelParameterType = QString();

                switch (modelParameter->type()) {
                case CellmlFileRuntimeModelParameter::Constant:
                    modelParameterType = "constant";

                    break;
                case CellmlFileRuntimeModelParameter::ComputedConstant:
                    modelParameterType = "computed constant";

                    break;
                case CellmlFileRuntimeModelParameter::State:
                    modelParameterType = "state";

                    break;
                case CellmlFileRuntimeModelParameter::Rate:
                    modelParameterType = "rate";

                    break;
                case CellmlFileRuntimeModelParameter::Algebraic:
                    modelParameterType = "algebraic";

                    break;
                default:
                    modelParameterType = "???";
                }

                qDebug("       - %s [degree: %d] [unit: %s] [component: %s] [type: %s] [index: %d]",
                       qPrintable(modelParameter->name()),
                       modelParameter->degree(),
                       qPrintable(modelParameter->unit()),
                       qPrintable(modelParameter->component()),
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
        // We will need to solve at least one NLA system, so...

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
                     "extern void doNonLinearSolve(char *, void (*)(double *, double *, void*), double *, int *, int, void *);\n";
        modelCode += "\n";
        modelCode += functionsString.replace("do_nonlinearsolve(", QString("doNonLinearSolve(\"%1\", ").arg(address()));
        modelCode += "\n";

        // Note: we rename do_nonlinearsolve() to doNonLinearSolve() because
        //       CellML's CIS service already defines do_nonlinearsolve(), yet
        //       we want to use our own non-linear solve routine defined in our
        //       Compiler plugin. Also, we add a new parameter to all our calls
        //       to doNonLinearSolve() so that doNonLinearSolve() can retrieve
        //       the correct instance of our NLA solver...
    }

    // Retrieve the body of the function that initialises constants and extract
    // the statements that are related to computed variables (since we want to
    // be able to recompute those whenever the user modifies a model parameter)
    // Note: ideally, we wouldn't have to do that, but the fact is that the
    //       CellML API doesn't distinguish between 'proper' and 'computed'
    //       constants, so...
    //       (See https://tracker.physiomeproject.org/show_bug.cgi?id=3499)

    QStringList initConstsList = QString::fromStdWString(genericCodeInformation->initConstsString()).split("\r\n");
    QString initConsts = QString();
    QString compCompConsts = QString();

    foreach (const QString &initConst, initConstsList)
        // Add the statement either to our list of 'proper' constants or
        // 'computed' constants

        if (QRegularExpression("^CONSTANTS\\[(0|[1-9][0-9]*)\\] = [^0-9]").match(initConst).hasMatch()) {
            // We have dealing with a statement for a computed constant, so...

            if (!compCompConsts.isEmpty())
                compCompConsts += "\n";

            compCompConsts += initConst;
        } else {
            if (!initConsts.isEmpty())
                initConsts += "\n";

            initConsts += initConst;
        }

    modelCode += functionCode("int initializeConstants(double *CONSTANTS, double *RATES, double *STATES)",
                              initConsts, true);
    modelCode += "\n";
    modelCode += functionCode("int computeComputedConstants(double *CONSTANTS, double *RATES, double *STATES)",
                              compCompConsts, true);
    modelCode += "\n";

    // Retrieve the body of the remaining functions

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

#ifdef Q_OS_WIN
    #ifdef QT_DEBUG
        modelCode.remove('\r');
    #endif
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

        reset(true, false);
    } else {
        // Add the symbol of any required external function, if any

        if (mAtLeastOneNlaSystem)
            llvm::sys::DynamicLibrary::AddSymbol("doNonLinearSolve",
                                                 (void *) (intptr_t) doNonLinearSolve);

        // Retrieve the ODE/DAE functions

        if (mModelType == Ode) {
            mInitializeConstants = (InitializeConstantsFunction) (intptr_t) mCompilerEngine->getFunction("initializeConstants");

            mComputeComputedConstants = (ComputeComputedConstantsFunction) (intptr_t) mCompilerEngine->getFunction("computeComputedConstants");
            mComputeRates             = (ComputeRatesFunction) (intptr_t) mCompilerEngine->getFunction("computeRates");
            mComputeVariables         = (ComputeVariablesFunction) (intptr_t) mCompilerEngine->getFunction("computeVariables");
        } else {
            mInitializeConstants = (InitializeConstantsFunction) (intptr_t) mCompilerEngine->getFunction("initializeConstants");

            mComputeComputedConstants  = (ComputeComputedConstantsFunction) (intptr_t) mCompilerEngine->getFunction("computeComputedConstants");
            mComputeResiduals          = (ComputeResidualsFunction) (intptr_t) mCompilerEngine->getFunction("computeResiduals");
            mComputeVariables          = (ComputeVariablesFunction) (intptr_t) mCompilerEngine->getFunction("computeVariables");
            mComputeEssentialVariables = (ComputeEssentialVariablesFunction) (intptr_t) mCompilerEngine->getFunction("computeEssentialVariables");
            mComputeRootInformation    = (ComputeRootInformationFunction) (intptr_t) mCompilerEngine->getFunction("computeRootInformation");
            mComputeStateInformation   = (ComputeStateInformationFunction) (intptr_t) mCompilerEngine->getFunction("computeStateInformation");
        }
    }

    // We are done, so return ourselves

    return this;
}

//==============================================================================

CellmlFileRuntimeModelParameter *CellmlFileRuntime::variableOfIntegration() const
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
