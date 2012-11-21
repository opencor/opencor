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

    SingleCellSimulationViewSimulation *mSimulation;
    QMap<QString, SingleCellSimulationViewSimulation *> mSimulations;

    QSplitter *mSplitter;

    SingleCellSimulationViewContentsWidget *mContentsWidget;
    QTextEdit *mOutput;

    QProgressBar *mProgressBar;

    void output(const QString &pMessage);

    void setSimulationMode(const bool &pEnabled, const bool &pRunVisible);

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

    void simulationRunning();
    void simulationPausing();
    void simulationStopped(const int &pElapsedTime);

    void simulationProgress(const double &pProgress);

    void outputError(const QString &pError);
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
