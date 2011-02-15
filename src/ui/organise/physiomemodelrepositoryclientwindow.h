#ifndef PHYSIOMEMODELREPOSITORYCLIENTWINDOW_H
#define PHYSIOMEMODELREPOSITORYCLIENTWINDOW_H

#include <QDockWidget>

namespace Ui {
    class PhysiomeModelRepositoryClientWindow;
}

class PhysiomeModelRepositoryClientWidget;

class PhysiomeModelRepositoryClientWindow : public QDockWidget
{
    Q_OBJECT

public:
    explicit PhysiomeModelRepositoryClientWindow(QWidget *pParent = 0);
    ~PhysiomeModelRepositoryClientWindow();

    void retranslateUi();

    void defaultSettings();

private:
    Ui::PhysiomeModelRepositoryClientWindow *mUi;

    PhysiomeModelRepositoryClientWidget *mPhysiomeModelRepositoryClientWidget;
};

#endif
