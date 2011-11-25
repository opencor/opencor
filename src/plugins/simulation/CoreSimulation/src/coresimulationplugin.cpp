//==============================================================================
// CoreSimulation plugin
//==============================================================================

#include "coresimulationplugin.h"

//==============================================================================

#include <QDir>
#include <QFile>
#include <QMainWindow>
#include <QPen>

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

void CoreSimulationPlugin::initialize()
{
    // Testing the JIT compilation side of LLVM
    // Note: this is a shameless copying/pasting of the How-to-use JIT
    //       example...

#ifndef Q_WS_WIN
    // Note #1: for some reasons, to call InitializeNativeTarget on Windows
    //          results in an error message being displayed when closing OpenCOR
    //          while not calling InitializeNativeTarget means that OpenCOR
    //          closes 'properly', so...
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

    llvm::outs() << "We just constructed this LLVM module:\n\n" << *module;
    llvm::outs() << "\n\nRunning foo: ";
    llvm::outs().flush();

    // Call fooFunc with no arguments

    std::vector<llvm::GenericValue> noargs;
    llvm::GenericValue genericValue = executionEngine->runFunction(fooFunc,
                                                                   noargs);

    // Output the result of the execution

    llvm::outs() << "Result: " << genericValue.IntVal << "\n";
    llvm::outs().flush();

    // Finish things off

    executionEngine->freeMachineCodeForFunction(fooFunc);

    delete executionEngine;

    llvm::llvm_shutdown();









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
