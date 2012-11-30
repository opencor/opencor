//==============================================================================
// Single cell simulation view widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWWIDGET_H
#define SINGLECELLSIMULATIONVIEWWIDGET_H

//==============================================================================

#include "solverinterface.h"
#include "viewwidget.h"

//==============================================================================

class QFrame;
class QLabel;
class QProgressBar;
class QScrollBar;
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

namespace Core {
    class ProgressBarWidget;
    class ToolBarWidget;
    class UserMessageWidget;
}   // namespace Core

//==============================================================================

namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewContentsWidget;
class SingleCellSimulationViewPlugin;
class SingleCellSimulationViewSimulation;

//==============================================================================

class SingleCellSimulationViewWidget : public Core::ViewWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewWidget(SingleCellSimulationViewPlugin *pPluginParent,
                                            QWidget *pParent = 0);
    ~SingleCellSimulationViewWidget();

    virtual void retranslateUi();

    void addSolverInterface(SolverInterface *pSolverInterface);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void initialize(const QString &pFileName);

    QIcon fileTabIcon(const QString &pFileName) const;

    SingleCellSimulationViewSimulation * simulation() const;

    QScrollBar *delayWidget() const;

    SingleCellSimulationViewContentsWidget * contentsWidget() const;

protected:
    virtual void paintEvent(QPaintEvent *pEvent);

private:
    Ui::SingleCellSimulationViewWidget *mGui;

    SingleCellSimulationViewPlugin *mPluginParent;

    bool mCanSaveSettings;

    SolverInterfaces mSolverInterfaces;

    SingleCellSimulationViewSimulation *mSimulation;
    QMap<QString, SingleCellSimulationViewSimulation *> mSimulations;

    QList<SingleCellSimulationViewSimulation *> mStoppedSimulations;

    Core::ToolBarWidget *mToolBarWidget;

    QScrollBar *mDelayWidget;
    QLabel *mDelayValueWidget;

    QSplitter *mSplitterWidget;
    QList<int> mSplitterWidgetSizes;

    SingleCellSimulationViewContentsWidget *mContentsWidget;

    Core::UserMessageWidget *mInvalidModelMessageWidget;

    QTextEdit *mOutputWidget;

    Core::ProgressBarWidget *mProgressBarWidget;

    QMap<QString, int> mProgresses;

    void output(const QString &pMessage);

    void setSimulationMode(const bool &pEnabled, const bool &pRunVisible);

    void clearGraphPanels();
    void clearActiveGraphPanel();

    int tabBarIconSize() const;

private Q_SLOTS:
    void on_actionRun_triggered();
    void on_actionPause_triggered();
    void on_actionStop_triggered();

    void on_actionDebugMode_triggered();

    void on_actionAdd_triggered();
    void on_actionRemove_triggered();

    void on_actionCsvExport_triggered();

    void updateDelayValue(const int &pDelayValue);

    void simulationRunning();
    void simulationPausing();
    void simulationStopped(const int &pElapsedTime);

    void resetProgressBar();
    void resetFileTabIcon();

    void simulationProgress(const double &pProgress,
                            SingleCellSimulationViewSimulation *pSimulation = 0);

    void simulationError(const QString &pError);

    void splitterWidgetMoved();
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
