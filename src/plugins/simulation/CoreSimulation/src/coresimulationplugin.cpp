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

#include "nanojit.h"

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
                      QStringList() << "Core" << "LLVM" << "Nanojit" << "Qwt",
                      descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(CoreSimulation, CoreSimulationPlugin)

//==============================================================================

void CoreSimulationPlugin::initialize()
{
    // Testing Nanojit

//    using namespace nanojit;

//    const uint32_t CACHE_SIZE_LOG2 = 20;

//    static avmplus::GC gc = avmplus::GC();
//    static avmplus::AvmCore core = avmplus::AvmCore();

//    LogControl lc;
//#ifdef DEBUG
//    lc.lcbits = LC_ReadLIR | LC_Assembly;
//#else
//    lc.lcbits = 0;
//#endif

//    // Set up the basic Nanojit objects.
//    Allocator *alloc = new VMAllocator();
//    CodeAlloc *codeAlloc = new CodeAlloc();
//    Assembler *assm = new (&gc) Assembler(*codeAlloc, *alloc, &core, &lc);
//    Fragmento *fragmento =
//        new (&gc) Fragmento(&core, &lc, CACHE_SIZE_LOG2, codeAlloc);
//    LirBuffer *buf = new (*alloc) LirBuffer(*alloc);

//    #ifdef DEBUG
//    fragmento->labels = new (*alloc) LabelMap(*alloc, &lc);
//    buf->names = new (*alloc) LirNameMap(*alloc, fragmento->labels);
//    #endif

//    // Create a Fragment to hold some native code.
//    Fragment *f = fragmento->getAnchor((void *)0xdeadbeef);
//    f->lirbuf = buf;
//    f->root = f;

//    // Create a LIR writer
//    LirBufWriter out(buf);

//    // Write a few LIR instructions to the buffer: add the first parameter
//    // to the constant 2.
//    out.ins0(LIR_start);
//    LIns *two = out.insImm(2);
//    LIns *firstParam = out.insParam(0, 0);
//    LIns *result = out.ins2(LIR_add, firstParam, two);
//    out.ins1(LIR_ret, result);

//    // Emit a LIR_loop instruction.  It won't be reached, but there's
//    // an assertion in Nanojit that trips if a fragment doesn't end with
//    // a guard (a bug in Nanojit).
//    LIns *rec_ins = out.insSkip(sizeof(GuardRecord) + sizeof(SideExit));
//    GuardRecord *guard = (GuardRecord *) rec_ins->payload();
//    memset(guard, 0, sizeof(*guard));
//    SideExit *exit = (SideExit *)(guard + 1);
//    guard->exit = exit;
//    guard->exit->target = f;
//    f->lastIns = out.insGuard(LIR_loop, out.insImm(1), rec_ins);

//    // Compile the fragment.
//    compile(assm, f, *alloc verbose_only(, fragmento->labels));
//    if (assm->error() != None) {
//        fprintf(stderr, "error compiling fragment\n");
//        return 1;
//    }
//    printf("Compilation successful.\n");

//    // Call the compiled function.
//    typedef JS_FASTCALL int32_t (*AddTwoFn)(int32_t);
//    AddTwoFn fn = reinterpret_cast<AddTwoFn>(f->code());
//    printf("2 + 5 = %d\n", fn(5));









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
