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
    explicit FileBrowserWindow(QWidget *pParent = 0);
    ~FileBrowserWindow();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

protected:
    virtual void updateActions();

private:
    Ui::FileBrowserWindow *mUi;

    FileBrowserWidget *mFileBrowserWidget;

    QStringList mPrevItems;
    QStringList mNextItems;

    void gotoOtherItem(QStringList &pItems, QStringList &pOtherItems);
    void updateItems(const QString &pItemPath, QStringList &pItems) const;

private Q_SLOTS:
    void on_actionHome_triggered();
    void on_actionParent_triggered();
    void on_actionPrevious_triggered();
    void on_actionNext_triggered();

    void beginLoadingSettings() const;
    void endLoadingSettings() const;
    void customContextMenu(const QPoint &) const;
    void itemDoubleClicked(const QModelIndex &itemIndex);

    void itemChanged(const QModelIndex &, const QModelIndex &pPrevItem);
};

//==============================================================================

}   // namespace FileBrowser
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
