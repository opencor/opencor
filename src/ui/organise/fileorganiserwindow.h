#ifndef FILEORGANISERWINDOW_H
#define FILEORGANISERWINDOW_H

#include "commonwidget.h"
#include "dockwidget.h"

namespace Ui {
    class FileOrganiserWindow;
}

class FileOrganiserWidget;

class FileOrganiserWindow : public DockWidget, public CommonWidget
{
    Q_OBJECT

public:
    explicit FileOrganiserWindow(QWidget *pParent = 0);
    ~FileOrganiserWindow();

    virtual void retranslateUi();

    virtual void loadSettings(const QSettings &pSettings, const QString &);
    virtual void saveSettings(QSettings &pSettings, const QString &);

private:
    Ui::FileOrganiserWindow *mUi;

    FileOrganiserWidget *mFileOrganiserWidget;
};

#endif
