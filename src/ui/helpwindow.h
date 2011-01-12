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
    explicit HelpWindow(QHelpEngine *engine, const QUrl& homepage, QDockWidget *parent = 0);
    ~HelpWindow();

    void setHomepage(const QUrl& homepage);

    void setHelpWidgetTextSizeMultiplier(const double& value);
    double helpWidgetTextSizeMultiplier();

private:
    Ui::HelpWindow *ui;

    HelpWidget *helpWidget;
};

#endif // HELPWINDOW_H
