#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include "dockwidget.h"

namespace Ui {
    class ViewerWindow;
}

namespace OpenCOR {
namespace Viewer {

class MmlViewerWidget;

class ViewerWindow : public Core::DockWidget
{
    Q_OBJECT

public:
    explicit ViewerWindow(QWidget *pParent = 0);
    ~ViewerWindow();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

private:
    Ui::ViewerWindow *mUi;

    MmlViewerWidget *mMmlViewerWidget;
};

} }

#endif
