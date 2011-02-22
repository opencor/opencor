#ifndef FILEBROWSERWINDOW_H
#define FILEBROWSERWINDOW_H

#include "commonwidget.h"

#include <QDockWidget>

namespace Ui {
    class FileBrowserWindow;
}

class FileBrowserWidget;

class FileBrowserWindow : public QDockWidget, public CommonWidget
{
    Q_OBJECT

public:
    explicit FileBrowserWindow(QWidget *pParent = 0);
    ~FileBrowserWindow();

    virtual void retranslateUi();

    virtual void loadSettings(const QSettings &pSettings, const QString &);
    virtual void saveSettings(QSettings &pSettings, const QString &);

private:
    Ui::FileBrowserWindow *mUi;

    FileBrowserWidget *mFileBrowserWidget;
};

#endif
