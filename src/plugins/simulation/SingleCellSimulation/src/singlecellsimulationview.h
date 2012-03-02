//==============================================================================
// Single cell simulation view
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEW_H
#define SINGLECELLSIMULATIONVIEW_H

//==============================================================================

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
};

//==============================================================================

}   // namespace SingleCellSimulation
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
