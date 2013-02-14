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

class QwtPlotCurve;
class QwtWheel;

//==============================================================================

namespace Ui {
    class SingleCellSimulationViewWidget;
}

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class Property;
    class ProgressBarWidget;
    class ToolBarWidget;
    class UserMessageWidget;
}   // namespace Core

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntimeModelParameter;
}   // namespace CellMLSupport

//==============================================================================

namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewContentsWidget;
class SingleCellSimulationViewGraphPanelWidget;
class SingleCellSimulationViewPlugin;
class SingleCellSimulationViewSimulation;
class SingleCellSimulationViewSimulationData;
class SingleCellSimulationViewSimulationResults;

//==============================================================================

class SingleCellSimulationViewWidget : public Core::ViewWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewWidget(SingleCellSimulationViewPlugin *pPluginParent,
                                            QWidget *pParent = 0);
    ~SingleCellSimulationViewWidget();

    virtual void retranslateUi();

    void setSolverInterfaces(const SolverInterfaces &pSolverInterfaces);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void initialize(const QString &pFileName);
    bool isManaged(const QString &pFileName) const;
    void finalize(const QString &pFileName);

    QIcon fileTabIcon(const QString &pFileName) const;

    SingleCellSimulationViewSimulation * simulation() const;

    SingleCellSimulationViewContentsWidget * contentsWidget() const;

private:
    enum ErrorType {
        General,
        InvalidCellmlFile,
        InvalidSimulationEnvironment
    };

    Ui::SingleCellSimulationViewWidget *mGui;

    SingleCellSimulationViewPlugin *mPluginParent;

    SolverInterfaces mSolverInterfaces;

    SingleCellSimulationViewSimulation *mSimulation;
    QMap<QString, SingleCellSimulationViewSimulation *> mSimulations;

    QList<SingleCellSimulationViewSimulation *> mStoppedSimulations;

    QMap<QString, int> mDelays;

    Core::ToolBarWidget *mToolBarWidget;

    QFrame *mTopSeparator;
    QFrame *mBottomSeparator;

    QwtWheel *mDelayWidget;
    QLabel *mDelayValueWidget;

    QSplitter *mSplitterWidget;
    QList<int> mSplitterWidgetSizes;

    SingleCellSimulationViewContentsWidget *mContentsWidget;

    Core::UserMessageWidget *mInvalidModelMessageWidget;

    QTextEdit *mOutputWidget;

    Core::ProgressBarWidget *mProgressBarWidget;

    QMap<QString, int> mProgresses;

    ErrorType mErrorType;

SingleCellSimulationViewGraphPanelWidget *mActiveGraphPanel;
QMap<QString, QwtPlotCurve *> mTraces;
//---GRY--- THE ABOVE IS TEMPORARY, JUST FOR OUR DEMO...

    void setDelayValue(const int &pDelayValue);

    void output(const QString &pMessage);

    void setSimulationMode(const bool &pEnabled, const bool &pRunVisible);

    void clearGraphPanels();
    void clearActiveGraphPanel();

    int tabBarIconSize() const;

    void updateInvalidModelMessageWidget();

QString parameterKey(const QString &pFileName,
                     CellMLSupport::CellmlFileRuntimeModelParameter *pParameter) const;
//---GRY--- THE ABOVE IS TEMPORARY, JUST FOR OUR DEMO...

private Q_SLOTS:
    void on_actionRun_triggered();
    void on_actionPause_triggered();
    void on_actionStop_triggered();

    void on_actionDebugMode_triggered();

    void on_actionAdd_triggered();
    void on_actionRemove_triggered();

    void on_actionCsvExport_triggered();

    void updateDelayValue(const double &pDelayValue);

    void simulationRunning();
    void simulationPausing();
    void simulationStopped(const int &pElapsedTime);

    void resetProgressBar();
    void resetFileTabIcon();

    void simulationProgress(const double &pProgress,
                            SingleCellSimulationViewSimulation *pSimulation = 0);

    void simulationError(const QString &pMessage,
                         const ErrorType &pErrorType = General);

    void splitterWidgetMoved();

    void simulationPropertyChanged(Core::Property *pProperty);
    void solversPropertyChanged(Core::Property *pProperty);

void parameterNeeded(const QString &pFileName,
                     CellMLSupport::CellmlFileRuntimeModelParameter *pParameter,
                     const bool &pNeeded);
void simulationResultsUpdated(SingleCellSimulationViewSimulationResults *pResults);
//---GRY--- THE ABOVE IS TEMPORARY, JUST FOR OUR DEMO...
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
