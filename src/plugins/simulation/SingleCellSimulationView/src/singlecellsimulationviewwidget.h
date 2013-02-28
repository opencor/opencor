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
class SingleCellSimulationViewGraphPanelPlotCurve;
class SingleCellSimulationViewGraphPanelWidget;
class SingleCellSimulationViewPlugin;
class SingleCellSimulationViewSimulation;

//==============================================================================

class SingleCellSimulationViewWidgetCurveData
{
public:
    explicit SingleCellSimulationViewWidgetCurveData(const QString &pFileName,
                                                     SingleCellSimulationViewSimulation *pSimulation,
                                                     CellMLSupport::CellmlFileRuntimeModelParameter *pParameter,
                                                     SingleCellSimulationViewGraphPanelPlotCurve *pCurve);

    QString fileName() const;

    SingleCellSimulationViewGraphPanelPlotCurve * curve() const;

    double * yData() const;

    bool isAttached() const;
    void setAttached(const bool &pAttached);

private:
    QString mFileName;

    SingleCellSimulationViewSimulation *mSimulation;

    CellMLSupport::CellmlFileRuntimeModelParameter *mParameter;

    SingleCellSimulationViewGraphPanelPlotCurve *mCurve;

    bool mAttached;
};

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

    struct AxisSettings {
        double minX;
        double maxX;
        double minY;
        double maxY;

        double localMinX;
        double localMaxX;
        double localMinY;
        double localMaxY;
    };

    QMap<QString, AxisSettings> mAxesSettings;

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

    QMap<QString, SingleCellSimulationViewWidgetCurveData *> mCurvesData;

    QMap<SingleCellSimulationViewSimulation *, qulonglong> mOldSimulationResultsSizes;

    QList<SingleCellSimulationViewSimulation *> mCheckResultsSimulations;

    void setDelayValue(const int &pDelayValue);

    void output(const QString &pMessage);

    void updateSimulationMode();

    int tabBarIconSize() const;

    void updateInvalidModelMessageWidget();

    void updateResults(SingleCellSimulationViewSimulation *pSimulation,
                       const qulonglong &pSize,
                       const bool &pReplot = false);
    void checkResults(SingleCellSimulationViewSimulation *pSimulation);

private Q_SLOTS:
    void on_actionRun_triggered();
    void on_actionPause_triggered();
    void on_actionStop_triggered();

    void on_actionReset_triggered();

    void on_actionDebugMode_triggered();

    void on_actionAdd_triggered();
    void on_actionRemove_triggered();

    void on_actionCsvExport_triggered();

    void updateDelayValue(const double &pDelayValue);

    void simulationRunning();
    void simulationPaused();
    void simulationStopped(const int &pElapsedTime);

    void resetProgressBar();
    void resetFileTabIcon();

    void simulationError(const QString &pMessage,
                         const ErrorType &pErrorType = General);

    void simulationDataModified(const bool &pIsModified);

    void splitterWidgetMoved();

    void simulationPropertyChanged(Core::Property *pProperty);
    void solversPropertyChanged(Core::Property *pProperty);

    void showModelParameter(const QString &pFileName,
                            CellMLSupport::CellmlFileRuntimeModelParameter *pParameter,
                            const bool &pShow);

    void callCheckResults();
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
