//==============================================================================
// Single cell view widget
//==============================================================================

#ifndef SINGLECELLVIEWWIDGET_H
#define SINGLECELLVIEWWIDGET_H

//==============================================================================

#include "solverinterface.h"
#include "viewwidget.h"
#include "qwt_series_data.h"

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
    class CellMLFileRuntimeModelParameter;
    class CellMLFileRuntimeCompiledModelParameter;
}   // namespace CellMLSupport

//==============================================================================

namespace SingleCellView {

//==============================================================================

class SingleCellViewContentsWidget;
class SingleCellViewGraphPanelPlotCurve;
class SingleCellViewGraphPanelWidget;
class SingleCellViewPlugin;
class SingleCellViewSimulation;
class SingleCellViewSimulationResults;

//==============================================================================

//==============================================================================

class SingleCellViewWidgetCurveData
{
public:
    explicit SingleCellViewWidgetCurveData(const QString &pFileName,
                                           SingleCellViewSimulation *pSimulation,
                                           QSharedPointer<CellMLSupport::CellMLFileRuntimeModelParameter>
                                             pModelParameter,
                                           SingleCellViewGraphPanelPlotCurve *pCurve);

    QString fileName() const;

    QSharedPointer<CellMLSupport::CellMLFileRuntimeCompiledModelParameter> modelParameter() const;

    SingleCellViewGraphPanelPlotCurve * curve() const;

    bool isAttached() const;
    void setAttached(const bool &pAttached);

    QSharedPointer<CellMLSupport::CellMLFileRuntimeCompiledModelParameter> modelParameterY()
    {
        return mModelParameterY;
    }

private:
    QString mFileName;

    SingleCellViewSimulation *mSimulation;

    QSharedPointer<CellMLSupport::CellMLFileRuntimeCompiledModelParameter> mModelParameterY;

    SingleCellViewGraphPanelPlotCurve *mCurve;

    bool mAttached;
};

class SingleCellViewQwtCurveDataAdaptor
    : public QwtSeriesData<QPointF>
{
public:
    SingleCellViewQwtCurveDataAdaptor(SingleCellViewSimulation* pSimulation,
                                      SingleCellViewWidgetCurveData* pCurveData);

    virtual QRectF boundingRect() const;
    virtual size_t size() const;
    virtual QPointF sample(size_t i) const;

private:
    double sampleBvar(size_t i) const;
    double sampleStateY(size_t i) const;
    double sampleRateY(size_t i) const;
    double sampleAlgebraicY(size_t i) const;
    double sampleConstantY(size_t i) const;

    double mConstantYValue;
    int mSampleYIndex;

    SingleCellViewSimulationResults* mSimulationResults;
    double (SingleCellViewQwtCurveDataAdaptor::* mSampleY)(size_t i) const;
    size_t mSize;
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
        InvalidCellMLFile,
        InvalidSimulationEnvironment
    };

    Ui::SingleCellViewWidget *mGui;

    SingleCellViewPlugin *mPluginParent;

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
                              QSharedPointer<CellMLSupport::CellMLFileRuntimeCompiledModelParameter>
                              pModelParameter);

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

    void simulationPropertyChanged(Core::Property *pProperty, bool pNeedReset = true);
    void solversPropertyChanged(Core::Property *pProperty);

    void showModelParameter(const QString &pFileName,
                            QSharedPointer<CellMLSupport::CellMLFileRuntimeModelParameter> pParameter,
                            const bool &pShow);

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
