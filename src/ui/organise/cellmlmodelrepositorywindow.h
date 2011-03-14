#ifndef CELLMLMODELREPOSITORYWINDOW_H
#define CELLMLMODELREPOSITORYWINDOW_H

#include "commonwidget.h"
#include "dockwidget.h"

namespace Ui {
    class CellmlModelRepositoryWindow;
}

class CellmlModelRepositoryWidget;

class CellmlModelRepositoryWindow : public DockWidget, public CommonWidget
{
    Q_OBJECT

public:
    explicit CellmlModelRepositoryWindow(QWidget *pParent = 0);
    ~CellmlModelRepositoryWindow();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings &pSettings);
    virtual void saveSettings(QSettings &pSettings);

private:
    Ui::CellmlModelRepositoryWindow *mUi;

    CellmlModelRepositoryWidget *mCellmlModelRepositoryWidget;
};

#endif
