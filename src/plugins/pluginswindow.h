#ifndef PLUGINSWINDOW_H
#define PLUGINSWINDOW_H

#include "commonwidget.h"

#include <QDialog>

namespace Ui {
    class PluginsWindow;
}

namespace OpenCOR {

class PluginsWindow : public QDialog, public CommonWidget
{
    Q_OBJECT

public:
    explicit PluginsWindow(QWidget *pParent = 0);
    ~PluginsWindow();

    virtual void retranslateUi();

private:
    Ui::PluginsWindow *mUi;
};

}

#endif
