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
    explicit MainWindow(QWidget *pParent = 0);
    ~MainWindow();

protected:
    virtual void closeEvent(QCloseEvent *pEvent);

private:
    Ui::MainWindow *mUi;

    QString mLocale;

    QHelpEngine *mHelpEngine;
    HelpWindow *mHelpWindow;

    QString mTempDirName;
    QString mQchFileName, mQhcFileName;

    void loadSettings();
    void saveSettings();

    void setLocale(const QString& pLocale);

    void notYetImplemented(const QString& pMsg);

    void updateGUI();

public slots:
    void singleAppMsgRcvd(const QString&);

    void resetAll(const bool& pClearUserSettings = true);

private slots:
    void on_actionAbout_triggered();
    void on_actionHomepage_triggered();
    void on_actionFrench_triggered();
    void on_actionEnglish_triggered();
};

#endif // MAINWINDOW_H
