#include "helpwindow.h"
#include "helpwidget.h"

#include "ui_helpwindow.h"

#include <QLayout>

HelpWindow::HelpWindow(QHelpEngine *helpEngine, const QUrl& homepage,
                       QDockWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::HelpWindow)
{
    ui->setupUi(this);

    helpWidget = new HelpWidget(helpEngine);

    this->setWidget(helpWidget);

    helpWidget->load(homepage);
}

HelpWindow::~HelpWindow()
{
    delete ui;
}

void HelpWindow::setHelpWidgetTextSizeMultiplier(const double& value)
{
    helpWidget->setTextSizeMultiplier(value);
}

double HelpWindow::helpWidgetTextSizeMultiplier()
{
    return helpWidget->textSizeMultiplier();
}
