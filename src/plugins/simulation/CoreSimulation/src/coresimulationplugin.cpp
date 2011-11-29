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

void CoreSimulationPlugin::initialize()
{
    // Test a few third-party plugins

    testCellml();
    testLlvmJit();

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
