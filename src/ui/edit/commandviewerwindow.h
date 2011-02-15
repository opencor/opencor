#ifndef COMMANDVIEWERWINDOW_H
#define COMMANDVIEWERWINDOW_H

#include "commonwidget.h"

#include <QDockWidget>

namespace Ui {
    class CommandViewerWindow;
}

class CommandViewerWidget;

class CommandViewerWindow : public QDockWidget, public CommonWidget
{
    Q_OBJECT

public:
    explicit CommandViewerWindow(QWidget *pParent = 0);
    ~CommandViewerWindow();

    virtual void retranslateUi();

    virtual void defaultSettings();

    virtual void loadSettings(const QSettings &pSettings, const QString &);
    virtual void saveSettings(QSettings &pSettings, const QString &);

private:
    Ui::CommandViewerWindow *mUi;

    CommandViewerWidget *mCommandViewerWidget;
};

#endif
