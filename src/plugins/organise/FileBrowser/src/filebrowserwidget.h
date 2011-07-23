#ifndef FILEBROWSERWIDGET_H
#define FILEBROWSERWIDGET_H

#include "treeview.h"

#include <QFileSystemModel>

namespace OpenCOR {
namespace FileBrowser {

class FileBrowserModel : public QFileSystemModel
{
    Q_OBJECT

public:
    explicit FileBrowserModel(QObject *pParent = 0);

    virtual Qt::ItemFlags flags(const QModelIndex &pIndex) const;
};

class FileBrowserWidget : public Core::TreeView
{
    Q_OBJECT

public:
    explicit FileBrowserWidget(const QString &pName, QWidget *pParent = 0);
    ~FileBrowserWidget();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    bool gotoPath(const QString &pPath, const bool &pExpand = false);

    QString homeFolder() const;
    void gotoHomeFolder(const bool &pExpand = false);

    QString currentPath() const;
    QString currentPathDir() const;
    QString currentPathParent() const;

    QString pathOf(const QModelIndex &pIndex) const;

protected:
    virtual QSize sizeHint() const;

    virtual bool viewportEvent(QEvent *pEvent);

    virtual void keyPressEvent(QKeyEvent *pEvent);

    virtual void mousePressEvent(QMouseEvent *pEvent);
    virtual void mouseMoveEvent(QMouseEvent *pEvent);

private:
    FileBrowserModel *mDataModel;

    bool mNeedDefColWidth;

    QStringList mInitPathDirs;
    QString mInitPathDir;
    QString mInitPath;

    void deselectFolders() const;

    QStringList selectedFiles() const;

Q_SIGNALS:
    void beginLoadingSettings();
    void endLoadingSettings();

    void filesOpened(const QStringList &pFileNames);

private Q_SLOTS:
    void directoryLoaded(const QString &pPath);
};

} }

#endif
