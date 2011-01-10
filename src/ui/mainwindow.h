#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class HelpWindow;

class QHelpEngine;
class QSettings;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    QString osName;

    QString appName;
    QString appVersion;

    QString locale;

    QHelpEngine *helpEngine;
    HelpWindow *helpWindow;

    QString tempDirName;
    QString qchFileName, qhcFileName;

    void loadSettings();
    void saveSettings();

    void setLocale(const QString& newLocale);

    void notYetImplemented(const QString& message);

    void updateGUI();

public slots:
    void singleAppMsgRcvd(const QString&);

    void resetAll(const bool& clearUserSettings = true);

private slots:
    void on_actionAbout_triggered();
    void on_actionHomepage_triggered();
    void on_actionFrench_triggered();
    void on_actionEnglish_triggered();
};

#endif // MAINWINDOW_H
