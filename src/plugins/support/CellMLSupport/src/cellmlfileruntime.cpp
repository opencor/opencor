/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// CellML file runtime
//==============================================================================

#ifdef _WIN32
    #define NOMINMAX
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

#include "llvmclangbegin.h"
    #include "llvm/ADT/StringRef.h"
    #include "llvm/Support/DynamicLibrary.h"
#include "llvmclangend.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileRuntimeParameter::CellmlFileRuntimeParameter(const QString &pName,
                                                       int pDegree,
                                                       const QString &pUnit,
                                                       const QStringList &pComponentHierarchy,
                                                       Type pType,
                                                       int pIndex,
                                                       double *pData) :
    mName(pName),
    mDegree(pDegree),
    mUnit(pUnit),
    mComponentHierarchy(pComponentHierarchy),
    mType(pType),
    mIndex(pIndex),
    mData(pData)
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

    if (componentHierarchy1 == componentHierarchy2) {
        if (pParameter1->name() == pParameter2->name()) {
            return pParameter1->degree() < pParameter2->degree();
        }

        return pParameter1->name().compare(pParameter2->name(), Qt::CaseInsensitive) < 0;
    }

    return componentHierarchy1.compare(componentHierarchy2, Qt::CaseInsensitive) < 0;
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

CellmlFileRuntimeParameter::Type CellmlFileRuntimeParameter::type() const
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

double * CellmlFileRuntimeParameter::data() const
{
    // Return our array

    return mData;
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

    return mComponentHierarchy.join('.');
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

    if (mDegree != 0) {
        perVoiUnitDegree += "/"+pVoiUnit;

        if (mDegree > 1) {
            perVoiUnitDegree += "^"+QString::number(mDegree);
        }
    }

    return mUnit+perVoiUnitDegree;
}

//==============================================================================

QMap<int, QIcon> CellmlFileRuntimeParameter::icons()
{
    // Return the mapping between a parameter type and its corresponding icon

    static QMap<int, QIcon> Icons = QMap<int, QIcon>();

    static const QIcon VoiIcon              = QIcon(":/CellMLSupport/voi.png");
    static const QIcon ConstantIcon         = QIcon(":/CellMLSupport/constant.png");
    static const QIcon ComputedConstantIcon = QIcon(":/CellMLSupport/computedConstant.png");
    static const QIcon RateIcon             = QIcon(":/CellMLSupport/rate.png");
    static const QIcon StateIcon            = QIcon(":/CellMLSupport/state.png");
    static const QIcon AlgebraicIcon        = QIcon(":/CellMLSupport/algebraic.png");
    static const QIcon DataIcon             = QIcon(":/CellMLSupport/data.png");

    // Initialise the mapping, if needed

    if (Icons.isEmpty()) {
        Icons.insert(int(CellmlFileRuntimeParameter::Type::Voi), VoiIcon);
        Icons.insert(int(CellmlFileRuntimeParameter::Type::Constant), ConstantIcon);
        Icons.insert(int(CellmlFileRuntimeParameter::Type::ComputedConstant), ComputedConstantIcon);
        Icons.insert(int(CellmlFileRuntimeParameter::Type::Rate), RateIcon);
        Icons.insert(int(CellmlFileRuntimeParameter::Type::State), StateIcon);
        Icons.insert(int(CellmlFileRuntimeParameter::Type::Algebraic), AlgebraicIcon);
        Icons.insert(int(CellmlFileRuntimeParameter::Type::Data), DataIcon);
    }

    return Icons;
}

//==============================================================================

QIcon CellmlFileRuntimeParameter::icon(Type pParameterType)
{
    // Return our corresponding icon

    return icons().value(int(pParameterType));
}

//==============================================================================

CellmlFileRuntime::CellmlFileRuntime(CellmlFile *pCellmlFile)
{
    update(pCellmlFile);
}

//==============================================================================

CellmlFileRuntime::~CellmlFileRuntime()
{
    // Reset our properties

    try {
        reset(false, true, true);
    } catch (...) {
    }
}

//==============================================================================

void CellmlFileRuntime::update(CellmlFile *pCellmlFile, bool pAll)
{
    // Reset the runtime's properties

    reset(true, true, pAll);

    // Retrieve the CellML model associated with the CellML file

    iface::cellml_api::Model *model = pCellmlFile->model();

    if (model == nullptr) {
        return;
    }

    // Retrieve the code information for the model

    retrieveCodeInformation(model);

    if (mCodeInformation == nullptr) {
        return;
    }

    // Retrieve various additional things
    // Note: this should only ever be needed when we are called from our
    //       constructor...

    if (pAll) {
        // Retrieve the number of constants, states/rates, algebraic variables
        // in the model
        // Note: this is to avoid having to go through the code information an
        //       unnecessary number of times when we want to retrieve either of
        //       those numbers (e.g. see SimulationResults::addPoint())...

        mConstantsCount = int(mCodeInformation->constantIndexCount());
        mStatesRatesCount = int(mCodeInformation->rateIndexCount());
        mAlgebraicCount = int(mCodeInformation->algebraicIndexCount());

        // Go through the variables defined or referenced in our main CellML
        // file and do a mapping between the source of that variable and that
        // variable itself
        // Note: indeed, when it comes to (real) CellML 1.1 files (i.e. not
        //       CellML 1.1 files that don't import any components), we only
        //       want to list the parameters that are either defined or
        //       referenced in our main CellML file. Not only does it make
        //       sense, but also only the parameters listed in a main CellML
        //       file can be referenced in SED-ML...

        QMap<iface::cellml_api::CellMLVariable *, iface::cellml_api::CellMLVariable *> mainVariables = QMap<iface::cellml_api::CellMLVariable *, iface::cellml_api::CellMLVariable *>();
        QList<iface::cellml_api::CellMLVariable *> realMainVariables = QList<iface::cellml_api::CellMLVariable *>();
        ObjRef<iface::cellml_api::CellMLComponentSet> localComponents = model->localComponents();
        ObjRef<iface::cellml_api::CellMLComponentIterator> localComponentsIter = localComponents->iterateComponents();

        for (ObjRef<iface::cellml_api::CellMLComponent> component = localComponentsIter->nextComponent();
             component != nullptr; component = localComponentsIter->nextComponent()) {
            ObjRef<iface::cellml_api::CellMLVariableSet> variables = component->variables();
            ObjRef<iface::cellml_api::CellMLVariableIterator> variablesIter = variables->iterateVariables();

            for (ObjRef<iface::cellml_api::CellMLVariable> variable = variablesIter->nextVariable();
                 variable != nullptr; variable = variablesIter->nextVariable()) {
                ObjRef<iface::cellml_api::CellMLVariable> sourceVariable = variable->sourceVariable();

                mainVariables.insert(sourceVariable, variable);

                // In CellML 1.0 models / some CellML 1.1 models, the source
                // variable is / may be defined in the main CellML file and may
                // be used (and therefore referenced) in different places in
                // that same main CellML file, in which case we need to keep
                // track of the real main variable, which is the one which
                // source variable is the same

                if (variable == sourceVariable) {
                    realMainVariables << variable;
                }
            }
        }

        // Go through all our computation targets and determine which ones are
        // referenced in our main CellML file, and sort them by component and
        // variable name

        ObjRef<iface::cellml_services::ComputationTargetIterator> computationTargetIter = mCodeInformation->iterateTargets();
        QString voiName = QString();
        QStringList voiComponentHierarchy = QStringList();

        for (ObjRef<iface::cellml_services::ComputationTarget> computationTarget = computationTargetIter->nextComputationTarget();
             computationTarget != nullptr; computationTarget = computationTargetIter->nextComputationTarget()) {
            // Make sure that our computation target is defined or referenced in
            // our main CellML file, if it has imports

            ObjRef<iface::cellml_api::CellMLVariable> variable = computationTarget->variable();
            iface::cellml_api::CellMLVariable *mainVariable = realMainVariables.contains(variable)?
                                                                  variable.getPointer():
                                                                  mainVariables.value(variable);
            iface::cellml_api::CellMLVariable *realVariable = (mainVariable != nullptr)?mainVariable:variable.getPointer();

            if (   (mainVariable == nullptr)
                && (computationTarget->type() != iface::cellml_services::VARIABLE_OF_INTEGRATION)) {
                continue;
            }

            // Determine the type of our computation target

            CellmlFileRuntimeParameter::Type parameterType = CellmlFileRuntimeParameter::Type::Unknown;

            switch (computationTarget->type()) {
            case iface::cellml_services::VARIABLE_OF_INTEGRATION:
                parameterType = CellmlFileRuntimeParameter::Type::Voi;

                break;
            case iface::cellml_services::CONSTANT:
                // We are dealing with a constant, but the question is whether
                // that constant is a 'proper' constant, a 'computed' constant
                // or even a rate, and this can be determined by checking
                // whether the computed target has an initial value or even a
                // degree
                // Note: a state variable that is initialised using the initial
                //       value of another variable will have its rate considered
                //       as a constant. However, when it comes to the GUI, we
                //       really want it to be seen as a rate hence we check for
                //       the degree of the computed target...

                if (QString::fromStdWString(variable->initialValue()).isEmpty()) {
                    // The computed target doesn't have an initial value, so it
                    // must be a 'computed' constant

                    parameterType = CellmlFileRuntimeParameter::Type::ComputedConstant;
                } else if (computationTarget->degree() != 0) {
                    // The computed target has a degree, so it is effectively a
                    // rate

                    parameterType = CellmlFileRuntimeParameter::Type::Rate;
                } else {
                    // The computed target has an initial value, so it must be a
                    // 'proper' constant

                    parameterType = CellmlFileRuntimeParameter::Type::Constant;
                }

                break;
            case iface::cellml_services::STATE_VARIABLE:
            case iface::cellml_services::PSEUDOSTATE_VARIABLE:
                parameterType = CellmlFileRuntimeParameter::Type::State;

                break;
            case iface::cellml_services::ALGEBRAIC:
                // We are dealing with either a 'proper' algebraic variable or a
                // rate variable
                // Note: if the variable's degree is equal to zero, then we are
                //       dealing with a 'proper' algebraic variable otherwise we
                //       are dealing with a rate variable...

                if (computationTarget->degree() != 0) {
                    parameterType = CellmlFileRuntimeParameter::Type::Rate;
                } else {
                    parameterType = CellmlFileRuntimeParameter::Type::Algebraic;
                }

                break;
            case iface::cellml_services::FLOATING:
                parameterType = CellmlFileRuntimeParameter::Type::Floating;

                break;
            case iface::cellml_services::LOCALLY_BOUND:
                parameterType = CellmlFileRuntimeParameter::Type::LocallyBound;

                break;
            }

            // Keep track of our computation target, should its type be of
            // interest

            if (   (parameterType != CellmlFileRuntimeParameter::Type::Floating)
                && (parameterType != CellmlFileRuntimeParameter::Type::LocallyBound)) {
                auto parameter = new CellmlFileRuntimeParameter(QString::fromStdWString(realVariable->name()),
                                                                int(computationTarget->degree()),
                                                                QString::fromStdWString(realVariable->unitsName()),
                                                                componentHierarchy(realVariable),
                                                                parameterType,
                                                                int(computationTarget->assignedIndex()));

                if (parameterType == CellmlFileRuntimeParameter::Type::Voi) {
                    if (mVoi == nullptr) {
                        mVoi = parameter;

                        voiName = parameter->name();
                        voiComponentHierarchy = parameter->componentHierarchy();
                    } else if (   (parameter->name() != voiName)
                               || (parameter->componentHierarchy() != voiComponentHierarchy)) {
                        // The CellML API wrongly validated a model that has
                        // more than one VOI (at least, according to the CellML
                        // API), but this is clearly wrong (not to mention that
                        // it crashes OpenCOR), so let the user know about it
                        // Note: we check the name and component hierarchy of
                        //       the parameter against those of our current VOI
                        //       since the CellML API may generate different
                        //       targets that refer to the same CellML variable
                        //       (!?), as is for example the case with
                        //       [CellMLSupport]/tests/data/bond_graph_model_old.cellml...

                        mIssues << CellmlFileIssue(CellmlFileIssue::Type::Error,
                                                   tr("a model can have only one variable of integration"));
                    }
                }

                if (realVariable == mainVariable) {
                    mParameters << parameter;
                }
            }
        }

        std::sort(mParameters.begin(), mParameters.end(), CellmlFileRuntimeParameter::compare);
    }

    // Generate the model code

    QString modelCode = QString();
    QString functionsString = cleanCode(mCodeInformation->functionsString());

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
                      "};\n"
                      "\n"
                      "extern void doNonLinearSolve(char *, void (*)(double *, double *, void*), double *, int, void *);\n"
                      "\n"
                     +functionsString
                     +"\n";
    }

    // Retrieve the body of the function that initialises constants and extract
    // the statements that are related to computed variables (since we want to
    // be able to recompute those whenever the user modifies a parameter)
    // Note: ideally, we wouldn't have to do that, but the CellML API doesn't
    //       distinguish between 'proper' and 'computed' constants (see
    //       https://tracker.physiomeproject.org/show_bug.cgi?id=3499)...

    static const QRegularExpression InitializationStatementRegEx = QRegularExpression(R"(^(CONSTANTS|RATES|STATES)\[\d*\] = [+-]?\d*\.?\d+([eE][+-]?\d+)?;$)");

    QStringList initConstsList = cleanCode(mCodeInformation->initConstsString()).split('\n');
    QString initConsts = QString();
    QString compCompConsts = QString();

    for (const auto &initConst : initConstsList) {
        // Add the statement either to our list of 'proper' constants or
        // 'computed' constants

        if (InitializationStatementRegEx.match(initConst).hasMatch()) {
            initConsts += (initConsts.isEmpty()?QString():"\n")+initConst;
        } else {
            compCompConsts += (compCompConsts.isEmpty()?QString():"\n")+initConst;
        }
    }

    modelCode += methodCode("initializeConstants(double *CONSTANTS, double *RATES, double *STATES)",
                            initConsts);
    modelCode += methodCode("computeComputedConstants(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC)",
                            compCompConsts);
    modelCode += methodCode("computeVariables(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC, double *CONDVAR)",
                            mCodeInformation->variablesString());
    modelCode += methodCode("computeRates(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC)",
                            mCodeInformation->ratesString());

    // Check whether the model code contains a definite integral, otherwise
    // compute it and check that everything went fine

    if (modelCode.contains("defint(func")) {
        mIssues << CellmlFileIssue(CellmlFileIssue::Type::Error,
                                   tr("definite integrals are not yet supported"));
    } else if (!mCompilerEngine->compileCode(modelCode)) {
        mIssues << CellmlFileIssue(CellmlFileIssue::Type::Error,
                                   mCompilerEngine->error());
    }

    // Keep track of the ODE functions, but only if no issues were reported

    if (!mIssues.isEmpty()) {
        reset(true, false, true);
    } else {
        // Add the symbol of any required external function, if any

        if (mAtLeastOneNlaSystem) {
            llvm::sys::DynamicLibrary::AddSymbol("doNonLinearSolve",
                                                 reinterpret_cast<void *>(doNonLinearSolve));
        }

        // Retrieve the ODE functions

        mInitializeConstants = reinterpret_cast<InitializeConstantsFunction>(mCompilerEngine->getFunction("initializeConstants"));
        mComputeComputedConstants = reinterpret_cast<ComputeComputedConstantsFunction>(mCompilerEngine->getFunction("computeComputedConstants"));
        mComputeVariables = reinterpret_cast<ComputeVariablesFunction>(mCompilerEngine->getFunction("computeVariables"));
        mComputeRates = reinterpret_cast<ComputeRatesFunction>(mCompilerEngine->getFunction("computeRates"));

        // Make sure that we managed to retrieve all the ODE functions

        if (   (mInitializeConstants == nullptr) || (mComputeComputedConstants == nullptr)
            || (mComputeVariables == nullptr) || (mComputeRates == nullptr)) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Type::Error,
                                       tr("an unexpected problem occurred while trying to retrieve the model functions"));

            reset(true, false, true);
        }
    }
}

//==============================================================================

QString CellmlFileRuntime::address() const
{
    // Return our address as a string

    return QString::number(quint64(this));
}

//==============================================================================

bool CellmlFileRuntime::isValid() const
{
    // The runtime is valid if no issues were found

    return mIssues.isEmpty();
}

//==============================================================================

bool CellmlFileRuntime::needNlaSolver() const
{
    // Return whether the model needs an NLA solver

    return mAtLeastOneNlaSystem;
}

//==============================================================================

void CellmlFileRuntime::importData(const QString &pName,
                                   const QStringList &pComponentHierarchy,
                                   int pIndex, double *pData)
{
    mParameters << new CellmlFileRuntimeParameter(pName, 0, QString(),
                                                  pComponentHierarchy,
                                                  CellmlFileRuntimeParameter::Type::Data,
                                                  pIndex, pData);
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

CellmlFileRuntime::ComputeVariablesFunction CellmlFileRuntime::computeVariables() const
{
    // Return the computeVariables function

    return mComputeVariables;
}

//==============================================================================

CellmlFileRuntime::ComputeRatesFunction CellmlFileRuntime::computeRates() const
{
    // Return the computeRates function

    return mComputeRates;
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

CellmlFileRuntimeParameters CellmlFileRuntime::dataParameters(const double *pData) const
{
    // Return the data parameter(s)

    CellmlFileRuntimeParameters res = CellmlFileRuntimeParameters();

    for (auto parameter : mParameters) {
        if (   (parameter->type() == CellmlFileRuntimeParameter::Type::Data)
            && ((pData == nullptr) || (parameter->data() == pData))) {
            res << parameter;
        }
    }

    return res;
}

//==============================================================================

void CellmlFileRuntime::resetCodeInformation()
{
    // Reset the code information
    // Note: setting it to zero will automatically delete the current instance,
    //       if any

    mCodeInformation = nullptr;
}

//==============================================================================

void CellmlFileRuntime::resetFunctions()
{
    // Reset the functions

    mInitializeConstants = nullptr;
    mComputeComputedConstants = nullptr;
    mComputeVariables = nullptr;
    mComputeRates = nullptr;
}

//==============================================================================

void CellmlFileRuntime::reset(bool pRecreateCompilerEngine, bool pResetIssues,
                              bool pResetAll)
{
    // Reset all of the runtime's properties

    mAtLeastOneNlaSystem = false;

    resetCodeInformation();

    delete mCompilerEngine;

    if (pRecreateCompilerEngine) {
        mCompilerEngine = new Compiler::CompilerEngine();
    } else {
        mCompilerEngine = nullptr;
    }

    resetFunctions();

    if (pResetIssues) {
        mIssues.clear();
    }

    if (pResetAll) {
        if (!mParameters.contains(mVoi)) {
            delete mVoi;
        }

        for (auto parameter : mParameters) {
            delete parameter;
        }

        mVoi = nullptr;

        mParameters.clear();
    }
}

//==============================================================================

void CellmlFileRuntime::couldNotGenerateModelCodeIssue(const QString &pExtraInfo)
{
    mIssues << CellmlFileIssue(CellmlFileIssue::Type::Error,
                               tr("the model code could not be generated (%1)").arg(pExtraInfo));
}

//==============================================================================

void CellmlFileRuntime::unknownProblemDuringModelCodeGenerationIssue()
{
    mIssues << CellmlFileIssue(CellmlFileIssue::Type::Error,
                               tr("an unknown problem occurred while trying to generate the model code"));
}

//==============================================================================

void CellmlFileRuntime::checkCodeInformation(iface::cellml_services::CodeInformation *pCodeInformation)
{
    Q_ASSERT(pCodeInformation);

    // Retrieve the code information's latest error message

    QString errorMessage = QString::fromStdWString(pCodeInformation->errorMessage());

    if (errorMessage.isEmpty()) {
        // The code generation went fine, so check the model's constraint level

        iface::cellml_services::ModelConstraintLevel constraintLevel = pCodeInformation->constraintLevel();

        if (constraintLevel == iface::cellml_services::UNDERCONSTRAINED) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Type::Error,
                                       tr("the model is underconstrained (i.e. some variables need to be initialised or computed)"));
        } else if (constraintLevel == iface::cellml_services::UNSUITABLY_CONSTRAINED) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Type::Error,
                                       tr("the model is unsuitably constrained (i.e. some variables could not be found and/or some equations could not be used)"));
        } else if (constraintLevel == iface::cellml_services::OVERCONSTRAINED) {
            mIssues << CellmlFileIssue(CellmlFileIssue::Type::Error,
                                       tr("the model is overconstrained (i.e. some variables are either both initialised and computed or computed more than once)"));
        }
    } else {
        mIssues << CellmlFileIssue(CellmlFileIssue::Type::Error,
                                   tr("a problem occurred during the generation of the model code (%1)").arg(Core::formatMessage(errorMessage)));
    }
}

//==============================================================================

void CellmlFileRuntime::retrieveCodeInformation(iface::cellml_api::Model *pModel)
{
    // Get a code generator bootstrap and create a code generator

    ObjRef<iface::cellml_services::CodeGeneratorBootstrap> codeGeneratorBootstrap = CreateCodeGeneratorBootstrap();
    ObjRef<iface::cellml_services::CodeGenerator> codeGenerator = codeGeneratorBootstrap->createCodeGenerator();

    // Generate some code for the model

    try {
        mCodeInformation = codeGenerator->generateCode(pModel);

        // Check that the code generation went fine

        checkCodeInformation(mCodeInformation);
    } catch (iface::cellml_api::CellMLException &exception) {
        couldNotGenerateModelCodeIssue(Core::formatMessage(QString::fromStdWString(exception.explanation)));
    } catch (...) {
        unknownProblemDuringModelCodeGenerationIssue();
    }

    // Check the outcome of the code generation

    if (!mIssues.isEmpty()) {
        resetCodeInformation();
    }
}

//==============================================================================

QString CellmlFileRuntime::methodCode(const QString &pCodeSignature,
                                      const QString &pCodeBody)
{
    // Generate and return the code for the given method

    QString res = "void "+pCodeSignature+"\n"
                  "{\n";

    if (!pCodeBody.isEmpty()) {
        res += pCodeBody;

        if (!pCodeBody.endsWith('\n')) {
            res += '\n';
        }
    }

    res += "}\n\n";

    return res;
}

//==============================================================================

QString CellmlFileRuntime::methodCode(const QString &pCodeSignature,
                                      const std::wstring &pCodeBody)
{
    // Generate and return the code for the given method

    return methodCode(pCodeSignature, cleanCode(pCodeBody));
}

//==============================================================================

QStringList CellmlFileRuntime::componentHierarchy(iface::cellml_api::CellMLElement *pElement)
{
    // Make sure that we have a given element

    if (pElement == nullptr) {
        return {};
    }

    // Try to retrieve the component that owns the given element, unless the
    // given element is a component itself (which will be the case when we come
    // here through recursion)

    ObjRef<iface::cellml_api::CellMLComponent> component = QueryInterface(pElement);
    ObjRef<iface::cellml_api::CellMLElement> parent = pElement->parentElement();
    ObjRef<iface::cellml_api::CellMLComponent> parentComponent = QueryInterface(parent);

    if ((component == nullptr) && (parentComponent == nullptr)) {
        // The element isn't a component and neither is its parent, so it
        // doesn't have a hierarchy

        return {};
    }

    // Recursively retrieve the component hierarchy of the given element's
    // encapsulation parent, if any

    ObjRef<iface::cellml_api::CellMLComponent> componentEncapsulationParent = (component != nullptr)?component->encapsulationParent():parentComponent->encapsulationParent();

    return componentHierarchy(componentEncapsulationParent) << QString::fromStdWString((component != nullptr)?component->name():parentComponent->name());
}

//==============================================================================

QString CellmlFileRuntime::cleanCode(const std::wstring &pCode)
{
    // Remove all the comments from the given code and return the resulting
    // cleaned up code

    static const QRegularExpression CommentRegEx = QRegularExpression("^/\\*.*\\*/$");

    QString res = QString();

    for (const auto &code : QString::fromStdWString(pCode).split("\r\n")) {
        if (!CommentRegEx.match(code.trimmed()).hasMatch()) {
            res += (res.isEmpty()?QString():"\n")+code;
        }
    }

    // Further clean up the given code by removing any reference to a 'return'
    // value)

    res.remove(", int* pret");
    res.remove(", pret");

    res.remove("#define pret rfi->aPRET\n");
    res.remove("#undef pret\n");
    res.remove("  rfi.aPRET = pret;\n");

    // Also rename do_nonlinearsolve() to doNonLinearSolve() since CellML's CIS
    // service already defines do_nonlinearsolve() and, yet, we want to use our
    // own non-linear solve routine defined in our Solver interface, and add a
    // new parameter to all our calls to doNonLinearSolve() so that
    // doNonLinearSolve() can retrieve the correct instance of our NLA solver

    res.replace("do_nonlinearsolve(", QString(R"(doNonLinearSolve("%1", )").arg(address()));

    return res;
}

//==============================================================================

CellmlFileRuntimeParameter *CellmlFileRuntime::voi() const
{
    // Return our VOI, if any

    return mVoi;
}

//==============================================================================

} // namespace CellMLSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
