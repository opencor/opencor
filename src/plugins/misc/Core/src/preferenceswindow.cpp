#include "preferenceswindow.h"

#include "ui_preferenceswindow.h"

namespace OpenCOR {
namespace Core {

PreferencesWindow::PreferencesWindow(QWidget *pParent) :
    QDialog(pParent),
    CommonWidget(pParent),
    mUi(new Ui::PreferencesWindow)
{
    // Set up the UI

    mUi->setupUi(this);
}

PreferencesWindow::~PreferencesWindow()
{
    // Delete the UI

    delete mUi;
}

void PreferencesWindow::retranslateUi()
{
    // Translate the whole window

    mUi->retranslateUi(this);
}

} }
