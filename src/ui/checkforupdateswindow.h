#ifndef CHECKFORUPDATESWINDOW_H
#define CHECKFORUPDATESWINDOW_H

#include "commonwidget.h"

#include <QDialog>

namespace Ui {
    class CheckForUpdatesWindow;
}

namespace OpenCOR {

class CheckForUpdatesWindow : public QDialog, public CommonWidget
{
    Q_OBJECT

public:
    explicit CheckForUpdatesWindow(QWidget *pParent = 0);
    ~CheckForUpdatesWindow();

    virtual void retranslateUi();

private:
    Ui::CheckForUpdatesWindow *mUi;
};

}

#endif
