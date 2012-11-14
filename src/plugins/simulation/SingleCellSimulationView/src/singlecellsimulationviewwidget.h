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

class SingleCellSimulationViewContentsWidget;
class SingleCellSimulationViewSimulationInformationWidget;

//==============================================================================

class SingleCellSimulationViewWidgetUserSettings
{
public:
    explicit SingleCellSimulationViewWidgetUserSettings();

    void get(SingleCellSimulationViewSimulationInformationWidget *pSimulationSettings);
    void set(SingleCellSimulationViewSimulationInformationWidget *pSimulationSettings);

private:
    double mStartingPoint;
    double mEndingPoint;
    double mPointInterval;
};

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

    SingleCellSimulationViewWidgetUserSettings *mUserSettings;
    QMap<QString, SingleCellSimulationViewWidgetUserSettings *> mModelUserSettings;

    QSplitter *mSplitter;

    SingleCellSimulationViewContentsWidget *mContentsWidget;
    QTextEdit *mOutput;

    QProgressBar *mProgressBar;

    void outputStatus(const QString &pStatus);
    void outputStatusError(const QString &pStatusError);
    void outputStatusSimulationError(const QString &pStatusSimulationError);

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
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
