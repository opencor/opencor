#ifndef FILEORGANISERWINDOW_H
#define FILEORGANISERWINDOW_H

#include <QDockWidget>

namespace Ui {
    class FileOrganiserWindow;
}

class FileOrganiserWidget;

class FileOrganiserWindow : public QDockWidget
{
    Q_OBJECT

public:
    explicit FileOrganiserWindow(QWidget *pParent = 0);
    ~FileOrganiserWindow();

    void retranslateUi();

    void defaultSettings();

private:
    Ui::FileOrganiserWindow *mUi;

    FileOrganiserWidget *mFileOrganiserWidget;
};

#endif
