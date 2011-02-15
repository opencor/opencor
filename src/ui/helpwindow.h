#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include "commonwidget.h"

#include <QDockWidget>

namespace Ui {
    class HelpWindow;
}

class HelpWidget;

class QHelpEngine;
class QUrl;

class HelpWindow : public QDockWidget, public CommonWidget
{
    Q_OBJECT

public:
    explicit HelpWindow(QHelpEngine *pEngine, const QUrl &pHomePage,
                        QWidget *pParent = 0);
    ~HelpWindow();

    virtual void retranslateUi();

    virtual void defaultSettings();

    virtual void loadSettings(const QSettings &pSettings, const QString &);
    virtual void saveSettings(QSettings &pSettings, const QString &);

    void gotoHomePage();

    int defaultZoomLevel();

    void setZoomLevel(const int &pZoomLevel);
    int zoomLevel();

private:
    Ui::HelpWindow *mUi;

    HelpWidget *mHelpWidget;

private Q_SLOTS:
    void on_actionHome_triggered();
    void on_actionBack_triggered();
    void on_actionForward_triggered();
    void on_actionCopy_triggered();
    void on_actionNormalSize_triggered();
    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();
    void on_actionPrint_triggered();

    void customContextMenu(const QPoint &pPos);

    void newUrl(const QUrl &pNewUrl);
    void newSelection();
    void newZoomLevel(const int &pNewZoomLevel);
};

#endif
