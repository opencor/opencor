#ifndef FILEORGANISERWINDOW_H
#define FILEORGANISERWINDOW_H

#include "dockwidget.h"

namespace Ui {
    class FileOrganiserWindow;
}

class FileOrganiserWidget;

class FileOrganiserWindow : public DockWidget
{
    Q_OBJECT

public:
    explicit FileOrganiserWindow(QWidget *pParent = 0);
    ~FileOrganiserWindow();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings &pSettings);
    virtual void saveSettings(QSettings &pSettings);

private:
    Ui::FileOrganiserWindow *mUi;

    FileOrganiserWidget *mFileOrganiserWidget;

private slots:
    void on_actionNew_triggered();
    void on_actionDelete_triggered();
};

#endif
