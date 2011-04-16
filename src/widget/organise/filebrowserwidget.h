#ifndef FILEBROWSERWIDGET_H
#define FILEBROWSERWIDGET_H

#include "treeview.h"

#include <QFileSystemModel>

class FileBrowserModel : public QFileSystemModel
{
    Q_OBJECT

public:
    explicit FileBrowserModel(QObject *pParent = 0);

    virtual Qt::ItemFlags flags(const QModelIndex &pIndex) const;
};

class FileBrowserWidget : public TreeView
{
    Q_OBJECT

public:
    explicit FileBrowserWidget(const QString &pName, QWidget *pParent);
    ~FileBrowserWidget();

    virtual void loadSettings(QSettings &pSettings);
    virtual void saveSettings(QSettings &pSettings);

    bool gotoPath(const QString &pPath, const bool &pExpand = false);

    QString homeFolder();
    void gotoHomeFolder(const bool &pExpand = false);

    QString currentPath();
    QString currentPathDir();
    QString currentPathParent();

    QString pathOf(const QModelIndex &pIndex);

protected:
    virtual QSize sizeHint() const;

    virtual bool viewportEvent(QEvent *pEvent);

    virtual void keyPressEvent(QKeyEvent *pEvent);

private:
    FileBrowserModel *mDataModel;

    bool mNeedDefColWidth;

    QStringList mInitPathDirs;
    QString mInitPathDir;
    QString mInitPath;

    QStringList selectedFiles();

Q_SIGNALS:
    void beginLoadingSettings();
    void endLoadingSettings();

    void filesOpened(const QStringList &pFileNames);

private Q_SLOTS:
    void directoryLoaded(const QString &pPath);
};

#endif
