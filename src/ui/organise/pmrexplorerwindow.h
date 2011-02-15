#ifndef PMREXPLORERWINDOW_H
#define PMREXPLORERWINDOW_H

#include <QDockWidget>

namespace Ui {
    class PmrExplorerWindow;
}

class PmrExplorerWidget;

class PmrExplorerWindow : public QDockWidget
{
    Q_OBJECT

public:
    explicit PmrExplorerWindow(QWidget *pParent = 0);
    ~PmrExplorerWindow();

    void retranslateUi();

    void defaultSettings();

private:
    Ui::PmrExplorerWindow *mUi;

    PmrExplorerWidget *mPmrExplorerWidget;
};

#endif
