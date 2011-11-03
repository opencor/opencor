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

    InitializeNativeTarget();

    LLVMContext Context;

    // Create some module to put our function into it.
    Module *M = new Module("test", Context);

    // Create the add1 function entry and insert this entry into module M.  The
    // function will have a return type of "int" and take an argument of "int".
    // The '0' terminates the list of argument types.
    Function *Add1F =
      cast<Function>(M->getOrInsertFunction("add1", Type::getInt32Ty(Context),
                                            Type::getInt32Ty(Context),
                                            (Type *)0));

    // Add a basic block to the function. As before, it automatically inserts
    // because of the last argument.
    BasicBlock *BB = BasicBlock::Create(Context, "EntryBlock", Add1F);

    // Get pointers to the constant `1'.
    Value *One = ConstantInt::get(Type::getInt32Ty(Context), 1);

    // Get pointers to the integer argument of the add1 function...
    assert(Add1F->arg_begin() != Add1F->arg_end()); // Make sure there's an arg
    Argument *ArgX = Add1F->arg_begin();  // Get the arg
    ArgX->setName("AnArg");            // Give it a nice symbolic name for fun.

    // Create the add instruction, inserting it into the end of BB.
    Instruction *Add = BinaryOperator::CreateAdd(One, ArgX, "addresult", BB);

    // Create the return instruction and add it to the basic block
    ReturnInst::Create(Context, Add, BB);

    // Now, function add1 is ready.


    // Now we going to create function `foo', which returns an int and takes no
    // arguments.
    Function *FooF =
      cast<Function>(M->getOrInsertFunction("foo", Type::getInt32Ty(Context),
                                            (Type *)0));

    // Add a basic block to the FooF function.
    BB = BasicBlock::Create(Context, "EntryBlock", FooF);

    // Get pointers to the constant `10'.
    Value *Ten = ConstantInt::get(Type::getInt32Ty(Context), 10);

    // Pass Ten to the call call:
    CallInst *Add1CallRes = CallInst::Create(Add1F, Ten, "add1", BB);
    Add1CallRes->setTailCall(true);

    // Create the return instruction and add it to the basic block.
    ReturnInst::Create(Context, Add1CallRes, BB);

    // Now we create the JIT.
    ExecutionEngine* EE = EngineBuilder(M).create();

    outs() << "We just constructed this LLVM module:\n\n" << *M;
    outs() << "\n\nRunning foo: ";
    outs().flush();

    // Call the `foo' function with no arguments:
    std::vector<GenericValue> noargs;
    GenericValue gv = EE->runFunction(FooF, noargs);

    // Import result of execution:
    outs() << "Result: " << gv.IntVal << "\n";
    EE->freeMachineCodeForFunction(FooF);
    delete EE;
    llvm_shutdown();









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
