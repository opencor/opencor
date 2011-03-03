#ifndef FILEBROWSERWIDGET_H
#define FILEBROWSERWIDGET_H

#include "commonwidget.h"

#include <QTreeView>

class QFileSystemModel;

class FileBrowserWidget : public QTreeView, public CommonWidget
{
    Q_OBJECT

public:
    FileBrowserWidget(QWidget *pParent = 0);
    ~FileBrowserWidget();

    virtual void loadSettings(const QSettings &, const QString &);
    virtual void saveSettings(QSettings &, const QString &);

    bool gotoFolder(const QString &pFolder, const bool &pExpand = false);

    QString homeFolder();
    void gotoHomeFolder(const bool &pExpand = false);

    QString path(const QModelIndex &pIndex);
    QString currentPath();
    bool currentPathVisible();

protected:
    virtual QSize sizeHint() const;

    virtual void mousePressEvent(QMouseEvent *pEvent);

private:
    QFileSystemModel *mFileSystemModel;

    bool mNeedDefaultColumnWidth;

    QString mInitialPathDir;
    QString mInitialPath;

private Q_SLOTS:
    void directoryLoaded(const QString &pPath);
};

#endif
