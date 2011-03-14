#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include "dockwidget.h"

namespace Ui {
    class ViewerWindow;
}

class MmlViewerWidget;

class ViewerWindow : public DockWidget
{
    Q_OBJECT

public:
    explicit ViewerWindow(QWidget *pParent = 0);
    ~ViewerWindow();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings &pSettings);
    virtual void saveSettings(QSettings &pSettings);

private:
    Ui::ViewerWindow *mUi;

    MmlViewerWidget *mMmlViewerWidget;
};

#endif
