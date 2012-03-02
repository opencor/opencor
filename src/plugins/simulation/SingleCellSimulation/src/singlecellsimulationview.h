//==============================================================================
// Single cell simulation view
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEW_H
#define SINGLECELLSIMULATIONVIEW_H

//==============================================================================

#include <QWidget>

//==============================================================================

class QFrame;
class QProgressBar;
class QSplitter;
class QTextEdit;

//==============================================================================

namespace Ui {
    class SingleCellSimulationView;
}

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulation {

//==============================================================================

class SingleCellSimulationGraphPanel;
class SingleCellSimulationGraphPanels;

//==============================================================================

class SingleCellSimulationView : public QWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationView(QWidget *pParent = 0);
    ~SingleCellSimulationView();

    virtual void retranslateUi();

    void updateWith(const QString &pFileName);

private:
    Ui::SingleCellSimulationView *mUi;

    SingleCellSimulationGraphPanels *mGraphPanels;
    QTextEdit *mOutput;

    QProgressBar *mProgressBar;

    QFrame * newSeparatingLine();

private Q_SLOTS:
    void on_actionAdd_triggered();
    void on_actionRemove_triggered();

    void updateGui();
};

//==============================================================================

}   // namespace SingleCellSimulation
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
