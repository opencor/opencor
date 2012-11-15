//==============================================================================
// Single cell simulation view widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWWIDGET_H
#define SINGLECELLSIMULATIONVIEWWIDGET_H

//==============================================================================

#include "solverinterface.h"
#include "widget.h"

//==============================================================================

//#include <QWidget>

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

class SingleCellSimulationViewContentsWidget;
class SingleCellSimulationViewSimulation;
class SingleCellSimulationViewSimulationInformationWidget;
class SingleCellSimulationViewSimulationWorker;

//==============================================================================

class SingleCellSimulationViewWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewWidget(QWidget *pParent = 0);
    ~SingleCellSimulationViewWidget();

    virtual void retranslateUi();

    void addSolverInterface(SolverInterface *pSolverInterface);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void initialize(const QString &pFileName);

protected:
    virtual void changeEvent(QEvent *pEvent);
    virtual void paintEvent(QPaintEvent *pEvent);

private:
    Ui::SingleCellSimulationViewWidget *mGui;

    bool mCanSaveSettings;

    SolverInterfaces mSolverInterfaces;

    CellMLSupport::CellmlFileRuntime *mCellmlFileRuntime;

    SingleCellSimulationViewSimulation *mSimulation;
    QMap<QString, SingleCellSimulationViewSimulation *> mSimulations;

    QSplitter *mSplitter;

    SingleCellSimulationViewContentsWidget *mContentsWidget;
    QTextEdit *mOutput;

    QProgressBar *mProgressBar;

    void outputStatus(const QString &pStatus);
    void outputStatusError(const QString &pStatusError);

    void setRunPauseMode(const bool &pRunEnabled);
    void setSimulationMode(const bool &pEnabled);

    void clearGraphPanels();
    void clearActiveGraphPanel();

    void setProgressBarStyleSheet();

private Q_SLOTS:
    void on_actionRun_triggered();
    void on_actionPause_triggered();
    void on_actionStop_triggered();

    void on_actionDebugMode_triggered();

    void on_actionAdd_triggered();
    void on_actionRemove_triggered();

    void on_actionCsvExport_triggered();

    void simulationWorkerRunning();
    void simulationWorkerPausing();
    void simulationWorkerStopped();

    void simulationWorkerProgress(const double &pProgress);

    void simulationStatus(const bool &pError, const QString &pStatus);
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
