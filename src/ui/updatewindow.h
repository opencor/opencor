#ifndef UPDATEWINDOW_H
#define UPDATEWINDOW_H

#include <QDialog>

namespace Ui {
    class UpdateWindow;
}

class UpdateWindow : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateWindow(QWidget *pParent = 0);
    ~UpdateWindow();

private:
    Ui::UpdateWindow *mUi;
};

#endif
