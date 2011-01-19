#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class HelpWindow;

class QHelpEngine;
class QSettings;
class QTranslator;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(bool *pRestart, QTranslator *pQtTranslator,
                        QTranslator *pAppTranslator, QWidget *pParent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *pEvent);

private:
    Ui::MainWindow *mUi;

    bool *mRestart;

    QString mLocale;

    QTranslator *mQtTranslator;
    QTranslator *mAppTranslator;

    QHelpEngine *mHelpEngine;
    HelpWindow *mHelpWindow;

    QString mTempDirName;
    QString mQchFileName, mQhcFileName;

    void defaultSettings();

    void loadSettings();
    void saveSettings();

    void setLocale(const QString& pLocale);

    void updateGUI();

public Q_SLOTS:
    void singleAppMsgRcvd(const QString&);

    void resetAll();

private Q_SLOTS:
    void on_actionAbout_triggered();
    void on_actionHomePage_triggered();
    void on_actionFrench_triggered();
    void on_actionEnglish_triggered();
};

#endif // MAINWINDOW_H
