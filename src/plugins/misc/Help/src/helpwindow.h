#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include "dockwidget.h"

class QHelpEngine;
class QUrl;

class QxtTemporaryDir;

namespace Ui {
    class HelpWindow;
}

namespace OpenCOR {

class HelpWidget;

class HelpWindow : public DockWidget
{
    Q_OBJECT

public:
    explicit HelpWindow(QWidget *pParent = 0);
    ~HelpWindow();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings &pSettings);
    virtual void saveSettings(QSettings &pSettings);

    void gotoHomePage();

    int defaultZoomLevel();

    void setZoomLevel(const int &pZoomLevel);
    int zoomLevel();

protected:
    virtual void updateActions();

private:
    Ui::HelpWindow *mUi;

    HelpWidget *mHelpWidget;

    QHelpEngine *mHelpEngine;

    QxtTemporaryDir *mTempDir;
    QString mQchFileName, mQhcFileName;

private Q_SLOTS:
    void on_actionHome_triggered();
    void on_actionBack_triggered();
    void on_actionForward_triggered();
    void on_actionCopy_triggered();
    void on_actionNormalSize_triggered();
    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();
    void on_actionPrint_triggered();

    void customContextMenu(const QPoint &);
    void needUpdateActions();
};

}

#endif
