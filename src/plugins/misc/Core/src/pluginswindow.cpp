#include "pluginswindow.h"

#include "ui_pluginswindow.h"

namespace OpenCOR {
namespace Core {

PluginsWindow::PluginsWindow(QWidget *pParent) :
    QDialog(pParent),
    CommonWidget(pParent),
    mUi(new Ui::PluginsWindow)
{
    // Set up the UI

    mUi->setupUi(this);
}

PluginsWindow::~PluginsWindow()
{
    // Delete the UI

    delete mUi;
}

void PluginsWindow::retranslateUi()
{
    // Translate the whole window

    mUi->retranslateUi(this);
}

} }
