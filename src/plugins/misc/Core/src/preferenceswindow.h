#ifndef PREFERENCESWINDOW_H
#define PREFERENCESWINDOW_H

#include "commonwidget.h"

#include <QDialog>

namespace Ui {
    class PreferencesWindow;
}

namespace OpenCOR {
namespace Core {

class PreferencesWindow : public QDialog, public CommonWidget
{
    Q_OBJECT

public:
    explicit PreferencesWindow(QWidget *pParent = 0);
    ~PreferencesWindow();

    virtual void retranslateUi();

private:
    Ui::PreferencesWindow *mUi;
};

} }

#endif
