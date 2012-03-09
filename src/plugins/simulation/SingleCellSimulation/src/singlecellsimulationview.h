//==============================================================================
// Single cell simulation view
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEW_H
#define SINGLECELLSIMULATIONVIEW_H

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
    class SingleCellSimulationView;
}

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {

//==============================================================================

class CellmlFileRuntime;

//==============================================================================

}   // namespace CellMLSuppoer

//==============================================================================

namespace SingleCellSimulation {

//==============================================================================

class SingleCellSimulationGraphPanel;
class SingleCellSimulationGraphPanels;

//==============================================================================

class SingleCellSimulationView : public Core::Widget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationView(QWidget *pParent = 0);
    ~SingleCellSimulationView();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void addSolverInterface(SolverInterface *pSolverInterface);

    void initialize(const QString &pFileName);

private:
    Ui::SingleCellSimulationView *mUi;

    QString mFileName;
    CellMLSupport::CellmlFileRuntime *mCellmlFileRuntime;

    SolverInterfaces mSolverInterfaces;

    SingleCellSimulationGraphPanels *mGraphPanels;
    QTextEdit *mOutput;

    QProgressBar *mProgressBar;

    QString mSolverErrorMsg;

    QFrame * newSeparatingLine();

    void clearGraphPanels();
    void clearActiveGraphPanel();

private Q_SLOTS:
    void on_actionRun_triggered();
    void on_actionAdd_triggered();
    void on_actionRemove_triggered();

    void solverError(const QString &pErrorMsg);
};

//==============================================================================

}   // namespace SingleCellSimulation
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
