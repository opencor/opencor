#ifndef PREFERENCESWINDOW_H
#define PREFERENCESWINDOW_H

#include <QDialog>

namespace Ui {
    class PreferencesWindow;
}

namespace OpenCOR {

class PreferencesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesWindow(QWidget *pParent = 0);
    ~PreferencesWindow();

    virtual void retranslateUi();

private:
    Ui::PreferencesWindow *mUi;
};

}

#endif
