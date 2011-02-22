#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include "commonwidget.h"

#include <QDockWidget>

namespace Ui {
    class ViewerWindow;
}

class MmlViewerWidget;

class ViewerWindow : public QDockWidget, public CommonWidget
{
    Q_OBJECT

public:
    explicit ViewerWindow(QWidget *pParent = 0);
    ~ViewerWindow();

    virtual void retranslateUi();

    virtual void loadSettings(const QSettings &pSettings, const QString &);
    virtual void saveSettings(QSettings &pSettings, const QString &);

private:
    Ui::ViewerWindow *mUi;

    MmlViewerWidget *mMmlViewerWidget;
};

#endif
