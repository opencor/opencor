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
    // Note: this is shameless copying/pasting of the How-to-use JIT example...

    llvm::InitializeNativeTarget();

    llvm::LLVMContext Context;

    // Create some module to put our function into it.
    llvm::Module *M = new llvm::Module("test", Context);

    // Create the add1 function entry and insert this entry into module M.  The
    // function will have a return type of "int" and take an argument of "int".
    // The '0' terminates the list of argument types.
    llvm::Function *Add1F =
      llvm::cast<llvm::Function>(M->getOrInsertFunction("add1", llvm::Type::getInt32Ty(Context),
                                            llvm::Type::getInt32Ty(Context),
                                            (llvm::Type *)0));

    // Add a basic block to the function. As before, it automatically inserts
    // because of the last argument.
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(Context, "EntryBlock", Add1F);

    // Get pointers to the constant `1'.
    llvm::Value *One = llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 1);

    // Get pointers to the integer argument of the add1 function...
    assert(Add1F->arg_begin() != Add1F->arg_end()); // Make sure there's an arg
    llvm::Argument *ArgX = Add1F->arg_begin();  // Get the arg
    ArgX->setName("AnArg");            // Give it a nice symbolic name for fun.

    // Create the add instruction, inserting it into the end of BB.
    llvm::Instruction *Add = llvm::BinaryOperator::CreateAdd(One, ArgX, "addresult", BB);

    // Create the return instruction and add it to the basic block
    llvm::ReturnInst::Create(Context, Add, BB);

    // Now, function add1 is ready.


    // Now we going to create function `foo', which returns an int and takes no
    // arguments.
    llvm::Function *FooF =
      llvm::cast<llvm::Function>(M->getOrInsertFunction("foo", llvm::Type::getInt32Ty(Context),
                                            (llvm::Type *)0));

    // Add a basic block to the FooF function.
    BB = llvm::BasicBlock::Create(Context, "EntryBlock", FooF);

    // Get pointers to the constant `10'.
    llvm::Value *Ten = llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 10);

    // Pass Ten to the call call:
    llvm::CallInst *Add1CallRes = llvm::CallInst::Create(Add1F, Ten, "add1", BB);
    Add1CallRes->setTailCall(true);

    // Create the return instruction and add it to the basic block.
    llvm::ReturnInst::Create(Context, Add1CallRes, BB);

    // Now we create the JIT.
    llvm::ExecutionEngine* EE = llvm::EngineBuilder(M).create();

    llvm::outs() << "We just constructed this LLVM module:\n\n" << *M;
    llvm::outs() << "\n\nRunning foo: ";
    llvm::outs().flush();

    // Call the `foo' function with no arguments:
    std::vector<llvm::GenericValue> noargs;
    llvm::GenericValue gv = EE->runFunction(FooF, noargs);

    // Import result of execution:
    llvm::outs() << "Result: " << gv.IntVal << "\n";
    EE->freeMachineCodeForFunction(FooF);
    delete EE;
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
