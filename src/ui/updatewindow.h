#ifndef UPDATEWINDOW_H
#define UPDATEWINDOW_H

#include "commonwidget.h"

#include <QDialog>

namespace Ui {
    class UpdateWindow;
}

namespace OpenCOR {

class UpdateWindow : public QDialog, public CommonWidget
{
    Q_OBJECT

public:
    explicit UpdateWindow(QWidget *pParent = 0);
    ~UpdateWindow();

    virtual void retranslateUi();

private:
    Ui::UpdateWindow *mUi;
};

}

#endif
