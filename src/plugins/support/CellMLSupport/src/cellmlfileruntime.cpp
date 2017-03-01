/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// CellML file runtime
//==============================================================================

#ifdef _WIN32
    #define _SCL_SECURE_NO_WARNINGS
#endif

//==============================================================================

#include "cellmlfile.h"
#include "cellmlfileruntime.h"
#include "compilerengine.h"
#include "compilermath.h"
#include "corecliutils.h"
#include "solverinterface.h"

//==============================================================================

#include <QRegularExpression>
#include <QStringList>

//==============================================================================

#include "cellmlapibegin.h"
    #include "CCGSBootstrap.hpp"
#include "cellmlapiend.h"

//==============================================================================

#include "llvmbegin.h"
    #include "llvm/ADT/StringRef.h"
    #include "llvm/Support/DynamicLibrary.h"
#include "llvmend.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileRuntimeParameter::CellmlFileRuntimeParameter(const QString &pName,
                                                       const int &pDegree,
                                                       const QString &pUnit,
                                                       const QStringList &pComponentHierarchy,
                                                       const ParameterType &pType,
                                                       const int &pIndex) :
    mName(pName),
    mDegree(pDegree),
    mUnit(pUnit),
    mComponentHierarchy(pComponentHierarchy),
    mType(pType),
    mIndex(pIndex)
{
}

//==============================================================================

bool CellmlFileRuntimeParameter::compare(CellmlFileRuntimeParameter *pParameter1,
                                         CellmlFileRuntimeParameter *pParameter2)
{
    // Determine which of the two parameters should be first
    // Note: the two comparisons are case insensitive, so that it's easier for
    //       people to find a parameter...

    QString componentHierarchy1 = pParameter1->formattedComponentHierarchy();
    QString componentHierarchy2 = pParameter2->formattedComponentHierarchy();

    if (!componentHierarchy1.compare(componentHierarchy2)) {
        if (!pParameter1->name().compare(pParameter2->name()))
            return pParameter1->degree() < pParameter2->degree();
        else
            return pParameter1->name().compare(pParameter2->name(), Qt::CaseInsensitive) < 0;
    } else {
        return componentHierarchy1.compare(componentHierarchy2, Qt::CaseInsensitive) < 0;
    }
}

//==============================================================================

QString CellmlFileRuntimeParameter::name() const
{
    // Return our name

    return mName;
}

//==============================================================================

int CellmlFileRuntimeParameter::degree() const
{
    // Return our degree

    return mDegree;
}

//==============================================================================

QString CellmlFileRuntimeParameter::unit() const
{
    // Return our unit

    return mUnit;
}

//==============================================================================

QStringList CellmlFileRuntimeParameter::componentHierarchy() const
{
    // Return our component hierarchy

    return mComponentHierarchy;
}

//==============================================================================

CellmlFileRuntimeParameter::ParameterType CellmlFileRuntimeParameter::type() const
{
    // Return our type

    return mType;
}

//==============================================================================

int CellmlFileRuntimeParameter::index() const
{
    // Return our index

    return mIndex;
}

//==============================================================================

QString CellmlFileRuntimeParameter::formattedName() const
{
    // Return a formatted version of our name

    return mName+QString(mDegree, '\'');
}

//==============================================================================

QString CellmlFileRuntimeParameter::formattedComponentHierarchy() const
{
    // Return a formatted version of our component hierarchy

    return mComponentHierarchy.join(".");
}

//==============================================================================

QString CellmlFileRuntimeParameter::fullyFormattedName() const
{
    // Return a fully formatted version of our name

    return formattedComponentHierarchy()+"."+formattedName();
}

//==============================================================================

QString CellmlFileRuntimeParameter::formattedUnit(const QString &pVoiUnit) const
{
    // Return a formatted version of our unit

    QString perVoiUnitDegree = QString();

    if (mDegree) {
        perVoiUnitDegree += "/"+pVoiUnit;

        if (mDegree > 1)
            perVoiUnitDegree += mDegree;
    }

    return mUnit+perVoiUnitDegree;
}

//==============================================================================

CellmlFileRuntime::CellmlFileRuntime(CellmlFile *pCellmlFile) :
    mCellmlFile(pCellmlFile),
    mOdeCodeInformation(0),
    mDaeCodeInformation(0),
    mConstantsCount(0),
    mStatesRatesCount(0),
    mAlgebraicCount(0),
    mCondVarCount(0),
    mCompilerEngine(0),
    mVariableOfIntegration(0),
    mParameters(CellmlFileRuntimeParameters())
{
    // Reset (initialise, here) our properties

    reset(true, false);
}

//==============================================================================

CellmlFileRuntime::~CellmlFileRuntime()
{
    // Reset our properties

    reset(false, true);
}

//==============================================================================

CellmlFile * CellmlFileRuntime::cellmlFile()
{
    // Return our CellML file

    return mCellmlFile;
}

//==============================================================================

QString CellmlFileRuntime::address() const
{
    // Return our address as a string

    return QString::number(qulonglong(this));
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

    return mModelType == CellmlFileRuntime::Ode;
}

//==============================================================================

bool CellmlFileRuntime::needDaeSolver() const
{
    // Return whether the model needs a DAE solver

    return mModelType == CellmlFileRuntime::Dae;
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

    return mConstantsCount;
}

//==============================================================================

int CellmlFileRuntime::statesCount() const
{
    // Return the number of states in the model

    return mStatesRatesCount;
}

//==============================================================================

int CellmlFileRuntime::ratesCount() const
{
    // Return the number of rates in the model

    return mStatesRatesCount;
}

//==============================================================================

int CellmlFileRuntime::algebraicCount() const
{
    // Return the number of algebraic equations in the model

    return mAlgebraicCount;
}

//==============================================================================

int CellmlFileRuntime::condVarCount() const
{
    // Return the number of conditional variables in the model

    return mCondVarCount;
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

CellmlFileRuntime::ComputeOdeRatesFunction CellmlFileRuntime::computeOdeRates() const
{
    // Return the computeOdeRates function

    return mComputeOdeRates;
}

//==============================================================================

CellmlFileRuntime::ComputeOdeVariablesFunction CellmlFileRuntime::computeOdeVariables() const
{
    // Return the computeOdeVariables function

    return mComputeOdeVariables;
}

//==============================================================================

CellmlFileRuntime::ComputeDaeEssentialVariablesFunction CellmlFileRuntime::computeDaeEssentialVariables() const
{
    // Return the computeDaeEssentialVariables function

    return mComputeDaeEssentialVariables;
}

//==============================================================================

CellmlFileRuntime::ComputeDaeResidualsFunction CellmlFileRuntime::computeDaeResiduals() const
{
    // Return the computeDaeResiduals function

    return mComputeDaeResiduals;
}

//==============================================================================

CellmlFileRuntime::ComputeDaeRootInformationFunction CellmlFileRuntime::computeDaeRootInformation() const
{
    // Return the computeDaeRootInformation function

    return mComputeDaeRootInformation;
}

//==============================================================================

CellmlFileRuntime::ComputeDaeStateInformationFunction CellmlFileRuntime::computeDaeStateInformation() const
{
    // Return the computeDaeStateInformation function

    return mComputeDaeStateInformation;
}

//==============================================================================

CellmlFileRuntime::ComputeDaeVariablesFunction CellmlFileRuntime::computeDaeVariables() const
{
    // Return the computeDaeVariables function

    return mComputeDaeVariables;
}

//==============================================================================

CellmlFileIssues CellmlFileRuntime::issues() const
{
    // Return the issue(s)

    return mIssues;
}

//==============================================================================

CellmlFileRuntimeParameters CellmlFileRuntime::parameters() const
{
    // Return the parameter(s)

    return mParameters;
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

void CellmlFileRuntime::resetFunctions()
{
    // Reset the functions

    mInitializeConstants = 0;

    mComputeComputedConstants = 0;

    mComputeOdeRates = 0;
    mComputeOdeVariables = 0;

    mComputeDaeEssentialVariables = 0;
    mComputeDaeResiduals = 0;
    mComputeDaeRootInformation = 0;
    mComputeDaeStateInformation = 0;
    mComputeDaeVariables = 0;
}

//==============================================================================

void CellmlFileRuntime::reset(const bool &pRecreateCompilerEngine,
                              const bool &pResetIssues)
{
    // Reset all of the runtime's properties

    mModelType = CellmlFileRuntime::Ode;
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

    if (!mParameters.contains(mVariableOfIntegration))
        delete mVariableOfIntegration;

    foreach (CellmlFileRuntimeParameter *parameter, mParameters)
        delete parameter;

    mVariableOfIntegration = 0;

    mParameters.clear();
}

//==============================================================================

void CellmlFileRuntime::couldNotGenerateModelCodeIssue(const QString &pExtraInfo)
{
    mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                               QObject::tr("the model code could not be generated (%1)").arg(pExtraInfo));
}

//==============================================================================

void CellmlFileRuntime::unknownProblemDuringModelCodeGenerationIssue()
{
    mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                               QObject::tr("an unknown problem occurred while trying to generate the model code"));
}

//==============================================================================

void CellmlFileRuntime::checkCodeInformation(iface::cellml_services::CodeInformation *pCodeInformation)
{
    Q_ASSERT(pCodeInformation);

    // Retrieve the code information's latest error message

    QString codeGenerationErrorMessage = QString::fromStdWString(pCodeInformation->errorMessage());

    if (codeGenerationErrorMessage.isEmpty()) {
        // The code generation went fine, so check the model's constraint level

        iface::cellml_services::ModelConstraintLevel constraintLevel = pCodeInformation->constraintLevel();

        if (constraintLevel == iface::cellml_services::UNDERCONSTRAINED) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       QObject::tr("the model is underconstrained (i.e. some variables need to be initialised or computed)"));
        } else if (constraintLevel == iface::cellml_services::UNSUITABLY_CONSTRAINED) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       QObject::tr("the model is unsuitably constrained (i.e. some variables could not be found and/or some equations could not be used)"));
        } else if (constraintLevel == iface::cellml_services::OVERCONSTRAINED) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       QObject::tr("the model is overconstrained (i.e. some variables are either both initialised and computed or computed more than once)"));
        }
    } else {
        mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                   QObject::tr("a problem occurred during the generation of the model code"));
    }
}

//==============================================================================

void CellmlFileRuntime::retrieveOdeCodeInformation(iface::cellml_api::Model *pModel)
{
    // Get a code generator bootstrap and create an ODE code generator

    ObjRef<iface::cellml_services::CodeGeneratorBootstrap> codeGeneratorBootstrap = CreateCodeGeneratorBootstrap();
    ObjRef<iface::cellml_services::CodeGenerator> codeGenerator = codeGeneratorBootstrap->createCodeGenerator();

    // Generate some code for the model

    try {
        mOdeCodeInformation = codeGenerator->generateCode(pModel);

        // Check that the code generation went fine

        checkCodeInformation(mOdeCodeInformation);
    } catch (iface::cellml_api::CellMLException &exception) {
        couldNotGenerateModelCodeIssue(Core::formatMessage(QString::fromStdWString(exception.explanation)));
    } catch (...) {
        unknownProblemDuringModelCodeGenerationIssue();
    }

    // Check the outcome of the ODE code generation

    if (mIssues.count())
        resetOdeCodeInformation();
}

//==============================================================================

void CellmlFileRuntime::retrieveDaeCodeInformation(iface::cellml_api::Model *pModel)
{
    // Get a code generator bootstrap and create a DAE code generator

    ObjRef<iface::cellml_services::CodeGeneratorBootstrap> codeGeneratorBootstrap = CreateCodeGeneratorBootstrap();
    ObjRef<iface::cellml_services::IDACodeGenerator> codeGenerator = codeGeneratorBootstrap->createIDACodeGenerator();

    // Generate some code for the model

    try {
        mDaeCodeInformation = codeGenerator->generateIDACode(pModel);

        // Check that the code generation went fine

        checkCodeInformation(mDaeCodeInformation);
    } catch (iface::cellml_api::CellMLException &exception) {
        couldNotGenerateModelCodeIssue(Core::formatMessage(QString::fromStdWString(exception.explanation)));
    } catch (...) {
        unknownProblemDuringModelCodeGenerationIssue();
    }

    // Check the outcome of the DAE code generation

    if (mIssues.count())
        resetDaeCodeInformation();
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

        if (pHasDefines) {
            res += "#define VOI 0.0\n"
                   "#define ALGEBRAIC 0\n"
                   "\n";
        }

        res += pFunctionBody;

        if (!pFunctionBody.endsWith("\n"))
            res += "\n";

        if (pHasDefines) {
            res += "\n"
                   "#undef ALGEBRAIC\n"
                   "#undef VOI\n";
        }

        res += "\n"
               "    return ret;\n";
    }

    res += "}\n";

    return res;
}

//==============================================================================

QStringList CellmlFileRuntime::componentHierarchy(iface::cellml_api::CellMLElement *pElement)
{
    // Make sure that we have a given element

    if (!pElement)
        return QStringList();

    // Try to retrieve the component that owns the given element, unless the
    // given element is a component itself (which will be the case when we come
    // here through recursion)

    ObjRef<iface::cellml_api::CellMLComponent> component = QueryInterface(pElement);
    ObjRef<iface::cellml_api::CellMLElement> parent = pElement->parentElement();
    ObjRef<iface::cellml_api::CellMLComponent> parentComponent = QueryInterface(parent);

    if (!component && !parentComponent) {
        // The element isn't a component and neither is its parent, so it
        // doesn't have a hierarchy

        return QStringList();
    }

    // Recursively retrieve the component hierarchy of the given element's
    // encapsulation parent, if any

    ObjRef<iface::cellml_api::CellMLComponent> componentEncapsulationParent = component?component->encapsulationParent():parentComponent->encapsulationParent();

    return componentHierarchy(componentEncapsulationParent) << QString::fromStdWString(component?component->name():parentComponent->name());
}

//==============================================================================

QString CellmlFileRuntime::cleanCode(const std::wstring &pCode)
{
    // Remove all the comments from the given code and return the resulting
    // cleaned up code

    static const QRegularExpression CommentRegEx = QRegularExpression("^/\\*.*\\*/$");

    QString res = QString();

    foreach (const QString &code, QString::fromStdWString(pCode).split("\r\n")) {
        if (!CommentRegEx.match(code).hasMatch())
            res += (res.isEmpty()?QString():"\n")+code;
    }

    return res;
}

//==============================================================================

void CellmlFileRuntime::update()
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
    //          type of a model, but there isn't...

    iface::cellml_api::Model *model = mCellmlFile->model();

    if (!model)
        return;

    // Retrieve the model's type
    // Note: this can be done by checking whether some equations were flagged
    //       as needing a Newton-Raphson evaluation...

    retrieveOdeCodeInformation(model);

    if (!mOdeCodeInformation)
        return;

    ObjRef<iface::mathml_dom::MathMLNodeList> flaggedEquations = mOdeCodeInformation->flaggedEquations();

    mModelType = flaggedEquations->length()?CellmlFileRuntime::Dae:CellmlFileRuntime::Ode;

    // If the model is of DAE type, then we don't want the ODE-specific code
    // information, but the DAE-specific code one

    ObjRef<iface::cellml_services::CodeInformation> genericCodeInformation;

    if (mModelType == CellmlFileRuntime::Ode) {
        genericCodeInformation = mOdeCodeInformation;
    } else {
        retrieveDaeCodeInformation(model);

        if (!mDaeCodeInformation)
            return;

        genericCodeInformation = mDaeCodeInformation;
    }

    // Retrieve the number of constants, states/rates, algebraic and conditional
    // variables in the model
    // Note: this is to avoid having to go through the ODE/DAE code information
    //       an unnecessary number of times when we want to retrieve either of
    //       those numbers (e.g. see
    //       SingleCellViewSimulationResults::addPoint())...

    if (mModelType == CellmlFileRuntime::Ode) {
        mConstantsCount = mOdeCodeInformation->constantIndexCount();
        mStatesRatesCount = mOdeCodeInformation->rateIndexCount();
        mAlgebraicCount = mOdeCodeInformation->algebraicIndexCount();
        mCondVarCount = 0;
    } else {
        mConstantsCount = mDaeCodeInformation->constantIndexCount();
        mStatesRatesCount = mDaeCodeInformation->rateIndexCount();
        mAlgebraicCount = mDaeCodeInformation->algebraicIndexCount();
        mCondVarCount = mDaeCodeInformation->conditionVariableCount();
    }

    // Go through the variables defined or referenced in our main CellML file
    // and do a mapping between the source of that variable and that variable
    // itself
    // Note: indeed, when it comes to (real) CellML 1.1 files (i.e. not CellML
    //       1.1 files that don't import any components), we only want to list
    //       the parameters that are either defined or referenced in our main
    //       CellML file. Not only does it make sense, but also only the
    //       parameters listed in a main CellML file can be referenced in
    //       SED-ML...

    QMap<iface::cellml_api::CellMLVariable *, iface::cellml_api::CellMLVariable *> mainVariables = QMap<iface::cellml_api::CellMLVariable *, iface::cellml_api::CellMLVariable *>();
    QList<iface::cellml_api::CellMLVariable *> realMainVariables = QList<iface::cellml_api::CellMLVariable *>();
    ObjRef<iface::cellml_api::CellMLComponentSet> localComponents = model->localComponents();
    ObjRef<iface::cellml_api::CellMLComponentIterator> localComponentsIter = localComponents->iterateComponents();

    for (ObjRef<iface::cellml_api::CellMLComponent> component = localComponentsIter->nextComponent();
         component; component = localComponentsIter->nextComponent()) {
        ObjRef<iface::cellml_api::CellMLVariableSet> variables = component->variables();
        ObjRef<iface::cellml_api::CellMLVariableIterator> variablesIter = variables->iterateVariables();

        for (ObjRef<iface::cellml_api::CellMLVariable> variable = variablesIter->nextVariable();
             variable; variable = variablesIter->nextVariable()) {
            ObjRef<iface::cellml_api::CellMLVariable> sourceVariable = variable->sourceVariable();

            mainVariables.insert(sourceVariable, variable);

            // In CellML 1.0 models / some CellML 1.1 models, the source
            // variable is / may be defined in the main CellML file and may be
            // used (and therefore referenced) in different places in that same
            // main CellML file, in which case we need to keep track of the real
            // main variable, which is the one which source variable is the same

            if (variable == sourceVariable)
                realMainVariables << variable;
        }
    }

    // Go through all our computation targets and determine which ones are
    // referenced in our main CellML file, and sort them by component and
    // variable name

    ObjRef<iface::cellml_services::ComputationTargetIterator> computationTargetIter = genericCodeInformation->iterateTargets();
    QString voiName = QString();
    QStringList voiComponentHierarchy = QStringList();

    for (ObjRef<iface::cellml_services::ComputationTarget> computationTarget = computationTargetIter->nextComputationTarget();
         computationTarget; computationTarget = computationTargetIter->nextComputationTarget()) {
        // Make sure that our computation target is defined or referenced in our
        // main CellML file, if it has imports

        ObjRef<iface::cellml_api::CellMLVariable> variable = computationTarget->variable();
        iface::cellml_api::CellMLVariable *mainVariable = realMainVariables.contains(variable)?
                                                              variable.getPointer():
                                                              mainVariables.value(variable);
        iface::cellml_api::CellMLVariable *realVariable = mainVariable?mainVariable:variable.getPointer();

        if (   !mainVariable
            &&  (computationTarget->type() != iface::cellml_services::VARIABLE_OF_INTEGRATION)) {
            continue;
        }

        // Determine the type of our computation target

        CellmlFileRuntimeParameter::ParameterType parameterType;

        switch (computationTarget->type()) {
        case iface::cellml_services::VARIABLE_OF_INTEGRATION:
            parameterType = CellmlFileRuntimeParameter::Voi;

            break;
        case iface::cellml_services::CONSTANT:
            // We are dealing with a constant, but the question is whether that
            // constant is a 'proper' constant, a 'computed' constant or even a
            // rate, and this can be determined by checking whether the computed
            // target has an initial value or even a degree
            // Note: a state variable that is initialised using the initial
            //       value of another variable will have its rate considered as
            //       a constant. However, when it comes to the GUI, we really
            //       want it to be seen as a rate hence we check for the degree
            //       of the computed target...

            if (QString::fromStdWString(variable->initialValue()).isEmpty()) {
                // The computed target doesn't have an initial value, so it must
                // be a 'computed' constant

                parameterType = CellmlFileRuntimeParameter::ComputedConstant;
            } else if (computationTarget->degree()) {
                // The computed target has a degree, so it is effectively a rate

                parameterType = CellmlFileRuntimeParameter::Rate;
            } else {
                // The computed target has an initial value, so it must be a
                // 'proper' constant

                parameterType = CellmlFileRuntimeParameter::Constant;
            }

            break;
        case iface::cellml_services::STATE_VARIABLE:
        case iface::cellml_services::PSEUDOSTATE_VARIABLE:
            parameterType = CellmlFileRuntimeParameter::State;

            break;
        case iface::cellml_services::ALGEBRAIC:
            // We are dealing with either a 'proper' algebraic variable or a
            // rate variable
            // Note: if the variable's degree is equal to zero, then we are
            //       dealing with a 'proper' algebraic variable otherwise we
            //       are dealing with a rate variable...

            if (computationTarget->degree())
                parameterType = CellmlFileRuntimeParameter::Rate;
            else
                parameterType = CellmlFileRuntimeParameter::Algebraic;

            break;
        case iface::cellml_services::FLOATING:
            parameterType = CellmlFileRuntimeParameter::Floating;

            break;
        case iface::cellml_services::LOCALLY_BOUND:
            parameterType = CellmlFileRuntimeParameter::LocallyBound;

            break;
        }

        // Keep track of our computation target, should its type be of interest

        if (   (parameterType != CellmlFileRuntimeParameter::Floating)
            && (parameterType != CellmlFileRuntimeParameter::LocallyBound)) {
            CellmlFileRuntimeParameter *parameter = new CellmlFileRuntimeParameter(QString::fromStdWString(realVariable->name()),
                                                                                   computationTarget->degree(),
                                                                                   QString::fromStdWString(realVariable->unitsName()),
                                                                                   componentHierarchy(realVariable),
                                                                                   parameterType,
                                                                                   computationTarget->assignedIndex());

            if (parameterType == CellmlFileRuntimeParameter::Voi) {
                if (!mVariableOfIntegration) {
                    mVariableOfIntegration = parameter;

                    voiName = parameter->name();
                    voiComponentHierarchy = parameter->componentHierarchy();
            } else if (   parameter->name().compare(voiName)
                           || (parameter->componentHierarchy() != voiComponentHierarchy)) {
                    // The CellML API wrongly validated a model that has more
                    // more than one variable of integration (at least,
                    // according to the CellML API), but this is clearly wrong
                    // (not to mention that it crashes OpenCOR), so let the user
                    // know about it
                    // Note: we check the name and component hierarchy of the
                    //       parameter against those of our current variable of
                    //       integration since the CellML API may generate
                    //       different targets that refer to the same CellML
                    //       variable (!?), as is for example the case with
                    //       [CellMLSupport]/tests/data/bond_graph_model_old.cellml...

                    mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                               QObject::tr("a model can have only one variable of integration"));
                }
            }

            if (realVariable == mainVariable)
                mParameters << parameter;
        }
    }

    std::sort(mParameters.begin(), mParameters.end(), CellmlFileRuntimeParameter::compare);

    // Generate the model code

    QString modelCode = QString();
    QString functionsString = QString::fromStdWString(genericCodeInformation->functionsString());

    if (!functionsString.isEmpty()) {
        // We will need to solve at least one NLA system

        mAtLeastOneNlaSystem = true;

        modelCode +=  "struct rootfind_info\n"
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
                      "extern void doNonLinearSolve(char *, void (*)(double *, double *, void*), double *, int *, int, void *);\n"
                      "\n"
                     +functionsString.replace("do_nonlinearsolve(", QString("doNonLinearSolve(\"%1\", ").arg(address()))
                     +"\n";

        // Note: we rename do_nonlinearsolve() to doNonLinearSolve() because
        //       CellML's CIS service already defines do_nonlinearsolve(), yet
        //       we want to use our own non-linear solve routine defined in our
        //       Compiler plugin. Also, we add a new parameter to all our calls
        //       to doNonLinearSolve() so that doNonLinearSolve() can retrieve
        //       the correct instance of our NLA solver...
    }

    // Retrieve the body of the function that initialises constants and extract
    // the statements that are related to computed variables (since we want to
    // be able to recompute those whenever the user modifies a parameter)
    // Note: ideally, we wouldn't have to do that, but the CellML API doesn't
    //       distinguish between 'proper' and 'computed' constants...
    //       (See https://tracker.physiomeproject.org/show_bug.cgi?id=3499)

    static const QRegularExpression InitializationStatementRegEx = QRegularExpression("^(CONSTANTS|RATES|STATES)\\[\\d*\\] = [+-]?\\d*\\.?\\d+([eE][+-]?\\d+)?;$");

    QStringList initConstsList = cleanCode(genericCodeInformation->initConstsString()).split("\n");
    QString initConsts = QString();
    QString compCompConsts = QString();

    foreach (const QString &initConst, initConstsList) {
        // Add the statement either to our list of 'proper' constants or
        // 'computed' constants

        if (InitializationStatementRegEx.match(initConst).hasMatch())
            initConsts += (initConsts.isEmpty()?QString():"\n")+initConst;
        else
            compCompConsts += (compCompConsts.isEmpty()?QString():"\n")+initConst;
    }

    modelCode += functionCode("int initializeConstants(double *CONSTANTS, double *RATES, double *STATES)",
                              initConsts, true);
    modelCode += "\n";
    modelCode += functionCode("int computeComputedConstants(double *CONSTANTS, double *RATES, double *STATES)",
                              compCompConsts, true);
    modelCode += "\n";

    // Retrieve the body of the remaining functions

    if (mModelType == CellmlFileRuntime::Ode) {
        modelCode += functionCode("int computeOdeRates(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC)",
                                  cleanCode(mOdeCodeInformation->ratesString()));
        modelCode += "\n";
        modelCode += functionCode("int computeOdeVariables(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC)",
                                  cleanCode(genericCodeInformation->variablesString()));
    } else {
        modelCode += functionCode("int computeDaeEssentialVariables(double VOI, double *CONSTANTS, double *RATES, double *OLDRATES, double *STATES, double *OLDSTATES, double *ALGEBRAIC, double *CONDVAR)",
                                  cleanCode(mDaeCodeInformation->essentialVariablesString()));
        modelCode += "\n";
        modelCode += functionCode("int computeDaeResiduals(double VOI, double *CONSTANTS, double *RATES, double *OLDRATES, double *STATES, double *OLDSTATES, double *ALGEBRAIC, double *CONDVAR, double *resid)",
                                  cleanCode(mDaeCodeInformation->ratesString()));
        modelCode += "\n";
        modelCode += functionCode("int computeDaeRootInformation(double VOI, double *CONSTANTS, double *RATES, double *OLDRATES, double *STATES, double *OLDSTATES, double *ALGEBRAIC, double *CONDVAR)",
                                  cleanCode(mDaeCodeInformation->rootInformationString()));
        modelCode += functionCode("int computeDaeStateInformation(double *SI)",
                                  cleanCode(mDaeCodeInformation->stateInformationString()));
        modelCode += "\n";
        modelCode += functionCode("int computeDaeVariables(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC, double *CONDVAR)",
                                  cleanCode(genericCodeInformation->variablesString()));
    }

    // Check whether the model code contains a definite integral, otherwise
    // compute it and check that everything went fine

    if (modelCode.contains("defint(func")) {
        mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                   QObject::tr("definite integrals are not yet supported"));
    } else if (!mCompilerEngine->compileCode(modelCode)) {
        mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                   mCompilerEngine->error());
    }

    // Keep track of the ODE/DAE functions, but only if no issues were reported

    if (mIssues.count()) {
        reset(true, false);
    } else {
        // Add the symbol of any required external function, if any

        if (mAtLeastOneNlaSystem)
            llvm::sys::DynamicLibrary::AddSymbol("doNonLinearSolve",
                                                 (void *) (intptr_t) doNonLinearSolve);

        // Retrieve the ODE/DAE functions

        mInitializeConstants = (InitializeConstantsFunction) (intptr_t) mCompilerEngine->getFunction("initializeConstants");

        mComputeComputedConstants = (ComputeComputedConstantsFunction) (intptr_t) mCompilerEngine->getFunction("computeComputedConstants");

        if (mModelType == CellmlFileRuntime::Ode) {
            mComputeOdeRates = (ComputeOdeRatesFunction) (intptr_t) mCompilerEngine->getFunction("computeOdeRates");
            mComputeOdeVariables = (ComputeOdeVariablesFunction) (intptr_t) mCompilerEngine->getFunction("computeOdeVariables");
        } else {
            mComputeDaeEssentialVariables = (ComputeDaeEssentialVariablesFunction) (intptr_t) mCompilerEngine->getFunction("computeDaeEssentialVariables");
            mComputeDaeResiduals = (ComputeDaeResidualsFunction) (intptr_t) mCompilerEngine->getFunction("computeDaeResiduals");
            mComputeDaeRootInformation = (ComputeDaeRootInformationFunction) (intptr_t) mCompilerEngine->getFunction("computeDaeRootInformation");
            mComputeDaeStateInformation = (ComputeDaeStateInformationFunction) (intptr_t) mCompilerEngine->getFunction("computeDaeStateInformation");
            mComputeDaeVariables = (ComputeDaeVariablesFunction) (intptr_t) mCompilerEngine->getFunction("computeDaeVariables");
        }

        // Make sure that we managed to retrieve all the ODE/DAE functions

        bool functionsOk =    mInitializeConstants
                           && mComputeComputedConstants;

        if (mModelType == CellmlFileRuntime::Ode) {
            functionsOk =    functionsOk
                          && mComputeOdeRates
                          && mComputeOdeVariables;
        } else {
            functionsOk =    functionsOk
                          && mComputeDaeEssentialVariables
                          && mComputeDaeResiduals
                          && mComputeDaeRootInformation
                          && mComputeDaeStateInformation
                          && mComputeDaeVariables;
        }

        if (!functionsOk) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Error,
                                       QObject::tr("an unexpected problem occurred while trying to retrieve the model functions"));

            reset(true, false);
        }
    }
}

//==============================================================================

CellmlFileRuntimeParameter *CellmlFileRuntime::variableOfIntegration() const
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
