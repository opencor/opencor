#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../misc/assistant.h"

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QString osName;

    QString appName;
    QString appVersion;

    QString locale;

    Assistant *assistant;

    void loadSettings();
    void saveSettings();

    void setLocale(const QString& newLocale);

    void notYetImplemented(const QString& message);

public slots:
    void singleAppMsgRcvd(const QString&);

private slots:
    void on_actionAbout_triggered();
    void on_actionHomepage_triggered();
    void on_actionHelp_triggered();
    void on_actionFrench_triggered();
    void on_actionEnglish_triggered();
    void on_actionExit_triggered();
};

#endif // MAINWINDOW_H
