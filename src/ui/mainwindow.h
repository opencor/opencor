#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>

namespace Ui {
    class MainWindow;
}

class HelpWindow;

class QHelpEngine;
class QSettings;

class QxtTemporaryDir;

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

    QTranslator mQtTranslator;
    QTranslator mAppTranslator;

    QHelpEngine *mHelpEngine;
    HelpWindow *mHelpWindow;

    QxtTemporaryDir *mTempDir;
    QString mQchFileName, mQhcFileName;

    void defaultSettings();

    void loadSettings();
    void saveSettings();

    void setLocale(const QString& pLocale);

    void updateGUI();

public Q_SLOTS:
    void singleAppMsgRcvd(const QString&);

private Q_SLOTS:
    void on_actionEnglish_triggered();
    void on_actionFrench_triggered();
    void on_actionHomePage_triggered();
    void on_actionAbout_triggered();

    void resetAll();
};

#endif
