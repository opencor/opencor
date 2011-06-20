#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QMainWindow>
#include <QTranslator>

class QSettings;

namespace Ui {
    class MainWindow;
}

namespace OpenCOR {

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum ExitCode
    {
        NeedRestart = 1789
    };

    explicit MainWindow(QWidget *pParent = 0);
    ~MainWindow();

protected:
    virtual void changeEvent(QEvent *pEvent);
    virtual void showEvent(QShowEvent *pEvent);
    virtual void closeEvent(QCloseEvent *pEvent);

private:
    Ui::MainWindow *mUi;

    QString mLocale;

    QTranslator mQtTranslator;
    QTranslator mAppTranslator;

    QMenu *mActionReopenMenu;
    QMenu *mActionOpenReopenMenu;

    QDir mActiveDir;

    void loadSettings();
    void saveSettings();

    void setLocale(const QString &pLocale);

public Q_SLOTS:
    void singleAppMsgRcvd(const QString &);

private Q_SLOTS:
    void on_actionFullScreen_triggered();
    void on_actionSystem_triggered();
    void on_actionEnglish_triggered();
    void on_actionFrench_triggered();
    void on_actionHomePage_triggered();
    void on_actionAbout_triggered();

    void resetAll();
};

}

#endif
