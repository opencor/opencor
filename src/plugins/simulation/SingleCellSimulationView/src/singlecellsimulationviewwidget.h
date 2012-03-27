//==============================================================================
// Single cell simulation view widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWWIDGET_H
#define SINGLECELLSIMULATIONVIEWWIDGET_H

//==============================================================================

#include "solverinterface.h"
#include "widget.h"

//==============================================================================

#include <QWidget>

//==============================================================================

class QFrame;
class QProgressBar;
class QSettings;
class QSplitter;
class QTextEdit;

//==============================================================================

namespace Ui {
    class SingleCellSimulationViewWidget;
}

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntime;
}   // namespace CellMLSuppoer

//==============================================================================

namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewGraphPanel;
class SingleCellSimulationViewGraphPanels;

//==============================================================================

class SingleCellSimulationViewWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewWidget(QWidget *pParent);
    ~SingleCellSimulationViewWidget();

    virtual void retranslateUi();

    void addSolverInterface(SolverInterface *pSolverInterface);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void initialize(const QString &pFileName);

private:
    Ui::SingleCellSimulationViewWidget *mUi;

    CellMLSupport::CellmlFileRuntime *mCellmlFileRuntime;

    enum {
        Unknown,
        VanDerPol1928,
        Hodgkin1952,
        Noble1962,
        Noble1984,
        LuoRudy1991,
        Noble1991,
        Noble1998,
        Zhang2000,
        Mitchell2003,
        Dae
    } mModel;

    int mStatesCount;
    int mCondVarCount;

    double *mConstants;
    double *mRates;
    double *mStates;
    double *mAlgebraic;
    double *mCondVar;

    double mVoiEnd;
    double mVoiStep;
    double mVoiMaximumStep;
    double mVoiOutput;

    QString mOdeSolverName;

    bool mSlowPlotting;

    SolverInterfaces mSolverInterfaces;

    SingleCellSimulationViewGraphPanels *mGraphPanels;
    QTextEdit *mOutput;

    QProgressBar *mProgressBar;

    QString mSolverErrorMsg;

    void clearGraphPanels();
    void clearActiveGraphPanel();

    void outputSolverErrorMsg();

private Q_SLOTS:
    void on_actionRun_triggered();
    void on_actionDebugMode_triggered();
    void on_actionAdd_triggered();
    void on_actionRemove_triggered();

    void solverError(const QString &pErrorMsg);
    void on_actionCsvExport_triggered();
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
