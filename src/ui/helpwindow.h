#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDockWidget>

namespace Ui {
    class HelpWindow;
}

class HelpWidget;

class QHelpEngine;
class QUrl;

class HelpWindow : public QDockWidget
{
    Q_OBJECT

public:
    explicit HelpWindow(QHelpEngine *pEngine, const QUrl& pHomepage, QDockWidget *pParent = 0);
    ~HelpWindow();

    void setHomepage(const QUrl& pHomepage);

    void setHelpWidgetTextSizeMultiplier(const double& pHelpWidgetTextSizeMultiplier);
    double helpWidgetTextSizeMultiplier();

private:
    Ui::HelpWindow *mUi;

    HelpWidget *mHelpWidget;
};

#endif // HELPWINDOW_H
