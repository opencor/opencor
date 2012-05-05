//==============================================================================
// File browser window
//==============================================================================

#ifndef FILEBROWSERWINDOW_H
#define FILEBROWSERWINDOW_H

//==============================================================================

#include "organisationwidget.h"

//==============================================================================

namespace Ui {
    class FileBrowserWindow;
}

//==============================================================================

class QModelIndex;

//==============================================================================

namespace OpenCOR {
namespace FileBrowser {

//==============================================================================

class FileBrowserWidget;

//==============================================================================

class FileBrowserWindow : public Core::OrganisationWidget
{
    Q_OBJECT

public:
    explicit FileBrowserWindow(QWidget *pParent);
    ~FileBrowserWindow();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

private:
    Ui::FileBrowserWindow *mGui;

    FileBrowserWidget *mFileBrowserWidget;

private Q_SLOTS:
    void on_actionHome_triggered();
    void on_actionParent_triggered();
    void on_actionPrevious_triggered();
    void on_actionNext_triggered();

    void customContextMenu(const QPoint &) const;
    void itemDoubleClicked(const QModelIndex &);
};

//==============================================================================

}   // namespace FileBrowser
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
