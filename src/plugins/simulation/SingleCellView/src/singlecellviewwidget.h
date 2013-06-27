//==============================================================================
// Single cell view widget
//==============================================================================

#ifndef SINGLECELLVIEWWIDGET_H
#define SINGLECELLVIEWWIDGET_H

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
    class SingleCellViewWidget;
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

namespace SingleCellView {

//==============================================================================

class SingleCellViewContentsWidget;
class SingleCellViewGraphPanelPlotCurve;
class SingleCellViewGraphPanelWidget;
class SingleCellViewPlugin;
class SingleCellViewSimulation;

//==============================================================================

class SingleCellViewWidgetCurveData
{
public:
    explicit SingleCellViewWidgetCurveData(const QString &pFileName,
                                           SingleCellViewSimulation *pSimulation,
                                           CellMLSupport::CellmlFileRuntimeModelParameter *pModelParameter,
                                           SingleCellViewGraphPanelPlotCurve *pCurve);

    QString fileName() const;

    CellMLSupport::CellmlFileRuntimeModelParameter * modelParameter() const;

    SingleCellViewGraphPanelPlotCurve * curve() const;

    double * yData() const;

    bool isAttached() const;
    void setAttached(const bool &pAttached);

private:
    QString mFileName;

    SingleCellViewSimulation *mSimulation;

    CellMLSupport::CellmlFileRuntimeModelParameter *mModelParameter;

    SingleCellViewGraphPanelPlotCurve *mCurve;

    bool mAttached;
};

//==============================================================================

class SingleCellViewWidget : public Core::ViewWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewWidget(SingleCellViewPlugin *pPluginParent,
                                  QWidget *pParent = 0);
    ~SingleCellViewWidget();

    virtual void retranslateUi();

    void setSolverInterfaces(const SolverInterfaces &pSolverInterfaces);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void initialize(const QString &pFileName);
    bool isManaged(const QString &pFileName) const;
    void finalize(const QString &pFileName);

    QIcon fileTabIcon(const QString &pFileName) const;

    SingleCellViewSimulation * simulation() const;

    SingleCellViewContentsWidget * contentsWidget() const;

private:
    enum ErrorType {
        General,
        InvalidCellmlFile,
        InvalidSimulationEnvironment
    };

    Ui::SingleCellViewWidget *mGui;

    SingleCellViewPlugin *mPluginParent;

    SolverInterfaces mSolverInterfaces;

    SingleCellViewSimulation *mSimulation;
    QMap<QString, SingleCellViewSimulation *> mSimulations;

    QList<SingleCellViewSimulation *> mStoppedSimulations;

    Core::ProgressBarWidget *mProgressBarWidget;

    QMap<QString, int> mProgresses;

    QMap<QString, bool> mResets;
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

    SingleCellViewContentsWidget *mContentsWidget;

    bool mRunActionEnabled;

    Core::UserMessageWidget *mInvalidModelMessageWidget;

    QTextEdit *mOutputWidget;

    ErrorType mErrorType;

    SingleCellViewGraphPanelWidget *mActiveGraphPanel;

    QMap<QString, SingleCellViewWidgetCurveData *> mCurvesData;

    QMap<SingleCellViewSimulation *, qulonglong> mOldSimulationResultsSizes;

    QList<SingleCellViewSimulation *> mCheckResultsSimulations;

    void setDelayValue(const int &pDelayValue);

    void output(const QString &pMessage);

    void updateSimulationMode();

    int tabBarIconSize() const;

    void updateRunPauseAction(const bool &pRunActionEnabled);

    void updateInvalidModelMessageWidget();

    void updateResults(SingleCellViewSimulation *pSimulation,
                       const qulonglong &pSize,
                       const bool &pReplot = false);
    void checkResults(SingleCellViewSimulation *pSimulation);

    QString modelParameterKey(const QString pFileName,
                              CellMLSupport::CellmlFileRuntimeModelParameter *pModelParameter);

private Q_SLOTS:
    void on_actionRunPauseResume_triggered();
    void on_actionStop_triggered();

    void on_actionReset_triggered();

    void on_actionDebugMode_triggered();

    void on_actionAdd_triggered();
    void on_actionRemove_triggered();

    void on_actionCsvExport_triggered();

    void updateDelayValue(const double &pDelayValue);

    void simulationRunning(const bool &pIsResuming);
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

/*---GRY---
    void showModelParameter(const QString &pFileName,
                            CellMLSupport::CellmlFileRuntimeModelParameter *pParameter,
                            const bool &pShow);
*/
    void requirePlotting(CellMLSupport::CellmlFileRuntimeModelParameter *pModelParameterX,
                         CellMLSupport::CellmlFileRuntimeModelParameter *pModelParameterY);

    void callCheckResults();
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
