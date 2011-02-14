#ifndef FILEBROWSERWINDOW_H
#define FILEBROWSERWINDOW_H

#include <QDockWidget>

namespace Ui {
    class FileBrowserWindow;
}

class FileBrowserWidget;

class FileBrowserWindow : public QDockWidget
{
    Q_OBJECT

public:
    explicit FileBrowserWindow(QWidget *pParent = 0);
    ~FileBrowserWindow();

    void retranslateUi();

    void defaultSettings();

private:
    Ui::FileBrowserWindow *mUi;

    FileBrowserWidget *mFileBrowserWidget;
};

#endif
