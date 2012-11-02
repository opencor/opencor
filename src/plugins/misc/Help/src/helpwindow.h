//==============================================================================
// Help window
//==============================================================================

#ifndef HELPWINDOW_H
#define HELPWINDOW_H

//==============================================================================

#include "dockwidget.h"

//==============================================================================

class QHelpEngine;
class QUrl;

//==============================================================================

namespace Ui {
    class HelpWindow;
}

//==============================================================================

namespace OpenCOR {
namespace Help {

//==============================================================================

class HelpWidget;

//==============================================================================

class HelpWindow : public Core::DockWidget
{
    Q_OBJECT

public:
    explicit HelpWindow(QWidget *pParent = 0);
    ~HelpWindow();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

private:
    Ui::HelpWindow *mGui;

    HelpWidget *mHelpWidget;

    QHelpEngine *mHelpEngine;

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

    void showCustomContextMenu(const QPoint &pPosition) const;
};

//==============================================================================

}   // namespace Help
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
