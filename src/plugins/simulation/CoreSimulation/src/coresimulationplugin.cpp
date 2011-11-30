//==============================================================================
// CoreSimulation plugin
//==============================================================================

#include "coresimulationplugin.h"
#include "coreutils.h"

//==============================================================================

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMainWindow>
#include <QPen>
#include <QThread>
#include <QUrl>

//==============================================================================

#include "cellml-api-cxx-support.hpp"
#include "CellMLBootstrap.hpp"
#include "IfaceCellML_APISPEC.hxx"
#include "IfaceCIS.hxx"
#include "CISBootstrap.hpp"

//==============================================================================

#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Constants.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Instructions.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/Target/TargetSelect.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/raw_ostream.h"

//==============================================================================

#include "cvode/cvode.h"
#include "cvode/cvode_dense.h"

#include <ida/ida.h>
#include <ida/ida_dense.h>

#include "nvector/nvector_serial.h"
#include "sundials/sundials_dense.h"
#include "sundials/sundials_types.h"

//==============================================================================

#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_grid.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSimulation {

//==============================================================================

PLUGININFO_FUNC CoreSimulationPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "The core simulation plugin for OpenCOR");
    descriptions.insert("fr", "L'extension de simulation de base pour OpenCOR");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::Gui,
                      PluginInfo::Simulation,
                      false,
                      QStringList() << "Core" << "LLVM" << "Qwt",
                      descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(CoreSimulation, CoreSimulationPlugin)

//==============================================================================

class SleeperThread : public QThread
{
public:
    static void msleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};

//==============================================================================

class TestProgressObserver : public iface::cellml_services::IntegrationProgressObserver
{
public:
    TestProgressObserver(iface::cellml_services::CellMLCompiledModel *pCompiledModel) :
        mCompiledModel(pCompiledModel),
        mCodeInformation(pCompiledModel->codeInformation()),
        mRefCount(1),
        mFinished(false)
    {
        // Determine the header for the results

        iface::cellml_services::ComputationTargetIterator *cti = mCodeInformation->iterateTargets();

        bool first = true;

        QString header = QString();

        while (true) {
            iface::cellml_services::ComputationTarget* ct = cti->nextComputationTarget();

            if (!ct)
                break;

            if (     ((ct->type() == iface::cellml_services::STATE_VARIABLE)
                   || (ct->type() == iface::cellml_services::ALGEBRAIC)
                   || (ct->type() == iface::cellml_services::VARIABLE_OF_INTEGRATION))
                && (ct->degree() == 0)) {
                if (!first)
                    header += ",";
                else
                    first = false;

                header += QString::fromWCharArray(ct->variable()->name());
            }
        }

        // Output the header to our results file

        mResultsFile.setFileName(QDir::tempPath()+QDir::separator()+"test_cellml_results.csv");

        mResultsFile.remove();   // Just in case...

        mResultsFile.open( QIODevice::WriteOnly);

        writeResults(header);
    }

    //==========================================================================

    virtual void add_ref() throw(std::exception &)
    {
        ++mRefCount;
    }

    //==========================================================================

    virtual void release_ref() throw(std::exception &)
    {
        --mRefCount;

        if (!mRefCount)
            delete this;
    }

    //==========================================================================

    virtual char * objid() throw (std::exception &)
    {
        return strdup("singletonTestProgressObserver");
    }

    //==========================================================================

    virtual void * query_interface(const char *pIface) throw (std::exception &)
    {
        if (!strcmp(pIface, "xpcom::IObject"))
            return static_cast<iface::XPCOM::IObject *>(this);
        else if (!strcmp(pIface, "cellml_services::IntegrationProgressObserver"))
            return static_cast<iface::cellml_services::IntegrationProgressObserver *> (this);

        return 0;
    }

    //==========================================================================

    virtual void computedConstants(uint32_t, double *) throw (std::exception &)
    {
        // Do nothing...
    }

    //==========================================================================

    virtual void results(uint32_t pNbOfValues, double *pValues) throw (std::exception&)
    {
        uint32_t rateIndexCount = mCodeInformation->rateIndexCount();
        uint32_t recSize =  2*rateIndexCount
                           +mCodeInformation->algebraicIndexCount()
                           +1;

        if (!recSize)
            return;

        for (uint32_t i = 0; i < pNbOfValues; i += recSize)
        {
            bool first = true;

            iface::cellml_services::ComputationTargetIterator *targetsIter = mCodeInformation->iterateTargets();

            QString values = QString();

            while (true)
            {
                iface::cellml_services::ComputationTarget *target = targetsIter->nextComputationTarget();

                if (!target)
                    break;

                if (target->degree())
                    continue;

                uint32_t varOff = 0;

                switch (target->type()) {
                case iface::cellml_services::STATE_VARIABLE:
                    varOff = 1+target->assignedIndex();

                    break;
                case iface::cellml_services::VARIABLE_OF_INTEGRATION:
                    varOff = 0;

                    break;
                case iface::cellml_services::ALGEBRAIC:
                    varOff = 1+2*rateIndexCount+target->assignedIndex();

                    break;
                default:
                    continue;
                }

                if (!first)
                    values += ",";
                else
                    first = false;

                values += QString::number(pValues[i+varOff]);
            }

            writeResults(values);
        }
    }

    //==========================================================================

    virtual void done() throw (std::exception &)
    {
        mFinished = true;
    }

    //==========================================================================

    virtual void failed(const char *pErrorMsg) throw (std::exception &)
    {
        qDebug("CIS error: Model integration failed (%s)", pErrorMsg);

        mFinished = true;
    }

    //==========================================================================

    bool finished()
    {
        return mFinished;
    }

    //==========================================================================

    void writeResults(const QString &pResults)
    {
        mResultsFile.write((pResults+"\n").toLatin1().constData());
        mResultsFile.flush();
    }

private:
    iface::cellml_services::CellMLCompiledModel *mCompiledModel;
    iface::cellml_services::CodeInformation *mCodeInformation;

    uint32_t mRefCount;
    bool mFinished;

    QFile mResultsFile;
};

//==============================================================================

void CoreSimulationPlugin::testCellml()
{
    // Testing the CellML API

    qDebug() << "================================================================================";
    qDebug();
    qDebug() << "Testing CellML";
    qDebug() << "--------------";
    qDebug();

    // Fetch a bootstrap object

    iface::cellml_api::CellMLBootstrap *cbs = CreateCellMLBootstrap();

    // Retrieve the model loader

    iface::cellml_api::DOMModelLoader *ml = cbs->modelLoader();

    // Load our test CellML model
    // Note: we do this within a try...catch statement since we might get an
    //       exception...

    QString testCellmlModelFileName = QDir::tempPath()+QDir::separator()+"test_cellml_model.cellml";

    Core::saveResourceAs(":test_cellml_model", testCellmlModelFileName);

    iface::cellml_api::Model *model;

    try {
        model = ml->loadFromURL(QUrl::fromLocalFile(testCellmlModelFileName).toString().toStdWString().c_str());
    } catch (iface::cellml_api::CellMLException &) {
        qDebug("Model loader error: %s", QString::fromWCharArray(ml->lastErrorMessage()).toLatin1().constData());

        usePrecomputedTestCellmlResults();

        return;
    }

    // Run the model

    iface::cellml_services::CellMLIntegrationService* cis = CreateIntegrationService();
    iface::cellml_services::ODESolverCompiledModel* compiledModel = 0;

    try
    {
        compiledModel = cis->compileModelODE(model);
    } catch (iface::cellml_api::CellMLException &) {
        qDebug("CIS error: %s", QString::fromWCharArray(cis->lastError()).toLatin1().constData());

        usePrecomputedTestCellmlResults();

        return;
    } catch (...) {
        qDebug("CIS error: Unexpected exception calling compileModel.");

        usePrecomputedTestCellmlResults();

        return;
    }

    qDebug("CIS information: The model was properly compiled and is now going to be run.");

    iface::cellml_services::ODESolverRun *odeSolverRun = cis->createODEIntegrationRun(compiledModel);

    TestProgressObserver* progressObserver = new TestProgressObserver(compiledModel);

    odeSolverRun->setProgressObserver(progressObserver);

    odeSolverRun->stepType(iface::cellml_services::BDF_IMPLICIT_1_5_SOLVE);
    odeSolverRun->setResultRange(0, 50, 1000);

    odeSolverRun->start();

    while (!progressObserver->finished())
        SleeperThread::msleep(1);

    QFile::remove(testCellmlModelFileName);

    qDebug();
}

//==============================================================================

void CoreSimulationPlugin::usePrecomputedTestCellmlResults()
{
    // Use the pre-computed test CellML results
    // Note: this is done as a result of not being able to use the CellML API
    //       binaries (most likely because we couldn't compile our test CellML
    //       file)

    Core::saveResourceAs(":test_cellml_results",
                         QDir::tempPath()+QDir::separator()+"test_cellml_results.csv");
}

//==============================================================================

void CoreSimulationPlugin::testLlvmJit()
{
    // Testing the JIT compilation side of LLVM
    // Note: this is a shameless copying/pasting of the How-to-use JIT
    //       example...

    qDebug() << "================================================================================";
    qDebug();
    qDebug() << "Testing LLVM-JIT";
    qDebug() << "----------------";
    qDebug();

#ifndef Q_WS_WIN
    // Note #1: for some reasons, to call InitializeNativeTarget on Windows
    //          results in an error message being displayed when closing OpenCOR
    //          while not calling it results in OpenCOR closeing 'properly',
    //          so...
    // Note #2: InitializeNativeTarget should normally be called, so why does it
    //          work when not calling it on Windows...?!

    llvm::InitializeNativeTarget();
#endif

    llvm::LLVMContext context;

    // Create a module

    llvm::Module *module = new llvm::Module("test", context);

    // Create a function entry and add it to our module
    // Note: the function has a return type of int and takes an argument of int
    //       while the 0 argument terminates the list of argument types...

    llvm::Function *addOneFunc = llvm::cast<llvm::Function>(module->getOrInsertFunction("addOne",
                                                                                        llvm::Type::getInt32Ty(context),
                                                                                        llvm::Type::getInt32Ty(context),
                                                                                        (llvm::Type *) 0));

    // Add a basic block to addOneFunc

    llvm::BasicBlock *basicBlock = llvm::BasicBlock::Create(context,
                                                            "entryBlock",
                                                            addOneFunc);

    // Get a pointer to the constant 1

    llvm::Value *one = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context),
                                              1);

    // Get a pointer to addOne's integer argument

    llvm::Argument *argX = addOneFunc->arg_begin();

    // Create an add instruction and add it to our basic block

    llvm::Instruction *add = llvm::BinaryOperator::CreateAdd(one, argX,
                                                             "addResult",
                                                             basicBlock);

    // Create a return instruction and add it to our basic block
    // Note: after this, addOneFunc is ready to be used...

    llvm::ReturnInst::Create(context, add, basicBlock);

    // Create another function entry and add it to our module
    // Note: this time, the function returns an int and takes no arguments

    llvm::Function *fooFunc = llvm::cast<llvm::Function>(module->getOrInsertFunction("foo",
                                                                                     llvm::Type::getInt32Ty(context),
                                                                                     (llvm::Type *) 0));

    // Add a basic block to fooFunc

    basicBlock = llvm::BasicBlock::Create(context, "entryBlock", fooFunc);

    // Get a pointer to the constant 10

    llvm::Value *ten = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context),
                                              10);

    // Pass ten to fooFunc

    llvm::CallInst *addOneCallResult = llvm::CallInst::Create(addOneFunc,
                                                              ten,
                                                              "addOne",
                                                              basicBlock);

    // Create a return instruction and add it to our basic block

    llvm::ReturnInst::Create(context, addOneCallResult, basicBlock);

    // Create the JIT

    llvm::ExecutionEngine *executionEngine = llvm::EngineBuilder(module).create();

    llvm::errs() << "We just constructed this LLVM module:\n\n" << *module;
    llvm::errs() << "\nRunning foo: ";
    llvm::errs().flush();

    // Call fooFunc with no arguments

    std::vector<llvm::GenericValue> noargs;
    llvm::GenericValue genericValue = executionEngine->runFunction(fooFunc,
                                                                   noargs);

    // Output the result of the execution

    llvm::errs() << "Result: " << genericValue.IntVal << "\n";
    llvm::errs().flush();

    // Finish things off

    executionEngine->freeMachineCodeForFunction(fooFunc);

    delete executionEngine;

    llvm::llvm_shutdown();

    qDebug();
}

//==============================================================================

#define Ith(v,i)    NV_Ith_S(v,i-1)       /* Ith numbers components 1..NEQ */
#define IJth(A,i,j) DENSE_ELEM(A,i-1,j-1) /* IJth numbers rows,cols 1..NEQ */

//==============================================================================

/*
 * f routine. Compute function f(t,y).
 */

static int f(realtype t, N_Vector y, N_Vector ydot, void *user_data)
{
    realtype y1, y2, y3, yd1, yd3;

    y1 = Ith(y,1); y2 = Ith(y,2); y3 = Ith(y,3);

    yd1 = Ith(ydot,1) = RCONST(-0.04)*y1 + RCONST(1.0e4)*y2*y3;
    yd3 = Ith(ydot,3) = RCONST(3.0e7)*y2*y2;
          Ith(ydot,2) = -yd1 - yd3;

    return(0);
}

//==============================================================================

/*
 * g routine. Compute functions g_i(t,y) for i = 0,1.
 */

static int g(realtype t, N_Vector y, realtype *gout, void *user_data)
{
    realtype y1, y3;

    y1 = Ith(y,1); y3 = Ith(y,3);
    gout[0] = y1 - RCONST(0.0001);
    gout[1] = y3 - RCONST(0.01);

    return(0);
}

//==============================================================================

/*
 * Jacobian routine. Compute J(t,y) = df/dy. *
 */

static int Jac(int N, realtype t,
               N_Vector y, N_Vector fy, DlsMat J, void *user_data,
               N_Vector tmp1, N_Vector tmp2, N_Vector tmp3)
{
    realtype y1, y2, y3;

    y1 = Ith(y,1); y2 = Ith(y,2); y3 = Ith(y,3);

    IJth(J,1,1) = RCONST(-0.04);
    IJth(J,1,2) = RCONST(1.0e4)*y3;
    IJth(J,1,3) = RCONST(1.0e4)*y2;
    IJth(J,2,1) = RCONST(0.04);
    IJth(J,2,2) = RCONST(-1.0e4)*y3-RCONST(6.0e7)*y2;
    IJth(J,2,3) = RCONST(-1.0e4)*y2;
    IJth(J,3,2) = RCONST(6.0e7)*y2;

    return(0);
}

//==============================================================================

/*
 *-------------------------------
 * Private helper functions
 *-------------------------------
 */

static void PrintCvodeOutput(realtype t, realtype y1, realtype y2, realtype y3)
{
#if defined(SUNDIALS_EXTENDED_PRECISION)
    qDebug("At t = %0.4Le      y =%14.6Le  %14.6Le  %14.6Le", t, y1, y2, y3);
#elif defined(SUNDIALS_DOUBLE_PRECISION)
    qDebug("At t = %0.4le      y =%14.6le  %14.6le  %14.6le", t, y1, y2, y3);
#else
    qDebug("At t = %0.4e      y =%14.6e  %14.6e  %14.6e", t, y1, y2, y3);
#endif

  return;
}

static void PrintCvodeRootInfo(int root_f1, int root_f2)
{
    qDebug("    rootsfound[] = %3d %3d", root_f1, root_f2);

    return;
}

//==============================================================================

/*
 * Check function return value...
 *   opt == 0 means SUNDIALS function allocates memory so check if
 *            returned NULL pointer
 *   opt == 1 means SUNDIALS function returns a flag so check if
 *            flag >= 0
 *   opt == 2 means function allocates memory so check if returned
 *            NULL pointer
 */

static int check_flag(void *flagvalue, char *funcname, int opt)
{
    int *errflag;

    /* Check if SUNDIALS function returned NULL pointer - no memory allocated */
    if (opt == 0 && flagvalue == NULL) {
        qDebug("SUNDIALS_ERROR: %s() failed - returned NULL pointer",
               funcname);
        return(1); }

    /* Check if flag < 0 */
    else if (opt == 1) {
        errflag = (int *) flagvalue;
        if (*errflag < 0) {
            qDebug("SUNDIALS_ERROR: %s() failed with flag = %d",
                   funcname, *errflag);
        return(1); }}

    /* Check if function returned NULL pointer - no memory allocated */
    else if (opt == 2 && flagvalue == NULL) {
        qDebug("MEMORY_ERROR: %s() failed - returned NULL pointer",
               funcname);
        return(1); }

    return(0);
}

//==============================================================================

/*
 * Get and print some final statistics
 */

static void PrintFinalCvodeStats(void *cvode_mem)
{
    long int nst, nfe, nsetups, nje, nfeLS, nni, ncfn, netf, nge;
    int flag;

    flag = CVodeGetNumSteps(cvode_mem, &nst);
    check_flag(&flag, (char *) "CVodeGetNumSteps", 1);
    flag = CVodeGetNumRhsEvals(cvode_mem, &nfe);
    check_flag(&flag, (char *) "CVodeGetNumRhsEvals", 1);
    flag = CVodeGetNumLinSolvSetups(cvode_mem, &nsetups);
    check_flag(&flag, (char *) "CVodeGetNumLinSolvSetups", 1);
    flag = CVodeGetNumErrTestFails(cvode_mem, &netf);
    check_flag(&flag, (char *) "CVodeGetNumErrTestFails", 1);
    flag = CVodeGetNumNonlinSolvIters(cvode_mem, &nni);
    check_flag(&flag, (char *) "CVodeGetNumNonlinSolvIters", 1);
    flag = CVodeGetNumNonlinSolvConvFails(cvode_mem, &ncfn);
    check_flag(&flag, (char *) "CVodeGetNumNonlinSolvConvFails", 1);

    flag = CVDlsGetNumJacEvals(cvode_mem, &nje);
    check_flag(&flag, (char *) "CVDlsGetNumJacEvals", 1);
    flag = CVDlsGetNumRhsEvals(cvode_mem, &nfeLS);
    check_flag(&flag, (char *) "CVDlsGetNumRhsEvals", 1);

    flag = CVodeGetNumGEvals(cvode_mem, &nge);
    check_flag(&flag, (char *) "CVodeGetNumGEvals", 1);

    qDebug();
    qDebug("Final Statistics:");
    qDebug("nst = %-6ld nfe  = %-6ld nsetups = %-6ld nfeLS = %-6ld nje = %ld",
           nst, nfe, nsetups, nfeLS, nje);
    qDebug("nni = %-6ld ncfn = %-6ld netf = %-6ld nge = %ld",
           nni, ncfn, netf, nge);
}

//==============================================================================

void CoreSimulationPlugin::testCvode()
{
    // Testing CVODE
    // Note: this is a shameless copying/pasting of the cvRoberts_dns.c code...

    qDebug() << "================================================================================";
    qDebug();
    qDebug() << "Testing CVODE";
    qDebug() << "----------------";
    qDebug();

    /* Problem Constants */

    #define NEQ   3                /* number of equations  */
    #define Y1    RCONST(1.0)      /* initial y components */
    #define Y2    RCONST(0.0)
    #define Y3    RCONST(0.0)
    #define RTOL  RCONST(1.0e-4)   /* scalar relative tolerance            */
    #define ATOL1 RCONST(1.0e-8)   /* vector absolute tolerance components */
    #define ATOL2 RCONST(1.0e-14)
    #define ATOL3 RCONST(1.0e-6)
    #define T0    RCONST(0.0)      /* initial time           */
    #define T1    RCONST(0.4)      /* first output time      */
    #define TMULT RCONST(10.0)     /* output time factor     */
    #define NOUT  12               /* number of output times */

    /*
     * The following is a simple example problem, with the coding
     * needed for its solution by CVODE. The problem is from
     * chemical kinetics, and consists of the following three rate
     * equations:
     *    dy1/dt = -.04*y1 + 1.e4*y2*y3
     *    dy2/dt = .04*y1 - 1.e4*y2*y3 - 3.e7*(y2)^2
     *    dy3/dt = 3.e7*(y2)^2
     * on the interval from t = 0.0 to t = 4.e10, with initial
     * conditions: y1 = 1.0, y2 = y3 = 0. The problem is stiff.
     * While integrating the system, we also use the rootfinding
     * feature to find the points at which y1 = 1e-4 or at which
     * y3 = 0.01. This program solves the problem with the BDF method,
     * Newton iteration with the CVDENSE dense linear solver, and a
     * user-supplied Jacobian routine.
     * It uses a scalar relative tolerance and a vector absolute
     * tolerance. Output is printed in decades from t = .4 to t = 4.e10.
     * Run statistics (optional outputs) are printed at the end.
     */

    realtype reltol, t, tout;
    N_Vector y, abstol;
    void *cvode_mem;
    int flag, flagr, iout;
    int rootsfound[2];

    y = abstol = NULL;
    cvode_mem = NULL;

    /* Create serial vector of length NEQ for I.C. and abstol */
    y = N_VNew_Serial(NEQ);
    if (check_flag((void *)y, (char *) "N_VNew_Serial", 0)) return;
    abstol = N_VNew_Serial(NEQ);
    if (check_flag((void *)abstol, (char *) "N_VNew_Serial", 0)) return;

    /* Initialize y */
    Ith(y,1) = Y1;
    Ith(y,2) = Y2;
    Ith(y,3) = Y3;

    /* Set the scalar relative tolerance */
    reltol = RTOL;
    /* Set the vector absolute tolerance */
    Ith(abstol,1) = ATOL1;
    Ith(abstol,2) = ATOL2;
    Ith(abstol,3) = ATOL3;

    /* Call CVodeCreate to create the solver memory and specify the
     * Backward Differentiation Formula and the use of a Newton iteration */
    cvode_mem = CVodeCreate(CV_BDF, CV_NEWTON);
    if (check_flag((void *)cvode_mem, (char *) "CVodeCreate", 0)) return;

    /* Call CVodeInit to initialize the integrator memory and specify the
     * user's right hand side function in y'=f(t,y), the inital time T0, and
     * the initial dependent variable vector y. */
    flag = CVodeInit(cvode_mem, f, T0, y);
    if (check_flag(&flag, (char *) "CVodeInit", 1)) return;

    /* Call CVodeSVtolerances to specify the scalar relative tolerance
     * and vector absolute tolerances */
    flag = CVodeSVtolerances(cvode_mem, reltol, abstol);
    if (check_flag(&flag, (char *) "CVodeSVtolerances", 1)) return;

    /* Call CVodeRootInit to specify the root function g with 2 components */
    flag = CVodeRootInit(cvode_mem, 2, g);
    if (check_flag(&flag, (char *) "CVodeRootInit", 1)) return;

    /* Call CVDense to specify the CVDENSE dense linear solver */
    flag = CVDense(cvode_mem, NEQ);
    if (check_flag(&flag, (char *) "CVDense", 1)) return;

    /* Set the Jacobian routine to Jac (user-supplied) */
    flag = CVDlsSetDenseJacFn(cvode_mem, Jac);
    if (check_flag(&flag, (char *) "CVDlsSetDenseJacFn", 1)) return;

    /* In loop, call CVode, print results, and test for error.
       Break out of loop when NOUT preset output times have been reached.  */
    qDebug() << "3-species kinetics problem:";
    qDebug();

    iout = 0;  tout = T1;
    while(1) {
      flag = CVode(cvode_mem, tout, y, &t, CV_NORMAL);
      PrintCvodeOutput(t, Ith(y,1), Ith(y,2), Ith(y,3));

      if (flag == CV_ROOT_RETURN) {
        flagr = CVodeGetRootInfo(cvode_mem, rootsfound);
        if (check_flag(&flagr, (char *) "CVodeGetRootInfo", 1)) return;
        PrintCvodeRootInfo(rootsfound[0],rootsfound[1]);
      }

      if (check_flag(&flag, (char *) "CVode", 1)) break;
      if (flag == CV_SUCCESS) {
        iout++;
        tout *= TMULT;
      }

      if (iout == NOUT) break;
    }

    /* Print some final statistics */
    PrintFinalCvodeStats(cvode_mem);

    /* Free y and abstol vectors */
    N_VDestroy_Serial(y);
    N_VDestroy_Serial(abstol);

    /* Free integrator memory */
    CVodeFree(&cvode_mem);

    qDebug();
}

//==============================================================================

/*
 * Print first lines of output (problem description)
 */

static void PrintHeader(realtype rtol, N_Vector avtol, N_Vector y)
{
    realtype *atval, *yval;

    atval  = NV_DATA_S(avtol);
    yval  = NV_DATA_S(y);

    qDebug("idadenx: Robertson kinetics DAE serial example problem for IDA");
    qDebug("         Three equation chemical kinetics problem.");
    qDebug();
    qDebug("Linear solver: IDADENSE, with user-supplied Jacobian.");
#if defined(SUNDIALS_EXTENDED_PRECISION)
    qDebug("Tolerance parameters:  rtol = %Lg   atol = %Lg %Lg %Lg",
           rtol, atval[0],atval[1],atval[2]);
    qDebug("Initial conditions y0 = (%Lg %Lg %Lg)",
           yval[0], yval[1], yval[2]);
#elif defined(SUNDIALS_DOUBLE_PRECISION)
    qDebug("Tolerance parameters:  rtol = %lg   atol = %lg %lg %lg",
           rtol, atval[0],atval[1],atval[2]);
    qDebug("Initial conditions y0 = (%lg %lg %lg)",
           yval[0], yval[1], yval[2]);
#else
    qDebug("Tolerance parameters:  rtol = %g   atol = %g %g %g",
           rtol, atval[0],atval[1],atval[2]);
    qDebug("Initial conditions y0 = (%g %g %g)",
           yval[0], yval[1], yval[2]);
#endif
    qDebug("Constraints and id not used.");
    qDebug();
    qDebug("-----------------------------------------------------------------------");
    qDebug("  t             y1           y2           y3      | nst  k      h");
    qDebug("-----------------------------------------------------------------------");
}

//==============================================================================

#define ZERO RCONST(0.0);
#define ONE  RCONST(1.0);

//==============================================================================

/*
 * Define the system residual function.
 */

int resrob(realtype tres, N_Vector yy, N_Vector yp, N_Vector rr, void *user_data)
{
    realtype *yval, *ypval, *rval;

    yval = NV_DATA_S(yy);
    ypval = NV_DATA_S(yp);
    rval = NV_DATA_S(rr);

    rval[0]  = RCONST(-0.04)*yval[0] + RCONST(1.0e4)*yval[1]*yval[2];
    rval[1]  = -rval[0] - RCONST(3.0e7)*yval[1]*yval[1] - ypval[1];
    rval[0] -=  ypval[0];
    rval[2]  =  yval[0] + yval[1] + yval[2] - ONE;

    return(0);
}

//==============================================================================

/*
 * Root function routine. Compute functions g_i(t,y) for i = 0,1.
 */

static int grob(realtype t, N_Vector yy, N_Vector yp, realtype *gout,
                void *user_data)
{
    realtype *yval, y1, y3;

    yval = NV_DATA_S(yy);
    y1 = yval[0]; y3 = yval[2];
    gout[0] = y1 - RCONST(0.0001);
    gout[1] = y3 - RCONST(0.01);

    return(0);
}

//==============================================================================

/*
 * Define the Jacobian function.
 */

int jacrob(int Neq, realtype tt,  realtype cj,
           N_Vector yy, N_Vector yp, N_Vector resvec,
           DlsMat JJ, void *user_data,
           N_Vector tempv1, N_Vector tempv2, N_Vector tempv3)
{
    realtype *yval;

    yval = NV_DATA_S(yy);

    IJth(JJ,1,1) = RCONST(-0.04) - cj;
    IJth(JJ,2,1) = RCONST(0.04);
    IJth(JJ,3,1) = ONE;
    IJth(JJ,1,2) = RCONST(1.0e4)*yval[2];
    IJth(JJ,2,2) = RCONST(-1.0e4)*yval[2] - RCONST(6.0e7)*yval[1] - cj;
    IJth(JJ,3,2) = ONE;
    IJth(JJ,1,3) = RCONST(1.0e4)*yval[1];
    IJth(JJ,2,3) = RCONST(-1.0e4)*yval[1];
    IJth(JJ,3,3) = ONE;

    return(0);
}

//==============================================================================

/*
 *--------------------------------------------------------------------
 * Private functions
 *--------------------------------------------------------------------
 */

static void PrintIdaOutput(void *mem, realtype t, N_Vector y)
{
    realtype *yval;
    int retval, kused;
    long int nst;
    realtype hused;

    yval  = NV_DATA_S(y);

    retval = IDAGetLastOrder(mem, &kused);
    check_flag(&retval, (char *) "IDAGetLastOrder", 1);
    retval = IDAGetNumSteps(mem, &nst);
    check_flag(&retval, (char *) "IDAGetNumSteps", 1);
    retval = IDAGetLastStep(mem, &hused);
    check_flag(&retval, (char *) "IDAGetLastStep", 1);
#if defined(SUNDIALS_EXTENDED_PRECISION)
    qDebug("%10.4Le %12.4Le %12.4Le %12.4Le | %3ld  %1d %12.4Le",
           t, yval[0], yval[1], yval[2], nst, kused, hused);
#elif defined(SUNDIALS_DOUBLE_PRECISION)
    qDebug("%10.4le %12.4le %12.4le %12.4le | %3ld  %1d %12.4le",
           t, yval[0], yval[1], yval[2], nst, kused, hused);
#else
    qDebug("%10.4e %12.4e %12.4e %12.4e | %3ld  %1d %12.4e",
           t, yval[0], yval[1], yval[2], nst, kused, hused);
#endif
}

static void PrintIdaRootInfo(int root_f1, int root_f2)
{
    qDebug("    rootsfound[] = %3d %3d", root_f1, root_f2);
    return;
}

//==============================================================================

/*
 * Print final integrator statistics
 */

static void PrintFinalIdaStats(void *mem)
{
    int retval;
    long int nst, nni, nje, nre, nreLS, netf, ncfn, nge;

    retval = IDAGetNumSteps(mem, &nst);
    check_flag(&retval, (char *) "IDAGetNumSteps", 1);
    retval = IDAGetNumResEvals(mem, &nre);
    check_flag(&retval, (char *) "IDAGetNumResEvals", 1);
    retval = IDADlsGetNumJacEvals(mem, &nje);
    check_flag(&retval, (char *) "IDADlsGetNumJacEvals", 1);
    retval = IDAGetNumNonlinSolvIters(mem, &nni);
    check_flag(&retval, (char *) "IDAGetNumNonlinSolvIters", 1);
    retval = IDAGetNumErrTestFails(mem, &netf);
    check_flag(&retval, (char *) "IDAGetNumErrTestFails", 1);
    retval = IDAGetNumNonlinSolvConvFails(mem, &ncfn);
    check_flag(&retval, (char *) "IDAGetNumNonlinSolvConvFails", 1);
    retval = IDADlsGetNumResEvals(mem, &nreLS);
    check_flag(&retval, (char *) "IDADlsGetNumResEvals", 1);
    retval = IDAGetNumGEvals(mem, &nge);
    check_flag(&retval, (char *) "IDAGetNumGEvals", 1);

    qDebug();
    qDebug("Final Run Statistics:");
    qDebug("Number of steps                    = %ld", nst);
    qDebug("Number of residual evaluations     = %ld", nre+nreLS);
    qDebug("Number of Jacobian evaluations     = %ld", nje);
    qDebug("Number of nonlinear iterations     = %ld", nni);
    qDebug("Number of error test failures      = %ld", netf);
    qDebug("Number of nonlinear conv. failures = %ld", ncfn);
    qDebug("Number of root fn. evaluations     = %ld", nge);
}

//==============================================================================

void CoreSimulationPlugin::testIda()
{
    // Testing IDA
    // Note: this is a shameless copying/pasting of the idaRoberts_dns.c code...

    qDebug() << "================================================================================";
    qDebug();
    qDebug() << "Testing IDA";
    qDebug() << "-----------";
    qDebug();

    /* Problem Constants */

    #define NEQ   3
    #define NOUT  12

    /*
     * This simple example problem for IDA, due to Robertson,
     * is from chemical kinetics, and consists of the following three
     * equations:
     *
     *      dy1/dt = -.04*y1 + 1.e4*y2*y3
     *      dy2/dt = .04*y1 - 1.e4*y2*y3 - 3.e7*y2**2
     *         0   = y1 + y2 + y3 - 1
     *
     * on the interval from t = 0.0 to t = 4.e10, with initial
     * conditions: y1 = 1, y2 = y3 = 0.
     *
     * While integrating the system, we also use the rootfinding
     * feature to find the points at which y1 = 1e-4 or at which
     * y3 = 0.01.
     *
     * The problem is solved with IDA using IDADENSE for the linear
     * solver, with a user-supplied Jacobian. Output is printed at
     * t = .4, 4, 40, ..., 4e10.
     */

    void *mem;
    N_Vector yy, yp, avtol;
    realtype rtol, *yval, *ypval, *atval;
    realtype t0, tout1, tout, tret;
    int iout, retval, retvalr;
    int rootsfound[2];

    mem = NULL;
    yy = yp = avtol = NULL;
    yval = ypval = atval = NULL;

    /* Allocate N-vectors. */
    yy = N_VNew_Serial(NEQ);
    if(check_flag((void *)yy, (char *) "N_VNew_Serial", 0)) return;
    yp = N_VNew_Serial(NEQ);
    if(check_flag((void *)yp, (char *) "N_VNew_Serial", 0)) return;
    avtol = N_VNew_Serial(NEQ);
    if(check_flag((void *)avtol, (char *) "N_VNew_Serial", 0)) return;

    /* Create and initialize  y, y', and absolute tolerance vectors. */
    yval  = NV_DATA_S(yy);
    yval[0] = ONE;
    yval[1] = ZERO;
    yval[2] = ZERO;

    ypval = NV_DATA_S(yp);
    ypval[0]  = RCONST(-0.04);
    ypval[1]  = RCONST(0.04);
    ypval[2]  = ZERO;

    rtol = RCONST(1.0e-4);

    atval = NV_DATA_S(avtol);
    atval[0] = RCONST(1.0e-8);
    atval[1] = RCONST(1.0e-14);
    atval[2] = RCONST(1.0e-6);

    /* Integration limits */
    t0 = ZERO;
    tout1 = RCONST(0.4);

    PrintHeader(rtol, avtol, yy);

    /* Call IDACreate and IDAMalloc to initialize IDA memory */
    mem = IDACreate();
    if(check_flag((void *)mem, (char *) "IDACreate", 0)) return;
    retval = IDAInit(mem, resrob, t0, yy, yp);
    if(check_flag(&retval, (char *) "IDAInit", 1)) return;
    retval = IDASVtolerances(mem, rtol, avtol);
    if(check_flag(&retval, (char *) "IDASVtolerances", 1)) return;

    /* Free avtol */
    N_VDestroy_Serial(avtol);

    /* Call IDARootInit to specify the root function grob with 2 components */
    retval = IDARootInit(mem, 2, grob);
    if (check_flag(&retval, (char *) "IDARootInit", 1)) return;

    /* Call IDADense and set up the linear solver. */
    retval = IDADense(mem, NEQ);
    if(check_flag(&retval, (char *) "IDADense", 1)) return;
    retval = IDADlsSetDenseJacFn(mem, jacrob);
    if(check_flag(&retval, (char *) "IDADlsSetDenseJacFn", 1)) return;

    /* In loop, call IDASolve, print results, and test for error.
       Break out of loop when NOUT preset output times have been reached. */

    iout = 0; tout = tout1;
    while(1) {

      retval = IDASolve(mem, tout, &tret, yy, yp, IDA_NORMAL);

      PrintIdaOutput(mem,tret,yy);

      if(check_flag(&retval, (char *) "IDASolve", 1)) return;

      if (retval == IDA_ROOT_RETURN) {
        retvalr = IDAGetRootInfo(mem, rootsfound);
        check_flag(&retvalr, (char *) "IDAGetRootInfo", 1);
        PrintIdaRootInfo(rootsfound[0],rootsfound[1]);
      }

      if (retval == IDA_SUCCESS) {
        iout++;
        tout *= RCONST(10.0);
      }

      if (iout == NOUT) break;
    }

    PrintFinalIdaStats(mem);

    /* Free memory */

    IDAFree(&mem);
    N_VDestroy_Serial(yy);
    N_VDestroy_Serial(yp);

    qDebug();
}

//==============================================================================

void CoreSimulationPlugin::initialize()
{
    // Test a few third-party plugins

    testCellml();
    testLlvmJit();
    testCvode();
    testIda();

    qDebug() << "================================================================================";

    // Create our simulation view widget

    mSimulationView = new QwtPlot(mMainWindow);

    // Hide our simulation view widget since it may not initially be shown in
    // our central widget

    mSimulationView->setVisible(false);

    // Customise our simulation view widget

    mSimulationView->setCanvasBackground(Qt::white);

    // Remove the canvas' border as it otherwise looks odd, not to say ugly,
    // with one

    mSimulationView->setCanvasLineWidth(0);

    // Add a grid to our simulation view widget

    QwtPlotGrid *grid = new QwtPlotGrid;

    grid->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));

    grid->attach(mSimulationView);

    // Read the pre-computed CellML simulation results

    QFile mResultsFile;

    mResultsFile.setFileName(QDir::tempPath()+QDir::separator()+"test_cellml_results.csv");

    mResultsFile.open( QIODevice::ReadOnly);

    QByteArray dataLine;
    bool header = true;

    dataLine = mResultsFile.readLine();

    while (dataLine != QByteArray()) {
        if (header)
            // Ignore the header

            header = false;
        else {
            // Retrieve the time and membrane potential

            QStringList dataLineStringList = QString(dataLine).split(",");

            mXData << dataLineStringList.at(0).toDouble();
            mYData << dataLineStringList.at(1).toDouble();
        }

        // Read the next line

        dataLine = mResultsFile.readLine();
    };

    // Add a curve to our simulation view widget

    QwtPlotCurve *curve = new QwtPlotCurve;

    curve->setRenderHint(QwtPlotItem::RenderAntialiased);
    curve->setPen(QPen(Qt::darkBlue));
    curve->setSamples(mXData, mYData);

    curve->attach(mSimulationView);

    // Set our settings

    mGuiSettings->addView(GuiViewSettings::Simulation, 0);
    // Note: this won't so much set a new view (since the simulation mode should
    //       have only one view), but ensure that the simulation mode becomes
    //       available
}

//==============================================================================

QWidget * CoreSimulationPlugin::viewWidget(const QString &, const int &)
{
    // Always return our simulation view widget, no matter which file name is
    // being passed

    return mSimulationView;
}

//==============================================================================

}   // namespace CoreSimulation
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
