//==============================================================================
// File browser widget
//==============================================================================

#ifndef FILEBROWSERWIDGET_H
#define FILEBROWSERWIDGET_H

//==============================================================================

#include "filebrowsermodel.h"
#include "treeviewwidget.h"

//==============================================================================

namespace OpenCOR {
namespace FileBrowser {

//==============================================================================

class FileBrowserWidget : public Core::TreeViewWidget
{
    Q_OBJECT

public:
    explicit FileBrowserWidget(QWidget *pParent = 0);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    QString currentPath() const;

    void goToHomeFolder();
    void goToParentFolder();

    void goToPreviousFileOrFolder();
    void goToNextFileOrFolder();

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);
    virtual void mouseMoveEvent(QMouseEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);
    virtual bool viewportEvent(QEvent *pEvent);

private:
    FileBrowserModel *mModel;

    bool mNeedDefColWidth;

    QStringList mInitPathDirs;
    QString mInitPathDir;
    QString mInitPath;

    QStringList mPreviousItems;
    QStringList mNextItems;

    void goToPath(const QString &pPath, const bool &pExpand = false);

    QString currentPathParent() const;

    QString pathOf(const QModelIndex &pIndex) const;

    void deselectFolders() const;

    QStringList selectedFiles() const;

    void emitItemChangedRelatedSignals();

    void updateItems(const QString &pItemPath, QStringList &pItems) const;
    void goToOtherItem(QStringList &pItems, QStringList &pOtherItems);

Q_SIGNALS:
    void filesOpened(const QStringList &pFileNames);

    void notHomeFolder(const bool &pNotHomeFolder);
    void goToParentFolderEnabled(const bool &pEnabled);

    void goToPreviousFileOrFolderEnabled(const bool &pEnabled);
    void goToNextFileOrFolderEnabled(const bool &pEnabled);

private Q_SLOTS:
    void itemChanged(const QModelIndex &, const QModelIndex &pPrevItem);

    void directoryLoaded(const QString &pPath);
};

//==============================================================================

}   // namespace FileBrowser
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
